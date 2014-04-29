/* mehPL:
 *    This is Open Source, but NOT GPL. I call it mehPL.
 *    I'm not too fond of long licenses at the top of the file.
 *    Please see the bottom.
 *    Enjoy!
 */





// This file contains tools for packing an icon/sprite into the least
// memory possible, while allowing the image-data itself to be somewhat
// viewable in the sprites' header files.
// Each sprite contains four color-indices (two bits per pixel)
// These four color-indices are converted into actual displayable colors
// via the sprite's color palette(s).
// a/o v66: * The sprite's mask is completely unused, as I recall.
//          * The color-indices were arbitrary until recently
//            Now some attempt has been made to use certain indices for
//            certain purposes (e.g. index=0 -> Sky)

#ifndef __ICONPACKING_H__
#define __ICONPACKING_H__

#include "gPixelVal.h"

#define ICON_WIDTH	16
#define ICON_HEIGHT	16
#define ICON_PIXELS	((ICON_WIDTH)*(ICON_HEIGHT))

#define ICON_PACKED_BYTES	((ICON_PIXELS)/4)
#define ICON_MASK_BYTES		((ICON_PIXELS)/8)

#define PACKED_BYTES_PER_ROW	(ICON_WIDTH/4)

#define PIXELS_PER_PACKAGE	4

#define PACKED_BITS_PER_PIXEL	(8/PIXELS_PER_PACKAGE)

typedef struct _BLAH_SHROOM_
{
	const uint8_t * p_image;
	const uint8_t * p_mask;
	const uint8_t * p_palette;
	const uint8_t numPalettes;
}	sprite_t;

/*
#define getGimpColorVal(sprite, row, col) \
		(((pgm_read_byte((uint8_t *)(&((sprite).p_image[ \
					(row)*PACKED_BYTES_PER_ROW + (col)/PIXELS_PER_PACKAGE])))\
		  )>>((col)%PIXELS_PER_PACKAGE)*(PACKED_BITS_PER_PIXEL))&0x03)
*/

/*
#define getGimpColorVal(sprite, palette, row, col) \
	pgm_read_byte((uint8_t *)(&((sprite).p_palette[ (palette)*4 + \
		(((pgm_read_byte((uint8_t *)(&((sprite).p_image[ \
					(row)*PACKED_BYTES_PER_ROW + (col)/PIXELS_PER_PACKAGE])))\
		  )>>((col)%PIXELS_PER_PACKAGE)*(PACKED_BITS_PER_PIXEL))&0x03) ])))
*/


sprite_t *skyOverrideSprite = NULL;
//uint8_t skyOverridePalette;

void setSpriteSkyColorOverride(sprite_t *sprite) //, uint8_t palette)
{
	skyOverrideSprite = sprite;
//	skyOverridePalette = palette;
}


uint8_t getRawPixelVal(sprite_t *sprite, uint8_t row, uint8_t col)
{

	return
		(((pgm_read_byte((uint8_t *)(&(sprite->p_image[ \
					(row)*PACKED_BYTES_PER_ROW + (col)/PIXELS_PER_PACKAGE])))\
		  )>>((col)%PIXELS_PER_PACKAGE)*(PACKED_BITS_PER_PIXEL))&0x03);
}


uint8_t rawPixValToGimpColorVal(uint8_t rawPixelVal, sprite_t *sprite, 
																			uint8_t palette)
{
	return 
	pgm_read_byte(
			(uint8_t *)(&(sprite->p_palette[ (palette)*4 + rawPixelVal ])));
}


//This doesn't exactly work as expected... see drawSpriteRow for a better
//implementation
uint8_t getGimpColorVal(sprite_t *sprite, uint8_t palette, uint8_t row, 
																			 	uint8_t col)
{
	//No shit, this entire function was a single "line" of code, at one
	//point... (broken up a/o v63)

	//This is the value as-seen in the icon's .h file... (0-3)
	uint8_t rawPixelVal = getRawPixelVal(sprite, row, col);
//		(((pgm_read_byte((uint8_t *)(&(sprite->p_image[ 
//					(row)*PACKED_BYTES_PER_ROW + (col)/PIXELS_PER_PACKAGE])))
//		  )>>((col)%PIXELS_PER_PACKAGE)*(PACKED_BITS_PER_PIXEL))&0x03);


	//The sky is always 0, as of recently (v63)
	// That's not to say the same color isn't used for other purposes
	// this isn't that smart...
	if((rawPixelVal == 0) && (skyOverrideSprite != NULL))
	{
		sprite = skyOverrideSprite;
		//palette = skyOverridePalette;
	}

	return rawPixValToGimpColorVal(rawPixelVal, sprite, palette); 
//	pgm_read_byte(
//			(uint8_t *)(&(sprite->p_palette[ (palette)*4 + rawPixelVal ])));
}



//Number of pixels that can be packed into a single byte
#define ROWPACKRATE	4
//Number of pixels that can be packed into a single MASK byte
#define MPACKRATE		8


//This appears to work in array intializations
// (Should probably be moved to bithandling)
// Maybe there's a simpler way, but this is pretty clear.
#define ISTRUE(a) (((a)!=0)?1:0)


#define ROWPACK(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p) \
	((((a)&0x03)) | (((b)&0x03)<<2) | (((c)&0x03)<<4) | (((d)&0x03)<<6)), \
	((((e)&0x03)) | (((f)&0x03)<<2) | (((g)&0x03)<<4) | (((h)&0x03)<<6)), \
	((((i)&0x03)) | (((j)&0x03)<<2) | (((k)&0x03)<<4) | (((l)&0x03)<<6)), \
	((((m)&0x03)) | (((n)&0x03)<<2) | (((o)&0x03)<<4) | (((p)&0x03)<<6))
/*
#define ROWPACK(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p) \
	((((a)&0x03)<<6) | (((b)&0x03)<<4) | (((c)&0x03)<<2) | ((d)&0x03)), \
	((((e)&0x03)<<6) | (((f)&0x03)<<4) | (((g)&0x03)<<2) | ((h)&0x03)), \
	((((i)&0x03)<<6) | (((j)&0x03)<<4) | (((k)&0x03)<<2) | ((l)&0x03)), \
	((((m)&0x03)<<6) | (((n)&0x03)<<4) | (((o)&0x03)<<2) | ((p)&0x03))
*/
//Create a mask from an ROWPACKed image row...
// masks are only 8 bytes, I think we can spare having the same functions
// handle all drawings whether they need a mask or not
#define MPACK(a,b,c,d) \
( \
 	(ISTRUE((a)&0xc0)<<7) | (ISTRUE((a)&0x30)<<6) | \
	(ISTRUE((a)&0x0c)<<5) | (ISTRUE((a)&0x03)<<4) | \
	(ISTRUE((b)&0xc0)<<3) | (ISTRUE((b)&0x30)<<2) | \
	(ISTRUE((b)&0x0c)<<1) | (ISTRUE((b)&0x03)) \
),( \
 	(ISTRUE((c)&0xc0)<<7) | (ISTRUE((c)&0x30)<<6) | \
	(ISTRUE((c)&0x0c)<<5) | (ISTRUE((c)&0x03)<<4) | \
	(ISTRUE((d)&0xc0)<<3) | (ISTRUE((d)&0x30)<<2) | \
	(ISTRUE((d)&0x0c)<<1) | (ISTRUE((d)&0x03)) \
)

#define IMAGE_INIT(imageName)	\
{ \
	imageName##R0, \
	imageName##R1, \
	imageName##R2, \
	imageName##R3, \
	imageName##R4, \
	imageName##R5, \
	imageName##R6, \
	imageName##R7, \
	imageName##R8, \
	imageName##R9, \
	imageName##RA, \
	imageName##RB, \
	imageName##RC, \
	imageName##RD, \
	imageName##RE, \
	imageName##RF \
}

#define GN2(imageRowName, rVal) imageRowName##rVal
#define GETNAMED(imageRowName, rVal) GN2(imageRowName, rVal)
#define MASK_INIT(imageRowName) \
{	\
	MPACK(GETNAMED(imageRowName,R0)), \
	MPACK(GETNAMED(imageRowName,R1)), \
	MPACK(GETNAMED(imageRowName,R2)), \
	MPACK(GETNAMED(imageRowName,R3)), \
	MPACK(GETNAMED(imageRowName,R4)), \
	MPACK(GETNAMED(imageRowName,R5)), \
	MPACK(GETNAMED(imageRowName,R6)), \
	MPACK(GETNAMED(imageRowName,R7)), \
	MPACK(GETNAMED(imageRowName,R8)), \
	MPACK(GETNAMED(imageRowName,R9)), \
	MPACK(GETNAMED(imageRowName,RA)), \
	MPACK(GETNAMED(imageRowName,RB)), \
	MPACK(GETNAMED(imageRowName,RC)), \
	MPACK(GETNAMED(imageRowName,RD)), \
	MPACK(GETNAMED(imageRowName,RE)), \
	MPACK(GETNAMED(imageRowName,RF)) \
}


#endif

/* mehPL:
 *    I would love to believe in a world where licensing shouldn't be
 *    necessary; where people would respect others' work and wishes, 
 *    and give credit where it's due. 
 *    A world where those who find people's work useful would at least 
 *    send positive vibes--if not an email.
 *    A world where we wouldn't have to think about the potential
 *    legal-loopholes that others may take advantage of.
 *
 *    Until that world exists:
 *
 *    This software and associated hardware design is free to use,
 *    modify, and even redistribute, etc. with only a few exceptions
 *    I've thought-up as-yet (this list may be appended-to, hopefully it
 *    doesn't have to be):
 * 
 *    1) Please do not change/remove this licensing info.
 *    2) Please do not change/remove others' credit/licensing/copyright 
 *         info, where noted. 
 *    3) If you find yourself profiting from my work, please send me a
 *         beer, a trinket, or cash is always handy as well.
 *         (Please be considerate. E.G. if you've reposted my work on a
 *          revenue-making (ad-based) website, please think of the
 *          years and years of hard work that went into this!)
 *    4) If you *intend* to profit from my work, you must get my
 *         permission, first. 
 *    5) No permission is given for my work to be used in Military, NSA,
 *         or other creepy-ass purposes. No exceptions. And if there's 
 *         any question in your mind as to whether your project qualifies
 *         under this category, you must get my explicit permission.
 *
 *    The open-sourced project this originated from is ~98% the work of
 *    the original author, except where otherwise noted.
 *    That includes the "commonCode" and makefiles.
 *    Thanks, of course, should be given to those who worked on the tools
 *    I've used: avr-dude, avr-gcc, gnu-make, vim, usb-tiny, and 
 *    I'm certain many others. 
 *    And, as well, to the countless coders who've taken time to post
 *    solutions to issues I couldn't solve, all over the internets.
 *
 *
 *    I'd love to hear of how this is being used, suggestions for
 *    improvements, etc!
 *         
 *    The creator of the original code and original hardware can be
 *    contacted at:
 *
 *        EricWazHung At Gmail Dotcom
 *
 *    This code's origin (and latest versions) can be found at:
 *
 *        https://code.google.com/u/ericwazhung/
 *
 *    The site associated with the original open-sourced project is at:
 *
 *        https://sites.google.com/site/geekattempts/
 *
 *    If any of that ever changes, I will be sure to note it here, 
 *    and add a link at the pages above.
 *
 * This license added to the original file located at:
 * /Users/meh/_avrProjects/LCDdirectLVDS/68-backToLTN/icons/iconPacking.h
 *
 *    (Wow, that's a lot longer than I'd hoped).
 *
 *    Enjoy!
 */

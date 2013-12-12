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
	uint8_t * p_image;
	uint8_t * p_mask;
	uint8_t * p_palette;
	uint8_t numPalettes;
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


uint8_t getGimpColorVal(sprite_t *sprite, uint8_t palette, uint8_t row, 
																			 	uint8_t col)
{
	return 
	pgm_read_byte((uint8_t *)(&(sprite->p_palette[ (palette)*4 + \
		(((pgm_read_byte((uint8_t *)(&(sprite->p_image[ \
					(row)*PACKED_BYTES_PER_ROW + (col)/PIXELS_PER_PACKAGE])))\
		  )>>((col)%PIXELS_PER_PACKAGE)*(PACKED_BITS_PER_PIXEL))&0x03) ])));
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


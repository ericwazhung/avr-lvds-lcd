/* mehPL:
 *    This is Open Source, but NOT GPL. I call it mehPL.
 *    I'm not too fond of long licenses at the top of the file.
 *    Please see the bottom.
 *    Enjoy!
 */


#include "iconPacking.h"


// This image-data was generated from screenshots from Nintendo's 
// Super Mario Brothers
// Converted for use here using The Gimp -> Save as Header File
// And hand-manipulated into this form for viewability/usability here

#define COINR0 ROWPACK(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0)
#define COINR1 ROWPACK(0,0,0,0,0,0,0,2,2,0,0,0,0,0,0,0)
#define COINR2 ROWPACK(0,0,0,0,0,0,2,2,2,2,0,0,0,0,0,0)
#define COINR3 ROWPACK(0,0,0,0,0,2,2,2,2,2,2,0,0,0,0,0)
#define COINR4 ROWPACK(0,0,0,0,0,2,2,3,1,2,2,0,0,0,0,0)
#define COINR5 ROWPACK(0,0,0,0,2,2,3,2,2,1,2,2,0,0,0,0)
#define COINR6 ROWPACK(0,0,0,0,2,2,3,2,2,1,2,2,0,0,0,0)
#define COINR7 ROWPACK(0,0,0,0,2,2,3,2,2,1,2,2,0,0,0,0)
#define COINR8 ROWPACK(0,0,0,0,2,2,3,2,2,1,2,2,0,0,0,0)
#define COINR9 ROWPACK(0,0,0,0,2,2,3,2,2,1,2,2,0,0,0,0)
#define COINRA ROWPACK(0,0,0,0,2,2,3,2,2,1,2,2,0,0,0,0)
#define COINRB ROWPACK(0,0,0,0,0,2,2,3,1,2,2,0,0,0,0,0)
#define COINRC ROWPACK(0,0,0,0,0,2,2,2,2,2,2,0,0,0,0,0)
#define COINRD ROWPACK(0,0,0,0,0,0,2,2,2,2,0,0,0,0,0,0)
#define COINRE ROWPACK(0,0,0,0,0,0,0,2,2,0,0,0,0,0,0,0)
#define COINRF ROWPACK(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0)


#define COIN1R0 ROWPACK(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0)
#define COIN1R1 ROWPACK(0,0,0,0,0,0,0,2,1,0,0,0,0,0,0,0)
#define COIN1R2 ROWPACK(0,0,0,0,0,0,0,2,1,0,0,0,0,0,0,0)
#define COIN1R3 ROWPACK(0,0,0,0,0,0,2,1,1,1,0,0,0,0,0,0)
#define COIN1R4 ROWPACK(0,0,0,0,0,0,2,1,1,1,0,0,0,0,0,0)
#define COIN1R5 ROWPACK(0,0,0,0,0,0,2,1,1,1,0,0,0,0,0,0)
#define COIN1R6 ROWPACK(0,0,0,0,0,0,2,1,1,1,0,0,0,0,0,0)
#define COIN1R7 ROWPACK(0,0,0,0,0,0,3,1,1,1,0,0,0,0,0,0)
#define COIN1R8 ROWPACK(0,0,0,0,0,0,3,1,1,1,0,0,0,0,0,0)
#define COIN1R9 ROWPACK(0,0,0,0,0,0,2,1,1,1,0,0,0,0,0,0)
#define COIN1RA ROWPACK(0,0,0,0,0,0,2,1,1,1,0,0,0,0,0,0)
#define COIN1RB ROWPACK(0,0,0,0,0,0,2,1,1,1,0,0,0,0,0,0)
#define COIN1RC ROWPACK(0,0,0,0,0,0,2,1,1,1,0,0,0,0,0,0)
#define COIN1RD ROWPACK(0,0,0,0,0,0,0,2,1,0,0,0,0,0,0,0)
#define COIN1RE ROWPACK(0,0,0,0,0,0,0,2,1,0,0,0,0,0,0,0)
#define COIN1RF ROWPACK(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0)


#define COIN2R0 ROWPACK(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0)
#define COIN2R1 ROWPACK(0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0)
#define COIN2R2 ROWPACK(0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0)
#define COIN2R3 ROWPACK(0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0)
#define COIN2R4 ROWPACK(0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0)
#define COIN2R5 ROWPACK(0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0)
#define COIN2R6 ROWPACK(0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0)
#define COIN2R7 ROWPACK(0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0)
#define COIN2R8 ROWPACK(0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0)
#define COIN2R9 ROWPACK(0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0)
#define COIN2RA ROWPACK(0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0)
#define COIN2RB ROWPACK(0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0)
#define COIN2RC ROWPACK(0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0)
#define COIN2RD ROWPACK(0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0)
#define COIN2RE ROWPACK(0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0)
#define COIN2RF ROWPACK(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0)


//It looks to me like this is a direct flip of COIN1...
// (But the colors are different)
#define COIN3R0 ROWPACK(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0)
#define COIN3R1 ROWPACK(0,0,0,0,0,0,0,3,1,0,0,0,0,0,0,0)
#define COIN3R2 ROWPACK(0,0,0,0,0,0,0,3,1,0,0,0,0,0,0,0)
#define COIN3R3 ROWPACK(0,0,0,0,0,0,3,3,3,1,0,0,0,0,0,0)
#define COIN3R4 ROWPACK(0,0,0,0,0,0,3,3,3,1,0,0,0,0,0,0)
#define COIN3R5 ROWPACK(0,0,0,0,0,0,3,3,3,1,0,0,0,0,0,0)
#define COIN3R6 ROWPACK(0,0,0,0,0,0,3,3,3,1,0,0,0,0,0,0)
#define COIN3R7 ROWPACK(0,0,0,0,0,0,3,3,3,1,0,0,0,0,0,0)
#define COIN3R8 ROWPACK(0,0,0,0,0,0,3,3,3,1,0,0,0,0,0,0)
#define COIN3R9 ROWPACK(0,0,0,0,0,0,3,3,3,1,0,0,0,0,0,0)
#define COIN3RA ROWPACK(0,0,0,0,0,0,3,3,3,1,0,0,0,0,0,0)
#define COIN3RB ROWPACK(0,0,0,0,0,0,3,3,3,1,0,0,0,0,0,0)
#define COIN3RC ROWPACK(0,0,0,0,0,0,3,3,3,1,0,0,0,0,0,0)
#define COIN3RD ROWPACK(0,0,0,0,0,0,0,3,1,0,0,0,0,0,0,0)
#define COIN3RE ROWPACK(0,0,0,0,0,0,0,3,1,0,0,0,0,0,0,0)
#define COIN3RF ROWPACK(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0)

//37 (sky), 7, 47, 2

const static uint8_t pgm_imageCOIN[ICON_PACKED_BYTES] PROGMEM =
   IMAGE_INIT(COIN);
   
const static uint8_t pgm_imageCOIN1[ICON_PACKED_BYTES] PROGMEM =
	IMAGE_INIT(COIN1);

const static uint8_t pgm_imageCOIN2[ICON_PACKED_BYTES] PROGMEM =
	IMAGE_INIT(COIN2);

const static uint8_t pgm_imageCOIN3[ICON_PACKED_BYTES] PROGMEM =
	IMAGE_INIT(COIN3);

#define pgm_maskCOIN	NULL
//static uint8_t * pgm_maskCOIN; //[ICON_MASK_BYTES]; // PROGMEM =
//  MASK_INIT(GETNAMED(QUESTION));


#define NUMPALETTES_COIN 1

//gimpPixelValToLColor should probably be taken into account.
const static uint8_t pgm_paletteCOIN[4*NUMPALETTES_COIN] PROGMEM =
 //  { 37, 7, 47, 2};
	{ 37, 2, 7, 47 };	//see Coin1.h for notes...
// 0 -> 0
// 1 -> 2
// 2 -> 3
// 3 -> 1

static sprite_t spriteCOIN =
      {pgm_imageCOIN, pgm_maskCOIN, pgm_paletteCOIN, NUMPALETTES_COIN};


uint8_t getRawPixelValCOIN(uint8_t spritePhase, uint8_t row, uint8_t col)
{
	const uint8_t * p_image;

	//Is this better accomplished as a switch-statement or with math...?
	switch(spritePhase%8)
	{
		case 0:
		case 1:
//		case 2:
			p_image = pgm_imageCOIN;
			break;
		case 2:
		case 3:
			p_image = pgm_imageCOIN1;
			break;
		case 4:
		case 5:
			p_image = pgm_imageCOIN2;
			break;
		case 6:
		case 7:
		default:
			p_image = pgm_imageCOIN3;
			break;
	}	

   return
      (((pgm_read_byte((uint8_t *)(&(p_image[ \
               (row)*PACKED_BYTES_PER_ROW + (col)/PIXELS_PER_PACKAGE])))\
        )>>((col)%PIXELS_PER_PACKAGE)*(PACKED_BITS_PER_PIXEL))&0x03);
}  

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
 *    2) Please do not change/remove others' credit/licensing/copywrite 
 *         info, where noted. 
 *    3) If you find yourself profitting from my work, please send me a
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
 *    (Wow, that's a lot longer than I'd hoped).
 *
 *    Enjoy!
 */

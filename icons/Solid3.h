/* mehPL:
 *    This is Open Source, but NOT GPL. I call it mehPL.
 *    I'm not too fond of long licenses at the top of the file.
 *    Please see the bottom.
 *    Enjoy!
 */


#ifndef __SOLID3_H__
#define __SOLID3_H__









#include "iconPacking.h"
#include "defaultMotion.c"
#include "Question3.h"	//for palette
// This image-data was generated from screenshots from Nintendo's 
// Super Mario Brothers
// Converted for use here using The Gimp -> Save as Header File
// And hand-manipulated into this form for viewability/usability here

#define SOLID3R0 ROWPACK(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1)
#define SOLID3R1 ROWPACK(1,3,3,3,3,3,3,3,3,3,3,3,3,3,3,1)
#define SOLID3R2 ROWPACK(1,3,1,3,3,3,3,3,3,3,3,3,3,1,3,1)
#define SOLID3R3 ROWPACK(1,3,3,3,3,3,3,3,3,3,3,3,3,3,3,1)
#define SOLID3R4 ROWPACK(1,3,3,3,3,3,3,3,3,3,3,3,3,3,3,1)
#define SOLID3R5 ROWPACK(1,3,3,3,3,3,3,3,3,3,3,3,3,3,3,1)
#define SOLID3R6 ROWPACK(1,3,3,3,3,3,3,3,3,3,3,3,3,3,3,1)
#define SOLID3R7 ROWPACK(1,3,3,3,3,3,3,3,3,3,3,3,3,3,3,1)
#define SOLID3R8 ROWPACK(1,3,3,3,3,3,3,3,3,3,3,3,3,3,3,1)
#define SOLID3R9 ROWPACK(1,3,3,3,3,3,3,3,3,3,3,3,3,3,3,1)
#define SOLID3RA ROWPACK(1,3,3,3,3,3,3,3,3,3,3,3,3,3,3,1)
#define SOLID3RB ROWPACK(1,3,3,3,3,3,3,3,3,3,3,3,3,3,3,1)
#define SOLID3RC ROWPACK(1,3,3,3,3,3,3,3,3,3,3,3,3,3,3,1)
#define SOLID3RD ROWPACK(1,3,1,3,3,3,3,3,3,3,3,3,3,1,3,1)
#define SOLID3RE ROWPACK(1,3,3,3,3,3,3,3,3,3,3,3,3,3,3,1)
#define SOLID3RF ROWPACK(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1)

//37 (sky), 2, not-used, 0

const static uint8_t pgm_imageSOLID3[1][ICON_PACKED_BYTES] PROGMEM =
	{ IMAGE_INIT(SOLID3) };


#define pgm_maskSOLID3	NULL
//static uint8_t pgm_maskSOLID3[ICON_MASK_BYTES]; // PROGMEM =
//  MASK_INIT(GETNAMED(QUESTION));


#define NUMPALETTES_SOLID3 1

#define pgm_paletteSOLID3	pgm_paletteQUESTION3
//gimpPixelValToLColor should probably be taken into account.
/*	const static uint8_t pgm_paletteSOLID3[4*NUMPALETTES_SOLID3] PROGMEM =
   { 37, 
		0,	//Black
		rgb2(3,1,1), //Peach, unused
		rgb2, 0, 0}; 
*/
const __flash sprite_t spriteSOLID3 =
      {
			pgm_imageSOLID3, 
			pgm_maskSOLID3, 
			pgm_paletteSOLID3, 
			NUMPALETTES_SOLID3,
			8,
			NadaFlip,
			NadaMotion,
			NadaLayer,
			NadaCamMotion,
			NULL,
			1
		};

#endif //__SOLID3_H__

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
 * /Users/meh/_avrProjects/LCDdirectLVDS/93-checkingProcessAgain/icons/Solid.h
 *
 *    (Wow, that's a lot longer than I'd hoped).
 *
 *    Enjoy!
 */

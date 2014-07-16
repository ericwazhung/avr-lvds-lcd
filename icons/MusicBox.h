/* mehPL:
 *    This is Open Source, but NOT GPL. I call it mehPL.
 *    I'm not too fond of long licenses at the top of the file.
 *    Please see the bottom.
 *    Enjoy!
 */












#ifndef __MUSICBOX_H__
#define __MUSICBOX_H__

#include "iconPacking.h"
#include "defaultMotion.c"

// This image-data was generated from screenshots from Nintendo's 
// Super Mario Brothers
// Converted for use here using The Gimp -> Save as Header File
// And hand-manipulated into this form for viewability/usability here
// NAH UH!... That message from other sprites just to show how much fun I
// had *entering it by hand* here. WEE!
// (eyeballed from images on the web)
// heh, apparently I'm off by a pixel, somewhere... too wide?
#define MUSICBOX1R0 ROWPACK(0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0)
#define MUSICBOX1R1 ROWPACK(0,0,1,1,3,3,2,2,2,1,1,0,0,0,0,0)
#define MUSICBOX1R2 ROWPACK(0,1,3,3,2,2,2,2,2,2,2,1,0,0,0,0)
#define MUSICBOX1R3 ROWPACK(1,3,3,2,2,2,2,2,2,2,2,2,1,0,0,0)
#define MUSICBOX1R4 ROWPACK(1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0)
#define MUSICBOX1R5 ROWPACK(1,3,3,3,3,3,3,3,3,3,3,3,1,0,0,0)
#define MUSICBOX1R6 ROWPACK(1,3,3,3,3,1,1,1,1,1,1,3,1,0,1,1)
#define MUSICBOX1R7 ROWPACK(1,3,3,3,3,1,3,3,3,3,1,3,1,0,1,0)
#define MUSICBOX1R8 ROWPACK(1,3,3,3,3,1,1,1,1,1,1,3,1,0,1,0)
#define MUSICBOX1R9 ROWPACK(1,3,3,3,3,1,3,3,3,3,1,3,1,1,1,0)
#define MUSICBOX1RA ROWPACK(1,3,3,3,3,1,3,3,3,3,1,3,1,0,0,0)
#define MUSICBOX1RB ROWPACK(1,3,3,1,1,1,3,3,1,1,1,3,1,0,0,0)
#define MUSICBOX1RC ROWPACK(1,3,1,1,1,1,3,1,1,1,1,3,1,0,0,0)
#define MUSICBOX1RD ROWPACK(1,3,1,1,1,3,3,1,1,1,3,3,1,0,0,0)
#define MUSICBOX1RE ROWPACK(1,3,3,3,3,3,3,3,3,3,3,3,1,0,0,0)
#define MUSICBOX1RF ROWPACK(1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0)

//Adding motion:
//This increased codesize from 7134 -> 7262Bytes! I coulda sworn this was
//packed more tightly than that! Each image takes 64 bytes?! yep.
#define MUSICBOX2R0 ROWPACK(0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0)
#define MUSICBOX2R1 ROWPACK(0,0,1,1,3,3,2,2,2,1,1,0,0,0,0,0)
#define MUSICBOX2R2 ROWPACK(0,1,3,3,2,2,2,2,2,2,2,1,0,0,0,0)
#define MUSICBOX2R3 ROWPACK(1,3,3,2,2,2,2,2,2,2,2,2,1,0,0,0)
#define MUSICBOX2R4 ROWPACK(1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0)
#define MUSICBOX2R5 ROWPACK(1,3,3,3,3,3,3,3,3,3,3,3,1,0,0,0)
#define MUSICBOX2R6 ROWPACK(1,3,3,3,3,1,1,1,1,1,1,3,1,0,0,0)
#define MUSICBOX2R7 ROWPACK(1,3,3,3,3,1,3,3,3,3,1,3,1,0,0,0)
#define MUSICBOX2R8 ROWPACK(1,3,3,3,3,1,1,1,1,1,1,3,1,0,1,1)
#define MUSICBOX2R9 ROWPACK(1,3,3,3,3,1,3,3,3,3,1,3,1,1,0,0)
#define MUSICBOX2RA ROWPACK(1,3,3,3,3,1,3,3,3,3,1,3,1,0,0,0)
#define MUSICBOX2RB ROWPACK(1,3,3,1,1,1,3,3,1,1,1,3,1,0,0,0)
#define MUSICBOX2RC ROWPACK(1,3,1,1,1,1,3,1,1,1,1,3,1,0,0,0)
#define MUSICBOX2RD ROWPACK(1,3,1,1,1,3,3,1,1,1,3,3,1,0,0,0)
#define MUSICBOX2RE ROWPACK(1,3,3,3,3,3,3,3,3,3,3,3,1,0,0,0)
#define MUSICBOX2RF ROWPACK(1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0)

#define MUSICBOX3R0 ROWPACK(0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0)
#define MUSICBOX3R1 ROWPACK(0,0,1,1,3,3,2,2,2,1,1,0,0,0,0,0)
#define MUSICBOX3R2 ROWPACK(0,1,3,3,2,2,2,2,2,2,2,1,0,0,0,0)
#define MUSICBOX3R3 ROWPACK(1,3,3,2,2,2,2,2,2,2,2,2,1,0,0,0)
#define MUSICBOX3R4 ROWPACK(1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0)
#define MUSICBOX3R5 ROWPACK(1,3,3,3,3,3,3,3,3,3,3,3,1,0,0,0)
#define MUSICBOX3R6 ROWPACK(1,3,3,3,3,1,1,1,1,1,1,3,1,0,0,0)
#define MUSICBOX3R7 ROWPACK(1,3,3,3,3,1,3,3,3,3,1,3,1,0,0,0)
#define MUSICBOX3R8 ROWPACK(1,3,3,3,3,1,1,1,1,1,1,3,1,0,0,0)
#define MUSICBOX3R9 ROWPACK(1,3,3,3,3,1,3,3,3,3,1,3,1,1,1,0)
#define MUSICBOX3RA ROWPACK(1,3,3,3,3,1,3,3,3,3,1,3,1,0,1,0)
#define MUSICBOX3RB ROWPACK(1,3,3,1,1,1,3,3,1,1,1,3,1,0,1,1)
#define MUSICBOX3RC ROWPACK(1,3,1,1,1,1,3,1,1,1,1,3,1,0,0,0)
#define MUSICBOX3RD ROWPACK(1,3,1,1,1,3,3,1,1,1,3,3,1,0,0,0)
#define MUSICBOX3RE ROWPACK(1,3,3,3,3,3,3,3,3,3,3,3,1,0,0,0)
#define MUSICBOX3RF ROWPACK(1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0)


#define MUSICBOX_IMAGES	3

const static uint8_t pgm_imageMUSICBOX[MUSICBOX_IMAGES][ICON_PACKED_BYTES] PROGMEM = 
	{ 
		IMAGE_INIT(MUSICBOX1),
		IMAGE_INIT(MUSICBOX2),
		IMAGE_INIT(MUSICBOX3),
  	};



#define pgm_maskMUSICBOX	NULL
//static uint8_t pgm_maskMUSICBOX[ICON_MASK_BYTES]; // PROGMEM =
//	MASK_INIT(MUSICBOX);


#define NUMPALETTES_MUSICBOX	1

//1up Mapping: 0->37 (sky/mask) 1->7, 2->4 3->47
//gimpPixelValToLColor should probably be taken into account.
const static uint8_t pgm_paletteMUSICBOX[4*NUMPALETTES_MUSICBOX] PROGMEM = 
	{ 37, //Sky
		0,	//Black outline 
	  //rgb2(3,2,2), //Shading
	  rgb2(3,1,1),
	  rgb2(3,3,3)  //White
  	};

const __flash sprite_t spriteMUSICBOX = 
		{ 
			pgm_imageMUSICBOX, 
			pgm_maskMUSICBOX, 
			pgm_paletteMUSICBOX, 
			NUMPALETTES_MUSICBOX,
			DEFAULT_MOTIONS,
			NadaFlip,
			DefaultMotion,
			DefaultLayer,
			DefaultCamMotion,
			NULL,
			MUSICBOX_IMAGES
		};

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
 * /Users/meh/_avrProjects/LCDdirectLVDS/93-checkingProcessAgain/icons/1up.h
 *
 *    (Wow, that's a lot longer than I'd hoped).
 *
 *    Enjoy!
 */

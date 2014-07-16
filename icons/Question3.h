/* mehPL:
 *    This is Open Source, but NOT GPL. I call it mehPL.
 *    I'm not too fond of long licenses at the top of the file.
 *    Please see the bottom.
 *    Enjoy!
 */




#ifndef __QUESTION3_H__
#define __QUESTION3_H__







#include "iconPacking.h"
#include "defaultMotion.c"

// This image-data was generated from screenshots from Nintendo's 
// Super Mario Brothers
// Converted for use here using The Gimp -> Save as Header File
// And hand-manipulated into this form for viewability/usability here
// (Actually, this one's entered manually from zoomed-screenshots)

//The question-box in Mario3 has a '?' scrolling from left-to-right
//There's a Frame that contains it...
// In that frame, only two colors are used, black for the frame and screws
// and a light peach shading
// Here we'll leave "0" which is generally the "sky" color, but is also
// used to indicate transparency...
// Thus, we'll draw the '?' first, then draw the frame on top of it.
// (Oddly, the '?' appears differently in the actual game, as it scrolls...
//  e.g. it's horizontally shortened when it's closer to the left-side
//  That's way too much work.)

#define QUESTIONFRAME3R0 ROWPACK(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1)
#define QUESTIONFRAME3R1 ROWPACK(1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1)
#define QUESTIONFRAME3R2 ROWPACK(1,2,1,0,0,0,0,0,0,0,0,0,0,1,0,1)
#define QUESTIONFRAME3R3 ROWPACK(1,2,0,0,0,0,0,0,0,0,0,0,0,0,0,1)
#define QUESTIONFRAME3R4 ROWPACK(1,2,0,0,0,0,0,0,0,0,0,0,0,0,0,1)
#define QUESTIONFRAME3R5 ROWPACK(1,2,0,0,0,0,0,0,0,0,0,0,0,0,0,1)
#define QUESTIONFRAME3R6 ROWPACK(1,2,0,0,0,0,0,0,0,0,0,0,0,0,0,1)
#define QUESTIONFRAME3R7 ROWPACK(1,2,0,0,0,0,0,0,0,0,0,0,0,0,0,1)
#define QUESTIONFRAME3R8 ROWPACK(1,2,0,0,0,0,0,0,0,0,0,0,0,0,0,1)
#define QUESTIONFRAME3R9 ROWPACK(1,2,0,0,0,0,0,0,0,0,0,0,0,0,0,1)
#define QUESTIONFRAME3RA ROWPACK(1,2,0,0,0,0,0,0,0,0,0,0,0,0,0,1)
#define QUESTIONFRAME3RB ROWPACK(1,2,0,0,0,0,0,0,0,0,0,0,0,0,0,1)
#define QUESTIONFRAME3RC ROWPACK(1,2,0,0,0,0,0,0,0,0,0,0,0,0,0,1)
#define QUESTIONFRAME3RD ROWPACK(1,2,1,0,0,0,0,0,0,0,0,0,0,1,0,1)
#define QUESTIONFRAME3RE ROWPACK(1,2,0,0,0,0,0,0,0,0,0,0,0,0,0,1)
#define QUESTIONFRAME3RF ROWPACK(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1)

#define QUESTIONMARK3R0 ROWPACK(3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3)
#define QUESTIONMARK3R1 ROWPACK(3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3)
#define QUESTIONMARK3R2 ROWPACK(3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3)
#define QUESTIONMARK3R3 ROWPACK(3,3,3,3,2,2,2,2,2,2,2,3,3,3,3,3)
#define QUESTIONMARK3R4 ROWPACK(3,3,3,2,2,2,2,2,2,2,2,2,3,3,3,3)
#define QUESTIONMARK3R5 ROWPACK(3,3,3,2,2,2,1,1,1,2,2,2,1,3,3,3)
#define QUESTIONMARK3R6 ROWPACK(3,3,3,3,1,1,1,3,3,2,2,2,1,3,3,3)
#define QUESTIONMARK3R7 ROWPACK(3,3,3,3,3,3,2,2,2,2,2,1,1,3,3,3)
#define QUESTIONMARK3R8 ROWPACK(3,3,3,3,3,3,2,2,2,1,1,1,3,3,3,3)
#define QUESTIONMARK3R9 ROWPACK(3,3,3,3,3,3,3,1,1,1,3,3,3,3,3,3)
#define QUESTIONMARK3RA ROWPACK(3,3,3,3,3,3,2,2,2,3,3,3,3,3,3,3)
#define QUESTIONMARK3RB ROWPACK(3,3,3,3,3,3,2,2,2,1,3,3,3,3,3,3)
#define QUESTIONMARK3RC ROWPACK(3,3,3,3,3,3,3,1,1,1,3,3,3,3,3,3)
#define QUESTIONMARK3RD ROWPACK(3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3)
#define QUESTIONMARK3RE ROWPACK(3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3)
#define QUESTIONMARK3RF ROWPACK(3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3)


const static uint8_t pgm_imageQUESTION3[2][ICON_PACKED_BYTES] PROGMEM =
{ 
	IMAGE_INIT(QUESTIONMARK3),	//Listed in the order to be drawn...
	IMAGE_INIT(QUESTIONFRAME3) //...frame on top
};


#define pgm_maskQUESTION3	NULL

#define NUMPALETTES_QUESTION3 1

const static uint8_t pgm_paletteQUESTION3[4*NUMPALETTES_QUESTION3] PROGMEM=
   { 
		37, 	// Usually used for "sky" but that's not drawn
				// It may be used for skyColorOverride	
		0,		//Black for frame/screws/shadow
#if(defined(__SONY_ACX705AKM_H__))
#warning "Why does it seem like green is only two shades?!"
		rgb2(3,2,1),	//Peach for frame-shadow and '?'
#else
		rgb2(3,1,1),	//Peach for frame-shadow and '?'
#endif
		rgb2(3,1,0)		//Orange for the background
	};


//Drawing QUESTION3 will require special-handling...
// especially for the motion of the '?'
// So, for now, there's no sense in creating a normal 'motion' for it...


const __flash sprite_t spriteQUESTION3 =
      { 
			.p_image 			= pgm_imageQUESTION3, 
			.p_mask 				= pgm_maskQUESTION3, 
			.p_palette 			= pgm_paletteQUESTION3, 
			.numPalettes 		= NUMPALETTES_QUESTION3,
			.totalCount 		= 16, //???//Q_TOTALCOUNT, //(12*5), 
														//stolen from prepNextSprite()
			.p_hFlip 			= NadaFlip,
			.p_motion 			= NadaMotion,
			.p_layer 			= NadaLayer,
			.p_camMotion 		= NadaMotion,
			.p_paletteAdvance = NULL,
			.numImages			= 1
		};

#endif //__QUESTION3_H__

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
 * /Users/meh/_avrProjects/LCDdirectLVDS/93-checkingProcessAgain/icons/Question.h
 *
 *    (Wow, that's a lot longer than I'd hoped).
 *
 *    Enjoy!
 */

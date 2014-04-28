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

#define DEADGOOMBAR0 ROWPACK(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0)
#define DEADGOOMBAR1 ROWPACK(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0)
#define DEADGOOMBAR2 ROWPACK(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0)
#define DEADGOOMBAR3 ROWPACK(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0)
#define DEADGOOMBAR4 ROWPACK(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0)
#define DEADGOOMBAR5 ROWPACK(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0)
#define DEADGOOMBAR6 ROWPACK(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0)
#define DEADGOOMBAR7 ROWPACK(0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0)
#define DEADGOOMBAR8 ROWPACK(0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0)
#define DEADGOOMBAR9 ROWPACK(0,1,1,3,3,3,1,1,1,1,3,3,3,1,1,0)
#define DEADGOOMBARA ROWPACK(1,1,2,2,2,2,3,3,3,3,2,2,2,2,1,1)
#define DEADGOOMBARB ROWPACK(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1)
#define DEADGOOMBARC ROWPACK(0,0,0,2,2,2,2,2,2,2,2,2,2,0,0,0)
#define DEADGOOMBARD ROWPACK(0,0,0,2,2,2,2,2,2,2,2,2,2,0,0,0)
#define DEADGOOMBARE ROWPACK(0,0,3,3,3,3,0,0,0,0,3,3,3,3,3,0)
#define DEADGOOMBARF ROWPACK(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0)

const static uint8_t pgm_imageDEADGOOMBA[ICON_PACKED_BYTES] PROGMEM =
   IMAGE_INIT(DEADGOOMBA);


#define pgm_maskDEADGOOMBA NULL
//static uint8_t * pgm_maskDEADGOOMBA; //[ICON_MASK_BYTES]; // PROGMEM =
//  MASK_INIT(GETNAMED(QUESTION));


#define NUMPALETTES_DEADGOOMBA 1

//gimpPixelValToLColor should probably be taken into account.
const static uint8_t pgm_paletteDEADGOOMBA[4*NUMPALETTES_DEADGOOMBA] PROGMEM =
{
	//AGAIN: This is NOT a safe method, normally... (rgb8() here)
	37,
	//rgb8(141,157,255),
	rgb8(206, 28, 60),
	rgb8(255,157,190),
	rgb8(  0, 24, 60)
};

static sprite_t spriteDEADGOOMBA =
      {pgm_imageDEADGOOMBA, pgm_maskDEADGOOMBA, pgm_paletteDEADGOOMBA, 
			NUMPALETTES_DEADGOOMBA};

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

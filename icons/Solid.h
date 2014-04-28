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

#define SOLIDR0 ROWPACK(0,3,3,3,3,3,3,3,3,3,3,3,3,3,3,0)
#define SOLIDR1 ROWPACK(3,1,1,1,1,1,1,1,1,1,1,1,1,1,1,3)
#define SOLIDR2 ROWPACK(3,1,3,1,1,1,1,1,1,1,1,1,1,3,1,3)
#define SOLIDR3 ROWPACK(3,1,1,1,1,1,1,1,1,1,1,1,1,1,1,3)
#define SOLIDR4 ROWPACK(3,1,1,1,1,1,1,1,1,1,1,1,1,1,1,3)
#define SOLIDR5 ROWPACK(3,1,1,1,1,1,1,1,1,1,1,1,1,1,1,3)
#define SOLIDR6 ROWPACK(3,1,1,1,1,1,1,1,1,1,1,1,1,1,1,3)
#define SOLIDR7 ROWPACK(3,1,1,1,1,1,1,1,1,1,1,1,1,1,1,3)
#define SOLIDR8 ROWPACK(3,1,1,1,1,1,1,1,1,1,1,1,1,1,1,3)
#define SOLIDR9 ROWPACK(3,1,1,1,1,1,1,1,1,1,1,1,1,1,1,3)
#define SOLIDRA ROWPACK(3,1,1,1,1,1,1,1,1,1,1,1,1,1,1,3)
#define SOLIDRB ROWPACK(3,1,1,1,1,1,1,1,1,1,1,1,1,1,1,3)
#define SOLIDRC ROWPACK(3,1,1,1,1,1,1,1,1,1,1,1,1,1,1,3)
#define SOLIDRD ROWPACK(3,1,3,1,1,1,1,1,1,1,1,1,1,3,1,3)
#define SOLIDRE ROWPACK(3,1,1,1,1,1,1,1,1,1,1,1,1,1,1,3)
#define SOLIDRF ROWPACK(0,3,3,3,3,3,3,3,3,3,3,3,3,3,3,0)

//37 (sky), 2, not-used, 0

const static uint8_t pgm_imageSOLID[ICON_PACKED_BYTES] PROGMEM =
   IMAGE_INIT(SOLID);


#define pgm_maskSOLID	NULL
//static uint8_t pgm_maskSOLID[ICON_MASK_BYTES]; // PROGMEM =
//  MASK_INIT(GETNAMED(QUESTION));


#define NUMPALETTES_SOLID 1

//gimpPixelValToLColor should probably be taken into account.
const static uint8_t pgm_paletteSOLID[4*NUMPALETTES_SOLID] PROGMEM =
   { 37, 2, 0, 0}; 

static sprite_t spriteSOLID =
      {pgm_imageSOLID, pgm_maskSOLID, pgm_paletteSOLID, NUMPALETTES_SOLID};

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

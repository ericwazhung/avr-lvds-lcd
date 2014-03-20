/* mehPL:
 *    This is Open Source, but NOT GPL. I call it mehPL.
 *    I'm not too fond of long licenses at the top of the file.
 *    Please see the bottom.
 *    Enjoy!
 */


#include "iconPacking.h"

#define FLOWERR0 ROWPACK(0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0)
#define FLOWERR1 ROWPACK(0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0)
#define FLOWERR2 ROWPACK(0,1,1,1,2,2,2,2,2,2,2,2,1,1,1,0)
#define FLOWERR3 ROWPACK(1,1,2,2,2,3,3,3,3,3,3,2,2,2,1,1)
#define FLOWERR4 ROWPACK(1,1,2,2,2,3,3,3,3,3,3,2,2,2,1,1)
#define FLOWERR5 ROWPACK(0,1,1,1,2,2,2,2,2,2,2,2,1,1,1,0)
#define FLOWERR6 ROWPACK(0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0)
#define FLOWERR7 ROWPACK(0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0)

#define FLOWERR8 ROWPACK(0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0)
#define FLOWERR9 ROWPACK(1,1,1,0,0,0,0,1,1,0,0,0,0,1,1,1)
#define FLOWERRA ROWPACK(0,1,1,1,0,0,0,1,1,0,0,0,1,1,1,0)
#define FLOWERRB ROWPACK(0,1,1,1,1,0,0,1,1,0,0,1,1,1,1,0)
#define FLOWERRC ROWPACK(0,0,1,1,1,1,0,1,1,0,1,1,1,1,0,0)
#define FLOWERRD ROWPACK(0,0,1,1,1,1,0,1,1,0,1,1,1,1,0,0)
#define FLOWERRE ROWPACK(0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0)
#define FLOWERRF ROWPACK(0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0)


//37, 47, 7, 4 (37, 4, unused, unused -> stem)
//37 (sky), 23 (outter Ring), 2 (inner ring), 0 middle
//37, 7, 5, 2

static uint8_t pgm_imageFLOWER[ICON_PACKED_BYTES] PROGMEM =
   IMAGE_INIT(FLOWER);
   

//Original plan was to use a mask, then that fourth color could be in the
// palette... but the new method is to switch palettes mid-drawing.
// so no mask necessary
#define pgm_maskFLOWER NULL
//static uint8_t pgm_maskFLOWER[ICON_MASK_BYTES]; // PROGMEM =
//  MASK_INIT(GETNAMED(FLOWER));


//Flower has two separate palettes during each draw...
// The first palette is, therefore, the one that remains the same for each
// other palette...
#define NUMPALETTES_FLOWER 4

#define FLOWER_PALETTE1_ROW	8

//gimpPixelValToLColor should probably be taken into account.
static uint8_t pgm_paletteFLOWER[4*NUMPALETTES_FLOWER] PROGMEM =
   { 
	  37,  4, 0, 0,
	  37, 47, 7, 4,
     37, 23, 2, 0,
     37,  7, 5, 2 };

static sprite_t spriteFLOWER =
      { pgm_imageFLOWER, pgm_maskFLOWER, pgm_paletteFLOWER, NUMPALETTES_FLOWER};

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

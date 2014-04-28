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

#define GOOMBAR0 ROWPACK(0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0)
#define GOOMBAR1 ROWPACK(0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0)
#define GOOMBAR2 ROWPACK(0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0)
#define GOOMBAR3 ROWPACK(0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0)
#define GOOMBAR4 ROWPACK(0,0,1,3,3,1,1,1,1,1,1,3,3,1,0,0)
#define GOOMBAR5 ROWPACK(0,1,1,1,2,3,1,1,1,1,3,2,1,1,1,0)
#define GOOMBAR6 ROWPACK(0,1,1,1,2,3,3,3,3,3,3,2,1,1,1,0)
#define GOOMBAR7 ROWPACK(1,1,1,1,2,3,2,1,1,2,3,2,1,1,1,1)
#define GOOMBAR8 ROWPACK(1,1,1,1,2,2,2,1,1,2,2,2,1,1,1,1)
#define GOOMBAR9 ROWPACK(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1)
#define GOOMBARA ROWPACK(0,1,1,1,1,2,2,2,2,2,2,1,1,1,1,0)
#define GOOMBARB ROWPACK(0,0,0,0,2,2,2,2,2,2,2,2,0,0,0,0)
#define GOOMBARC ROWPACK(0,0,0,0,2,2,2,2,2,2,2,2,3,3,0,0)
#define GOOMBARD ROWPACK(0,0,0,3,3,2,2,2,2,2,3,3,3,3,3,0)
#define GOOMBARE ROWPACK(0,0,0,3,3,3,2,2,2,3,3,3,3,3,3,0)
#define GOOMBARF ROWPACK(0,0,0,0,3,3,3,2,3,3,3,3,3,3,0,0)


const static uint8_t pgm_imageGOOMBA[ICON_PACKED_BYTES] PROGMEM =
   IMAGE_INIT(GOOMBA);
   
   
#define pgm_maskGOOMBA NULL
//static uint8_t * pgm_maskGOOMBA; //[ICON_MASK_BYTES]; // PROGMEM =
//  MASK_INIT(GETNAMED(QUESTION));


#define NUMPALETTES_GOOMBA 6



#define GOOMBA_BODY	rgb8(206, 28, 60)
#define GOOMBA_BLACK	rgb8(  0, 24, 60)
#define GOOMBA_WHITE	rgb8(255,157,190)

#define rgb2(r,g,b) \
		(r | (g<<2) | (b<<4))
//gimpPixelValToLColor should probably be taken into account.
const static uint8_t pgm_paletteGOOMBA[4*NUMPALETTES_GOOMBA] PROGMEM =
{
	//rgb8(141,157,255),	//First encounter with rgb8 not working as expected
								//But I kinda like the badguy on a non-sky color
								// might leave this
								// ALSO forgot that these values don't take into
								// account that gimpColor is being modified...
								// (right?)
	rgb2(0,1,0),
	GOOMBA_BODY, //rgb8(206, 28, 60),
	GOOMBA_WHITE, //rgb8(255,157,190),
	GOOMBA_BLACK, //rgb8(  0, 24, 60),
		//Other colors look right, sos I'mma leave 'em
	//Getting Dangerous
	rgb2(0,1,0), //0x0b, 					//yellow
	GOOMBA_BODY, //rgb8(206, 28, 60),
	GOOMBA_WHITE, //rgb8(255,157,190),
	GOOMBA_BLACK, //rgb8(  0, 24, 60),
	//Too Late
	rgb2(1,1,0), //0x0f,						//yellow-orange
	GOOMBA_BODY, //rgb8(206, 28, 60),
	GOOMBA_WHITE, //rgb8(255,157,190),
	GOOMBA_BLACK, //rgb8(  0, 24, 60)

	rgb2(3,1,0), //0x07,        		//orange
	GOOMBA_BODY, 
	GOOMBA_WHITE,
	GOOMBA_BLACK,

	rgb2(3,0,0), //0x03,        		//red
	rgb2(1,0,0), //GOOMBA_BODY, 
	rgb2(2,1,2), //GOOMBA_WHITE,
	GOOMBA_BLACK,

	0x00,        					//black
	0x20, //GOOMBA_BODY, 		//dark-blue
	0x01, //GOOMBA_WHITE,		//dark-brown
	0x20  //GOOMBA_BLACK,		//dark-blue

};

static sprite_t spriteGOOMBA =
      {pgm_imageGOOMBA, pgm_maskGOOMBA, pgm_paletteGOOMBA, NUMPALETTES_GOOMBA};

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

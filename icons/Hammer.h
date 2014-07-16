/* mehPL:
 *    This is Open Source, but NOT GPL. I call it mehPL.
 *    I'm not too fond of long licenses at the top of the file.
 *    Please see the bottom.
 *    Enjoy!
 */












#ifndef __HAMMER_H__
#define __HAMMER_H__

#include "iconPacking.h"
#include "defaultMotion.c"

// This image-data was generated from screenshots from Nintendo's 
// Super Mario Brothers
// Converted for use here using The Gimp -> Save as Header File
// And hand-manipulated into this form for viewability/usability here
// NAH UH!... That message from other sprites just to show how much fun I
// had *entering it by hand* here. WEE!
// (eyeballed from images on the web)
// (Several from, specifically:
// http://www.gameinformer.com/blogs/editors/b/gidan_blog/archive/2011/11/23/my-ten-favorite-mario-power-ups.aspx)
#define HAMMERTOP1 ROWPACK(0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0)
#define HAMMERTOP2 ROWPACK(0,0,0,0,0,0,0,1,2,2,1,0,0,0,0,0)
#define HAMMERA1   ROWPACK(0,0,0,0,0,1,1,1,1,1,1,1,0,1,1,1)
#define HAMMERA2   ROWPACK(0,0,0,1,1,1,3,3,3,3,3,1,1,3,3,3)
#define HAMMERA3   ROWPACK(0,0,1,3,3,3,1,1,1,1,1,1,1,3,1,1)
#define HAMMERA4   ROWPACK(0,1,3,1,1,1,1,1,1,1,1,1,1,1,1,1)
#define HAMMERA5   ROWPACK(0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1)
#define HAMMERA6   ROWPACK(1,1,3,3,1,1,1,1,1,1,1,1,1,1,1,1)
#define HAMMERA7   ROWPACK(1,3,1,1,1,1,1,1,1,1,1,1,0,1,1,1)
#define HAMMERA8   ROWPACK(0,1,1,0,0,0,1,1,1,1,1,1,0,1,1,1)
#define HAMMERA9   ROWPACK(0,0,0,0,0,0,0,1,2,2,1,0,0,0,0,0)
#define HAMMERAA   ROWPACK(0,0,0,0,0,0,0,1,2,2,1,0,0,0,0,0)
#define HAMMERAB   ROWPACK(0,0,0,0,0,0,0,1,2,2,1,0,0,0,0,0)
#define HAMMERAC   ROWPACK(0,0,0,0,0,0,0,1,2,2,1,0,0,0,0,0)
#define HAMMERAD   ROWPACK(0,0,0,0,0,0,0,1,2,2,1,0,0,0,0,0)
#define HAMMERAE   ROWPACK(0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0)


#define HAMMER_NO_TOP	TRUE

#if(defined(HAMMER_NO_TOP) && HAMMER_NO_TOP)
#define HAMMERR0  HAMMERA1
#define HAMMERR1	HAMMERA2
#define HAMMERR2	HAMMERA3
#define HAMMERR3	HAMMERA4
#define HAMMERR4	HAMMERA5
#define HAMMERR5	HAMMERA6
#define HAMMERR6	HAMMERA7
#define HAMMERR7	HAMMERA8
#define HAMMERR8	HAMMERA9
#define HAMMERR9	HAMMERAA
#define HAMMERRA	HAMMERAA
#define HAMMERRB	HAMMERAA
#define HAMMERRC	HAMMERAB
#define HAMMERRD	HAMMERAC
#define HAMMERRE	HAMMERAD
#define HAMMERRF	HAMMERAE
#else	//Normal (according to the web image) with handle showing out the top
#define HAMMERR0  HAMMERTOP1
#define HAMMERR1	HAMMERTOP2
#define HAMMERR2	HAMMERA1
#define HAMMERR3	HAMMERA2
#define HAMMERR4	HAMMERA3
#define HAMMERR5	HAMMERA4
#define HAMMERR6	HAMMERA5
#define HAMMERR7	HAMMERA6
#define HAMMERR8	HAMMERA7
#define HAMMERR9	HAMMERA8
#define HAMMERRA	HAMMERA9
#define HAMMERRB	HAMMERAA
#define HAMMERRC	HAMMERAB
#define HAMMERRD	HAMMERAC
#define HAMMERRE	HAMMERAD
#define HAMMERRF	HAMMERAE
#endif



//Hammer might be the first potential use for a rotate-motion...
// rotate 90deg Clockwise, then back...
// Something to ponder
// (Might even be necessary.. it's hard to recognize since the handle is so
// short due to the screen aspect-ratio...

const static uint8_t pgm_imageHAMMER[1][ICON_PACKED_BYTES] PROGMEM = 
	{ IMAGE_INIT(HAMMER) };



#define pgm_maskHAMMER	NULL
//static uint8_t pgm_maskHAMMER[ICON_MASK_BYTES]; // PROGMEM =
//	MASK_INIT(HAMMER);


#define NUMPALETTES_HAMMER	1

#define BROWN_HAMMER_HANDLE TRUE

//1up Mapping: 0->37 (sky/mask) 1->7, 2->4 3->47
//gimpPixelValToLColor should probably be taken into account.
const static uint8_t pgm_paletteHAMMER[4*NUMPALETTES_HAMMER] PROGMEM = 
	{ 37, //Sky
		0,	//Black outline 
#if(defined(BROWN_HAMMER_HANDLE) && BROWN_HAMMER_HANDLE)
	  rgb2(2,0,0), //Brown stolen from Leaf //unused
#else
	  //According to the image on the 'web, the handle is white...
	  // but I think brown would be more indicative
	  rgb2(3,3,3),
#endif
	  rgb2(3,3,3)  //White
  	};

const __flash sprite_t spriteHAMMER = 
		{ 
			pgm_imageHAMMER, 
			pgm_maskHAMMER, 
			pgm_paletteHAMMER, 
			NUMPALETTES_HAMMER,
			DEFAULT_MOTIONS,
			NadaFlip,
			DefaultMotion,
			DefaultLayer,
			DefaultCamMotion,
			NULL,
			1
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

/* mehPL:
 *    This is Open Source, but NOT GPL. I call it mehPL.
 *    I'm not too fond of long licenses at the top of the file.
 *    Please see the bottom.
 *    Enjoy!
 */




#ifndef __MARIORUNS_H__
#define __MARIORUNS_H__

#include "Mario.h"	//used for the palette

/*static char header_data_cmap[256][3] = {
	{141,157,255},	//sky
	{206,157, 60},	//mustache
	{255, 28, 60}, //hat
	{255,157, 60}, //face/skin
	};
*/
#define NUM_IMAGES_MARIORUNS	3

#define MARIORUNS0R0 ROWPACK(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0)
#define MARIORUNS0R1 ROWPACK(0,0,0,0,0,2,2,2,2,2,0,0,0,0,0,0)
#define MARIORUNS0R2 ROWPACK(0,0,0,0,2,2,2,2,2,2,2,2,2,0,0,0)
#define MARIORUNS0R3 ROWPACK(0,0,0,0,1,1,1,3,3,1,3,0,0,0,0,0)
#define MARIORUNS0R4 ROWPACK(0,0,0,1,3,1,3,3,3,1,3,3,3,0,0,0)
#define MARIORUNS0R5 ROWPACK(0,0,0,1,3,1,1,3,3,3,1,3,3,3,0,0)
#define MARIORUNS0R6 ROWPACK(0,0,0,1,1,3,3,3,3,1,1,1,1,0,0,0)
#define MARIORUNS0R7 ROWPACK(0,0,0,0,0,3,3,3,3,3,3,3,0,0,0,0)
#define MARIORUNS0R8 ROWPACK(0,0,0,0,1,1,1,1,2,1,0,3,0,0,0,0)
#define MARIORUNS0R9 ROWPACK(0,0,0,3,1,1,1,1,1,1,3,3,3,0,0,0)
#define MARIORUNS0RA ROWPACK(0,0,3,3,2,1,1,1,1,1,3,3,0,0,0,0)
#define MARIORUNS0RB ROWPACK(0,0,1,1,2,2,2,2,2,2,2,0,0,0,0,0)
#define MARIORUNS0RC ROWPACK(0,0,1,2,2,2,2,2,2,2,2,0,0,0,0,0)
#define MARIORUNS0RD ROWPACK(0,1,1,2,2,2,0,2,2,2,0,0,0,0,0,0)
#define MARIORUNS0RE ROWPACK(0,1,0,0,0,0,1,1,1,0,0,0,0,0,0,0)
#define MARIORUNS0RF ROWPACK(0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0)

#define MARIORUNS1R0 ROWPACK(0,0,0,0,0,2,2,2,2,2,0,0,0,0,0,0)
#define MARIORUNS1R1 ROWPACK(0,0,0,0,2,2,2,2,2,2,2,2,2,0,0,0)
#define MARIORUNS1R2 ROWPACK(0,0,0,0,1,1,1,3,3,1,3,0,0,0,0,0)
#define MARIORUNS1R3 ROWPACK(0,0,0,1,3,1,3,3,3,1,3,3,3,0,0,0)
#define MARIORUNS1R4 ROWPACK(0,0,0,1,3,1,1,3,3,3,1,3,3,3,0,0)
#define MARIORUNS1R5 ROWPACK(0,0,0,1,1,3,3,3,3,1,1,1,1,0,0,0)
#define MARIORUNS1R6 ROWPACK(0,0,0,0,0,3,3,3,3,3,3,3,0,0,0,0)
#define MARIORUNS1R7 ROWPACK(0,0,1,1,1,1,2,2,1,1,0,0,0,0,0,0)
#define MARIORUNS1R8 ROWPACK(3,3,1,1,1,1,2,2,2,1,1,1,3,3,3,0)
#define MARIORUNS1R9 ROWPACK(3,3,3,0,1,1,2,3,2,2,2,1,1,3,3,0)
#define MARIORUNS1RA ROWPACK(3,3,0,0,2,2,2,2,2,2,2,0,0,1,0,0)
#define MARIORUNS1RB ROWPACK(0,0,0,2,2,2,2,2,2,2,2,2,1,1,0,0)
#define MARIORUNS1RC ROWPACK(0,0,2,2,2,2,2,2,2,2,2,2,1,1,0,0)
#define MARIORUNS1RD ROWPACK(0,1,1,2,2,2,0,0,0,2,2,2,1,1,0,0)
#define MARIORUNS1RE ROWPACK(0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0)
#define MARIORUNS1RF ROWPACK(0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0)

#define MARIORUNS2R0 ROWPACK(0,0,0,0,0,2,2,2,2,2,0,0,0,0,0,0)
#define MARIORUNS2R1 ROWPACK(0,0,0,0,2,2,2,2,2,2,2,2,2,0,0,0)
#define MARIORUNS2R2 ROWPACK(0,0,0,0,1,1,1,3,3,1,3,0,0,0,0,0)
#define MARIORUNS2R3 ROWPACK(0,0,0,1,3,1,3,3,3,1,3,3,3,0,0,0)
#define MARIORUNS2R4 ROWPACK(0,0,0,1,3,1,1,3,3,3,1,3,3,3,0,0)
#define MARIORUNS2R5 ROWPACK(0,0,0,1,1,3,3,3,3,1,1,1,1,0,0,0)
#define MARIORUNS2R6 ROWPACK(0,0,0,0,0,3,3,3,3,3,3,3,0,0,0,0)
#define MARIORUNS2R7 ROWPACK(0,0,0,0,1,1,2,1,1,1,0,0,0,0,0,0)
#define MARIORUNS2R8 ROWPACK(0,0,0,1,1,1,1,2,2,1,1,0,0,0,0,0)
#define MARIORUNS2R9 ROWPACK(0,0,0,1,1,1,2,2,3,2,2,3,0,0,0,0)
#define MARIORUNS2RA ROWPACK(0,0,0,1,1,1,1,2,2,2,2,2,0,0,0,0)
#define MARIORUNS2RB ROWPACK(0,0,0,2,1,1,3,3,3,2,2,2,0,0,0,0)
#define MARIORUNS2RC ROWPACK(0,0,0,0,2,1,3,3,2,2,2,0,0,0,0,0)
#define MARIORUNS2RD ROWPACK(0,0,0,0,0,2,2,2,1,1,1,0,0,0,0,0)
#define MARIORUNS2RE ROWPACK(0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0)
#define MARIORUNS2RF ROWPACK(0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0)


const static uint8_t 
	pgm_imageMARIORUNS[NUM_IMAGES_MARIORUNS][ICON_PACKED_BYTES] PROGMEM =
{
	IMAGE_INIT(MARIORUNS0),
	IMAGE_INIT(MARIORUNS1),
	IMAGE_INIT(MARIORUNS2)
};

#define pgm_maskMARIORUNS	NULL

//MarioRuns starts after Mario is revealed...
// Plausibly, we can use the motion-stuff starting at that point...
#define MARIORUNS_MOTIONS	(DEFAULT_MOTIONS - MARIO_MOTIONS)

const uint8_t MarioRunsX[MOTION_BYTES(MARIORUNS_MOTIONS)] MOTION_MEM =
{
//	PACK_MOTION_BYTE( 0, 0, 0, 0),
//	PACK_MOTION_BYTE( 0, 0, 0, 0),
//	PACK_MOTION_BYTE( 0, 0, 0, 0),
//	PACK_MOTION_BYTE( 0, 0, 0, 0),

	PACK_MOTION_BYTE( 2, 2, 2, 2),
	PACK_MOTION_BYTE( 2, 2, 2, 2),
	
	PACK_MOTION_BYTE( 0, 0, 0, 0),
	PACK_MOTION_BYTE( 0, 0, 0, 0),
	PACK_MOTION_BYTE( 0, 0, 0, 0),
	PACK_MOTION_BYTE( 0, 0, 0, 0)
};

const __flash motion_t MarioRunsMotion[2] = 
	{ {0, MarioRunsX}, {16, &(DefaultY[MOTION_BYTES(MARIO_MOTIONS)])} };

const __flash motion_t MarioRunsCamMotion[2] = 
	{ {0, NULL}, {16, &(DefaultCamY[MOTION_BYTES(MARIO_MOTIONS)])} };


const __flash sprite_t spriteMARIORUNS =
      {
         pgm_imageMARIORUNS,
         pgm_maskMARIORUNS,
         pgm_paletteMARIO,
         NUMPALETTES_MARIO,
         MARIORUNS_MOTIONS,
         NadaFlip,
         MarioRunsMotion, //DefaultMotion,
         &(DefaultLayer[LAYER_BYTES(MARIO_MOTIONS)]),
         MarioRunsCamMotion,
         NULL,
         NUM_IMAGES_MARIORUNS
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
 * /Users/meh/_avrProjects/LCDdirectLVDS/90-reGitting/icons/MarioRuns.h
 *
 *    (Wow, that's a lot longer than I'd hoped).
 *
 *    Enjoy!
 */

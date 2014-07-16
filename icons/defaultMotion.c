/* mehPL:
 *    This is Open Source, but NOT GPL. I call it mehPL.
 *    I'm not too fond of long licenses at the top of the file.
 *    Please see the bottom.
 *    Enjoy!
 */






#ifndef __DEFAULT_MOTION_C__
#define __DEFAULT_MOTION_C__


#include "spriteMotion.h"

//This is used by most sprites


//a/o v84:
// This is the *original* description...
// It should probably be updated for defaultMotion.c
// (Now the camera moves!)

//There're two scrolling-cases
// selected == Q && next == other
// selected == other && (next == Q || next == SOLID)
//
//The first case shows the next sprite's top above the Q-box
// the "window" is on the next sprite, but the Q box overlaps it
// and the Q-box scrolls downward
//      _________
//  ^  |   ___   | \  <--Next
//  |  |  /   \  |  | next sprite appears to scroll up
//  |  ||¯¯¯¯¯¯¯||  | but really the overlapping selected-sprite scrolls
//     || *   * || /  down...
//      |   ?   |
//      | *   * | <--Selected
//       ¯¯¯¯¯¯¯
//
//The second case shows the two sprites atop each other
// the "window" scrolls down the two
//      _________
//     |   ___   | <--Selected
//   ..|../___\..|.. 
// | : |   |_|   | : \  The window scrolls down, but the two sprites
// | : |_________| :  | do not change position relative to each other
// V : |         | : /
//   ..|.*.....*.|..
//     |    ?    |
//     | *     * | <--Next
//      ¯¯¯¯¯¯¯¯¯
// NEW:
//  The first case is kinda boring... it looks like the question-box is
//  being pulled-away to reveal the item, rather than that the item is
//  coming out of the question-box
//  Instead, raise the item from the bottom (in the background) and
//  lower the box... that way both are moving at the same time.
//
//       _____                               ___
//      |     |     _____         __        /___\        //
//      |  ?  | -> |     |  ->  _/__\_  ->   |_|
//      |_____|    |  ?  |     |      |





#define DEFAULT_MOTIONS 40

const uint8_t DefaultY[MOTION_BYTES(DEFAULT_MOTIONS)] MOTION_MEM =
{
   PACK_MOTION_BYTE( 2, 2, 2, 2),
   PACK_MOTION_BYTE( 2, 2, 2, 2),
   PACK_MOTION_BYTE( 2, 2, 2, 2),
   PACK_MOTION_BYTE( 2, 2, 2, 2),


   PACK_MOTION_BYTE( 0, 0, 0, 0 ),
   PACK_MOTION_BYTE( 0, 0, 0, 0 ),
   
	PACK_MOTION_BYTE( 0, 0, 0, 0 ),
   PACK_MOTION_BYTE( 0, 0, 0, 0 ),
   PACK_MOTION_BYTE( 0, 0, 0, 0 ),
   PACK_MOTION_BYTE( 0, 0, 0, 0 ),
};

const __flash motion_t DefaultMotion[2] = { {0, NULL}, {-16, DefaultY} };

//This can still be used for motions up to 64 steps long...
// since the important part is background until the sprite's on top...
//(maybe default doesn't need to be ever foreground, in which case
//NadaLayer would be background...?)
const uint8_t DefaultLayer[LAYER_BYTES(64)] LAYER_MEM =
{
   PACK_LAYER_BYTE(_BG,_BG,_BG,_BG,_BG,_BG,_BG,_BG),
   PACK_LAYER_BYTE(_BG,_BG,_BG,_BG,_BG,_BG,_BG,_BG),

   PACK_LAYER_BYTE(_FG,_FG,_FG,_FG,_FG,_FG,_FG,_FG),
	PACK_LAYER_BYTE(_FG,_FG,_FG,_FG,_FG,_FG,_FG,_FG),
   PACK_LAYER_BYTE(_FG,_FG,_FG,_FG,_FG,_FG,_FG,_FG),
   PACK_LAYER_BYTE(_FG,_FG,_FG,_FG,_FG,_FG,_FG,_FG),
	PACK_LAYER_BYTE(_FG,_FG,_FG,_FG,_FG,_FG,_FG,_FG),
   PACK_LAYER_BYTE(_FG,_FG,_FG,_FG,_FG,_FG,_FG,_FG),
};


const uint8_t DefaultCamY[MOTION_BYTES(DEFAULT_MOTIONS)] MOTION_MEM =
{ 
   PACK_MOTION_BYTE( 1, 1, 1, 1 ),
   PACK_MOTION_BYTE( 1, 1, 1, 1 ),
   PACK_MOTION_BYTE( 1, 1, 1, 1 ),
   PACK_MOTION_BYTE( 1, 1, 1, 1 ),
   
   PACK_MOTION_BYTE( 0, 0, 0, 0 ),
   PACK_MOTION_BYTE( DIR_TOGGLE, 0, 0, 0 ),


   PACK_MOTION_BYTE( 1, 1, 1, 1 ),
   PACK_MOTION_BYTE( 1, 1, 1, 1 ),
   PACK_MOTION_BYTE( 1, 1, 1, 1 ),
   PACK_MOTION_BYTE( 1, 1, 1, 1 ),
};

const __flash motion_t DefaultCamMotion[2] = { {0, NULL}, {0, DefaultCamY} };


#define NadaCamMotion NadaMotion

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
 * /Users/meh/_avrProjects/LCDdirectLVDS/93-checkingProcessAgain/icons/defaultMotion.c
 *
 *    (Wow, that's a lot longer than I'd hoped).
 *
 *    Enjoy!
 */

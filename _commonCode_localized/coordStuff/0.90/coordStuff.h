/* mehPL:
 *    This is Open Source, but NOT GPL. I call it mehPL.
 *    I'm not too fond of long licenses at the top of the file.
 *    Please see the bottom.
 *    Enjoy!
 */




// coordStuff.h 0.90-2
// 	coord_t (int32_t), and related functions...
// 0.90-2 adding _UNUSEDs for threePinIDer...
//        coord_max and coord_sign are unused by xyTracker...
// 0.90-1 adding coord_limit()... and COORD_MIN/MAX
// 0.90 First Version (coord_t was previously defined in xyTracker...


#ifndef __COORDSTUFF_H__
#define __COORDSTUFF_H__

#include <inttypes.h>

//Since coordStuff is seldom (if ever?) used without xyTracker...
// we'll assign max and min values according to xyTracker's...
// However, if xyTracker is not used, these could be INT32_MAX, etc...
// Just be sure to watch out for weird cases... 
//     such as -(INT32_MIN) = INT32_MIN
//   And be sure to use int64_t when multiplying, etc.
// XYT redefines these intentionally
typedef int32_t coord_t;
//This is from the XYT header...
#define XYT_MAX (((uint32_t)1<<30)-1)

#define COORD_MAX XYT_MAX
#define COORD_MIN (-XYT_MAX)

typedef uint32_t ucoord_t;

//Returns values in the range [0,INT32_MAX]
// If value = INT32_MIN it will return INT32_MAX
// ( IRL:  | INT32_MIN | = INT32_MAX + 1 > INT32_MAX )
// Realistically, we'll seldom use values like this
//   (especially with risk of wraparound)
//   So maybe this is bloatware...
//   @@@ Maybe a #define to disable this to save code?
//     In which case coord_abs could just be 
//     #define coord_abs(val) ( (val < 0) ? -val : val)
//     (would run faster...)
//     What about:
//     #define coord_abs(val) 
//       ( (val < -(INT32_MAX)) ? INT32_MAX : ((val < 0) ? -val : val ) ) 
coord_t coord_abs(coord_t value);

#if (!defined(COORD_MAX_UNUSED) || !COORD_MAX_UNUSED)
//This could be inline... or even #define, it's so simple...
coord_t coord_max(coord_t value1, coord_t value2);
#endif

#if (!defined(COORD_SIGN_UNUSED) || !COORD_SIGN_UNUSED)
//Returns -1, 0, 1 depending on the sign or zero-ness
//This could be inline or #define as well
int8_t coord_sign(coord_t value);
#endif

int8_t coord_limit(coord_t min, coord_t *val, coord_t max);
int8_t ucoord_limit(ucoord_t min, ucoord_t *val, ucoord_t max);

//#define coord_limit(min, val, max) 
//	(((val)<(min)) ? min : (((val)>(max)) ? max : val))

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
 * /Users/meh/_avrProjects/LCDdirectLVDS/68-backToLTN/_commonCode_localized/coordStuff/0.90/coordStuff.h
 *
 *    (Wow, that's a lot longer than I'd hoped).
 *
 *    Enjoy!
 */

/* mehPL:
 *    This is Open Source, but NOT GPL. I call it mehPL.
 *    I'm not too fond of long licenses at the top of the file.
 *    Please see the bottom.
 *    Enjoy!
 */




//xyTracker 3.05-2
// NOTE: run=0 is replaced with run=1. 
//       rise-sign is determined by rise EXCLUSIVELY
//         because xyt_next() implies *time* which is always increasing
//         (thus for rise/run=y/x xyt_next() should be followed by x+=xDir)
//         (run is now unsigned)
//         signed values for run are abs()'d but do not change outsign
//       run has limits abs(run) <= INT32_MAX for certain (TBD)
//       LIMITS:
//         rise is 30bits: RISEMAX = 2^30-1 = 1,073,741,823
//              after dropping the sign... (RISEMIN = -RISEMAX)
//              so unmolested input values are RISEMIN >= rise >= RISEMAX
//         run is 31bits: RUNMAX = 2^31-1 = 2,147,483,647
//              run=0 is replaced with run=1 in order to avoid while-lockup
//              so unmolested input values are RUNMIN >= run >= RUNMAX
//       See 0.93 notes and notes in xyt_init/next code.
//
// NOTE!!!
//   addition of coordStuff (mainly?) increases code size by 300+ bytes!
//3.05-2 Adding this note: 
//       TODO: hfm has been modified to remove the need for runningSum
//             The same could probably be done here
//             Further: It could reduce necessity for huge integers...
//             Further: It could handle signedness without outSign... (?)
//3.05-1 nextOutput now a macro... might as well be inlined
//3.05 when rise > run we know the base increment...
//     e.g. rise = 17, run = 5
//     17/5 = 3 (integer)
//     no steps will be smaller than 3
//     Apparently a significant speed improvement (at the cost of size...
//      nearly 200B)
//     was 28000 cyc/sec (with other code) now 31600 cyc/sec
//      (with xyTracker being used in dmsUpdate in Timer0 at 9600Hz)
//3.00-1 attempting to shrink code-size for threePinIDer47i
//       a failed attempt at creating _USED makefile options
//       ended up in 47iUSED...
//3.00 Working with gotoPoxn3, etc... 
//     replacing xyt_nextOutput with xyt_update (returns recent change)
//     Making run sign-stuff and max more... intuitive
//     removing all signs of signedness in run.
//     Maximum/Minimum values...
//.93-1 NOTES Re: 
//      RUN CANNOT EXCEED (not certain this is the max) INT32_MAX
//         due to coord_t in xyt_init
//      THERE MIGHT BE OTHER LIMITS
//      ALSO: run must always be positive!!! (since outsign is not changed
//         when negative)
//.93 using coordStuff now for coord_t
//    HACK: run=0 would cause a stall in the while-loop
//    ASSUMED: run=0 means increment to endpoint immediately (next update)
//      e.g. run = 1
//    I can't imagine a case where I'd WANT it to stall...
//.92 rise > run NOW OK (but ONLY SLIGHTLY slower)
//    using while() instead of if()
//    For motions, however, it should probably still be rise<run
//    (since the motor has to keep up... then again...
//     near should take care of that for SMALL abs(rise-run))
//.91-7 runningSum remainder
//.91-6 notes re: rise vs. run
//.91-5 clearing "Anonymous Struct" error with g++ (working with glTest)
//.91-4 cleaning up comparisons between signed and unsigned...
//Doh//.91-3 fix abs mistake in xytInit... (VERY unlikely, but let's fix it)
//#warning "xyT.91 has no necessary changes?!"
//#warning "no, output is always same-sign as rise since run is always /incremented/ once, 
//actual value is inc/decremented externally... x(t), y(t) rather than y(x)..."
//.91-2 minor cleanup, annoying warning removal
//.91-1 dividend/divisor -> rise/run
//.91 runningSum addition error fix (secondary-axis direction fix) 
//.90 stolen from hfModulation 0.92.2
//---------
//.92 added makefile, updated bithandling...
//.91cf moved to _common from MotorDriver11
//.91 stepNum implemented
//.90 Stolen from heartbeat .95

#ifndef __XYTRACKER_H__
#define __XYTRACKER_H__

#include <inttypes.h>
#include _COORDSTUFF_HEADER_


/*	basically:
	given m (deltaY/deltaX), the output (y) is incremented/decremented 
	as appropriate given a trigger to update...
	
	e.g.
	deltaY = 5
	deltaX = 7
	
	first update:
	runningSum = 5
	run = 7
	5 < 7
	out = 0
	
	second:
	rS += 5 // = 10
	run = 7
	10 > 7
	rS -= 7	// = 10-7 = 3
?	(is there any case where 7 might go into rS twice?)
	out++	// = 1

	third:
	rS += 5 // = 3+5 = 8
	8>7
	rS -= 7	//	8-7 = 1
	out++	// = 2
	
	fourth:
	rS += 5	// = 1+5 = 6
	6 < 7
	out		// = 2 no change
	
	fifth:
	rS += 5	// = 6+5 = 11
	11 > 7
	rS -= 7	// 11-7=4
	out++	// = 3
	
	sixth:
	rS += 5	// = 4+5 = 9
	9 > 7
	rS -= 7	// = 9-7 = 2
	out++	// = 4
	
	seventh:
	rS += 5	// = 2+5 = 7
	7 = 7
	rS -= 7	// = 7-7 = 0
	//Don't stop just because rS = 0, what if 4/8?...
	out++	// = 5
	
	
	in general:
	update:
	runningSum += rise;
	if(runningSum >= run)
		runningSum -= run;
		output += outSign;
		
*/

//"Anonymous structs" are not allowed in g++
// This isn't necessary for gcc, but it shouldn't hurt
// ("_XYTSTRUCTRECORD" won't be used anywhere)
typedef volatile struct _XYTSTRUCTRECORD
{
	ucoord_t runningSum;
	coord_t incrementBase; // for rise>run this is floor(rise/run)
	//rise and run are unsigned for efficiency since they're initted in init
	// Also to assure no overflows in math
	ucoord_t rise;		// 30bits, max
	// RISE IS NOW the rise-difference 
	// e.g. riseSetup=17, run=5
	// rise(Here) = 2 (incrementBase=floor(17/5)=3 riseHere=riseSetup-3*5
	ucoord_t run;		// 31bits, max
	int8_t outSign;         // sign of inc/decrement of the output
	coord_t output;			// current output of the division process...
}  xyt_t;


//This value (+/-) is safe to use for both rise and run
// xyt_init limits inputs safely, 
//   but could destroy math/motion that called it
#define XYT_MAX (((uint32_t)1<<30)-1)

#define XYT_RISE_MAX XYT_MAX
#define XYT_RISE_MIN (-XYT_MAX)

#define XYT_RUN_MAX (((uint32_t)1<<31)-1)
#define XYT_RUN_MIN 1

//Intentional redefinitions:
#define COORD_MAX XYT_MAX
#define COORD_MIN (-XYT_MAX)

// Rise is signed... Run is UNSIGNED (see notes and definitions above)
// out-of-range values are limited as above and 0x01 is ored with 0x02
//  to indicate run or rise limited, respectively
uint8_t xyt_init(xyt_t *line, coord_t rise, ucoord_t run);

//Calculates the next output...
// Returns the dY value (the amount of change in this call)
coord_t xyt_update(xyt_t *line);


static __inline__ \
coord_t xyt_nextOutput(xyt_t *line) 
									__attribute__((__always_inline__));

__inline__ \
coord_t xyt_nextOutput(xyt_t *line)
{
	xyt_update(line);
	return line->output;
}



/*
//LEGACY: Consider using xyt_update instead...
// Calculates the next output...
//Returns the DY value, if given DX calls to nextOutput
// IOW: Returns the change in rise (line.output) since xyt_init
// (line.output could just be read directly after xyt_update,
//   which is why this is legacy)
coord_t xyt_nextOutput(xyt_t *line);
*/

/*
//Calculates the next output...
// Returns the dY value (the amount of change in this call)
coord_t xyt_update(xyt_t *line);
*/

#if (!defined(XYT_REMAINDER_UNUSED) || !XYT_REMAINDER_UNUSED)
//Returns the remainder (runningSum) from the last call of nextOutput/update
// thus, a decimal fraction would be remainder*100/run
coord_t xyt_remainder(xyt_t *line);
#endif

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
 * /Users/meh/_avrProjects/LCDdirectLVDS/68-backToLTN/_commonCode_localized/xyTracker/3.05/xyTracker.h
 *
 *    (Wow, that's a lot longer than I'd hoped).
 *
 *    Enjoy!
 */

/* mehPL:
 *    This is Open Source, but NOT GPL. I call it mehPL.
 *    I'm not too fond of long licenses at the top of the file.
 *    Please see the bottom.
 *    Enjoy!
 */




//xyTracker 3.00

#include "xyTracker.h"
//#include "../../bithandling/0.93/bithandling.h"

// abs(rise) <= XYT_RISE_MAX
//  1 <= run <= XYT_RUN_MAX
// Returns:
//    0 if rise and run values are valid
//    Or the OR of the following:
//    0x01 if run was limited
//    0x02 if rise was limited
uint8_t xyt_init(xyt_t *line, coord_t rise, ucoord_t run)
{
	uint8_t retVal = 0;

	//Track the sign of the rise (berfore making it unsigned)
	if(rise < 0)
		line->outSign = -1;
	else
		line->outSign = +1;


	//Check if the rise is out of range and fit it...
	// (still signed at this point)
	if(coord_limit(XYT_RISE_MIN, &rise, XYT_RISE_MAX))
		retVal |= 0x02;

	//Now make the rise positive (handled later)
	//rise = coord_abs(rise);


	// Don't allow run==0, or nextOutput will stall
	// This works well for xyT where y=TIME since infinite incrementing makes
	// no sense... and nor does a zero-change in time.
	// xyT working directly with x and y requires special handling
	// e.g. when drawing lines... 
	// Part of the point of xyT is to traverse every point in between
	//  This is no longer a *requirement* with multiStepping
	//   but it would draw dotted-lines in that case...
	//   IOW for drawing/traversing functions it makes the most sense to have
	//   rise/run <= 1
	//   (for DC Servos where run == TIME, rise/run > 1 is acceptable, 
	//    but it might make more sense to decrease the encoder resolution 
	//    and/or increase time resolution in the calling function...)
//	if (run == 0)
//		run = 1;

	if(ucoord_limit(XYT_RUN_MIN, &run, XYT_RUN_MAX))
		retVal |= 0x01;

//	if(coord_abs(rise) >= coord_abs(run))
//	{
		coord_t incBase = rise/run;
		//signed
		line->incrementBase = incBase;
		//line->additionalRise = line->incrementBase * run;
		line->rise = coord_abs(rise) - coord_abs(incBase * run);
//	}
//	else
//	{
//		line->incrementBase = 0;
//		line->rise = 
//	}
	line->run = run;	//unsigned 31bits (32nd used in runningSum!)
//	line->rise = coord_abs(rise); //Also unsigned 30bit
	line->runningSum = 0;
	line->output = 0;
	
	return retVal;
}


// Returns the amount of change during this update...
// the old: coord_t xyt_nextOutput(xyt_t *line) only slightly modified
coord_t xyt_update(xyt_t *line)
{
	coord_t thisChange = line->incrementBase; //0
	//assuming the first time this is called dX = 1 (not 0 since those coords are already known)

	//line.runningSum should not overflow, otherwise we'll be stuck in the
	// while loop forever...
	// rise is one bit smaller than run, so worst-case-scenario:
	// run=RUNMAX rise=RISEMAX:
	//  in 8-bit variables:
	//  run=7bit RUNMAX = 127
	//  rise=6bit RISEMAX = 63
	//  runningSum = 0->63->126->189 (thus necessitating riseBits<=varBits-1)
	// In 32bit (unsigned): run is 31bits, rise is 30bits
	// then RISEMAX = 2^30-1 = 1,073,741,823
	// Seems like plenty for most coordinate-systems...
	// There *might* be a way to do this with integer-overflow
	//  but I can't determine how off-hand.
	line->runningSum += line->rise;

	//This used to be "if" but rise>run requires "while"	
	// What a SIMPLE change!
	while(line->runningSum >= line->run)
	{
		line->runningSum -= line->run;
		//line->output += line->outSign;
      thisChange += line->outSign;
	}

	line->output += thisChange;
	return thisChange;
//	return line->output;
}

/*
coord_t xyt_nextOutput(xyt_t *line)
{
	xyt_update(line);
	return line->output;
}
*/

#if (!defined(XYT_REMAINDER_UNUSED) || !XYT_REMAINDER_UNUSED)
coord_t xyt_remainder(xyt_t *line)
{
	return line->runningSum;
}
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
 * /Users/meh/_avrProjects/LCDdirectLVDS/68-backToLTN/_commonCode_localized/xyTracker/3.05/xyTracker.c
 *
 *    (Wow, that's a lot longer than I'd hoped).
 *
 *    Enjoy!
 */

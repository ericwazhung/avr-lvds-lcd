/* mehPL:
 *    This is Open Source, but NOT GPL. I call it mehPL.
 *    I'm not too fond of long licenses at the top of the file.
 *    Please see the bottom.
 *    Enjoy!
 */


//tcnter 0.10
#ifdef _TCNTER_HEADER_
 #include _TCNTER_HEADER_
#else
 #include "tcnter.h"
#endif



#if (defined(PU_PC_DEBUG) && PU_PC_DEBUG)
	#include <stdio.h>
	#define DPRINT	printf
#else
	#define DPRINT(...) {}
#endif

#if (defined(TCNTER_SOURCE_EXTERNED) && TCNTER_SOURCE_EXTERNED)
	extern tcnter_source_t	TCNTER_SOURCE_VAR;
#endif


#if (defined(TCNTER_INITS_TIMER) && TCNTER_INITS_TIMER)
 #ifndef TCNTER_SOURCE_VAR
  #ifdef __AVR_AT90PWM161__
   #define TCNTER_SOURCE_VAR	TCNT1
  #elif (defined(TCNT0))
   #define TCNTER_SOURCE_VAR	TCNT0
  #elif(defined(TCNT0L))
   #define TCNTER_SOURCE_VAR	TCNT0L
  #else
   #error "Gonna have to set TCNTER_SOURCE_VAR explicitly"
  #endif
 #endif

 #ifndef TCNTER_SOURCE_OVERFLOW_VAL
  #ifdef __AVR_AT90PWM161__
   #define TCNTER_SOURCE_OVERFLOW_VAL	0xffff
  #else
   //Assuming an 8-bit timer...
   #define TCNTER_SOURCE_OVERFLOW_VAL	0xff
  #endif
 #endif

 #ifndef TCNTER_AVRTIMER_NUM
  #ifdef __AVR_AT90PWM161__
   #define TCNTER_AVRTIMER_NUM	1
  #else
   #define TCNTER_AVRTIMER_NUM	0
  #endif
 #endif
 //This moved to tcnter.h
/*
 #ifndef TCNTER_AVRTIMER_CLKDIV
  //This choice is arbitrary...
  // According to timerCommon, it exists in most typical systems
  // The only exception I've run into so far is AT90PWM161, which ONLY has
  // CLKDIV1, which wouldn't work too well since tcnter_update() would have
  // to run every 255 clock cycles (or 65535?)
  // LCDdirectLVDS69 uses the timer for dms... so it must work, ish.
  #define TCNTER_AVRTIMER_CLKDIV CLKDIV64
 #endif
*/
#endif

//DON'T CHANGE THIS WITHOUT CHANGING both!
//#define tcnter_source_t	uint8_t
//#define tcnter_compare_t	int16_t
//#define tcnter_t	uint32_t

//Running TCNTs since last init
volatile tcnter_t tcnter_myTcnter = 0;
//Last TCNT value read in update()
// this is not used, except internally
// (global for reset on init)
tcnter_source_t tcnter_lastTcnt = 0;
// Before inlining functions:
//INLINING EXPERIMENTS: INLINE=FALSE 4098B INLINE=TRUE 4240B
//   after all inlining, it's still bigger than without inlining!
//Inlined: Saved 24B (4216)	
TCNTER_INLINEABLE
void tcnter_init(void)
{
#if (defined(TCNTER_INITS_TIMER) && TCNTER_INITS_TIMER)
	//What if the timer is 16bit (do they default as such?)
	timer_setWGM(TCNTER_AVRTIMER_NUM, WGM_NORMAL); //Necessary?
	timer_selectDivisor(TCNTER_AVRTIMER_NUM, TCNTER_AVRTIMER_CLKDIV);
#endif

	tcnter_lastTcnt = TCNTER_SOURCE_VAR;
	tcnter_myTcnter = 0;

	DPRINT("tcnter_init()\n");
}


void tcnter_wait(myTcnter_t tcnts)
{
	myTcnter_t startTime = tcnter_get();

	while(!tcnter_isItTime(&startTime, tcnts))
	{
		tcnter_update();
		//asm("nop;");
	}
}


#if(defined(TCNT_UPDATE_ONCE_PER_OVERFLOW) && \
		TCNT_UPDATE_ONCE_PER_OVERFLOW)
//65536 calls should be plenty, right...?
// what if we're in a tcnter_wait() function, and the overflow-value is
// large?
// There's an easy-ish way to check this, right...?
// e.g. assume no more than one update can be called every 20 clock cycles
// (just an estimate) Then determine how long an overflow is in terms of
// clock-cycles... 
// then we could #if(MAXUPDATES < 65535) use uint16... etc.
uint32_t tcnter_updatesSinceOverflowUpdate = 0;
// This is the tcnt value at the time of the last overflow...
tcnter_source_t tcnter_lastOverflowTcnt = 0;

//If called, e.g. in the timer's overflow-interrupt, then tcnter should
//advance by TCNTER_SOURCE_OVERFLOW_VAL each time...
// Of course, the actual call might occur a few tcnts after the interrupt
// (and after the actual overflow)... and therefore could be called at
// slightly different (+/-) times WRT the actual overflow...
TCNTER_INLINEABLE
void tcnter_overflowUpdate(void)
{
	tcnter_source_t thisTcnt = TCNTER_SOURCE_VAR;

	//This will be calculated before being added to myTcnter...
	tcnter_compare_t deltaTcnt;


	//If tcnter_update() has been called inbetween, just treat this like a
	//normal tcnter_update()	
	if(tcnter_updatesSinceOverflowUpdate)
	{
		//wrap-around can be handled here...
		// (This shouldn't happen, right? Since overflowUpdate is called?)
		if(thisTcnt < tcnter_lastTcnt)
			deltaTcnt = (tcnter_compare_t)(TCNTER_SOURCE_OVERFLOW_VAL);
		else
			deltaTcnt = 0;

		deltaTcnt += (tcnter_compare_t)thisTcnt
						- (tcnter_compare_t)tcnter_lastTcnt;
	}
	else
	{
		//Any error will be subtracted later...
		deltaTcnt = TCNTER_SOURCE_OVERFLOW_VAL;

		//This should probably be small, e.g. +-1 WRT the last overflowUpdate
		// since it likely occurs in the overflow interrupt.
		deltaTcnt += (tcnter_compare_t)thisTcnt
						- (tcnter_compare_t)tcnter_lastOverflowTcnt;

	}


	//Add the delta to myTcnter
	tcnter_myTcnter += deltaTcnt;
	
	//And prep for the next call...
	tcnter_updatesSinceOverflowUpdate = 0;
	tcnter_lastTcnt = thisTcnt;
	tcnter_lastOverflowTcnt = thisTcnt;
}
#endif



//#warning "Changing TCNT for this purpose makes the timer unusable for more than one UAR/T or other device at a time!"
// What about a tcnt DIFFERENCE, instead?
// How difficult would this be with wraparound and/or resets at arbitary
// values?
// Can we use the other devices' (e.g. DMS) OCR values?
// What about an xyTracker for more precision and less cumulative error?

//Inlined: WTF?! 4240->4880B! (used twice... once on init)
//  Also seems to cause the main loop to not function
//  (how could it take *more* instructions?!)
//   trying to shuffle registers?
//TCNTER_INLINEABLE
void tcnter_update(void)
{
	tcnter_source_t thisTcnt = TCNTER_SOURCE_VAR;	//e.g. TCNT0
	
	tcnter_compare_t deltaTcnt = (tcnter_compare_t)thisTcnt 
										- (tcnter_compare_t)tcnter_lastTcnt;

	
#if(defined(TCNT_UPDATE_ONCE_PER_OVERFLOW) && \
		TCNT_UPDATE_ONCE_PER_OVERFLOW)
	//The odd-case being if e.g. overflowUpdate is called at TCNT=1
	// and update() is called after, at TCNT=1
	// update won't increment, and the updatesSinceOverflow, if allowed to
	// be incremented, would indicate to overflowUpdate that it should treat
	// the next overflow as a normal update, which may well occur at, again,
	// TCNT=1, which would result in a non-advancement at all.
	if(thisTcnt == tcnter_lastOverflowTcnt)
		return;

	tcnter_updatesSinceOverflowUpdate++;
#endif
	
	// Handle wrap-around...
	if (thisTcnt < tcnter_lastTcnt)
		deltaTcnt += (tcnter_compare_t)(TCNTER_SOURCE_OVERFLOW_VAL);

	tcnter_lastTcnt = thisTcnt;

	tcnter_myTcnter += (tcnter_t)deltaTcnt;
}



//Inlined: Saved 40B
TCNTER_INLINEABLE
tcnter_t tcnter_get(void)
{
	return tcnter_myTcnter;
}


//This is a recurring dilemma... dms6sec_t had issues as well
// Should create such a function there, as well..
// e.g. 
//thisTime Wraps at 0xff... (8bit)
// nextTime is in 16 units (timeDiff) (nextTime=0x00)
// thisTime is at 0xf0
// then nextTime is met when thisTime = 0x00
//Can't say if(thisTime > nextTime) (obviously)
// but could do (thisTime - timeDiff) > startTime) ???
// or maybe even (startTime + timeDiff > thisTime) ???
// or (thisTime - startTime > timeDiff) ???
//  ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ ---- GOOD
//  Wrapping Math handles this case...
//  Additionally, nice(?), no calculation necessary for nextTime... 
// (thisTime > nextTime)
// *** StartTime is updated so that no cumulative error occurs...
// THE WHOLE POINT OF THIS IS TO REDUCE INSTRUCTIONS....
//  The old method (thisTime > nextTime)
//   resulted in sometimes 12 instructions (4=readThisTime, 4=readNextTime
//    4=compare) Then 4=calcNextTime...

#if 0  //These are now in the header-file, as they're always inline...	
TCNTER_INLINEABLE
uint8_t tcnter_isItTime8(tcnter8_t *startTime, tcnter8_t deltaTime)
{
	tcnter8_t thisDelta = (uint8_t)tcnter_myTcnter - (uint8_t)(*startTime);
	
	if(thisDelta >= deltaTime)
	{
		//Say thisDelta = 5, deltaTime = 4
		// that means we're 1 after the desired time
		//  (thisDelta - deltaTime) = 1
		// So set startTime (for the next go-round) to thisTime - 1
		//  (to hopefully eliminate cumulative error)

		// Could this math be combined with >= above?
		*startTime = (uint8_t)tcnter_myTcnter - (thisDelta - deltaTime);
		return TRUE;
	}
	else
		return FALSE;
// return (((uint8_t)tcnter_myTcnter - (uint8_t)startTime) > (uint8_t)deltaTime);
}


TCNTER_INLINEABLE
uint8_t tcnter_isItTime(tcnter_t *startTime, tcnter_t deltaTime)
{
	tcnter_t thisDelta = tcnter_myTcnter - *startTime;

	if(thisDelta >= deltaTime)
	{
		*startTime = tcnter_myTcnter - (thisDelta - deltaTime);
		return TRUE;
	}
	else
		return FALSE;
}
#endif

/* poll_uar notes, could be useful here...?
	Timing Considerations:
		TCNT increments:
			near bit-rate (e.g. 1.5 TCNTS = 1 bit)
				high bit-rate
					Risky, updates might not be often enough regardless of xyt
				slow counter
					NEEDs xyt
			good (e.g. 52.9 TCNTS = 1 bit)
				since .9 is dropped from calculations we have 52 TCNTS/bit
				after 10 bits (one frame) this is only 9 TCNTS away from center
					or 9/52 = ~.2 bits away from center
			   	(and next Start-edge should realign again)
				Fast Counter
					multiple tcnts per update (possibly)
						HEREIN LIES THE PROBLEM:
							How to track sample times...?
								tcnt likely to overflow several times per byte
									running tcnt?
				
				xyt would probably be aceptable
				  though it's increasingly likely that each update will have
				  multiple tcnts (and xyt will need to update multiple times)
			far/risky (e.g. 250 TCNTS = 1 bit)
				math gets difficult due to constant wraparound
				Fast Counter
					xyt would be extremely difficult to keep-up
						then again, much less necessary; high-precision

*/

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
 * /Users/meh/_avrProjects/LCDdirectLVDS/90-reGitting/_commonCode_localized/tcnter/0.30/tcnter.c
 *
 *    (Wow, that's a lot longer than I'd hoped).
 *
 *    Enjoy!
 */

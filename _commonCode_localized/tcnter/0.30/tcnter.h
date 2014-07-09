/* mehPL:
 *    This is Open Source, but NOT GPL. I call it mehPL.
 *    I'm not too fond of long licenses at the top of the file.
 *    Please see the bottom.
 *    Enjoy!
 */


//tcnter 0.30-6
// Similar functionality to dmsTimer
//   Doesn't require interrupts
//   MUCH higher resolution-potential (e.g. for bit-rate generation)
//      limited only by the tcnter_update() call rate(?)
// TODO: Make some, if not all, functions inline.
//       CONSIDER: if timing is highly critical, it may make sense
//                 to call update() with every get()
//                 THOUGH: this would add a lot of calculations...
//       CONSIDER2: could myTcnter_t be smaller, and use wraparound math?
//                 e.g.  if(get() >= next) NOGO ? next=3, get=254
//                 e.g.2 if(get() - next >= 0) ... same
//                  wasn't there a way to do this?
//                       if(get() - last >= next - last) ?
//                             next=3, get=254, last=253
//                          get()-last = 254-253 = 1
//                          next-last = 3-253 = 6
//                  Promotion issues may occur, watch casts.
//                  See cTools/unsignedSubtraction.c
//                             next=5, get=7, last=6
//					Create "wrappableTimeCompare()" macro?
//             Also usable in dms, etc...
//0.30-6 adding TCNT_UPDATE_ONCE_PER_OVERFLOW:
//			and tcnter_overflowUpdate()
//       ...
//       LCDdirectLVDS77 attempts to call tcnter_update() only once per
//			timer-overflow (in the interrupt)
//       So it's quite likely that each tcnter_update() occurs at or around
//       the same TCNT value. (which might explain why the heart isn't
//       fading).
//0.30-5 Checking if TCNTER_MS is <= 0
//0.30-4 TCNTER_INITS_TIMER is configured by default IF timerCommon is not
//			included in the makefile before tcnter is...
//       This is a bit hokey... (what if timerCommon is included *after*?)
//       TODO: Figure this out.
//0.30-3 PWM161 wasn't handled...
//0.30-2 8V2, not V28
//       Adding tcnter_wait(tcnts)
//0.30-1 moving TCNTER_AVRTIMER_CLKDIV to tcnter.h so it can be used by
//			e.g. heartbeat to calculate timings...
//0.30  tcnter on an AVR will most-likely use a timer, e.g. timer0
//      so should set some defaults, not have to explicitly include
//      timerCommon in the project's makefile, etc.
//0.21  per dmsTimer1.14, a/o heartbeat1.50...
//      isItTime functions have an option to NOT worry about
//      cumulative-error...
//      Also, they've been made inline, so now included in the header-file
//0.20-10 Replacing tests' makefiles with those that refer to tcnter
//        properly... a/o/ala anaButtons 0.50
//0.20-9 adding this TODO: Apparently math doesn't work with
//			tcnter_source_t = uint16_t???
//       And looking into it...
//0.20-8 a/o polled_uar/0.40/avrTest...
//       adding some Usage notes...
//0.20-7 adding isItTime (non 8-bit)
//0.20-6 adding to COM_HEADERS when INLINED
//0.20-5 Renaming Globals to tcnter_*... nastiness
//       see puar notes...
//0.20-4 tcnter8_t -> tcnter_isItTime8(startTime, delta)
//       HANDY: When inlined, if(tcnter_isItTime()) results in
//       an if statement from inside tcnter_isItTime, which is used
//       *in place of* the if surrounding tcnter_isItTime()
//       Inlining optimization is great!
//0.20-3 myTcnter_t is stupid -> tcnter_t
//0.20-2 adding _SPECIALHEADER_FOR_TCNTER_
//       e.g. TCNTER OVERFLOW VAL = _DMS_OCR_VAL_ requires dmsTimer.h
//       so CFLAGS+=-D'_SPECIALHEADER_FOR_TCNTER_=_DMSTIMER_HEADER_'
//0.20-1 inlining...
//				SEE NOTES in .c before using inlining
//          and experiment and stuff
//0.20 TCNTER_SOURCE_MAX renamed to TCNTER_SOURCE_OVERFLOW_VAL
//0.10-3 fixing names in test (to match 0.10-1 changes)
//       experimenting with test
//0.10-2 Need <avr/io.h> when TCNT, etc. is used...
//0.10-1 (forgot to backup first version?)
//       renaming all tcnt and whatnot to tcnter
//0.10 First version, stolen and modified from polled_uar 0.10-6
//
//---------
//polled_uar 0.10-6:
//   Doesn't use interrupts (no lag-times, etc.)
//0.10-4 
//       Looking into running-tcnt
//				myTcnter and nextTcnter now implemented
//          Fixes potential issues with multi-TCNTs per update
//            Aiding cumulative-error fixing
//            (Next time was dependent on the time of the current update)
//0.10-3 test app using makefile...
//			cleanup
//0.10-1 More mods, test app
//0.10 First Version stolen and modified heavily from usi_uart 0.22-3

#ifndef __TCNTER_H__
#define __TCNTER_H__
//This shouldn't be necessary... (or its necessity removed soon)
//#include <avr/io.h>
#include <stdint.h>

#ifdef __AVR_ARCH__
 //This is only necessary if TCNT0 (etc.) is used
 // according to the project makefile
 #include <avr/io.h>
#endif

//This is for, e.g. where TCNTER_SOURCE_OVERFLOW_VAL = _DMS_OCR_VAL_
// Then tcnter needs to be aware of the definition of _DMS_OCR_VAL_
#ifdef _SPECIALHEADER_FOR_TCNTER_
 #include _SPECIALHEADER_FOR_TCNTER_
#endif


#if (defined(TCNTER_INITS_TIMER) && TCNTER_INITS_TIMER)
 #include _TIMERCOMMON_HEADER_

 //Moved here so it can be referenced elsewhere for determining timings...
 #ifndef TCNTER_AVRTIMER_CLKDIV
  #ifdef __AVR_AT90PWM161__
	// This is the only option on the PWM161
   #define TCNTER_AVRTIMER_CLKDIV CLKDIV1
  #else
   //This choice is arbitrary...
   // According to timerCommon, it exists in most typical systems
   // The only exception I've run into so far is AT90PWM161, which ONLY has
   // CLKDIV1, which wouldn't work too well since tcnter_update() would
   // have to run every 255 clock cycles (or 65535?)
   // LCDdirectLVDS69 uses the timer for dms... so it must work, ish.
   #define TCNTER_AVRTIMER_CLKDIV CLKDIV64
  #endif
 #endif

 //These can be used by other code...
 #define TCNTS_PER_SECOND  (F_CPU/(1<<TCNTER_AVRTIMER_CLKDIV))
 //At FCPU = 16MHz, TCNTER_MS is 250...
#else
 #warning "TCNTER_INITS_TIMER is NOT TRUE. This can happen a number of ways, the weirdest being that timerCommon is included before tcnter. A reasonable test, as long as it's not included *after* tcnter... so it should be better-implemented."
 #warning "If you get weird errors, did you check that TCNTER_SEC, etc. have been set for your project...? And probably some other things... see the PC-based tcnter Test code for the necessary variables for an external source"
#warning "Check: TCNTER_SOURCE_VAR, TCNTER_SOURCE_OVERFLOW_VAL, and TCNTS_PER_SECOND, also _SPECIALHEADER_FOR_TCNTER_"
#endif


#if((TCNTS_PER_SECOND/1000) > 0)
 #define TCNTS_PER_MS		(TCNTS_PER_SECOND/1000)
 #define TCNTER_MS			(TCNTS_PER_MS)
#else
 #warning "TCNTS_PER_MS <= 0, so TCNTER_MS is disabled"
#endif

#define TCNTER_SEC			(TCNTS_PER_SECOND)

#if((TCNTS_PER_SECOND/10000) > 0)
 #define TCNTER_DMS			(TCNTS_PER_SECOND/10000)
#else
 #warning "TCNTER_DMS <= 0, so disabled"
#endif

/*
// This'd be nice to change...
#if (!defined(PU_PC_DEBUG) || !PU_PC_DEBUG)
// #include "../../bithandling/0.94/bithandling.h"
#else
 #include <stdio.h>
#endif
*/

//DON'T CHANGE THIS WITHOUT CHANGING both!
#if ( (!defined(tcnter_source_t) && defined(tcnter_compare_t)) \
    ||(!defined(tcnter_compare_t) && defined(tcnter_source_t)) )
 #error "tcnter_source_t and tcnter_compare_t must *both* be overridden"
#endif

#ifndef tcnter_source_t
 #ifdef __AVR_AT90PWM161__
  #define tcnter_source_t	uint16_t
 #else
  #define tcnter_source_t   uint8_t
 #endif
#endif

#ifndef tcnter_compare_t
 #ifdef __AVR_AT90PWM161__
  #define tcnter_compare_t  int32_t
 #else
  #define tcnter_compare_t  int16_t
 #endif
#endif

//This is a stupid name...
#define myTcnter_t  uint32_t
//This makes more sense...
// But I think I did it that way because I might later want to make this
// an internal structure, in case I want multiple tcnters...
// (e.g. like xyt, or hfm, etc...)
// seems less likely in this case...
#define tcnter_t uint32_t
#define tcnter8_t uint8_t


//********** TCNTER Prototypes **********//

//These are the functions that are used in main code...
// General initialization...
void tcnter_init(void);
// To be called in the main loop...
// (Handles reading the TCNT deltas and adding to myTcnter, etc)
// this should be called MORE OFTEN than overflows
// If using an overflow-interrupt as the only reliable source of updates
// then use tcnter_overflowUpdate() (other tcnter_updates can also be
// called)
void tcnter_update(void);

#if(defined(TCNT_UPDATE_ONCE_PER_OVERFLOW) && \
		      TCNT_UPDATE_ONCE_PER_OVERFLOW)
void tcnter_overflowUpdate(void);
#endif

// Get the current value of myTcnter
tcnter_t tcnter_get(void);

void tcnter_wait(myTcnter_t tcnts);

//Of course, this is only safe if it's called often enough...
// and deltaTime is small enough...
// and tcnterUpdate is called often enough, as well (?)
//uint8_t tcnter_isItTime8(tcnter8_t *startTime, tcnter8_t deltaTime);
//uint8_t tcnter_isItTime(tcnter_t *startTime, tcnter_t deltaTime);

// dmsTimer has these inlined, and lists them in the header file
// (I guess it makes sense, they're small enough to always inline, and that
// won't happen if they're in the C file...)

// For now, the notes are still in the C file.

// a/o V2: dontAllowCumulation should optimize-out into a smaller function
//         (if FALSE)
extern volatile tcnter_t tcnter_myTcnter;

static __inline__
uint8_t tcnter_isItTime8V2(tcnter8_t *startTime, tcnter8_t deltaTime,
										uint8_t dontAllowCumulation)
{
   tcnter8_t thisDelta = (uint8_t)tcnter_myTcnter - (uint8_t)(*startTime);

   if(thisDelta >= deltaTime)
   {
		if(dontAllowCumulation)
      	*startTime = (uint8_t)tcnter_myTcnter - (thisDelta - deltaTime);
      else
			*startTime = (uint8_t)tcnter_myTcnter;
		return TRUE;
   }
   else
      return FALSE;
}

static __inline__
uint8_t tcnter_isItTimeV2(tcnter_t *startTime, tcnter_t deltaTime,
										uint8_t dontAllowCumulation)
{
   tcnter_t thisDelta = tcnter_myTcnter - *startTime;

   if(thisDelta >= deltaTime)
   {
		if(dontAllowCumulation)
      	*startTime = tcnter_myTcnter - (thisDelta - deltaTime);
      else
			*startTime = tcnter_myTcnter;
      return TRUE;
   }
   else
      return FALSE;
}
//For backwards-compatibility...
#define tcnter_isItTime(a,b) tcnter_isItTimeV2((a),(b),TRUE)
#define tcnter_isItTime8(a,b) tcnter_isItTime8V2((a),(b),TRUE)







#if (defined(_TCNTER_INLINE_) && _TCNTER_INLINE_)
   #define TCNTER_INLINEABLE extern __inline__
   #include "tcnter.c"
#else
   #define TCNTER_INLINEABLE //Nothing Here
#endif


//Basic Usage:
// (Not particularly accurate...)
// (See also polled_uar/0.40/avrTest/)
// makefile:
// CFLAGS+=-D'TCNTER_SOURCE_VAR=TCNT0'
// CFLAGS+=-D'TCNTER_SOURCE_OVERFLOW_VAL=255' #should this be 255 or 256?
// #OVERFLOW_VAL Should be the value at which it overflows back to 0
// so for a uint8_t that wraps-around on its own, the overflowVal should be
// ___256___

// # Or if the DMS timer is running (heartBeat included?)
// CFLAGS += -D'TCNTER_SOURCE_VAR=(TCNT0)'
// CFLAGS += -D'TCNTER_SOURCE_OVERFLOW_VAL=(_DMS_OCR_VAL_)'
// CFLAGS += -D'_SPECIALHEADER_FOR_TCNTER_=_DMSTIMER_HEADER_'

// init();
// while(1)
// {
//		update();
//    ...
//		if(tcnter_get() >= nextTime)
//			nextTime += delay;
//			...
// }
//
//  This should probably be changed to be #defined application-specifically

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
 * /Users/meh/_avrProjects/LCDdirectLVDS/90-reGitting/_commonCode_localized/tcnter/0.30/tcnter.h
 *
 *    (Wow, that's a lot longer than I'd hoped).
 *
 *    Enjoy!
 */

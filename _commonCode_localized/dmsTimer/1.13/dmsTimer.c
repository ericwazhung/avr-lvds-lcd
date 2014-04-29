/* mehPL:
 *    This is Open Source, but NOT GPL. I call it mehPL.
 *    I'm not too fond of long licenses at the top of the file.
 *    Please see the bottom.
 *    Enjoy!
 */




#include "dmsTimer.h"

//#if (defined(__COMPILING_AVR_LIBC__) || (_DMS_EXTERNALUPDATE_))
#if (defined(__AVR_ARCH__) || (_DMS_EXTERNALUPDATE_))
volatile uint32_t dmsCount	= 0;	//deci-milliseconds
#endif



//These WERE specific to externalUpdate, 
// however they can also be used for dmsFrac with timer0 interrupt
// (Along with dmsFrac, below)
// Also with nonAVR code, where us is read directly...
//   these defaults are changed in init functions
//   (defaults correspond to us-reading)
#if (!defined(DMS_FRAC_UNUSED) || !DMS_FRAC_UNUSED)
uint8_t dmsNumUpdates = 100; 
uint8_t dmsIncrementSize = 1;
#endif

#if (!defined(DMS_FRAC_UNUSED) || !DMS_FRAC_UNUSED)
//Fractional part of the last-read dmsTime...
// This'll likely be handled differently in different cases
// for now it's only implemented in NON-AVR code or AVR code based on
// timer interrupt 0 (untested)
volatile uint8_t dmsFrac = 0;
#endif

//DMS timer is updated spurradically by an externally-defined source...
#if (_DMS_EXTERNALUPDATE_)	
// MOST externalUpdate-specific code is now in dmsExternal.c
// SEE dmsTimer.h for its -> inclusion <-
xyt_t dmsLine;

// #if !defined(__COMPILING_AVR_LIBC__)
//  #error "External update for DMS is not implemented for this system"
// #endif


//Otherwise, if we're using an AVR, Timer0 is DMS-specific...
//#elif defined(__COMPILING_AVR_LIBC__)
#elif defined(__AVR_ARCH__)

//#if defined(__AVR_ATmega644__)
 #if defined(SIG_OUTPUT_COMPARE0A)
SIGNAL(SIG_OUTPUT_COMPARE0A) 
 #elif defined(SIG_OUTPUT_COMPARE0)
SIGNAL(SIG_OUTPUT_COMPARE0)
 #elif defined(TIMER0_COMPA_vect)
SIGNAL(TIMER0_COMPA_vect)
 #else
  #error "this MCU's Timer0 interrupt is not supported (or nonexistent)"
 #endif
{
	dmsCount++;
}

void init_dmsTimer(void)
{
#if (!defined(DMS_FRAC_UNUSED) || !DMS_FRAC_UNUSED)
	dmsNumUpdates = _DMS_OCR_VAL_;
		 // = (uint8_t)((uint32_t)F_CPU / 
		 //        (uint32_t)(1<<_DMS_CLKDIV_)/(uint32_t)10000);
	dmsIncrementSize = 1;
#endif

	//Init_timer0Int(CLKDIV64);
	//t0_setOverflowFreq(10000);	//deci-milliseconds
 #if (F_CPU != 16000000)
  #warning "this clock frequency isn't guaranteed to give accurate DMS timing..."
 #endif
	//Init_timer0CompareInt(_DMS_CLKDIV_, 10000);
 #if defined(OCR0)
	OCR0 = _DMS_OCR_VAL_; //dmsNumUpdates;
	//OCR0 = (uint8_t)((uint32_t)F_CPU/(uint32_t)(1<<_DMS_CLKDIV_)/(uint32_t)10000);
 #elif defined(OCR0A)
	OCR0A = _DMS_OCR_VAL_; //dmsNumUpdates;
	//OCR0A = (uint8_t)((uint32_t)F_CPU/(uint32_t)(1<<_DMS_CLKDIV_)/(uint32_t)10000);
 #else
  #error "DMS doesn't yet support this processor with dedicated Timer0, try DMS_EXTERNAL_UPDATE"
 #endif

	timer_setWGM(0, WGM_CLR_ON_COMPARE);
	timer_selectDivisor(0, _DMS_CLKDIV_);
	timer_compareMatchIntEnable(0, OUT_CHANNELA);
	//@@@#warning "timerCommon doesn't yet support frequency-setting!" (handled above, manually)
}

#endif

// 2.3 assign an address to the function pointer
//     Note: Although you may ommit the address operator on most compilers
//     you should always use the correct way in order to write portable code.

// C
//int DoIt  (float a, char b, char c){ printf("DoIt\n");   return a+b+c; }
//int DoMore(float a, char b, char c)const{ printf("DoMore\n"); return a-b+c; }

//pt2Function = DoIt;      // short form
//pt2Function = &DoMore;   // correct assignment using address operator

#if (!defined(DMS_WAITFN_UNUSED) || !DMS_WAITFN_UNUSED)
void dmsWaitFn(uint32_t time, void (*p_waitFn)(void))		//Wait time deci-milliseconds
{
	uint32_t startTime;
	
	startTime = dmsGetTime();
	
	while(dmsGetTime() - startTime < time)
	{
		//http://www.newty.de/fpt/fpt.html#callconv
		// 2.4 comparing function pointers
		//if(pt2Function >0){                           // check if initialized
		//  if(pt2Function == &DoIt)
		//      printf("Pointer points to DoIt\n"); }
		//else
		//   printf("Pointer not initialized!!\n");

		//WTF was I thinking?
      //if(*waitFn != 0)
		if(p_waitFn != 0)
		{
			//2.5  Calling a Function using a Function Pointer
			//In C you call a function using a function pointer by explicitly dereferencing it using the * operator.
			// Alternatively you may also just use the function pointer's instead of the funtion's name. 

			// 2.5 calling a function using a function pointer
			//int result1 = pt2Function    (12, 'a', 'b');          // C short way
			//int result2 = (*pt2Function) (12, 'a', 'b');          // C
			
			//waitFn();
			(*p_waitFn)();
		}

	}

}
#endif

/*
void dmsWait6secFn(dms6sec_t time, void (*waitFn)(void))
{
	dms6sec_t startTime;
	
	startTime = dmsGet6sec();
	
	while(dmsGet6sec() - startTime < time)
	{
		if(*waitFn != 0)
			waitFn();
	}

}
*/

#if (_DMS_EXTERNALUPDATE_)
//Shouldn't actually need these in this case
// #define dms_cli()	{}
// #define dms_sei()	{}
//#elif defined(__COMPILING_AVR_LIBC__)
#elif defined(__AVR_ARCH__)
 #define dms_cli()	cli()
 #define dms_sei()	sei()
 #warning "UNTESTED: cli() around dmsCount"
#else
 //Shouldn't actually need these in this case
#endif

/* From avr/interrupts.h:
		In order to implement atomic access to multi-byte objects,
		consider using the macros from <util/atomic.h>, rather than
		implementing them manually with cli() and sei().
*/

/* From avr/power.h:
		static __inline__ void clock_prescale_set(clock_div_t) __attribute__((__always_inline__));
		...This function is inlined, even if compiler
		optimizations are disabled.

	From util/delay_basic.h:
		static inline void _delay_loop_1(uint8_t __count) __attribute__((always_inline));
*/


//dmsGetTime for externalUpdate is handled in dmsExternal.c
#if !(_DMS_EXTERNALUPDATE_)
dms4day_t dmsGetTime(void)
{
//#elif defined(__COMPILING_AVR_LIBC__)
 #if defined(__AVR_ARCH__)
	dms4day_t time;

	// If a dmsUpdate (timer interrupt) occurs DURING a read of dmsCount
	//  dmsCount could be scrambled:
	//  e.g. 0x00ff -> 0x0100 could return 0x1ff or possibly 0x0000
	//  since each byte is handled separately (?)
	// THIS IS NEW as of glTest and UNTESTED in AVR.
	dms_cli();
	//There's a *slight* possibility the CLI allows one clock-cycle for:
	// *interrupt-processing which *might* allow TCNT0 to increment dmsCount
	//  which would be OK...?
	// *TCNT0 *might* overflow before it's read and AFTER/During CLI
	//  in which case, dmsCount will be one less than it should
	//  for the corresponding dmsFrac...
	//  I think the SAFE way to handle this is to read dmsCount, then dmsFrac
	//  then reread dmsCount (after interrupts are reenabled)
  #if (!defined(DMS_FRAC_UNUSED) || !DMS_FRAC_UNUSED)
   #warning "dmsFrac may not be properly-synced with dmsCount"
	#if defined(TCNT0)
	 dmsFrac = TCNT0;
	#elif defined(TCNT0L)
	 //This isn't particularly all-encompassing...
	 // The Tiny861 uses TCNT0L alone in 8-bit mode (which should do)
	 dmsFrac = TCNT0L;
	#else
	 #error "TCNT0 is not available... untested MCU!"
	#endif
  #endif
   time=dmsCount;
	dms_sei();

	return time;
 #else
	//GNU/FreeBSD:
	// long int tv_sec (seconds since the "epoch")
	// long int tv_usec (fractional part in microseconds)
	struct timeval now;

	gettimeofday(&now, NULL);

  #if (!defined(DMS_FRAC_UNUSED) || !DMS_FRAC_UNUSED)
	dmsFrac = now.tv_usec%100;
  #endif
	//This *WILL* overflow, but it should still work.... (?)
	return (dms4day_t)((now.tv_sec*DMS_SEC) + (now.tv_usec/100));
 #endif
}
#endif //NOT dmsExternalUpdate...


#if (!defined(DMS_FRAC_UNUSED) || !DMS_FRAC_UNUSED)
//Gives the fractional-part of the time AT the last dmsGetTime()
// so e.g. run: really_long_int usTime=(dmsGetTime()*100 + dmsGetFrac());
//Returns 0-99 (100ths of a DMS or us) 
//CURRENTLY ONLY VALID FOR incrementSize = 1. (timer0, PC)
// (must use xyT for externalUpdate...)
uint8_t dmsGetFrac(void)
{
	return (uint8_t)((uint16_t)(dmsFrac) * (uint16_t)100  / (uint16_t)dmsNumUpdates);
}
#endif

/*
dms6sec_t dmsGet6sec(void)
{
	return (dms6sec_t)dmsCount;
}
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
 * /Users/meh/_avrProjects/LCDdirectLVDS/68-backToLTN/_commonCode_localized/dmsTimer/1.13/dmsTimer.c
 *
 *    (Wow, that's a lot longer than I'd hoped).
 *
 *    Enjoy!
 */

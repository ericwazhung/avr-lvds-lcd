/* mehPL:
 *    This is Open Source, but NOT GPL. I call it mehPL.
 *    I'm not too fond of long licenses at the top of the file.
 *    Please see the bottom.
 *    Enjoy!
 */




//dmsTimer 1.13-6
//1.13-6 a/o at90pwm161 toward LCDdirectLVDS67:
//       "this MCU not yet supported" error message made a little more
//       informative...
//1.13-5 _DMS_EXTERNALUPDATE_ is not defined warning...
//1.13-4 a/o heartbeat1.30-testDMS: It ain't woikin!
//       Looking into this...
//1.13-3 dmsIsItTime()
//1.13-2 dmsIsItTime6sec() from tcnter0.20
//1.13-1 removing TCOMDIR in favor of COMDIR
//1.13 updating to latest timerCommon and xyTracker (untested)
//     fix _DMS_OCR_VAL instead of _DMS_OCR_VAL_ (whoops)
//1.12-1 looking into init_dmsExternalUpdate with uint32's... (see below)
//     no code-size change, since xyt_init is called with uint32s
//       padding was necessary, either way the same number of bytes used
//     HOWEVER: with dmsExternalUpdate AND dmsFrac, it's been broken
//       easy fix based on your needs...
//1.12 removing dmsNumUpdates and dmsIncrementSize if dmsFrac is unused
//     NOTE: changes to NON-dmsExternal are UNTESTED
//           (minor, and verified proper, but UNTESTED)
//     pondering using init_dmsExtenalUpdate with int32's instead...
//     since it's inlined, it shouldn't be any more code...
//1.11-2 removing unused variables in dmsUpdate
//1.11-1 attempting to shrink code-size for threePinIDer47i
//       a failed attempt at makefile _USED options can be found in
//       47iUSED
//1.11 dmsUpdate inlining... addition of dmsExternal.h
//1.10-3 Precision-fixing... OCR_VAL should be ... - 1
//1.10-2 adding _DMS_OCR_VAL_
//       (and moving its math to #define instead of initializing a variable)
//       Also adding an error if it's greater than 255
//         (and therefore won't work with OCR0)
//1.10-1 making it work with newer versions of timerCommon (and its header)
//1.10 xyTracker for externalUpdate
//     (xyT now supports rise>run!)
//1.09 added cli/sei around dmsCount in dmsGetTime (AVR) (UNTESTED)
//     adding dmsFrac
//     adding test program
//1.08 attempting to make this work with glTest (non-AVR)
//1.00-8 adding 1284p
//1.00-7 fixing function pointer comparison and call never caused a problem, but it coulda...
//		dmsWait is not tested...
//		(six not backed up, see krautPoxn117 tarball)
//1.00-6 adding dmsGet6sec macro
//1.00-5 removing dms6sec specific functions, doesn't make sense to waste the program memory to save 2 bytes on the stack...
//1.00-4 adding dms6sec_t and dms4day_t
//1.00-3 dmsWait is dangerous, only defining if not defined in heartbeat... (1.00-2 wasn't backed-up)
//1.00-2 updating to timerCommon 1.00 if DMS_EXTERNALUPDATE == FALSE, for krautPoxn38
//1.00 prepping for 1.00 codes...
// changes to T0OVERFLOW -> EXTERNALUPDATE
// TODO: Update for TimerCommon 1.00
//.93.1-1 notes in makefile re T0OVERFLOW... should probably change this sometime...
//.93.1 added _DMSTIMER_INCLUDED_, removed error re: T0Overflow assumption... if it's handled elsewhere we'll get a multiple-definition error.
//.93-4ish WTF about toggling PD0?!
//.93-1 adding __..._WARN__'s
//.93 attempting to make compatible with pwm0 using timer0 overflows and counting instead of compare matches?
//.92.2a added _DMS_CLKDIV_
//.92.2 makefile configured better...
//.92 makefile... and timer0_0.95
//.91cf	moved to _common from MotorDriver11
//.91 combined .9h with unversionated from LL80ish
//.9h HACKED to save program memory
//.9 stolen from ll master 27, modified for use with 8515 as well

#ifndef __DMSTIMER_H__
#define __DMSTIMER_H__

#include <inttypes.h>
#if (defined(_DMS_EXTERNALUPDATE_) && _DMS_EXTERNALUPDATE_)
 #include _XYTRACKER_HEADER_
#elif (!defined(_DMS_EXTERNALUPDATE_))
 #warning "_DMS_EXTERNALUPDATE_ is not defined... either set DMS_EXTERNALUPDATE in your makefile, or leave it alone and we'll assume it's FALSE"
#endif


//This message is probably overkill, it should be handled, now, by a
//regular "F_CPU is not defined" message
// but this can't hurt.
#ifndef F_CPU
#error "###############################################################"
#error "F_CPU is not defined, make sure your makefile has e.g. 'FCPU = 16000000UL'"
#error "YES, that's FCPU in the makefile, not F_CPU"
#error "###############################################################"
#endif


//THIS IS OUTDATED... 
// A) call in makefile
// B) can work in other interrupt sources
// C) doesn't have to be faster than 1dms... can be updated spurradically in larger increments... 
//Uncomment this, or define it in the calling makefile
// if sharing Timer0 using overflows instead of compare-matches, i.e. for pwm0
// This is only possible with CLKDIV1 since overflows occur after 256 counts...
// Be sure to call dms_update in the T0 overflow interrupt!
//#define	_DMS_T0OVERFLOW_

//the DMS timer uses uint32_t's, which gives >4days of measurement...
// For measurements that are less than 6.5sec it doesn't make sense to use u32s...
//    except for the case where it may be used 6.5sec after init and it's initialized to 0.... (consider later...)
//uint16_t->MAX = 65535dms = 6.5sec 
typedef uint16_t dms6sec_t;
//uint32_t->MAX = 4,294,967,296dms = 429,496.7sec = 7,158.28min = 119.3hr = 4.97days
typedef uint32_t dms4day_t;
typedef uint32_t dmsMax_t;

#define DMS_MS 10l
#define DMS_SEC   (1000l*DMS_MS)
#define DMS_MIN   (60l*DMS_SEC)


#ifndef _DMS_EXTERNALUPDATE_
 #warning "_DMS_EXTERNALUPDATE_ not defined. Assuming T0 interrupt is exclusively for use by dmsTimer (Remove this by making it true in your CFLAGS...)"
 #define _DMS_EXTERNALUPDATE_	FALSE
#endif

//Doing this in the headerfile causes problems 
//'cause the define is coming from makefiles which may be executed after the header includes
#if (_DMS_EXTERNALUPDATE_)
//#warning "using Timer0 overflow instead of compare-match for dmsTimer"

 //dms_update is called from the application...
static __inline__ \
void dms_update(void) \
	  __attribute__((__always_inline__));

 //These two arguments determine the number of updates to count before incrementing the timer...
 // incrementsize allows higher accuracy at the cost of precision
 // (i.e. increment 4dms every 25 overflows rather than incrementing 1dms every 6 overflows and losing .25 overflows...)
 // Could also be used if clkDivT0 is > CLKDIV1 or F_CPU < 16000000...
static __inline__ \
void init_dmsExternalUpdate(uint32_t numUpdates, uint32_t incrementSize) \
	  __attribute__((__always_inline__));

 
/* 
 #define DMS_INCREMENTSIZE	4
 #define DMS_OVERFLOWCOUNT	25

 #if (F_CPU != 16000000UL)
  #error "dmsTimer doesn't yet handle CPU clocks other than 16MHz... doing-so would require individual dmsUpdates of >1dms"
  #define DMS_NUMOVERFLOWS 
 #endif
 
 //_DMS_CLKDIV_ should be defined in the calling make-file to indicate
 #if (_DMS_CLKDIV_ != CLKDIV1)
  #error "Define _DMS_CLKDIV_ in the calling makefile... its value is determined by the T0ClkDiv set by another function/library. Currently it can only be CLKDIV1"
 #endif
*/
 
//This isn't defined without the avr/io.h header file (?)
//#elif defined(__COMPILING_AVR_LIBC__)
//But: touch foo.h; avr-cpp -dM foo.h gives a listing of predefined macros
// AVR, __AVR, and __AVR_ARCH__ are all predefined... I'mma use __AVR_ARCH__
// since it's the least likely to be defined on other architectures
#elif defined(__AVR_ARCH__)
 #ifdef _TIMERCOMMON_HEADER_
  #include _TIMERCOMMON_HEADER_
 #else
  //Should no longer be necessary:
  //#include "../../timerCommon/1.00/timerCommon.h"
 #endif

 void init_dmsTimer(void);

 //_DMS_CLKDIV_ could be defined in the calling make-file...
 #ifndef _DMS_CLKDIV_
 
 //CLKDIV8 should work with 16MHz as well as 1MHz...
 //However CLKDIV64 was used previously...
  #if (__UNKN_WARN__)
  #warning "CLKDIV8 is now used by dmsTimer, rather than CLKDIV64... verify this doesn't conflict with other timer0 based code!"
  #endif
 
  #define _DMS_CLKDIV_	CLKDIV8
 #endif //_DMS_CLKDIV_



 //From the datasheet: IF waveform mode is set to toggle on compare-match
 // f_oc = f_clk / (2 * CLKDIV * (1 + OCR))
 // then:
 // f_oc = DMS_SEC
 // f_clk = F_CPU
 // 2 * CLKDIV * (1 + OCR) = F_CPU / DMS_SEC
 // 1+OCR = F_CPU / DMS_SEC / 2 / CLKDIV
 // OCR = F_CPU / CLKDIV / DMS_SEC / 2 - 1
 //  Of course, we're not toggling, but the OCR value should be -1

 #define _DMS_OCR_VAL_UNLIMITED_ \
		(F_CPU / (1<<_DMS_CLKDIV_) / DMS_SEC - 1)
 //  ((uint32_t)F_CPU / (uint32_t)(1<<_DMS_CLKDIV_)/(uint32_t)10000)
 #if (_DMS_OCR_VAL_UNLIMITED_ <= 255)
  #define _DMS_OCR_VAL_ (_DMS_OCR_VAL_UNLIMITED_)
 						//((uint8_t)(_DMS_OCR_VAL_UNLIMITED_))
 #else
  #error "_DMS_OCR_VAL_ > TCNT_MAX... change _DMS_CLKDIV_"
 #endif
 
 //Not an AVR... and not externalUpdate; use GNU's stuff...
#else
 #include <sys/time.h>
#endif


//#endif



//#if defined(__COMPILING_AVR_LIBC__)
#if defined(__AVR_ARCH__)
//Not necessary in GNU...
extern volatile uint32_t dmsCount;	//deci-milliseconds
#endif


#if (_DMS_EXTERNALUPDATE_)
static __inline__ \
dms4day_t dmsGetTime(void)\
	__attribute__((__always_inline__));
#else
//uint32_t dmsGetTime(void);
//Should be fine to getTime into a 6sec_t since it'd just wrap-around
dms4day_t dmsGetTime(void);
#endif

#if (!defined(DMS_FRAC_UNUSED) || !DMS_FRAC_UNUSED)
//Get the fractional-part of the dms...
// (0-99us)
// THIS DOES NOT WORK with externalUpdate if incrementSize != 1
uint8_t dmsGetFrac(void);
#endif

#define dmsGet6sec()	((dms6sec_t)(dmsGetTime()))
//dms6sec_t dmsGet6sec(void);

#if (!defined(DMS_WAITFN_UNUSED) || !DMS_WAITFN_UNUSED)
void dmsWaitFn(uint32_t time, void (*p_waitFn)(void));		
//Wait time deci-milliseconds
//THIS IS DANGEROUS! WDT will timeout if heartUpdate isn't called often enough...
// ??? Where would dmsWait be defined elsewhere...?
#if !defined(dmsWait)
 #define dmsWait(time)  dmsWaitFn((time), (void*)0)
#endif


#endif


//This idea is is stolen from tcnter0.20 see there for notes on wrapping
// startTime is updated such that error hopefully won't cumulate
// for things that need that (otherwise startTime could be set to 
// dmsGetTime at each start...)
static __inline__
uint8_t dmsIsItTime6sec(dms6sec_t *startTime, dms6sec_t deltaTime)
{
	dms6sec_t thisTime = dmsGetTime();
	dms6sec_t thisDelta = thisTime - *startTime;

	if(thisDelta >= deltaTime)
	{
		*startTime = thisTime - (thisDelta - deltaTime);
		return TRUE;
	}
	else
		return FALSE;
}


static __inline__
uint8_t dmsIsItTime(dms4day_t *startTime, dms4day_t deltaTime)
{
	dms4day_t thisTime = dmsGetTime();
	dms4day_t thisDelta = thisTime - *startTime;

	if(thisDelta >= deltaTime)
	{
		*startTime = thisTime - (thisDelta - deltaTime);
		return TRUE;
	}
	else
		return FALSE;
}



//This resets krautPoxn 63... not sure why...
//void dmsWait6secFn(dms6sec_t time, void (*waitFn)(void));	//Wait less than 6sec...

#if (_DMS_EXTERNALUPDATE_)
 #include "dmsExternal.h"
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
 * /Users/meh/_avrProjects/LCDdirectLVDS/68-backToLTN/_commonCode_localized/dmsTimer/1.13/dmsTimer.h
 *
 *    (Wow, that's a lot longer than I'd hoped).
 *
 *    Enjoy!
 */

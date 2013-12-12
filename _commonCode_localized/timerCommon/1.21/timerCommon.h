//timerCommon 1.21-1ish
//1.21-1ish: using _ERRORHANDLING_HEADER_ in order to localize 
//           LCDdirectLVDS 59, for git
//1.21  Some devices apparently have a Timer1 without WGM modes...
//      Similar functionality could probably be implemented, but for now
//      I'm just returning an error on setWGM(1, ...) calls:
//      fixing some errors for ATtiny85:
//      TCCR1A, WGM10, TCCR1B, WGM12  undeclared
//1.20-3 looking into codespace savings via _UNUSED for threePinIDer50i
//1.20-2 WHOA!
//       compareMatchIntSetup had a serious flaw!
//       disable didn't function properly when disabling the interrupt
//       on OCR0A or OCR0B (but not OCR0)
//       due to clearing the overflow flag IN THE WRONG REGISTER
//       (resulting in *enabling* the interrupt)
//       It's *plausible* that TIMSK is used on some devices as the flag
//         register, as well, but seems highly unlikely. Checked Docs:
//         8515, 162, 644, Tiny861, 168, Tiny2313, Tiny84 and Tiny85
//       Mistaken TIMSK writes replaced with TIFR writes...
//1.20-1 using errorHandling 0.99 so we can trace errors...
//1.20   to work with pwm1.20 for TinyX61, etc... with odd output channel
//       configurations
//       Also adds support for more than two output channels
//       (up to four timers, and four channels each)
//1.00-8 using writebit instead of writeMasked... should be more compact
//1.00-7 adding support for TinyX61
//       (doesn't support all the "standard" features of other chips...
//        so now things are getting complicated...)
//1.00-6 removing obnoxious warning...
//1.00-5 fix missing breaks...
//1.00-4 making compareMatch int disableable...
//       TODO: do the same for other ints, etc...
//1.00-3 bithandling.94
//1.00-2 Timer3 fixup in compareMatchIntEnable (re: m162)
//1.00-1 add compareMatchIntEnable(timer,channel)
//       THIS IS UNVERIFIED... only to compile on 8515 with T0/OCIE0
//		FIX T2_TIFR in Timer3 overflowIntEnable 
//1.00	duplicate of 0.98-3, new numbering scheme...
//.98-3 added overflow int enable...
//.98-2 added COM settings and timer_init...
//.98 first version, common header-stuff for [hopefully all] timers...
// TODO Add Timer Stopped
//      Add External sources
//      Add Other clock divisors for other clocks...
#ifndef __TIMERCOMMON_H__
#define __TIMERCOMMON_H__

#include <avr/interrupt.h>
#ifndef _BITHANDLING_HEADER_
 #error "Not gonna letcha get away with this... _BITHANDLING_HEADER_ is not defined"
 #include "../../bithandling/0.94/bithandling.h"
#else
 #include _BITHANDLING_HEADER_
#endif

#ifndef _ERRORHANDLING_HEADER_
 #include "../../errorHandling/0.99/errorHandling.h"
#else
 #include _ERRORHANDLING_HEADER_
#endif


///BEWARE:::
//
//#warning "power-on defaults necessary for timer configuration, don't reconfigure in run-time..."
//
//
//BEWARE:::


//This only handles up to four timers...
// don't change this without verifying it's safe to in timerCommon.c!
#define MAXTIMERS	4
//Currently handles up to four channels (A,B,C,D) per timer...
// Though they're not all handled equally...
#define MAXCHANNELSPERTIMER 4
//Clock Select Bit Description to be used in the Register containing the Clock Select Bits
// A/B in TCCRn does NOT indicate CHANNEL A/B, but Control Register A/B where two are necessary
//                      CSReg (CSn2:n0)
// m162 (8515 pinout equivalent):
// Timer0 8bit  1 OCR	TCCR0	"Typical"
// Timer1 16bit 2 OCRs	TCCR1B	"Typical"
// Timer2 8bit  1 OCR	TCCR2	*** DOES NOT MATCH! See "Alternate" Below ***
// Timer3 16bit 2 OCRs	TCCR3B	"Typical" **EXCEPT EXTERNAL** <-- "Obscure"
// mXX4P:
// Timer0 8bit  2 OCRs	TCCR0B	"Typical"
// Timer1 16bit 2 OCRs	TCCR1B	"Typical"
// Timer2 8bit  2 OCRs	TCCR2B	*** DOES NOT MATCH! See "Alternate" Below ***
// m8515:
// Timer0 8bit  1 OCR	TCCR0	"Typical"
// Timer1 16bit 2 OCRs	TCCR1B	"Typical"
// mXX8:
// Timer0 8bit  2 OCRs	TCCR0B	"Typical"
// Timer1 16bit 2 OCRs	TCCR1B	"Typical"
// Timer2 8bit  2 OCRs	TCCR2B	*** DOES NOT MATCH! See "Alternate" Below ***
// TinyX61:
// Timer0 8bit  ???
// Timer1 8bit  4 OCRs  TCCR1B   Non-standard in every way...

/*
TYPICAL:
CSn2 CSn1 CSn0   Description 
  0    0    0    No clock source (Timer/Counter stopped). 
  0    0    1    clkI/O/1 (No prescaling) 
  0    1    0    clkI/O/8 (From prescaler) 
  0    1    1    clkI/O/64 (From prescaler) 
  1    0    0    clkI/O/256 (From prescaler) 
  1    0    1    clkI/O/1024 (From prescaler) 
 *1    1    0    External clock source on Tn pin. Clock on falling edge. 
 *1    1    1    External clock source on Tn pin. Clock on rising edge. 
 (Above unless otherwise marked below... "OBSCURE")
 *1    1    0    clkI/O/16 (From prescaler) ***   m162 Timer3   ***
 *1    1    1    clkI/O/32 (From prescaler) ***   m162 Timer3   ***

ALTERNATE: (mXX4 Timer2, m162 Timer2, mXX8 Timer2)
CS22 CS21 CS20   Description 
  0    0    0    No clock source (Timer/Counter stopped). 
  0    0    1    clkT2S/1 (No prescaling) 
  0    1    0    clkT2S/8 (From prescaler) 
  0    1    1    clkT2S/32 (From prescaler) 
  1    0    0    clkT2S/64 (From prescaler) 
  1    0    1    clkT2S/128 (From prescaler) 
  1    1    0    clkT2S/256 (From prescaler) 
  1    1    1    clkT2S/1024 (From prescaler) 
*/

//Also Clock stopped and external clocks... not implemented!
//values are 2^value... or (1<<CLKDIVx) returns x
//See above tables for compatibility
//TYPICAL
#define CLKDIV1		0
#define CLKDIV8		3
#define CLKDIV64	6
#define CLKDIV256	8
#define CLKDIV1024	10
//Other divisors may be available and are somewhat burried below...


//Is the timer typical?
// Of all MCUs explored, Timer0 and Timer1 are always Typical
#define IS_CSTYP(tn) (((tn) == 0) || ((tn) == 1))


//ALTERNATE (available on all explored MCUs with Timer2)
#if (defined(TCCR2) || defined(TCCR2B))
 #define CLKDIV32	5
 #define CLKDIV128	7
 //and for conditional compilation...
 #define CSALT
 //Does the timer use the alternate clock divisor scheme?
 #define IS_CSALT(tn) ((tn) == 2)
#endif

//OBSCURE (only available on m162 Timer3... not sure about other MCUs' Timer3)
#if defined(__AVR_ATmega162__)
 #define CLKDIV16	4
 //and for conditional compilation...
 #define CSOBS
 //Does the timer use the obscure divisor scheme?
 #define IS_CSOBS(tn)  ((tn) == 3)
#endif

uint8_t timer_selectDivisor(uint8_t timerNum, uint8_t clockDiv);


//Timer Waveform Modes...
//These four modes were universal amongst all timers on all MCUs explored...
//If there's a need for greater functionality, deal with that elsewhere and/or later...
// The values are the actual values written to WGMn1:n0 IN MOST CASES
// EXCEPTIONS:
// *TinyX61's Timer0 is an exception... it only has a WGM00
//  IT HAS NO PWM modes
//  Its functionality is determined by the upper (second) bit of these 
//  values... (which would be written to WGM01 in the other cases)
//  VALID MODES are WGM_NORMAL and WGM_CLR_ON_COMPARE
// *TinyX61's Timer1 is ALSO an exception... 
//  "Normal" mode refers to no PWM, with Clear-On-Compare (with OCR1C)
//  thus, there is no explicit CLR_ON_COMPARE mode
//  WGM11:10 are used exclusively for determining the type of PWM
//  (but PWM1x must be enabled as well, otherwise WGM11:10 are DON'T CAREs)
//  VALID MODES are WGM_NORMAL, WGM_PHASE_PWM, and WGM_FAST_PWM
//       (WGM_NORMAL == WGM_CLR_ON_COMPARE;
//        No PWM and, unlike PWM modes, OCR1x is updated immediately)
// EXCEPTIONS are hokey... it doesn't really make sense to use these values
//  if they're not valid modes... Or, worse, if they're functionally
//  different from one chip to the next...
#define WGM_NORMAL         0  //Count from 0-0xff and reset
#define WGM_PHASE_PWM      1  //Count up to 0xff then down to 0 and repeat 
                              //(pwm happens in the middle, half-freq)
#define WGM_CLR_ON_COMPARE 2  //Clear the timer on a compare match with OCRA
#define WGM_FAST_PWM       3  //Count from 0-0xff and reset,
                              //trigger port on compare match

//The above values correspond with the bit-values loaded into the WGMnx bits, if appropriately shifted/masked...
// i.e. WGMn1:n0 (TCCRn.3, TCCRn.6) on timer0 and timer2 of the m162
//		WGM01:00 (TCCR0.3, TCCR0.6) on timer0 (no timer2) of the m8515
//		WGMn1:n0 (TCCRnA.1:0)		on timer0 and timer2 of the m168 
//		WGMn1:n0 (TCCRnA.1:0)		on timer0 and timer2 of the mxx4P
//	16bit counters are a little more complicated, but not much... leaving all other WGM bits = 0...
//		WGMn2,WGMn0 (TCCR1B.3, TCCR1A.0) on timer1 of the m168, m8515, and the mxx4P
//		WGMn2,WGMn0 (TCCRnB.3, TCCRnA.0) on timer1 and timer3 of the m162
// Unfortunately, these bits aren't always in the same register...
//  so write them individually
//  define a low-reg and high-reg (could be the same)
// The clear defining-line between the access-means is based on the timer number...
//
// Timer 0 and Timer 2 appear to always be configurable in a single register
// WGM bit numbers are consistant between the two... but the actual bit locations are not...
// In other words, each bit needs to be written individually...
#if defined(TCCR0)
 #define T0_WGMReg	TCCR0
#endif
#if defined(TCCR0A)
 #define T0_WGMReg TCCR0A
#endif
#if defined(TCCR2)
 #define T2_WGMReg	TCCR2
#endif
#if defined(TCCR2A)
 #define T2_WGMReg	TCCR2A
#endif
// Timer 1 and timer 3 need to be configured in two registers... they'll always be TCCR1A and 1B, 
// so no need to define a shortcut
// They are NOT written to WGMn1:0, but TCCRnB.WGMn2 and TCCRnA.WGMn0
uint8_t timer_setWGM(uint8_t timerNum, uint8_t wgm);

//Compare Output modes...
// Vary depending on the selected WGM mode
// However, they are very similar
// This from 8515 (excluding ing/non-inv comments)
//Mode#	COM01:00	Normal/CTC OC0 Operation					FastPWM	OC0				PhasePWM OC0
//-----	--------	------------------------					-----------				------------
//0		0 0 		Normal port operation, OC0 disconnected. 	<--						<--
//1		0 1 		Toggle OC0 on Compare Match.				Reserved				Reserved
//2		1 0 		Clear OC0 on Compare Match.					+ Sets at TOP (non-inv)	CompMatch: Up=Clr, Down=Set
//3		1 1 		Set OC0 on Compare Match.					+ Clears at TOP	(inv)	CompMatch: Up=Set, Down=Clr

//Same for all WGMs
#define COM_DISCONNECTED		0
//Exclusive to Normal/Clr_on_compare modes
//PWM modes typically Reserved, other functionality exists 
//  on some timers/MCUs but isn't implemented here
#define COM_TOGGLE_ON_COMPARE	1
//Same for all WGMs
//Some PWM units alternate at TOP (8515, 162), others at BOTTOM (168, XX4P)
// Not yet sure about the significance...
// Probably a good idea to read about the special case when the OCR matches TOP, etc...
// leaving PWM specifics to pwm.h...
#define COM_CLR_ON_COMPARE		2
#define COM_SET_ON_COMPARE		3

//Luckily, COMnx1:0 are always sequential, but need to be shifted...
// And always in the same registers!

// These aren't _as_ used as they were... use OUT_A where appropriate...
#define OUT_CHANNELA	0
#define OUT_CHANNELB	1


//COM bits need to be shifted...
// If there're two channels (A,B) COMA is in bits 7:6
//							  and COMB is in bits 5:4
// If there's one channel (OC0)   COM is in bits 5:4
//  (wouldn't it be nice if they were in A's location?!)
#define COM_SHIFT		4
#define COMA_SHIFT		6
#define COMB_SHIFT		4
#define COM_MASK		(0x03<<COM_SHIFT)
#define COMA_MASK		(0x03<<COMA_SHIFT)
#define COMB_MASK		(0x03<<COMB_SHIFT)
//It's probably not necessary to do this test... but it won't hurt...
#if defined(_AVR_IOTNx61_H_)
#define COMD_SHIFT	2
#define COMD_MASK		(0x03<<COMD_SHIFT)
#define T1_COMDReg	TCCR1C
#endif

//Can determine which OCnx pins are available by checking the FOCnx 
// definitions (i.e. FOC3A)
// THIS IS BETTER than checking for OCRnx because e.g. OCR1C on the TinyX61
// does not correspond to an output pin (only to the TOP value of timer1)
// These values do NOT correspond to the pin numbers and may overlap, 
// so create our own...
// The following values will also NOT correspond to the pin numbers 
// (that'll get messy, MCU-specific! Handled in _commonCode/pwm)
//
// The high nibble corresponds directly to the timer number
// the low nibble corresponds to a mask of the output
// (If there's only one, it's represented by OUT_A)
#define OUT_A 0x01
#define OUT_B 0x02
#define OUT_C 0x04
#define OUT_D 0x08
// e.g. OC1D is represented by 0x18
//The following will most-likely be used only internally...
// NOTE: These Values Have Changed as of 1.20 (A was 0 and B was 1)
#if defined(FOC0)
 // 0x01
 #define OUT_OC0	(0x00 | OUT_A)
#endif
#if defined(FOC0A)
 // 0x01
 #define OUT_OC0A	(0x00 | OUT_A)
#endif
#if defined(FOC0B)
 // 0x02
 #define OUT_OC0B	(0x00 | OUT_B)
#endif
#if defined(FOC1)
 // 0x11
 #define OUT_OC1	(0x10 | OUT_A)
#endif
#if defined(FOC1A)
 // 0x11
 #define OUT_OC1A	(0x10 | OUT_A)
#endif
#if defined(FOC1B)
 // 0x12
 #define OUT_OC1B	(0x10 | OUT_B)
#endif
#if defined(FOC1C)
 // 0x14
 #warning "haven't run into this case yet... Untested MCU"
 #define OUT_OC1C (0x10 | OUT_C)
#endif
#if defined(FOC1D)
 // 0x18
 #define OUT_OC1D (0x10 | OUT_D)
#endif
#if defined(FOC2)
 #define OUT_OC2	(0x20 | OUT_A)
#endif
#if defined(FOC2A)	
 #define OUT_OC2A	(0x20 | OUT_A)
#endif
#if defined(FOC2B)
 #define OUT_OC2B	(0x20 | OUT_B)
#endif
#if defined(FOC3)
 #define OUT_OC3	(0x30 | OUT_A)
#endif
#if defined(FOC3A)
 #define OUT_OC3A	(0x30 | OUT_A)
#endif
#if defined(FOC3B)
 #define OUT_OC3B	(0x30 | OUT_B)
#endif






//Create a standard interface for the COM registers...
// This gets harder as, e.g. Tiny861 uses two regs for COM1A/B and COM1D
#if defined(TCCR0)
 #define T0_COMReg	TCCR0
#endif
#if defined(TCCR0A)
 #define T0_COMReg	TCCR0A
#endif
#if defined(TCCR1)
 #define T1_COMReg	TCCR1	//seems unlikely
#endif
#if defined(TCCR1A)
 #define T1_COMReg	TCCR1A
#endif
//T1_COMDReg defined above
#if defined(TCCR2)
 #define T2_COMReg	TCCR2
#endif
#if defined(TCCR2A)
 #define T2_COMReg	TCCR2A
#endif
#if defined(TCCR3)
 #define T3_COMReg	TCCR3
#endif
#if defined(TCCR3A)
 #define T3_COMReg	TCCR3A
#endif


//Timer Overflow Interrupt Enable is handled by the appropriate bit in the xTIMSKn register...
// The XX4P and 168 are easy, each timer has its own register...
// The 8515 matches the 162 as far as the first two timers, Timer 2 (the third) in the 162, 
// and they all share the same register
// Timer 3 in the 162 is in its own register...
// Let's pretend by creating fake registers for each timer...
#if defined(TIMSK)
 #if defined(TOIE0)
	#define T0_TIMSK	TIMSK
	#define T0_TIFR		TIFR
 #endif
 #if defined(TOIE1)
	#define T1_TIMSK	TIMSK
	#define T1_TIFR		TIFR
 #endif
 #if defined(TOIE2)
	#define T2_TIMSK	TIMSK
	#define T2_TIFR		TIFR
 #endif
 #if defined(TOIE3)
	#define T3_TIMSK	ETIMSK
	#define T3_TIFR		ETIFR
 #endif
#endif
#if defined(TIMSK0)
	#define T0_TIMSK	TIMSK0
	#define T0_TIFR		TIFR0
#endif
#if defined(TIMSK1)
	#define T1_TIMSK	TIMSK1
	#define T1_TIFR		TIFR1
#endif
#if defined(TIMSK2)
	#define T2_TIMSK	TIMSK2
	#define T2_TIFR		TIFR2
#endif
#if defined(TIMSK3)
	#define T3_TIMSK	TIMSK3
	#define T3_TIFR		TIFR3
#endif


//For some reason this wasn't here originally... (odd?)
// Maybe it hasn't worked with timerCommon?
// Don't use it without verifying...
uint8_t timer_init(uint8_t timerNum, uint8_t clockDiv, uint8_t wgm);

uint8_t timer_overflowIntEnable(uint8_t timerNum);
//uint8_t timer_compareMatchIntEnable(uint8_t timerNum, uint8_t outputChannel);
//enable is either TRUE or FALSE...
// This exits with sei, independent of its original state or enable value...
//  outputChannel is either OUT_CHANNEL_A or OUT_CHANNEL_B
//  !!! THIS SHOULD BE MODIFIED TO MATCH OUT_A OUT_B... (?)
#if (!defined(TIMER_COMPAREMATCHINTSETUP_UNUSED) \
		|| !TIMER_COMPAREMATCHINTSETUP_UNUSED)
uint8_t timer_compareMatchIntSetup(uint8_t timerNum, uint8_t outputChannel,
	  												uint8_t enable);
#define timer_compareMatchIntEnable(timerNum, outputChannel) \
			timer_compareMatchIntSetup((timerNum), (outputChannel), TRUE)
#endif

//outputChannelMask can be e.g. (OUT_A | OUT_B | OUT_D)
uint8_t timer_setOutputModes(uint8_t timerNum, uint8_t outputChannelMask,
										uint8_t outputMode);

/* Example usage... */
// Currently this depends on Power-on defaults for unwritten registers!
// Thus, it's not currently predictable what will happen if timer settings are changed twice...
//
// Typically:
// 0) disable interrupts
//    if starting compare-match or overflow interrupts...
// 1) set the WGM
// 2) set the Output Compare pin mode.......
// 3) set the OCR value 
//    (esp necessary here if trying to set a specific frequency, CLR_ON_COMPARE mode...)
//    (or to set an initial PWM value... to prevent glitching) 
// 4) enable the interrupt, and clear its flag
// 5) enable interrupts
// 6) select the clock divisor (starts counting)

/* The following is where the interrupt handler should be added */
//SIGNAL(SIG_OVERFLOW0){}        /* signal handler for tcnt0 overflow interrupt */

#endif

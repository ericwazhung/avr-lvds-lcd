/* mehPL:
 *    This is Open Source, but NOT GPL. I call it mehPL.
 *    I'm not too fond of long licenses at the top of the file.
 *    Please see the bottom.
 *    Enjoy!
 */



#ifndef __LVDS161_H__
#define __LVDS161_H__

#include _BITHANDLING_HEADER_


//TODO: Make note: RXclk is inverted, RXclk-/+ need to be swapped
//

// Other chips worth looking into:
// Also with 16kb FLASH, 1kb SRAM, surface-mount (SOIC)
// AT90PWM216-16SU/SN
// AT90PWM316-16SU/SN

//delayCyc is used to try to synchronize the PSC2 with the PSCR...
#include _DELAYCYC_HEADER_

// This file is derived/modified from lvds.c (which was for the Tiny861)
// See lvds.c for more thorough explanations.

// Tiny861 LVDS/FPD-Link timing:

//            |<--- (LCDdirectLVDS: "pixel") --->|
//  Timer1:   |<-- One Timer1 Cycle (OCR1C=6) -->|
//  TCNT:     |  0   1    2    3    4    5    6  |  0   1    2    3    
//            |____.____.____.____               |____.____.____.____
//  RXclk+:   /         |         \    .    .    /         |         \ //
//            |         |          ¯¯¯¯ ¯¯¯¯ ¯¯¯¯|         |
// One Pixel: |         |<--- One FPD-Link Pixel Cycle --->|
//            |                                  |
// "Blue/DVH" |____ ____v____ ____ ____v____ ____|____ ____
//  RXin2:    X B3 X B2 X DE X /V X /H X B5 X B4 X B3 X B2 X ...
//            |¯¯¯¯ ¯¯¯¯^¯¯¯¯ ¯¯¯¯ ¯¯¯¯^¯¯¯¯ ¯¯¯¯|¯¯¯¯ ¯¯¯¯
//            |         |<--Not Blue-->|         |
//            |                                  |
// "Green"    |____ ____v____ ____v____ ____ ____|____ ____
//  RXin1:    X G2 X G1 X B1 X B0 X G5 X G4 X G3 X G2 X G1 X ...
//            |¯¯¯¯ ¯¯¯¯^¯¯¯¯ ¯¯¯¯^¯¯¯¯ ¯¯¯¯ ¯¯¯¯|¯¯¯¯ ¯¯¯¯ 
//            |         |<------->|-Not Green    |
//
// "Red"      |____ ____v____v____ ____ ____ ____|____ ____
//  RXin0:    X R1 X R0 X G0 X R5 X R4 X R3 X R2 X R1 X R0 X ...
//            |¯¯¯¯ ¯¯¯¯^¯¯¯¯^¯¯¯¯ ¯¯¯¯ ¯¯¯¯ ¯¯¯¯ ¯¯¯¯ ¯¯¯¯
//            |         |<-->|-Not Red
//
//   Of course: The Not Green/Red bits above are low-bits and
//              basically have little/no visible effect
//
//
//#error "Is this right?! Will this work with +V and +H separable?!"
// see notes in lvds.c

// According to PWM_TESTING, any arbitrary pulse can be generated,
// regardless of whether it wraps past the OCR2RB...
// So for blue we have:
//               bit: 0123456    5601234
//                    32dvh54 -> 5432dvh				OCR2SA	OCR2RA	Wraps?

// B5, B4, B3, B2 =   ¯¯¯¯¯¯¯ -> ¯¯¯¯¯¯¯		60		0			8			N

// B5, B4, B3     =   ¯_¯¯¯¯¯    ¯¯¯_¯¯¯		56		1        0			Y

// B5, B4         =   __¯¯¯¯¯    ¯¯__¯¯¯		48    1        6			N

// B5             =   __¯¯¯¯_    ¯___¯¯¯		32    1        5			N

// BLACK          =   __¯¯¯__    ____¯¯¯		0     1        4			N


// B5, B4      B2 =   _¯¯¯¯¯¯    ¯¯_¯¯¯¯		52    0        6			N

// B5,         B2 =   _¯¯¯¯¯_    ¯__¯¯¯¯		36    0        5			N

//             B2 =   _¯¯¯¯__    ___¯¯¯¯		4     0        4			N


// B5,     B3, B2 =   ¯¯¯¯¯¯_    ¯_¯¯¯¯¯		44    6        5			Y

//         B3, B2 =   ¯¯¯¯¯__    __¯¯¯¯¯		12    6        4			Y


//     B4, B3, B2 =   ¯¯¯¯¯_¯    _¯¯¯¯¯¯		28    5        4			Y

// That's actually pretty decent coverage
// 
// Using BLUE_ALLSHADES with ALLSHADES_GRADIENT
// It seems there's some difficulty with switching between values which
// wrap-around (reset before set, in a cycle) and those that don't...
// So for now our colors are limited to those that don't require wrapping.
// 60, 48, 32, 0, 52, 36, 4
// And a bit of looking-into gives a bit of insight...
//      S R  bit: 32dvh54  5432dvh
// 60 = 0,8	      ¯¯¯¯¯¯¯  ¯¯¯¯¯¯¯  8 is necessary, 7-> all low.
// 52 = 0,6       _¯¯¯¯¯¯  ¯¯_¯¯¯¯
// 36 = 0,5       _¯¯¯¯¯_  ¯__¯¯¯¯
// 4  = 0,4       _¯¯¯¯__  ___¯¯¯¯
//
// So we can get away with only needing to change one register..
// (DE_only might be best set to 0,4...)
// This should probably be done in assembly, so each branch is the same
// number of cycles (see writeColor())
// So this should be used somewhat sparingly.
#define SETBLUE_ASM TRUE

#if(!defined(SETBLUE_ASM) || !SETBLUE_ASM)
#define setBlue4(val)	\
	({ \
		uint8_t ocrVal = ((val) == 3) ? 8 : (val) + 4 ; \
		lockPSC2(); \
		OCR2RAL = ocrVal; \
		unlockPSC2(); \
	})

#else

//Doing the "if" statement in assembly assures that it takes the same
//number of clock-cycles regardless of the branch taken
// ("0" in the input-operands tells the assembler to use the same register
//  for input as well as output, otherwise just having blueOCR_Val in the
//  output-operand causes it to assume that its initial value doesn't
//  matter. Annoying.
#define setBlue4(val)	\
	({ \
		uint8_t blueOCR_Val; \
		blueOCR_Val = (val) + 4; \
		uint8_t sbblueVal; \
		sbblueVal = (val); \
		__asm__ __volatile__ \
	   ( \
    		"cpi   %1, 3 ; \n\t" \
    		"brne  nothingToDo_%= ; \n\t" \
    		"ldi   %0, 8 ; \n\t" \
  		"nothingToDo_%=: \n\t" \
    		: "=d" (blueOCR_Val) \
    		: "d" (sbblueVal), \
			  "0" (blueOCR_Val) \
   	); \
		lockPSC2(); \
		OCR2RAL = blueOCR_Val; \
		unlockPSC2(); \
	})

#endif

// Green:
// Still assuming green is the time-keeper (OCR0RB=6, never changes)
//      S R  bit: 21Bb543  54321Bb
// 56 = 3 6       ____¯¯¯  ¯¯¯____  (S=2,1 only affects blue, S=0->58)
// 24 = 4 6       _____¯¯
// 16 = 5 6       ______¯
//
// Yeah, no. 
// Green OUTPUT INVERTED:
//                0123456
//      S R  bit: 21Bb543    21Bb543         
// 4 =  0 6       _¯¯¯¯¯¯ -> ¯______   UNUSED
// 6 =  3 6       ____¯¯¯ -> ¯¯¯¯___
// 38 = 4 6       _____¯¯ -> ¯¯¯¯¯__
// 54 = 5 6       ______¯ -> ¯¯¯¯¯¯_
// 62 = 6 6       _______ -> ¯¯¯¯¯¯¯

//This is new... (not in Tiny861)
// val=0-3
#define setGreen4(val)	{ lockPSC0(); OCR0SB=(val+3); unlockPSC0(); }

// Red OUTPUT INVERTED:
//                0123456
//      S R  bit: 10g5432    10g5432        54321Bb
//      0 6       _¯¯¯¯¯¯ -> ¯______
//      2 6       ___¯¯¯¯ -> ¯¯¯____
//      3 6       ____¯¯¯ -> ¯¯¯¯___
//      4 6       _____¯¯ -> ¯¯¯¯¯__
//      5 6       ______¯ -> ¯¯¯¯¯¯_
//      6 6       _______ -> ¯¯¯¯¯¯¯

// Red OUTPUT INVERTED, green-matched:
// (set R=5, then only one bit needs changing)
//                0123456
//      S R  bit: 10g5432    10g5432
//      0 5       _¯¯¯¯¯_ -> ¯_____¯
// 7  = 2 5       ___¯¯¯_ -> ¯¯¯___¯
// 39 = 3 5       ____¯¯_ -> ¯¯¯¯__¯
// 55 = 4 5       _____¯_ -> ¯¯¯¯¯_¯
// 63 = 5 5       _______ -> ¯¯¯¯¯¯¯     (Check BLUE experiments)

#define setRed4(val)	{ lockPSC0(); OCR0SA=(val+2); unlockPSC0(); }

// a/o v67-18_ALLSHADES_AND_COLORS:
// fullGreen = OCR0SB = 0
// noGreen =   OCR0SB = 6
// Prior to inversion...
// It seems to work, so.... those values added to the table, above
// (taking into account inversion)
// Actually, duh, S=0 -> bit0 (G2) being low, so might as well carry that
// over through the other bits...
//(but then with inversion, it means those bits are ON... so...? 0 might be
//best)
// with S=3, the base-line "black" is with G=6, B=3
// which has the additional nicity that no lookup table is necessary...
//
// pwm161 in One Ramp mode:
// This image taken from the data-sheet, page 108.
// This image also matches (as I recall) the PSCR controller.
//
//                                               OCR2RB
//                                     OCR2SB       v
//     PSC     |              OCR2RA      v___---¯¯¯|
//    Counter  |OCR2SA           v___---¯¯¯         |
//             |   v     ___---¯¯¯                  |
//             |___---¯¯¯                           |___---
//              0                                    0
// PSCOUT20(A) ____|¯¯¯¯¯¯¯¯¯¯¯¯¯|_____________________
//
// PSCOUT21(B) |__________________________|¯¯¯¯¯¯¯¯¯|____
//
// (First notes, a/o v67-0)
// OUT_A and OUT_B are allowed to overlap in "one ramp" mode.
// The specs are unclear about how/where the PSC Counter resets.
// From the drawing, it seems it always occurs at OCR2RB, which makes
// things difficult, since it would require that OCR2RB always == 7 clock
// cycles.
//
// The spec sheet defines "On-Time" and "Dead-Time", but these values are 
// *calculated* from OCRxyz register-values, rather than being
// register-values, themselves.
//
// This differs dramatically from the Tiny861.
//
// Initial idea: 
//   OCR2RB will have to correspond to RXclk's transition at some edge.
//   The particular edge is unimportant (so far), since the pin could be
//   inverted.
//
//   Then, OCR2SB will always be constant.
//
//   OCR2SA/RA can be used anywhere between for e.g. DE/V/H/Blue
//
// CAVEAT:
//   OCR2SA (and OCR2SB?) must occur at least one tick after 0
//
// SO for RxCLK and Rx"Blue" we're set...
//
// BUT:
// What about the second PSC (PSCR), which needs both its outputs used for
// Rx"Green" and Rx"Red"?
//
// Ideas:
//  The polarities of all outputs are individually controllable(?)
//   (so, e.g. OCRRSB = 0, OCRRRB = 6(?) could be used for both Full-Green
//    and No-Green, while OCRRSA/RA could be independent)
//  NO. Polarity affects BOTH outputs
//  Alternatively, maybe, Output Matrix (Four Ramp?)
//  Alternatively, another, take advantage of PSC2's MUXing for out22/23?
//   (and use PSCR for Clk/Blue)


// These notes compiled from Tiny861 from lvds.c:

//            |<--- (LCDdirectLVDS: "pixel") --->|
//  Timer1:   |<-- One Timer1 Cycle (OCR1C=6) -->|
//  TCNT:     |  0   1    2    3    4    5    6  |
//            |____.____.____.____               |
//  RXclk+:   /                   \    .    .    /
//            |                    ¯¯¯¯ ¯¯¯¯ ¯¯¯¯|
//
// DE:          B3   B2   DE   /V   /H   B5   B4  For BLUE DATA (not D/V/H)
// "Blue/DVH" |____ ____ ____.____.____ ____ ____      C=OCR1A
//  OC1A:  D0>/ D1>/ D2>/           C4>\ C5>\ C6...    D=DT(H/L?)
//            |¯¯¯¯ ¯¯¯¯                ¯¯¯¯ ¯¯¯¯     (D must be 0 for CLK)
//
//             ____ ____      ____      ____ ____|____ 
//  H (only): X xx X xx \    /    \    / xx X xx X xx X    
//   state1    ¯¯¯¯ ¯¯¯¯ ¯¯¯¯      ¯¯¯¯ ¯¯¯¯ ¯¯¯¯|¯¯¯¯ 
//            >______________/¯¯¯¯\______________|    DT=3, OCR=3
//
//             ____ ____           ____ ____ ____|____ 
//  V w/o H:  X xx X xx \    .    /    \ xx X xx X xx X
//   state3    ¯¯¯¯ ¯¯¯¯ ¯¯¯¯ ¯¯¯¯      ¯¯¯¯ ¯¯¯¯|¯¯¯¯
//            >___________________/¯¯¯¯\_________|    DT=4, OCR=4 (+?)

//             ____ ____                ____ ____|____ 
//  V w/ H:   X xx X xx \    .    .    / xx X xx X xx X
//   state4    ¯¯¯¯ ¯¯¯¯ ¯¯¯¯ ¯¯¯¯ ¯¯¯¯ ¯¯¯¯ ¯¯¯¯|¯¯¯¯ 
//            >__________________________________|    DT=X, OCR=0

//             ____ ____      ____ ____ ____ ____|____ 
//  Nada:     X xx X xx \    /    .    \ xx X xx X xx X    
//   state0    ¯¯¯¯ ¯¯¯¯ ¯¯¯¯           ¯¯¯¯ ¯¯¯¯|¯¯¯¯     
//            >______________/¯¯¯¯¯¯¯¯¯\_________|    DT=3, OCR=4 (+?)

// So, best-case, with direct implementation, with DE active:
//    (not inverting anything, etc)
//    B3 is always low
//    B2 is selectable
//    B2           = 4
//    B2, B5, B4   = 52
//    B2, B5       = 36
//        B5       = 32
//        B5, B4   = 48
// Not as much range-increase as I'd expected, maybe another way
// For now, it seems it's plausible to at least reimplement what was
// already.










// One potential option is to use the look-up-table
// This gives a value based on the ramp-step number (in a 4-ramp
// configuration)

//  Ramp# (aka lookup-table index)
//    0     1     2     3
// |-----|-----|-----|------|
//
//
// So how many cases are there with two channels and assuming we match the
// old configuration where there's only one active period that gets shifted
// left/right and grows/shrinks in width...?

// |________________________|
// |________________________|

//       0        1
// |____________¯¯¯¯¯¯¯¯¯¯¯¯|
// |________________________|

//   0         1    2
// |____________¯¯¯¯¯¯¯¯¯¯¯¯|
// |__________¯¯¯¯¯¯¯¯¯¯¯¯¯¯|

//   0           1   2
// |____________¯¯¯¯¯¯¯¯¯¯¯¯|
// |_______________¯¯¯¯¯¯¯¯¯|

//   0           1  2 3
// |____________¯¯¯¯¯_______|     This sort've matches one-ramp...
// |_______________¯¯¯¯¯¯¯¯¯|     Since this always changes at the end.

//   0           1  2 3  4
// |____________¯¯¯¯¯_______|     NOGO.
// |_______________¯¯¯¯¯¯___|

// So, it looks like it's plausible to use for a few cases, and certainly
// all those that one-ramp would allow
// but doesn't allow for completely arbitrary waveforms
// And when combined with the additional math, it'd be complicated
// (math used to determine where to place each ramp-edge, since
// order-matters and would have to be sorted between red/green)



/// So back to one-ramp mode...
// Blue/CLK:
//
// CLK:
// OCR2RB = 6 (starting from 0)
// OCR2SB = 3
//
// H(only):	OCR2SA = 2, OCR2RA = 3
// V w/o H: OCR2SA = 3, OCR2RA = 4
// V w/ H : OCR2SA >=6, OCR2RA = 0 ....???
// Nada   : OCR2SA = 2, OCR2RA = 4
// 
// DE/Blue: 
// DE:          B3   B2   DE   /V   /H   B5   B4  For BLUE DATA (not D/V/H)
// "Blue/DVH" |     ____ ____.____.____ ____ ____      S=OCR2SA
//  OC1A:     | S0>/ S1>/           R4>\ R5>\ R6...    R=OCR2RA
//            |¯¯¯¯ ¯¯¯¯                ¯¯¯¯ ¯¯¯¯
// This closely matches the Tiny861's setup, where S=D+1, R=C
// PWM161 = Tiny861
// ----------------
// OCR2SA = DT - 1
// OCR2RA = OCR1A
//

//ANOTHER DIFFICULTY:
//  synchronizing PSC with PSCR (inputs?)
//
// PSCR: "In one ramp mode, Retrigger Inputs ... reset the current PSCR
// counting to zero"
// PSCR allows using the Analog Comparator output as the PSCR Input...
// If PSC2 does, as well, then both should be sync-able via AnaComp
//
// Which, too, might be doable in software, entirely, via driving an
// output-value on the same pin as the anacomp input...? Or maybe selecting
// an internal source (thermal?) ...
// Input Mode 8, p166
// (Might be difficult if they use prescalers other than 1, since who knows
// where the prescaler-counters are with respect to PSCR and PSC2)
//




// More colors (Red/Green)?
//  Possibly, sync PSC with PSCR at the other LVDS-CLK edge
//  (they'd be out of phase)
//  Then the OCR2SB-ish signal which causes the one-ramp reset
//  could be located at a much lower color-bit-value

// Green Notes:
// (Assuming Green is connected to OCR2SB-ish, and thus required to be high
// until the 7th bit, for resetting the one-ramp mode)
//   On the ATTiny861 green has basically three values based on the
//   dead-timer from the lvds-clock edge...
//      G5 = 32/63, G5+G4 = 48/63, G5+G4+G3 = 56/63
//   PWM161 is reversed!
//     G3 = 8/63, G3+G4 = 24/63, G3+G4+G5 = 56/63
// Interesting side-note???
//    One is ~logarithmic, and the other is ~exponential?
//   BUT MAYBE the same could be accomplished by inverting Green again...?
//


#if (defined(REMOVE_DC) && REMOVE_DC)
 #error "REMOVE_DC hasn't been tested since lcdStuff, or long prior"
 #error "Further, it hasn't been implemented AT ALL for the PWM161"
#endif

//Is +1 true, since DT adds its own delay...? Probably, since DT adds the
//delay to *all* channels...
//#define DT1_EQUIV(x)   (OCR2SA = (x)-1)

#define lockPSC2()		(setbit(PLOCK2, PCNF2))
#define unlockPSC2()		(clrbit(PLOCK2, PCNF2))

#define lockPSC0()      (setbit(PLOCK0, PCNF0))
#define unlockPSC0()		(clrbit(PLOCK0, PCNF0))

#define FULL_INIT_TESTS	TRUE
#warning "NEED TO REVISIT TRANSITIONS, Especially DE->setBlue"

#define Nada_init()     { lockPSC2(); OCR2SA=2; OCR2RA=4; unlockPSC2(); }
//#error "Check these again with ATtiny861..."
//#error "Kept forgetting that V and H are active-low"
//Unused, normally...
#define Vsync_init()    { lockPSC2(); OCR2SA=3; OCR2RA=4; unlockPSC2(); }
#define VplusH_init()   { lockPSC2(); OCR2SA=6; OCR2RA=6; unlockPSC2(); }
#define Hsync_init()    { lockPSC2(); OCR2SA=2; OCR2RA=3; unlockPSC2(); }
#define DEonly_init()   { lockPSC2(); OCR2SA=1; OCR2RA=4; unlockPSC2(); }
//#define DEonly_init()   { lockPSC2(); OCR2SA=0; OCR2RA=4; unlockPSC2(); }
#define DEblue_init()   { lockPSC2(); OCR2SA=0; OCR2RA=6; unlockPSC2(); }

#if (defined(DE_ONLY_DISPLAY) && DE_ONLY_DISPLAY)
 #error "DE_ONLY_DISPLAY is not implemented for PWM161"
 #error " But don't be discouraged... an actual LCD that's DE-Only"
 #error " should work fine WITHOUT DE_ONLY_DISPLAY set true"
 #error " Really, DE_ONLY_DISPLAY was just a test... see lvds.c"
#elif (defined(FULL_INIT_TESTS) && FULL_INIT_TESTS)
//#error"OK"
 #define Vsync_fromNada()      Vsync_init()
 #define VplusH_fromVsync()      VplusH_init()
 #define Vsync_fromVplusH()      Vsync_init()
 #define Nada_fromVsync()      Nada_init()
 #define Hsync_fromNada()      Hsync_init()
 #define Nada_fromHsync()      Nada_init()
 #define DEonly_fromNada()      DEonly_init()
 #define DEblue_fromDEonly()   DEblue_init()
 #define Nada_fromDEonly()      Nada_init()
 #define Nada_fromDEblue()      Nada_init()
 #define DEonly_fromDEblue()   DEonly_init()
#else   //NOT FULL_INIT_TESTS
//These are the minimal changes necessary, for high-speed processing...
 #define Vsync_fromNada()   { lockPSC2(); OCR2SA=3; unlockPSC2(); } 
 #define VplusH_fromVsync() VplusH_init()
	//Might be able to get away with merely changing OCR2SA to 6... then
	//reset would occur before set, and set would never occur...?
#define Vsync_fromVplusH() Vsync_init()
	// See above
#define Nada_fromVsync()   { lockPSC2(); OCR2SA=2; unlockPSC2(); }
#define Hsync_fromNada()   { lockPSC2(); OCR2RA=3; unlockPSC2(); }
#define Nada_fromHsync()   { lockPSC2(); OCR2RA=4; unlockPSC2(); }
#define Nada_fromDEblue()  Nada_init()
#define Nada_fromDEonly()  { lockPSC2(); OCR2SA=2; unlockPSC2(); }
#define DEonly_fromNada()  { lockPSC2(); OCR2SA=1; unlockPSC2(); }
#define DEblue_fromNada()  DEblue_init()
#define DEblue_fromDEonly() DEblue_init()

#define DEonly_fromDEblue() DEonly_init()

#endif

#define fullBlue()	DEblue_init()

// So far, there's no "noBlue()" like there is (now) for red/green
// it gets confusing since doing-so would also affect the timing signals


//This'll be a macro, eventually...
// but I want these warnings/errors...
/*void fullRed(void);

#if(defined(ALLOW_UNIMPLEMENTED) && ALLOW_UNIMPLEMENTED)
 #warning "UNIMPLEMENTED FUNCTION: fullRed()"
#else
 #error "UNIMPLEMENTED FUNCTION: fullRed()"
#endif  

void fullRed(void)
{

}
*/
//For now, we're assuming the Set/Reset active-levels are
//directly-corresponding to the FPD-Link bit-values...
// e.g. "OCR0SA" = set the output, so the corresponding bit is high
//THIS MIGHT CHANGE
// e.g. inverting "green" might allow for the color-scheme to be stretched
// more normally...
// There're a couple ways to do this, and it's too confusing for me to
// parse right now... invert the output, or flip the LVDS-converters...?
//#define fullRed()	{ lockPSC0(); OCR0SA = 0; OCR0RA = 6; unlockPSC0(); }
//#define noRed()	{ lockPSC0(); OCR0SA = 6; OCR0RA = 0; unlockPSC0(); }	//Would this be allowed?
																//Would its ON be *around*
																//the counter-reset?
//#define noRed()	{ lockPSC0(); OCR0SA = 6; OCR0RA = 6; unlockPSC0(); }
#define noRed()	setRed4(0)
#define fullRed()	setRed4(3)

//#define fullGreen()	{ lockPSC0(); OCR0SB = 0; unlockPSC0(); }
//#define noGreen()		{ lockPSC0(); OCR0SB = 6; unlockPSC0(); }
#define noGreen()	  setGreen4(0)
#define fullGreen() setGreen4(3)


//These are most-likely unnecessary for PWM161
#define lvds_disableGreen_MakeClockInsensitiveToDT()
#define lvds_enableGreen_MakeClockSensitiveToDT()


void lvds_timerInit(void);

//This is causing display-syncing problems...
#define LVDS_MAXIMIZE_OSCCAL TRUE
#define LVDS_USE_PLL	TRUE

#if(defined(LVDS_USE_PLL) && LVDS_USE_PLL)
 // 0 selects "Slow Clock Input" aka systemI/O-clock
 // 1 selects "Fast Clock Input" aka PLL_CLK
 // This is the case for both PSC2 and PSCR
 #define PCLKSELn	1
#else
 #warning "LVDS_USE_PLL is not TRUE... using system clock"
 #define PCLKSELn 0
#endif


#if(defined(PLL_SYSCLK) && PLL_SYSCLK)
 #warning "The AT90PWM161 has not yet been configured to use PLL_SYSCLK"
 #warning "Math relying on it may be messed-up... e.g. boot PLL_MULT=6"
// #error "Comment-out the line 'PLLSYSCLK=1' in your makefile"
#endif




//These notes prior to usage of delayCyc to synchronize the PSCs
// delayCyc may have its own problems, which are not yet apparent, so I'll
// leave these notes...
//
//Notes re: retriggering (restarting the counter from 0)
// PSC2 retrigger sources:
//    PSCIN2, PSCIN2A
//    AC2O, AC3O	-- Analog Comparator outputs
// PSCR retrigger sources:
//    PSCIN0, PSCIN0A, PSCIN0B
//    AC1O  -- Analog Comparator output
//
// None are directly-internally shared...
// PE1 (pin 7) ?
//   ACMP1_OUT -> AC1O
//   PSCIN2
// Using PE1 may introduce a delay from ACMP1_OUT -> PSCIN2, despite the
// fact they're on the same pin, since the digital input sampling occurs on
// a specific CPU clock edge...
// OTOH, since it's a PSC input, it may handle it differently
// Even still, the Analog Comparator output goes straight to AC10
//  but traverses through a multiplexer to ACMP1_OUT
//  which may introduce a slight delay
// IOW: It may work great at CPU speeds, but with the PLL...???
//
// The only other alternative I can think of is to try to align the PLL
// clock with the system-clock, somehow, and count the number of
// instructions between the start of one PSC and the start of the next...
// (Noting that the *start* of one must align with the 7-bit nature of the
// other) (56 instructions?)
//
// What about starting them without the PLL running, then starting the PLL?


#endif //__LVDS161_H__

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
 * /Users/meh/_avrProjects/LCDdirectLVDS/93-checkingProcessAgain/_interfaces/lvds161.h
 *
 *    (Wow, that's a lot longer than I'd hoped).
 *
 *    Enjoy!
 */

/* mehPL:
 *    This is Open Source, but NOT GPL. I call it mehPL.
 *    I'm not too fond of long licenses at the top of the file.
 *    Please see the bottom.
 *    Enjoy!
 */



/*
This is an old note from main() upon init...
And, actually, the Timer0 stuff may not be related to the comment.
  I have no idea how it functioned, it seems the rest was deleted.
   //For synchronizing timer1 settings (to avoid glitches)
   // count the number of CPU cycles...
   // depending on how many cycles it takes to start this and whatnot, 
   // there may be an offset. But the jist is we know every 7 cycles
   // the timer1 reset is synchronized with the CPU instructions...
// OCR0A = 6;
// timer_setWGM(0, WGM_CLR_ON_COMPARE);
// timer_selectDivisor(0, CLKDIV1);
*/




//a/o v62: Are these necessary if NOT using writeColor??? TODO

//#############
//For Red and Green (NOT Blue) This enables four shades, instead of three
// (including black)
// Doing so increases pixel-processing time, thus the pixel-widths
// (thus decreasing resolution)
// each color takes 9 cycles to process in three-shade mode
// or 12 cycles for red and green, plus 9 for blue in four-shade mode
// a/o v59: I don't think this does anything in ROW_SEG_BUFFER
#define FOUR_SHADES TRUE


//a/o v60
//I can't find this anywhere else... Might not be looking hard enough
#define	NUM_COLORS	48
//##############



//a/o v60: These are old notes, as well...
// As I recall, there was some issue with DC signals on the LVDS lines.
// I think this was when I was attempting to use AC-Coupling (long before
// the 74LS86 was deemed functional)
// and probably has no relevence now.
//DC_DE is only used in drawPix...
//#define DC_DE_DISABLE TRUE
//This should remove DC from Vsync, etc...
// Currently only implemented with FULL_INIT_TESTS below...
//#define REMOVE_DC TRUE

//There are two methods for configuring the PWM channels to output
// pseudo-serial data for changes between timing modes... 
// (e.g. Horizontal Front Porch -> Hsync -> Back Porch -> Data Enabled)
// * Write all the necessary/related registers upon each transition
//		Leads to redundancy, as many of the registers are already properly
//    configured from the last transition.
// * Write only the changed registers
//    (As I recall, it was possible to have only a single register-change
//     on most, if not all, transitions...)
//
// The benefit of the latter being: Since the pixel-clock is generally
// faster than the processor-clock, or at least not-aligned, reducing the
// number of instructions to *one* would allow for glitchless transitions
// in the actual PWM signal... guaranteeing that at least the data being
// transmitted would conform to FPD-Link compatible signals...
//  (no glitches that might, e.g. cause a couple serial-frames to send both
//   Data Enable *and* VSync active at the same time. Or, e.g. to send a
//   serial-frame which indicates an H-sync while transitioning between
//   the H-Back-Porch and Data-Enable)
//   Since the only display I've gotten working via LVDS, so far, is 
//    DE-Only, these tests and the importance of these glitches will
//    probably have to be revisited when working with a display that pays
//    attention to the Hsync and Vsync signals.
// FULL_INIT_TESTS uses the first option, which guarantees that if I missed
// some register-transition (while coding), that at least it *should* send
//  the proper signals (after the transition).
//  (The transition-time may, of course, lead to incompatible signals)
// So, it's mostly for testing, and should be removed if possible.
//#define FULL_INIT_TESTS TRUE

//This should just reduce the number of instructions which actually get
// executed, since Hsync doesn't need to be sent, etc.
// Actually, I'm working with a DE-Only display right now, and this is NOT
// set to TRUE... I haven't tested it with this TRUE
//#define DE_ONLY_DISPLAY  TRUE



//PWM Timing:
// (ATTiny861, Timer1, FastPWM, PWM1X;
//  a/o LCDdirectLVDS1_5_PWMtimingTests):
//
//  PWM output on OC1A
//    if OCR1A = 0, OC1A remains constant High
//       OCR1A = 1, OC1A is low for 2 counts
//       OCR1A = 2, OC1A is low for 3 counts
//       OCR1A = 3, OC1A is low for 4 counts
//       OCR1A = OCR1C, OC1A remains constant Low
//
//    THUS:
//       compare-match occurs when TCNT changes AWAY from match
//         (assuming TCNT starts at 0 for one pulse, 1 for one pulse...)
//       TOP (OCR1C) is included in the count...
//       There is no single-count pulse-width
//         (Though, it seems dead-time could simulate it...)
//         (set a deadtime of 1 on the BOTTOM edge and OCR1A)
//         (but then there's no high for only one clock)



// Rather than *possibly* mess with timing (on other displays?)
// Also for testing...
//#define DT0_BLUES_ONLY   TRUE

//A/O v13: Using the Samsung LTN display, instead of the IDTech IAXG
// LTN appears to be content with my pseudo-LVDS scheme.
// IAXG has never unblanked
//      Even though suitable timings were found with SwitchResX
//      Maybe due to psuedo-LVDS
//            slight timing issues // It probably doesn't work with all cases...
// Definitely with drawPix/Images...(RC oscillator variances?)
//            glitches when switching LVDS states 
//                 (thought I had that figured out, originally)
//      The fact the LTN appears to recognize the signalling suggests the
//      IAXG would be worth further exploration...
//         Unfortunately, the CCFT blew out my inverter
//                        And I must have put it back together incorrectly
//                        (backlight filters out of order or flipped?)
//                        (which actually makes for some very interesting
//                         visuals, but hard to develop with)
//      IAXG: uses DE, V, and H
//             At low pixel-clock DE is active for fewer pixels...
//              Last Tested: 680 was full-screen
//             Nice because it increases the frame-rate!
//      LTN:  uses DE only


// The idea is to use FastPWM with the PLL to implement 64-85Mbits/sec
//  (the PLL on the Tiny861 supposedly maxes out at 85MHz)
//  (Though I am currently running with OSCAL set to the highest frequency
//   and the PLL seems to be syncing at about 128MHz)


// Wiring:
//   Many iterations of AVR->Differential "LVDS" circuitry resulted in the
//   simplest of all:
//
//   Believe it or not, the XOR is a standard TTL LS-series XOR: 74LS86
//     Specifically: TI SN74LS86N from 1980 (the only XOR in my collection)
//   It's spec'd to run from 4.5-5.5V, and its propagatio delays and slew
//     rates aren't really spec'd to be good enough for 128MHz pixel clock
//     yet it's working...
//   Further, the output voltages are right in the LVDS range,
//     IIRC (last I 'scoped) around 1.5V High and 1.0V Low
//      (Don't forget the LCD has a 100ohm resistor between
//         RXinN/clk+ and RXinN/clk-)
//   Most signals are connected as shown (RXin0, RXin2, RXclk)
//   Green is the only one which has RXin1- and RXin1+ swapped
//     since it is on the /OC1B (inverted) output
//   Green may benefit from a pull-up resistor on /OC1B
//     there was some weird noise appearing like a floating-input
//     when the full frame was not filled with pixels
//     (but it should've waited to disable /OC1B until *after* the delays,
//      etc. So I'm not sure what it was)
//
//   It's probably best to use two XORs from the same chip for a single
//    LVDS channel, since different chips may have slightly different
//    characteristics. 
//
//   The entire circuit, thus, requires TWO 74LS86's 
//    (four XORs apiece, two per LVDS channel, 8-total)
//
//
//              VCC3V3   VCC3V3
//                |        |
//                +---\ \¯¯¯-_
//                     | |    ¯-
//                     | | XOR   >------> RXinN/clk-
//   AVR               | |    _-
//   OC1x >----+------/ /___-¯
//   output    |
//             |   
//             |      
//             `------\ \¯¯¯-_
//                     | |    ¯-
//                     | | XOR   >------> RXinN/clk+
//                     | |    _-
//                +---/ /___-¯
//                |        |
//               GND      GND
//
//  
//  Also used: The TTL 74AHC series...
//    I found some one-gang 74AHC1G32 and 74AHC1G86's on an old iBook
//     motherboard. (An OR and an XOR, respectively). These are spec'd for
//     3.3V operation, and faster. The output voltages appear OK for LVDS
//     (with a 100ohm load in the LCD)
//
//  NOTE: Since I only had enough of these 1-gang devices for two LVDS
//     channels, I had to implement Red and Green with the LS 
//     before switching all channels to the LS... Using different chips
//     (specifically, different TYPES of chips) for different channels
//     caused timing issues: Since the AHC is faster, the clock and Blue
//     signals are synchronized, but the red and green signals were shifted
//     a bit or two (resulting in "Black" appearing green, of course
//     true black isn't really possible with my timing scheme... see below)
//
//  For other circuits attempted, see oldNotes.txt
//     (and boy there were many, involving voltage dividers, AC coupling,
//      reference voltages, BJT differential amplifiers... I doubt I 
//      documented them all, or even most. Whoda thunk the simplest, 
//      especially under- AND over-spec'd--timing, supply voltage, and
//      output voltage--would be the one...?)
//
// LVDS/FPD-Link timing:

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
// Ponderings on using /OC1x's and OC1D for other colors...
//    /OC1D could easily be used for another color, unaffected by others
//          since DT1(L) and OCR1D are unused
//
// DE:   Blue Values
//       -----------
//       OCR1A=4,5,6   (OCR1A=6, Full-Blue would force /OC1A low,
//       DT1(H)=0,1,2                      e.g. Green=Full-Green or Off)
//                      Also: OCR1A=4,5 would affect Green
//                            (DT1(L) is from this edge...)
//                            DT1(H) also affects complementary OC1D
//
// CLK:  OCR1B=3       CLOCK can NOT be complementary-output mode
//                                   (/OC1B unusuable)
//                                   otherwise, DT1 would affect clock
//
//
// CLOCK_INSENSITIVITY_TESTING:
//   (a/o 56-36-3ish, no longer testable)
//    Testing of DE/Blue's DeadTime values on Clocking...
//       Image-shift (not sure why, more calculations? Not *that* many!)
//       Blue now has only two shades besides black
//         ~66% and 100%
//       Now each blue pixel (the ones appearing black)
//         is bordered by a blue line...
//    All doable. Would have preferred 3 shades besides black,
//       but two isn't bad.
//
//  New Idea: Since CLOCK can be used with DT1H (during DE)
//    DE/Blue DT values are 0, 1, or 2 (D0, D1, D2)
//     Dn corresponds to the low-to-high dead-timer value
//                   (aka counter-reset delay, on OCR1x)
//     dn corresponds to the high-to-low dead-timer value
//                   (aka OCR match delay, on /OCR1x)
//     Cn corresponds to OCR1x=n
//
// This was easier to comprehend in v<26... now it's more informative
//   but harder to view...
//                          OCR1B = 3
//  TCNT:     |  0   1    2    3  | 4    5    6
//
//  CLKideal: /¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯\______________/
//
//            |____ ____ ____.____v              |
//  CLK:   D0>/ D1>/ D2>/         \    .    .    /
//  OC1B      |¯¯¯¯ ¯¯¯¯          |¯¯¯¯ ¯¯¯¯ ¯¯¯¯|
//            |                   |
//            | G2   G1   B1   B0 | G5   G4   G3
//  Green:    |                   |____ ____ ____
//  /OC1B     \    .    .    . d0>/ d1>/ d2>/ d3?\  //
//            |¯¯¯¯ ¯¯¯¯ ¯¯¯¯ ¯¯¯¯ ¯¯¯¯ ¯¯¯¯ ¯¯¯¯
//            |
//            | R1   R0   G0   R5   R4   R3   R2
//  Red:      |____ ____ ____ ____ ____ ____ ____
//  OC1D   D0>/ D1>/ D2>/ C2>\ C3>\ C4>\ C5>\ C6...
//             ¯¯¯¯ ¯¯¯¯      ¯¯¯¯ ¯¯¯¯ ¯¯¯¯ ¯¯¯¯
//             ____ ____ ____ ____ ____ ____ ____
//  /OC1D     \ ^OCR1D>=6^ L>/  M>/  N>/  O>/ P?>...
//             ¯¯¯¯ ¯¯¯¯ ¯¯¯¯ ¯¯¯¯ ¯¯¯¯ ¯¯¯¯ ¯¯¯¯
//
//              B3   B2   DE   /V   /H   B5   B4
// "Blue/DVH" |____ ____ ____.____.____ ____ ____
//  OC1A:  D0>/ D1>/ D2>/           C4>\ C5>\ C6...
//            |¯¯¯¯ ¯¯¯¯                ¯¯¯¯ ¯¯¯¯
//             ____ ____ ____ ____ ____ ____ ____
//  /OC1A:    \  ^^^--OCR1A>=6--^^^  X>/  Y>/ Z?>\  //
//  (usable?)  ¯¯¯¯ ¯¯¯¯ ¯¯¯¯ ¯¯¯¯ ¯¯¯¯ ¯¯¯¯ ¯¯¯¯
//                 X: OCR1A=4, DTL=0
//                 Y: OCR1A=4, DTL=1; OCR1A=5, DTL=0
//                 Z: OCR1A=4, DTL=2; OCR1A=5, DTL=1; OCR1A>=6

//I've looked it over extensively, albiet exhaustedly, and it seems
// there's only one way to do this. Unfortunately, DTH=1,2 *does* affect
// the clocking. Everything displays properly, but the lighter shades of 
// blue enabled by DTH=1,2 don't display at all (or dang-near black)
// So there's really no benefit to using DTH=1,2 during DE for blues...
// That gives two (similar) shades of bright blue 63/63 and 47/63
// *'d are arbitrarily chosen for implementation
// F'd are selected when FOUR_SHADES is TRUE
//   (experimentation might show other choices are better)
//
// Red is connected to OC1D, Complementary output is not necessary
//  OCR1D determines its brightness:
//   (+OC1D => RX0+)
//   *Off (0/63): OCR1D = 0  \      //
//    3/63:         OCR1D = 1   > These three appear identical in glColorTest
//    3/63:         OCR1D = 2  /  (G0 Active, from here down)
//   *35/63:      OCR1D = 3
//   F51/63:      OCR1D = 4
//    59/63:      OCR1D = 5  \  These two appear identical in glColorTest
//   *63/63:      OCR1D >= 6 /
//    (+OC1D => RX0-)
//    Not really worth pursuing, next step from 60/63 is 28/63 then 12
//
// Green is connected to /OC1B (the complementary channel to CLK)
//  Its polarity is reversed (but that's easy since we have a single-ended
//  to differential converter, and its outputs can just be swapped)
//  DTL1 determines its brightness (G0 affected by Red):
//   (/OC1B => RX1+)
//    Off (0/63):   DTL1 = 3 (is this possible?)
//    8-9/63:         DTL1 = 2
//    24-25/63:      DTL1 = 1
//    56-57/63:      DTL1 = 0
//   (/OC1B => RX1-)            (B1,0 Active, as well as G2,1)
//     *Off (6/63):   DTL1 = 0
//   *38-39/63:      DTL1 = 1
//   F54-55/63:      DTL1 = 2
//   *62-63/63:      DTL1 = 3
//
// Blue, as in previous versions, is connected to OC1A, DTH=0 during DE 
//  so as not to interfere with the clock
//  OCR1A determines the brightness (B1,0 active, per Green->RX1-):
//    (+OC1A => RX2+)               (B3,2 Active from here down)
//    *Off (15/63):   OCR1A=4
//    *47/63:          OCR1A=5
//      *63/63:         OCR1A=6
//
// The clock is single-ended (complementary-mode disabled) during NON-DE
//  because the DE/V/H signals (except in DE mode) require DT1H to vary.
//  When DE is active the clock channel (OC1B) is switched to 
//  complementary-output-mode to enable the Green PWM output
//
// For easier viewing:
//   Red: (+OC1D => RX0+)
//    Off (0/63): OCR1D = 0
//    35/63:      OCR1D = 3
//    51/63:      OCR1D = 4   (FOUR_SHADES only)
//    63/63:      OCR1D >= 6
//   Green: (/OC1B => RX1-)          (B1,0 Active, as well as G2,1)
//    Off (6/63): DTL1 = 0
//    38-39/63:      DTL1 = 1
//    54-55/63:      DTL1 = 2   (FOUR_SHADES only)
//    62-63/63:      DTL1 = 3
//   Blue: (+OC1A => RX2+)               (B3,2 Active from here down)
//    Off (15/63):  OCR1A=4
//    47/63:        OCR1A=5
//    63/63:        OCR1A=6
//

// Toward creating a GIMP palette... v54.5
// Probably absurd, but this was brown/orange on GIMP and it's yellow here
//
//   Red: (+OC1D => RX0+)
//    Off (0/63): OCR1D = 0   r=0
//    35/63:      OCR1D = 3   r=142
//    51/63:      OCR1D = 4   r=206
//    63/63:      OCR1D >= 6   r=255
//   Green: (/OC1B => RX1-)          (B1,0 Active, as well as G2,1)
//    Off (6/63): DTL1 = 0      g=24
//    38-39/63:      DTL1 = 1 g=154
//    54-55/63:      DTL1 = 2   g=218
//    62-63/63:      DTL1 = 3 g=251
//   Blue: (+OC1A => RX2+)               (B3,2 Active from here down)
//    Off (15/63):  OCR1A=4   b=61
//    47/63:        OCR1A=5   b=190
//    63/63:        OCR1A=6   b=255
//
//  These numbers don't look entirely accurate...
//    they vary depending on the other colors... and why is blue so high
//    even when it's off?
//  Maybe I'm looking at old notes...?
//   SEE lvdsColorExperiments.c Now Here.

//  Implementations/prospects:
//  * rowBuffer.c (more like row-settings-buffer)
//     calculate a row's worth of pixels before drawing that row
//     (uses packed color settings in a single byte per drawable pixel)
//     64 drawable pixels across, regardless of LVDS speed
//  * For faster pixels: these settings values could be stored 
//     in individual bytes. Gives about 1/3 more pixels at 3x the memory
//     (not implemented)
//  * rowSegBuffer.c
//     also calculates an entire row before drawing it
//     instead of storing pixels, store "segments"
//     i.e. each segment is defined by a color value and a length
//      the color-value is stored as DT/OCR values (not RGB values)
//     Number of segments is limited only by memory...
//      e.g. 64 segments per row (max) is 64*3Bytes
//       Three bytes for color, and *really simple* packing for seg-length
//      BUT: at slow LVDS speeds, the resolution of these segments could be
//       as high as one LCD pixel.
//      possibly: at *really* low LVDS speeds we could be 64*2Bytes
//      (with packing)
//      Actually: Using GB_COMBINED gives 2Bytes per segment
//      only adds two clock cycles to each "pixel", so probably worth it

//   In Any Case: There's not enough RAM for a full frame
//          64 pixels across * 64 pixels down is 4096 bytes
//          So whatever method, we need to precalculate each row before
//          displaying it
//          Could be as simple as loading direct from program memory
//

// Since low-bits are barely visible, their effect is neglected.
//   Thus: Green is affected only by the Compare-Match deadTimer (DT1L)
//         Red is affected only by OCR1D, and could be single-ended
//   OTOH:
//         The Visible blue values (with DT affecting clock)
//         are only the high-two values, which are affected only by OCR1A
//         So counter-reset dead-time (DT1H) needn't be varied
//         And, then, the clock won't be affected at all
//         (assuming we set it to single-ended during DE disabled, for V/H)
//
// NEW CONSIDERATION: "The dead timer delays the waveform by a minimum of
//    "of one count, when DT=0..."
// So changing the clock from single-ended to complementary
//   might actually cause the clock output to be shifted!


// DE/V/H Timing (LCDdirectLVDS):
//  
//
//            |  0   1    2    3    4    5    6       All: set @ 0
//            |____.____.____.____                         OCR1C = 6 
//  Clock:    /                   \    .    .    /         Complementary-
//                                 ¯¯¯¯ ¯¯¯¯ ¯¯¯¯          Output Mode
//                                                          required for DT
//   Signal:    B3   B2   DE   /V   /H   B5   B4 | B3
//             ____ ____ ____ ____ ____ ____ ____|____    
//   DE:      X B3 X B2 /    .    .    \ B5 X B4 X B3 X    
//   state2    ¯¯¯¯ ¯¯¯¯                ¯¯¯¯ ¯¯¯¯|¯¯¯¯    
//   DE_BLUE: >¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯|    DT=X, OCR=0xff
//                         Watch the transition!! -------^
//   DE_NORM: >_________/¯¯¯¯¯¯¯¯¯¯¯¯¯¯\_________|    DT=2, OCR=4
//   DC_DISABLED:
//   maxBlue: >¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯\____|      DT=0, OCR=5
//             See below for more blue settings...
//
//             ____ ____      ____      ____ ____|____ 
//  H (only): X xx X xx \    /    \    / xx X xx X xx X    
//   state1    ¯¯¯¯ ¯¯¯¯ ¯¯¯¯      ¯¯¯¯ ¯¯¯¯ ¯¯¯¯|¯¯¯¯ 
//            >______________/¯¯¯¯\______________|    DT=3, OCR=3
//  DC_DISABLED:
//   Not much can be done...
//
//             ____ ____           ____ ____ ____|____ 
//  V w/o H:  X xx X xx \    .    /    \ xx X xx X xx X
//   state3    ¯¯¯¯ ¯¯¯¯ ¯¯¯¯ ¯¯¯¯      ¯¯¯¯ ¯¯¯¯|¯¯¯¯
//            >___________________/¯¯¯¯\_________|    DT=4, OCR=4 (+?)
// DC_DISABLED:
//            >___________________/¯¯¯¯¯¯¯¯¯\____|      DT=4, OCR=5

//             ____ ____                ____ ____|____ 
//  V w/ H:   X xx X xx \    .    .    / xx X xx X xx X
//   state4    ¯¯¯¯ ¯¯¯¯ ¯¯¯¯ ¯¯¯¯ ¯¯¯¯ ¯¯¯¯ ¯¯¯¯|¯¯¯¯ 
//            >__________________________________|    DT=X, OCR=0
//                             TransitionWatch!!! -------^
//                             Shouldn't matter... DT from no-edge
// DC_DISABLED:
//            >¯¯¯¯¯¯¯¯¯\________________________|      DT=0, OCR=1

//             ____ ____      ____ ____ ____ ____|____ 
//  Nada:     X xx X xx \    /    .    \ xx X xx X xx X    
//   state0    ¯¯¯¯ ¯¯¯¯ ¯¯¯¯           ¯¯¯¯ ¯¯¯¯|¯¯¯¯     
//            >______________/¯¯¯¯¯¯¯¯¯\_________|    DT=3, OCR=4 (+?)
// DC_DISABLED:
//            >______________/¯¯¯¯¯¯¯¯¯¯¯¯¯¯\____|      DT=3, OCR=5

// Transitions: OLD!!!!! WRONG!!!!
//   (are they? There're several additional transitions, now, for one.)
//  The idea is to reduce the number of instructions between each
//  LVDS "state."
//  These are implemented below in the case:
//    "#else //NOT FULL_INIT_TESTS"
//  Since each instruction takes *longer than* a single pixel
//    two instructions would *at best* occur on two consecutive pixels
//    Thus, there's likely to be a transition-glitch
//    (e.g. old OCR value with new DT value)
//  Note that the new DT values are implemented at the next corresponding
//    edge after the DT-write instruction completes
//    (for +OC1A/DT1H, when TCNT is reset to 0)
//    (for -OC1A/DT1L, when TCNT passes OCR1A)
//    New OCR values are delayed until the next TCNT reset to 0
//
//  Initial:
//  Nada               DT=2  NADA_OCR = (3<=OCR<(=?)6)

//  Nada   ->   H         OCR=2
//  H      ->   Nada      OCR=NADA_OCR
//  
//  Nada   ->   V         DT=3
//  V      ->   V+H      OCR=0
//  V+H   ->   V         OCR=NADA_OCR
//  V      ->   Nada      DT=2
//
//  Nada   ->   DE         DT=1
//  DE   ->   Nada      DT=2


#if (defined(REMOVE_DC) && REMOVE_DC)
#error "REMOVE_DC hasn't been tested since lcdStuff, or long prior"
 #define Nada_init()         { DT1=(3<<4); OCR1A=5; }

 //Unused, normally...
 #define Vsync_init()       { DT1=(4<<4);  OCR1A=5; }
 #define VplusH_init()      { DT1=0;       OCR1A=1; }
 #define Hsync_init()       { DT1=(3<<4);  OCR1A=3; }
 #define DEonly_init()      { DT1=(2<<4);  OCR1A=4; }
 #define DEblue_init()       { DT1=0;       OCR1A=5; }

#else //!REMOVE_DC
//Nada init
 #define Nada_init()         { DT1=(3<<4); OCR1A=4; }

//Unused, normally...
 #define Vsync_init()         { DT1=(4<<4);   OCR1A=4; }
 #define VplusH_init()      { DT1=0;         OCR1A=0; }
 #define Hsync_init()         { DT1=(3<<4);   OCR1A=3; }
// #define DE_init()            { DT1=(2<<4);   OCR1A=4; } //...SHOULD BE
 #define DEonly_init()         { DT1=(2<<4);  OCR1A=4; }
 #define DEblue_init()         { DT1=0;       OCR1A=6; }
//#define DE_init()            { DT1=(1<<4);   OCR1A=2; } //Works with DE_ONLY
 //#define DE_init()            { DT1=0;         OCR1A=0xff; }   //DE_BLUE
#endif //REMOVE_DC



//Display is DE-Only (doesn't use H/Vsync)
// Shouldn't be necessary to select this if it is,
// since DE timing is the same either way
// but I want to test whether a single-bit is being detected
// (e.g. maybe the rise/fall-times of the output aren't fast enough for a
//  single bit, which might explain why the other display didn't work)
#if (defined(DE_ONLY_DISPLAY) && DE_ONLY_DISPLAY)
#define Vsync_fromNada()      Nada_init()
#define VplusH_fromVsync()    Nada_init()
#define Vsync_fromVplusH()    Nada_init()
#define Nada_fromVsync()      Nada_init()
#define Hsync_fromNada()      Nada_init()
#define Nada_fromHsync()      Nada_init()
#define DEonly_fromNada()     DE_init()
#define DEblue_fromDEonly()   DE_init()
#define Nada_fromDEblue()      Nada_init()
#define DEonly_fromDEblue()	DE_init()
 #if(DE_BLUE)
  #warning "DE_BLUE is true, but not implemented with DE_ONLY_DISPLAY"
  #warning "...The display should be blank"
 #endif
//Use full initialization of each LVDS state
// (might not be good during transitions, but should guarantee
//  steady-state, in case my transitions aren't correct...)
#elif (defined(FULL_INIT_TESTS) && FULL_INIT_TESTS)
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
#define DEonly_fromDEblue()	DEonly_init()

#else   //NOT FULL_INIT_TESTS
//#define Vsync_fromNada()   { DT1=(4<<4); }
#define Vsync_fromNada()   { DT1=(4<<4); OCR1A=5; } //Leave two bits high
                                                    //for easy-scoping
                  //Three would be nicer, but I'm pretty sure OCR=TOP=ON
#define VplusH_fromVsync() { OCR1A=0; }
//#define Vsync_fromVplusH() { OCR1A=4; }
#define Vsync_fromVplusH() { OCR1A=5; }            //Extra bits for scoping
//#define Nada_fromVsync()   { DT1=(3<<4); }
#define Nada_fromVsync()   { DT1=(3<<4); OCR1A=4; } //scoping...
#define Hsync_fromNada()   { OCR1A=3; }
#define Nada_fromHsync()   { OCR1A=4; }

#define Nada_fromDEblue()   { DT1=(3<<4); OCR1A=4; }
#define Nada_fromDEonly()   { DT1=(3<<4); }
#define DEonly_fromNada()   { DT1=(2<<4); }
#define DEblue_fromNada()   { OCR1A=0xff; }
#define DEblue_fromDEonly() { OCR1A=0xff; }

//a/o v62: This hadn't existed previously, necessary for
//BLUE_VERT_BAR_REVERSED
#warning "This is just a guess, piecing together Nada_fromDEblue and whatnot"
//But it seems to work... 
#define DEonly_fromDEblue() { OCR1A=4; }

/*
#if (!defined(DE_BLUE) || !DE_BLUE)
 //Nada -> DE
 //#define DE_fromNada()      { DT1=(1<<4); }   //DT1 = (1<<4);
 #define DE_fromNada()      { DT1=(2<<4); }
 //DE -> Nada
 //#define Nada_fromDE()      { DT1=(2<<4); }   //DT1 = (2<<4);
 #define Nada_fromDE()      { DT1=(3<<4); }
#else
 //Nada -> DE
 //#define DE_fromNada()      { OCR1A=0xff; }   //DT1 = (1<<4);
 #define DE_fromNada()      { OCR1A=0xff; }
 //DE -> Nada
 //#define Nada_fromDE()      { OCR1A=3; }   //DT1 = (2<<4);
 #define Nada_fromDE()      { OCR1A=4; }
#endif
*/
#endif //End Of FULL_INIT_TESTS



// NOTES:
//    DeadTimer requires
//        COM1x1:0 = 01 ("Complementary Compare Output Mode" ?)
//      Complementary Compare Output Mode:
//        OCW1A: cleared on match, set at BOTTOM
//    If PWM1X (pwm inversion) is used, OC1A = !OCW1A
//        (Does not affect DT)
//
// CLOCK:
//    Can NOT use differential mode with:
//       OC1B pin is -differential input (through a resistor)
//    /OC1B pin is +differential input (through a resistor)
//    BECAUSE: DeadTimer affects ALL PWM channels
//

//
//
// ISSUES:
//    Using Dead-Timer does not allow for use of complementary outputs
//      as complementary LVDS inputs... 
//      DeadTimer affects ALL PWM channels in complementary mode
//    (e.g. RXclkin+ on OC1B and RXclkin- on /OC1B is not an option)
//


// The typical patterns look like this (not at all to scale):
//   ----______------------------------------------------___----- V
//      --_--_--_--_--_--_--_--_--_--_--_--_--_--_ H
//      __________-__-__-__-__-__-__-__-__-__-  DE
//                 
//             ^^^^\                                          //blah
//             1234 5?
// Pixels are sent during DE High (basically all the CPU will be used here)
//Ideally, 
//  there won't be any glitches when changing from one state to another
//
//  NOTES: PWM1X inversion affects all PWM channels!
//
//  Unchangeables:
//    FastPWM
//      inverted with PWM1X
//
//  Init (pre 1):
//    DeadTimerRising=1
//
//  The states are:
//   (Not necessarily accurate, just looking into necessary changes)
//   (from Vsync L->H)
//
//   1  NothingActive (long, No DE, VporchFrontTimes)
//      *  1<=DeadTimerRising<=5
//        (OCW1A: set @ BOTTOM, cleared @ OCR1A
//      *  OCR1A == 6
//
//   2  Hsync
//      *  DeadTimer _OFF_ -> Horiz
//        (OCW1A: set @ BOTTOM, cleared @ OCR1A
//        (OCR1A == 6
//   3  NothingActive (short)
//      *  1<=DeadTimerRising<=5
//        (OCW1A: set @ BOTTOM, cleared @ OCR1A
//        (OCR1A == 6
//   4  DE
//        (1<=DeadTimerRising<(=?)5 (>1 for blue pixels?) 
//            (==5 cancelled by OCR1A match?)
//        (OCW1A: set @ BOTTOM, cleared @ OCR1A
//      *  OCR1A = 5 (or 1<=OCR1A<5 for blue pixels?)
//   5  NothingActive(?)
//        (1<=DeadTimerRising<=5
//        (OCW1A: set @ BOTTOM, cleared @ OCR1A
//      *  OCR1A == 6
//
//   7  Repeat 2-5 for each row
//
//   8  NothingActive (long, No DE, VporchBackTimes)
//      ?  1<=DeadTimerRising<=5
//        (OCW1A: set @ BOTTOM, cleared @ OCR1A
//        (OCR1A == 6
//
//   9  V w/o H
//      ?  OFF <= DeadTimer <= (5 - OCR1A)
//     **  OCW1A: cleared at BOTTOM, set at OCR1A
//            ??? What is the effect of changing this while running?
//      *  1 <= OCR1A <= 5
//   10 V w/  H
//      ?   DeadTimer OFF (H -> Low)
//      *   OCW1A: set @ BOTTOM, cleared @ OCR1A
//      *   OCR1A > (=?) OCR1C
//
//   11 Repeat 9-10 for Vsync time...
//

// Here's how it worked pre-lvds:
// HSYNC, VSYNC, and DE refer to the actual pins
// In the LVDS setup, there're modes corresponding to each pin-combination
//
// Timer Interrupt:
//  loadData:
//   HSYNC active
//   HSYNC Low delay
//   HSYNC inactive
//   if(dataEnable)
//    DE active
//     Send row data
//    DE inactive
//  //Prep for next interrupt
//  switch(hsyncCount++)
//    //Vsync H->L (active)
//    1:
//          dataEnable=FALSE
//          VSYNC active
//    //Vsync L->H (inactive)
//    T_Vlow:
//          VSYNC inactive
//    //Start of frame
//    T_VD +(T_Vlow):
//          dataEnable=TRUE
//    //All rows have been displayed
//    V_COUNT +(T_VD+T_Vlow):
//          dataEnable=FALSE
//    //Frame Complete
//    T_DV +(V_COUNT+T_VD+T_Vlow): 
//          hsyncCount=0
//  if(dataEnable)
//    Use the remaining time to load the next row to memory

//  So:
//Interrupt0       End
//    |             |  Interrupt1
//    v             v  v
//  H |_|¯¯¯¯¯¯¯¯¯¯¯¯¯¯|_|¯¯¯¯¯¯¯¯¯¯¯¯¯¯|_|¯¯¯¯¯¯¯¯¯¯¯¯¯¯ ...
// DE ___________________________________________________ ...
//  V ¯¯¯¯¯¯¯¯¯¯¯¯¯|_____________________________________ ...
//       |        ||
// DETIME^--------^|
//                 |
//                 VSYNC active
//                 dataEnable=FALSE (not necessary?)
//
//InterruptT_Vlow   End
//    |             |
//    v             v
//  H |_|¯¯¯¯¯¯¯¯¯¯¯¯¯¯|_|¯¯¯¯¯¯¯¯¯¯¯¯¯¯|_|¯¯¯¯¯¯¯¯¯¯¯¯¯¯ ...
// DE ___________________________________________________ ...
//  V _____________|¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯ ...
//       |        ||
// DETIME^--------^|
//                 |
//                 VSYNC inactive
//
//
//InterruptT_VD   End  InterruptT_VD+1
//    |             |  |            End
//    v             v  v            v
//  H |_|¯¯¯¯¯¯¯¯¯¯¯¯¯¯|_|¯¯¯¯¯¯¯¯¯¯¯¯¯¯|_|¯¯¯¯¯¯¯¯¯¯¯¯¯¯ ...
// DE ____________________|¯¯¯¯¯¯¯¯|_______|¯¯¯¯¯¯¯¯|____ ...
//  V ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯ ...
//       |        ||      |        |
// DETIME^--------^|      ^--------^
//                 |
//                 dataEnable=TRUE
//
//InterruptV_COUNT  End
//    |             |
//    v             v
//  H |_|¯¯¯¯¯¯¯¯¯¯¯¯¯¯|_|¯¯¯¯¯¯¯¯¯¯¯¯¯¯|_|¯¯¯¯¯¯¯¯¯¯¯¯¯¯ ...
// DE ___|¯¯¯¯¯¯¯¯|______________________________________ ...
//  V ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯ ...
//       |        ||      |        |
// DETIME^--------^|      ^--------^
//                 |
//                 dataEnable=FALSE
//
//
// This isn't really a state, it's basically just:
// if(hsyncCount == NUM_HYSYNCS_PER_FRAME)
//    hsyncCount = 0;
//                    .....................................
//                    .                                   .
//InterruptT_DV   End .Interrupt0                         .
//    |             | .|                                  .
//    v             v .v                                  .
//  H |_|¯¯¯¯¯¯¯¯¯¯¯¯¯¯|_|¯¯¯¯¯¯¯¯¯¯¯¯¯¯|_|¯¯¯¯¯¯¯¯¯¯¯¯¯¯ .
// DE ___________________________________________________ .
//  V ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯|____________________ .
//       |        ||  .   |        |                      .
// DETIME^--------^|  .   ^--------^                      .
//                 |  .                                   .
//                 hsyncCount=0 (repeat from Interrupt0)  .
//                    .                                   .
//                    .....................................
//


// NOW to compare with LVDS states:
//
//Interrupt0       End
//    |             |  Interrupt1
//    v             v  v
//  H |_|¯¯¯¯¯¯¯¯¯¯¯¯¯¯|_|¯¯¯¯¯¯¯¯¯¯¯¯¯¯|_|¯¯¯¯¯¯¯¯¯¯¯¯¯¯ ...
// DE ___________________________________________________ ...
//  V ¯¯¯¯¯¯¯¯¯¯¯¯¯|_____________________________________ ...
//    ^ ^          ^   ^ ^
//    | |          |   | |
//    | |          |   | +--- V w/o H  \ These two toggle until
//    | |          |   +----- V w/ H   / next LCD state...
//    | |          |
//    | |          +--------- V w/o H > Intermediate change of state
//    | |
//    | +----- NothingActive  \ From Previous State (?)
//    +------- H_Only         /


//InterruptT_Vlow   End
//    |             |
//    v             v
//  H |_|¯¯¯¯¯¯¯¯¯¯¯¯¯¯|_|¯¯¯¯¯¯¯¯¯¯¯¯¯¯|_|¯¯¯¯¯¯¯¯¯¯¯¯¯¯ ...
// DE ___________________________________________________ ...
//  V _____________|¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯ ...
//    ^ ^          ^   ^ ^
//    | |          |   | |
//    | |          |   | +--- NothingActive  \ These two toggle until
//    | |          |   +----- H_Only         / next LCD state...
//    | |          |
//    | |          +--------- NothingActive > Intermediate change of state
//    | |
//    | +----- V w/o H   \ From previous state
//    +------- V w/ H    /


//InterruptT_VD   End  InterruptT_VD+1
//    |             |  |            End
//    v             v  v            v
//  H |_|¯¯¯¯¯¯¯¯¯¯¯¯¯¯|_|¯¯¯¯¯¯¯¯¯¯¯¯¯¯|_|¯¯¯¯¯¯¯¯¯¯¯¯¯¯ ...
// DE ____________________|¯¯¯¯¯¯¯¯|_______|¯¯¯¯¯¯¯¯|____ ...
//  V ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯ ...
//    ^ ^              ^ ^^        ^ 
//    | |              | ||        |
//    | |              | ||        +--- NothingActive  \  These four
//    | |              | |+------------ DE              | cycle until
//    | |              | +------------- NothingActive   | next LCD state...
//    | |              +--------------- H_Only  ^      /
//    | |                                       |
//    | +----- NothingActive  \ From previous   |
//    +------- H_Only         / state           +-This intermediate state
//                                                may not be necessary
//                                                (IAXG01 shows 160 dots!)



//InterruptV_COUNT  End
//    |             |
//    v             v
//  H |_|¯¯¯¯¯¯¯¯¯¯¯¯¯¯|_|¯¯¯¯¯¯¯¯¯¯¯¯¯¯|_|¯¯¯¯¯¯¯¯¯¯¯¯¯¯ ...
// DE ___|¯¯¯¯¯¯¯¯|______________________________________ ...
//  V ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯ ...
//    ^ ^^        ^    ^ ^
//    | ||        |    | |
//    | ||        |    | +--- NothingActive \  These two toggle
//    | ||        |    +----- H_Only        /  until next LCD state
//    | ||        |
//    | ||        +---- NothingActive \                         //blah
//    | |+------------- DE             |  From previous state
//    | +-------------- NothingActive  |
//    +---------------- H_Only        /

// This isn't really a state, it's essentially just:
//  if(hsyncCount == NUM_HSYNCS_PER_FRAME)
//    hsyncCount = 0;
//                    .....................................
//                    .                                   .
//InterruptT_DV   End .Interrupt0                         .
//    |             | .|                                  .
//    v             v .v                                  .
//  H |_|¯¯¯¯¯¯¯¯¯¯¯¯¯¯|_|¯¯¯¯¯¯¯¯¯¯¯¯¯¯|_|¯¯¯¯¯¯¯¯¯¯¯¯¯¯ .
// DE ___________________________________________________ .
//  V ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯|____________________ .
//    ^ ^             .                                   .
//    | |             .....................................
//    | +--- NothingActive \                                 //blah
//    +----- H_Only        / From previous state...



//  LVDS State transitions:
//   1 H_Only
//   2 NothingActive
//
//   3 V w/o H
//   4 V w/ H
//    
//   (repeat 3,4)
//
//   5 V w/o H
//
//   6 NothingActive
//   7 H_Only
//
//   (repeat 6,7)
//
//   8 NothingActive
//
//   9 H_Only
//  10 NothingActive
//  11 DE
//  12 NothingActive
//
//  (repeat 9-12)
//
//  13 H_Only
//  14 NothingActive
//
//  (repeat 13-14)
//
//   These should be reviewed to determine which changes are necessary
//   between each state...
//   (the fewer changes, the less likely we'll glitch...?)






// This'll probably be better rearranged...
//  For now, though, OC1A is in all the notes above, for the D/V/H signal
//  so OC1B is for the clock...
// CHANGING THESE does NOT change channel association.
#define DVH_OCR   OCR1A
#define CLOCK_OCR   OCR1B




// a/o v62-17:
// See notes in lvds_timerInit()...
// Basically, Green should be enabled at the beginning of a drawPix
// function, and disabled at the end.
// I can't remember the *exact* details, but I think V/H signals might
// require dead-time values which would affect the clock if it is in
// complementary-output-mode (but *don't* when it's not)
#define lvds_disableGreen_MakeClockInsensitiveToDT() \
	TCCR1A = ( (0<<COM1A1) | (1<<COM1A0) \
			   | (1<<COM1B1) | (0<<COM1B0) \
				| (1<<PWM1A) | (1<<PWM1B) )

#define lvds_enableGreen_MakeClockSensitiveToDT() \
	TCCR1A = ( (0<<COM1A1) | (1<<COM1A0) \
				| (0<<COM1B1) | (1<<COM1B0) \
				| (1<<PWM1A) | (1<<PWM1B) )


void lvds_timerInit(void)
{
   //Timer1 is used for LVDS (in PLL clocking mode)
  
     //pll_enable();   
   
   //We want it to count 7 bits, 0-6 and reset at 7
   OCR1C = 6;

   //We want the clock to go low at TCNT=0 and high at TCNT=4
   CLOCK_OCR = 3; //2; //3;
   

// My 'scope is only 20MHz, and I'd rather be able to use the digital mode
// which is even slower...


//Overridden when SLOW_EVERYTHING_TEST is true...
//#define TOOFAST_TEST TRUE
#warning "HERE AND BELOW, doesn't OSCCAL have special write requirements?"
//OSCCAL = 0x00;
#if (defined(SLOW_EVERYTHING_TEST) && SLOW_EVERYTHING_TEST)
//FOR TESTING. This should slow the clock...
   // Gives roughly 4MHz...
   OSCCAL = 0x00; //0x80; 

   // This should divide the system clock by 256
   // does this affect the PLL? NO.
   // The PLL is clocked only by the RC Oscillator
   //   OSCCAL does affect it.
   // OPTIONS FOR SIMULATING FULL FUNCTIONALITY AT LOW SPEED:
   // Run PLL as normal
   //  Prescale Timer1 and DeadTimer equally
   //  Prescale System clock equally
   CLKPR = (1<<CLKPCE);
   CLKPR = (1<<CLKPS1) | (1<<CLKPS0);   //CLKDIV8
   //4MHz / 8 = .5Mhz...
   //PLL is 4MHz*8 = 32MHz
   // then TimerClockDivisor=8 gives 4MHz
   pll_enable();
   #define SLOW_LVDS_TEST TRUE
#elif (defined(OSCCAL_VAL))
   OSCCAL = OSCCAL_VAL;
#endif



#if (defined(SLOW_LVDS_TEST) && SLOW_LVDS_TEST)
   //This case doesn't really make sense without SLOW_EVERYTHING...
   //8x prescaler for Timer1
   #define CSBITS (1<<CS12)
   //8x prescaler for the dead-timer
   #define DTPSBITS ((1<<DTPS11) | (1<<DTPS10))
#elif (defined(LVDS_PRESCALER))
   //Timer1 on the Tiny861 uses a strange CLKDIV scheme...
   // (but it's nicer!)
   // The divisor is (1<<(csbits-1))
   // so a divisor of 1 = (1<<0) = (1<<(1-1)), (csbits = 0x1)
   // 256 = (1<<8) = (1<<(9-1)), (csbits = 0x9)
   // 512 = (1<<9) = (1<<(10-1)), (csbits = 0xA)
   // ...
   // (0x0 stops the timer)
   /*
      uint16_t divisor;
      uint8_t csbits = 0;
      for(divisor=CLKDIV; divisor != 0; divisor>>=1)
         csbits++;
         writeMasked(csbits, 0x0f, TCCR1B);
   */

//#if ((LVDS_PRESCALER != 64) && (LVDS_PRESCALER != 32) && \
//     (LVDS_PRESCALER != 16) && 
#if ((LVDS_PRESCALER != 8) && \
     (LVDS_PRESCALER != 4)  && (LVDS_PRESCALER != 2) && \
     (LVDS_PRESCALER != 1))
#error "LVDS_PRESCALER must be a power of 2, from 1 to 8"
#endif

   //Figured this out in cTools/dePower.c...
   //64 is overkill here, since the deadTimer prescaler only goes to 8...
#define divToCS(div) \
   ( (div == 64) ? 7 : (div == 32) ? 6 : (div == 16) ? 5 : (div == 8) ? 4 \
     : (div == 4) ? 3 : (div == 2) ? 2 : (div == 1) ? 1 : 0)


   //CSBITS (through PLL/8) (CS10 is bit 0)
   //CS12:10   CS12   CS11   CS10      PLL division
   //1         0      0      1         1
   //2         0      1      0         2
   //3         0      1      1         4
   //4         1      0      0         8
   #define CSBITS divToCS(LVDS_PRESCALER) //<<CS10 should be redundant
   //DTPSBITS (DTPS10 is bit 4)
   //DTPS11:10   DTPS11 DTPS10   PLL division
   //0         0      0         1
   //1         0      1         2
   //2         1      0         4
   //3         1      1         8
   #define DTPSBITS ((CSBITS-1)<<DTPS10)

   //Whoops! Forgot this (a/o 42-som'n WTF1pix)
   // so, it's promising to get single-pixel control...
   //  the result was kinda ugly, though... scroll was quite slow
   // Further, it was only drawing a certain number of pixels...
   //  (horizontally... pre LVDS_PRESCALER affecting DOTS_TO_CYC)
   //  so accessing *all* pixels in a row would make it even slower...
   // Further, it didn't seem to be paying attention to the fact that
   // the same pixels are drawn on multiple rows...
   // WTF? (the image was scaled, proportionately! 1pix x 1pix)
   pll_enable();

#else
   //No clock divisor
   #define CSBITS (1<<CS10)
   //No dead-timer divisor
   #define DTPSBITS 0
   pll_enable();
   //#warning "The PLL configuration code is not in here yet!"
#endif

   //Set the Timer1 clock prescaler...
   writeMasked(CSBITS, 
               ((1<<CS13) | (1<<CS12) | (1<<CS11) | (1<<CS10)),
               TCCR1B);

   //Set the DeadTime prescaler (no prescaling, same speed as TCNT1)...
   // Allegedly this is prescaled from the PCK (or CK)
   //    NOT from the Timer1 prescaler...
   writeMasked(DTPSBITS,
               ((1<<DTPS11) | (1<<DTPS10)),
               TCCR1B);



   //All LVDS modes (and signals) use FastPWM.. 
 
   //FastPWM
//Now Handled in lvds_xxxOnCompare():

   //These are also written below (excluding PWM1D)
   setbit(PWM1A, TCCR1A);  //Enable PWM on OC1A for DVH
                           //Need to do the same for other channels
  
   setbit(PWM1B, TCCR1A);  //Enable PWM on OC1B for CLOCK 

   setbit(PWM1D, TCCR1C);

   //PWM1D is not in TCCR1A...

   setoutPORT(PB1, PORTB);   //+OC1A, DVH/BLUE, MISO (usually heart)
//   setoutPORT(PB0, PORTB);   //-OC1A    MOSI unused
   setoutPORT(PB2, PORTB); //-OC1B, -GREEN    (INVERTED) SCK
   setoutPORT(PB3, PORTB); //+OC1B Clock (OC1B, not inverted)
   setoutPORT(PB5, PORTB); //+OC1D, RED


   writeMasked(((0<<WGM11) | (0<<WGM10)), //FastPWM (combined with above)
               ((1<<WGM11) | (1<<WGM10)), // (affects all PWM channels)
               TCCR1D);

   //OC1A is used, but /OC1A isn't
// Not Valid (deadTime only functions if in complementary-mode):
   // OTOH: dead-time is not necessary in the only case we need to switch
   // Most lvds states use clear on compare-match, set at 0
   // But there is one that uses the opposite
   // This can be toggled by a single bit-change
   //  COM1A1 = 1
   //  COM1A0 value indicates what happens on Compare-Match
   //                 or ! at BOTTOM
//#define lvds_clrOnCompare() clrbit(COM1A0, TCCR1A)
//#define lvds_setOnCompare() setbit(COM1A0, TCCR1A)
   //HOWEVER: the alternative is possible:
   // Use complementary (clear-on-compare)
   // and single-ended with set-on-compare
   // COM1A0 = 1
   // COM1A1 value indicates what happens on Compare-Match

/* These are three instructions apiece!
#define lvds_ComplementaryClrOnCompare() clrbit(COM1A1, TCCR1A)

#define lvds_setOnCompare() setbit(COM1A1, TCCR1A)
#define lvds_clrOnCompare() lvds_ComplementaryClrOnCompare()
*/

//Page 99:
// "In Fast PWM Mode ... when the COM1x1:0 bits are set to “01” ...
//  an user programmable Dead Time delay is inserted for 
//  these complementary output pairs (OC1x and OC1x)."

//Page 98:
// "The counter is loaded with a 4-bit DT1H or DT1L value from DT1 
//  I/O register, depending on the edge of the Waveform 
//  Output (OCW1x) when the dead time insertion is started."
// In other words, (as it appears from the diagram)
//  the dead-time value is loaded into the counter when the corresponding
//  edge in OCW1x is detected
//  So changing the value of DT1 affects the *next* corresponding edge 
//   (NOT if the dead timer is already running)

//Page 96:
// "The OCR1x Registers are double buffered when using any of the 
// Pulse Width Modulation (PWM) modes."
// "The double buffering synchronizes the update of the OCR1x 
// Compare Registers to either top or bottom of the counting sequence.
// The synchronization prevents the occurrence of odd-length, non-sym- 
// metrical PWM pulses, thereby making the output glitch-free."

//Page 100:
//"A change of the COM1x1:0 bits state will have effect 
// at the first Compare Match after the bits are written."
//Page 97: HAH!
//"Be aware that the COM1x1:0 bits are not double buffered 
// together with the compare value. 
// Changing the COM1x1:0 bits will take effect immediately."


//#define lvds_setOnCompare()   
//   TCCR1A = ( (1<<COM1A1) | (1<<COM1A0) 
//            | (0<<COM1B1) | (1<<COM1B0) 
//            | (1<<PWM1A) | (1<<PWM1B) )
//#define lvds_ComplementaryClrOnCompare() \ //
   //Do TCCR1C first, because it contains shadow-bits of TCCR1A that I
   // don't want to have to rewrite...
   TCCR1C = ( (1<<COM1D1) | (0<<COM1D0)
            | (1<<PWM1D) );
/* a/o v62-17: The notes are sparse as to why I disabled the Green signal
 * at all times, except during DE... As I recall, it was an experiment with
 * whether it was possible to get greater control over BLUE, by hacking the
 * clock signal during the DE period... e.g. since none of the V/H timings
 * are necessary during DE, maybe it was possible to have a clock that
 * wasn't perfectly as-spec'd... it didn't work. So, why not remove the
 * necessity for enabling/disabling the green signal...
 (Yes, it's that tightly-knit that affecting the clock affects green,
 affects blue... but there's some confusion in my mind as to why green
 couldn't've remained active anyhow...)
 Ahh, wait, something about when Complementary mode is enabled, then the
 clock is affected by Dead-Times... Still a bit vague.
*/
//	TCCR1A = ( (0<<COM1A1) | (1<<COM1A0) 
//            | (1<<COM1B1) | (0<<COM1B0) //Don't use complementary for CLK
//            | (1<<PWM1A) | (1<<PWM1B) );
	lvds_disableGreen_MakeClockInsensitiveToDT();

/* ... but this didn't work... nothing's showing up at all.
	TCCR1A = ( (0<<COM1A1) | (1<<COM1A0)
	         | (0<<COM1B1) | (1<<COM1B0)
	         | (1<<PWM1A) | (1<<PWM1B) );
*/

//#define lvds_clrOnCompare() lvds_ComplementaryClrOnCompare()

   // Enable Complementary-Mode (and thus the dead-timer)
   // This'll be changed as needed, but we need COM1A0 set prior to that

   //   lvds_clrOnCompare();
//   writeMasked(((0<<COM1A1) | (1<<COM1A0)), 
//               ((1<<COM1A1) | (1<<COM1A0)), 
//               TCCR1A);



   //THIS IS NOT THOROUGHLY THOUGHT-OUT...
   // as I recall, we need to use /OC1B for the clock output
   //  because it won't be affected by the deadtime...
//Now Handled in lvds_xxxOnCompare():
//   writeMasked(((0<<COM1B1) | (1<<COM1B0)),
//               ((1<<COM1B1) | (1<<COM1B0)),
//               TCCR1A);

   //Since PWM inversion affects all channels, great-pains were taken
   // to assure that it needn't be changed in any lvds state...
   // PWM inversion must be enabled:

   //inverted with PWM1X
//   setbit(PWM1X, TCCR1B);
 
    Nada_init();
}




//For a first go...
// B5 = OCR=5
// B4 = OCR=6+
// B3 = DT=0
// B2 = DT=1
// Possible combinations:
// B3, B2, B5, B4      OCR>6 (DT=0)
// B3, B2, B5         OCR=5, DT=0
// B3, B2            OCR=4, DT=0
//     B2, B5, B4      OCR=6, DT=1 (is OCR=6 possible?)
//     B2, B5         OCR=5, DT=1
//     B2            OCR=4, DT=1
//         B5, B4      OCR=6, DT=2 (is OCR=6 possible?)
//         B5         OCR=5, DT=2
// OCR=6 is full-on...

//            |  0   1    2    3    4    5    6    
//            |____ ____ ____ ____ ____ ____ ____
//   DE:      X B3 X B2 /    .    .    \ B5 X B4 X
//            |¯¯¯¯ ¯¯¯¯                ¯¯¯¯ ¯¯¯¯

// Active Bits     | Brightness   |
//                 | (0-63)       |    Configuration
// ----------------+--------------+-------------------------
// B5  B4  B3  B2  |     60       |      OCR>6 (DT=0)  //DC
// B5      B3  B2  |     44        |      OCR=5, DT=0
// B5          B2  |     36        |      OCR=5, DT=1
// B5              |     32        |      OCR=5, DT=2  
//         B3  B2  |     12        |      OCR=4, DT=0  //Damn-near black...
//             B2  |     4         |      OCR=4, DT=1  //Even closer
// None = Black    |     0        |    OCR=4, DT=2
//
// The colors 60, 44, 36, and 32 are all discernable
// (though 36 and 32 mightn't be if not side-by-side)
// 12, 4, and 0 are damn-near identical and may not be worth the overhead
// There is a SIGNIFICANT jump between 32 and 12
//   32 is I'd say half-bright, maybe more
//   12 is damn-near black

#define fullBlue()   DEblue_init()

/*
static __inline__ \
void writeBlue(uint8_t settingVal) \
     __attribute__((__always_inline__));
*/

#if FALSE
#define GREEN_PIN_MASK   0x03
#define GREEN_PORT      PORTA
#define GREEN_ON         0x01
#define GREEN_ON_STRING   "0x01"
#define GREEN_OFF         0x02
#define GREEN_OFF_STRING "0x02"
// in settingVal: bit 3 is unused by blue (4<=OCR<=6), so use it for green
#define GREEN_SETTING_BIT   3
#define GREEN_SETTING_BIT_STRING   "3"
// for setBlue:
#define GREEN_BLUEVAL_BIT   0

#define RED_PIN_MASK      0x0C
#define RED_PORT         PORTA
#define RED_ON            0x04
#define RED_ON_STRING   "0x04"
#define RED_OFF         0x08
#define RED_OFF_STRING   "0x08"
#define RED_SETTING_BIT   7
#define RED_SETTING_BIT_STRING "7"
#define RED_BLUEVAL_BIT   1

#define REDGREEN_PORT    RED_PORT


void writeBlue(uint8_t settingVal)
{
   //Best to do these calculations first and write the registers later
   // it's a noticeable change (green is offset a bit, but much less)
   uint8_t dt = settingVal & 0x70;
   uint8_t ocr = settingVal & 0x07;

   //The red and green PORT value will be temporarily calculated here...
   uint8_t redGreen; 


   //ASM Notes:
   // avr-libc-user-manual-1.7.0/inline__asm.html

   //This is timing-critical... the amount of time taken in writeBlue
   // determines the width of each pixel.
   // Most importantly: if standard C-style if/else statements are used
   //  it compiles differently each time (based on optimization, etc.)
   //  Often, though not always, it would compile such that different
   //  color values would use more instructions than others (branching)...
   //  It was too unpredictable (and believe me I tried) to code it in C
   //  and make it consistent. e.g. adding a nop in an else-case caused
   //  it to compile using brne, but not adding the nop caused breq

   //Simply: if(getbit(greenBit, settingVal))    redGreen=GREEN_ON;
   //        else                                 redGreen=GREEN_OFF;
__asm__ __volatile__
        //SBRC takes 1 cycle if not skipping, 2 (or 3) if skipping
        ( "sbrc %1, " GREEN_SETTING_BIT_STRING "; \n\t"            //0,1
                     //Skip the jump if !getbit(GREEN, settingVal)
          "rjmp .+4; \n\t"  //jump if getbit(GREEN, settingVal)   //1-
          "ldi  %0, " GREEN_OFF_STRING "; \n\t"                     //0-
          "rjmp .+4; \n\t"  // skip setting GREEN_ON               //0-
          "nop; \n\t"
          "ldi  %0, " GREEN_ON_STRING "; \n\t"                     //1-
         : "=r" (redGreen)      //redGreen is assigned to %0
         : "r"  (settingVal)   //settingVal is assigned to %1
         );
   //Simply: if(getbit(redBit, settingVal))      redGreen|=RED_ON;
   //          else                                 redGreen|=RED_OFF;
__asm__ __volatile__
        ( "sbrc %1, " RED_SETTING_BIT_STRING "; \n\t"
                     //Skip the jump if !getbit(GREEN, settingVal)
          "rjmp .+4; \n\t"  //jump if getbit(GREEN, settingVal)
          "ori  %2, " RED_OFF_STRING "; \n\t"
          "rjmp .+4; \n\t"  // skip setting GREEN_ON
          "nop; \n\t"
          "ori  %2, " RED_ON_STRING "; \n\t"
         : "=r" (redGreen)    //redGreen is assigned to %0
         : "r"  (settingVal),  //settingVal is assigned to %1
           "d0"  (redGreen) //d is necessary for ori
         );                 // 0 means 2 is shared with 0 for r/w... 
                            //(its value is also an input)
   //See notes in [the new] setBlue()

   //Since instructions are longer than pixels, it's damn-near impossible
   // to have perfectly sharp edges... this order seems best,
   // but I haven't experimented much
   // May be that the reverse order is best when switching from lighter
   // to darker, which would require more overhead to detect
   // making pixels even longer. Best to have a single-LCD-pixel of ugly
   // than to make displayable pixels wider, reducing resolution...
   OCR1A = ocr;
   DT1 = dt;
   REDGREEN_PORT = redGreen;

   //This is just to try to mimic 17's timing...
   // (right now, there's some timing issues, 
   //   line seems to start somewhat randomly, near the right edge
   //   some PIXEL_SCROLLs give static...)
   // <= 46 optimizes out, somehow, even though the code-size is larger
   // Should be fixed now... (see delay_cyc hacks)
//   delay_Dots(15);

}
#endif //FALSE





/*
void setBlue(uint8_t val, uint8_t r, uint8_t c)
{
   //Each If statement consists of (rougly):
   //  comparison (cpi)
   //  jump if false to next (brcs)
   //    load immediate -> register
   //    out OCR1a <- register
   //    load immediate -> register
   //    out DT1 <- register
   //    jump to end of If's...

   //THUS: the brighter the color, the fewer cycles are executed
   // (fewer comparisons, fewer jumps)
   
   // One option: insert NOPs...
   // Another option (and probably better all 'round):
   //   Store the OCR1A and DT1 values in the settingBuffer
   //   instead of storing the color value
   //   (IOW: do this test when writing the "settingBuffer"
   //    instead of when reading)
   //   Then, here, just write OCR1A and DT1
   //   If a single-byte is used for both, DT1 could be written directly
   //     e.g. blueSetting=(DTVal<<4) | OCR1AVal
   //     DT1 = blueSetting; //OK since /OCR1n outputs are unused
   //     OCR1A = (blueSetting & 0x0f);



   // | (val & GREEN_SETTING_BIT)  is a hack a/o v20, for GREEN
   uint8_t green =
      getbit(GREEN_BLUEVAL_BIT, val) ? (1<<GREEN_SETTING_BIT) : 0 ;
   uint8_t red =
      getbit(RED_BLUEVAL_BIT, val) ? (1<<RED_SETTING_BIT) : 0 ;

   uint8_t redGreen = green | red;



#if(defined(DC_DE_DISABLE) && DC_DE_DISABLE)
// #define NUM_BLUES 5      //Currently Unused... (not including black)
#else
// #define NUM_BLUES 6      //Currently Unused... (not including black)

   if(val>=(60<<2)) 
   {
      //OCR1A = 0xff;
      settingBuffer[r][c] = (0<<4) | 6 | redGreen;
   }
   else
#endif
   if(val >= (44<<2))
   {
      //OCR1A = 5;
      //DT1 = (0<<4);
      settingBuffer[r][c] = (0<<4) | 5 | redGreen;
   }
#if (!defined(DT0_BLUES_ONLY) || !DT0_BLUES_ONLY)
   else if(val >= (36<<2)) //OK
   {
      //OCR1A = 5;
      //DT1 = (1<<4);
      settingBuffer[r][c] = (1<<4) | 5 | redGreen;
   }
   else if(val >= (32<<2)) //OK
   {
      //DT1=(2<<4);
      //OCR1A = 5;
      settingBuffer[r][c] = (2<<4) | 5 | redGreen;
   }
   else if(val >= (12<<2)) //DIM
#else //DT0_BLUES_ONLY
   else //Closest to black we can get with DT=0
#endif
   {
      //DT1=(0<<4);
      //OCR1A = 4;
      settingBuffer[r][c] = (0<<4) | 4 | redGreen;
   }
#if (!defined(DT0_BLUES_ONLY) || !DT0_BLUES_ONLY)
   else if(val >= (4<<2)) //DIM
   {
      //DT1=(1<<4);
      //OCR1A = 4;
      settingBuffer[r][c] = (1<<4) | 4 | redGreen;
   }
   else   //Black
   {
      //DT1=(2<<4);
      //OCR1A = 4;
      settingBuffer[r][c] = (2<<4) | 4 | redGreen;
   }
#endif
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
 *    2) Please do not change/remove others' credit/licensing/copywrite 
 *         info, where noted. 
 *    3) If you find yourself profitting from my work, please send me a
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
 *    (Wow, that's a lot longer than I'd hoped).
 *
 *    Enjoy!
 */

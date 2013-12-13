#include "projInfo.h"   //Don't include in main.h 'cause that's included in other .c's?
#include "main.h"
#include <util/delay.h> //For delay_us in pll_enable
#include <avr/pgmspace.h>
#include _TIMERCOMMON_HEADER_
//#include "../../../_commonCode/charBitmap/0.10/charBitmap.h"
#include _CHARBITMAP_HEADER_
#include "fbColors.h"

#include _SINETABLE_HEADER_
#include "tetStuff.c"




// Latest Version is v59-10 or so.. in which I'm adding a lot of notes re:
// defines, etc... labelled a/o v59
// These notes, when conflicting with older notes, should be considered
// the most recent.



//The current state is such that basically ONLY the row-segment-buffer
// display-method has been tested for quite some time. The other code
// remains, but I can't promise it still works.

//Only one of the following SEG_whatevers should be TRUE
// (Assuming we're using the Row-Segment-Buffer, as explained above).
// IF NONE ARE TRUE:
//  The default is to convert whatever's in the row-buffer into 
//  the row-segment-buffer...
//  This is where my old-code starts getting iffy, as the row-buffer hasn't
//  been used for quite some time.
// Best to stick with assigning one of these SEG_xxx things true.


//Draws a diagonal white line on a red background...
// I think it's supposed to repeat three times and not fill the entire
// screen... though it's been a long time since I've used this.
//#define SEG_LINE TRUE

//A very nice test-pattern... shows a sine-wave, the under-side of which
// is horizontal color-bars, above it is vertical color-bars. 
// Above that is two lines of text, showing all available characters
// And above that is color-patterns using up the remaining Row-Segments
// This is meant to be rotated 90-degrees
// Like SEG_HFM, it might be handy to adjust NUM_SEGMENTS for experimenting
// Note that color-segments which are too long to be stored in a single
// row-segment are automatically put into the next.
// And that existing segments are automatically stretched if the next-added
// segment is the same color.
// This displays all available colors and shows the resolution capabilities
#define SEG_SINE TRUE

//Uses "High-Frequency Modulation" to display an interesting pattern...
// HFM is kinda like PWM. The idea is to have an output ON for
//  a certain percentage of the time...
//  In PWM, that's done by turning it on for a fraction of a cycle 
//  (the "width" of the pulse), then off for the remainder.
//  In HFM, it's accomplished by knowing the fraction of time it should be
//  on... (the "power"). The fractions are automatically reduced 5/10->1/2.
//  e.g. if the on-time should be 1/100th of the time, it will be on
//  during one update-period, and off for 99, then repeat.
//  If the on-time should be 1/2 of the time, it will be on during one
//  update, off during the next, on again, and so-on.
//  If the on-time is some strange fraction, like 3/5, it will distribute
//  the pulses accordingly (e.g. on, off, on, off, on, repeat)
//  Thus, the output toggles as quickly as possible to achieve the desired
//  power... thus "High-Frequency"
//  (See _commonCode.../hfModulation/...)
//  I've been using HFM in ways never originally intended: e.g. it can be
//  used for smoothing lines between two distant points...
//  It's used this way in "SEG_RACER" in order to use a low-resolution
//  course in memory, and increase the resolution by knowing that there are
//  a certain number of rows in which it has to get from point1 to point2
//  so the "power" of the HFM is set to (p2-p1)/numRows.
//  The nice thing about it, is it doesn't use any actual division (which
//  is quite slow) because it knows that every point inbetween will be 
//  traversed.
// THIS visualizes that, and actually looks pretty cool. Like moire
//  patterns, or magnetic-field-lines.
// Each row increases in power, essentially: rowNum/NUM_SEGMENTS
// (Experimenting with NUM_SEGMENTS is fun, in this case, just don't exceed
//  the available memory, and keep in mind that there's a stack and stuff)
//#define SEG_HFM		TRUE
// You can override NUM_SEGMENTS here, for that purpose...
// OTHERWISE, it should probably be handled in rowSegBuffer.c
#if(defined(SEG_HFM) && SEG_HFM)
 #define NUM_SEGMENTS   127 //128 //95//96//128 //68 //128//68
#endif

//Displays a Question-Mark box, ala Mario-Brothers. Press the button and
// receive an award (and occasional goomba)
// Demonstrates usage of program-memory-based images... (16x16 pixels WOO!)
//#define SEG_QUESTION	TRUE

//A Game! Ala "Racer" from the ol' TI-82 days...
// Use a potentiometer to try to keep the "car" on the race-track
// It gets harder the longer you stay on course!
// (Ideally: when this isn't true, remove ADC stuff from the makefile
//  to save codespace. I don't think I've ever actually paid attention to
//  this...)
//#define SEG_RACER 	TRUE








//#warning "This needs to be moved... and conditional"
//#include "rowSegBuffer.c"

//Called as: pgm_readImageByte(pgm_image1, row, col)
#define pgm_readImageByte(image, row, col)   \
	   pgm_read_byte((uint8_t *)(&((image)[(row)*FB_WIDTH+(col)])))



// I hereby declare this FPD-Link simulation technique to forever be called
//  PW-BANGing


//For testing of slower LVDS pixel-rates
// (maybe we can increase the resolution)
// value must be 1, 2, 4, or 8
// This is overridden by 8 when SLOW_LVDS_TEST is true...
// If commented-out the default of 1 is used...
// AFAICT, this only increases codesize in HLow_delay...
// CHANGING THIS MIGHT AFFECT DISPLAYABILITY...
// One place to look is the delay_cyc in drawPix
//  Currently 1, 2, and 8 seem to work.
//  8 no longer works, noticed a/o v46
// a/o v59, this has been 8 for quite some time... it works well with
// RowSegBuffer, to increase resolution at the expense of frame-rate
// I have since tried it with 2... interesting effect
//   Surprisingly, it works, despite the fact that in this mode, I think it
//   should be still keeping Data-Enable active for the full image
//   which extends way beyond the edge of the screen.
//   Oddly, it seems to be scaling BOTH horizontally *and* vertically
//   I can't explain this.
//   It offers quite a bit of potential, though. The refresh-rate is
//   increased dramatically, just by changing this value from 8 to 2
//   
#define LVDS_PRESCALER 8//1//2//8//2//1//8//2//4//8//2//8//2
//8//2//1//2//2//2//2//2//2//2//2//2//2//2//8//4 //1 //2//4//8//2//4


//#define LIFE TRUE
#if (defined(LIFE) && LIFE)
 #include "lifeStuff.c"
#endif

// a/o v59
//This should probably always be TRUE now... It's been a LONG time since I
// experimented with it otherwise.
#define ROW_SEG_BUFFER	TRUE
//now, SEG_STRETCH >= 3 causes weirdness... (repeated rows)
// previously 3 was OK
// This is fixed a/o newSeg, etc.
//a/o v59 I believe this is only used in the case when no SEG_(mode) is set
// to stretch a low-resolution row-buffer across the screen
// And that case (No SEG_mode set) doesn't really do anything anymore
#define SEG_STRETCH 5//4//3//2//3//4//6//3//4//6	//Stretch pixels using longer segments

#if (defined(ROW_SEG_BUFFER) && ROW_SEG_BUFFER)
 #warning "ROW_SEG_BUFFER requires ROW_BUFFER, but this is a hack"
 #if (!defined(LVDS_PRESCALER) || (LVDS_PRESCALER < 2))
  #error "ROW_SEG_BUFFER uses 20cyc/pixel, which isn't compatible with ROW_BUFFER -> width=64, since ROW_BUFFER used 16cyc/pixel. Bump your LVDS_PRESCALER up, or comment this error out to see what happens"
 #endif
 #define ROW_BUFFER TRUE
 #include "rowSegBuffer.c"
#endif

//a/o v59:
//If this is not true, then it uses the frameBuffer... which is no longer
// tested/implemented...
#define ROW_BUFFER TRUE
#if (defined(ROW_BUFFER) && ROW_BUFFER)
 #include "rowBuffer.c"
#endif

//Only valid with ROW_BUFFER=TRUE above:
//#define HORIZONTAL_COLOR_BARS TRUE









// a/o v59:
//See its old definition elsewhere
// for an explanation of the math... and a bunch of remaining questions.
#define NUM_PSEGS (1024/3+16)



#if(defined(SEG_RACER) && SEG_RACER)
	#include _ADC_HEADER_
//	#include <stdio.h>

//Returns 0-9 or -1 if we've reached 0...
int8_t grabDecimal_u32(uint32_t value, uint8_t newVal)
{
   static uint32_t divisor = 1000000000;

   static uint32_t thisVal;

   if(newVal)
   {
      divisor = 1000000000;
      thisVal = value;
   }

   uint8_t decimal = 0;



   if(divisor == 0)
      return -1;

   while(thisVal >= divisor)
   {
      decimal++;
      thisVal -= divisor;

   }




   divisor /= 10;

   return decimal;
}


//Including raceWidth, and maybe some buffer...
#define RACE_WIDTH_PSEGS	272


void showScore(uint16_t rowNum, uint32_t score, uint8_t color)
{

	//The first few rows aren't displaying?
//		rowNum-=6;
		int8_t decimal;
		uint8_t scoreFound = FALSE;

		static uint32_t thisScore;

		if(rowNum == 0)
			thisScore = score; // goodCount;

		decimal = grabDecimal_u32(thisScore, 1);

		uint8_t col;

		uint8_t charRow = 0;

		for(col=0; col<NUM_PSEGS-3-RACE_WIDTH_PSEGS; col++)
		{
//			uint8_t charRow = 0;
			
			if(col%8==0)
			{
				if((decimal != 0) && (decimal != -1))
					scoreFound = TRUE;

				if((decimal == -1) || (!scoreFound))
					charRow = 0;
				else
					charRow = getCharRow(decimal+'0', rowNum/2);
				decimal = grabDecimal_u32(thisScore, 0);
			}

			if(getbit(col%8, charRow))
				addSegfb(1,color);
			else
				addSegfb(1,_B);
		}
}	

#endif








//Options LTN, IDT, or TESTVALS
// IDT display *still* doesn't sync... maybe my H/V signals aren't right
#define DISPLAY TESTVALS //LTN //TESTVALS //IDT //LTN

//For Red and Green (NOT Blue) This enables four shades, instead of three
// (including black)
// Doing so increases pixel-processing time, thus the pixel-widths
// (thus decreasing resolution)
// each color takes 9 cycles to process in three-shade mode
// or 12 cycles for red and green, plus 9 for blue in four-shade mode
// a/o v59: I don't think this does anything in ROW_SEG_BUFFER
#define FOUR_SHADES TRUE

// TODO: Between OSCCAL_VAL and FRAME_UPDATE_DELAY
//  it seems the speed (at least for the LTN) isn't so important after all
//  This could be used to my advantage...
//  Maybe bump OSCCAL_VAL all the way up, but use a slower "bit-rate"
//  Thus, pixels could be skinnier, thus higher-resolution.
//   Or more instructions could be used for the same resolution
//   allowing, e.g. sprites from program-memory, without necessitating a
//   large (or any?) frame-buffer...


//This used to be handled by TOOFAST_TEST, but now can be configured here
// 0xff sets the processor to run as fast as possible 
//  (~16MHz? ~128MHz bit-rate)
// 0x00 sets it as slow as possible (~4MHz, ~32MHz bit-rate)
//  good for testing whether odd colors
//  or bad syncing is due to bits being shifted, etc. 
//  Maybe due to slow "LVDS" conversion chips, or whatnot.
//   This seems to be the case right now... 0xff: black is appearing green
//   There should be *some* green in it, but not this much.
//   Likely due to different propagation delays between my under-driven
//    74LS86's (running red and green), from 3.3V!
//    and the happier and newer 74AHC XOR and OR currently driving the
//    clock and DVH/Blue signals
//  Highly dependent on the screen itself, some may not run at all with
//  such slow bitrates... heck, some may not run at all even at the fastest
// If SLOW_EVERYTHING_TEST is true, this value is overridden
// LTN last used 0x20
// a/o v59:
// 0xD8 is the lowest value that syncs reliably, a few glitches from time
// to time.
// The effect of lower values isn't *quite* what I was expecting, but
// definitely suggests that using ROW_SEG_BUFFER at this bit/refresh-rate
// is dang-near right at the minimum required for this display.
// This might be why the other (same model) display didn't work, but its
// flakeyness at 0xff appeared different than this one's at lower OSCCALs
#define OSCCAL_VAL	0xff//0xDB//0xDC//0xE0//0//0xff//0x20//0xff //0x00

//This would normally be 0 for the highest-speed frame-refresh possible
// However, increasing this value can slow the frame-refresh rate
//   if desired
// e.g. 50000 causes it to refresh ~ every few seconds
//   (durn-near the limit for how long the LCD will retain an image)
// The benefit being that it doesn't flicker at annoying rates for
//  stationary images
//  worth exploring...
// This may be highly dependent on the screen capabilities...
//   of course, this sorta thing is undocumented
//   it just has to be experimented with
//   Further, it may be quite hard on the Liquid Crystals, since they'd 
//   likely be driven DC... not sure what the long term effects are.
// This could be a hell of a lot more sophisticated...
// e.g. refresh after a frameBuffer change
//      refresh based on a seconds-timer
//      ...
// AS-IS: Does refreshes completely independent of the frame-buffer
//      if it's too slow, some images may be missed completely.
//      This is not true for FRAME_SYNC TRUE (below)
//      (and it's not really useful anymore with FRAME_SYNC)
// a/0 v59: This is no longer used (!?) I coulda sworn it was still
//          responsible for modifying the timer's interrupt frequency
//          This was also used in an attempt to make the Horizontal Back
//          Porch constant regardless of row calculation-time
//          So, either, the HBP is long enough for the current math, or
//          the display is not sensitive to varying HBPs between rows
//          (see also lcdStuff.c)
//#define FRAME_UPDATE_DELAY 0//5000//50000 //2000 //5000 //20000 //50000

//SHOULD PROBABLY BE REVISITED
// It probably doesn't work with all cases...
// Definitely with drawPix/Images...
//This causes main o update the image *after* FRAME_UPDATE_DELAY
// a/o v59: I don't think this is used anymore...
#define FRAME_SYNC TRUE

//New Idea:
// Disable the Hsync timer interrupt after the frame completes
//  and reenable elsewhere...
//  (It seems pixels changing from one shade to another take an
//   intermediate step halfway between)
//  (Thus, two updates then a pause might clear that up)
//  (Duh... it doesn't clear that up, because the fastest refresh rate
//   is the same...)
//  (but it is an interesting effect)
//  (Double-Duh... I was using a slow refresh with FRAME_UPDATE_DELAY)
//  Reworded: Having several frame refreshes before a pause helps to
//  "solidify" the color changes... especially the sharp changes 
//  (e.g. yellow<->black)
//  (Side Note: I saw this effect on a regular (NON-LVDS) TFT display 
//   in LCDdirect that was only connected such that R, G, and B were each
//   either full-off or full-on, i.e. all Red bits were connected to a
//   single output pin (full on when 1, full off when 0).
//   I hadn't attributed it to improper refresh synchronization
//   as the refresh rate was too fast to see... That's a handy way to get
//   intermediate shades/colors without refresh-flicker from pulsing them!)
//  connected with each color 
//  0 means no delay (except FRAME_UPDATE_DELAY) via interrupt disabling
//  If you WANT this effect (for intermediate shades?) 
//   set FRAME_COUNT_TO_DELAY to 1
//  Then, each change of color will (apparently) result in a value halfway
//   between the old color and the new color
//  Not sure whether it's an electrical problem, but these half-shades 
//   appear dithered, sometimes ugilly, sometimes it's nice. I guess it
//   depends on the goal...
// a/o v59: I believe this doesn't work with ROW_SEG_BUFFER...
#define FRAME_COUNT_TO_DELAY 8//2//7//1//5//2//3


//These two are mutually-exclusive (BLUE_VERT_BAR overrides DE_BLUE)
// (BLUE_DIAG_BAR overrides both...)


// Default is drawPix, from program memory...

//These are only used in drawPix...
//This probably has no purpose outside of COLOR_BAR_SCROLL below
// Actually, it's kinda funny watcching the top of the smiley's
// head scroll... like he's getting a brain transplant
//#define PIXEL_SCROLL TRUE

//This is only valid in drawPix with an image...
#define IMAGE_CHANGE TRUE


//#define COLOR_BAR_SCROLL TRUE
#if (defined(COLOR_BAR_SCROLL) && COLOR_BAR_SCROLL)

//#warning "COLOR_BAR_SCROLL is broken a/o v29, possibly earlier"
// Probably just a timing issue...

//Optional, but only useful here...
 #define PIXEL_SCROLL TRUE

 //Handles all the displayable colors in a single row for scrolling...
 #define FB_WIDTH 64 //28
 //Two different arrangements of colors...
 #define FB_HEIGHT 2

#else //drawPix from program memory
 #define FB_WIDTH	16
 #define FB_HEIGHT 16
#endif



//DC_DE is only used in drawPix...
//#define DC_DE_DISABLE TRUE
//This should remove DC from Vsync, etc...
// Currently only implemented with FULL_INIT_TESTS below...
//#define REMOVE_DC TRUE

//#define FULL_INIT_TESTS TRUE


//#define BLUE_DIAG_BAR TRUE

//A single black bar, followed by a single blue bar... ~1/3 and 2/3's width
//#define BLUE_VERT_BAR TRUE

//#define DE_BLUE TRUE
// NONE SELECTED DOESN'T SYNC!!! DE not detected???


//#define DE_ONLY_DISPLAY	TRUE


//a/0 v24, this allows the clock signal to be affected by DT
// if it works, we'll be able to use Green on /OC1B
// Sort of works... Blues with DT=0 are good, timing's not screwed
// but blues with DT!=0 are damn-near black except for a light pixel
// before-hand
//a/0 v26, this is now required for Green IN MODES OTHER THAN DRAWPIX
// Not sure where that note makes sense...
// a/o 56-36-3ish it's been removed completely.
//DO NOT define CLOCK_INSENSITIVITY_TESTING TRUE

// Rather than *possibly* mess with timing (on other displays?)
// Also for testing...
//#define DT0_BLUES_ONLY	TRUE

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
//    	Image-shift (not sure why, more calculations? Not *that* many!)
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
//    3/63:			OCR1D = 1   > These three appear identical in glColorTest
//    3/63:			OCR1D = 2  /  (G0 Active, from here down)
//   *35/63:		OCR1D = 3
//   F51/63:		OCR1D = 4
//    59/63:		OCR1D = 5  \  These two appear identical in glColorTest
//   *63/63:		OCR1D >= 6 /
//    (+OC1D => RX0-)
//    Not really worth pursuing, next step from 60/63 is 28/63 then 12
//
// Green is connected to /OC1B (the complementary channel to CLK)
//  Its polarity is reversed (but that's easy since we have a single-ended
//  to differential converter, and its outputs can just be swapped)
//  DTL1 determines its brightness (G0 affected by Red):
//   (/OC1B => RX1+)
//    Off (0/63):	DTL1 = 3 (is this possible?)
//    8-9/63:			DTL1 = 2
//    24-25/63:		DTL1 = 1
//    56-57/63:		DTL1 = 0
//   (/OC1B => RX1-)				(B1,0 Active, as well as G2,1)
//	  *Off (6/63):	DTL1 = 0
//   *38-39/63:		DTL1 = 1
//   F54-55/63:		DTL1 = 2
//   *62-63/63:		DTL1 = 3
//
// Blue, as in previous versions, is connected to OC1A, DTH=0 during DE 
//  so as not to interfere with the clock
//  OCR1A determines the brightness (B1,0 active, per Green->RX1-):
//    (+OC1A => RX2+)			      (B3,2 Active from here down)
//    *Off (15/63):	OCR1A=4
//    *47/63:		 	OCR1A=5
//		*63/63:			OCR1A=6
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
//    51/63:      OCR1D = 4	(FOUR_SHADES only)
//    63/63:      OCR1D >= 6
//   Green: (/OC1B => RX1-)          (B1,0 Active, as well as G2,1)
//    Off (6/63): DTL1 = 0
//    38-39/63:      DTL1 = 1
//    54-55/63:		DTL1 = 2	(FOUR_SHADES only)
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
//    Off (0/63): OCR1D = 0	r=0
//    35/63:      OCR1D = 3	r=142
//    51/63:      OCR1D = 4	r=206
//    63/63:      OCR1D >= 6	r=255
//   Green: (/OC1B => RX1-)          (B1,0 Active, as well as G2,1)
//    Off (6/63): DTL1 = 0		g=24
//    38-39/63:      DTL1 = 1 g=154
//    54-55/63:		DTL1 = 2	g=218
//    62-63/63:      DTL1 = 3 g=251
//   Blue: (+OC1A => RX2+)               (B3,2 Active from here down)
//    Off (15/63):  OCR1A=4	b=61
//    47/63:        OCR1A=5	b=190
//    63/63:        OCR1A=6	b=255
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
//   maxBlue: >¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯\____|		DT=0, OCR=5
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
//            >___________________/¯¯¯¯¯¯¯¯¯\____|		DT=4, OCR=5

//             ____ ____                ____ ____|____ 
//  V w/ H:   X xx X xx \    .    .    / xx X xx X xx X
//   state4    ¯¯¯¯ ¯¯¯¯ ¯¯¯¯ ¯¯¯¯ ¯¯¯¯ ¯¯¯¯ ¯¯¯¯|¯¯¯¯ 
//            >__________________________________|    DT=X, OCR=0
//                             TransitionWatch!!! -------^
//                             Shouldn't matter... DT from no-edge
// DC_DISABLED:
//            >¯¯¯¯¯¯¯¯¯\________________________|		DT=0, OCR=1

//             ____ ____      ____ ____ ____ ____|____ 
//  Nada:     X xx X xx \    /    .    \ xx X xx X xx X    
//   state0    ¯¯¯¯ ¯¯¯¯ ¯¯¯¯           ¯¯¯¯ ¯¯¯¯|¯¯¯¯     
//            >______________/¯¯¯¯¯¯¯¯¯\_________|    DT=3, OCR=4 (+?)
// DC_DISABLED:
//            >______________/¯¯¯¯¯¯¯¯¯¯¯¯¯¯\____|		DT=3, OCR=5

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
//  Nada					DT=2  NADA_OCR = (3<=OCR<(=?)6)

//  Nada	->	H			OCR=2
//  H		->	Nada		OCR=NADA_OCR
//  
//  Nada	->	V			DT=3
//  V		->	V+H		OCR=0
//  V+H	->	V			OCR=NADA_OCR
//  V		->	Nada		DT=2
//
//  Nada	->	DE			DT=1
//  DE	->	Nada		DT=2


#if (defined(REMOVE_DC) && REMOVE_DC)
#error "REMOVE_DC hasn't been tested since lcdStuff, or long prior"
 #define Nada_init()         { DT1=(3<<4); OCR1A=5; }

 //Unused, normally...
 #define Vsync_init()       { DT1=(4<<4);  OCR1A=5; }
 #define VplusH_init()      { DT1=0;       OCR1A=1; }
 #define Hsync_init()       { DT1=(3<<4);  OCR1A=3; }
 #define DEonly_init()      { DT1=(2<<4);  OCR1A=4; }
 #define DEblue_init()		 { DT1=0;       OCR1A=5; }

#else //!REMOVE_DC
//Nada init
 #define Nada_init()         { DT1=(3<<4); OCR1A=4; }

//Unused, normally...
 #define Vsync_init()			{ DT1=(4<<4);	OCR1A=4; }
 #define VplusH_init()		{ DT1=0;			OCR1A=0; }
 #define Hsync_init()			{ DT1=(3<<4);	OCR1A=3; }
// #define DE_init()				{ DT1=(2<<4);	OCR1A=4; } //...SHOULD BE
 #define DEonly_init()			{ DT1=(2<<4);  OCR1A=4; }
 #define DEblue_init()			{ DT1=0;       OCR1A=6; }
//#define DE_init()				{ DT1=(1<<4);	OCR1A=2; } //Works with DE_ONLY
 //#define DE_init()				{ DT1=0;			OCR1A=0xff; }	//DE_BLUE
#endif //REMOVE_DC


//#define FULL_INIT_TESTS TRUE

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
#define DEblue_fromDEonly()	DE_init()
#define Nada_fromDEblue()		Nada_init()
 #if(DE_BLUE)
  #warning "DE_BLUE is true, but not implemented with DE_ONLY_DISPLAY"
  #warning "...The display should be blank"
 #endif
//Use full initialization of each LVDS state
// (might not be good during transitions, but should guarantee
//  steady-state, in case my transitions aren't correct...)
#elif (defined(FULL_INIT_TESTS) && FULL_INIT_TESTS)
#define Vsync_fromNada()		Vsync_init()
#define VplusH_fromVsync()		VplusH_init()
#define Vsync_fromVplusH()		Vsync_init()
#define Nada_fromVsync()		Nada_init()
#define Hsync_fromNada()		Hsync_init()
#define Nada_fromHsync()		Nada_init()
#define DEonly_fromNada()		DEonly_init()
#define DEblue_fromDEonly()	DEblue_init()
#define Nada_fromDEonly()		Nada_init()
#define Nada_fromDEblue()		Nada_init()


#else	//NOT FULL_INIT_TESTS
//#define Vsync_fromNada()   { DT1=(4<<4); }
#define Vsync_fromNada()   { DT1=(4<<4); OCR1A=5; } //Leave two bits high
																	 //for easy-scoping
						//Three would be nicer, but I'm pretty sure OCR=TOP=ON
#define VplusH_fromVsync() { OCR1A=0; }
//#define Vsync_fromVplusH() { OCR1A=4; }
#define Vsync_fromVplusH() { OCR1A=5; }				//Extra bits for scoping
//#define Nada_fromVsync()   { DT1=(3<<4); }
#define Nada_fromVsync()   { DT1=(3<<4); OCR1A=4; } //scoping...
#define Hsync_fromNada()   { OCR1A=3; }
#define Nada_fromHsync()   { OCR1A=4; }

#define Nada_fromDEblue()	{ DT1=(3<<4); OCR1A=4; }
#define Nada_fromDEonly()	{ DT1=(3<<4); }
#define DEonly_fromNada()	{ DT1=(2<<4); }
#define DEblue_fromNada()	{ OCR1A=0xff; }
#define DEblue_fromDEonly() { OCR1A=0xff; }

/*
#if (!defined(DE_BLUE) || !DE_BLUE)
 //Nada -> DE
 //#define DE_fromNada()      { DT1=(1<<4); }	//DT1 = (1<<4);
 #define DE_fromNada()      { DT1=(2<<4); }
 //DE -> Nada
 //#define Nada_fromDE()      { DT1=(2<<4); }	//DT1 = (2<<4);
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
//		 OC1B pin is -differential input (through a resistor)
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
//             ^^^^\														//blah
//             1234 5?
// Pixels are sent during DE High (basically all the CPU will be used here)
//Ideally, 
//  there won't be any glitches when changing from one state to another
//
//  NOTES: PWM1X inversion affects all PWM channels!
//
//  Unchangeables:
//    FastPWM
//		inverted with PWM1X
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
//    | ||        +---- NothingActive \ 								//blah
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
//    | +--- NothingActive \											//blah
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
#define DVH_OCR	OCR1A
#define CLOCK_OCR	OCR1B


void pll_enable(void)
{
	//Stolen from threePinIDer109t:

   //Set Timer1 to use the "asynchronous clock source" (PLL at 64MHz)
   // With phase-correct PWM (256 steps up, then back down) and CLKDIV1
   // this is 64MHz/512=125kHz
   // The benefit of such high PWM frequency is the low RC values necessary
   //  for filtering to DC.
   // "To change Timer/Counter1 to the async mode follow this procedure"
   // 1: Enable the PLL
   setbit(PLLE, PLLCSR);
   // 2: Wait 100us for the PLL to stabilize
   // (can't use dmsWait since the timer updating the dmsCount hasn't yet been started!)
   _delay_us(100);
//   dmsWait(1);
   // 3: Poll PLOCK until it is set...
   while(!getbit(PLOCK, PLLCSR))
   {
      asm("nop");
   }
   // 4: Set the PCKE bit to enable async mode
   setbit(PCKE, PLLCSR);

}

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
	CLKPR = (1<<CLKPS1) | (1<<CLKPS0);	//CLKDIV8
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
//	  (LVDS_PRESCALER != 16) && 
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
	//CS12:10	CS12	CS11	CS10		PLL division
	//1			0		0		1			1
	//2			0		1		0			2
	//3			0		1		1			4
	//4			1		0		0			8
	#define CSBITS divToCS(LVDS_PRESCALER) //<<CS10 should be redundant
	//DTPSBITS (DTPS10 is bit 4)
	//DTPS11:10	DTPS11 DTPS10	PLL division
	//0			0		0			1
	//1			0		1			2
	//2			1		0			4
	//3			1		1			8
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
	// 	NOT from the Timer1 prescaler...
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

	setoutPORT(PB1, PORTB);	//+OC1A, DVH/BLUE, MISO (usually heart)
//	setoutPORT(PB0, PORTB);	//-OC1A 	MOSI unused
	setoutPORT(PB2, PORTB); //-OC1B, -GREEN	 (INVERTED) SCK
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
	//					  or ! at BOTTOM
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
//	TCCR1A = ( (1<<COM1A1) | (1<<COM1A0) 
//			   | (0<<COM1B1) | (1<<COM1B0) 
//			   | (1<<PWM1A) | (1<<PWM1B) )
//#define lvds_ComplementaryClrOnCompare() \ //
	//Do TCCR1C first, because it contains shadow-bits of TCCR1A that I
	// don't want to have to rewrite...
	TCCR1C = ( (1<<COM1D1) | (0<<COM1D0)
				| (1<<PWM1D) );

	TCCR1A = ( (0<<COM1A1) | (1<<COM1A0) 
            | (1<<COM1B1) | (0<<COM1B0) //Don't use complementary for CLK
            | (1<<PWM1A) | (1<<PWM1B) );

//#define lvds_clrOnCompare() lvds_ComplementaryClrOnCompare()

	// Enable Complementary-Mode (and thus the dead-timer)
	// This'll be changed as needed, but we need COM1A0 set prior to that

	//	lvds_clrOnCompare();
//	writeMasked(((0<<COM1A1) | (1<<COM1A0)), 
//					((1<<COM1A1) | (1<<COM1A0)), 
//					TCCR1A);



	//THIS IS NOT THOROUGHLY THOUGHT-OUT...
	// as I recall, we need to use /OC1B for the clock output
	//  because it won't be affected by the deadtime...
//Now Handled in lvds_xxxOnCompare():
//	writeMasked(((0<<COM1B1) | (1<<COM1B0)),
//					((1<<COM1B1) | (1<<COM1B0)),
//					TCCR1A);

   //Since PWM inversion affects all channels, great-pains were taken
   // to assure that it needn't be changed in any lvds state...
   // PWM inversion must be enabled:

	//inverted with PWM1X
//   setbit(PWM1X, TCCR1B);
 
 	Nada_init();
}


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




//This isn't valid, who knows how many instructions the for loop takes
static __inline__ \
void delay_cyc(int32_t numCyc) \
	  __attribute__((__always_inline__));

//This'll optimze-out in some cases.
// The timings and instructions used are probably specific to the
// instruction-set in the MCU, the GCC version, optimization-level, etc.
// a/o LCDdirectLVDS11 HLow_delay() this is how it's compiling...

// The actual number of cycs will probably be a few higher...
// (or who knows, if things optimze-out, e.g. small ~0-8 numCycs)
// Due to init, and division...
// Using rounding-up as well...

// the argument is int32_t to allow for negative value testing
// BUT: numCyc is only tested against a uint16_t...
//  the greatest value (?) is (UINT16_MAX - 7)
// (maybe it'd make more sense to do the math outside the delay_cyc call
// and allow the preprocessor to change it to 0, instead of doing it here.

//This should probably be reimplemented using _delay_loop_1/2() from 
// util/delay_basic.h
//  1 uses three cycles per count, counts from 1-256, 
//		256 counts: _delay_loop_1(0)
//  2 uses four, counts from 1-65536, 65536 is passed as 0

#define DELAY_CYC_DELAY_LOOP TRUE//FALSE

#if(defined(DELAY_CYC_DELAY_LOOP) && DELAY_CYC_DELAY_LOOP)
#warning "This has only been tested with v54+... rowSegBuffer, etc."
void delay_cyc(int32_t numCyc)
{
	if(numCyc <= 0)
		return;

	uint16_t numLoops = (numCyc+3)/4;

	_delay_loop_2(numLoops);

/*	uint32_t delayLoops = (numCyc+3)/4;

	do
	{
		delayLoops--;
//		_delay_loop_2(delayLoops&0xffff);

//		delayLoops -= (delayLoops&0xffff);
	} while(delayLoops);
*/
/*
	//Since this is inline, AND it's only called with values computed
	// at compile time, only one of these should be compiled in...
	// as necessary...
	// If called without precomputed value (i.e. a variable)????
	// Maybe I should only use delay_loop_2...
	if(numCyc < 256*3)
		_delay_loop_1(numCyc/&0xff);

	else
*/
}

#else
void delay_cyc(int32_t numCyc)
{

	//This shouldn't happen often, but some delay_cyc() calls use math
	// to calculate the number of cycles, and it could be negative
	if(numCyc < 0)
		return;
											//#Clocks
	//Two instructions:
	// ldi r24, 0						//1 			i=0
	// ldi r25, 0						//1
	uint16_t i;


	//Loop instructions:
	//nop									//1
	//adiw  r24, 0x01   ; 1			//2 			i++
	//cpi   r24, 0x77   ; 119		//1 			i<numCyc (119 in this case)
	//cpc   r25, r1					//1 			Apparently it's also testing
											//  			the high byte is 0
	//brne  .-10        ;			//"1/2"		return to nop
											//				if I understand, this is two
											//				cyc when branching
											//				or one if not (when complete)

	//READ THIS:
	// Current Compilation Settings: A/O v18:
	//    THIS LOOP WILL BE UNROLLED if numLoops <= 5!!!
	//    Which then turns into numLoops*2 cycles (instead of numLoops*8)
	// 
	// Here's an attempted hack...
	//  it *should* optimize the test out in either case, so it's like a 
	//  preprocessing directive...
	uint16_t numLoops = (((uint16_t)(numCyc)+7)>>3);


	// HACK ATTEMPT 2: THATS A LOT OF CODE.
		switch(numLoops)
		{
			case 5:
				asm("nop");
				asm("nop");
				asm("nop");
				asm("nop");
				asm("nop");
				asm("nop");
				asm("nop");
				asm("nop");
			case 4:
				asm("nop");
				asm("nop");
				asm("nop");
				asm("nop");
				asm("nop");
				asm("nop");
				asm("nop");
				asm("nop");
			case 3:
				asm("nop");
				asm("nop");
				asm("nop");
				asm("nop");
				asm("nop");
				asm("nop");
				asm("nop");
				asm("nop");
			case 2:
				asm("nop");
				asm("nop");
				asm("nop");
				asm("nop");
				asm("nop");
				asm("nop");
				asm("nop");
				asm("nop");
			case 1:
				asm("nop");
				asm("nop");
				asm("nop");
				asm("nop");
				asm("nop");
				asm("nop");
				asm("nop");
				asm("nop");
				return;
				break;
			default:
				break;
		}
	// THUS:
	// Each loop is 7 cycles, make it 8 by adding an extra nop and we can
	// use >> instead of / for calculations...
	// +7 assures rounding-up...
#warning "This loop seems to be optimizing out!"
	//for(i=0; i<((numCyc+7)>>3); i++)
	for(i=0; i<numLoops; i++)
	{
		//THIS IS A HACK DUE TO OPTIMIZATION, see above
		// It will NOT likely be happy with different versions of gcc...
	/*	// NOGO: Apparently it won't expand the loop if this is part of it
		// so then we have 5 loops AND 8 instructions /within/ the loop
		// AND the comparison overhead!`
		if(numLoops <= 5)
		{
		   asm("nop");
			asm("nop");
		//	asm("nop");
		//	asm("nop");
		//	asm("nop");
		//	asm("nop");
		}
		else
		{
			asm("nop");
			asm("nop");
		}
	*/
		//Apparently this loop will optimize-out without this:
		// Obviously, one instruction each...
		asm("nop");
		asm("nop");
	}
}
#endif

//One dot-clock is 7/8ths of a CPU cycle... (prescaler = 1)
//                 14/8ths with prescaler = 2 ...
//
//    1 dot = 7/8 cyc
//    n dots = n * 7/8 cyc
//
//    1 = 1 dot / (7/8) cyc
//    1 = 8 dots / 7 cyc
//
//  With prescaling = 2:
//
//    1 dot = 7/8 * 2 cyc = 14/8 cyc
//   
//    8/14 dots = 1 cyc
//    n dots = n*14/8 cyc
//   
//    I coulda sworn the prescaler was on the divide-side...
//   Also, is there any benefit to adding +7*LVDS_PRESCALER-1 for rounding?
#define DOTS_TO_CYC(dots) ((((dots)*(7l)*(LVDS_PRESCALER))/8))

#define delay_Dots(numDots) delay_cyc(DOTS_TO_CYC(numDots))


//THESE NOTES ARE OLD:
//This has to be changed every time the DE code is changed...
// Actually, I think it should just match IMAGE_WIDTH
//#define FB_WIDTH 28 //21//19
//#define IMAGE_WIDTH 21


#define TESTVALS	0
#define LTN 1
#define IDT 2


//TIMING:
//  Measurements below (in microseconds) are referring to preLVDS
//  There may be other such old-notes...


//        |<--T_Hhigh-->| |<-T_Hlow
//    __   _____________   ____________  3.6V
// H    |_|             |_|           _  0V
//
//    Data Enable
//       Only active when there's valid pixel data
//       (for 512 of the 717 clocks in HSYNC-active)
//       Low during Vsync... (~720us)
//

//IDTech display: 136 dots
//2-136+
//IDTech, last used: 40
//LTN Last used 8
// LTN: 1072-1344-1500 clocks per line, 1024DE... Hsync unused
// SEE Hlow_Delay() notes if this value is large!

#if (DISPLAY == LTN)
#define H_LOW_DOTS	8
#elif (DISPLAY == IDT)
#define H_LOW_DOTS   40//8//1//40 //30 //2//136
#elif (DISPLAY == TESTVALS)
#define H_LOW_DOTS	100//0 //LTN doesn't use H_LOW (DE-only)
							  // And having a value here increases codesize
#endif


//H is set low immediately upon entry of the timer interrupt
// its low-time is controlled via nops
// (so if HLow is long for a particular display, this might need revising)
#define Hlow_Delay() \
	delay_Dots(H_LOW_DOTS)


//#define T_Hlow_CYC 1//8 //(T_Halow_REAL*3)
#define T_Hlow_CYC DOTS_TO_CYC(H_LOW_DOTS)



//This was found expermentally with SwitchResX
//IDTech Last Used: 680
//LTN last used 1024
// Oddly: For the LTN, this doesn't seem to affect the horizontal
//        it does, however, affect the vertical(!)
// If I recall Correctly, this is used only for setting the timer interrupt
//  rate...
#if (DISPLAY == LTN)
#define DE_ACTIVE_DOTS	1024
#elif (DISPLAY == IDT)
#define DE_ACTIVE_DOTS	680 //1024 //800 //920 //943 //812 //472 //1024
#elif (DISPLAY == TESTVALS)
#define DE_ACTIVE_DOTS	1024
#endif

#define T_DE_CYC	DOTS_TO_CYC(DE_ACTIVE_DOTS)

//Horizontal Blank Time: HD_DOTS + H_LOW + DH_DOTS
//  90-320+
//Horizontal Total Time: 1206-1344-2047
// ... but Total = Blank + Pixels, so 90+1024 != 1206
// Excess waits should be thrown in DH (since that's between interrupts)
//T_HD is the time from H inactivated to DE activated
//aka H back-porch: 1-160+
//IDTech Last Used: 50
//LTN last used 5

#if (DISPLAY == LTN)
#define HD_DOTS 5
#elif (DISPLAY == IDT)
#define HD_DOTS	50 //5 //0//20//5 //50 //30 //1 //160
#elif (DISPLAY == TESTVALS)
#define HD_DOTS 50//5
#endif

#define HD_Delay()	delay_Dots(HD_DOTS)

//#define T_HD_CYC 1//3//(T_HD_REAL*3)
#define T_HD_CYC	DOTS_TO_CYC(HD_DOTS)

// This is the number of CPU cycles between DE->L and Horiz->L
// This is just used in calculating the OCR value...
// aka H Front Porch: 0-24+
// IDTech Last Used: 24
// LTN last used 46
// 1072-1024-1-1 = 46
#if (DISPLAY == LTN)
#define DH_DOTS	46
#elif (DISPLAY == IDT)
#define DH_DOTS	24 //46 //480//6//12//24 //46 //24 //122//30 //0 //24
#elif (DISPLAY == TESTVALS)
#define DH_DOTS	0//1000//100//46
#endif

#define T_DH_CYC	DOTS_TO_CYC(DH_DOTS)

//       |
//       v
//      T_DH
//      ||    |<-T_DE->|  |  |<--T_HD (Hsync->DE)
// H   __.  ____________  .___________
//      .|_|  .        .|_|  .
//     _.     .________.     .___________
// DE   |_____| 512clk |_____|
//      ^              ^
//       \___________   \                        (just avoiding \ warning)
//                   \  |     768 Hsyncs
//                    v v
//     _             _ _ _ _ _ _ _ _ _ _ 
// DE   |___________| | | | | | | | | | |______
//
// one Hsync = T_HD+T_DH+T_Hlow+T_DE = 19.865us

//aka V-sync back porch: 7-29-63 "Vbp should be static"
// IDTech Last Used: 29
// LTN last used 3
//LTN Frame: 772-806-1000 lines...
#if (DISPLAY == LTN)
#define T_VD 3
#elif (DISPLAY == IDT)
#define  T_VD 29 //3//29 //7 //29
#elif (DISPLAY == TESTVALS)
#define T_VD 3
#endif

//aka V-sync front porch: 1-3+
// IDTech Last Used: 3
// LTN last used 3
#if (DISPLAY == LTN)
#define T_DV 3
#elif (DISPLAY == IDT)
#define T_DV 3//3 //1 //3
#elif (DISPLAY == TESTVALS)
#define T_DV 3
#endif

//1-6+ (1 is used by the first switch-case...)
// IDTech Last Used: 6
// LTN Last used 16
// 772 - 768 - 1 - 1 = 2
#if (DISPLAY == LTN)
#define T_Vlow 16
#elif (DISPLAY == IDT)
#define T_Vlow	6 //16//6 //2 //6
#elif (DISPLAY == TESTVALS)
#define T_Vlow 32
#endif

// IDTech Last Used: 768
#if (DISPLAY == LTN)
#define V_COUNT	768
#elif (DISPLAY == IDT)
#define V_COUNT	768
#elif (DISPLAY == TESTVALS)
 #if (LVDS_PRESCALER == 8)
  #define V_COUNT 768
 #else
  #define V_COUNT	(768*2/3)
 #endif
#endif

//                       
//                      |           
//                      V           
//              |<-T_DV>|           |<-T_VD-->|
//              .       .           .         .
//     _____________||__.   124us   .____||___________________
//  V           .   ||  |___________|    ||   .
//     _ _ _ _ _ _ _|| _ _ _ _ _ _ _ _ _ ||_ _ _ _ _ _ _ _ _ _ _
//  H   | | | | | | ||| | | | | | | | | ||| | | | | | | | | | |
//              .   ||                   ||   .
//     _ _ _ _ _.   ||       720us       ||   ._ _ _ _ _ _ _ _ _
// DE   | | | | |___||___________________||___| | | | | | | | |
//                  ||                   ||
//
//                        1
//                        |
//                        v               
//     _________________   ___________________   _____________
//  V                   |_|                   |_|
//  H  |||||||||||||||||||||||||||||||||||||||||||||||||||||||
// DE  ||||||||||||||||_____|||||||||||||||||_____||||||||||||
//                          ^
//                          |
//                          2




static __inline__ \
void loadData(uint16_t rowNum, uint8_t dataEnable) \
	  __attribute__((__always_inline__));

//volatile uint8_t rowBuffer[FB_WIDTH] =
//There's not enough memory for a full frame-buffer...
// but this is how LCDdirect worked, and changing it would be a pain.
//uint8_t settingBuffer[FB_HEIGHT][FB_WIDTH]; 
#if(defined(IMAGE_BUFFER) && IMAGE_BUFFER)
uint8_t frameBuffer[FB_HEIGHT][FB_WIDTH];
#else
uint8_t *pimage;
#endif

/* in LCDStuff
uint8_t dataEnable = 0;
uint8_t vSync = 0;
uint16_t hsyncCount = 0;
*/

/* in LCDStuff, along with the if, woot
//frameCount=0 can be used in main to detect whether we've completed a 
//  a frame AND its FRAME_UPDATE_DELAY
#if (FRAME_COUNT_TO_DELAY != 0)
uint8_t frameCount = 0;
#endif
*/

static __inline__ \
void loadRow(uint16_t rowNum) \
	__attribute__((__always_inline__));


#include _LCDSTUFF_CFILE_
//#include "../../../_commonCode/lcdStuff/0.50ncf/lcdStuff.c"
//#include "lcdUpdate.c"

//SIGNAL(TIMER1_COMPA_vect)
SIGNAL(TIMER0_COMPA_vect)
{
	static uint8_t frameCount = 0;

#if (defined(SLOW_EVERYTHING_TEST) && SLOW_EVERYTHING_TEST)
	while(TCNT0L != 7){};
	//This won't work... who knows how many cycles interrupt-entry will be
	//Above should fix that...
	//I believe this was only so I'd have something for the 'scope to
	// trigger on... 
#error "These shouldn't be used anymore, since PB2 is GREEN"
	setpinPORT(PB2, PORTB);
	clrpinPORT(PB2, PORTB);
#endif


	if(updateLCD())
	{
#if (FRAME_COUNT_TO_DELAY != 0)
			//static uint8_t frameCount = 0;
			frameCount++;
			if(frameCount == FRAME_COUNT_TO_DELAY)
			{
				frameCount = 0;
				timer_compareMatchIntSetup(0, OUT_CHANNELA, FALSE);
				//timer_compareMatchIntDisable(0, OUT_CHANNELA);
			}
#endif
	}

#if(defined(LOADROW) && LOADROW)
	//Load the next row into the row(seg)buffer
	//data is enabled after T_VD+T_Vlow...
	if((hsyncCount >= T_VD+T_Vlow) && (hsyncCount < T_VD+T_Vlow+V_COUNT))
		loadRow(hsyncCount - (T_VD+T_Vlow));
#endif
	// Here is where loadRow used to be called
	// it has been moved to oldNotes.txt, but it would make more sense
	// to look at LCDdirectLVDS <25 or LCDdirect
	// loadRow used the time between DE inactive and the next Hsync
	// to load a row to a row-buffer (as opposed to a frame-buffer)
	// thus, DE could gain more pixels because there would be fewer
	// calculations to determine the memory location to write from
	// This effect has since been minimized
	// BUT loadRow might be necessary for program-memory-based images...
}


#define nibbletochar(val)	\
	(((val)<=9) ? ((val) + '0') : ((val) + ('A' - 10)))


#if 0
uint8_t hexColor = 0;

static __inline__ \
void hexColor_drawRow(uint8_t rbRowNum, uint8_t rowBuffer[]) \
	  __attribute__((__always_inline__));

//Takes a nibble and makes it a hex character...

#define HEXCOLOR_TOPROW 32
void hexColor_drawRow(uint8_t rbRowNum, uint8_t rowBuffer[])
{
//	static uint8_t color = 0;

	if( (rbRowNum >= HEXCOLOR_TOPROW) &&
		 (rbRowNum < HEXCOLOR_TOPROW+8) )
	{
		uint8_t charRow1, charRow2;
		//uint8_t color;

		charRow1 = 
			getCharRow(nibbletochar((hexColor)>>4), rbRowNum-HEXCOLOR_TOPROW);
		charRow2 = 
			getCharRow(nibbletochar(hexColor&0x0f), rbRowNum-HEXCOLOR_TOPROW);

		uint8_t col;
		for(col=0; col<8; col++)
		{
			if(getbit(col, charRow1))
				rowBuffer[col+1] = fb_to_rb(hexColor);
			//else
			//	rowBuffer[col] = fb_to_rb(0x00);

			if(getbit(col, charRow2))
				rowBuffer[col+8+1] = fb_to_rb(hexColor);
		}
	}
}
#endif //0


#if(defined(LOADROW) && LOADROW)
#warning "loadRow is currently in an intermediate phase..."
void loadRow(uint16_t rowNum)
{

#if 0
	uint8_t i=0;

//	if((rowNum < 0) || (rowNum >= V_COUNT))
//		return;

	//THIS DOES NOT MAKE AN OUNCE OF SENSE TO ME:
	// why do I need *2 here?! Without it, the image is stretched and cut
	// in half vertically!
	// Apparently has to do with delayed Hsyncs due to calculations...?
	// LIFE with delay = 10 causes 2/3 frame at *2
	// The screen itself appears to be repeating rows...
#if(!defined(HORIZONTAL_COLOR_BARS) || !HORIZONTAL_COLOR_BARS)
 #if(LVDS_PRESCALER == 8)
  #define ROW_SCALE 1
 #else
  #define ROW_SCALE 2
 #endif
	uint8_t rbRowNum = ((uint32_t)rowNum)*RB_HEIGHT*ROW_SCALE/V_COUNT;

	//Old note pre tet_drawRow... (code deleted a/o v45)	
	//WITHOUT THIS: syncing seems OK
	// But this is necessary for the right-border...
	// Adding it causes syncing problems at _b and _c -> _W
	// TODO: Look into sync-problems above!!!
	//  (revisit 41-1)
	//i++;

	//Clear the rowBuffer, it will be reloaded, where appropriate, after
	// This isn't particularly speed-efficient, since most pixels will 
	// be written twice... but it should make things more plug-n-play...
	for(i=0; i<RB_WIDTH; i++)
		rowBuffer[i]=fb_to_rb(0);


	tet_drawRow(rbRowNum, rowBuffer);


	hexColor_drawRow(rbRowNum, rowBuffer);

	
	i=63-40;

	static uint16_t colorShift = 0;

	//!!! This *should* only increment once per frame (RIGHT?)
	// BUT, set colorShift>>5 to colorShift>>3 or even >>0
	// and it seems this is not true... WTF, again?
	if(rbRowNum == 0)
		colorShift++;

	//Just fill up the remaining pixels with color...
	// i-- to overwrite the blank column in the characters
	// Leaving one column of black to check into syncing problems
	// with LVDS_PRESCALER=8...
	// apparently, the garbage-lines are related to the last color drawn(?!)
	for( ;i<RB_WIDTH;i++)
	{
			uint8_t colorVal = (i + rbRowNum + (colorShift>>5)) & 0x3f;

			//We don't want Black, since it's used for LIFE...
			if(colorVal==0)
				colorVal = 0x15;

			//Only draw colors where there isn't already data
			// (e.g. LIFE or the SMILEY)
			// Actually, this can't work, since Black is a possibility...
			//if(!rowBuffer[i])
				rowBuffer[i] = fb_to_rb(colorVal);
	}

/* This is now handled in the rowSegBuffer loading...
	I guess that doesn't fix it for the old non-row-seg case...

	//Apparently *black* causes issues... as well as some dark colors
	// (certain shades of blue?)
	// White, as the last pixel, causes syncing to work fine, ish
	//And adding a black column doesn't do anything?!
	// either way, thsi should be redundant due to rowBuffer clearing
	// and RB_WIDTH-2 in the color-filling
	// WTF?
	//rowBuffer[RB_WIDTH-2] = fb_to_rb(_K);
	rowBuffer[RB_WIDTH-1] = fb_to_rb(_W);
*/

	//Draw the smiley on top of the color pixels
#define SMILE_TOP_ROW (20)
#define SMILE_HEIGHT FB_HEIGHT
#define SMILE_SHIFT	(24+(40-FB_WIDTH)/2)
	if((rbRowNum >= SMILE_TOP_ROW) &&
			(rbRowNum < (SMILE_HEIGHT+SMILE_TOP_ROW)))
	{
			for(i=0; i<FB_WIDTH; i++)
			{
				uint8_t temp;
			#if(defined(IMAGE_BUFFER) && IMAGE_BUFFER)
         	temp=frameBuffer[rbRowNum-SMILE_TOP_ROW][i];
			#else
         	temp=pgm_readImageByte(pimage, rbRowNum-SMILE_TOP_ROW, i);
			#endif

				if(!(temp & 0xC0))
					rowBuffer[i+SMILE_SHIFT] = fb_to_rb(temp);
			}
	}


	#if(defined(LIFE) && LIFE)
	//This has to be called *after* the colored background is drawn...
	// (that could be changed... with _Tr?)
	life_drawRow(rbRowNum, &(rowBuffer[RB_WIDTH-LIFE_WIDTH]));
	#endif //LIFE


#else //HORIZONTAL_COLOR_BARS
	for(i=0; i<RB_WIDTH; i++)
		rowBuffer[i] = fb_to_rb(rowNum);
#endif


#endif //0

	

//#define SEG_QUESTION TRUE
	//Isn't BLAH = 1 necessary so we don't get a row with no data?
	// *looks* like it's working, but I dunno...
#define BLAH 0


#if(defined(ROW_SEG_BUFFER) && ROW_SEG_BUFFER)
// Wasn't sure where 357 came from... and now I'm wondering where the
// notes I wrote regarding it disappeared to...
// Anyhow, from line 72, if ROW_SEG_BUFFER && !LVDS_PRESCALER
// "ROW_SEG_BUFFER uses 20cyc/pixel" (doesn't match the math I've 
// elsewhere) 1024/20*7 = 358.4... so maybe that's something to do with it
// TODO: If it was 21cyc/pixel, wouldn't we have perfect alignment with
//       color-transitions and segment widths?
//#define NUM_PSEGS	(1024/3+16) //357
 #if (defined(SEG_RACER) && SEG_RACER)
  #include <stdlib.h>	//rand()
  #define NEXT_COLOR	_R
  #define THIS_COLOR	_G
  #define OVERLAP_COLOR	_Y
  #define BG_COLOR	_C
  #define BG_BAD_COLOR	_M



	int16_t racerPseg = NUM_PSEGS/2 - 2;

	adc_startConversion();
	while(adc_isBusy())
		asm("nop;");
	racerPseg = (adc_getValue()>>2)+5;
	if(racerPseg > NUM_PSEGS-2)
		racerPseg = NUM_PSEGS-2;

	segClear();
	addSegfb(1, _W);
	static uint16_t raceWidth = 16;
	static uint8_t trackPosition = 0;
  #define TRACK_LENGTH	24	//24*32 = 768
  #define TRACK_ROWS	(V_COUNT/TRACK_LENGTH)
	static uint16_t raceTrack[TRACK_LENGTH*2];

	uint8_t bgColor = BG_COLOR;

	//racePseg += ((rand()&0xff0)>>4)*2/0xff - 1;

	uint8_t thisTrackRow = rowNum/TRACK_ROWS; //(V_COUNT/TRACK_LENGTH);
	uint8_t thisPosition = thisTrackRow + (trackPosition? TRACK_LENGTH : 0);
	uint8_t nextPosition = thisTrackRow + (trackPosition? 0 : TRACK_LENGTH);

	static hfm_t thisSmoothingHFM, nextSmoothingHFM;
	static uint8_t thisTrackGrowing, nextTrackGrowing;

/*
	static theta_t racerTheta = 0;
	if(racerTheta >= SINE_2PI)
		racerTheta -= SINE_2PI;

	racerPseg = sineRaw8(racerTheta) + -(INT8_MIN);
	racerTheta++;
*/

	uint8_t outOfTrack = FALSE;
	typedef struct _IST_BLAH_
	{
		uint16_t length;
		uint8_t color;
	}	intermediateSeg_t;

#define ISEGS_PER_WHATEVER	3
#define IS_SEGS (5*ISEGS_PER_WHATEVER)
	intermediateSeg_t is[IS_SEGS] = 
	{ {0,_W}, {0,_W}, {0,_W}, {0,_W}, {0,_W},
	  {0,_W}, {0,_W}, {0,_W}, {0,_W}, {0,_W},
	  {0,_W}, {0,_W}, {0,_W}, {0,_W}, {0,_W} };

	uint8_t isPos = 0;

#define addSegIS(len, c) \
({\
 	is[isPos].length = (len); \
 	is[isPos].color = (c); \
 	isPos+=ISEGS_PER_WHATEVER; \
 	{}; \
})



	int8_t prevNextPosition;
	int8_t prevThisPosition;

	static uint16_t thisIS, nextIS;

	static uint8_t level = 1;


	if(rowNum%TRACK_ROWS == 0) //(V_COUNT/TRACK_LENGTH) == 0)
	{
		prevNextPosition = nextPosition - 1;
		if(prevNextPosition < 0)
			prevNextPosition = TRACK_LENGTH*2 - 1; //+prevNextPosition;

		prevThisPosition = thisPosition - 1;
		if(prevThisPosition < 0)
			prevThisPosition = TRACK_LENGTH*2 - 1; //+prevThisPosition;
		
		static theta_t racerTheta=0;

		int16_t nextTrackPseg;

		nextTrackPseg = sineRaw8(racerTheta) + -(INT8_MIN);
		racerTheta+=level;
		if(racerTheta >= SINE_2PI)
			racerTheta-=SINE_2PI;
/*
	int8_t randomVal = rand()&0x03;
	switch(randomVal)
	{
		case 1:
			if(nextTrackPseg < NUM_PSEGS-2-raceWidth)
				nextTrackPseg += 1;
			break;
		case 2:
			if(nextTrackPseg > 1)
				nextTrackPseg -= 1;
			break;
		default:
			break;
	}
*/


		raceTrack[nextPosition] = nextTrackPseg;

		thisIS = raceTrack[prevThisPosition];
		nextIS = raceTrack[prevNextPosition];
		uint8_t nextHFMdist, thisHFMdist;

		if(raceTrack[nextPosition] > nextIS)
		{
			nextTrackGrowing = TRUE;
			nextHFMdist = raceTrack[nextPosition] - nextIS;
		}
		else
		{
			nextTrackGrowing = FALSE;
			nextHFMdist = nextIS - raceTrack[nextPosition];
		}
		
		if(raceTrack[thisPosition] > thisIS)
		{
			thisTrackGrowing = TRUE;
			thisHFMdist = raceTrack[thisPosition] - thisIS;
		}
		else
		{
			thisTrackGrowing = FALSE;
			thisHFMdist = thisIS - raceTrack[thisPosition];
		}

		hfm_setup(&nextSmoothingHFM, nextHFMdist, TRACK_ROWS);

		hfm_setup(&thisSmoothingHFM, thisHFMdist, TRACK_ROWS);
	}


	if(thisTrackGrowing)//raceTrack[thisPosition] > raceTrack[prevThisPosition])
		thisIS += hfm_nextOutput(&thisSmoothingHFM);
	else
		thisIS -= hfm_nextOutput(&thisSmoothingHFM);

	if(nextTrackGrowing)//raceTrack[nextPosition] > raceTrack[prevNextPosition])
		nextIS += hfm_nextOutput(&nextSmoothingHFM);
	else
		nextIS -= hfm_nextOutput(&nextSmoothingHFM);

	if(thisIS < nextIS)
	{
		addSegIS(thisIS, bgColor);

		//Tracks overlap
		if(nextIS < thisIS + raceWidth)
		{
			addSegIS(nextIS - thisIS, THIS_COLOR);
			addSegIS(thisIS + raceWidth - nextIS, OVERLAP_COLOR);
			addSegIS(nextIS - thisIS, NEXT_COLOR);
			//outOfTrack=TRUE;
		}
		else
		{
			addSegIS(raceWidth, THIS_COLOR);
			addSegIS(nextIS - thisIS - raceWidth, bgColor);
			addSegIS(raceWidth, NEXT_COLOR);
		}

		//addSegIS(NUM_PSEGS-2-nextIS-raceWidth, bgColor);
		addSegIS(RACE_WIDTH_PSEGS-nextIS-raceWidth, bgColor);

	}
	else if(thisIS > nextIS)
	{
		addSegIS(nextIS, bgColor);

		//Tracks overlap
		if(thisIS < nextIS + raceWidth)
		{
			addSegIS(thisIS - nextIS, NEXT_COLOR);
			addSegIS(nextIS + raceWidth - thisIS, OVERLAP_COLOR);
			addSegIS(thisIS - nextIS, THIS_COLOR);
			//outOfTrack=TRUE;
		}
		else
		{
			addSegIS(raceWidth, NEXT_COLOR);
			addSegIS(thisIS - nextIS - raceWidth, bgColor);
			addSegIS(raceWidth, THIS_COLOR);
		}
	
		//addSegIS(NUM_PSEGS-2-thisIS-raceWidth, bgColor);
		addSegIS(RACE_WIDTH_PSEGS-thisIS-raceWidth, bgColor);
	}
	else //Positions equal
	{
		//outOfTrack=TRUE;
		addSegIS(thisIS, bgColor);

		addSegIS(raceWidth, OVERLAP_COLOR);
		//addSegIS(NUM_PSEGS-2-thisIS-raceWidth, bgColor);
		addSegIS(RACE_WIDTH_PSEGS-thisIS-raceWidth, bgColor);
	}
/*
addSegfb(raceWidth, _W);
	addSegfb(NUM_PSEGS-racePseg-raceWidth-2, _R);
*/
	uint8_t isSeg;
	uint16_t isSegTrackPos = 0;
	uint16_t oldTrackPos = 0;
	//uint8_t racerFound = FALSE;

	for(isSeg = 0; isSeg<IS_SEGS; isSeg++)
	{
		isSegTrackPos += is[isSeg].length;

		//if(!racerFound && 
		if((isSegTrackPos >= (uint16_t)racerPseg))
		{
			//racerFound = TRUE;
			uint16_t oldLength = is[isSeg].length;
			is[isSeg].length = racerPseg - oldTrackPos;
			is[isSeg+1].length = 1;
			is[isSeg+1].color = _K;
			//Not sure why this shouldn't be -1...
			is[isSeg+2].length = oldLength - is[isSeg].length; //- 1;
			is[isSeg+2].color = is[isSeg].color;
			if((is[isSeg].color != THIS_COLOR) && (is[isSeg].color != OVERLAP_COLOR))
				outOfTrack = TRUE;
			isSeg++;
			break;
		}
		oldTrackPos = isSegTrackPos;
	}

	
	for(isSeg = 0; isSeg<IS_SEGS; isSeg++)
  	{
		if(is[isSeg].length == 0)
			continue;
		if(outOfTrack && (is[isSeg].color==BG_COLOR))
			addSegfb(is[isSeg].length, BG_BAD_COLOR);
		else
			addSegfb(is[isSeg].length, is[isSeg].color);
	}

	static uint32_t badCount=0;
	static uint32_t goodCount=0;

	if(outOfTrack)
		badCount++;
	else
		goodCount++;

	
	addSegfb(1, _B);


	uint8_t percentage = goodCount*100/(badCount+goodCount);
	static uint32_t lastLevelUpdate = 0;

	if((percentage > 50) && (goodCount - lastLevelUpdate > V_COUNT*3))
	{
		lastLevelUpdate = goodCount;
		level++;
	}

	if((rowNum>=6) && (rowNum < 22))
		showScore(rowNum-6, level, _C);

	else if((rowNum>=24) && (rowNum <40))
		showScore(rowNum-24, goodCount, _G);
	else if((rowNum >= 42) && rowNum < 58)
		showScore(rowNum-42, badCount, _R);
	else if((rowNum >= 60) && (rowNum < 76))
		showScore(rowNum-60, percentage, _Y);


	addSegfb(NUM_PSEGS-2-RACE_WIDTH_PSEGS, _B);

	addSegfb(1, _W);
	segTerminate();

	if(rowNum >= V_COUNT-1)
		trackPosition = !trackPosition;

 #elif (defined(SEG_HFM) && SEG_HFM)
	segClear();
	//Just some experimenting... 1024/3+16 seems to be the width...
	// == 357 segments (Where does this come from?)
//Pixel Segments...
#define BORDER_PSEGS	1
#define SEGS_AVAILABLE (NUM_SEGMENTS-2)
#define PSEGS_AVAILABLE	(NUM_PSEGS-BORDER_PSEGS*2)
#define HFM_PSEGS	(PSEGS_AVAILABLE/SEGS_AVAILABLE)
#define HFM_PSEGS_REMAINDER	(PSEGS_AVAILABLE%SEGS_AVAILABLE)
	addSegfb(BORDER_PSEGS,_W);

	hfm_t hfmThing;
	hfm_setup(&hfmThing, (uint8_t)((uint32_t)rowNum*(uint32_t)SEGS_AVAILABLE/V_COUNT), SEGS_AVAILABLE);

	hfm_t hfmColStretcher;
	hfm_setup(&hfmColStretcher, HFM_PSEGS_REMAINDER, SEGS_AVAILABLE);

	uint16_t pseg;
	for(pseg=0; pseg<SEGS_AVAILABLE; pseg++)
	{
		if(hfm_nextOutput(&hfmThing))
			addSegfb(HFM_PSEGS + hfm_nextOutput(&hfmColStretcher),_R);
		else
			addSegfb(HFM_PSEGS + hfm_nextOutput(&hfmColStretcher),_C);
	}

	//addSegfb(NUM_PSEGS-BORDER_PSEGS*2-SEGS_AVAILABLE*HFM_PSEGS, _M);
	addSegfb(BORDER_PSEGS,_W);
	segTerminate();

 #elif (defined(SEG_QUESTION) && SEG_QUESTION)

//Actually, this probably won't work right...
// because the rgb values aren't exact like that, right?
// e.g. black = 0,0,0 but it shows as 0,0,60 ish...
// then again, going in reverse like this might work fine...
// due to rounding (seems OK with this color-scheme)
// The idea wasn't so much to be able to input an 8-bit RGB value to get
// a close match (would probably want rounding up for that, not that it'd
// be even close)
// but to be able to read color-values from GIMP when using the 
//  LCDdirectLVDS color-palette... which isn't especially necessary since
//  now the colors are named with 3-based colors.
#define rgb8(r,g,b) \
		rgb((((r)*3)/255), (((g)*3)/255), (((b)*3)/255))



//THESE DON'T BELONG HERE
// AND they shouldn't be "Q" specific, at all.
#define Q_WIDTH	16
#define Q_HEIGHT	16
		//This should be an init-thing, I guess... it's only a 2-cycle
		// instruction, so it doesn't hurt too much to have it in the loop
		// BUT it does have to occur a while before the first getpinPORT
		// to allow time for the pull-ups to do their job
		setinpuPORT(PB0, PORTB);

	static uint8_t qCount = 0;

	segClear();
	//Not sure why this is stretched without /2...
	// I think I ran into this elsewhere as well.
	//!!! INSTEAD: filled the rest of the row properly...
	// (wasn't filling up the entire row, due to rounding...
	//   result was the rows were carrying-over into the next)
	uint8_t qRow;// = (rowNum*Q_HEIGHT/(V_COUNT)); ///2));
	uint8_t qCol;
	
	//Guess white helps with sync...?
	// can't hurt.
	//addSegfb(3, _W);
	// Looks like some white is already being inserted (or syncing on end-
	// white?) about 3pixels wide.

	// 1024/3/32 = 10.67... so for every 3 segments, add 2
#define Q_STRETCHTOP	1//0//28//2
#define Q_STRETCHDIV 5//30//3
	hfm_t colincrementer_hfm;
	hfm_setup(&colincrementer_hfm, Q_STRETCHTOP, Q_STRETCHDIV);

	for(qCol=0; qCol<Q_WIDTH; qCol++)
	{
		//Not sure where this is defined, if it is...
		// each "segment length" appears to be 3 physical pixels
		//+1 is a result of random experiments with stretch
		// resulting in a stretch larger than 1
		#define Q_SEGLENGTH	(1024/3/Q_WIDTH+1)

	//These are not included in NUM_ICONS
	#include "icons/Question.h"
	#include "icons/Solid.h"
	#include "icons/GoombaDead.h"

	//These are included in NUM_ICONS
	#include "icons/1up.h"
	#include "icons/Biggie.h"
	#include "icons/StarV.h"
	#include "icons/FlowerPowerV.h"
	#include "icons/Goomba.h"
	#include "icons/Coin.h"

	//Excluding Solid and Question, etc...
	#define NUM_ICONS 6
		/*
		static uint8_t iconSelection = 0;
		static uint8_t * icon[NUM_ICONS] = { pgm_imageUP, pgm_imageCOIN};
		static uint8_t iconFrames[NUM_ICONS] = {NUM_UPS, NUM_COINS};

		static uint8_t * p_image = pgm_imageUP; // = icon[0]; //(pgm_imageFLOWER);
		static uint8_t NumFramesInImage = NUM_UPS; // = NUM_FLOWERS;
		*/
		//For changing images...
		static sprite_t *p_selectedSprite = &spriteQ;
		static uint16_t buttonPressed = FALSE;

#define DEBUGGING_PRESSIMMEDIATE TRUE

#if(!defined(DEBUGGING_PRESSIMMEDIATE) || !DEBUGGING_PRESSIMMEDIATE)
		if((p_selectedSprite == &spriteQ) 
				|| (p_selectedSprite == &spriteGOOMBA))
#else
	#warning "Debugging mode... players can change their result!"
#endif
		if(!getpinPORT(PB0, PORTB))
		{
			if(!buttonPressed)
			{
				buttonPressed = rowNum;
				if(buttonPressed == 0)
					buttonPressed = 1;
			}
			else if((rowNum-buttonPressed) >= 64)
				buttonPressed += 0x4000;
		}


#define REALLY_IMMEDIATE	TRUE
#if (defined(REALLY_IMMEDIATE) && REALLY_IMMEDIATE)
		if((rowNum == 0) || (buttonPressed&0x8000))
#else
		if(rowNum==0)
#endif
		{
			if(!(buttonPressed&0x8000) && !((p_selectedSprite == &spriteQ) ||
												(p_selectedSprite == &spriteGOOMBA)))
				buttonPressed = FALSE;

			if(buttonPressed)
			{
				if(p_selectedSprite == &spriteGOOMBA)
				{
					if(qCount == 1)
						p_selectedSprite = &spriteDEADGOOMBA;

				}
				else
				{
					//Duh...
					// But still, 255 % 3 should give a value from 0 to 2
					//buttonPressed=FALSE;
					qCount = 0;
					//static uint8_t iconSelection;
					//iconSelection++;
					//iconSelection%=NUM_ICONS;
					//NumFramesInImage = iconFrames[iconSelection];
//#define RANDOMSPRITE_OVERRIDE 0
#if(defined(RANDOMSPRITE_OVERRIDE))
					switch(RANDOMSPRITE_OVERRIDE)
#else
					switch ((buttonPressed-1)%(NUM_ICONS*2))
#endif
					{
						case 0:
							p_selectedSprite = &spriteFLOWER;
							break;
						case 1:
							p_selectedSprite = &sprite1UP;
							break;
						case 2:
							p_selectedSprite = &spriteBIG;
							break;
						case 3:
							p_selectedSprite = &spriteSTAR;
							break;
						case 4:
							p_selectedSprite = &spriteGOOMBA;
							break;
						default:
							p_selectedSprite = &spriteCOIN;
							break;
					}
				}

				//buttonPressed=FALSE;
			}
			if(p_selectedSprite == &spriteCOIN)
			{
				if(qCount >= 2)
					p_selectedSprite = &spriteSOLID;
			}
			else if(p_selectedSprite != &spriteQ)
			{
				if(qCount >= 4)
				{
					qCount = 0;
					if(p_selectedSprite != &spriteSOLID)
						p_selectedSprite = &spriteSOLID;
					else
						p_selectedSprite = &spriteQ;
				}
			}

			buttonPressed = FALSE;
		}

		qRow = (rowNum*ICON_HEIGHT/(V_COUNT)); ///2));
		



//		p_image = (icon[iconSelection] + 
//						(qCount%NumFramesInImage)*ICON_HEIGHT*ICON_WIDTH);

		// Should use readImageByte... but it's specific to FB_WIDTH
		//uint8_t data=
		//  pgm_read_byte((uint8_t *)(&((p_image)[(qRow)*Q_WIDTH+(qCol)])));
		uint8_t data;
	  
		if(p_selectedSprite == &spriteFLOWER)
		{
			if(qRow<FLOWER_PALETTE1_ROW)
				data = getGimpColorVal(&spriteFLOWER,
							qCount%(spriteFLOWER.numPalettes-1) + 1, qRow, qCol);
			else
				data = getGimpColorVal(&spriteFLOWER,0,qRow, qCol);
		}
		else if(p_selectedSprite == &spriteGOOMBA)
		{
			uint8_t gCol = qCol;
			//Goomba moves by flipping horizontally...
			if(qCount & 0x01)
				gCol = 15-qCol;

			data = getGimpColorVal(&spriteGOOMBA, 0, qRow, gCol);
		}
		else
		{
			data = getGimpColorVal(p_selectedSprite, 
							qCount%p_selectedSprite->numPalettes, qRow, qCol);
		}
		data = gimpPixelValToLColor(data);

		addSegfb(Q_SEGLENGTH + hfm_nextOutput(&colincrementer_hfm), data);
	}


	//Wow, is it really so smart as to recognize that rowNum is never >=
	// V_COUNT?! was 7684 when if(rowNum==0), then 7668 after this bug...
	//if(rowNum >= V_COUNT)
	if(rowNum >= V_COUNT-1)
	{
		qCount++;
	}


	//Wait, what?! We can't fit 1024 segments! But this sorta works
	// We're gtting syncing, a tiny bit of white-space on the right
	// which seems reasonable based on shotty math, 
	//                10     *  32   +  10   *    2       /   3 = 326
	// but 1024/3 is 341 (where's my math error?)
	//addSegfb(1024-Q_SEGLENGTH*Q_WIDTH-Q_WIDTH*Q_STRETCHTOP/Q_STRETCHDIV, _W);
	// But, nope, 341-... doesn't sync (vertical stretching again)
	// But, 342 does. Sweet. 1024/3=341.33333, actually.
	//addSegfb(342-Q_SEGLENGTH*Q_WIDTH-Q_WIDTH*Q_STRETCHTOP/Q_STRETCHDIV, _W);
	// Because it's not 10*2/3, it's 32*2/3=21.3...
	// which is exactly right
	// which then doesn't explain why it has whitespace and doesn't sync
	// without 342 above...
	// something to do with white being necessary?
	// Can't recall where 1024/3 was determined, maybe it varies
	// since the LVDS clock is 7 cycles...?
	//OK, so maybe we just need a white pixel at the end...
	// I mean, this works.
	addSegfb(1,_W);


	segTerminate();

 #elif (defined(SEG_LINE) && SEG_LINE)

	//syncing issues due to recursion overflowing the stack???
	// Apparently was
	segClear();
	newSeg(3,0x06, (6<<4) | 3);									//W
	newSeg((rowNum&0xff) | BLAH, 0x06, (4<<4) | 0);				//R
	newSeg(1,0x06, (6<<4) | 3);									//W
	newSeg((255-(rowNum&0xff)) | BLAH, 0x06, (4<<4) | 0 );	//R
	newSeg(3,0x06, (6<<4) | 3);									//W
	segTerminate();

 #elif (defined(SEG_SINE) && SEG_SINE)
	static uint16_t lastThetaOffset = 0;
	static uint8_t frameCount = 0;

	if(rowNum == 0)
	{
		lastThetaOffset+=16;
		frameCount++;
	}
	//pgm_read_sine(sineTable[theta])

	segClear();
	//addSegfb(3,0x06, (6<<4) | 3);
	addSegfb(3, _W);

	uint8_t sineVal = (uint8_t)((int8_t)128 + 
			sineRaw8(rowNum+lastThetaOffset));

	uint8_t nextSineVal = (uint8_t)((int8_t)128 +
			sineRaw8(rowNum+1+lastThetaOffset));

	uint8_t length; 

	//If the step-size is greater than one pixel, we'll see hashing
	// this should fix that.
	if(nextSineVal < sineVal)
	{
		length = (sineVal - nextSineVal);
		//This assures that the additional length of the white-segment
		// goes in the proper direction...
		sineVal-=length;//*4;
	}
	else if(nextSineVal > sineVal)
		length = (nextSineVal - sineVal);
		
	//But we don't want zero-length white segments (where the sine values
	// are horizontal)
	if(length == 0)
		length = 1;

	uint8_t rgbVal=0;
	uint8_t J=0;
	uint8_t j = frameCount;

#define WIDTH 5//3//4
	while(1)
	{
		J++;
		j++; // = J+frameCount;
		
		rgbVal = rgbGradient(j);

		if((uint16_t)J*(uint16_t)WIDTH>=sineVal)
			break;

		addSegfb(WIDTH, rgbVal);
	}

	J--;

	addSegfb(sineVal-(uint16_t)J*(uint16_t)WIDTH, rgbVal); 

	addSegfb(length, _W); 
	addSegfb(257-sineVal-length, rgbGradient(rowNum/8)); //rowNum/8);

	addSegfb(3, _W);

	//Draw a sideways character...
	{
		addSegfb(3, _R);

		uint8_t i;

		//Without + ' ' we get the same alphabet but shifted
		// starting with the lower-case letters which are shifted down a row
	   // and some garbage in the top row...
		// now I get that I'm reading weird memory locations, but this
		// doesn't seem right. I should be accessing outside the array 
		// altogether, no? 	
		char thisChar = (rowNum/8)%('~'-' '+1) + ' '; //pgm_read_byte(&(string[rowNum/8])); 
		//'A'; //string[rowNum/8];

		//Ironically, there are exactly the number of characters to be
		// displayed in 768 lines (plus one on the right, to center)
		for(i=8; i>0; i--)
		{
			if(getbit(rowNum%8, getCharRow(thisChar,i-1)))
				addSegfb(1, _Y); //0x06, (4<<4) | 3);
			else
				addSegfb(1, _R); //0x06, (4<<4) | 0);
		}

		addSegfb(3, _R); //0x06, (4<<4) | 0);

		addSegfb(1, _W); //0x04, (6<<4) | 3);

		//PGM_P charPtr = strchrnul_P(
		PGM_P tqbf = PSTR(" The Quick Brown Fox Jumped Over The Lazy Dog! \177"); 
		//, rowNum/8);

		strncpy_P(&thisChar, tqbf+(rowNum/8), 1);

		addSegfb(3, rgb(0,0,1));//_B); //0, (6<<4) | 0);

		if(rowNum/8 < strlen_P(tqbf))
		{
			for(i=8; i>0; i--)
			{
				if(getbit(rowNum%8, getCharRow(thisChar,i-1)))
					addSegfb(1, rgb(3,0,1));//_C); //0, (6<<4) | 3);
				else
					addSegfb(1, rgb(0,0,1));//_B); //0, (6<<4) | 0);
			}
		}
		else
			addSegfb(8, rgb(0,0,1));//_B); //0,(6<<4) | 0);

		addSegfb(3, rgb(0,0,1));//_B); //0,(6<<4)|0);

	}

	//newSeg(3,0x06,(4<<4) | 0);

	uint8_t i;
	for(i=0; i<64; i++)
		addSegfb(1, (i+rowNum/4)&0x3f);

	addSegfb(3, _W); //0x06, (6<<4) | 3);

	segTerminate();

 #else //NOT SEG_LINE NOR SEG_SINE NOR ANY OTHERS...
  #if(!defined(SEG_STRETCH))
	#define SEG_STRETCH 1
  #endif

	segClear();

	//Good for syncing to have white on both borders...
	newSeg(1, 0x06, (6<<4) | 3);
	
	uint16_t i;
	//i+1 because we don't want to overwrite the white border...
	for(i=0; i<RB_WIDTH; i++)
		rbpix_to_seg(rowBuffer[i], i+1, SEG_STRETCH);
	
	//white...
	newSeg(1, 0x06, (6<<4) | 3);
	segTerminate();
 #endif //SEG_LINE
#endif //ROW_SEG_BUFFER
}
#endif //LOAD_ROW

void init_timer0Hsync(void)
{
	//T_HD + T_DH + T_low + T_DE
	// For the original system, 19.865us = 695 dot-clocks
	// We'd need to take advantage of the 16-bit timer
	//  OCR1A may not be written properly (with a temporary high-byte)
	//  ... not sure.
	// TIMER0 DOES NOT HAVE CTC MODE in 16-bit mode

// This is a hack, for testing syncability with long row-calculations
// before actually drawing it... e.g. loading from program memory
// This value needs to be larger than the calculation time
// otherwise, it seems, syncing isn't stable
// BUT THIS IS A HACK, has nothing to do with actual calculation time
// And for slow LVDS or whatnot, this may be unnecessarily HUGE

#define DISPLAY_CYC_COUNT \
	(T_Hlow_CYC + T_HD_CYC + T_DE_CYC + T_DH_CYC)

//NOTE that with longer ROW_CALCULATION_CYCS, lines can be repeated...
// apparently an effect of the display, definitely not a code-thing
// in which case the vertical resolution decreases
// though, this could be used to advantage... 
// (e.g. more processing time outside the drawing interrupts?)
// NOTE that if the value is too small and calculations take longer in some
// lines than others (?), those lines may repeat, or syncing could be off
#if (LVDS_PRESCALER == 8)
 //#define ROW_CALCULATION_DELAY 1//20
 #define ROW_CALCULATION_CYCS (50000) //0 //(70000) //(100000)
#else
 //#define ROW_CALCULATION_DELAY 9//7//5//2//1//10
 #define ROW_CALCULATION_CYCS	(8*DISPLAY_CYC_COUNT)
#endif

#define TOTAL_CYC_COUNT \
	(ROW_CALCULATION_CYCS + DISPLAY_CYC_COUNT)
//	(ROW_CALCULATION_DELAY * (T_Hlow_CYC + T_HD_CYC + T_DE_CYC + T_DH_CYC))
//#define TOTAL_CYC_COUNT 2000
#if (TOTAL_CYC_COUNT > 255)
 //Adding +7 causes it to round up (hopefully)
 #define TIMER0_TCNTS ((TOTAL_CYC_COUNT+7)/8)
 #define TIMER0_CLKDIV	CLKDIV8
 #warning "TIMER0_TCNTS = (TOTAL_CYC_COUNT+7)/8: Timer0 CLKDIV8"
 #if (TIMER0_TCNTS > 255)
  #warning "TIMER0_TCNTS = (TOTAL_CYC_COUNT+63)/64: Timer0 CLKDIV64"
  #define TIMER0_TCNTS ((TOTAL_CYC_COUNT+63)/64)
  #define TIMER0_CLKDIV CLKDIV64
  #if (TIMER0_TCNTS > 255)
	 #warning "TIMER0_TCNTS = (TOTAL_CYC_COUNT+255)/256: Timer0 CLKDIV256"
	 #define TIMER0_TCNTS ((TOTAL_CYC_COUNT+255)/256)
	 #define TIMER0_CLKDIV CLKDIV256
	 #if (TIMER0_TCNTS > 255)
		#warning "TIMER0_TCNTS = (TOTAL_CYC_COUNT+1023)/1024: Timer0 CLKDIV1024"
		#define TIMER0_TCNTS ((TOTAL_CYC_COUNT+1023)/1024)
		#define TIMER0_CLKDIV	CLKDIV1024
	 #endif
  #endif
 #endif
 #if (TIMER0_TCNTS == 0)
  #error "TIMER0_OCRVAL == 0"
 #endif
#else
 #define TIMER0_TCNTS TOTAL_CYC_COUNT
 #define TIMER0_CLKDIV CLKDIV1
#endif

#if (defined(SLOW_EVERYTHING_TEST) && SLOW_EVERYTHING_TEST)
 //For testing, I want to see the pulses with the 'scope
 // Unless the timer is divisible by 7 the pulses will not align with the
 // timer overflow...
 //but shit... it's running at clkdiv8 so each TCNT is 64 bits!
 // but this shouldn't matter for visualizing, that's only ~10 pixel clocks
 // and we'll only start visualizing *after* the extra TCNTs...
 //Since the timer counts from 0 to (and including) OCR1A
 // Add one to this test...
 #if (((TIMER0_TCNTS + 1) % 7) == 0)
  #warning "(TIMER0_TCNTS + 1) % 7 == 0"
  #warning "   so... TIMER0_OCRVAL = TIMER0_TCNTS"
  #define TIMER0_OCRVAL (TIMER0_TCNTS)

 #elif (((TIMER0_TCNTS + 2) % 7) == 0)
  #warning "(TIMER0_TCNTS + 2) % 7 == 0"
  #warning "   so... TIMER0_OCRVAL = TIMER0_TCNTS + 1"
  #define TIMER0_OCRVAL (TIMER0_TCNTS + 1)

 #elif (((TIMER0_TCNTS + 3) % 7) == 0)
  #warning "(TIMER0_TCNTS + 3) % 7 == 0"
  #warning "   so... TIMER0_OCRVAL = TIMER0_TCNTS + 2"
  #define TIMER0_OCRVAL (TIMER0_TCNTS + 2)

 #elif (((TIMER0_TCNTS + 4) % 7) == 0)
  #warning "(TIMER0_TCNTS + 4) % 7 == 0"
  #warning "   so... TIMER0_OCRVAL = TIMER0_TCNTS + 3"
  #define TIMER0_OCRVAL (TIMER0_TCNTS + 3)

 #elif (((TIMER0_TCNTS + 5) % 7) == 0)
  #warning "(TIMER0_TCNTS + 5) % 7 == 0"
  #warning "   so... TIMER0_OCRVAL = TIMER0_TCNTS + 4"
  #define TIMER0_OCRVAL (TIMER0_TCNTS + 4)

 #elif (((TIMER0_TCNTS + 6) % 7) == 0)
  #warning "(TIMER0_TCNTS + 6) % 7 == 0"
  #warning "   so... TIMER0_OCRVAL = TIMER0_TCNTS + 5"
  #define TIMER0_OCRVAL (TIMER0_TCNTS + 5)

 #elif (((TIMER0_TCNTS) % 7) == 0)
  #warning "(TIMER0_TCNTS) % 7 == 0"
  #warning "   so... TIMER0_OCRVAL = TIMER0_TCNTS - 1"
  #define TIMER0_OCRVAL (TIMER0_TCNTS - 1)

 #else
  #error "WTF?"
 #endif
#else
 #define TIMER0_OCRVAL (TIMER0_TCNTS-1)
#endif

#if (TIMER0_OCRVAL > 255)
 #error "Despite all my efforts, TIMER0_OCRVAL is *still* > 255!"
#endif

   OCR0A = TIMER0_OCRVAL; //T_Hlow_CYC + T_HD_CYC + T_DE_CYC + T_DH_CYC;
	timer_setWGM(0, WGM_CLR_ON_COMPARE);
   timer_selectDivisor(0, TIMER0_CLKDIV); //CLKDIV1);
   timer_compareMatchIntEnable(0, OUT_CHANNELA);
}

//For a first go...
// B5 = OCR=5
// B4 = OCR=6+
// B3 = DT=0
// B2 = DT=1
// Possible combinations:
// B3, B2, B5, B4		OCR>6 (DT=0)
// B3, B2, B5			OCR=5, DT=0
// B3, B2				OCR=4, DT=0
//     B2, B5, B4		OCR=6, DT=1 (is OCR=6 possible?)
//     B2, B5			OCR=5, DT=1
//     B2				OCR=4, DT=1
//         B5, B4		OCR=6, DT=2 (is OCR=6 possible?)
//         B5			OCR=5, DT=2
// OCR=6 is full-on...

//            |  0   1    2    3    4    5    6    
//            |____ ____ ____ ____ ____ ____ ____
//   DE:      X B3 X B2 /    .    .    \ B5 X B4 X
//            |¯¯¯¯ ¯¯¯¯                ¯¯¯¯ ¯¯¯¯

// Active Bits     | Brightness   |
//                 | (0-63)       |    Configuration
// ----------------+--------------+-------------------------
// B5  B4  B3  B2  |     60		 |		OCR>6 (DT=0)  //DC
// B5      B3  B2  |     44 		 |		OCR=5, DT=0
// B5          B2  |     36 		 |		OCR=5, DT=1
// B5              |     32 		 |		OCR=5, DT=2  
//         B3  B2  |     12 		 |		OCR=4, DT=0  //Damn-near black...
//             B2  |     4  		 |		OCR=4, DT=1  //Even closer
// None = Black    |     0        |    OCR=4, DT=2
//
// The colors 60, 44, 36, and 32 are all discernable
// (though 36 and 32 mightn't be if not side-by-side)
// 12, 4, and 0 are damn-near identical and may not be worth the overhead
// There is a SIGNIFICANT jump between 32 and 12
//   32 is I'd say half-bright, maybe more
//   12 is damn-near black

#define fullBlue()	DEblue_init()

/*
static __inline__ \
void writeBlue(uint8_t settingVal) \
	  __attribute__((__always_inline__));
*/

#if FALSE
#define GREEN_PIN_MASK	0x03
#define GREEN_PORT		PORTA
#define GREEN_ON			0x01
#define GREEN_ON_STRING	"0x01"
#define GREEN_OFF			0x02
#define GREEN_OFF_STRING "0x02"
// in settingVal: bit 3 is unused by blue (4<=OCR<=6), so use it for green
#define GREEN_SETTING_BIT	3
#define GREEN_SETTING_BIT_STRING	"3"
// for setBlue:
#define GREEN_BLUEVAL_BIT	0

#define RED_PIN_MASK		0x0C
#define RED_PORT			PORTA
#define RED_ON				0x04
#define RED_ON_STRING	"0x04"
#define RED_OFF			0x08
#define RED_OFF_STRING	"0x08"
#define RED_SETTING_BIT	7
#define RED_SETTING_BIT_STRING "7"
#define RED_BLUEVAL_BIT	1

#define REDGREEN_PORT 	RED_PORT


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

	//Simply: if(getbit(greenBit, settingVal)) 	redGreen=GREEN_ON;
	//        else											redGreen=GREEN_OFF;
__asm__ __volatile__
		  //SBRC takes 1 cycle if not skipping, 2 (or 3) if skipping
	     ( "sbrc %1, " GREEN_SETTING_BIT_STRING "; \n\t"				//0,1
							//Skip the jump if !getbit(GREEN, settingVal)
			 "rjmp .+4; \n\t"  //jump if getbit(GREEN, settingVal)	//1-
			 "ldi  %0, " GREEN_OFF_STRING "; \n\t"							//0-
			 "rjmp .+4; \n\t"  // skip setting GREEN_ON					//0-
			 "nop; \n\t"
			 "ldi  %0, " GREEN_ON_STRING "; \n\t"							//1-
			: "=r" (redGreen)		//redGreen is assigned to %0
			: "r"  (settingVal)	//settingVal is assigned to %1
			);
	//Simply: if(getbit(redBit, settingVal))		redGreen|=RED_ON;
	//			 else											redGreen|=RED_OFF;
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
//	delay_Dots(15);

}
#endif //FALSE

static __inline__ \
void writeColor(uint8_t colorVal) \
	  __attribute__((__always_inline__));

#if(defined(ROW_BUFFER) && ROW_BUFFER)
//THIS IS JUST AN ESTIMATE
 #define WRITE_COLOR_CYCS	(13)
#elif(defined(FOUR_SHADES) && FOUR_SHADES)
 // Roughly...
 #define WRITE_COLOR_CYCS	(12*2+9+3)
#else
 // Roughly...
 #define WRITE_COLOR_CYCS	(9*3+3)
#endif

void writeColor(uint8_t colorVal)
{
//#warning "I'm absolutely certain this'll need to be revised, probably asm"
	//   Red: (+OC1D => RX0+)
	//    Off (0/63): OCR1D = 0
	//    35/63:      OCR1D = 3
	//    63/63:      OCR1D >= 6

/* No Shit: This compiles to a 16-bit test!
	switch((uint8_t)(colorVal & (uint8_t)0x03))
	{
		case (uint8_t)0:
			OCR1D = 0;
			break;
		case (uint8_t)1:
			OCR1D = 3;
			break;
		case (uint8_t)2:
		default:
			OCR1D = 6;
			break;
	}
*/

#if(defined(ROW_BUFFER) && ROW_BUFFER)
	// In this case, colorVal is actually settingVal...
	// Between LDI, these instructions, and OCR/DT register writes
	// this is 14 cycles... or 16 pixels...

	//										//ldi (colorVal) (2 cyc)
	//Red: (temp)
	uint8_t ocrd = colorVal >> 2;	//mov, shl, shl
	//Green:
	uint8_t dt = colorVal & 0x03; //andi
	//Blue:
	uint8_t ocra = ocrd >> 3;		//mov, shl, shl, shl
	//And red...
	ocrd &= 0x07;						//andi
											//out OCRD, out DT, out OCRA

#else //NOT ROW_BUFFER (FRAMEBUFFER)

//	uint8_t redVal; // = colorVal & 0x03;
	uint8_t ocrd;

/*
	if(redVal == 0x00)
		ocrd = 0;
	else if(redVal == 0x01)
		ocrd = 3;
	else //2, 3
		ocrd = 6;
*/
#if(defined(FOUR_SHADES) && FOUR_SHADES)
 // "nop; nop; nop;" compiles to just a single nop! 
 //"\n\t" or maybe the space is necessary
 #define FOUR_SHADES_NOPS "nop ; \n\t nop ; \n\t nop ; \n\t"
#else
 #define FOUR_SHADES_NOPS "\n\t"
#endif
	//Each branch is 9 cycles... (12 with FOUR_SHADES)
__asm__ __volatile__
	( "mov 	%0, %1 	; \n\t"	// ocrd (redVal) = colorVal			  //1
	  "andi	%0, 0x03	; \n\t"	// ocrd = ocrd & 0x03					  //1
	  "brne	red1tst_%=; \n\t"	// if(ocrd != 0x00) jump to red1test  //1`2
	  "ldi   %0, 0x00	; \n\t"	// (ocrd==0x00) add some delays		  //1 .
	  "nop            ; \n\t"  //                                    //1 .
	  "nop            ; \n\t"  //                                    //1 .
	  "nop				; \n\t"  //                                    //1 .
	  FOUR_SHADES_NOPS         //                                    //N .
	  "rjmp  end_%=	; \n\t"	//   jump to the end						  //2 .
	  									// (ocrd_reg = redVal_reg = 0)            .
	"red1tst_%=:"					//"%=" is a unique identifier for this asm.
										//  invocation, so the label won't be     .
										//  mistaken from another invocation      .
	  "cpi	%0, 0x01	; \n\t"	// if(ocrd-0x01 != 0)					  //  1
	  "brne	red23_%=	; \n\t"	//   jump to red=2,3						  //  1`2
	  FOUR_SHADES_NOPS         //                                    //  N .
	  "ldi	%0, 0x03	; \n\t"	// else ocrd = 0x03                   //  1 .
	  "rjmp	end_%=	; \n\t"	//      jump to the end               //  2 .
	"red23_%=:"                                                      //    .
#if (defined(FOUR_SHADES) && FOUR_SHADES)                           //   /.
	  "cpi	%0, 0x02 ; \n\t"	// if(ocrd-0x02 !=0)                  //( . 1
	  "brne  red3_%=	; \n\t"	//		jump to red=3                   //( . 1`2
	  "ldi   %0, 0x04 ; \n\t"	// else ocrd=4                        //( . 1 .
	  "rjmp  end_%=	; \n\t"	//      jump to the end               //( . 2 .
	"red3_%=:"                                                       //( .   /
#endif                                                              //(  \ /
     "ldi	%0, 0x06	; \n\t"	// ocrd = 0x06                        //    1
	  "nop				; \n\t"  // one delay...							  //    1
  "end_%=:"

	  : "=r" (ocrd)      //Output only "%0"
	  : "r"  (colorVal)  //colorVal is "%1"
	  //,  "d0"  (ocrd)     //ocrd is also used for andi, and is %2
	);


//	OCR1D = ocrd;


	//   Green: (/OC1B => RX1-)          (B1,0 Active, as well as G2,1)
	//    Off (6/63): DTL1 = 0
	//    38-39/63:      DTL1 = 1
	//    62-63/63:      DTL1 = 3
/*	switch(colorVal & 0x0C)
	{
		case 0x00:
			DT1 = 0;
			break;
		case 0x04:
			DT1 = 1;
			break;
		case 0x08:
		default:
			DT1 = 3;
			break;
	}
*/
//	uint8_t greenVal = colorVal & 0x0C;
	uint8_t dt;
/*	if(greenVal == 0x00)
		dt=0;
	else if(greenVal == 0x04)
		dt=1;
	else //0x06, 0x0C
		dt=3;
*/
   //Each branch is 9 cycles... (12 with FOUR_SHADES)
__asm__ __volatile__
   ( "mov   %0, %1   ; \n\t"  // dt (greenVal) = colorVal           //1
     "andi  %0, 0x0C ; \n\t"  // dt = dt & 0x0C                     //1
     "brne  grn4tst_%=; \n\t" // if(dt != 0x00) jump to grn4test    //1`2
     "ldi   %0, 0x00 ; \n\t"  // (dt==0x00) add some delays         //1 .
     "nop            ; \n\t"  //                                    //1 .
     "nop            ; \n\t"  //                                    //1 .
     "nop            ; \n\t"  //                                    //1 .
     FOUR_SHADES_NOPS         //                                    //N .
	  "rjmp  end_%=   ; \n\t"  //   jump to the end                  //2 .
   "grn4tst_%=:"              //"%=" is a unique identifier for this asm.
                              //  invocation, so the label won't be     .
                              //  mistaken from another invocation      .
     "cpi   %0, 0x04 ; \n\t"  // if(dt-0x04 != 0)                   //  1
     "brne  grn8C_%= ; \n\t"  //   jump to green=8,C                //  1`2
     "ldi   %0, 0x01 ; \n\t"  // else dt = 0x01                     //  1 .
     FOUR_SHADES_NOPS         //                                    //  N .
	  "rjmp  end_%=   ; \n\t"  //      jump to the end               //  2 .
   "grn8C_%=:"                                                      //    .
#if (defined(FOUR_SHADES) && FOUR_SHADES)                           //   /.
     "cpi   %0, 0x08 ; \n\t"  // if(dt-0x08 !=0)                    //( . 1
     "brne  grn3_%=  ; \n\t"  //    jump to green=3                 //( . 1`2
     "ldi   %0, 0x02 ; \n\t"  // else dt=2                          //( . 1 .
     "rjmp  end_%=   ; \n\t"  //      jump to the end               //( . 2 .
   "grn3_%=:"                                                       //( .   /
#endif                                                              //(  \ /
     "ldi   %0, 0x03 ; \n\t"  // dt = 0x03                          //    1
     "nop            ; \n\t"  // one delay...                       //    1
   "end_%=:"

     : "=r" (dt)      //Output only "%0"
     : "r"  (colorVal)  //colorVal is "%1"
     //,  "d0"  (ocrd)     //ocrd is also used for andi, and is %2
   );



//	DT1 = dt;
	//   Blue: (+OC1A => RX2+)               (B3,2 Active from here down)
	//    Off (15/63):  OCR1A=4
	//    47/63:        OCR1A=5
	//    63/63:        OCR1A=6
/*	switch(colorVal & 0x30)
	{
		case 0x00:
			OCR1A = 4;
			break;
		case 0x10:
			OCR1A = 5;
			break;
		case 0x20:
		default:
			OCR1A = 6;
			break;
	}
*/
//	uint8_t blueVal = colorVal & 0x30;
	uint8_t ocra;
/*	if(blueVal == 0x00)
		ocra=4;
	else if(blueVal == 0x10)
		ocra=5;
	else //0x20, 0x30
		ocra=6;
*/

   //Each branch is 9 cycles...
__asm__ __volatile__
   ( "mov   %0, %1   ; \n\t"  // ocra (blueVal) = colorVal          //1
     "andi  %0, 0x30 ; \n\t"  // ocra = ocra & 0x30                 //1
     "brne  blu1tst_%=; \n\t" // if(ocra != 0x00) jump to red1test  //1`2
     "ldi   %0, 0x04 ; \n\t"  // (ocra==0x00) add some delays       //1 .
     "nop            ; \n\t"  //                                    //1 .
     "nop            ; \n\t"  //                                    //1 .
     "nop            ; \n\t"  //                                    //1 .
     "rjmp  end_%=   ; \n\t"  //   jump to the end                  //2 .
                              // (ocra_reg = blueVal_reg = 0)            .
   "blu1tst_%=:"              //"%=" is a unique identifier for this asm.
                              //  invocation, so the label won't be     .
                              //  mistaken from another invocation      .
     "cpi   %0, 0x10 ; \n\t"  // if(ocra-0x10 != 0)                 //  1
     "brne  blu23_%= ; \n\t"  //   jump to red=2,3                  //  1`2
     "ldi   %0, 0x05 ; \n\t"  // else ocra = 0x05                   //  1 .
     "rjmp  end_%=   ; \n\t"  //      jump to the end               //  2 .
   "blu23_%=:"                                                      //    .
     "ldi   %0, 0x06 ; \n\t"  // ocra = 0x06                        //    1
     "nop            ; \n\t"  // one delay...                       //    1
   "end_%=:"

     : "=r" (ocra)      //Output only "%0"
     : "r"  (colorVal)  //colorVal is "%1"
     //,  "d0"  (ocra)     //ocra is also used for andi, and is %2
   );

#endif //SETTING vs. FRAMEBUFFER

	DT1 = dt;
	OCR1D = ocrd;
	OCR1A=ocra;
}

#if(defined(IMAGE_BUFFER) && IMAGE_BUFFER)
void setColor(uint8_t red, uint8_t green, uint8_t blue, 
					uint8_t row, uint8_t col)
{
	red = (red > 3) ? 3 : red;
	green = (green > 3) ? 3 : green;
	blue = (blue > 3) ? 3 : blue;

	frameBuffer[row][col] = red | ((green)<<2) | ((blue)<<4);
}
#endif

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
// #define NUM_BLUES 5		//Currently Unused... (not including black)
#else
// #define NUM_BLUES 6		//Currently Unused... (not including black)

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
	else	//Black
	{
		//DT1=(2<<4);
		//OCR1A = 4;
		settingBuffer[r][c] = (2<<4) | 4 | redGreen;
	}
#endif
}
*/

/*
static __inline__ \
void drawPix(uint8_t rowNum) \
	  __attribute__((__always_inline__));
*/
#if (!defined(ROW_SEG_BUFFER) || !ROW_SEG_BUFFER)
void drawPix(uint8_t rowNum)
{
	//uint8_t *setting = &(settingBuffer[rowNum][0]);
#if(defined(ROW_BUFFER) && ROW_BUFFER)
	uint8_t *color = &(rowBuffer[0]);
#else
	uint8_t *color = &(frameBuffer[rowNum][0]);
#endif
	/*
      DEonly_fromNada();
      //Enable complementary-output for Green (on /OC1B, where CLK is OC1B)
      TCCR1A = ( (0<<COM1A1) | (1<<COM1A0)
               | (0<<COM1B1) | (1<<COM1B0)
               | (1<<PWM1A) | (1<<PWM1B) );
	*/
		//The Greenish-bar on the left is due to the time it takes to execute
		// the first writeColor (since its value is only written at the END)
		// Thus the greenish-bar is about one write-color wide...

	//Judging by some weird experiences re v21/22,
	// it's not entirely likely this will be predictable
	// it may try to recalculate the Z register between writeBlues...
	// hopefully not, for now. I should probably assemblify this
		writeColor(*(color+0));

		//Moving this here not only removes (most of) the green bar
		// but also seems to make the pixel edges significantly sharper
		// (v29 has ~1/8in of noise, v30 has ~1pixel noise at the right edge)
		TCCR1A = ( (0<<COM1A1) | (1<<COM1A0)
		         | (0<<COM1B1) | (1<<COM1B0)
		         | (1<<PWM1A) | (1<<PWM1B) );

		writeColor(*(color+1));	 
		writeColor(*(color+2));	 
		writeColor(*(color+3)); 
		writeColor(*(color+4));	 
		writeColor(*(color+5));					 
		writeColor(*(color+6));  
		writeColor(*(color+7));                         
		writeColor(*(color+8));                         
		writeColor(*(color+9));                         
		writeColor(*(color+10));                         
		writeColor(*(color+11));                         
		writeColor(*(color+12));                         
		writeColor(*(color+13));                         
		writeColor(*(color+14));                         
		writeColor(*(color+15));   
#define COLORS_WRITTEN	16		
#if ( (defined(COLOR_BAR_SCROLL) && COLOR_BAR_SCROLL) \
	|| (defined(ROW_BUFFER) && (ROW_BUFFER)) )
		writeColor(*(color+16));
		writeColor(*(color+17));
		writeColor(*(color+18));
		writeColor(*(color+19));
writeColor(*(color+20));
writeColor(*(color+21));
writeColor(*(color+22));
writeColor(*(color+23));
writeColor(*(color+24));
writeColor(*(color+25));
writeColor(*(color+26));
writeColor(*(color+27));
#define COLORS_WRITTEN	28
#if (defined(ROW_BUFFER) && (ROW_BUFFER))
writeColor(*(color+28));
writeColor(*(color+29));
writeColor(*(color+30));
writeColor(*(color+31));
//Some sort of syncing problem after 32... (?)

writeColor(*(color+32));
writeColor(*(color+33));
writeColor(*(color+34));
writeColor(*(color+35));
writeColor(*(color+36));
writeColor(*(color+37));
writeColor(*(color+38));
writeColor(*(color+39));
writeColor(*(color+40));
writeColor(*(color+41));
writeColor(*(color+42));
writeColor(*(color+43));
writeColor(*(color+44));
writeColor(*(color+45));
writeColor(*(color+46));
writeColor(*(color+47));
writeColor(*(color+48));
writeColor(*(color+49));
writeColor(*(color+50));
writeColor(*(color+51));
writeColor(*(color+52));
writeColor(*(color+53));
writeColor(*(color+54));
writeColor(*(color+55));
writeColor(*(color+56));
writeColor(*(color+57));
writeColor(*(color+58));
writeColor(*(color+59));
writeColor(*(color+60));
writeColor(*(color+61));
writeColor(*(color+62));
writeColor(*(color+63));
// WriteColor writes the pixel *after* the calculations...
// thus the pixel appears basically after writeColor completes
// These nops assure the 64th pixel is fully-displayed before exitting
// (Not sure how the other following instructions apply to this)
// The number of nops was found experimentally...
asm("nop");
asm("nop");
asm("nop");
asm("nop");
asm("nop");
asm("nop");
asm("nop");
asm("nop");
//count "0" below, as well..
#define COLORS_WRITTEN 65
#endif //ROW_BUFFER
#else
//	writeColor(0);
#endif //COLOR_BARS || ROW_BUFFER

/*		reg[17] = colorBuffer[rowNum][17];                         
		writeColor(reg[17]);                         
		...
		reg[20] = colorBuffer[rowNum][20];                         
		writeColor(reg[20]);  
		
		//REPEATING to fill screen... (delayDots = 342 worked prior to this)
		reg[0] = colorBuffer[rowNum][0];
		writeColor(reg[0]);
		...
		reg[10] = colorBuffer[rowNum][10];
		writeColor(reg[10]);
*/
		//Display the rest as black...
		writeColor(0);
		//writeColor(0xff);
	  	//delay_Dots(500);//142); //Don't want to disable DE too early...	
		//900 leaves a buffer for various calculations while also showing
		// a blue bar at the right-side...
		//LTN Last Used 900
		// -68 is from 900's intent, IIRC
		//  seems arbitrary, but its value (especially if too small)
		// causes blank lines... (?!)
		// -60 makes more sense for a delay (was the original post-900)
		//  (outside DOTS_TO_CYC because it's for cycles used for calcs...
		// -68 worked for LVDS_PRE=2
		// -60 for 1
		// 4 doesn't work... blue-lines

#else		//ROW_SEG_BUFFER
void drawPix(uint8_t rowNum)
{
	TCCR1A = ( (0<<COM1A1) | (1<<COM1A0)
	         | (0<<COM1B1) | (1<<COM1B0)
	         | (1<<PWM1A) | (1<<PWM1B) );

		drawSegs();

#if (defined(GB_COMBINED) && GB_COMBINED)
 #define WRITE_COLOR_CYCS 20
#else
 #define WRITE_COLOR_CYCS 16
#endif
#define COLORS_WRITTEN	64

#endif



#define ROW_COMPLETION_DELAY \
		(DOTS_TO_CYC(DE_ACTIVE_DOTS) -60  \
		 - WRITE_COLOR_CYCS * COLORS_WRITTEN)
/*

#error "should add SEG_STRETCH here..."
#if (ROW_COMPLETION_DELAY > 0)
		delay_cyc(DOTS_TO_CYC(DE_ACTIVE_DOTS) -60 // - 68)// - 60
				- WRITE_COLOR_CYCS*COLORS_WRITTEN);
#else
#warning "ROW_COMPLETION_DELAY <= 0"
#endif
*/		//DE->Nada transition expects fullBlue...
		//Also helps to show the edge of the DE timing...

		//!!! Not sure what the state is at this point...
		// could be any DE+Blue level, or could be NADA...
		// Nada: DT1=3, still leaves one bit for clocking, might be OK
         
		//Among the things that don't make sense...
		// This appears to go into affect BEFORE delay_cyc (?)
		// as, without a pull-up resistor on the /OC1B output, 
		// green seems to be floating between the last pixel and the
		// delay_cyc (!)
		//Disable complementary-output for Green 
		//  (on /OC1B, where CLK is OC1B)
		// Since Nada, V, and H DT's might be bad for clocking.
		TCCR1A = ( (0<<COM1A1) | (1<<COM1A0)
         | (1<<COM1B1) | (0<<COM1B0)
         | (1<<PWM1A) | (1<<PWM1B) );

		fullBlue();
		Nada_fromDEonly();
}

#if 0   //in LCDStuff...
// SCOPING has been removed in LVDS, revisit LCDdirect50
//#else //NOT SCOPING
void loadData(uint16_t rowNum, uint8_t dataEnable)
{
	//H Active pulse...
   if(vSync)
   {
		//Vsync_fromNada() is called at the end of the last interupt
      VplusH_fromVsync();
      Hlow_Delay();
      Vsync_fromVplusH();
      HD_Delay();
   }
   else
   {
      Hsync_fromNada();
      Hlow_Delay();
      Nada_fromHsync();
      HD_Delay();
   }

   if(dataEnable)
   {

/* WTF, how did this work?! Am I not using drawPix?!
	Yes, but drawPix calls drawSegs, which doesn't have an argument
	because loadRow takes the row argument elsewhere, drawSegs just
	draws 'em
		//#warning "see 'shouldBe' here..."
		//Should be:
		rowNum = rowNum*FB_HEIGHT/V_COUNT;
		//rowNum = rowNum / (768/FB_HEIGHT);
*/

		//DEonly_fromNada();
      drawPix(rowNum);
		//Nada_init();
//#warning "THIS ISN'T SAFE"
//		Nada_fromDEonly();
//#endif

	}
}
#endif
/* These are old tests from above, under if(dataEnable)
	They should all be implemented in lcdStuff.c, but have not been
	retested since...
#if(defined(BLUE_DIAG_BAR) && BLUE_DIAG_BAR)
		uint16_t blueCyc = DOTS_TO_CYC(rowNum);
		uint16_t notBlueCyc = DOTS_TO_CYC(DE_ACTIVE_DOTS)-blueCyc;
		
		DEonly_fromNada();
		delay_cyc(notBlueCyc);
		DEblue_fromDEonly();
		delay_cyc(blueCyc);

		Nada_fromDEblue();
#elif(defined(BLUE_VERT_BAR) && BLUE_VERT_BAR)
		DEonly_fromNada();
		delay_cyc(DOTS_TO_CYC(DE_ACTIVE_DOTS)/3);
		DEblue_fromDEonly();
		delay_cyc(DOTS_TO_CYC(DE_ACTIVE_DOTS)*2/3);
		Nada_fromDEblue();
#elif(defined(DE_BLUE) && DE_BLUE)
		DEblue_fromNada();
		delay_cyc(DOTS_TO_CYC(DE_ACTIVE_DOTS));
		Nada_fromDEblue();
#else  //NOT BLUE_DIAG_BAR, BLUE_VERT_BAR, NOR DE_BLUE
 #if ( defined(COLOR_BAR_SCROLL) && COLOR_BAR_SCROLL)
		if(rowNum <= 256)
			rowNum=0;
		else
			rowNum=1;
 #else //NOT COLOR_BAR_SCROLL... drawPix...
*/

//Moved Above
//Called as: pgm_readImageByte(pgm_image1, row, col)
//#define pgm_readImageByte(image, row, col)	\
//	pgm_read_byte((uint8_t *)(&((image)[(row)*FB_WIDTH+(col)])))

#if( !defined(COLOR_BAR_SCROLL) || !COLOR_BAR_SCROLL)
#if ( (FB_WIDTH != 16) || (FB_HEIGHT != 16) )
 #error "image1 dimensions don't match FB_HEIGHT/WIDTH"
 #error "It's likely I just need to create a specific DRAWIMAGE definition and tests"
#endif

#define RED_IMAGEVAL_BIT 0
#define GREEN_IMAGEVAL_BIT 1
#define BLUE_IMAGEVAL_BIT 2

/*
#define _R	0x03
#define _O	0x07
#define _G 0x0C
#define _B 0x30
#define _V 0x13
#define _Y	0x0F
#define _C 0x3C
#define _M	0x33
#define _K	0x00
#define _W 0x3F
#define _r 0x01
#define _g 0x04
#define _b 0x10
#define _c 0x14
#define _m 0x11
#define _y 0x05
#define _k 0x15
#define Tr 0xC0	//Transparet
*/
#if (TRUE || (!defined(LIFE) || !LIFE))
uint8_t pgm_image1[256] PROGMEM =
{
	Tr,Tr,Tr,Tr,Tr,_K,_K,_K,_K,_K,_K,Tr,Tr,Tr,Tr,Tr,
	Tr,Tr,Tr,_K,_K,_Y,_Y,_Y,_Y,_Y,_Y,_K,_K,Tr,Tr,Tr,
	Tr,Tr,_K,_K,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_K,_K,Tr,Tr,
	Tr,_K,_K,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_K,_K,Tr,
	Tr,_K,_Y,_Y,_Y,_K,_Y,_Y,_Y,_Y,_K,_Y,_Y,_Y,_K,Tr,
	_K,_Y,_Y,_Y,_K,_K,_K,_Y,_Y,_K,_K,_K,_Y,_Y,_Y,_K,
	_K,_Y,_Y,_Y,_Y,_K,_Y,_Y,_Y,_Y,_K,_Y,_Y,_Y,_Y,_K,
	_K,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_K,
	_K,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_K,
	_K,_Y,_Y,_K,_K,_Y,_Y,_Y,_Y,_Y,_Y,_K,_K,_Y,_Y,_K,
	_K,_Y,_Y,_Y,_K,_K,_Y,_Y,_Y,_Y,_K,_K,_Y,_Y,_Y,_K,
	Tr,_K,_Y,_Y,_Y,_K,_K,_K,_K,_K,_K,_Y,_Y,_Y,_K,Tr,
	Tr,_K,_K,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_K,_K,Tr,
	Tr,Tr,_K,_K,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_K,_K,Tr,Tr,
	Tr,Tr,Tr,_K,_K,_Y,_Y,_Y,_Y,_Y,_Y,_K,_K,Tr,Tr,Tr,
	Tr,Tr,Tr,Tr,Tr,_K,_K,_K,_K,_K,_K,Tr,Tr,Tr,Tr,Tr
};
#else
/*uint8_t pgm_image1[256] PROGMEM =
{  
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};
*/
/*
{  
   _K,_K,_K,_K,_K,_K,_K,_K,_K,_K,_K,_K,_K,_K,_K,_K,
   _K,_K,_K,_K,_K,_K,_K,_K,_K,_K,_K,_K,_K,_K,_K,_K,
   _K,_K,_K,_K,_K,_K,_K,_K,_K,_K,_K,_K,_K,_K,_K,_K,
   _K,_K,_K,_K,_K,_R,_R,_R,_K,_K,_K,_K,_K,_K,_K,_K,
   _K,_K,_K,_K,_K,_K,_K,_K,_K,_K,_K,_K,_K,_K,_K,_K,
   _K,_K,_K,_K,_K,_K,_K,_K,_K,_K,_K,_K,_K,_K,_K,_K,
   _K,_K,_K,_K,_K,_K,_K,_K,_K,_K,_K,_R,_R,_K,_K,_K,
   _K,_K,_K,_K,_K,_K,_K,_K,_K,_K,_K,_R,_R,_K,_K,_K,
   _K,_K,_K,_K,_R,_R,_R,_K,_K,_K,_K,_K,_K,_K,_K,_K,
   _K,_K,_K,_K,_K,_R,_R,_R,_K,_K,_K,_K,_K,_K,_K,_K,
   _K,_K,_K,_K,_K,_K,_K,_K,_K,_K,_K,_R,_R,_R,_K,_K,
   _K,_K,_K,_K,_K,_K,_K,_K,_K,_K,_K,_R,_K,_K,_K,_K,
   _K,_K,_K,_K,_K,_K,_K,_K,_K,_K,_K,_K,_R,_K,_K,_K,
   _K,_K,_K,_K,_K,_K,_K,_K,_K,_K,_K,_K,_K,_K,_K,_K,
   _K,_K,_K,_K,_K,_K,_K,_K,_K,_K,_K,_K,_K,_K,_K,_K,
   _K,_K,_K,_K,_K,_K,_K,_K,_K,_K,_K,_K,_K,_K,_K,_K
};
*/



#endif


uint8_t pgm_image2[256] PROGMEM =
{
	Tr,Tr,Tr,Tr,Tr,_K,_K,_K,_K,_K,_K,Tr,Tr,Tr,Tr,Tr,
	Tr,Tr,Tr,_K,_K,_Y,_Y,_Y,_Y,_Y,_Y,_K,_K,Tr,Tr,Tr,
	Tr,Tr,_K,_K,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_K,_K,Tr,Tr,
	Tr,_K,_K,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_K,_K,Tr,
	Tr,_K,_Y,_Y,_Y,_K,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_K,Tr,
	_K,_Y,_Y,_Y,_K,_K,_K,_Y,_Y,_K,_Y,_Y,_Y,_Y,_Y,_K,
	_K,_Y,_Y,_Y,_Y,_K,_Y,_Y,_Y,_Y,_K,_K,_Y,_Y,_Y,_K,
	_K,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_K,
	_K,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_K,
	_K,_Y,_Y,_K,_K,_Y,_Y,_Y,_Y,_Y,_Y,_K,_K,_Y,_Y,_K,
	_K,_Y,_Y,_Y,_K,_K,_Y,_Y,_Y,_Y,_K,_K,_Y,_Y,_Y,_K,
	Tr,_K,_Y,_Y,_Y,_K,_K,_K,_K,_K,_K,_Y,_Y,_Y,_K,Tr,
	Tr,_K,_K,_Y,_Y,_Y,_K,_R,_K,_R,_K,_Y,_Y,_K,_K,Tr,
	Tr,Tr,_K,_K,_Y,_Y,_K,_R,_R,_R,_K,_Y,_K,_K,Tr,Tr,
	Tr,Tr,Tr,_K,_K,_Y,_Y,_K,_K,_K,_Y,_K,_K,Tr,Tr,Tr,
	Tr,Tr,Tr,Tr,Tr,_K,_K,_K,_K,_K,_K,Tr,Tr,Tr,Tr,Tr
};
#endif //COLOR_BAR_SCROLL

/*
#if (IMAGE_WIDTH == 21)
uint8_t mainData[2][768>>5][IMAGE_WIDTH] =
{
	{
	{6,5,4,3,2,1,6,5,4,3,2,1,6,5,4,3,2,1,6,5,4},
	{5,4,3,2,1,6,0,0,0,0,0,0,0,0,0,2,1,6,5,4,3},
	{4,3,2,1,0,0,0,3,3,3,3,3,3,3,0,0,0,5,4,3,4},
	{3,2,1,0,0,3,3,3,3,3,3,3,3,3,3,3,0,0,3,2,1},
	{2,1,0,0,3,3,3,3,3,3,3,3,3,3,3,3,3,0,0,1,6},
	{1,0,0,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,0,0,5},
	{6,0,3,3,3,3,3,0,3,3,3,3,0,3,3,3,3,3,3,0,4},
	{0,0,3,3,3,3,0,0,0,3,3,0,0,0,3,3,3,3,3,0,0},
	{0,3,3,3,3,3,0,0,0,3,3,0,0,0,3,3,3,3,3,3,0},
	{0,3,3,3,3,3,3,0,3,3,3,3,0,3,3,3,3,3,3,3,0},
	{0,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,0},
	{0,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,0},
	{0,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,0},
	{0,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,0},
	{0,3,3,3,3,0,0,3,3,3,3,3,3,3,0,0,3,3,3,3,0},
	{0,0,3,3,3,3,0,0,3,3,3,3,3,0,0,3,3,3,3,0,0},
	{6,0,3,3,3,3,3,0,0,0,0,0,0,0,3,3,3,3,3,0,4},
	{5,0,0,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,0,0,3},
	{4,3,0,0,3,3,3,3,3,3,3,3,3,3,3,3,3,0,0,3,2},
	{3,2,6,0,0,3,3,3,3,3,3,3,3,3,3,3,0,0,3,2,1},
	{2,1,5,5,0,0,0,3,3,3,3,3,3,3,0,0,0,3,2,1,6},
	{1,6,3,4,3,2,0,0,0,0,0,0,0,0,0,4,3,2,1,6,5},
	{6,5,4,3,2,1,6,5,4,3,2,1,6,5,4,3,2,1,6,5,4},
	{5,4,3,2,1,6,5,4,3,2,1,6,5,4,3,2,1,6,5,4,3}
	},
	{	
	{5,4,3,2,1,6,5,4,3,2,1,6,5,4,3,2,1,6,5,4,3},
	{4,3,2,1,6,5,0,0,0,0,0,0,0,0,0,1,6,5,4,3,2},
	{3,2,1,6,0,0,0,3,3,3,3,3,3,3,0,0,0,4,3,2,1},
	{2,1,6,0,0,3,3,3,3,3,3,3,3,3,3,3,0,0,2,1,6},
	{1,6,0,0,3,3,3,3,3,3,3,3,3,3,3,3,3,0,0,6,5},
	{6,0,0,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,0,0,4},
	{5,0,3,3,3,3,3,0,3,3,3,3,3,3,3,3,3,3,3,0,3},
	{0,0,3,3,3,3,0,0,0,3,3,0,3,3,3,3,3,3,3,0,0},
	{0,3,3,3,3,3,0,0,0,3,3,3,0,0,0,3,3,3,3,3,0},
	{0,3,3,3,3,3,3,0,3,3,3,3,3,3,3,3,3,3,3,3,0},
	{0,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,0},
	{0,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,0},
	{0,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,0},
	{0,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,0},
	{0,3,3,3,3,0,0,3,3,3,3,3,3,3,0,0,3,3,3,3,0},
	{0,0,3,3,3,3,0,0,3,3,3,3,3,0,0,3,3,3,3,0,0},
	{5,0,3,3,3,3,3,0,0,0,0,0,0,0,3,3,3,3,3,0,3},
	{4,0,0,3,3,3,3,3,3,0,1,0,1,0,3,3,3,3,0,0,2},
	{3,2,0,0,3,3,3,3,3,0,1,1,1,0,3,3,3,0,0,2,1},
	{2,1,6,0,0,3,3,3,3,3,0,0,0,3,3,3,0,0,2,1,6},
	{1,6,5,4,0,0,0,3,3,3,3,3,3,3,0,0,0,2,1,6,5},
	{6,5,4,3,2,1,0,0,0,0,0,0,0,0,0,3,2,1,6,5,4},
	{5,4,3,2,1,6,5,4,3,2,1,6,5,4,3,2,1,6,5,4,3},
   {4,3,2,1,6,5,4,3,2,1,6,5,4,3,2,1,6,5,4,3,2}
	}
};
#else
	#error
#endif
*/


int main(void)
{

#if(defined(SEG_RACER) && SEG_RACER)
	adc_takeInput(6);
	adc_init();
	adc_select(6);
#endif

	tetInit(7);

	init_timer0Hsync();

	//This starts pretty late... watch out for WDT
	init_heartBeat();

	setHeartRate(0);

	lvds_timerInit();

	//For synchronizing timer1 settings (to avoid glitches)
	// count the number of CPU cycles...
	// depending on how many cycles it takes to start this and whatnot, 
	// there may be an offset. But the jist is we know every 7 cycles
	// the timer1 reset is synchronized with the CPU instructions...
//	OCR0A = 6;
//	timer_setWGM(0, WGM_CLR_ON_COMPARE);
//	timer_selectDivisor(0, CLKDIV1);

/*
	segPosition=0;
	newSeg(32, 0x06, (6<<4) | 3);
	newSeg(16, 0x00, (4<<4) | 0);
	newSeg(8, 0x06, (4<<4) | 0);
	newSeg(32, 0x00, (4<<4) | 3);
	newSeg(32, 0x06, (6<<4) | 3);
	segTerminate();
*/

/* I Think these were LONG AGO when Green and Red weren't implemented
	//For Green...
	// Possibly later to be reimplemented on PWM, but deadTime might inhibit
	setoutPORT(PA0, PORTA);
	setoutPORT(PA1, PORTA);
	//Red:
	setoutPORT(PA2, PORTA);
	setoutPORT(PA3, PORTA);
*/


	// MUCH Of this is outside the screen...

#if (defined(COLOR_BAR_SCROLL) && COLOR_BAR_SCROLL)
/*	uint8_t frameBuffer[FB_HEIGHT][FB_WIDTH] = 
	{ {
		(0<<2), 
			(0<<2) | (1<<RED_BLUEVAL_BIT), (0<<2) | (1<<GREEN_BLUEVAL_BIT),
	   	(1<<RED_BLUEVAL_BIT) | (0<<2) | (1<<GREEN_BLUEVAL_BIT),
		(4<<2), 
			(4<<2) | (1<<RED_BLUEVAL_BIT), (4<<2) | (1<<GREEN_BLUEVAL_BIT),
			(1<<RED_BLUEVAL_BIT) | (4<<2) | (1<<GREEN_BLUEVAL_BIT),
	   (12<<2), 
			(12<<2) | (1<<RED_BLUEVAL_BIT), (12<<2) | (1<<GREEN_BLUEVAL_BIT),
	   	(1<<RED_BLUEVAL_BIT) | (12<<2) | (1<<GREEN_BLUEVAL_BIT),
		(32<<2), 
			(32<<2) | (1<<RED_BLUEVAL_BIT), (32<<2) | (1<<GREEN_BLUEVAL_BIT),
	   	(1<<RED_BLUEVAL_BIT) | (32<<2) | (1<<GREEN_BLUEVAL_BIT),
		(36<<2), 
			(36<<2) | (1<<RED_BLUEVAL_BIT), (36<<2) | (1<<GREEN_BLUEVAL_BIT),
	   	(1<<RED_BLUEVAL_BIT) | (36<<2) | (1<<GREEN_BLUEVAL_BIT),
		(44<<2), 
			(44<<2) | (1<<RED_BLUEVAL_BIT), (44<<2) | (1<<GREEN_BLUEVAL_BIT),
	   	(1<<RED_BLUEVAL_BIT) | (44<<2) | (1<<GREEN_BLUEVAL_BIT),
		(60<<2),
	  		(60<<2) | (1<<RED_BLUEVAL_BIT), (60<<2) | (1<<GREEN_BLUEVAL_BIT),
			(1<<RED_BLUEVAL_BIT) | (60<<2) | (1<<GREEN_BLUEVAL_BIT)
	  },
	  {
		(0<<2) | (1<<RED_BLUEVAL_BIT), (4<<2) | (1<<RED_BLUEVAL_BIT),
	  	(12<<2) | (1<<RED_BLUEVAL_BIT), (32<<2) | (1<<RED_BLUEVAL_BIT),
	  	(36<<2) | (1<<RED_BLUEVAL_BIT), (44<<2) | (1<<RED_BLUEVAL_BIT),
		(60<<2) | (1<<RED_BLUEVAL_BIT),
		
		(0<<2) | (1<<GREEN_BLUEVAL_BIT), (4<<2) | (1<<GREEN_BLUEVAL_BIT), 
		(12<<2) | (1<<GREEN_BLUEVAL_BIT), (32<<2) | (1<<GREEN_BLUEVAL_BIT), 
		(36<<2)| (1<<GREEN_BLUEVAL_BIT), (44<<2) | (1<<GREEN_BLUEVAL_BIT),
		(60<<2) | (1<<GREEN_BLUEVAL_BIT), 
		
		(0<<2), (4<<2), (12<<2),(32<<2), (36<<2), (44<<2), (60<<2),

		(0<<2) | (1<<RED_BLUEVAL_BIT) | (1<<GREEN_BLUEVAL_BIT),
	  	(4<<2) | (1<<RED_BLUEVAL_BIT) | (1<<GREEN_BLUEVAL_BIT),
		(12<<2) | (1<<RED_BLUEVAL_BIT) | (1<<GREEN_BLUEVAL_BIT),
	  	(32<<2) | (1<<RED_BLUEVAL_BIT) | (1<<GREEN_BLUEVAL_BIT),
		(36<<2) | (1<<RED_BLUEVAL_BIT) | (1<<GREEN_BLUEVAL_BIT),
	  	(44<<2) | (1<<RED_BLUEVAL_BIT) | (1<<GREEN_BLUEVAL_BIT),
		(60<<2) | (1<<RED_BLUEVAL_BIT) | (1<<GREEN_BLUEVAL_BIT)

  	  }
	};
*/


	//Init:
	uint8_t i;
	for(i=0; i<FB_WIDTH; i++)
	{
		uint8_t j=FB_WIDTH-1-i;
		setColor((i&0x03), (i&0x0C)>>2, (i&0x30)>>4, 0, i);
		setColor((i&0x30)>>4, (i&0x0C)>>2, (i&0x03), 1, i);
//		setBlue(frameBuffer[0][i], 0, i);
//		setBlue(frameBuffer[1][i], 1, i);
	}
#else	//NOT COLOR_BAR_SCROLL
	//uint8_t frameBuffer[16][16];

	uint8_t row,col;
/*	for(r=0; r<FB_HEIGHT; r++)
	{
		for(c=0; c<FB_WIDTH; c++)
		{
			uint8_t imagePixel=pgm_readImageByte(pgm_image1, r, c);

			setColor(
				(getbit(RED_IMAGEVAL_BIT, imagePixel) ? 0x03 : 0),
				(getbit(GREEN_IMAGEVAL_BIT, imagePixel) ? 0x0C : 0),
				(getbit(BLUE_IMAGEVAL_BIT, imagePixel) ? 0x30 : 0),
				r, c);
//			setBlue(
//					(
//	(getbit(BLUE_IMAGEVAL_BIT, imagePixel) ? (60<<2) : 0) |
//	(getbit(GREEN_IMAGEVAL_BIT, imagePixel) ? (1<<GREEN_BLUEVAL_BIT) : 0) |
//	(getbit(RED_IMAGEVAL_BIT, imagePixel) ? (1<<RED_BLUEVAL_BIT) : 0)
//					), r, c);
		}
	}
*/
 #if (defined(IMAGE_CHANGE) && IMAGE_CHANGE)
	uint32_t counter=0;
	uint8_t imageNum = 0;
	uint8_t colorShift = 0;
	while(1)
	{
 #if (defined(FRAME_SYNC) && FRAME_SYNC)
		if(frameCount==0 && !getbit(OCIE0A, T0_TIMSK))
 #else
		if(counter==0)
 #endif
		{
 #if (defined(IMAGE_BUFFER) && IMAGE_BUFFER)
			uint8_t *pimage;
 #endif

	#if 0
			tetUpdate();

			if(imageNum == 0)
			{
				imageNum = 1;
				pimage = pgm_image1;
				tetColorScheme = 1;
			}
			else
			{
				hexColor++;
				hexColor&=0x3f;
				imageNum = 0;
				pimage = pgm_image2;
				tetColorScheme = 0;
			}
	#endif //0

 #if (defined(IMAGE_BUFFER) && IMAGE_BUFFER)
			for(row=0; row<FB_HEIGHT; row++)
	      for(col=0; col<FB_WIDTH; col++)
	      {
	         uint8_t imagePixel=pgm_readImageByte(pimage, row, col);


				if(imagePixel == Tr)
					frameBuffer[row][col] = colorShift+row+col;
				else
					frameBuffer[row][col] = imagePixel;	
			/*
	         setColor(
		         (getbit(RED_IMAGEVAL_BIT, imagePixel) ? 0x03 : 0),
	            (getbit(GREEN_IMAGEVAL_BIT, imagePixel) ? 0x0C : 0),
	            (getbit(BLUE_IMAGEVAL_BIT, imagePixel) ? 0x30 : 0),
	            r, c);
			*/
			}
			colorShift++;
 #endif
 #if (defined(LIFE) && LIFE)
			lifeRound();
 #endif //LIFE
 
 #if (FRAME_COUNT_TO_DELAY != 0)
			timer_compareMatchIntEnable(0, OUT_CHANNELA);
 #endif //FRAME_COUNT_TO_DELAY
		}
 #if (!defined(FRAME_SYNC) && !FRAME_SYNC)
		counter+=16384;
 #else
		//for LIFE testing...
		counter++;
#endif
		heartUpdate();
	}
 #else
  #error "IMAGE_CHANGE is now required for drawPix with image..."
 #endif
#endif

#if (defined(PIXEL_SCROLL) && PIXEL_SCROLL)
	uint32_t counter=0;

	while(1)
	{
		if(counter==0)
		{
			uint8_t firstCol[2];
			firstCol[0] = frameBuffer[0][0];
			firstCol[1] = frameBuffer[1][0];

			uint8_t i;
			for(i=0; i<FB_WIDTH-1; i++)
			{
				frameBuffer[0][i] = frameBuffer[0][i+1];
				frameBuffer[1][i] = frameBuffer[1][i+1];
//				setBlue(frameBuffer[0][i], 0, i);
//				setBlue(frameBuffer[1][i], 1, i);
			}

			frameBuffer[0][i] = firstCol[0];
			frameBuffer[1][i] = firstCol[1];
//			setBlue(frameBuffer[0][i], 0, i);
//			setBlue(frameBuffer[1][i], 1, i);
		}
		counter+=16384;
		heartUpdate();
	}
#else
	while(1)
	{
		heartUpdate();
	}
#endif

}





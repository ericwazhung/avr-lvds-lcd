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

#if(defined(SEG_LINE) && SEG_LINE)
//****************************
//** GENERAL NOTE: (NOT specific to SEG_LINE)
//****************************
// this is getting ridiculous. The only change was using PLL_SYSCLK and
// LVDS_PRESCALER=4...
// ROW_CALCULATION_CYCS is set to something like 40000UL by default WITH
// LVDS_PRESCALER=8
// But, when LVDS_PRESCALER<8, ROW_CALCULATION_CYCS defaults to a lower
// value. Fine. But the effect is nearly identical with EVERY SEG_...
// The image seems to be stretched vertically, such that half the image
// fills the screen. 
// The effect could be quite useful... e.g. could double the frame-rate
// by halving the vertical resolution. I don't know how to harness this
// effect, yet. It's a strange effect, because it's *not* the data being
// sent... the display itself is somehow repeating the previous row's data
// The effect is crisp.
// The bit I don't get is that setting ROW_CALCULATION_CYCS to 0 previously
// caused jitter at the left-edge, but not line-doubling.
// So... weird.
// And now, setting it to 0 causes the refresh rate to increase
// (duh) but no repeated lines.

// More notes in timer0Setup.h.
// Experiments show that long RCS's give more repeated lines
// e.g. 100,000 repeats each row once
// 200,000 repeats it twice (?)
// 225,000 causes apparently one line to be repeated down the whole display
// 250,000 is too much for Timer0
// So, maybe this can't be used to bump up the refresh rate
// So what could it be used for...? Well, *tons* more calculation-time
//

#define ROW_CALCULATION_CYCS 0UL //200000//(8*DISPLAY_CYC_COUNT)//(1024*8) //0UL //40000UL
#endif


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
//#define SEG_SINE TRUE

#if(defined(SEG_SINE) && SEG_SINE)
#define ROW_CALCULATION_CYCS 40000UL
#endif

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
//#define SEG_HFM      TRUE
// You can override NUM_SEGMENTS here, for that purpose...
// OTHERWISE, it should probably be handled in rowSegBuffer.c
#if(defined(SEG_HFM) && SEG_HFM)
 #define NUM_SEGMENTS   224//192//127 //128 //95//96//128 //68 //128//68
 #define ROW_CALCULATION_CYCS 40000UL
#endif

//Displays a Question-Mark box, ala Mario-Brothers. Press the button and
// receive an award (and occasional goomba)
// Demonstrates usage of program-memory-based images... (16x16 pixels WOO!)
//#define SEG_QUESTION   TRUE

#if(defined(SEG_QUESTION) && SEG_QUESTION)
#define ROW_CALCULATION_CYCS 40000UL
#endif


//A Game! Ala "Racer" from the ol' TI-82 days...
// Use a potentiometer to try to keep the "car" on the race-track
// It gets harder the longer you stay on course!
// (Ideally: when this isn't true, remove ADC stuff from the makefile
//  to save codespace. I don't think I've ever actually paid attention to
//  this...)
//#define SEG_RACER    TRUE

#if(defined(SEG_RACER) && SEG_RACER)
#define ROW_CALCULATION_CYCS 40000UL
#endif


#define SEG_TET	TRUE
// Look into SEG_TET case in loadRow for some configurables
// (transparency overlay, etc).

//Tetris was written for the row-buffer
// and uses conversion...
// it's a bit wasteful, since it requires *both* a rowBuffer AND a
// rowSegBuffer, but it should work.
#if(defined(SEG_TET) && SEG_TET)
 #define ROW_BUFFER	TRUE
 #define NUM_SEGMENTS	96 //128//RB_WIDTH+10
 #define ROW_CALCULATION_CYCS 0//40000UL //10000UL //0UL

#endif


//SEG_GRADIENT draws a "smooth" gradient from black to some color 
// (currently white)... It uses HFM with a "power"-value that increases 
// with the row-number...
// It alternates rows between two shades until we've reached "full power"
// at the next shade. Then it repeats between the next two shades, etc.
// Mainly it's just for testing how plausible it is to create intermediate
// color-values... e.g. SEG_QUESTION's color-scheme isn't quite right...
// that question-box is supposed to look like copper!
// So one possibility is to alternate between two colors for some pixels
// Since the pixels are so large, the alternating colors would be barely 
// visible, and it would likely look more like a smooth color. 
// This kinda gives the ability to figure out how much we can get away with
// Also try SEG_TET, which has gradients between colors 
// (rather than shades)
//#define SEG_GRADIENT TRUE
#if(defined(SEG_GRADIENT) && SEG_GRADIENT)
 #define ROW_CALCULATION_CYCS	40000UL
#endif





//#warning "This needs to be moved... and conditional"
//#include "rowSegBuffer.c"




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
#if (defined(PLL_SYSCLK) && PLL_SYSCLK)
//a/o v60: This isn't required... using 8 allows for double-resolution
//         BUT, I've two seemingly identical displays with different 
//         revisions of the same LVDS-receiver chip... one will not sync
//         at the low bit-rate achieved with LVDS_PRESCALER=8
//         (I swear it did without a problem a while back... weird)
//         So, this doubles the bit-rate rather than doubling the 
//         resolution
#define LVDS_PRESCALER 4//8//1//2//8//2//1//8//2//4//8//2//8//2
//8//2//1//2//2//2//2//2//2//2//2//2//2//2//8//4 //1 //2//4//8//2//4
#else
#define LVDS_PRESCALER 8
#endif


// a/o v59
//This should probably always be TRUE now... It's been a LONG time since I
// experimented with it otherwise.
#define ROW_SEG_BUFFER   TRUE

/*
//now, SEG_STRETCH >= 3 causes weirdness... (repeated rows)
// previously 3 was OK
// This is fixed a/o newSeg, etc.
//a/o v59 I believe this is only used in the case when no SEG_(mode) is set
// to stretch a low-resolution row-buffer across the screen
// And that case (No SEG_mode set) doesn't really do anything anymore
#define SEG_STRETCH 5//4//3//2//3//4//6//3//4//6   //Stretch pixels using longer segments
//a/o v60: It's since been made specific for those cases, in loadRow()
//         (e.g. SEG_TET)
*/

#if (defined(ROW_SEG_BUFFER) && ROW_SEG_BUFFER)
 //a/o v60-5ish, this is no longer the case:
 //#warning "ROW_SEG_BUFFER requires ROW_BUFFER, but this is a hack"
 #if (!defined(LVDS_PRESCALER) || (LVDS_PRESCALER < 2))
  #error "ROW_SEG_BUFFER uses 20cyc/pixel, which isn't compatible with ROW_BUFFER -> width=64, since ROW_BUFFER used 16cyc/pixel. Bump your LVDS_PRESCALER up, or comment this error out to see what happens"
 #endif
 //ROW_SEG_BUFFER uses LOADROW... kinda the whole point.
 #define LOADROW	TRUE
// #define ROW_BUFFER TRUE
// #include "rowSegBuffer.c"
#endif

//a/o v60: ROW_SEG_BUFFER can now be TRUE without ROW_BUFFER
//         So, the below note isn't exclusively the case...
//a/o v59:
//If this is not true, then it uses the frameBuffer... which is no longer
// tested/implemented...
//#define ROW_BUFFER TRUE

//Only valid with ROW_BUFFER=TRUE above:
//#define HORIZONTAL_COLOR_BARS TRUE



//a/o v59-12ish, PLL_SYSCLK:
//Attempting to make the Timer reset at a value evenly divisible by 
// both 7 and 4... which might help to keep columns straight.
//Actually, it works excellently.
// but there's still some jitter when the number of pixels varies
// (e.g. SEG_RACER jitters on lines with text. SEG_SINE seems to jitter
//  with the sharp edges caused by running out of segments)
// This effect is unrelated, and likely due to experiments with zeroing
//  ROW_CALCULATION_DELAY (?) YEP.
#define ALIGN_TIMER_WITH_PIXCLK TRUE




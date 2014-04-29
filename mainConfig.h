/* mehPL:
 *    This is Open Source, but NOT GPL. I call it mehPL.
 *    I'm not too fond of long licenses at the top of the file.
 *    Please see the bottom.
 *    Enjoy!
 */






//a/o v66:
//This file is used for configuring the project at a high-level...
// Most configuration-options are simplified, here, by uncommenting various
// options.
// The basic idea, with a new/untested display is:
//   select BLUE_TESTING=TRUE (FRAMEBUFFER_TESTING=FALSE/commented-out)
//   select DE_BLUE=TRUE (all remaining = FALSE/commented-out)
//   If the display shows *anything* blue, then it's quite likely the
//    display can be used with this system with a little tweaking of
//    timing-values, etc.
//    If it shows completely blank (black or white) then you might be out 
//    of luck with this particular display... or maybe there's an
//    electrical or other problem (check those LVDS voltages)?
//    Don't select any of the other options until you can get DE_BLUE 
//    working (see troubleshooting.txt, maybe)
//
//   Once DE_BLUE is working, comment it out, and select BLUE_VERT_BAR...
//   Continue down the line...
//
//   Once all the "BLUE" tests are running (except maybe BLUE_ADC), 
//    quite a bit can be done by just manipulating the "BLUE" code...
//
//   OR:
//    comment-out BLUE_TESTING and select FRAMEBUFFER_TESTING
//   So far, there's only one FB_xxx test (FB_QUESTION)
//    Unfortunately, it's pretty complex as far as a starting-point.
//    If that works, you can do quite a bit with the display, already, at
//     low-resolution and basically the highest refresh-rate possible with
//     this system.
//    Other FB-based code exists, but hasn't been (re)implemented for use
//     in mainConfig.h. e.g. smiley.c, tetStuff.c, lifeStuff.c,
//     colorBarScroll.c (as I recall)
//
//    Also, RowBuffer code has not been (re)implemented in mainConfig.h
//     This code works by calculating each row's pixel-data before drawing
//     then storing that pixel-data in a row-buffer which is drawn in
//     real-time with the display-update. The benefit being much higher
//     resolution (16x16 pixel framebuffer -> 256 pixel rowBuffer, maybe
//     128, which can be recalculated for *every* row: e.g. 128x768 on a
//     1024x768 display) TODO
//
//   If you want more resolution (at the cost of refresh-rate) try
//    commenting-out FRAMEBUFFER_TESTING and BLUE_TESTING
//    and look into the SEG_xxx tests.
//    These work similarly to RowBuffer, but rather than storing each
//    individual pixel in a row, it stores "row-segments" which consist of
//    a color-value and a number of (drawable) pixels.
//   THIS IS HIGHLY DEPENDENT on the display's abilities. Some displays do
//   not like unpredictable/long delays between one row and the next... and
//   since, in this case, each row is calculated before being drawn, this
//   can introduce some *long* delays between rows. 
//   Unfortunately, TODO: I haven't yet had the chance to develop a more
//   step-by-step approach to determining a display's sensitivity to these
//   delays, so it's kinda all-or-nothing if the SEG_xxx method will work.
//   The latest, as I recall, is an "addressable" resolution of 680
//   drawable pixels across 1024, but of course there's not enough memory
//   for 680 color-changes. That means, any drawable pixel-segment can be 
//   any multiple of (and including) 1/680th of the screen-width, but 
//   there's a limit to how many such pixel-segments can be drawn.


// Again, start with BLUE_TESTING and DE_BLUE
// The "BLUE" methods are simple enough they can be easily modified for
// your own purposes.
// Some other things to consider, for your own projects:
//  (see lvds.c)
//  Since there are, essentially, separate wires for "Red" and "Green",
//  this system could be used purely for timing-purposes, using DE_BLUE,
//  and the "Red" and "Green" signals could be controlled externally (e.g. 
//  by analog-comparators, to create a dual-trace oscilloscope)
//  Or, DE_BLUE could be modified with different color-values to turn the
//  display into a color-filter for a stage-light... (or just hook the
//  "red" and "green" signals to switches?)
//  TODO: Add some color into the "BLUE" tests... before FRAMEBUFFER_TESTs



// Latest Version is v66... 

// Various notes exist from previous versions, and may be outdated...
// This file (like all, here) could stand to be reorganized.


// v66: Adding implementation notes to most files...
//      (Probably forgot quite a bit inbetween)
// v61: Significant cleanup, Experimenting with colors, etc...
//      notes labelled "a/o v61"
// v59: in which I'm adding a lot of notes re: defines, etc... 
//      labelled "a/o v59"



//The current state is such that basically ONLY the row-segment-buffer
// display-method has been tested for quite some time. Some of the other 
// code remains, but I can't promise it still works. Most of it has been
// moved to _old/ and _unusedIdeas/
// One odd-case: "RowBuffer" is (still) used in some SEG_whatever modes
//  below... e.g. SEG_TET. In this case, the *old* row-buffer methods are
//  used, and loadRow() converts this data into row-segments
//  "RowBuffer" is a bit of a misnomer, as it sort of implies that it
//  contains color-data. This isn't exactly true. It contains *register
//  values*, to be written to the PWM registers in real-time.
//  Likewise, the "RowSegmentBuffer" contains sets of two bytes which 
//  contain color-value and segment-length. But this data is packed in
//  these bytes in an odd way to make it calculation-efficient (increasing
//  the resolution).
//  In both cases (RowBuffer *and* RowSegBuffer) use function-calls to
//  write the values, rather than writing to the arrays directly.



//a/o v62, in which I attempt to get the ChiMei screen working



//START HERE. 
//Set this true, then see the explanations below.
//#define BLUE_TESTING	TRUE

//Later, comment-out the above, and try this one...
#define FRAMEBUFFER_TESTING	TRUE

//This is for testing a new (unimplemented) chip
// could also be useful for testing an implemented chip, since we're really
// pushing the chip's limits...
//For now, this just sets a specific and unchanging PWM signal, for
//'scoping.
//#define PWM_TESTING	TRUE

//TODO: This should probably be changed such that ROW_SEG_BUFFER is only
//paid attention to when it's TRUE... as it stands (a/o v67) it must be
//explicitly either TRUE or FALSE...
#define ROW_SEG_BUFFER FALSE
//This'll probably get changed during testing...
#define LVDS_PRESCALER 1
//This needs to be TRUE for LVDS_PRESCALER == 1
#define LVDS_PRESCALER_ERROR_OVERRIDE TRUE

#if (defined(BLUE_TESTING) && BLUE_TESTING)

//These strip it down to basically the bare-minimum for the highest
//refresh-rate and bit-rate possible with this system... 
//Just ignore these for now.
#define EXTERNAL_DRAWPIX	TRUE
#define ROW_SEG_BUFFER FALSE
#define LVDS_PRESCALER 1
#define LVDS_PRESCALER_ERROR_OVERRIDE TRUE
#define ROW_CALCULATION_CYCS 0

//START HERE (continued):
//When starting with a new (untested) display, you're best-off starting
//with one of these "BLUE" options
// Benefits are many. As I recall, they assure proper timing
// (Porch-widths, etc. as-configured) since no calculations are done 
// off-display.
//
// Additionally, since there's so little calculation being done, period,
// these tests give kind-of a go-nogo as to whether a display can sync with
// the maximum bit-rate and/or refresh-rate this system can provide (being
// that this system's maximum is probably far-below the display's specs).
//
// These may be better-described in lcdStuff.c
//
// When starting with a new display, probably best to try each of these in
// order... uncomment DE_BLUE, flash-it, try it... if it works, then
// comment it back out, and uncomment BLUE_VERT_BAR, and so-on.
//
//(In these tests, in LCDdirectLVDS, "blue" is actually more of a
// cyan/blue-green. I can't recall the exact details (they're documented
// somewhere) but I think I decided at one point to display green as an
// indicator after an entire row's pixels is completely drawn, in case I
// happened to be writing fewer pixels than the display expects, I'd
// know by seeing green... I think that code is carrying-over into these
// "older" tests. Or else I have a bug somewhere. Oh, and where it shows
// green, code-wise it would be "black.")
//
// Note that the timing-values are *not* exact, there's a bit more
// explanation in BLUE_BORDER, below... So, if you have trouble, maybe try
// fudging some timing-values...
//
// DE_BLUE displays solid blue on the screen
// This gives a pretty good idea whether the display is even capable of
// syncing. If you see *any* solid blue, (offset, or otherwise) then it's
// quite likely this display can be synced with a little experimentation.
// If it's solid ble and nothing else, then you lucked out!
//#define DE_BLUE TRUE

// BLUE_VERT_BAR, BLUE_VERT_BAR_REVERSED, and BLUE_BORDER display "black" (or green) and "blue" in vertical columns... 
// Again, a good test for the display's ability to sync
// If you want to lose hope, read this: It's quite plausible that the
// display may just be repeating one row's data, rather than actually
// paying attention to each (identical) row's data... So cross your fingers
// when you move on to BLUE_DIAG_BAR
// These two display one column in blue and another in "black"
//#define BLUE_VERT_BAR TRUE
//#define BLUE_VERT_BAR_REVERSED TRUE
// 
// BLUE_BORDER takes a value and draws (roughly) a vertical border on each
// side that number of pixels-wide. 
// Note, at small blue-borders it may be visible that the right edge is cut
// short due to integer-rounding, delay-loops, etc...
// (Note that this also means that DE may actually be active for longer
// than 1024 pixel-clocks! These tests are pretty simple, using simple
// delay-counter-loops which aren't *extremely* precise. In most cases
// they'll *increase* the DE-width, so if your display is having trouble
// syncing with values you're pretty sure should work, try decreasing the
// values in this project... e.g. DE_ACTIVE_DOTS on a 1024x768 display
// *should be* 1024, but with these delays you might have better luck with
// 1000... I've yet to run into such a display, but they might exist. Also,
// if it's *way* longer than the display's expecting, I've noticed that
// some displays continue onto the next line, or even repeat the previous
// line... weird.).
//
// Also, an odd experience: reflashing from 10 to 341 led to no syncing at
// all, until powered-down and powered-up. Dunno, maybe the MISO/MOSI pins
// were sending stray data to the display which confused it.
//#define BLUE_BORDER 341//10//20



// BLUE_DIAG_BAR displays two triangles, essentially, of blue, and black.
// This is where things get iffy... I've seen some amazingly weird
// patterns with this one; one time I swear ancient egyptians were trying
// to communicate through time by transmitting their heiroglyphs to my
// display. Really, it's just supposed to be the screen divided diagonally
// from upper-right to lower-left (roughly) the top half "black" and the 
// bottom-half "blue".
//Now for the moment of truth for my ChiMei display:
// Worked on the First Go!
// And no visible refresh?! Wow!
//#define BLUE_DIAG_BAR TRUE

//A new test (for this refreshless display)
// BLUE_DIAG_BAR_SCROLL just takes the above idea, and scrolls it
// vertically, one row at a time.
// 
// Amazing... no visible refresh *at all*, also lines seem highly precise,
// resolution-wise... almost shockingly-so... TODO revisit this math!
// The entire diagonal pattern cycles at 1:04, it's 1024 pixels tall
// that's a refresh-rate of... 64seconds for 1024 refreshes, or dang-near
// exactly 16Hz refresh.
// Really, this all seems too good to be true. 
//#define BLUE_DIAG_BAR_SCROLL TRUE


//Another new test
// BLUE_DIAG_SCROLL_FLASH takes the above idea and alternates the colors in
// each frame... Another good visual test for refresh-rate and method.
// Also, this'll probably cause seizures, so be warned.
// On this "refreshless" display, it causes some pretty interesting
// illusions... while the image itself is scrolling up, peripheral-vision
// seems to think it's it's scrolling down. Totally unintentional, just an
// interesting observation.
//#define BLUE_DIAG_SCROLL_FLASH TRUE
//This just disables flashing, and alternates rows, instead...
// probably mostly-useless. It should be enabled *with*
// BLUE_DIAG_SCROLL_FLASH
//#define BDSF_ALTERNATE_ROWS TRUE


//New a/o pwm161 (v67):
// BLUE_AND_COLORS does quick-testing for the other colors as well
// It should display black, red, blue, and cyan in diagonal "bars"
// I should probably move its code to _commonCode.../lcdStuff
// but for now it's in main.c
// THIS HAS NOT BEEN TESTED on the Tiny861, but should be implemented...
//#define BLUE_AND_COLORS	TRUE

#if(defined(BLUE_AND_COLORS) && BLUE_AND_COLORS)
 #undef EXTERNAL_DRAWPIX
#endif

//New a/o pwm161 (v67):
// BLUE_ALLSHADES fades from black to bright blue...
// THIS DOES NOT WORK with the Tiny861
#define BLUE_ALLSHADES	TRUE
//ALLSHADES_GRADIENT shows the BLUE_ALLSHADES as a gradient
// (BLUE_ALLSHADES should also be TRUE, to use ALLSHADES_GRADIENT)
//It doesn't work! Does something *really* weird
// Not sure what's going on... maybe the bits are misaligned...?
// except, that BLUE_ALLSHADES seems to work right....
// Could have to do with DelayDots() being too long...?
// Also, maybe, consider the fact that having a white border seemed to help
// syncing in much older versions...
// I've tried *numerous* different ways, and it just ain't woikin.
// Also plausible... Maybe One-Ramp-Mode doesn't actually pay attention to
// 2RB, explicitly... in which case, switching things 'round might cause
// the clock signal to be distorted...
// That seems to be it... or close, anyways. Now it works.
// (by disabling the shades that require wrap-around)
// So we're left with essentially four discernable shades, 
// which is one more than the Tiny861, Woot!
#define ALLSHADES_GRADIENT TRUE
//ALLSHADES_AND_COLORS adds Red, Green, and Yellow horizontal bars
// to ALLSHADES_GRADIENT (which must be true, for this to also be)
//The name's a bit misleading, it's not "All Shades-and-colors" it's 
// "All-Shades and colors"
#define ALLSHADES_AND_COLORS	TRUE

#if(defined(BLUE_ALLSHADES) && BLUE_ALLSHADES)
 #undef EXTERNAL_DRAWPIX
 #ifndef __AVR_AT90PWM161__
  #error "BLUE_ALLSHADES only works with the AT90PWM161, so far..."
 #endif
#endif


//Yet Another New Test (prior to v67)
// BLUE_ADC reads the ADC value before drawing each row...
// The value read determines the horizontal position of the color-change
// thus, it's a very simple Oscilloscope!
//This is pretty much identical to most of the BLUE_TESTING in lcdStuff.c
// but, it throws some stuff before DEonly, which means that the
// horizontal-back-porch is extended during this calculation/measurement
// time... The amount may vary, so if the display requires a stable HFP,
// then this may cause syncing problems. Further, the delayDots function is
// really best-optimized to non-varying values, so it may cause DE to be
// extended longer than in previous tests. (Wait, DIAG_BAR is varying...)
// TODO: Consider creating a specific test for a display's immunity to 
//   varying porch-times...? (This is kinda hokey to implement, because
//   some displays seem more/less immune depending on the particular
//   refresh-rate, etc.)
// Interestingly, the ChiMei display seems to sync almost perfectly despite
// this varying HFP, etc. but has an *audible* squeel (power-switching
// circuitry?). The LTD display does not appear to respond to this at all.
// it shows an unchanging solid vertical bar
// TODO: Look into this!


//This is specific to LCDdirectLVDS... only because I've already
//implemented the adc here. It could be handy as a test in lcdStuff, in
//general.
//
// Also, Have thought about using the analog-comparator, instead... the
// benefit being that its interrupt could be used to trigger color-change,
// which would leave the processor *free* during the DE-Active time
//Again, its functionality is somewhat limited, one display doesn't like
//it at all, and the other quite literally squeels.

//THIS HAS NOT BEEN TESTED AT ALL with the PWM161
//#define BLUE_ADC	TRUE

#if(defined(BLUE_ADC) && BLUE_ADC)
 #undef EXTERNAL_DRAWPIX
#endif


// If you've gotten it going with all the above tests, then you're well on
// your way to doing some pretty groovy things. 
// Check out lcdStuff.c and create your own "drawPix" functions based on
// those in lcdStuff, or try commenting-out BLUE_TESTING, at the top of
// this file and continue below.



#elif (defined(FRAMEBUFFER_TESTING) && FRAMEBUFFER_TESTING)



//If FB_REFRESH_ON_CHANGE is TRUE, only refresh the LCD when there's a
//change in the image... It works well, but on single-refresh changes, some
//displays leave a ghost of the prior image. Thus, it's been designed to do
//*two* refreshes upon each change, which is nice... but it's not
//sophisticated enough to recognize when an image is about to change again,
//in which case it shouldn't need to refresh-twice if, e.g. the image is
//alternating (like the Star)
// In other words, it's kinda seizure-inducing.
// In another design, with only a seldom-changing image, it'd be sweet.
//This is loosely derived from _unusedIdeas/frameCountToDelay.c
//If it's NOT TRUE, then the LCD refreshes occur back-to-back regardless of
//changes in the image
//NOTE: If this is enabled, and you write your own code, you must keep in
//mind that the TFT will eventually fade unless refreshed...
// It's also not particularly smooth, not sure why exactly.
// (e.g. sometimes it updates quicker than others... dms problem?)
//NOTE 2: If an image is left for too long, apparently it sticks a bit...
// e.g. running Smiley for a while, then running Question, the outline of
// the smiley-face can be seen in solid-color images...
// This might be hard on the pixels, I dunno.
//THIS REQUIRES DMS_TIMER (as-implemented)
// which is not available on ATTiny861, probably only due to
// space-limitations... It could probably be re-added in the makefile...
#if (defined(__AVR_AT90PWM161__))

//COMMENT OR UNCOMMENT AS DESIRED:
//#define FB_REFRESH_ON_CHANGE TRUE

#endif

//a/o v62:
//Not sure why this isn't working in this case...
//Some rows "wobble" with this TRUE... it looks better if this is FALSE...
// (Still jitters, but less noticeable)
// Is this a ChiMei thing, or bad math somewhere...?
#define ALIGN_TIMER_WITH_PIXCLK FALSE

//My delay-loop should be more precise *when its argument is constant*
// Here it's used for stretching the framebuffer pixels horizontally
#define DELAY_CYC_DELAY_LOOP FALSE

#define LVDS_PRESCALER 1
#define LVDS_PRESCALER_ERROR_OVERRIDE TRUE
#define ROW_CALCULATION_CYCS 0
#define ROW_BUFFER FALSE
#define ROW_SEG_BUFFER FALSE


//Loads a stationary smiley-face image into the frame-buffer and display it
#define FB_SMILEY	TRUE


//Loads the frameBuffer equivalent of SEG_QUESTION (see description, below)
// Note there are several options for testing in fb_question.c
// It's entirely plausible I might've left one enabled that shouldn't be...
// e.g. "AUTO_HIT" or random-override...
//#define FB_QUESTION	TRUE




#if(defined(FB_QUESTION) && FB_QUESTION)
#undef ALIGN_TIMER_WITH_PIXCLK
#define ALIGN_TIMER_WITH_PIXCLK TRUE
#endif


#elif (defined(PWM_TESTING) && PWM_TESTING)



#else	//NOT BLUE_TESTING, FRAMEBUFFER_TESTING, or PWM_TESTING
		//(SEG_Whatevers)

//Re the next two settings: (a/o v62)
//This is a quick-attempt at getting the ChiMei display working with
//SEG_Watevers, below...
//The ChiMei display seems to need higher bit-rates than the other display,
//so remove the prescaler...
#define LVDS_PRESCALER 1
//Some math relies on a higher LVDS_PRESCALER value... There's an #error
//regarding this, but it can be ignored in some cases... 
// so let's ignore it.
#define LVDS_PRESCALER_ERROR_OVERRIDE TRUE
#define ROW_CALCULATION_CYCS 0

//From here-on I've done a ton of development with a particular display 
// which seemed particularly-forgiving of things like DE-Active for way 
// longer (or shorter) than expected... Further, it was more than happy 
// refreshing all the way down to 1/5th of a Hz. Yes, that's 5 seconds 
// to refresh the entire display. This is nice for high-resolution, but 
// also it's quite probable that most displays can't handle these sorts of
// refreshes. (This is part of why I've put off the ChiMei display for so 
// long, though have been pleasantly surprised with it since I 
// reimplemented the BLUE_TESTs to get it going.

// Realistically, there's a significant bit that's been removed inbetween
// the BLUE_TESTING and the SEG_whatevers below...
// (With regards to the original tests with the LTN display)
// These tests have been removed, but I might need to rethink them for this
// ChiMei display, which worked fine with all the BLUE_TESTs but not yet
// with the SEG_Whatevers, below...
// * A *tiny* frame-buffer (16pixels x 16pixels stretched across the LCD)
//   (Nice for early testing, because it doesn't require much processing
//    and therefore doesn't interfere with timing)
// * A *similarly tiny* row-buffer (Calculations and/or loading from 
//    program-space are done during the Horizontal Front Porch, but if they
//    take longer than the HFP, some displays might not be happy)
//
// And then, here, these "SEG_whatevers" basically do *all* the
// calculations (and a lot of them) during the HFP, almost always causing
// the HFP to actually be *longer* than the actual time it takes to display
// a single row (the DE-Active time).


//CONFIGURATION:

//Only one of the following SEG_whatevers should be TRUE
// (Assuming we're using the Row-Segment-Buffer, as explained above).
//  HAH !!! I SEEM TO HAVE LOST THAT EXPLANATION A/O v62!!! TODO TODO TODO
// IF NONE ARE TRUE:
//  The default is to convert whatever's in the row-buffer into 
//  the row-segment-buffer...
//  This is where my old-code starts getting iffy, as the row-buffer hasn't
//  been used (exclusively) for quite some time.
// a/o v61: I think it's the case that an error will be generated if none
//  of the SEG_whatevers are true. Further, as noted above, now the 
//  row-buffer translation into the RowSegBuffer has been tested...
//  See "SEG_TET" (and loadRow()) for examples.
// Best to stick with assigning one of these SEG_xxx things true.
// NUM_SEGMENTS and ROW_CALCULATION_CYCS:
//  These values are overrides for the default, and are mostly for testing-
//  purposes... e.g. in SEG_LINE ROW_CALCULATION_CYCS is 0... this does NOT
//  mean that SEG_LINE mysteriously creates the next row's data without
//  using any processor cycles. It does *not* affect how many cycles the
//  row-calculation takes. For now, it's just a number that affects
//  display timing a little bit... read more about it and NUM_SEGMENTS 
//  elsewhere before changing these overrides.

//Draws a diagonal white line on a red background...
// I think it's supposed to repeat three times and not fill the entire
// screen... though it's been a long time since I've used this.
#define SEG_LINE TRUE

#if(defined(SEG_LINE) && SEG_LINE)

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
// SEG_HFM visualizes that, and actually looks pretty cool. Like moire
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
// receive a reward (and occasional goomba)
// Demonstrates usage of program-memory-based images in rowSegBuffer... 
//(16x16 pixels WOO!)
// This is much less functional than FB_QUESTION
//#define SEG_QUESTION   TRUE

//a/o v62:
// ROW_CALCULATION_CYCS is highly finicky with the screen's syncing at
// LVDS_PRESCALER==1
#if(defined(SEG_QUESTION) && SEG_QUESTION)
 #if(LVDS_PRESCALER == 1)
  #define NUM_SEGMENTS 128
  #define ROW_CALCULATION_CYCS 8000UL
 #else
  //This is from memory, should be compared with an older version
  #define ROW_CALCULATION_CYCS 40000UL
 #endif
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


//#define SEG_TET	TRUE
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


#endif //BLUE_TESTING and SEG_whatevers...



// I hereby declare this FPD-Link simulation technique to forever be called
//  PW-BANGing


//For testing of slower LVDS pixel-rates
// (maybe we can increase the resolution)
// LVDS_PRESCALER value must be 1, 2, 4, or 8
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
// a/o v61: PLL_SYSCLK is true by default (and thus LVDS_PRESCALER=4)
//
#ifndef LVDS_PRESCALER
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
#endif

// a/o v59
//This should probably always be TRUE now... It's been a LONG time since I
// experimented with it otherwise.
#ifndef ROW_SEG_BUFFER
 #define ROW_SEG_BUFFER   TRUE

// a/o v62: This shouldn't need to be TRUE for blue-testing, etc... right?
 #error "WTF?"
#endif

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
  #if (!defined(LVDS_PRESCALER_ERROR_OVERRIDE) || \
		  !LVDS_PRESCALER_ERROR_OVERRIDE)
   #error "ROW_SEG_BUFFER uses 20cyc/pixel, which isn't compatible with ROW_BUFFER -> width=64, since ROW_BUFFER used 16cyc/pixel. Bump your LVDS_PRESCALER up, or comment this error out to see what happens"
  #endif
 #endif
 //ROW_SEG_BUFFER uses LOADROW... kinda the whole point.
 #define LOADROW	TRUE
// #define ROW_BUFFER TRUE
// #include "rowSegBuffer.c"
#endif

//a/o v60: ROW_SEG_BUFFER can now be TRUE without ROW_BUFFER
//         (but I don't think it works anymore the other way 'round)
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
//  ROW_CALCULATION_CYCS (?) YEP.
#ifndef ALIGN_TIMER_WITH_PIXCLK
#define ALIGN_TIMER_WITH_PIXCLK TRUE
#endif



// Random Notes...
//
//  Basically, for new development, I'd say don't use RowBuffer translation
//  (Though, rowBuffer *without* rowSegBuffer has *not* been tested for
//   quite some time. It does have some benefits: It's a little faster,
//   so with low-resolution the frame-rate can be bumped up a bit... which
//   might be necessary for some displays... I need to look into this again
//   soon... Maybe try again to get these other displays working.)
//  And, actually, using rowBuffer *with* rowSegBuffer (as in SEG_TET)
//   has the benefit that it's known how long each DE will be... I'll have
//   to think about this some more.
//  So lemme reword that:
//   Since I got this working with the LTN121X1 display, I have focussed
//   quite a bit on its capabilities rather than general display 
//   functionality... At one time this code was general enough that
//   sync-times were dang-near exact... Where, e.g. one could hook up a
//   display to a computer and mess with sync-parameters there, until the 
//   display was found to function at the lowest-possible timing parameters
//   at which point, if those were low enough to be mimicked with the bit-
//   rates here, then you'd have a pretty good idea it'd work.
//   That testing-ability has been somewhat compromised since then...
//   E.G. RowSegBuffer (and RowBuffer) does its drawing calculations during
//    what looks to the display like the Horizontal Back Porch...
//    These calculations are *long* and probably not exactly mimickable via
//    a computer's screen-timing-parameters.
//    Further, the duration of a "line" varies with ROW_CALCULATION_CYCS
//    and the total length of the row-segments drawn...
//    It is possible to have a total of row-segments that is far longer (or
//    far shorter) than DE. Ideally, for syncing, the total would match DE,
//    but I've been pretty loose with this requirement, since the display
//    I'm working with doesn't seem to care. SEG_SINE is a good example of
//    this looseness. SEG_LINE is a good example where the duration is
//    constant line-by-line, but DE is held active for far less than the
//    specified time. SEG_RACER, as of recent versions, has been adapted
//    to keep a pretty constant DE-duration, line-by-line, AND to match the
//    DE specification, but the addition of ROW_CALCULATION_CYCS really
//    stretches the horizontal-back-porch...
//   E.G.2. Changing things like the LVDS_PRESCALER, OSCCAL, etc. affects 
//    the bit-rate, and thus the pixel-clock... There's no simple way (in
//    code, via macros, whatever) to determine how this all would 
//    correspond to matching timing-parameters with a computer's... it's a
//    lot of math, in a lot of places).
//   E.G.3. Changing things like the above to match the timing-parameters
//    found on a computer may likely result in values which are so fast
//    that the code would have to be modified... For around ~20Hz refresh
//    (which I had once) a *tiny* frame-buffer was necessary, limiting the
//    image to something like 32 pixels by 32 pixels. Good for inital
//    testing of timing-values, but that code is no longer implemented.
//    I've ordered a VGA->LVDS converter, which may help me redevelop that
//    code as a good starting-point for new displays. Ve Shall See.
//    (Previously, I did those experiments by connecting the displays as my
//     main display on my main developing system... it was hokey and a bit
//     traumatizing, and not at all recommended). 




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
 * /Users/meh/_avrProjects/LCDdirectLVDS/68-backToLTN/mainConfig.h
 *
 *    (Wow, that's a lot longer than I'd hoped).
 *
 *    Enjoy!
 */

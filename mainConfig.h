/* mehPL:
 *    This is Open Source, but NOT GPL. I call it mehPL.
 *    I'm not too fond of long licenses at the top of the file.
 *    Please see the bottom.
 *    Enjoy!
 */





#ifndef __MAIN_CONFIG_H__
#define __MAIN_CONFIG_H__





//##################################################
//##################################################
//###   THIS BIT IS IN EARLY-TESTING             ###
//###   Unless using a Sony ACX705AKM            ###
//###   or similar (parallel-interfaced)         ###
//###   display, just comment out the next line. ###
//###   Note that other displays have not been   ###
//###   tested recently, but should still work.  ###
//##################################################
#define SONY_TESTING TRUE

#if(defined(SONY_TESTING) && SONY_TESTING)
//EARLY TESTING>>>>>

//So far, this is only implemented with the Sony parallel-LCD
// It should be *really easy* to implement with other displays/interfaces
// Just grep this.
// If the display is partially covered by display-housing, then we'd like
// to fit the frame-buffer into the *visible* portion
// This'll likely be something *near* but *smaller than* DE_ACTIVE_DOTS
#define VISIBLE_ROW_DOTS	225

//Override the default WHITE at the end of the row with BLACK...
#define FB_END_OF_ROW_COLOR	_K

//This is an attempt at moving calculations *before* Hsync, so as to work
//with a non-DE display... had a *minor* effect...
#define WC_SETUP TRUE
//This is pretty much exclusively for bit-banged MCK on the 6bitParallel
//interface...
// bit-banged seems to have fixed the hsync-shift problem, but now the
// display flickers...
#define LCDINTERFACE_BITBANGED_DOTCLOCK	TRUE

//This attempts to prevent the REFRESH_ON_CHANGE issue where alternating
//rows seem to be not-displayed... NOGO...
//The idea was to continue running the dot-clock via PWM whenever the
//display isn't being refreshed.
//SO FAR: FB_REFRESH_ON_CHANGE TRUE, _COUNT=1:
// _DELAY=10ms is OK, but 100ms causes the lines to alternate and the
// screen to flicker...
//FB_REFRESH_ON_CHANGE FALSE displays perfectly, but causes there to be no
//CPU power for other processes (like updating the image)
#define LCDINTERFACE_BITBANGED_DOTCLOCK_PWM	TRUE



#if(defined(LCDINTERFACE_BITBANGED_DOTCLOCK) && \
		LCDINTERFACE_BITBANGED_DOTCLOCK)
 #define LCDSTUFF_INCLUDE_NON_DE TRUE
#endif


#if(defined(LCDSTUFF_INCLUDE_NON_DE) && LCDSTUFF_INCLUDE_NON_DE)
 #warning "LCDSTUFF_INCLUDE_NON_DE is used almost exclusively, currently, for indicating LCD_INTERFACE_BITBANGED_DOTCLOCK. Most references to INCLUDE_NON_DE should probably be replaced..."
#endif


//_LCD_SPECIFIC_HEADER_ is the header for the particular display being used
// and handles Hsync/Vsync timing, etc.
// It is #included in main.c, but defined here for configuration-purposes
// If a specific header doesn't exist for your display, try looking into 
// _displays/testDisplay.h...
// CURRENTLY (a/o v80) most displays are using testDisplay.h (lvds)
// Including the Samsung LTN display as well as the BOE Hydis display
// (What about Chi Mei???)
// And, actually, the addition of _LCD_SPECIFIC_HEADER_ comes *after*
// working with the various displays, so it's no longer guaranteed to work
// with them all...  TODO.

//_LCD_INTERFACE_CFILE_ contains the initialization code for the display's
// electrical-interface (generally LVDS/FPD-Link)
// It handles initializing the pixel-clock timer, as well as setting 
// port-directions, etc.
// This is #included in main.c

//GENERALLY:
// The above two can be left undefined, as defaults are fine for most
// displays I've worked with... see the DEFAULTS, below.

//For the Sony ACX705AKM (parallel-interface LCD):
//These FOUR go together (and should all be either commented or
//uncommented, together)
//Later, maybe, _LCD_INTERFACE_CFILE should be handled in
// _displays/sonyACX705AKM.h?
// (Though, technically, the display *could* be interfaced via an FPD-Link
//  receiver chip, using _interfaces/lvds.c... Unlikely, but maybe useful
//  to keep these things separate for future displays/interfaces?)
//#######################
//COMMENT OR UNCOMMENT THESE FOUR TOGETHER.
#define _LCD_SPECIFIC_HEADER_ "_displays/sonyACX705AKM.h"
#define _LCD_INTERFACE_CFILE_ "_interfaces/6bitParallel.c"
//Since the MISO pin is available for the heart, it's used instead of the
//default assigned by the makefile
#define _HEART_PINNUM_	PB1
#define _HEART_PINPORT_	PORTB
//####   TO HERE   ######
//
// END OF EARLY (Sony/Parallel) TESTING
//################################################
#endif







//THESE ARE THE DEFAULTS FOR:
// 1024x768 display via LVDS/FPD-Link Interface
// To select another display/interface and override these defaults
//  add your own #defines above
#ifndef _LCD_SPECIFIC_HEADER_
 #define _LCD_SPECIFIC_HEADER_	"_displays/testDisplay.h"
#endif
#ifndef _LCD_INTERFACE_CFILE_
 #if (defined(__AVR_AT90PWM161__))
  #define _LCD_INTERFACE_CFILE_ "_interfaces/lvds161.c"
 #else	//Assumed ATtiny861:
  #define _LCD_INTERFACE_CFILE_ "_interfaces/lvds.c"
 #endif
#endif //_LCD_INTERFACE_CFILE_ not previously defined


//a/o v82: This is for FB_QUESTION... TODO: This should be moved.
#define BUMP_SWITCH	TRUE


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




// a/o v80:
// NOTE RE: BLUE TESTS:
//
// These tests were developed with displays which make use of the Data
// Enable signal... These displays only draw data when DE is active, and
// appear to be very flexible with regard to varying delays between each
// row's Hsync and the corresponding Data Enable.
// THUS: It has only now come to my attention (as-of experimenting with a
// parallel-interfaced display *lacking* a Data-Enable signal)
// Just How Varying these tests are with regards to timing.
// 
// Conclusion: This Project Works Well With Displays that have DE
//   And not so well with those that rely on specific timing values.
//
// That'll likely change soon, since I want to get this Sony display
// working...
//
// Additional Conclusion: DE-Displays seem to have a lot of flexibility
// that's nice to work with, and lend themselves toward quite a bit more
// hacking and limit-stretching.
//



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




//This is for testing a new (unimplemented) chip
// could also be useful for testing an implemented chip, since we're really
// pushing the chip's limits...
//For now, this just sets a specific and unchanging PWM signal, for
//'scoping.
//#define PWM_TESTING	TRUE






//START HERE. 
//Set this true, then see the explanations below.
//#define BLUE_TESTING	TRUE

//Later, comment-out the above, and try this one...
#define FRAMEBUFFER_TESTING	TRUE

//a/0 v70:
// RowBuffer hasn't been implemented in some time...
// Time to do it!
//#define ROWBUFFER_TESTING	TRUE

//a/0 v70+
// This used to be the else-case, but now it's explicit...
//#define ROWSEGBUFFER_TESTING TRUE

//a/o v69:
//Comment them both (and PWM_TESTING) out for SEG_whatevers...
// This should probably become SEG_TESTING or something.
//Actually, that's not quite right... we're still missing RowBuffer
//testing!



//These are defaults that will probably be overridden in the corresponding
//_TESTING tests, below.
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

//This one makes more sense... it draws a blue bar roughly 1/3rd of the
//screen-width at the left-edge...
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
//#define BLUE_BORDER (DE_ACTIVE_DOTS/4) //341//10//20



// BLUE_DIAG_BAR displays two triangles, essentially, of blue, and black.
// This is where things get iffy... I've seen some amazingly weird
// patterns with this one; one time I swear ancient egyptians were trying
// to communicate through time by transmitting their heiroglyphs to my
// display. Really, it's just supposed to be the screen divided diagonally
// from upper-right to lower-left (roughly) the top half "black" and the 
// bottom-half "blue".
// a/o v80: Top-half and bottom-half have been flipped, so the actual
// data-starting-point can be seen for syncing. (top half is blue)
//Now for the moment of truth for my ChiMei display:
// Worked on the First Go!
// And no visible refresh?! Wow!
// a/o v80: (for parallel-interfaced display with no DE signal)
//  NOTE that BLUE_DIAG_BAR takes a few more instructions than the previous
//  tests... so there may be a greater shift. Displays which have a DE
//  signal (e.g. every FPD-Link display I've run into) this doesn't matter,
//  since the data doesn't start displaying until DE is active (regardless
//  of whether the delay between the Hsync and DE varies). But the Sony
//  display, for example, relies on a specific number of dot-clocks between
//  the Hsync and the actual display of data, so... hmmm...
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
#define BLUE_DIAG_BAR_SCROLL TRUE


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
//a/o v80: This displays ONLY BLACK on the Sony parallel-interfaced
//display, likely due to the number of calculations necessary before the
//drawing is started. 
//#define BLUE_AND_COLORS	TRUE

#if(defined(BLUE_AND_COLORS) && BLUE_AND_COLORS)
 #undef EXTERNAL_DRAWPIX
#endif

//New a/o pwm161 (v67):
// BLUE_ALLSHADES fades from black to bright blue...
// THIS DOES NOT WORK with the Tiny861
//a/o v80: It likely won't work with parallel-interfaced displays, either.
//#define BLUE_ALLSHADES	TRUE

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
 #define USE_ADC TRUE
#endif


// If you've gotten it going with all the above tests, then you're well on
// your way to doing some pretty groovy things. 
// Check out lcdStuff.c and create your own "drawPix" functions based on
// those in lcdStuff, or try commenting-out BLUE_TESTING, at the top of
// this file and continue below.



#elif (defined(FRAMEBUFFER_TESTING) && FRAMEBUFFER_TESTING)


//This is just for generalizing things here...
// it might make more sense to put something like this in heartbeat
// since it's the easy way to get a timer going, using two common methods
#if(defined(_HEART_DMS_) && _HEART_DMS_)
 #define MAIN_MS	DMS_MS
#elif(defined(_HEART_TCNTER_) && _HEART_TCNTER_)
 #define MAIN_MS	TCNTER_MS
#else
// #error "This error only applies to those that use DMS_MS, or TCNTER_MS, it can probably be deleted. (e.g. frameBuffer.c with REFRESH_ON_CHANGE)"
#endif


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
// I think we're there...
//#if (defined(__AVR_AT90PWM161__))


//WITHOUT FB_REFRESH_ON_CHANGE, if the framebuffer-updater function (e.g.
//tet_update() takes too long, there's nothing can be done to avoid the
//image's being only partially-written in the middle of a refresh.
// This *despite* the fact, the framebuffer-updater function is only called
// at the end of a frame.
//
// Alternatively (to avoid partial framebuffer refreshes) set this TRUE and
// set REFRESH_ON_CHANGE_COUNT 1 and REFRESH_ON_CHANGE_DELAY = 0
// it should be roughly the same, but slightly slower because it pauses
// refresh until after the framebuffer-updater function completes.
//COMMENT OR UNCOMMENT AS DESIRED:
#define FB_REFRESH_ON_CHANGE TRUE




//Number of screen refreshes to perform whenever the framebuffer changes
// (1 usually leaves some ghosting, 2 usually clears that up)
#if(!defined(LCDINTERFACE_BITBANGED_DOTCLOCK) || \
		!LCDINTERFACE_BITBANGED_DOTCLOCK)
 #define FB_REFRESH_ON_CHANGE_COUNT	2
#else
//Experiments for the sony parallel-bitbanged LCD...
//#define FB_REFRESH_ON_CHANGE_COUNT	4
 #define FB_REFRESH_ON_CHANGE_COUNT	1
#endif

//After the screen refresh(es) complete, wait this long before calling the
//framebuffer-updater function (e.g. tet_update, or question_update)
// which calculates a new frame image (which may be different from the
// current one)



//This value was last-used
  #define FB_REFRESH_ON_CHANGE_DELAY	(100*MAIN_MS) //(0*DMS_MS)
 //#define FB_REFRESH_ON_CHANGE_DELAY	(1*MAIN_MS) //*MAIN_MS) //(0*DMS_MS)


// TODO:
// using various settings (currently COUNT=2, DELAY=1)
// it's quite probable that a changing-image will cause fb_updater to be
// called less-often than an unchanging image...
// e.g. if a mushroom is shown, and expected to remain in place for 5
// fb_question_update() calls, it will go *much quicker* than 5
// fb_question_update() calls that result in a scrolling image.
// Not sure what to do to stabilize this, off hand... 
// That particular example isn't such a big deal, but consider the case
// where the question-box scrolls down once every 3 updates, but the coin
// rotates once every 2 updates, then the motion of the coin and scrolling
// occurs at a spurradic rate, and once the coin is alone, it rotates
// faster than it did while scrolling.

//PARTIAL_REFRESH (currently only works with fb_question)
// causes the screen to only refresh down to the lowest line that changed
// in the last framebuffer update
// CURRENTLY it does NOT work with FB_REFRESH_ON_CHANGE_COUNT > 1
// because having the partial-refreshes back-to-back causes the screen to
// think it's a continuation of the previous (unfinished) refresh
// Likewise, if the frame-updates come too quickly, the same can happen
// The effect is odd, it actually partially-refreshes at the top, as
// expected, but *also* loads the same data lower down the display.
// So, if that happens, try bumping up FB_REFRESH_ON_CHANGE_DELAY
//
// For this to be useful it needs some other features...
// TODO:
// e.g. it should definitely be able to double-refresh to reduce ghosting
// when transitioning to a stationary image (e.g. a mushroom)
// double-refresh + partial-refresh might get hokey, as described above
// Something about inserting a delay... but then what's the benefit if not
// to refresh faster...?
// (check out stopRefreshAtRow in _options/frameBuffer.c, re: +2)
// Something about memory for the last-refresh...? (double should fix that)
// Something about how non-double-refresh is kinda nice for some things,
// e.g. the rotating coin, ghosting looks like motion-blur
// 
//#define PARTIAL_REFRESH	TRUE

#if(defined(PARTIAL_REFRESH) && PARTIAL_REFRESH)
 #define FB_REFRESH_ON_CHANGE_COUNT 1
 #define FB_REFRESH_ON_CHANGE_DELAY (200*MAIN_MS)
#endif

//#endif	//PWM161

//a/o v62:
//Not sure why this isn't working in this case...
//Some rows "wobble" with this TRUE... it looks better if this is FALSE...
// (Still jitters, but less noticeable)
// Is this a ChiMei thing, or bad math somewhere...?
//#define ALIGN_TIMER_WITH_PIXCLK FALSE

//My delay-loop should be more precise *when its argument is constant*
// Here it's used for stretching the framebuffer pixels horizontally
#define DELAY_CYC_DELAY_LOOP FALSE

#define LVDS_PRESCALER 1
#define LVDS_PRESCALER_ERROR_OVERRIDE TRUE
#define ROW_CALCULATION_CYCS 0
#define ROW_BUFFER FALSE
#define ROW_SEG_BUFFER FALSE


//Loads a stationary smiley-face image into the frame-buffer and display it
//#define FB_SMILEY	TRUE


//Loads the frameBuffer equivalent of SEG_QUESTION (see description, below)
// Note there are several options for testing in fb_question.c
// It's entirely plausible I might've left one enabled that shouldn't be...
// e.g. "AUTO_HIT" or random-override...
#define FB_QUESTION	TRUE

//AUTO_HIT is mostly for debugging, but also makes it useful when there's
//no button...
//#define AUTO_HIT TRUE

#if(defined(FB_QUESTION) && FB_QUESTION)
#undef ALIGN_TIMER_WITH_PIXCLK
#define ALIGN_TIMER_WITH_PIXCLK TRUE
#endif

#if(defined(FB_QUESTION) && FB_QUESTION)
//If you're using a piezo-element for a hit-detector, then uncomment this:
//(Hasn't been fully reimplemented, it may or may not work)
//#define PIEZO_HIT_DETECTION TRUE
#if(defined(PIEZO_HIT_DETECTION) && PIEZO_HIT_DETECTION)
#define USE_ADC TRUE
#endif
#endif

//a/o v69:
//Tetris hasn't been used since the ol' "SEG_TET" days long ago...
// But originally it was written for the frame-buffer
// and then it was modified for the row-buffer
// and finally it was modified again for the row-seg-buffer
//So trying to make it more flexible, again...
//#define FB_TETRIS	TRUE


//also a/o v69... much later:
//HexColor hasn't been used for a while, either...
// Let's get-er-done
//HexColor merely shows the hex-value of a framebuffer color in that color
// Each color is represented by an 8-bit value, of which only 6 correspond
// to the color. Bits 5:4 = Blue, 3:2 = Green, 1:0 = Red
// Technically bits 7:6 represent an alpha-value (or translucency), but
// that's not well-implemented... anywhere (maybe in FB_SMILEY)
// On the Tiny861 blue only has 3 separate shades, rather than 4... so the
// total number of colors is 48 (rather than 64)
// And I often forget how that's implemented (is 0x20 == 0x30 or 0x10,
// or...? This can help sort that out.
// On the PWM161 we have 64 colors! That's right!
// (If you're really keen on colors, there're some experimental code files
//  burried in here regarding how to squeeze more out of the system...
//  they're not very well implemented, if at all.
//  One *really easy* method is to just stick with the 64 colors already 
//  available, and alternate the rows between nearby colors to "dither."
//  Another method is electrical: invert the FPD-Link output signals to get
//  different color-palettes... etc. This method, and many others, generate
//  very specific color-palettes but have a nice gradient-effect. E.G. a
//  lot of browns/tans/coppers with inverted Red/Green signals.)
// By default, the 64/48 color palette shown here stretches pretty evenly
// across the spectrum from black to white.
//#define FB_HEXCOLOR TRUE





#elif (defined(PWM_TESTING) && PWM_TESTING)



#elif (defined(ROWBUFFER_TESTING) && ROWBUFFER_TESTING)
//The row-buffer method increases the resolution dramatically, when
//compared to the frame-buffer method... all that memory which was used for
//a 2D image can now be dedicated to a single row! (e.g. a 16x16 frame
//buffer method requires 256 Bytes of RAM, a 24x24 frame buffer requires
//576 bytes (more than the ATTiny861 has available).
//
//Instead, calculate each row immediately before drawing it (during the
//LCD's "Horizontal Front Porch").
// Thus, that i.e. 576 bytes originally used for an entire frame can now be
// dedicated to each row. Further, since each row on the display itself has
// to be drawn individually, it can increase the vertical resolution to 
// that of the display (768, in my case).
//
//The "row-buffer" method is somewhat poorly-named.
// It might be more appropriate to call it a "row-settings buffer", or
// something similar, as each byte is not an RGB color-value (as in frame
// buffer) but a packed representation of the actual register-values that
// generate the desired color. This reduces calculations during the actual
// row-drawing process, allowing the fewest number of calculations per
// "pixel" by pushing the vast majority of calculations *before* the 
// drawing process.
// Fewer calculations per pixel => skinnier pixels => more pixels can be
// squished in horizontally.
// 
//BUT: Since this requires recalculating *each row* *each time it's drawn*
// we start to run into various limitations:
//   The refresh-rate is limited by all that calculation time.
//   Some displays will not sync with long delays between rows.
//   Some displays seem to require *stable* delays between rows
//      so even if some rows require very few cycles to calculate, the
//      delay between each row needs to be kept somewhat constant
//      (time for another timer and/or repurpose of the hsync timer?)
//      Further, visually it's more comfortable (?) if the refresh doesn't 
//      vary in speed on each row (due to varying calculation times).
//
//As it stands, only the LTN display has been tested with the row-buffer
// It has been found quite stable. The Chi Mei display was tested with the
// Row-Segment-Buffer, and found incompatible, so likely would be with the
// row-buffer as well. Though further experimenting would be worthwhile,
// that display has since been made-use-of rather permanently.

//So, where's this leave us...?
// The resolution, using row-buffer, is not as much limited by memory, as
// it is by the actual number of cpu instructions necessary to load the
// registers from the packed data vs. the display's pixel-clock (the LVDS
// prescaler). There are various ways to optimize it based on needs: e.g.
// with the PWM161's 1K of RAM, value-packing may not be necessary, then
// unpacking isn't either, making each pixel require even fewer CPU
// cycles. But doing so requires three bytes per pixel (three times the
// memory!)
//
// FOR NOW: value-packing is de-facto, as this was developed with only 512B
// of RAM.
//
// Also, changing the lvds-prescaler could increase resolution at the cost
// of refresh-speed, if the display can handle slower rates.
// 
//
//a/o v70, the rowBuffer hasn't been implemented for quite some time, and
//not at all on the PWM161, so there's some work ahead.
// Getting there...
//
//More contemplations:
// If a project is designed with drawRow() functions, rather than
// drawFrame, it's easy enough to port either way, as can be seen in
// tetStuff, etc.
// e.g. a drawRow function might contain:
//   void drawRow(uint16_t rowNum, uint8_t *rowBuffer) {
//     for(i=0; i<ROW_WIDTH; i++) rowBuffer[i] = _W; // (white)
//   }
//
// using frameBuffer is as simple as having a function to loop through all
// the rows calling drawRow as:
//    drawRow(rowNum, frameBuffer[rowNum])
// Note that frameBuffer is two dimensional, but given to drawRow with a
// specific row-index, so appears to drawRow as a rowBuffer.
//
// The only other change is to set rowBuffer[i] = fb_to_rb(_W);
//   instead of merely rowBuffer[i] = _W;
//
// fb_to_rb converts a frameBuffer color value (RGB) to a packed byte to
// store in the row(settings)buffer
//
//


#define ROW_BUFFER TRUE

//These can be varied for testing...
#define LVDS_PRESCALER 1
#define LVDS_PRESCALER_ERROR_OVERRIDE TRUE

//Interesting... this seems to be problematic now.
#define ROW_CALCULATION_CYCS 0 //4000

//Try out the 




//#else	//NOT BLUE_TESTING, FRAMEBUFFER_TESTING, or PWM_TESTING
		//(SEG_Whatevers)
#elif(defined(ROWSEGBUFFER_TESTING) && ROWSEGBUFFER_TESTING)
#define ROW_SEG_BUFFER TRUE
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
#define SEG_SINE TRUE

#if(defined(SEG_SINE) && SEG_SINE)
#define ROW_CALCULATION_CYCS 40000UL
#define USE_ADC TRUE
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
#define USE_ADC TRUE
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





#endif //__MAIN_CONFIG_H__


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
 * /Users/meh/_avrProjects/LCDdirectLVDS/90-reGitting/mainConfig.h
 *
 *    (Wow, that's a lot longer than I'd hoped).
 *
 *    Enjoy!
 */

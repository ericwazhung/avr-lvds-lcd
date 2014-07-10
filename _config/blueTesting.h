/* mehPL:
 *    This is Open Source, but NOT GPL. I call it mehPL.
 *    I'm not too fond of long licenses at the top of the file.
 *    Please see the bottom.
 *    Enjoy!
 */




//This file is to be #included in <project>/mainConfig.h when BLUE_TESTING
//is true...

//Modify (comment/uncomment) options here as you wish.

#ifndef __BLUE_TESTING_H__
#define __BLUE_TESTING_H__



//This file is used for configuring the project at a high-level...
// Most configuration-options are simplified, here, by uncommenting various
// options.
// The basic idea, with a new/untested display is:
//   in mainConfig.h: select BLUE_TESTING=TRUE
//   here: select DE_BLUE=TRUE (all remaining = FALSE/commented-out)
//   These "BLUE TESTS" are simple enough that they should work with nearly
//   any display (possibly given a bit of fine-tuning).
//
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
//    in mainConfig: comment-out BLUE_TESTING 
//                   select FRAMEBUFFER_TESTING = TRUE
//                   and visit frameBufferTesting.h



// NOTE RE: BLUE TESTS and displays lacking DataEnable:
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









//START HERE. 

//This should be uncommented *in mainConfig.h*
// //Set this true, then see the explanations below.
// //#define BLUE_TESTING	TRUE











//This #if is now handled by the conditional #inclusion of this file from
//mainConfig.h...
//#if (defined(BLUE_TESTING) && BLUE_TESTING)




//These are defaults that will probably be overridden in the corresponding
//_TESTING tests, below.
//These strip it down to basically the bare-minimum for the highest
//refresh-rate and bit-rate possible with this system... 
//(Which basically tests whether the display can even possibly work with
//the limitations of this system)
//Just ignore these for now.
#define EXTERNAL_DRAWPIX	TRUE
//TODO: This should probably be changed such that ROW_SEG_BUFFER is only
//paid attention to when it's TRUE... as it stands (a/o v67) it must be
//explicitly either TRUE or FALSE...
#define ROW_SEG_BUFFER FALSE
#define LVDS_PRESCALER 1
//This needs to be TRUE for LVDS_PRESCALER == 1
#define LVDS_PRESCALER_ERROR_OVERRIDE TRUE
#define ROW_CALCULATION_CYCS 0










//START HERE (continued):
//When starting with a new (untested) display, you're best-off starting
//with one of these "BLUE" options
// Benefits are many. They most-closely match the timing values set-up for
// the display by its configuration file in _displays/ 
// (Porch/Sync-widths, etc.)
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
// green, code-wise it would be referred-to as "black.")
//
// Note that the timing-values are *not* exact, there's a bit more
// explanation in BLUE_BORDER, below... So, if you have trouble, maybe try
// fudging some timing-values...




//############################
//##  BLUE_TESTING OPTIONS  ##
//##  (uncomment ONE)       ##
//############################


// DE_BLUE displays solid blue on the screen
// This gives a pretty good idea whether the display is even capable of
// syncing. If you see *any* solid blue, (offset, or otherwise) then it's
// quite likely this display can be synced with a little experimentation.
// If it's solid blue and nothing else, then you lucked out!
#define DE_BLUE TRUE





// BLUE_VERT_BAR, BLUE_VERT_BAR_REVERSED, and BLUE_BORDER display "black" 
// (or green) and "blue" in vertical columns... 
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
// (These issues are not as relevant in bit-banged configurations...)
//
// Also, an odd experience: reflashing from 10 to 341 led to no syncing at
// all, until powered-down and powered-up. Dunno, maybe the MISO/MOSI pins
// were sending stray data to the display which confused it.
// (note a/o v90: How can DE_ACTIVE_DOTS be referred-to here, if it's not
//  yet defined, in the #inclusion-hierarchy? TODO: Retest this.)
//#define BLUE_BORDER (DE_ACTIVE_DOTS/4) //341//10//20



// BLUE_DIAG_BAR displays two triangles, essentially, of blue, and black.
// This is where things get iffy... I've seen some amazingly weird
// patterns with this one; one time I swear ancient egyptians were trying
// to communicate through time by transmitting their hieroglyphs to my
// display. Really, it's just supposed to be the screen divided diagonally
// from upper-right toward lower-left, the top half "black" and the 
// bottom-half "blue".
// a/o v80: Top-half and bottom-half have been flipped, so the actual
// data-starting-point can be seen for syncing. (top/left half is blue)
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
//  (I've since switched-over that display to bit-banged pixel-clocks, so
//   it aligns properly, regardless of the number of CPU instructions
//   necessary to calculate the diagonal... This is not possible with LVDS,
//   so technically, the DE-Active signal may arrive quite a few dot-clocks
//   later than set-up in the display's configuration file. Again, this
//   hasn't been a problem with most displays I've used. Additionally,
//   there's been a bit of effort toward moving calculations to a
//   less-timing-specific section (e.g. *before* Hsync, rather than after),
//   that's an option that' not yet been well-implemented).
//#define BLUE_DIAG_BAR TRUE









//########################################################################
// If you've gotten it going with all the above tests, then you're well on
// your way to doing some pretty groovy things. 
// Check out lcdStuff.c and create your own "drawPix" functions based on
// those in lcdStuff, or try commenting-out BLUE_TESTING, in mainConfig.h
// and continue with FRAMEBUFFER_TESTING, in mainConfig.h and
// _config/frameBufferTesting.h
// (Note, there are more BLUE_TESTS below, but their functionality is not
//  necessary to move on to FRAMEBUFFER_TESTING)
//########################################################################















//A new test (developed for this ChiMei invisible-refresh display)
// BLUE_DIAG_BAR_SCROLL just takes the above idea, and scrolls it
// vertically, one row per refresh.
// 
// ChiMei note:
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
// BLUE_DIAG_SCROLL_FLASH, above.
//#define BDSF_ALTERNATE_ROWS TRUE





//New a/o pwm161 (v67):
// BLUE_AND_COLORS does quick-testing for the other colors as well
// It should display black, red, blue, and cyan in diagonal "wedges"
// I should probably move its code to _commonCode.../lcdStuff
// but for now it's in main.c
// THIS HAS NOT BEEN TESTED on the Tiny861, but should be implemented...
//a/o v80: This displays ONLY BLACK on the Sony parallel-interfaced
//display, likely due to the number of calculations necessary before the
//drawing is started. (This note, likely before bit-banged pixel-clock)
//#define BLUE_AND_COLORS	TRUE
















//New a/o pwm161 (v67):
// These are not-well-implemented, and needn't be running in order to
// move-on to FRAMEBUFFER_TESTING...

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

//#define ALLSHADES_GRADIENT TRUE
//ALLSHADES_AND_COLORS adds Red, Green, and Yellow horizontal bars
// to ALLSHADES_GRADIENT (which must be true, for this to also be)
//The name's a bit misleading, it's not "All Shades-and-colors" it's 
// "All-Shades and colors"
//#define ALLSHADES_AND_COLORS	TRUE






//Yet Another New Test (prior to v67)
// BLUE_ADC reads the ADC value before drawing each row...
// The value read determines the horizontal position of the color-change
// thus, it's a very simple Oscilloscope!
//This is pretty much identical to most of the BLUE_TESTING in lcdStuff.c
// but, it throws some stuff between Hsync and the pixel-data, Thus: The
// horizontal-back-porch is extended during this calculation/measurement
// time... The amount may vary, so if the display requires a stable HBP,
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




//#########################################################################
//### THAT'S IT for BLUE_TESTING! Maybe move on to FRAMEBUFFER_TESTING? ###
//#########################################################################









//Just leave this bit alone...
#if(defined(BLUE_ADC) && BLUE_ADC)
 #undef EXTERNAL_DRAWPIX
 #define USE_ADC TRUE
#endif

//Just leave this bit alone...
#if(defined(BLUE_ALLSHADES) && BLUE_ALLSHADES)
 #undef EXTERNAL_DRAWPIX
 #ifndef __AVR_AT90PWM161__
  #error "BLUE_ALLSHADES only works with the AT90PWM161, so far..."
 #endif
#endif

//Just leave this bit alone...
#if(defined(BLUE_AND_COLORS) && BLUE_AND_COLORS)
 #undef EXTERNAL_DRAWPIX
#endif

#endif //__BLUE_TESTING_H__


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

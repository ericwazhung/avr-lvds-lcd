/* mehPL:
 *    This is Open Source, but NOT GPL. I call it mehPL.
 *    I'm not too fond of long licenses at the top of the file.
 *    Please see the bottom.
 *    Enjoy!
 */






//This file is to be #included in <project>/mainConfig.h 
// when ROWSEGBUFFER_TESTING is true...

//Modify (comment/uncomment) options here as you wish.

#ifndef __ROWSEGBUFFER_TESTING_H__
#define __ROWSEGBUFFER_TESTING_H__



//This file is used for configuring the project at a high-level...
// Most configuration-options are simplified, here, by uncommenting various
// options.
//
// RowSegBuffer functions by calculating each row during the
// horizontal-front-porch, similar to rowBuffer.
// Additionally, the resolution is increased by using "row-segments."
// Rather than storing a buffer full of individual pixels, it instead
// stores a buffer containing a color-value and a number of pixels to
// stretch that color across.
// At one point, using this method, I was able to get ~680 pixels of
// resolution (stretched across a 1024pixel display) with only 512Bytes of
// available RAM... Where, here, "resolution" refers to the precision of
// the rowSegBuffer method; e.g. there's no way there was enough memory for
// 680 color-changes, but any of e.g. 200 color-changes could be positioned
// anywhere within 680 columns on the screen.

// Like rowBuffer:
// This method may not work with most displays... So far it's only been
// tested-functional with the Samsung LTN...
//
// The problem is, quite a bit of processing is necessary to calculate
// each row, and those calculations must be done during *each* row.
// Some displays don't particularly like long horizontal-blanking periods.
// Others don't like when that blanking-period varies from row-to-row
// (as it would when one row is easier to calculate than the next)
// Additionally, it reduces the refresh-rate *dramatically*
// 
// a/o v90:
// CURRENTLY: there's really no testing-method to see about a display's
// compatibility with row[Seg]Buffer... it's kinda all-or-nothing.
// FURTHER: It hasn't been developed for quite some time, nor even
// tested... At one point it was *heavily* developed, though, so the code
// exists, and may be worth experimenting with.







//START HERE.


//This should be an option in mainConfig.h...
//Do not uncomment it here. It's left here purely for 
//documentation-purposes...
// //#define ROWSEGBUFFER_TESTING	TRUE

#define LVDS_PRESCALER 8

//THE REMAINDER IS STRAIGHT FROM mainConfig.h:

#define ROW_SEG_BUFFER TRUE
//Re the next two settings: (a/o v62)
//This is a quick-attempt at getting the ChiMei display working with
//SEG_Watevers, below...
//The ChiMei display seems to need higher bit-rates than the other display,
//so remove the prescaler...
//#define LVDS_PRESCALER 1
//Some math relies on a higher LVDS_PRESCALER value... There's an #error
//regarding this, but it can be ignored in some cases... 
// so let's ignore it.
//#define LVDS_PRESCALER_ERROR_OVERRIDE TRUE
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
//#define SEG_LINE TRUE

#if(defined(SEG_LINE) && SEG_LINE)

#define ROW_CALCULATION_CYCS 0UL //200000//(8*DISPLAY_CYC_COUNT)//(1024*8) 
//0UL //40000UL
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
//a/o v95 this is no longer functional.
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

//a/o v95, SEG_TET is no longer functional
//#define SEG_TET TRUE
// Look into SEG_TET case in loadRow for some configurables
// (transparency overlay, etc).

//Tetris was written for the row-buffer
// and uses conversion...
// it's a bit wasteful, since it requires *both* a rowBuffer AND a
// rowSegBuffer, but it should work.
#if(defined(SEG_TET) && SEG_TET)
 #define ROW_BUFFER  TRUE
 #define NUM_SEGMENTS   96 //128//RB_WIDTH+10
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
 #define ROW_CALCULATION_CYCS 40000UL
#endif





#endif //__ROWSEGBUFFER_TESTING_H__


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
 * /Users/meh/_avrProjects/LCDdirectLVDS/93-checkingProcessAgain/_config/rowSegBufferTesting.h
 *
 *    (Wow, that's a lot longer than I'd hoped).
 *
 *    Enjoy!
 */

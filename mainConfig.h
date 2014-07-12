/* mehPL:
 *    This is Open Source, but NOT GPL. I call it mehPL.
 *    I'm not too fond of long licenses at the top of the file.
 *    Please see the bottom.
 *    Enjoy!
 */







#ifndef __MAIN_CONFIG_H__
#define __MAIN_CONFIG_H__


//########################
//###  OVERVIEW:       ###
//########################

//This file is used for configuring the project at a high-level...
// Most configuration options are simplified (here and in the various files
// under _config/) by uncommenting various options.
//
// Basic Options:
//  * Display 
//    (timing-parameters defined in _displays/*)
//
// (* Interface                 )
// (  * LVDS/FPD-Link  (DEFAULT))
// (  * 6-bit parallel          )
// (  defined in _interfaces/*  )
//
//  * Drawing Mode
//    (See the corresponding file for more configuration-options)
//    * BLUE_TESTING				(see: _config/blueTesting.h)
//    * FRAMEBUFFER_TESTING	(see: _config/frameBufferTesting.h)
//    * ROWBUFFER_TESTING		(see: _config/rowBufferTesting.h)
//    * ROWSEGBUFFER_TESTING	(see: _config/rowSegBufferTesting.h)






//Best to just leave this alone...

//v91, notes to self:
//Thought the multiplication in the .lss was coming from delay_cyc
// but it's coming from delay_Dots(dots) -> delay_cyc(dots*7/4) ish
//v92 (copying v66.51-64 notes):
//This seems to have the same effect... the slope is wrong
// are lines being doubled?
// Regardless, FALSE seems to shift the diagonal right a bit (GCC4.8)
//LATER:
// Setting this TRUE helps with first few rows of DIAG_BAR, which were
// random-length...
//v92:
// The end-result is TRUE is ideal, at least with DIAG_BAR(_SCROLL)
#define DELAY_CYC_DELAY_LOOP TRUE



//##################################################
//##################################################
//###   THIS BIT IS IN EARLY-TESTING             ###
//###   Unless using a Sony ACX705AKM            ###
//###   or similar (parallel-interfaced)         ###
//###   display, just comment out the next line. ###
//###   (This overrides _LCD_SPECIFIC_HEADER_    ###
//###    options, as selected, below)            ###
//##################################################
//#define SONY_TESTING_CONFIG TRUE


#if (!defined(SONY_TESTING_CONFIG) || !SONY_TESTING_CONFIG)
//################################################
//### Select a display                         ###
//### Uncomment *one* of the following         ###
//### (or add your own)                        ###
//###                                          ###
//### Leave all commented-out to use           ###
//### _displays/testDisplay.h                  ###
//###                                          ###
//### (See _displays/displayNotes.txt)         ###
//################################################

//For the most part,
//These _displays/ files simply contain timing-values
// E.G. Width/Height, Hsync-width, etc. in pixels
// (These values aren't necessarily those on the data-sheets!)

//Samsung LTN121X1:
// 1024x768, 12.1in
// There're a few options for this display... I'll try to consolidate
// them, soon. Until then, select one... If it doesn't work, try another.
//Try _v6651 first, it's the last-tested
#define _LCD_SPECIFIC_HEADER_   "_displays/samsungLTN121X1_v6651.h"
//#define _LCD_SPECIFIC_HEADER_   "_displays/samsungLTN121X1.h"
//#define _LCD_SPECIFIC_HEADER_   "_displays/samsungNogo.h"

//Boe Hydis HD12X21:
// 1024x768, 12.1in
//#define _LCD_SPECIFIC_HEADER_   "_displays/boeHydisHT12X21.h"

//Sony ACX705AKM:
// 240x160, ~2in (parallel-interfaced)
//AS OF v92: DON'T SELECT THIS HERE, Instead select SONY_TESTING_CONFIG
// above.
//#define _LCD_SPECIFIC_HEADER_		"_displays/sonyACX705AKM.h"

//"Test Display":
// This was once the default for most 1024x768 displays...
// It's left here for easy manipulation for your own needs
// But, it's better if you create your own (copy the file, as a
// starting-point?)
// (If _LCD_SPECIFIC_HEADER_ is not defined, it defaults to this)
//#define _LCD_SPECIFIC_HEADER_		"_displays/testDisplay.h"

//ALTERNATIVELY:
//Add your own file to _displays/ and add it here:
//#define _LCD_SPECIFIC_HEADER_		"_displays/YOUR_FILE_HERE.h"



#endif //NOT SONY_TESTING_CONFIG, using a manual-display-selection





//This file is used for configuring the project at a high-level...
// Most configuration-options are simplified, here, by uncommenting various
// options.
// The basic idea, with a new/untested display is:
//   in mainConfig.h: uncomment the line '#define BLUE_TESTING TRUE'
//                    (make sure other _TESTINGs are commented-out)
//   in _config/blueTesting.h: uncomment the line '#define DE_BLUE TRUE'
//										 (all remaining = FALSE/commented-out)
//
//   If the display shows *anything* blue, then it's quite likely the
//    display can be used with this system with a little tweaking of
//    timing-values, etc. found in _displays/
//
//    If it shows completely blank (black or white) then you might be out 
//    of luck with this particular display... or maybe there's an
//    electrical or other problem (check those LVDS voltages)?
//    Don't select any of the other options until you can get DE_BLUE 
//    working (see troubleshooting.txt, maybe)
//
//   Once DE_BLUE is working, comment it out, and select BLUE_VERT_BAR...
//   Continue down the line...
//
//   Once all the important "BLUE" tests are running 
//    quite a bit can be done by just manipulating the "BLUE" code...
//
//   OR:
//    in mainConfig.h: comment-out BLUE_TESTING 
//                     and uncomment FRAMEBUFFER_TESTING
//    and look into _config/frameBufferTesting.h
//
//   For higher resolution (at the cost of refresh-rate) look into
//   "ROWBUFFER_TESTING" and "ROWSEGBUFFER_TESTING"
//   (Note that these methods don't work with all displays)

// OLD NOTES, need to be reorganized:
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





//#### IGNORE THESE NOTES, THEY NEED TO BE UPDATED  ####
//THIS IS OLD... Actually RSB hasn't been implemented for a while...
// I've gone back to frame-buffer for most new development, as it works on
// nearly all displays without a hitch.
//THIS IS OLD:
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
//#### TO HERE ####









//####################################################################
//###  Uncomment ONE xxx_TESTING option below                      ###
//###  And view its corresponding configuration file in _config/   ###
//####################################################################




//PWM_TESTING is for testing a new (unimplemented) chip
// could also be useful for testing an implemented chip, since we're really
// pushing the chip's limits...
//For now, this just sets a specific and unchanging PWM signal, for
//'scoping.
//(This test probably needn't be run, in most cases)
//#define PWM_TESTING	TRUE





//1) Set this true, then see _config/blueTesting.h
//BLUE_TESTING is pretty much the simplest signal that can be sent, so it's
//likely to work with a display, if the display is capable of being
//worked-with. START HERE.
#define BLUE_TESTING	TRUE



//2) Later, comment-out the above, and try this one, and see
//_config/frameBufferTesting.h
//If BLUE_TESTING works, then FRAMEBUFFER_TESTING likely will, as well...
//This gives a (usually) 16x16 framebuffer stretched across the entire
//screen.
//#define FRAMEBUFFER_TESTING	TRUE



//3) OPTIONAL
// RowBuffer takes the same 16x16=256 memory-locations and applies them to
// *each row* on the display, dramatically increasing resolution at the
// cost of refresh-rate and compatibility (some displays can't handle the
// nasty timing-signals created by this method).
//a/0 v70:
// RowBuffer hasn't been implemented in some time...
// Time to do it!
//a/o v93:
// This actually does something still with the LTN (and possibly most LVDS
// displays!) But, NYI for the parallel-interfaced.
// Currently getting 64 horizontal "pixels" by 384 rows (Rows must be
// doubling due to the LTN's infamous doubled-row thing, right?)
// Anyways, it's under-developed, but certainly has potential.
// Basically as soon as I got it working, I moved on to rowSegBuffer
//#define ROWBUFFER_TESTING	TRUE


//4) OPTIONAL
// RowSegBuffer uses a similar method as RowBuffer to increase resolution
// and extend the memory as far as possible. 
// At the highest-resolution (slowest refresh-rate) e.g. a rowSegment can 
// start at any column (of ~680 stretched across a 1024x768 display) and 
// end at any column... but the number of such segments is limited by
// memory, to around 200.
// Each row, on the display, is handled this way, so the smallest "pixel"
// that can be resolved as a different color from its surrounding pixels
// is 1/680th of a screen-width, and 1/768th of a screen-height.
//a/o v90, rowSegBuffer has yet to be tested on any display other than the
//Samsung LTN, and it's been quite a while...
//a/o v93, whoa, this code actually still compiles, and it sorta displays
//something still on the LTN... To Be Revisited.
//#define ROWSEGBUFFER_TESTING TRUE













//#########################################################################
//### Pretty much everything from here down can be ignored...
//### (And could *really* stand to be cleaned up a bit)
//### But don't forget to look into the selected _config/...Testing.h file.
//#########################################################################





//Old note left here for my TODO: Check this note's continued relevence...
//a/o v69:
//Comment them both (and PWM_TESTING) out for SEG_whatevers...
// This should probably become SEG_TESTING or something.
//Actually, that's not quite right... we're still missing RowBuffer
//testing!











#if(defined(BLUE_TESTING) && BLUE_TESTING)
#include "_config/blueTesting.h"
#elif(defined(FRAMEBUFFER_TESTING) && FRAMEBUFFER_TESTING)
#include "_config/frameBufferTesting.h"
#elif(defined(ROWBUFFER_TESTING) && ROWBUFFER_TESTING)
#include "_config/rowBufferTesting.h"
#elif(defined(ROWSEGBUFFER_TESTING) && ROWSEGBUFFER_TESTING)
#include "_config/rowSegBufferTesting.h"
#elif(defined(PWM_TESTING) && PWM_TESTING)
 //Nothing to do here... but we need the else-case...
#else
 #error "Must select an xxx_TESTING option"
#endif








//These are defaults that will probably be overridden in the corresponding
//_TESTING tests, below.
//TODO: This should probably be changed such that ROW_SEG_BUFFER is only
//paid attention to when it's TRUE... as it stands (a/o v67) it must be
//explicitly either TRUE or FALSE...
#ifndef ROW_SEG_BUFFER
#define ROW_SEG_BUFFER FALSE
#endif
//This'll probably get changed during testing...
#ifndef LVDS_PRESCALER
#define LVDS_PRESCALER 1
#endif
//This needs to be TRUE for LVDS_PRESCALER == 1
#ifndef LVDS_PRESCALER_ERROR_OVERRIDE
#define LVDS_PRESCALER_ERROR_OVERRIDE TRUE
#endif





/*
#if (defined(BLUE_TESTING) && BLUE_TESTING)
 //It's all been moved to _config/blueTesting.h...
#elif (defined(FRAMEBUFFER_TESTING) && FRAMEBUFFER_TESTING)
 //It's all been moved to _config/frameBufferTesting.h...
#elif (defined(PWM_TESTING) && PWM_TESTING)
 //Nothing Necessary, here...
#elif (defined(ROWBUFFER_TESTING) && ROWBUFFER_TESTING)
 //Moved to _config/rowBufferTesting.h... it's not well-implemented.
#elif(defined(ROWSEGBUFFER_TESTING) && ROWSEGBUFFER_TESTING)
 //Moved to _config/rowSegBufferTesting.h... it's not well-documented.
#endif //BLUE_TESTING and SEG_whatevers...
*/


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











//FROM HERE ON:
//This bit can be ignored...

#if(defined(SONY_TESTING_CONFIG) && SONY_TESTING_CONFIG)
 #include "_config/sonyTestingConfig.h"
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
// These can be left undefined, as defaults are fine for most
// displays I've worked with... see the DEFAULTS, below.
// Though, if you need to change testDisplay.h for your own timings, it'd
// be better to copy it and #define _LCD_SPECIFIC_HEADER_ as appropriate...

//THESE ARE THE DEFAULTS FOR:
// 1024x768 display via LVDS/FPD-Link Interface
// To select another display/interface and override these defaults
//  add your own #defines above
#ifndef _LCD_SPECIFIC_HEADER_
 #define _LCD_SPECIFIC_HEADER_	"_displays/testDisplay.h"
#endif
#ifndef _LCD_INTERFACE_CFILE_
 #if (defined(__AVR_AT90PWM161__))
  #define _LCD_INTERFACE_HEADER_ "interfaces/lvds161.h"
  #define _LCD_INTERFACE_CFILE_ "_interfaces/lvds161.c"
 #else	//Assumed ATtiny861:
  #define _LCD_INTERFACE_HEADER_ "_interfaces/lvds.h"
  #define _LCD_INTERFACE_CFILE_ "_interfaces/lvds.c"
 #endif
#endif //_LCD_INTERFACE_CFILE_ not previously defined










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
 * /Users/meh/_avrProjects/LCDdirectLVDS/93-checkingProcessAgain/mainConfig.h
 *
 *    (Wow, that's a lot longer than I'd hoped).
 *
 *    Enjoy!
 */

/* mehPL:
 *    This is Open Source, but NOT GPL. I call it mehPL.
 *    I'm not too fond of long licenses at the top of the file.
 *    Please see the bottom.
 *    Enjoy!
 */






//This file is to be #included in <project>/mainConfig.h 
// when FRAMEBUFFER_TESTING is true...

//Modify (comment/uncomment) options here as you wish.

#ifndef __FRAMEBUFFER_TESTING_H__
#define __FRAMEBUFFER_TESTING_H__



//This file is used for configuring the project at a high-level...
// Most configuration-options are simplified, here, by uncommenting various
// options.
// The basic idea, with a new/untested display is:
//   FIRST: Try BLUE_TESTING (in mainConfig.h)
//   Once all the important "BLUE" tests are running, 
//    quite a bit can be done by just manipulating the "BLUE" code...
//
//   THEN (optionally):
//    in mainConfig: comment-out BLUE_TESTING 
//                   and select FRAMEBUFFER_TESTING = TRUE
//    There are several FB_xxx tests, but the most-well-developed is 
//    FB_QUESTION
//    Unfortunately, it's pretty complex as far as a starting-point...
//    If that works, you can do quite a bit with the display, already, at
//     low-resolution and basically the highest refresh-rate possible with
//     this system.
//    Other FB-based code exists, but hasn't been (re)implemented for use
//     in mainConfig.h. e.g. smiley.c, tetStuff.c, lifeStuff.c,
//     colorBarScroll.c (as I recall...)
//    a/o v90: These other FB_xxx tests HAVE been tested since the above
//    note... but it's been a minute.
//
//   LATER (optionally):
//    If you want higher resolution (at the cost of refresh-rate)
//    choose ROWBUFFER_TESTING in mainConfig, and view the corresponding
//    file...

// a/o v90: FB_QUESTION has been *heavily* developed recently... 
//          It should work with all display/chip/interface configurations
//          currently-implemented.




//START HERE.


//This should be an option in mainConfig.h...
//Do not uncomment it here. It's left here purely for 
//documentation-purposes...
// //#define FRAMEBUFFER_TESTING	TRUE




//###################################
//##  Uncomment ONE FB_xxx option  ##
//###################################


//FB_SMILEY loads a stationary smiley-face image into the frame-buffer 
// and displays it... (Actually, it may no longer be stationary)
// It's been a while since I've tested this, but it should be the simplest
// frame-buffer-based code...
//#define FB_SMILEY	TRUE




//FB_HEXCOLOR shows the hex-value of a frameBuffer color in that color
//HexColor hasn't been used for a while, either...
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





//FB_TETRIS is, yep, the game Tetris (Props to the original author)
// But it doesn't have a controller-interface (yet), so it's just random
// pieces falling randomly.
// It uses the frameBuffer, and stretches the memory to do-so (16x16 wasn't
// enough)
//a/o v69:
//Tetris hasn't been used since the ol' "SEG_TET" days long ago...
// But originally it was written for the frame-buffer
// and then it was modified for the row-buffer
// and finally it was modified again for the row-seg-buffer
//So trying to make it more flexible, again...
//a/o v90: FB_TETRIS was reimplemented at one point, so it may well run,
//now...
//a/o v93: It doesn't work on the sony-display, and requires too much
//program-memory for the LVDS-displays
//#define FB_TETRIS	TRUE



//FB_QUESTION displays a "Mario Question Box" which, when hit, gives a
//reward... it makes heavy use of sprites, sprite-motions, etc. All on a
//16x16 frame-buffer.
// (A better description might be found in SEG_QUESTION, see
// _config/rowSegBufferTesting.h)
//
// Note there are SEVERAL options for testing FB_QUESTION
// Many of these can be found later in this file, others in fb_question.c
// It's entirely plausible that prior to uploading, I might've left one of
// these options enabled that shouldn't be default...
// e.g. "AUTO_HIT" or random-override.
// Again, many options are later in this file...
//
// ALSO optionally comment/uncomment MARIO3, below
// (If it's too big, make sure to comment-out MARIO3)
#define FB_QUESTION	TRUE

#if(defined(FB_QUESTION) && FB_QUESTION)
//MARIO3 enables BOTH sprites from Super Mario Bros AND Super Mario Bros 3
// (The question-box randomly switches between v3 and v1, and rewards the
// appropriate version's sprites)
// MARIO3 increases codesize somewhat dramatically, so if you're running
// out of space, leave this commented-out...
#define MARIO3	TRUE
#endif






//#####################################
//## DEFAULTS, Don't Change these... ##
//#####################################

//These are defaults that will probably be overridden in the corresponding
//FB_xxx tests, selected above.
//These strip it down to basically the bare-minimum for the highest
//refresh-rate and bit-rate possible with this system... 
//(Which basically tests whether the display can even possibly work with
//the limitations of this system)
//Just ignore these for now.

//a/o v62:
//Not sure why this isn't working in this case...
//Some rows "wobble" with this TRUE... it looks better if this is FALSE...
// (Still jitters, but less noticeable)
// Is this a ChiMei thing, or bad math somewhere...?
//#define ALIGN_TIMER_WITH_PIXCLK FALSE

//TODO: This should probably be changed such that ROW_SEG_BUFFER is only
//paid attention to when it's TRUE... as it stands (a/o v67) it must be
//explicitly either TRUE or FALSE...
#define ROW_SEG_BUFFER FALSE
//My delay-loop should be more precise *when its argument is constant*
// Here it's used for stretching the framebuffer pixels horizontally
//a/o v92: See mainConfig.h re: v66.51-64 copy-over...
//#define DELAY_CYC_DELAY_LOOP FALSE

#define ROW_CALCULATION_CYCS 0
//This shouldn't be necessary, right?
#define ROW_BUFFER FALSE
#define LVDS_PRESCALER 1
//This needs to be TRUE for LVDS_PRESCALER == 1
#define LVDS_PRESCALER_ERROR_OVERRIDE TRUE


#warning "MAIN_MS definitions should probably be moved elsewhere..."
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








//#################################
//## GENERAL FRAMEBUFFER OPTIONS ##
//## (apply to all FB_ tests)    ##
//#################################






//If FB_REFRESH_ON_CHANGE is TRUE, only refresh the LCD when there's a
//change in the image... It works well, but on single-refresh changes, some
//displays leave a ghost of the prior image. Thus, it's been designed to do
//*two* refreshes upon each change, which is nice... but it's not
//sophisticated enough to recognize when an image is about to change again,
//in which case it shouldn't need to refresh-twice if, e.g. the image
//changes often (e.g. a stationary image that cycles through colors)
// In other words, it can be kinda seizure-inducing.
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
// a/o v93: I think the timer-situation has been fixed with fb_timer_t...
//#if (defined(__AVR_AT90PWM161__))


//WITHOUT FB_REFRESH_ON_CHANGE, if the framebuffer-updater function (e.g.
//tet_update() takes too long, there's nothing can be done to avoid the
//image's being only partially-written by the time of the next refresh.
// This *despite* the fact, the framebuffer-updater function is only called
// at the end of a frame.
//
// Alternatively (to avoid partial framebuffer-loading before refreshes)
// set this TRUE and set REFRESH_ON_CHANGE_COUNT 1 and 
// REFRESH_ON_CHANGE_DELAY = 0
// it should be roughly the same, but slightly slower because it pauses
// refresh until after the framebuffer-updater function completes.
#if(defined(FB_HEXCOLOR) && FB_HEXCOLOR)
 #define FB_REFRESH_ON_CHANGE FALSE
#else
 //COMMENT OR UNCOMMENT AS DESIRED:
 //NOTE: most of the FB_xxx tests no longer fully implement refreshOnChange
 // (e.g. FB_QUESTION now handles multiple-overlapping sprites by drawing
 //  each one on top of the other... Thus, there's currently no way to
 //  detect whether a pixel has changed color since the last refresh, 
 //  because it has likely changed colors two or three times in a single 
 //  frame-buffer reload (between two refreshes). Thus, every
 //  framebuffer-reload (which occurs at the end of every refresh) results
 //  in another refresh... The effect is similar to 
 //  REFRESH_ON_CHANGE_COUNT=1 REFRESH_ON_CHANGE_DELAY=0, described above.
//   Ultimately, the change-detection code should be more sophisticated
//   than just detecting whether a pixel has changed... e.g. if the
//   sprite-layout/positions/palettes have changed THEN refresh. TODO)
//   (Except, this TODO doesn't allow for PARTIAL_REFRESH... which isn't
//   really used anyhow)
 #define FB_REFRESH_ON_CHANGE TRUE
#endif



//Number of screen refreshes to perform whenever the framebuffer changes
// (1 usually leaves some ghosting, 2 usually clears that up)
#if(!defined(SONY_TESTING_CONFIG) || !SONY_TESTING_CONFIG)
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





//############################
//##  FB_QUESTION OPTIONS   ##
//##  These can be changed  ##
//##  as necessary....      ##
//############################



#if(defined(FB_QUESTION) && FB_QUESTION)
 #undef ALIGN_TIMER_WITH_PIXCLK
 #define ALIGN_TIMER_WITH_PIXCLK TRUE

 //If you're using a piezo-element for a hit-detector, then uncomment this:
 //(Hasn't been fully reimplemented, it may or may not work)
 //#define PIEZO_HIT_DETECTION TRUE
 #if(defined(PIEZO_HIT_DETECTION) && PIEZO_HIT_DETECTION)
 #define USE_ADC TRUE
 #endif
 
 //BUMP_SWITCH was a method for detecting a bump via, literally, a
 //conductive-ball inside a cage made up of conductive pins... the ball
 //would, at rest, likely short out two contacts. When bumped the ball
 //momentarily breaks contact. The idea's sound, but the conductivity was
 //hokey. The end-result is that instead of detecting whether a pin is
 //pulled-low or released-from-being-pulled-low, it would detect whether
 //there was *any change* (in case, for instance, the ball somehow landed
 //where it *didn't* make contact, but bumping it caused it to do so 
 //briefly)... The end-result is that this same code can be used with a
 //regular switch just as easily, and might well become the default rather
 //than just detecting for a low.
 #define BUMP_SWITCH	TRUE

 //AUTO_HIT is mostly for debugging, but also makes it useful when there's
 //no button...
 // It automatically "hits" after a certain duration
 //#define AUTO_HIT TRUE
#endif








#endif //__FRAMEBUFFER_TESTING_H__


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
 * /Users/meh/_avrProjects/LCDdirectLVDS/93-checkingProcessAgain/_config/frameBufferTesting.h
 *
 *    (Wow, that's a lot longer than I'd hoped).
 *
 *    Enjoy!
 */

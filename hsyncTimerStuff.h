/* mehPL:
 *    This is Open Source, but NOT GPL. I call it mehPL.
 *    I'm not too fond of long licenses at the top of the file.
 *    Please see the bottom.
 *    Enjoy!
 */












// Timer0 is used to synchronize rows...
// Its interrupt is responsible all display-timing. And all drawing is
// handled within it.

#ifndef __HSYNC_TIMER_H__
#define __HSYNC_TIMER_H__

#include "lcdDefines.h"

//included in lcdDefines...
//#include "mainConfig.h"

#include _BITHANDLING_HEADER_

//#include <avr/io.h>
#include _TIMERCOMMON_HEADER_

//These defaults for the ATTiny861
// They can be overridden in makefile with CFLAGS+=-D'...'
#ifndef HSYNC_TIMER_OCR
 #define HSYNC_TIMER_OCR	OCR0A
#endif

#ifndef HSYNC_TIMER_NUM
 #define HSYNC_TIMER_NUM	0
#endif

#ifndef HSYNC_TIMER_MAX
 #error "Whoops, typo...?"
 #define HSYNCT_TIMER_MAX	255
#endif

#ifndef HSYNC_TIMER_INTERRUPT_VECT
 #define HSYNC_TIMER_INTERRUPT_VECT TIMER0_COMPA_vect
#endif

//Each row takes this amount of time:
//T_HD + T_DH + T_low + T_DE
// For the original system, 19.865us = 695 dot-clocks
// We'd need to take advantage of the 16-bit timer
//  OCR1A may not be written properly (with a temporary high-byte)
//  ... not sure.
// TIMER0 DOES NOT HAVE CTC MODE in 16-bit mode
// (So use the prescaler! Though, this limits the resolution...)
#define DISPLAY_CYC_COUNT \
   (T_Hlow_CYC + T_HD_CYC + T_DE_CYC + T_DH_CYC)
// a/o v60:
// Strange effecs using this in ROW_CALCULATION_CYCS with LVDS_PRESCALER=4
// and PLL_SYSCLK
// DOTS_TO_CYC(dots) ((((dots)*(7l)*(LVDS_PRESCALER))/4))
// H_LOW_DOTS = 100 -> 700
// HD_DOTS = 50 -> 350
// DE_ACTIVE_DOTS = 1024 -> 7168
// DH_DOTS = 0 -> 0
//  == 8218
// * 8 = 65,744 CYC





// ROW_CALCULATION_CYCS:
// This is a hack, for testing syncability with long row-calculations
// before actually drawing it... e.g. loading from program memory
// This value needs to be larger than the calculation time
// otherwise, it seems, syncing isn't stable
// BUT THIS IS A HACK, has nothing to do with actual calculation time
// And for slow LVDS or whatnot, this may be unnecessarily HUGE
// (a/o v59-12ish, apparently this value isn't entirely important...?!
//  then why'd I say it affects syncing?!)
// Anyways, the basic effect is to extend the horizontal front porch


//NOTE that with longer ROW_CALCULATION_CYCS, lines can be repeated...
// apparently an effect of the display, definitely not a code-thing
// in which case the vertical resolution decreases
// though, this could be used to advantage... 
// (e.g. more processing time outside the drawing interrupts?)
// NOTE that if the value is too small and calculations take longer in some
// lines than others (?), those lines may repeat, or syncing could be off
// (this too might be somewhat better-understood a/o v59-12ish)

// ROW_CALCULATION_CYCS can also be used as a method to slow the refresh-
// rate...
// And I haven't figured out the logic behind this, but apparently if it's
// too large, the display syncs weirdly (partial-drawings)
// Again, it's strange... I musta been right at the limit, because it only
// presented itself after adding PIX/SYSCLK alignment

// a/o v59-22ish:
// Setting ROW_CALCULATION_CYCS to Zero *does* affect things...
//  Where portions of the screen take longer to calculate (e.g. the "score"
//  in SEG_RACER), the pixel alignment jitters...
//  regardless of ALIGN with PIX/SYS clocks.

// a/o v60:
// Using PLL_SYSCLK and LVDS_PRESCALER=4; should be identical in the
// signal being transmitted, except the bit-rate is doubled.
// There're some odd effects... May-be display-specific, but line-doubling
// is the go-to in nearly all cases... which could be a handy effect to
// explore. More notes in mainConfig: SEG_LINE of all places.
// Also, above. Apparently the ROW_CALCULATION_CYCS is *huge*
// which now makes more sense... I thought it was smaller
// So, if a row takes too long, it's repeated
// kinda handy, really....

// I dunno where I came up with this logic... I think it's completely
// arbitrary:
#ifndef ROW_CALCULATION_CYCS
#if (LVDS_PRESCALER == 8)
 //#define ROW_CALCULATION_DELAY 1//20
 //#define ROW_CALCULATION_CYCS (50000UL) //0 //(70000) //(100000)
 #define ROW_CALCULATION_CYCS 40000UL //10000UL //0UL
#else
 //#define ROW_CALCULATION_DELAY 9//7//5//2//1//10
 #define ROW_CALCULATION_CYCS   (8*DISPLAY_CYC_COUNT)
#endif
#endif


// Also, there's quite a bit of leeway here, as this is based on 
// ___CYC values, which are rounded from dot-values...
#define TOTAL_CYC_COUNT \
      ((0UL + ROW_CALCULATION_CYCS + DISPLAY_CYC_COUNT))



//Attempt to determine the minimum prescaler necessary
#if (!defined(HSYNC_TIMER_CLKDIV) && (TOTAL_CYC_COUNT > HSYNC_TIMER_MAX))
 //Adding +7 causes it to round up (hopefully)
 #define HSYNC_TIMER_TCNTS ((TOTAL_CYC_COUNT+7)/8)
 #define HSYNC_TIMER_CLKDIV   CLKDIV8
 #warning "HSYNC_TIMER_TCNTS = (TOTAL_CYC_COUNT+7)/8: Timer0 CLKDIV8"


 #if (HSYNC_TIMER_TCNTS > HSYNC_TIMER_MAX)
  #warning "HSYNC_TIMER_TCNTS = (TOTAL_CYC_COUNT+63)/64: Timer0 CLKDIV64"
  #define HSYNC_TIMER_TCNTS ((TOTAL_CYC_COUNT+63)/64)
  #define HSYNC_TIMER_CLKDIV CLKDIV64

  #if(defined(ALIGN_TIMER_WITH_PIXCLK) && ALIGN_TIMER_WITH_PIXCLK)
   //Attempting interrupt/pixel alignment...
   // this is where we're at, right now.
   // a/o v59-13ish
   #warning "THIS IS JUST A TEST FOR PIXEL-ALIGNING"
   //#define CLOCKCYCS (HSYNC_TIMER_TCNTS * 64)
   //We know that CLOCKCYCS is divisible by four... (the system clock)
   // so we just need to round to the nearest 7...
	//a/o v62: Where does 4 come from? Is this an LVDS-Prescaler thing?

//   #define ROUND_TO(minVal, roundTo) \ //
//      ((((minVal)+((roundTo)-1))/(roundTo))*(roundTo))

   #define HSYNC_TIMER_TCNTS_A (ROUND_UP_TO(TOTAL_CYC_COUNT, 64*7)/64)


   #if (HSYNC_TIMER_TCNTS_A <= HSYNC_TIMER_MAX)
      #define HSYNC_TIMER_TCNTS ((HSYNC_TIMER_TCNTS_A))
   #else
      #warning "Rounded past HSYNC_TIMER_MAX"
   #endif
  #endif



  #if ((HSYNC_TIMER_TCNTS > HSYNC_TIMER_MAX) || \
		 (defined(ALIGN_TIMER_WITH_PIXCLK) && \
		  		(HSYNC_TIMER_TCNTS_A > HSYNC_TIMER_MAX)))
    #warning "HSYNC_TIMER_TCNTS = (TOTAL_CYC_COUNT+255)/256: Timer0 CLKDIV256"
    #define HSYNC_TIMER_TCNTS ((TOTAL_CYC_COUNT+255)/256)
    #define HSYNC_TIMER_CLKDIV CLKDIV256
   
    #if(defined(ALIGN_TIMER_WITH_PIXCLK) && ALIGN_TIMER_WITH_PIXCLK)
     //#define CLOCKCYCS (HSYNC_TIMER_TCNTS * 256)
     #define HSYNC_TIMER_TCNTS_B (ROUND_UP_TO(TOTAL_CYC_COUNT, 256*7)/256)


     #if (HSYNC_TIMER_TCNTS_B <= HSYNC_TIMER_MAX)
      #define HSYNC_TIMER_TCNTS (HSYNC_TIMER_TCNTS_B)
     #else
      #warning "Rounded past HSYNC_TIMER_MAX"
     #endif
    #endif


    #if ((HSYNC_TIMER_TCNTS > HSYNC_TIMER_MAX) || \
			(defined(ALIGN_TIMER_WITH_PIXCLK) && \
			 		(HSYNC_TIMER_TCNTS_B > HSYNC_TIMER_MAX)))
      #warning "HSYNC_TIMER_TCNTS = (TOTAL_CYC_COUNT+1023)/1024: Timer0 CLKDIV1024"
      #define HSYNC_TIMER_TCNTS ((TOTAL_CYC_COUNT+1023)/1024)
      #define HSYNC_TIMER_CLKDIV   CLKDIV1024

      #if (defined(ALIGN_TIMER_WITH_PIXCLK) && ALIGN_TIMER_WITH_PIXCLK)
       #define HSYNC_TIMER_TCNTS_C (ROUND_UP_TO(TOTAL_CYC_COUNT, 1024*7)/1024)


       #if (HSYNC_TIMER_TCNTS_C <= HSYNC_TIMER_MAX)
        #define HSYNC_TIMER_TCNTS (HSYNC_TIMER_TCNTS_C)
       #else
        #error "Rounded past HSYNC_TIMER_MAX"
       #endif
      #endif
    #endif
  #endif
 #endif
 #if (HSYNC_TIMER_TCNTS == 0)
  #error "HSYNC_TIMER_OCRVAL == 0"
 #endif
#else //Either it fit, or HSYNC_TIMER_CLKDIV was already defined...
 #define HSYNC_TIMER_TCNTS TOTAL_CYC_COUNT
 #ifndef HSYNC_TIMER_CLKDIV
  #define HSYNC_TIMER_CLKDIV CLKDIV1
 #endif
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
 #if (((HSYNC_TIMER_TCNTS + 1) % 7) == 0)
  #warning "(HSYNC_TIMER_TCNTS + 1) % 7 == 0"
  #warning "   so... HSYNC_TIMER_OCRVAL = HSYNC_TIMER_TCNTS"
  #define HSYNC_TIMER_OCRVAL (HSYNC_TIMER_TCNTS)

 #elif (((HSYNC_TIMER_TCNTS + 2) % 7) == 0)
  #warning "(HSYNC_TIMER_TCNTS + 2) % 7 == 0"
  #warning "   so... HSYNC_TIMER_OCRVAL = HSYNC_TIMER_TCNTS + 1"
  #define HSYNC_TIMER_OCRVAL (HSYNC_TIMER_TCNTS + 1)

 #elif (((HSYNC_TIMER_TCNTS + 3) % 7) == 0)
  #warning "(HSYNC_TIMER_TCNTS + 3) % 7 == 0"
  #warning "   so... HSYNC_TIMER_OCRVAL = HSYNC_TIMER_TCNTS + 2"
  #define HSYNC_TIMER_OCRVAL (HSYNC_TIMER_TCNTS + 2)

 #elif (((HSYNC_TIMER_TCNTS + 4) % 7) == 0)
  #warning "(HSYNC_TIMER_TCNTS + 4) % 7 == 0"
  #warning "   so... HSYNC_TIMER_OCRVAL = HSYNC_TIMER_TCNTS + 3"
  #define HSYNC_TIMER_OCRVAL (HSYNC_TIMER_TCNTS + 3)

 #elif (((HSYNC_TIMER_TCNTS + 5) % 7) == 0)
  #warning "(HSYNC_TIMER_TCNTS + 5) % 7 == 0"
  #warning "   so... HSYNC_TIMER_OCRVAL = HSYNC_TIMER_TCNTS + 4"
  #define HSYNC_TIMER_OCRVAL (HSYNC_TIMER_TCNTS + 4)

 #elif (((HSYNC_TIMER_TCNTS + 6) % 7) == 0)
  #warning "(HSYNC_TIMER_TCNTS + 6) % 7 == 0"
  #warning "   so... HSYNC_TIMER_OCRVAL = HSYNC_TIMER_TCNTS + 5"
  #define HSYNC_TIMER_OCRVAL (HSYNC_TIMER_TCNTS + 5)

 #elif (((HSYNC_TIMER_TCNTS) % 7) == 0)
  #warning "(HSYNC_TIMER_TCNTS) % 7 == 0"
  #warning "   so... HSYNC_TIMER_OCRVAL = HSYNC_TIMER_TCNTS - 1"
  #define HSYNC_TIMER_OCRVAL (HSYNC_TIMER_TCNTS - 1)

 #else
  #error "WTF?"
 #endif
#else
 #define HSYNC_TIMER_OCRVAL (HSYNC_TIMER_TCNTS-1)
#endif

#if (HSYNC_TIMER_OCRVAL > HSYNC_TIMER_MAX)
 #error "Despite all my efforts, HSYNC_TIMER_OCRVAL is *still* > HSYNC_TIMER_MAX!"
#endif






#endif	//__HSYNC_TIMER_H__






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
 * /Users/meh/_avrProjects/LCDdirectLVDS/93-checkingProcessAgain/hsyncTimerStuff.h
 *
 *    (Wow, that's a lot longer than I'd hoped).
 *
 *    Enjoy!
 */

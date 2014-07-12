/* mehPL:
 *    This is Open Source, but NOT GPL. I call it mehPL.
 *    I'm not too fond of long licenses at the top of the file.
 *    Please see the bottom.
 *    Enjoy!
 */








//
//
// THIS FILE IS NOT USED
// It remains for historic purposes...
// that I can't quite recall whether I ever found a specific set of timings
// that worked with *all* the displays I've tested (despite the fact,
// they've all been tested using "DISPLAY == TESTVALS"
// I think I was manually changing those values... but maybe somewhere the
// history remains, and could be dug-up if necessary.
//
//


//lcdDefines.h contains project-specific definitions (and redefinitions?)
//  for functions/macros/constants/timing-values used by:
//  _commonCode.../lcdStuff
//  This, along with lcdStuff, is not properly implemented, as far as
//  c/header files... so this must be #included in main.c *before* lcdStuff



#ifndef __LCD_DEFINES_H__
#define __LCD_DEFINES_H__

//for LVDS_PRESCALER...
#include "mainConfig.h"

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
// a/o v59-12ish, upon PLL_SYSCLK addition:
// I'm not sure this math is right... but can't visualize it right now
// Regardless, changing the scaler in PLL_SYSCLK doesn't seem to affect
// the apparent syncing problem that resulted from PLL_SYSCLK
// This *should* have some effect! It affects the timer, which determines
// the duration of a row!
// No, it is *definitely* affecting the refresh-rate, but these white bars
// still exist.
// I've seen these before... what was the deal? carry-over from a previous row?
#warning "REVISIT DOTS_TO_CYC MATH"
#warning "DOTS_TO_CYC DOESN'T ROUND UP"
#if(defined(PLL_SYSCLK) && PLL_SYSCLK)
   #define DOTS_TO_CYC(dots) ((((dots)*(7l)*(LVDS_PRESCALER))/4))
#else
   #define DOTS_TO_CYC(dots) ((((dots)*(7l)*(LVDS_PRESCALER))/8))
#endif

#define delay_Dots(numDots) delay_cyc(DOTS_TO_CYC(numDots))

#define DE_DotDelay(numDots) delay_Dots(numDots)

//THESE NOTES ARE OLD:
//This has to be changed every time the DE code is changed...
// Actually, I think it should just match IMAGE_WIDTH
//#define FB_WIDTH 28 //21//19
//#define IMAGE_WIDTH 21


#define TESTVALS   0		//Currently TESTVALS are being used with the LTN
								// display... 
#define LTN 1				//LTN and IDT haven't been used for a while.
#define IDT 2


//Options LTN, IDT, or TESTVALS
// IDT display *still* doesn't sync... maybe my H/V signals aren't right
#define DISPLAY TESTVALS //LTN //TESTVALS //IDT //LTN





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


//a/o v75: IDTech never worked and has since bit-the-dust
//
// Up to this point, both the LTN and the Chi Mei, which have been working
// were using "TESTVALS"
// and I haven't been keeping track of which vals belong to which.
// so, as of v74 (LTN, PWM161):
// LTN was last using H_LOW_DOTS = 0, HD_DOTS = 0
// These were changed (not sure when)
// from 100 and 50, in v66
//
// Now's the kicker:
//  v66 works with the new BOE display, but v74 does not.
//  So reverting for that...

// This applies to all DE-only displays
// (Of which all have been so far)
// Having a value for H_LOW_DOTS increases codesize
// Those might be better-moved elsewhere (e.g. HFP?)

#if (DISPLAY == LTN)
//Though the values have varied over time...:
#error "DISPLAY == TESTVALS has been used for quite some time with various displays"
#define H_LOW_DOTS   0	//8==OLD unused... 0==last use a/o v74 in TESTVALS
								//LTN doesn't use H_LOW (DE-only)
#elif (DISPLAY == IDT)
#error "DISPLAY == TESTVALS has been used for quite some time with various displays"
 #define H_LOW_DOTS   40//8//1//40 //30 //2//136
#elif (DISPLAY == TESTVALS)
 #define H_LOW_DOTS   100
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
#define DE_ACTIVE_DOTS   1024
#elif (DISPLAY == IDT)
#define DE_ACTIVE_DOTS   680 //1024 //800 //920 //943 //812 //472 //1024
#elif (DISPLAY == TESTVALS)
#define DE_ACTIVE_DOTS   1024
#endif

#define T_DE_CYC   DOTS_TO_CYC(DE_ACTIVE_DOTS)

//Horizontal Blank Time: HD_DOTS + H_LOW + DH_DOTS
//  90-320+
//Horizontal Total Time: 1206-1344-2047
// ... but Total = Blank + Pixels, so 90+1024 != 1206
// Excess waits should be thrown in DH (since that's between interrupts)
//T_HD is the time from H inactivated to DE activated
//aka H back-porch: 1-160+
//IDTech Last Used: 50
//LTN last used 5

//See notes about TESTVALS and whatnot above in: H_LOW_DOTS
#if (DISPLAY == LTN)
#define HD_DOTS 0 //5==last used long ago. Latest TESTVALS a/o v74 was 0
#elif (DISPLAY == IDT)
#define HD_DOTS   50 //5 //0//20//5 //50 //30 //1 //160
#elif (DISPLAY == TESTVALS)
#define HD_DOTS 50//5
#endif

#define HD_Delay()   delay_Dots(HD_DOTS)

//#define T_HD_CYC 1//3//(T_HD_REAL*3)
#define T_HD_CYC   DOTS_TO_CYC(HD_DOTS)

// This is the number of CPU cycles between DE->L and Horiz->L
// This is just used in calculating the OCR value...
// aka H Front Porch: 0-24+
// IDTech Last Used: 24
// LTN last used 46
// 1072-1024-1-1 = 46
#if (DISPLAY == LTN)
#define DH_DOTS   46
#elif (DISPLAY == IDT)
#define DH_DOTS   24 //46 //480//6//12//24 //46 //24 //122//30 //0 //24
#elif (DISPLAY == TESTVALS)
#define DH_DOTS   0//1000//100//46
#endif

#define T_DH_CYC   DOTS_TO_CYC(DH_DOTS)

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
#define T_Vlow   6 //16//6 //2 //6
#elif (DISPLAY == TESTVALS)
#define T_Vlow 32
#endif

// IDTech Last Used: 768
#if (DISPLAY == LTN)
#define V_COUNT   768
#elif (DISPLAY == IDT)
#define V_COUNT   768
#elif (DISPLAY == TESTVALS)
 #if (LVDS_PRESCALER == 8)
  #define V_COUNT 768
 #else
#warning "V_COUNT for LVDS_PRESCALER != 8 was originally 2/3 * 768... this has been removed"
  //This must've been due to stretching... or repeated-rows due to extended
  // DE's? Doesn't make sense, should be the opposite, the case when 
  //  LVDS_PRESCALER is large, no?
  //#define V_COUNT   (768*2/3)
  #define V_COUNT (768)
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

#endif
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
 * /Users/meh/_avrProjects/LCDdirectLVDS/93-checkingProcessAgain/_displays/originalLCDdefines_fromV79.h
 *
 *    (Wow, that's a lot longer than I'd hoped).
 *
 *    Enjoy!
 */

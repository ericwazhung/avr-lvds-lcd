/* mehPL:
 *    This is Open Source, but NOT GPL. I call it mehPL.
 *    I'm not too fond of long licenses at the top of the file.
 *    Please see the bottom.
 *    Enjoy!
 */





//This file copied from v92's samsungLTN121X1.h and modified for the values
//tested in v66.51
// (Along with notes)


//This file stolen from the original lcdDefines.h, and probably still
//contains quite a bit of info that's not really display-specific...


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

// #define H_LOW_DOTS   100//100
#define H_LOW_DOTS   0//100//50//100//0 //LTN doesn't use H_LOW (DE-only)
                       // And having a value here increases codesize


//H is set low immediately upon entry of the timer interrupt
// its low-time is controlled via nops
// (so if HLow is long for a particular display, this might need revising)
//#define Hlow_Delay() 
//   delay_Dots(H_LOW_DOTS)


////#define T_Hlow_CYC 1//8 //(T_Halow_REAL*3)
//#define T_Hlow_CYC DOTS_TO_CYC(H_LOW_DOTS)



//This was found expermentally with SwitchResX
//IDTech Last Used: 680
//LTN last used 1024
// Oddly: For the LTN, this doesn't seem to affect the horizontal
//        it does, however, affect the vertical(!)
// If I recall Correctly, this is used only for setting the timer interrupt
//  rate...
//#define DE_ACTIVE_DOTS   1024
#if(defined(BLUE_FRAME) && BLUE_FRAME)
#define DE_ACTIVE_DOTS   1000//1060//960//1024
#else
#define DE_ACTIVE_DOTS   920//1060//960//1024
#endif


//#define T_DE_CYC   DOTS_TO_CYC(DE_ACTIVE_DOTS)

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
//#define HD_DOTS 50//5
#define HD_DOTS 0//50//50//5


//#define HD_Delay()   delay_Dots(HD_DOTS)

//#define T_HD_CYC 1//3//(T_HD_REAL*3)
//#define T_HD_CYC   DOTS_TO_CYC(HD_DOTS)

// This is the number of CPU cycles between DE->L and Horiz->L
// This is just used in calculating the OCR value...
// aka H Front Porch: 0-24+
// IDTech Last Used: 24
// LTN last used 46
// 1072-1024-1-1 = 46
//#define DH_DOTS 0// 0//1000//100//46
//oldGCC, HD=0, HLow=0, DH must be <620...
// slightly higher causes one line to repeat full-screen
// double that and some interesting stuff happens...
// 100,000 is interesting...
// but all this suggests that rowCalculationCycs couldn't've possibly
// worked... right? This seems so horrendously sensitive to timings, now!
// (10 is too low, 100 seems fine)
#define DH_DOTS   100//100000//610//100//1000//1700//5000//1000//100//46



//#define T_DH_CYC   DOTS_TO_CYC(DH_DOTS)

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
//#define T_VD 3
// #if(__AVR_LIBC_VERSION__ == 10800UL) //GCC4.8
//  #define T_VD 1 // This worked-ish for the faulty/stretched DIAG_BAR
// #else          // but not for FB_SMILEY
#define T_VD 3//3//0//3


//aka V-sync front porch: 1-3+
// IDTech Last Used: 3
// LTN last used 3
#define T_DV 3

//1-6+ (1 is used by the first switch-case...)
// IDTech Last Used: 6
// LTN Last used 16
// 772 - 768 - 1 - 1 = 2
//#define T_Vlow 32
//Interesting... T_Vlow seems to affect the width of the refresh (?!)
// ABSOLUTELY. 1 vs 100 is the difference between a strobelight and a minor
// flicker (LTN's almost as good as the ChiMei with 1 here!)
// That with GCC48 and bad-syncing (duplicated lines?)
// 
// With oldGCC: apparently T_DV and T_VD need values as well
// as putting their values here doesn't sync
// Does the LTN actually pay attention to the Vsync signal?!
// And T_DV 10 seems to leave a solid non-flickering bar at the top with
// refreshes below, but 1 seems to clear that.
// And T_VD appears to have little effect (except that it needs to be >2?)
// T_Vlow seems to have little-effect with oldGCC
// (stable at 3, 1, 10)
//
// Back to newGCC
// T_VD=1 worked for the stretched-thing, but 3 is necessary for FB_SMILEY
#warning "These values/notes were untested with the new GCC4.8 and the new Delay_Dots() and should probably be revisited."
#define T_Vlow 1//400//100//0



// IDTech Last Used: 768
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
 * /Users/meh/_avrProjects/LCDdirectLVDS/93-checkingProcessAgain/_displays/samsungLTN121X1_v6651.h
 *
 *    (Wow, that's a lot longer than I'd hoped).
 *
 *    Enjoy!
 */

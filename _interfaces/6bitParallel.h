/* mehPL:
 *    This is Open Source, but NOT GPL. I call it mehPL.
 *    I'm not too fond of long licenses at the top of the file.
 *    Please see the bottom.
 *    Enjoy!
 */



#ifndef __6BITPARALLEL_H__
#define __6BITPARALLEL_H__

// a/o v80, this is a first-attempt at running a parallel-interfaced LCD
// (specifically, the Sony ACX)
//
// The display is 9-bits (3-bits per R/G/B)
// But since this project has been 2-bits per R/G/B for so long, it'd be
// easiest to only use two. Also, since it only requires one port, later
// attempts with FrameBuffer, etc. will only require one instruction (one
// PORT = frameBuffer[row][col]) to handle all color-bits.

// From the <= v79 scheme, this is essentially a replacement for lvds.c
// Other changes will have to be made elsewhere (e.g. writeColor.c)

// This file pieced-together from lvds161.c as a template, since it's
// basically stripped-down to the essentials (lvds.c is much wordier, and a
// good place to look for info)

// Also, not exactly sure what to do about port-selection, since it might
// make sense to change the pinout based on the particular chip, etc.

// So, for now, this is for the ATtiny861
//  (since I have so many)
//  AND overrides the pinout shown in pinout.h
//
//                           ATtiny861
//                          ____________________
//                         |         |_|        |
//                  MOSI --|  1 PB0      PA0 20 |--   Red0
//           Heart  MISO --|  2 PB1      PA1 19 |--   Red1
//                  SCK  --|  3 PB2      PA2 18 |--   Green0
//  MCK    (OC1B)  --------|  4 PB3      PA3 17 |--   Green1
//                   VCC --|  5 VCC     AGND 16 |-- (GND)
//                   GND --|  6 GND     AVCC 15 |-- (VCC)
//               Hsync   --|  7 PB4      PA4 14 |--   Blue0
//               Vsync   --|  8 PB5      PA5 13 |--   Blue1  
//               S/D     --|  9 PB6      PA6 12 |-- 
//                /Reset --| 10 PB7      PA7 11 |-- PCINT7: NEW BUMP SENSOR
//                         |____________________|   (when implemented)
//
// NEW BUMP SENSOR:
// a/o v82: TODO: This note should probably be elsewhere...
// Heart cannot be used multiplexed with the new bump-sensor idea... so we
// need another pin, one with an interrupt... Not many to choose from.
// Also, it can't be shared with any other pins, since it's mostly active
// and only inactive briefly when bumped.
//
// As-Implemented, PORTA is the only port with available pins
// But the entire port is written during each writeColor() call...
// TODO: It might be wise to switch S/D to PORTA(?) Since it needn't change
// often (as-implemented, it's unused entirely)
// 
// The problem is enabling/disabling the internal pull-ups might cause an
// interrupt to be detected... if e.g. PA7 was used for the bump-sensor
//
// But I think that's what I'mma do.



#include _BITHANDLING_HEADER_
#ifndef __AVR_ATtiny861__
#error "This currently only works with the ATTiny861... It's mostly just a pinout issue... of NYIishness"
#endif

#if (!defined(LCDINTERFACE_BITBANGED_DOTCLOCK) || \
		!LCDINTERFACE_BITBANGED_DOTCLOCK)
 #include "delay_cyc.h"
#else //LCDINTERFACE_BITBANGED_DOTCLOCK. Use delayDots instead of delayCyc
 //Indicate to lcdStuff that it needs to include dot-delays DURING
 //v-blanking in order to write the pixel-clock
 // (as opposed to its being handled via PWM cycling between the HSync 
 //  timer interrupts)
 #define LCDSTUFF_INCLUDE_NON_DE TRUE
#endif



// TODO: Figure out: Is there a particular interface that would be best for
// handling the unused LSB of each color...? 
// (maybe such that the colors on this display match those on an LVDS one?)
// For now: Assuming that e.g. R0/R1 -> uC:Red0 R2 -> uC:Red1
//a/o a while ago (this being v86):
// This has been explored, and must be better-noted somewhere, by now...
// Essentially, as I recall, it's best to combine R0 and R2 on Red1 and R1
// on Red0, the color-scheme is similar to that of LVDS, and relatively
// linear. Again, I think that's already noted somewhere (maybe
// writeColor?)

// MCK is handled by OC1B (matches LVDS's clock signal)
// THESE SHOULD NOT BE CHANGED without also looking into the
// setbit(PWM1B...) portion of _init()
#define MCK_OCR		OCR1B
// These will probably only be used for initialization:
//a/o a while ago (this being v86): That's not true, MCK is bit-banged...
#define MCK_PIN		PB3
#define MCK_PORT		PORTB

// Hsync is on PB4, and is active-low
#define HSYNC_PIN		PB4
#define HSYNC_PORT	PORTB
// Vsync is on PB5, and is active-low
#define VSYNC_PIN		PB5
#define VSYNC_PORT	PORTB

//This is just for handling writeMasked's when writing BOTH Hsync and Vsync
// (e.g. for _init() macros)
//And, as-implemented, requires that Hsync and Vsync be on the same port
#define HV_MASK		( (1<<(HSYNC_PIN)) | (1<<(VSYNC_PIN)) )
#define HV_PORT		HSYNC_PORT
#define HSYNC_ACTIVE		(0<<(HSYNC_PIN))
#define HSYNC_INACTIVE	(1<<(HSYNC_PIN))
#define VSYNC_ACTIVE		(0<<(VSYNC_PIN))
#define VSYNC_INACTIVE	(1<<(VSYNC_PIN))

// S/D is on PB6
// S/D is oddly-named, it's active-low
// More logically it'd be called DisplayEnable and be active-high
//(and it's relatively unused/unimplemented)
#define SD_PIN			PB6
#define SD_PORT		PORTB

// PORTA is used for R/G/B data
// PA0 Red0
// PA1 Red1
// PA2 Green0
// PA3 Green1
// PA4 Blue0
// PA5 Blue1
// PA6 (unused)
// PA7 (unused)	//Now used by bump-sensor/switch
#define RGB_PORT	PORTA





#define RED_SHIFT		0
#define GREEN_SHIFT	2
#define BLUE_SHIFT	4

#define RED_MASK		(0x03<<(RED_SHIFT))
#define GREEN_MASK 	(0x03<<(GREEN_SHIFT))
#define BLUE_MASK 	(0x03<<(BLUE_SHIFT))


//TODO: This cause writeColor to leave bits 7 and 6 on the RGB_PORT
// HIGH... (e.g. for inputs with pull-ups...)
// This isn't well-implemented, and shouldn't be necessary since
// BUMP_SWITCH is supposed to be #included before this file...
// but that doesn't seem to be working, so this is an override.
// This adds an ori instruction in writeColor, so it increases the number
// of cycles which is important to consider if not using bitbanged MCK...
// Then again, non-bitbanged MCK has been pretty much abandoned.
#define OTHER_BITS_HIGH TRUE





//Now, on to the actual functions/macros needed by all _interface/ code:


#define setRed4(val) \
	writeMasked( ((val)<<(RED_SHIFT)), RED_MASK, RGB_PORT )


#define setGreen4(val) \
	writeMasked( ((val)<<(GREEN_SHIFT)), GREEN_MASK, RGB_PORT )


#define setBlue4(val) \
	writeMasked( ((val)<<(BLUE_SHIFT)), BLUE_MASK, RGB_PORT )




#if (defined(REMOVE_DC) && REMOVE_DC)
 #error "REMOVE_DC hasn't been tested since lcdStuff, or long prior"
 #error "Further, it's completely irrelevent for a parallel-interface."
#endif



//FOR NOW: Assuming that color-data is ignored except where necessary
#define Nada_init()		\
{ \
	writeMasked( (HSYNC_INACTIVE | VSYNC_INACTIVE), HV_MASK, HV_PORT ); \
	setBlue4(0); \
}
#define Vsync_init()    \
	writeMasked( (HSYNC_INACTIVE | VSYNC_ACTIVE), HV_MASK, HV_PORT )
#define VplusH_init()   \
	writeMasked( (HSYNC_ACTIVE | VSYNC_ACTIVE), HV_MASK, HV_PORT )
#define Hsync_init()    \
	writeMasked( (HSYNC_ACTIVE | VSYNC_INACTIVE), HV_MASK, HV_PORT )
//This display doesn't use DE ("Data-Enable") to indicate where the
//pixel-data starts, but these are still used by lcdStuff...
// DEonly/DEblue_init() don't handle red/green, since it was designed for
// FPD-link displays.
#define DEonly_init()   \
{ \
	writeMasked( (HSYNC_INACTIVE | VSYNC_INACTIVE), HV_MASK, HV_PORT ); \
	setBlue4(0); \
}
#define DEblue_init()   \
{ \
	writeMasked( (HSYNC_INACTIVE | VSYNC_INACTIVE), HV_MASK, HV_PORT ); \
	setBlue4(3); \
}





#if (defined(DE_ONLY_DISPLAY) && DE_ONLY_DISPLAY)
 #error "DE_ONLY_DISPLAY is not implemented with this interface"
 #error " But don't be discouraged... an actual LCD that's DE-Only"
 #error " should work fine WITHOUT DE_ONLY_DISPLAY set true"
 #error " Really, DE_ONLY_DISPLAY was just a test... see lvds.c"
#elif (defined(FULL_INIT_TESTS) && FULL_INIT_TESTS)
//#error"OK"
 #define Vsync_fromNada()      Vsync_init()
 #define VplusH_fromVsync()      VplusH_init()
 #define Vsync_fromVplusH()      Vsync_init()
 #define Nada_fromVsync()      Nada_init()
 #define Hsync_fromNada()      Hsync_init()
 #define Nada_fromHsync()      Nada_init()
 #define DEonly_fromNada()      DEonly_init()
 #define DEblue_fromDEonly()   DEblue_init()
 #define Nada_fromDEonly()      Nada_init()
 #define Nada_fromDEblue()      Nada_init()
 #define DEonly_fromDEblue()   DEonly_init()
#else   //NOT FULL_INIT_TESTS
//These are the minimal changes necessary, for high-speed processing...
 #define Vsync_fromNada()   clrpinPORT(VSYNC_PIN, VSYNC_PORT) 
 #define VplusH_fromVsync() clrpinPORT(HSYNC_PIN, HSYNC_PORT)
 #define Vsync_fromVplusH() setpinPORT(HSYNC_PIN, HSYNC_PORT)//Vsync_init()
 #define Nada_fromVsync()   setpinPORT(VSYNC_PIN, VSYNC_PORT)
 #define Hsync_fromNada()   clrpinPORT(HSYNC_PIN, HSYNC_PORT)
 #define Nada_fromHsync()   setpinPORT(HSYNC_PIN, HSYNC_PORT)
 #define Nada_fromDEblue()  Nada_init()
 #define Nada_fromDEonly()  setBlue4(0)
 #define DEonly_fromNada()  
 #define DEblue_fromNada()  setBlue4(3)
 #define DEblue_fromDEonly() setBlue4(3)
 #define DEonly_fromDEblue() setBlue4(0)

#endif



#define fullBlue()   DEblue_init()
// So far, there's no "noBlue()" like there is (now) for red/green
// it gets confusing since doing-so would also affect the timing signals
// (in FPD-Link... since this was originally designed for that)
// But, that means noBlue() is never called anywhere anyways, so doesn't
// matter...
// If something like it is necessary in your project, use setBlue4(0), no?

#define noRed()   setRed4(0)
#define fullRed() setRed4(3)
#define noGreen()   setGreen4(0)
#define fullGreen() setGreen4(3)
//These are unnecessary for parallel-displays, but the macro is called...
#define lvds_disableGreen_MakeClockInsensitiveToDT()
#define lvds_enableGreen_MakeClockSensitiveToDT()

#if (defined(SLOW_EVERYTHING_TEST) && SLOW_EVERYTHING_TEST)
 #error "SLOW_EVERYTHING_TEST is true, but hasn't been implemented for parallel LCDs..."
#endif



//This is temporary, for compatibility with lvds.c...
#define lvds_timerInit()	lcdInterface_init()


static __inline__
void lcdInterface_init(void)
	__attribute__((__always_inline__));


static __inline__
void lcdInterface_pwmDotClockInit(uint8_t onOff)
	__attribute__((__always_inline__));


//a/o a while ago (this being v86):
// since MCK is now bit-banged, pwmDotClock is only used between
// screen-refreshes



#if (defined(LCDINTERFACE_BITBANGED_DOTCLOCK) && \
		LCDINTERFACE_BITBANGED_DOTCLOCK)

 #define delay_OneDot() \
 { \
	 asm("nop;"); \
	 clrpinPORT(MCK_PIN, MCK_PORT); \
	 asm("nop;"); \
	 setpinPORT(MCK_PIN, MCK_PORT); \
 }

 #define delay_Dots(numDots) \
 { \
	 uint16_t i; \
	 for(i=0; i<numDots; i++) \
	 { \
		 delay_OneDot(); \
	 } \
 }

#endif

#if(defined(FRAMEBUFFER_TESTING) && FRAMEBUFFER_TESTING)
//#error "OK2"
//This is for compatibility with _options/writeColor.c, if it's used
// (e.g. for frameBuffer and possibly later for rowBuffer/rowSegBuffer?)
//Since it's inline, it won't be compiled unless it's used.

//writeColor.c will declare/define writeColor() unless it's already defined
// so do-so here.
#define writeColor	parallelLCD_writeColor




#if (!defined(LCDINTERFACE_BITBANGED_DOTCLOCK) || \
		!LCDINTERFACE_BITBANGED_DOTCLOCK)
 //parallelLCD_writeColor() uses only one instruction when not bit-banged
 // (PORT=color)
 #define WRITE_COLOR_CYCS 1

 //Calculate WRITE_COLOR_DELAY...
 #include "_options/writeColorDelay.h"

#else //LCDINTERFACE_BITBANGED_DOTCLOCK

 //parallelLCD_writeColor() uses four instructions when bit-banged, for now
 // The idea is write the port-value with MCK high, bring MCK low, bring it
 // high. There's probably no reason it has to be four cycles, except to
 // try not to exceed 4MHz when running at 16MHz F_CPU
// #define WRITE_COLOR_CYCS 5
// The above shouldn't be necesssary, right...? We need delay_Dots...
 //We use this to stretch each writeColor() call to fill the screen...
 // -1 takes into account that one dot is already drawn in each call to 
 // writeColor (when bitbanged).
 #if(!defined(VISIBLE_ROW_DOTS))
  #define WRITE_COLOR_DOT_DELAY	((DE_ACTIVE_DOTS/FB_WIDTH)-1)
 #else
  //In this case, some of the dots in the row may be (e.g.) covered by the
  //project's housing...
  //The display itself probably requires the same timing regardless, but
  //we'd like the framebuffer to be completely visible (so therefore shrunk
  //to fit in the visible dots)
  //TODO: There'll need to be extra Black drawn at the end of the row...
  #define WRITE_COLOR_DOT_DELAY  ((VISIBLE_ROW_DOTS/FB_WIDTH)-1)
 #endif
 //This is a hack... prevent writeColorDelay.h from being included in
 //writeColor.c...

//a/o v86: This apparently can't be working, since "H__" is "H_"
// but the code's been functioning for quite some time.
#warning "WTF?! This ain't right... but it's been working... writeColorDelay.h is still being included...?"

 #define __WRITECOLORDELAY_H_

	//This is where delay_Dots() used to be defined...
	// wtf was I thinking?!


#endif



static __inline__
void parallelLCD_writeColor(uint8_t includeDEinit, uint8_t includeDelay,
																		uint8_t colorVal)
	__attribute__((__always_inline__));


#elif (defined(ROWBUFFER_TESTING) && ROWBUFFER_TESTING)
	#warning "ROWBUFFER is *barely* implemented..."
	#define WRITE_COLOR_DOT_DELAY ((DE_ACTIVE_DOTS/RB_WIDTH)-1)
	#error "need writeColor to be defined here as parallelLCD_writeColor..."
	#error "but doesn't it need to be written specifically for rowBuffer?"
#endif //FRAMEBUFFER_TESTING 



#endif // __6BITPARALLEL_H__

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
 * /Users/meh/_avrProjects/LCDdirectLVDS/93-checkingProcessAgain/_interfaces/6bitParallel.h
 *
 *    (Wow, that's a lot longer than I'd hoped).
 *
 *    Enjoy!
 */

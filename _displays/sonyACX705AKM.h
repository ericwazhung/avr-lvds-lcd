/* mehPL:
 *    This is Open Source, but NOT GPL. I call it mehPL.
 *    I'm not too fond of long licenses at the top of the file.
 *    Please see the bottom.
 *    Enjoy!
 */




#ifndef __SONY_ACX705AKM_H__
#define __SONY_ACX705AKM_H__

//This is pretty much exclusively for bit-banged MCK on the 6bitParallel
//interface...
// bit-banged seems to have fixed the hsync-shift problem, but now the
// display flickers...
#define LCDINTERFACE_BITBANGED_DOTCLOCK   TRUE


//This attempts to prevent the REFRESH_ON_CHANGE issue where alternating
//rows seem to be not-displayed... (originally NOGO)...
//The idea was to continue running the dot-clock via PWM whenever the
//display isn't being refreshed.
//SO FAR: FB_REFRESH_ON_CHANGE TRUE, _COUNT=1:
// _DELAY=10ms is OK, but 100ms causes the lines to alternate and the
// screen to flicker...
//FB_REFRESH_ON_CHANGE FALSE displays perfectly, but causes there to be no
//CPU power for other processes (like updating the image)
//a/o v90: This has been used for some time, now...
//THIS IS DIFFERENT than LCDSTUFF_INCLUDE_NON_DE...
// Whereas, in this case, that indicates to lcdStuff to continue to
// bitbanging the pixel-clock even when there's no *visible* pixel-data
// (during V-blanks)
// THIS indicates to switch over to PWM-pixel-clocking when there is no
// data to be sent... (especially between refreshes)
// That could be considered an extended v-blank, but it's different in that
// this form of v-blank is somewhat unpredictable in duration
// lcdStuff's v-blank is highly precise to the specs supplied...
// In other words, this *continues* to send pixel-clocks even when
// lcdStuff's lcdUpdate() is disabled... and does-so by means of switching
// over the pixel-clock from bit-banging to PWM...
// I believe the importance lies in that this PWM mode is actually during 
// the V-Front-Porch (?) whereas LCDSTUFF_INCLUDE_NON_DE assures a specific
// number of pixel-clocks *during and after* the vsync, *immediately
// before* data is actually drawn.
// TODO? Since this seems to be a somewhat common thing to do, maybe it'd
// make sense to use *one* method (either bitbanging or pwm) and tell
// lcdUpdate() to run in "extended-VFP" mode...?
// OTOH, bitbanging eats up CPU time, and it's nice to be able to free that
// for other purposes...
#define LCDINTERFACE_BITBANGED_DOTCLOCK_PWM  TRUE



//a/o LCDdirectLVDS80:
// This file will likely replace lcdDefines.h when this display is used
// since lcdDefines.h contains quite a bit of LVDS-specific stuff...
// Further: It may later be divided-up into two files,
//  e.g. only have display-specific stuff here (timings, etc)
//       and another file for 6-bit parallel displays (using a single port)
//        (ignoring the LSB of each color)


//Sony ACX705AKM LCD:
// 240 horizontal pixels by 160 vertical rows
// 6.92cm TFT
// 9 bits per pixel (3 bits each: R, G, B) = 512 colors
// Reflective display with LED front-lighting
// PARALLEL LVTTL timing/data signals.
//
//Pinout:
//   ______________________
//  |                      |
//  |                      |
//  |                      |
//  |         BACK         |
//  |                      |
//  |                      |
//  |                 ___  |
//  |________________\   \_|
//                    \   \
//             ________|   |
//            /            |
//            |    ________/
//            |   |
//           _|   |_
//          |_______|
//          |       |
//          |_______|
//  Pin:     1     22

//  1  MIC1 (Mic Not Connected)
//  2  MIC2 (Mic Not Connected)
//  3  Vcc1   +3.8V   (3.50-4.20V)
//  4  Vcc1   +3.8V
//  5  GND
//  6  GND
//  7  R0     Red LSB
//  8  R1     Red
//  9  R2     Red MSB
// 10  G0     Green LSB
// 11  G1     Green
// 12  G2     Green MSB
// 13  B0     Blue LSB
// 14  B1     Blue
// 15  B2     Blue MSB
// 16  Hsync  
// 17  Vsync
// 18  S/D    Shut Down
// 19  MCK    Master Clock (<= 4.2MHz) (pixel clock?)
// 20  Vcc2   +3.0V   (2.55-3.15V)
// 21  LED Vdd  (see note)
// 22  LED GND
//
// LED forward-voltage is not specified.
//  Spec-sheet shows a 15mA current-source (@25deg C, acceptable to 70degC)
//  TODO: Determine the necessary voltage
//  Seriously "The lamp life shall be greater than 50 hours" ?!

// From: http://www.edaboard.com/thread31088.html
// Member: Ace-X
// "LED light in this LCD has about 6 volts forward drop at the 15mA
// specified in the datasheet"

// From: http://forum.lcdinfo.com/viewtopic.php?t=1668
// Member: Michael
// "forward voltage drop of LCD Backlight LED array is approx 6.75v when
// measured with a current of 15mA flowing though the LEDs."

//This page has a ton of schematics, etc.
// From: http://cas.et.tudelft.nl/~rene/acx705akm.html
// "The backlight are two white leds in series. They need a minimal
// voltage of 6.3V."

// The Current Source shown at the above page, reiterated here:
// "The unit has a current source which is adjustable ( a trimmer).The
// maximim current is 15mA."
// Haven't done the math, so I'm assuming the 82ohm resistor limits it to
// 15mA...
//
//                                     .----/\/\/\--+------> (<=15mA)
//                  LM317              |    82ohms  |
//                  ___________        v            |
// Vin (7.5V?) >---| Vi     Vo |-----/\/\/\         |
//                 |    Adj    |     1Kohms         |
// Worst Case:     |___________|                    |
// V_F_LED=6.75V         |__________________________|
// Then Vin needs to take into account the LM317's drop, as well as the two
// resistors... 
// In the circuit supplied from the above page, a MAX860 is used as a
// voltage-doubler from (what appears to be) 5V, so Vin is 10V in that
// case.
//
// From the LM317 datasheet:
// Iout = (Vref / R1) + Iadjust ~= 1.25V / R1 := 1.25 / 82 = 15.24mA
// So, Vin must be > 6.75 + 1.25V
// But, also, Vi-Vo is specified as >= 3.0V So... that'd require 11V?
// Dunno how the LM317 functions at low Vi-Vo values...


// I opted to use a Texas Instruments 3.0V 50mA LDO regulator in a similar
// configuration, since I have several in stock: TPS76030
//
//               1 _____________  5     200ohms
// Vin 12V >---+--| Vi       Vo |---+--/\/\/\----.
//             | 3|             |   |            |
//             '--| En   "GND"  |  --- 4.7uF     |
//                |_____________|  --- Tantalum  |
//                         | 2      |            |
//                         '--------+------------+------> 15mA out
//
// Because this is LDO, the input voltage requirement is nearly (?) the
// same as for the LM317, despite the large "adjust" voltage of 3V
// Maximum drop-out (between pins 1 and 5) is .18V
// (as opposed to most regulators' requiring nearly 2V)
// So the total Vin needs to be 3.18V greater than the LED forward-voltage
// (The LM317 datasheet I have doesn't list the minimum Vi-Vo but is
// specified for 3V drop-out!)
//
// The TPS76030 specifies that a >= 2.2uF tantalum capacitor is necessary
// for LDO regulators to ground. It doesn't specify usage as a
// current-regulator, so it's just a guess that it should work smoothly as
// shown. (It's lit, but hasn't been 'scoped).
//
// V_F_LED is measuring 6.73V


// Digital Input Voltages (R/G/B, syncs, ShutDown, MCK)
//  Relative to Vcc2 (Absolute Max: Vcc2 + .3V)
// IOW: The entire digital-system (including the AVR) 
//  should be run from 3.0V (though 3.3V might be acceptable)


// IMPORTANT TIDBITS:
//  AVR should be run at 3.0V
//  Display requires two voltages:
//     3.8V @ < 17 mA
//     3.0V @ < 4 mA
//  MCK samples inputs on falling-edge
//  TODO: LED forward-voltage?


// Not ideal, but I typically disregard these specs:
//  Power-Up Sequence:
//   Vcc1/2 -> MCK/Hsync/Vsync valid for one "field" -> S/D active (high)
//  Power-Down Sequence:
//   S/D inactive (low) -> Vsync for 13 fields -> Vsync low -> Vcc1/2 = 0V

// Timing Specs:
//                    Min    Typ   Max
// MCK Freq:          ---    3.26  4.20  MHz
// Horiz Freq:        ---    12.0  15.4  kHz
// Vert Freq:         ---    60.0  77.2  Hz
// Hsync Low Pulse:   9      ---   16    clks
// Vsync Low Pulse:   2      ---   20    lines
// ("Vsync falling edge to Hsync Falling Edge phase difference")
// (                  254    ---   269   clks                  )
// (TODO: This is a weird spec... see below)


// APPARENTLY (from timing-diagrams):
//  Pixel 0 (the first) is loaded 16 clks after Hsync goes low
//   SO: T_HLOW + T_HBP == 16 (clks/dots)
//  T_HFP is drawn as 16 clks: 
//   T_HFP == 16 (clks/dots)
//
//  Row 0 (the first) is loaded 20 lines after Vsync goes low
//   SO: T_VLOW + T_VBP == 20 (lines)
//  T_VFP is drawn as 20 lines: 
//   T_VFP == 20 (lines)
//
//  THUS (Not All Visible): 
//   272 dots per line
//   200 lines per frame


// TODO: Vsync's falling edge is spec'd to be a certain number of clks
// before the next Hsync... 
// From the timing specs, copied/pasted:
//  "Vsync falling edge to Hsync falling edge phase difference 
//    tvhde 254 — 269 tclk"
//
// From the timing chart, redrawn with seeming error:
//              .      .
//       ___   ______   ______   ______   _____
// Hsync    \_/ .    \_/      \_/      \_/
//       _______.      .             ___________
// Vsync        \___________________/
//              .      .
//              |<---->|
//                tvhde
// The timing chart seems to suggest this period is between Vsync's Falling
// edge and Hsync's *Rising* edge, contrary to the timing specs.
//
// REGARDLESS:
//  TODO: This may conflict with the current lcdUpdate() scheme!!!
//        (Isn't Vsync changed at the end of a line?)


// TODO: Also spec'd: DATA lines (R/G/B) are LOW between dot-data...
//       Does this really matter???





// USING 6bit:
//
//bit: 2 1 0
//val: 4 2 1
//     | | |
//     +-|-'
//     | |
//bit: 1 0 | val
//    -----+----
//     0 0 | 0
//     0 1 | 2
//     1 0 | 5
//     1 1 | 7

//This is dependent on how the pixel-clock is implemented...
// Assuming we're running at F_CPU=16MHz, best-bet might be to run the
// pixel-clock via a timer, much like the LVDS implementation...
// So, the OCR value for *setting* the output pin should be 2 (1?)
// And the OCR value for resetting the timer should be 4 (3?)
// Then each dot is four clock cycles...
//a/o a while ago (this being v86):
// we're actually using bit-banging for the MCK pin, it seems to work fine,
// as long as the pixel-clock keeps running between refreshes... So, after
// a refresh (bitbanged), the MCK pin is switched-over to the timer (via
// PWM), as described above. Better notes elsewhere (maybe in
// _interfaces/6bitParallel.c?) 
#define DOTS_TO_CYC(dots) \
	((dots)*(4l))


//These should probably be left elsewhere for generalities...
// (already in lcdDefines.h)

//#define delay_Dots(numDots)	delay_cyc(DOTS_TO_CYC(numDots))
//#define DE_DotDelay(numDots)	delay_Dots(numDots)
//#define Hlow_Delay()				delay_Dots(H_LOW_DOTS)
//#define T_Hlow_CYC DOTS_TO_CYC(H_LOW_DOTS)
//#define T_DE_CYC   DOTS_TO_CYC(DE_ACTIVE_DOTS)
//#define HD_Delay()   delay_Dots(HD_DOTS)
//#define T_HD_CYC   DOTS_TO_CYC(HD_DOTS)
//#define T_DH_CYC   DOTS_TO_CYC(DH_DOTS)

//Display-specific values
// These are (currently) as spec'd in timing charts/specs...
// Hsync Low:
//	#define H_LOW_DOTS	16
// DE isn't used for this display, but Active-Dots is still relevent...
	#define DE_ACTIVE_DOTS	240
// Hsync -> DE (H-back-porch, no?)
//	#define HD_DOTS	(16 - H_LOW_DOTS)
// DE -> Hsync (H-Front-Porch, no?)
//	#define DH_DOTS	16
// Vsync Low:
	#define T_Vlow 16
// Vsync Back Porch
	#define T_VD		(20 - T_Vlow)
// Vsync Front Porch
	#define T_DV		20
// Display rows:
	#define V_COUNT	160



//Overriding the specs, to attempt to sync-better:
// lcdStuff explicitly inserts the HD_Delay() between H_LOW and DataEnable
// But since this display doesn't use DataEnable, and instead relies on a
// specific number of dots from the Hsync (low-edge?) inserting HD_Dots,
// here, with the addition of other calculations, causes a large shift
//
// These experiments seem to be having the expected effect, but I'm a bit
// lost as to how the shift could still be so great after only two H_LOW
// dots, and no HD_DOTS... still seems to be shifted about 10 pixels in
// DE_BLUE, which should, realistically, only be a handful of CPU
// instructions between Hsync_fromNada() + Hlow_Delay()...
// Nada_fromHsync() -> HD_Delay() (0 should optimize out, no?)
// -> if(dataEnable) -> drawPix(rowNum) (inlined)
// -> DEblue_fromNada()...
// Seems this should all be under 16 clock cycles, *certainly* under 16*4,
// since dot-clock is 1/4th the CPU clock!
#if(defined(LCDINTERFACE_BITBANGED_DOTCLOCK) && \
		LCDINTERFACE_BITBANGED_DOTCLOCK)
	#define H_LOW_DOTS	9
	#define HD_DOTS	(16 - H_LOW_DOTS)
#else
   #define H_LOW_DOTS	2//9//16
	#define HD_DOTS	0//(16 - H_LOW_DOTS)
#endif

//Reducing this to 0 didn't shift the image at all, but caused frames to be
//missed (?!)
	#define DH_DOTS	16

// EXPERIMENTATION:
//
// Because this display relies on a specific number of pixel-clocks between
// Hsync and the beginning of data, the whole shebang of lcdStuff.c's BLUE
// tests are somewhat flakey...
//
// BLUE_DIAG_BAR shifts right quite a bit more than DE_BLUE, etc.
// Likely due to the number of calculations performed to determine where to
// draw the dots...
//
// Realistically, I suppose, the BLUE tests are a bit flakey in that
// regard, they rely heavily on the fact of most displays I've worked with
// having (and paying attention to) the DataEnable (DE) signal... so
// differences in timing between Hsync and the actual data being available
// to display doesn't matter in those cases. This case is *entirely
// different* in that regard. 
//
// BLUE_DIAG_BAR_SCROLL is an extreme example. Whereas the above
// timing-values (H_LOW_DOTS=2, HD_DOTS=0) caused a shift of merely 10ish
// pixels in DE_BLUE, BLUE_DIAG_BAR_SCROLL seems to be shifted nearly half
// a screen-width. WHOA.
//
// INTERESTING RESULT(?)
// The very tip of the diagonal can be seen wrapped to the left of the
// screen... THIS COULD BE USEFUL...
// It suggests there may be a workaround to the highly-sensitive (and hard
// to match?) timing between Hsync and data, by relying on wraparound...?
// Possibly the entire display could be drawn without using Hsync at
// all...? (e.g. by "drawing" off-screen until it wraps to the next
// line...?)
// 
// It could be that e.g. the frameBuffer would be entirely capable of
// syncing properly with a little fine-tuning. That may be the Next Step,
// for now.

// MORE:
// Switching Hsync to another pin, such that the hsync signal is not
// recieved (nor initialized) (PB0) causes no image
// Doing the same with Vsync causes a scrolling image, but still
// right-shifted by the same amount...
// (Trying to figure out whether Hsync is being paid attention to, as it
// shouldn't be more than 64 cycles, or 16 dot-clocks between H-Low and the
// data, right...? in FrameBuffer mode with drawPixSetup)
// So, yes, Hsync is paid-attention-to

//BIT-BANGED MCK:
//a/o a while back (this being v86)
// I've switched over to a bit-banged MCK pin... I guess that's in
// _interfaces/6bitParallel.c
// bit-banged MCK seems fine, and relieves the above timing problems
// dramatically.













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
 * /Users/meh/_avrProjects/LCDdirectLVDS/90-reGitting/_displays/sonyACX705AKM.h
 *
 *    (Wow, that's a lot longer than I'd hoped).
 *
 *    Enjoy!
 */

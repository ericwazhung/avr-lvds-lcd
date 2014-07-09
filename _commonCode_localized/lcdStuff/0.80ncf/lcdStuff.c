/* mehPL:
 *    This is Open Source, but NOT GPL. I call it mehPL.
 *    I'm not too fond of long licenses at the top of the file.
 *    Please see the bottom.
 *    Enjoy!
 */


//lcdStuff 0.80ncf-2

//0.80ncf-2 - Fixing vsync-case which went back to Nada...
//            e.g. for bit-banged parallel-LCD and/or SDRAMthing
//            TODO: Remove other case (for SDRAMthing)
//0.80ncf-1 - Adding LCDSTUFF_INCLUDE_NON_DE
//            This is the same as LCDSTUFF_NOT_IN_TIMER, except
//            better-named. This calls DE_DotDelay(), etc. even when in a
//            vertical-blanking-period.
//            This, a/o LCDdirectLVDS81 in order to use a dot-clock that
//            *isn't* timer-based (e.g. bit-banged)
//            ALSO replacing delay_cycs with delay_Dots in relevent
//            sections
//0.80ncf - drawPixSetup function, for large-calculations to occur *before*
//          Hsync (a/o LCDdirectLVDS-80, for non-DE display)
//0.70ncf-1 - swapping halves in BLUE_DIAG_BAR so the sync position can be
//				  seen...
//            Also in BLUE_DIAG_BAR_SCROLL
//            BUT NOT (TODO?) In others...
//0.70ncf - a/o LCDdirectLVDS-80, in which we attempt to run a NON LVDS
//				display...
//          First step was creating new *separate* files for each display's
//          timing parameters...
//          This *discontinues* the need for #define DISPLAY TESTVALS, etc.
//          So, ultimately, TODO: move _displays/ here
//          Until then, disable DISPLAY default, here...
//          ACTUALLY: Apparently just forgot to disable assignment of
//          DE_ACTIVE_DOTS if it's already defined...
//          So, DISPLAY is still set, but not used anywhere...
//0.60ncf - continuation of 0.56ncf-2 (which was untested)
//          standardizing function names... updateLCD() -> lcd_update()
//          lcdInit() -> lcd_init()
//          lcd_update() now returns Row Number
//0.56ncf-2 - a/o LCDdirectLVDSv73:
//            fixing lcd_init() to reset hsyncCount and dataEnable
//            so it can be used to start a new frame prior to finishing the
//            last
//            NOT TESTED, SEE 0.60
//0.56ncf-1 - reuninstating loadRow in lcdUpdate()
//            apparently it's handled in LCDdirectLVDS itself
//            And maybe it's better that way...
//            SO NOW: aside from a few notes about loadRow, this is
//            identical to 0.55ncf-2
//0.56ncf - reinstating loadRow in lcdUpdate()
//          a/o LCDdirectLVDS70 for working with the rowBuffer
//          TODOne: How is it that it was removed so long ago... wasn't I
//          using rowSegBuffer then, still...? (see above)
//0.55ncf-2 - a/o LCDdirectLVDS67:
//            Adding notes to loadData() re: transitions...
//            TODO: it's still a little unclear...
//0.55ncf-1 - adding BLUE_DIAG_SCROLL_FLASH
//0.55ncf - drawPix had uint8_t rowNum... switching to uint16_t
//          a/o LCDdirectLVDS-62 (attempting to use DE_BLUE with ChiMei
//          display)
//          Also added BLUE_DIAG_BAR_SCROLL
//0.52ncf-1 - Adding notes re: old mental hurdles.
//0.52ncf - getting rid of git in this directory...
//0.51ncf-git-1ish Adding a few notes remaining from LCDdirectLVDS:
//						"SCOPING has been removed in LVDS, revisit LCDdirect50"
//0.51ncf-git first attempt at using git for uploading to googleCode...
//        this version isn't yet used by any projects
//0.50ncf cleanup, colorOverride is unused...
//				SMILEY -> LCD_SMILEY
//0.40ncf-1 loadRow unused... removal untested...
//0.40ncf drawPix...
//0.30ncf-6 HSYNC_INACTIVE, etc. definable elsewhere
//0.30ncf-5 Adding BLUE_VERT_BAR_REVERSED
//0.30ncf-4 Adding NOTES: DON'T START WITH BLUE_DIAG_BAR
//0.30ncf-3 T_DV and T_Vlow overrideable
//0.30ncf-2 Not sure if I've modified it between... but here: 
//        FRAME_COUNT_TO_DELAY wasn't defined, giving warnings
//0.30ncf-1 updateLCD() has return-values for state-switches
//          (VFP, VSync, VBP, DE)
//        ACTUALLY, that's unnecessary... state-transitions can be
//        determined by vSync and dataEnable...
//0.30ncf Haven't been keeping these backed up regularly...
//        More toward realtime adjustable timing values
//        V Front Porch now occurs at the beginning of the frame, instead
//        of at the end...
//
//0.20ncf More Timing Value experiment preparation... 
//0.10ncf-3ish looking into reloading methods for e.g. experimenting with
//          timing values: sdramThing2.0-7
//          How to "move" timing-values around data which needn't be
//          changed...
//0.10ncf-andsomemore
//          Added MAKELONG to fix integer overflow warnings
//          Revised it from "... + MAKELONG" to "MAKELONG(...)"
//          (The +MAKELONG version is lost in the ether... if you *really*
//           want it, there's a localized version in sdramThing2.0-6)
//0.10ncf-1ish - Adding HSYNC_COUNT_FRAME_COMPLETE
//               Removal of LVDS_PRESCALER stuff...
//0.10ncf - Stolen from LCDdirectLVDS/54-moreColorStuff/main.c
//          Removal of LCDdirectLVDS-specific stuff...
//				A little bit of rewriting of notes to be more generalized
//          All delay_cyc references replaced with delay_Dots,
//					math changed accordingly...


// I hereby declare this FPD-Link simulation technique to forever be called
//  PW-BANGing

// a/o 0.10ncf-4: (sdramThing2.0v8-28)
//   DON'T START WITH BLUE_DIAG_BAR
//   Unless you want to be depressed
//   Good Starting Point DE_BLUE



// GENERAL TIMING NOTES
// a/o 0.52ncf-1:
//   Upon revisiting an ancient display controller project I did 15 years
//   ago, I'm finding myself falling into similar thought-hurdles I had
//   the first go-round with a graphic LCD display... so here're some
//   concepts:
//
// Most graphical displays without onboard memory have basically these 
// same signals:
//
//	Pixel Clock: Each clock-pulse *generally* corresponds to one pixel...
//   Whether these clock-pulses are used for drawing actual pixels, or
//   otherwise...
//
// Horizontal Sync: Each time the Hsync pin is active, the display starts a
// 	new row. It syncs up the pixels horizontally, at the left of the
// 	screen.
//
//   (This was one of my biggest mental-hurdles from those old days...
//    aided by the fact that my display's timing-chips had different names
//    for the Pixexl-Clock, Hsync, and Vsync pins.) 
//   Hsync is only pulsed *once* at the *beginning* of each row. 
//   It is *not* pulsed with each pixel. It syncs the edge of the
//   pixel-data with the edge of the screen.
//
// Vertical Sync: Each time the Vsync pin is active, the display starts a
// new frame. It syncs the pixel-data with the top of the screen.
//
// (Data Enable: This tells the (usually LCD) display that actual pixels
//  are now being transmitted. It's a bit redundant, and some displays,
//  especially older non-TFT displays don't even have it. 
//  Some newer TFT displays *only* use Data-Enable, determining the
//  horizontal and vertical positions of the pixels based on the delay
//  between Data-Enable pulses).
//
// I can't recall what the names were on my old display... but they were
// danged confusing. Especially to someone who'd never messed with graphics
// prior and had to reverse-engineer the whole thing. 
// Oh yes, here they are: CPX, Frame, and Load. 

// As far as these three signals (Pixel Clock, Hsync, and Vsync):
//  The actual timing-requirements may vary from display to display.
//  But, it's pretty-well-established that for most displays these signals
//  remain pretty constant. For example, an Hsync pulse may arrive at a
//  specific rate (the duration of a "row"). Vsync, therefore, remains
//  active for a number of "rows", even though no pixel-data is being sent
//  during this time. And, again, even though no pixel-data is being sent,
//  the Hsync pulse still occurs at the same rate it would if pixels were
//  being drawn.
//
//  This, I guess, is a remnant of ancient times, probably something to do
//  with analog TV signals... the durations necessary for the electro-
//  magnets to be moved from one side of the screen to the next, and the
//  time necessary to move them again from top-to-bottom.
//
//  In this digital era, it doesn't seem these timings would be necessary,
//  and, in fact, most displays I've gotten working are highly tolerant of
//  timing that doesn't match this rigidity.
//
//  E.G. my *ancient* LCD project was designed long before I understood
//  these "standard" timing specifications. As I recall, I don't have any
//  delays between the "Frame" signal indicating the beginning of the next
//  frame and the "Load" signal indicating the beginning of the first row
//  of data. Certainly not several "rows" of Frame (aka Vsync)
//  pulse-active.
//
//  Further, most of these standardized timings involve "porches", both
//  front and back. The "Horizontal Front Porch" is the time between the
//  end of a row and the Hsync signal. The "Horizontal Back Porch" is the
//  time between the Hsync signal and the beginning of the actual 
//  pixel-data. During these "porches", no pixel-data is sent, and the
//  Hsync signal is not active. Again, in the digital era these are pretty
//  much unnecessary, but some displays still rely on them, and most
//  specify timing-values for them.
//


//new notes a/o 0.10ncf-3ish:
// The basic idea is that timing is something that needs to be experimented
// with, often... so, for cases where LCD timing/drawing data is loaded
// beforehand (e.g. sdramThing2.0-7), it would be nice not to have to
// reload *all* the data, only to change the necessary timing data
//
// Neglecting the SDRAM for a second, we have essentially:
// (The exact placement is not necessarily in order... e.g. DH_DOTS could
//  be at the end of a row, or at the beginning... VSYNC rows could be at
//  the top, or at the bottom.....)
// (See also notes "Here's how it worked pre-lvds:", which actually are a
//   bit off, e.g. I don't think VSYNC L->H was delayed by DE_TIME after
//   Hsync)

// H_LOW_DOTS + HD_DOTS + DH_DOTS = "Horizontal Blank Time"
// (This "front/back" thing always confused me...
//  DH_DOTS is *in front of* Hsync as far as *time* is concerned...
//  i.e. looking at a 'scope trace... But something about reading left-to-
//  right causes me to feel like "front" is the furthest to the right
//  which would be *later* in time... weird.
//  
//
//        |--|<--H_LOW_DOTS                       "Horiz Front Porch"
//        |  |--|<--HD_DOTS="Horiz Back Porch"    |<---->|<--DH_DOTS
//        |  .  .                                 .      |
//        |<............................................./
//        |  .  .                                 .    
//        v  v  .                                 .      
//     /V _______________________________________________  \ VSYNC Rows
//     /H |__|¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯  | (Repeat
//     DE _______________________________________________  /  T_Vlow times)
//              .                                 .
//              .                                 .      "Vsync Back Porch"
//     /V ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯  \ VSYNC->DE Rows
//     /H |__|¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯  | (Repeat
//     DE _______________________________________________  /  T_VD times)
//              .                                 .         "Vbp should be
//              v                                 v          static"
//              ***********************************
//              *                                 *
//     /V ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯   \  Drawn Rows
//     /H |__|¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯   |  Line 0
//     DE ______|¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯|______   /
//              *                                 *
//              *<------ DE_ACTIVE_DOTS --------->*        (Repeat V_COUNT
//              *      .                          *         =1024 times)
//                     .   Displayed Pixels
//              *      .                          *
//              *                                 *
//              *                                 *
//     /V ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯   \  Drawn Rows
//     /H |__|¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯   |  Line 1023
//     DE ______|¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯|______   /
//              *                                 *
//              ***********************************      
//                                                      "Vsync Front Porch"
//     /V ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯  \ DE->VSYNC Rows
//     /H |__|¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯  | (Repeat
//     DE _______________________________________________  /  T_DV times)
//
//
#warning "TODO Maybe: LCDdirectLVDS (which so far is the only version which synced with such low refresh rates) didn't wait for DE_ACTIVE_DOTS between HSync L->H and Vsync Low... Further, not sure the interrupts were perfectly-regular... e.g. row-calculations mighta been done after DE, before Hsync, in drawn-rows, but not in non-drawn-rows (blank), which mighta delayed some interrupts..."

// Front vs. Back porches....
//   If I were to draw a car driving down a street... 
//   I would draw it with its front to the right, driving to the right...
//
//       _____/¯¯¯¯\________     \O/
//   ...|__   _________   __\     |
//      ...\_/         \_/       / \							//
//
//   The person is *in front of* the car... which means the car has not yet
//   arrived at the person, but will soon... 
//   (later in time, not *before* in time)
//  This is my logic...
//   So The "Horizontal Front Porch", to me, should be to the RIGHT of the
//   Horizontal Sync Pulse...
//   But that ain't how it is (unless I've confused myself into a circle
//   where my logic is actually right, and I just think it's wrong?)
//   
//  By the logic that the front porch occurs *before* the sync pulse,
//   ... I suppose we could think of a person in front of a house... it
//   makes more sense, anyhow, since it's a porch in front of a house, 
//   not person in front of a car.
//   The person's walking into the house, crosses the *front*-porch 
//   *before* enterring the doorway. If he continues walking all the way
//   through the house and out the back-door, he'll be crossing the *back*
//   porch later in time...
//
// Right, so the logic makes sense... but for some reason I have to run 
//  through this whole diatribe every time I see the terms.
// Heh, and even after going through it, the damned thing seems backwards.
//  (Probably because I drew it with the front-porch to the right side...)
//  (AND THEN there's the issue that it seems to me like the front-porch
//   should be to the left of the drawing-area whereas the back-porch
//   should be to the right of it... rather than to the HSYNC pulse)
//
//
// Anyways, all that drawing was to figure out where to position this
// in memory, such that it would be the least processing-effort to modify
// timing values for testing... (because loading the SDRAM's free-running 
// data takes ~20minutes!)



//Options LTN, IDT, or TESTVALS
// IDT display *still* doesn't sync... maybe my H/V signals aren't right
#ifndef DISPLAY
#define DISPLAY TESTVALS //LTN //TESTVALS //IDT //LTN
#endif

//oldNote:
// TODO: Between OSCCAL_VAL and FRAME_UPDATE_DELAY
//  it seems the speed (at least for the LTN) isn't so important after all
//  This could be used to my advantage...
//  Maybe bump OSCCAL_VAL all the way up, but use a slower "bit-rate"
//  Thus, pixels could be skinnier, thus higher-resolution.
//   Or more instructions could be used for the same resolution
//   allowing, e.g. sprites from program-memory, without necessitating a
//   large (or any?) frame-buffer...


//This would normally be 0 for the highest-speed frame-refresh possible
// However, increasing this value can slow the frame-refresh rate
//   if desired
// e.g. 50000 causes it to refresh ~ every few seconds
//   (durn-near the limit for how long the LCD will retain an image)
// The benefit being that it doesn't flicker at annoying rates for
//  stationary images
//  worth exploring...
// This may be highly dependent on the screen capabilities...
//   of course, this sorta thing is undocumented
//   it just has to be experimented with
//   Further, it may be quite hard on the Liquid Crystals, since they'd 
//   likely be driven DC... not sure what the long term effects are.
// This could be a hell of a lot more sophisticated...
// e.g. refresh after a frameBuffer change
//      refresh based on a seconds-timer
//      ...
// AS-IS: Does refreshes completely independent of the frame-buffer
//      if it's too slow, some images may be missed completely.
//      This is not true for FRAME_SYNC TRUE (below)
//      (and it's not really useful anymore with FRAME_SYNC)
// a/o sdramThing2.0, this is not really relevent...
// 	if the frame needs updating, just stop free-running, right?
// 	well, doing-so might cause issues, since the LVDS clock will not be
//    running... this is a TBD
#define FRAME_UPDATE_DELAY 0//5000//50000 //2000 //5000 //20000 //50000


//New Idea:
// Disable the Hsync timer interrupt after the frame completes
//  and reenable elsewhere...
//  (It seems pixels changing from one shade to another take an
//   intermediate step halfway between)
//  (Thus, two updates then a pause might clear that up)
//  (Duh... it doesn't clear that up, because the fastest refresh rate
//   is the same...)
//  (but it is an interesting effect)
//  (Double-Duh... I was using a slow refresh with FRAME_UPDATE_DELAY)
//  Reworded: Having several frame refreshes before a pause helps to
//  "solidify" the color changes... especially the sharp changes 
//  (e.g. yellow<->black)
//  (Side Note: I saw this effect on a regular (NON-LVDS) TFT display 
//   in LCDdirect that was only connected such that R, G, and B were each
//   either full-off or full-on, i.e. all Red bits were connected to a
//   single output pin (full on when 1, full off when 0).
//   I hadn't attributed it to improper refresh synchronization
//   as the refresh rate was too fast to see... That's a handy way to get
//   intermediate shades/colors without refresh-flicker from pulsing them!)
//  connected with each color 
//  0 means no delay (except FRAME_UPDATE_DELAY) via interrupt disabling
//  If you WANT this effect (for intermediate shades?) 
//   set FRAME_COUNT_TO_DELAY to 1
//  Then, each change of color will (apparently) result in a value halfway
//   between the old color and the new color
//  Not sure whether it's an electrical problem, but these half-shades 
//   appear dithered, sometimes ugilly, sometimes it's nice. I guess it
//   depends on the goal...
//#define FRAME_COUNT_TO_DELAY 2//7//1//5//2//3


//A/O v13: Using the Samsung LTN display, instead of the IDTech IAXG
// LTN appears to be content with my pseudo-LVDS scheme.
// IAXG has never unblanked
//      Even though suitable timings were found with SwitchResX
//      Maybe due to psuedo-LVDS
//            slight timing issues // It probably doesn't work with all cases...
// Definitely with drawPix/Images...(RC oscillator variances?)
//            glitches when switching LVDS states 
//                 (thought I had that figured out, originally)
//      The fact the LTN appears to recognize the signalling suggests the
//      IAXG would be worth further exploration...
//         Unfortunately, the CCFT blew out my inverter
//                        And I must have put it back together incorrectly
//                        (backlight filters out of order or flipped?)
//                        (which actually makes for some very interesting
//                         visuals, but hard to develop with)
//      IAXG: uses DE, V, and H
//             At low pixel-clock DE is active for fewer pixels...
//              Last Tested: 680 was full-screen
//             Nice because it increases the frame-rate!
//      LTN:  uses DE only


// The idea is to use FastPWM with the PLL to implement 64-85Mbits/sec
//  (the PLL on the Tiny861 supposedly maxes out at 85MHz)
//  (Though I am currently running with OSCAL set to the highest frequency
//   and the PLL seems to be syncing at about 128MHz)
//  (New Note, a/o sdramThing2.0v5... it's kinda ironic that I was able to
//   run it at 128MHz with nothing more than an AVR and some gates...
//   seeing as how sdramThing has been fidgetty just running at 8MHz
//   Anyways, an eventual goal is to run at 133MHz+, using basically
//   nothing more than the SDRAM itself for timing and refreshes...
//   would allow for full resolution and full color-depth)




// Wiring:
//   Many iterations of AVR->Differential "LVDS" circuitry resulted in the
//   simplest of all:
//
//   Believe it or not, the XOR is a standard TTL LS-series XOR: 74LS86
//     Specifically: TI SN74LS86N from 1980 (the only XOR in my collection)
//   It's spec'd to run from 4.5-5.5V, and its propagation delays and slew
//     rates aren't really spec'd to be good enough for 128MHz pixel clock
//     yet it's working...
//   Further, the output voltages are right in the LVDS range,
//     IIRC (last I 'scoped) around 1.5V High and 1.0V Low
//      (Don't forget the LCD has a 100ohm resistor between
//         RXinN/clk+ and RXinN/clk-)
//
//   It's probably best to use two XORs from the same chip for a single
//    LVDS channel, since different chips may have slightly different
//    characteristics. 
//
//   The entire circuit, thus, requires TWO 74LS86's 
//    (four XORs apiece, two per LVDS channel, 8-total)
//
//
//              VCC3V3   VCC3V3
//                |        |                           ______________
//                +---\ \¯¯¯-_                        |  LCD (1 channel
//                     | |    ¯-                      |       of 4)
//                     | | XOR   >------> RXinN/clk-  | ----.
//   AVR, etc.         | |    _-                      |     |
//   3.3V TTL >---+---/ /___-¯                        |     \ 100
//   output       |                                   |     /
//                |                                   |     \ ohms
//                |                                   |     /
//                `---\ \¯¯¯-_                        |     |
//                     | |    ¯-                      |     |
//                     | | XOR   >------> RXinN/clk+  | ----'
//                     | |    _-                      |
//                +---/ /___-¯                        |
//                |        |                          |
//               GND      GND
//
//  
//  Also used: The TTL 74AHC series...
//    I found some one-gang 74AHC1G32 and 74AHC1G86's on an old iBook
//     motherboard. (An OR and an XOR, respectively). These are spec'd for
//     3.3V operation, and faster. The output voltages appear OK for LVDS
//     (with a 100ohm load in the LCD)
//
//  NOTE: Since I only had enough of these 1-gang devices for two LVDS
//     channels, I had to implement Red and Green with the LS 
//     before switching all channels to the LS... Using different chips
//     (specifically, different TYPES of chips) for different channels
//     caused timing issues: Since the AHC is faster, the clock and Blue
//     signals are synchronized, but the red and green signals were shifted
//     a bit or two (resulting in "Black" appearing green)
//
//  For other circuits attempted, see oldNotes.txt
//     (and boy there were many, involving voltage dividers, AC coupling,
//      reference voltages, BJT differential amplifiers... I doubt I 
//      documented them all, or even most. Whoda thunk the simplest, 
//      especially under- AND over-spec'd--timing, supply voltage, and
//      output voltage--would be the one...?)
//
// LVDS/FPD-Link timing:

//            |<--- (LCDdirectLVDS: "pixel") --->|
//  Timer1:   |<-- One Timer1 Cycle (OCR1C=6) -->|
//  TCNT:     |  0   1    2    3    4    5    6  |  0   1    2    3    
//            |____.____.____.____               |____.____.____.____
//  RXclk+:   /         |         \    .    .    /         |         \ //
//            |         |          ¯¯¯¯ ¯¯¯¯ ¯¯¯¯|         |
// One Pixel: |         |<--- One FPD-Link Pixel Cycle --->|
//            |                                  |
// "Blue/DVH" |____ ____v____ ____ ____v____ ____|____ ____
//  RXin2:    X B3 X B2 X DE X /V X /H X B5 X B4 X B3 X B2 X ...
//            |¯¯¯¯ ¯¯¯¯^¯¯¯¯ ¯¯¯¯ ¯¯¯¯^¯¯¯¯ ¯¯¯¯|¯¯¯¯ ¯¯¯¯
//            |         |<--Not Blue-->|         |
//            |                                  |
// "Green"    |____ ____v____ ____v____ ____ ____|____ ____
//  RXin1:    X G2 X G1 X B1 X B0 X G5 X G4 X G3 X G2 X G1 X ...
//            |¯¯¯¯ ¯¯¯¯^¯¯¯¯ ¯¯¯¯^¯¯¯¯ ¯¯¯¯ ¯¯¯¯|¯¯¯¯ ¯¯¯¯ 
//            |         |<------->|-Not Green    |
//
// "Red"      |____ ____v____v____ ____ ____ ____|____ ____
//  RXin0:    X R1 X R0 X G0 X R5 X R4 X R3 X R2 X R1 X R0 X ...
//            |¯¯¯¯ ¯¯¯¯^¯¯¯¯^¯¯¯¯ ¯¯¯¯ ¯¯¯¯ ¯¯¯¯ ¯¯¯¯ ¯¯¯¯
//            |         |<-->|-Not Red
//
//   Of course: The "Not Green/Red" bits above are least-significant-bits
//              and basically have little/no visible effect
//
//
//  Implementations/prospects:
//  * rowBuffer.c (more like row-settings-buffer)
//     calculate a row's worth of pixels before drawing that row
//     (uses packed color settings in a single byte per drawable pixel)
//     64 drawable pixels across, regardless of LVDS speed
//  * For faster pixels: these settings values could be stored 
//     in individual bytes. Gives about 1/3 more pixels at 3x the memory
//     (not implemented)
//  * rowSegBuffer.c
//     also calculates an entire row before drawing it
//     instead of storing pixels, store "segments"
//     i.e. each segment is defined by a color value and a length
//     Number of segments is limited only by memory...
//      e.g. 64 segments per row (max) is 64*3Bytes
//       Three bytes for color, and *really simple* packing for seg-length
//      BUT: at slow LVDS speeds, the resolution of these segments could be
//       as high as one LCD pixel.
//      possibly: at *really* low LVDS speeds we could be 64*2Bytes
//      (with packing)
//      Actually: Using GB_COMBINED gives 2Bytes per segment
//      only adds two clock cycles to each "pixel", so probably worth it

//   In Any Case: There's not enough RAM for a full frame
//          64 pixels across * 64 pixels down is 4096 bytes
//          So whatever method, we need to precalculate each row before
//          displaying it
//          Could be as simple as loading direct from program memory
//

// Since low-bits are barely visible, their effect is neglected.


// DE/V/H Timing (LCDdirectLVDS):
//  
//
//            |  0   1    2    3    4    5    6       All: set @ 0
//            |____.____.____.____                         OCR1C = 6 
//  Clock:    /                   \    .    .    /         Complementary-
//                                 ¯¯¯¯ ¯¯¯¯ ¯¯¯¯          Output Mode
//                                                          required for DT
//   Signal:    B3   B2   DE   /V   /H   B5   B4 | B3
//             ____ ____ ____ ____ ____ ____ ____|____    
//   DE:      X B3 X B2 /    .    .    \ B5 X B4 X B3 X    
//   state2    ¯¯¯¯ ¯¯¯¯                ¯¯¯¯ ¯¯¯¯|¯¯¯¯    
//   DE_BLUE: >¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯|    DT=X, OCR=0xff
//                         Watch the transition!! -------^
//   DE_NORM: >_________/¯¯¯¯¯¯¯¯¯¯¯¯¯¯\_________|    DT=2, OCR=4
//   DC_DISABLED:
//   maxBlue: >¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯\____|		DT=0, OCR=5
//             See below for more blue settings...
//
//             ____ ____      ____      ____ ____|____ 
//  H (only): X xx X xx \    /    \    / xx X xx X xx X    
//   state1    ¯¯¯¯ ¯¯¯¯ ¯¯¯¯      ¯¯¯¯ ¯¯¯¯ ¯¯¯¯|¯¯¯¯ 
//            >______________/¯¯¯¯\______________|    DT=3, OCR=3
//  DC_DISABLED:
//   Not much can be done...
//
//             ____ ____           ____ ____ ____|____ 
//  V w/o H:  X xx X xx \    .    /    \ xx X xx X xx X
//   state3    ¯¯¯¯ ¯¯¯¯ ¯¯¯¯ ¯¯¯¯      ¯¯¯¯ ¯¯¯¯|¯¯¯¯
//            >___________________/¯¯¯¯\_________|    DT=4, OCR=4 (+?)
// DC_DISABLED:
//            >___________________/¯¯¯¯¯¯¯¯¯\____|		DT=4, OCR=5

//             ____ ____                ____ ____|____ 
//  V w/ H:   X xx X xx \    .    .    / xx X xx X xx X
//   state4    ¯¯¯¯ ¯¯¯¯ ¯¯¯¯ ¯¯¯¯ ¯¯¯¯ ¯¯¯¯ ¯¯¯¯|¯¯¯¯ 
//            >__________________________________|    DT=X, OCR=0
//                             TransitionWatch!!! -------^
//                             Shouldn't matter... DT from no-edge
// DC_DISABLED:
//            >¯¯¯¯¯¯¯¯¯\________________________|		DT=0, OCR=1

//             ____ ____      ____ ____ ____ ____|____ 
//  Nada:     X xx X xx \    /    .    \ xx X xx X xx X    
//   state0    ¯¯¯¯ ¯¯¯¯ ¯¯¯¯           ¯¯¯¯ ¯¯¯¯|¯¯¯¯     
//            >______________/¯¯¯¯¯¯¯¯¯\_________|    DT=3, OCR=4 (+?)
// DC_DISABLED:
//            >______________/¯¯¯¯¯¯¯¯¯¯¯¯¯¯\____|		DT=3, OCR=5

//So far this is the only case I've seen:
// /Vsync
#ifndef VSYNC_ACTIVE
#define VSYNC_ACTIVE		0
#endif
#ifndef VSYNC_INACTIVE
#define VSYNC_INACTIVE	1
#endif
// /Hsync
#ifndef HSYNC_ACTIVE
#define HSYNC_ACTIVE		0
#endif
#ifndef HSYNC_INACTIVE
#define HSYNC_INACTIVE	1
#endif
// DE (active high)
#ifndef DE_ACTIVE
#define DE_ACTIVE			1
#endif
#ifndef DE_INACTIVE
#define DE_INACTIVE		0
#endif

// HERE!




// Transitions: OLD!!!!! WRONG!!!!
//   (are they? There're several additional transitions, now, for one.)
//  The idea is to reduce the number of instructions between each
//  LVDS "state."
//  These are implemented below in the case:
//    "#else //NOT FULL_INIT_TESTS"
//  Since each instruction takes *longer than* a single pixel
//    two instructions would *at best* occur on two consecutive pixels
//    Thus, there's likely to be a transition-glitch
//    (e.g. old OCR value with new DT value)
//  Note that the new DT values are implemented at the next corresponding
//    edge after the DT-write instruction completes
//    (for +OC1A/DT1H, when TCNT is reset to 0)
//    (for -OC1A/DT1L, when TCNT passes OCR1A)
//    New OCR values are delayed until the next TCNT reset to 0
//
//  Initial:
//  Nada					DT=2  NADA_OCR = (3<=OCR<(=?)6)

//  Nada	->	H			OCR=2
//  H		->	Nada		OCR=NADA_OCR
//  
//  Nada	->	V			DT=3
//  V		->	V+H		OCR=0
//  V+H	->	V			OCR=NADA_OCR
//  V		->	Nada		DT=2
//
//  Nada	->	DE			DT=1
//  DE	->	Nada		DT=2

#if 0
#if (defined(REMOVE_DC) && REMOVE_DC)
 #define Nada_init()         { DT1=(3<<4); OCR1A=5; }

 //Unused, normally...
 #define Vsync_init()       { DT1=(4<<4);  OCR1A=5; }
 #define VplusH_init()      { DT1=0;       OCR1A=1; }
 #define Hsync_init()       { DT1=(3<<4);  OCR1A=3; }
 #define DEonly_init()      { DT1=(2<<4);  OCR1A=4; }
 #define DEblue_init()		 { DT1=0;       OCR1A=5; }

#else //!REMOVE_DC
//Nada init
 #define Nada_init()         { DT1=(3<<4); OCR1A=4; }

//Unused, normally...
 #define Vsync_init()			{ DT1=(4<<4);	OCR1A=4; }
 #define VplusH_init()		{ DT1=0;			OCR1A=0; }
 #define Hsync_init()			{ DT1=(3<<4);	OCR1A=3; }
// #define DE_init()				{ DT1=(2<<4);	OCR1A=4; } //...SHOULD BE
 #define DEonly_init()			{ DT1=(2<<4);  OCR1A=4; }
 #define DEblue_init()			{ DT1=0;       OCR1A=6; }
//#define DE_init()				{ DT1=(1<<4);	OCR1A=2; } //Works with DE_ONLY
 //#define DE_init()				{ DT1=0;			OCR1A=0xff; }	//DE_BLUE
#endif //REMOVE_DC
#endif

//#define FULL_INIT_TESTS TRUE

//Display is DE-Only (doesn't use H/Vsync)
// Shouldn't be necessary to select this if it is,
// since DE timing is the same either way
// but I want to test whether a single-bit is being detected
// (e.g. maybe the rise/fall-times of the output aren't fast enough for a
//  single bit, which might explain why the other display didn't work)
//These are just examples... the project should define them:
#if 0
#if (defined(DE_ONLY_DISPLAY) && DE_ONLY_DISPLAY)
#define Vsync_fromNada()      Nada_init()
#define VplusH_fromVsync()    Nada_init()
#define Vsync_fromVplusH()    Nada_init()
#define Nada_fromVsync()      Nada_init()
#define Hsync_fromNada()      Nada_init()
#define Nada_fromHsync()      Nada_init()
#define DEonly_fromNada()     DE_init()
#define DEblue_fromDEonly()	DE_init()
#define Nada_fromDEblue()		Nada_init()
 #if(DE_BLUE)
  #warning "DE_BLUE is true, but not implemented with DE_ONLY_DISPLAY"
  #warning "...The display should be blank"
 #endif
//Use full initialization of each LVDS state
// (might not be good during transitions, but should guarantee
//  steady-state, in case my transitions aren't correct...)
#elif (defined(FULL_INIT_TESTS) && FULL_INIT_TESTS)
#define Vsync_fromNada()		Vsync_init()
#define VplusH_fromVsync()		VplusH_init()
#define Vsync_fromVplusH()		Vsync_init()
#define Nada_fromVsync()		Nada_init()
#define Hsync_fromNada()		Hsync_init()
#define Nada_fromHsync()		Nada_init()
#define DEonly_fromNada()		DEonly_init()
#define DEblue_fromDEonly()	DEblue_init()
#define Nada_fromDEonly()		Nada_init()
#define Nada_fromDEblue()		Nada_init()


#else	//NOT FULL_INIT_TESTS
//#define Vsync_fromNada()   { DT1=(4<<4); }
#define Vsync_fromNada()   { DT1=(4<<4); OCR1A=5; } //Leave two bits high
																	 //for easy-scoping
						//Three would be nicer, but I'm pretty sure OCR=TOP=ON
#define VplusH_fromVsync() { OCR1A=0; }
//#define Vsync_fromVplusH() { OCR1A=4; }
#define Vsync_fromVplusH() { OCR1A=5; }				//Extra bits for scoping
//#define Nada_fromVsync()   { DT1=(3<<4); }
#define Nada_fromVsync()   { DT1=(3<<4); OCR1A=4; } //scoping...
#define Hsync_fromNada()   { OCR1A=3; }
#define Nada_fromHsync()   { OCR1A=4; }

#define Nada_fromDEblue()	{ DT1=(3<<4); OCR1A=4; }
#define Nada_fromDEonly()	{ DT1=(3<<4); }
#define DEonly_fromNada()	{ DT1=(2<<4); }
#define DEblue_fromNada()	{ OCR1A=0xff; }
#define DEblue_fromDEonly() { OCR1A=0xff; }

#endif //End Of FULL_INIT_TESTS
#endif



// The typical patterns look like this (not at all to scale):
//   ----______------------------------------------------______-- V
//             ....
//      --_--_--_--_--_--_--_--_--_--_--_--_--_--_--_--_--_--_--_--_ H
//             ....
//      __________-__-__-__-__-__-__-__-__-__-__________________  DE
//                 
//             ^^^^\														//blah
//             1234 5?
// Pixels are sent during DE High (basically all the CPU will be used here)
//Ideally, 
//  there won't be any glitches when changing from one state to another
//
//  Init (pre 1):
//    DeadTimerRising=1
//
//  The states are:
//   (Not necessarily accurate, just looking into necessary changes)
//   (from Vsync L->H)
//
//   1  NothingActive (long, No DE, VporchFrontTimes)
//
//   2  Hsync
//
//   3  NothingActive (short)
//
//   4  DE
//
//   5  NothingActive(?)
//
//   7  Repeat 2-5 for each row
//
//   8  NothingActive (long, No DE, VporchBackTimes)
//
//   9  V w/o H
//
//   10 V w/  H
//
//   11 Repeat 9-10 for Vsync time...
//

// Here's how it worked pre-lvds:
// HSYNC, VSYNC, and DE refer to the actual pins
// In the LVDS setup, there're modes corresponding to each pin-combination
//
// Timer Interrupt:
//  loadData:
//   HSYNC active
//   HSYNC Low delay
//   HSYNC inactive
//   if(dataEnable)
//    DE active
//     Send row data
//    DE inactive
//  //Prep for next interrupt
//  switch(hsyncCount++)
//    //Vsync H->L (active)
//    1:
//          dataEnable=FALSE
//          VSYNC active
//    //Vsync L->H (inactive)
//    T_Vlow:
//          VSYNC inactive
//    //Start of frame
//    T_VD +(T_Vlow):
//          dataEnable=TRUE
//    //All rows have been displayed
//    V_COUNT +(T_VD+T_Vlow):
//          dataEnable=FALSE
//    //Frame Complete
//    T_DV +(V_COUNT+T_VD+T_Vlow): 
//          hsyncCount=0
//  if(dataEnable)
//    Use the remaining time to load the next row to memory

//  So:
//Interrupt0       End
//    |             |  Interrupt1
//    v             v  v
//  H |_|¯¯¯¯¯¯¯¯¯¯¯¯¯¯|_|¯¯¯¯¯¯¯¯¯¯¯¯¯¯|_|¯¯¯¯¯¯¯¯¯¯¯¯¯¯ ...
// DE ___________________________________________________ ...
//  V ¯¯¯¯¯¯¯¯¯¯¯¯¯|_____________________________________ ...
//       |        ||
// DETIME^--------^|
//                 |
//                 VSYNC active
//                 dataEnable=FALSE (not necessary?)
//
//InterruptT_Vlow   End
//    |             |
//    v             v
//  H |_|¯¯¯¯¯¯¯¯¯¯¯¯¯¯|_|¯¯¯¯¯¯¯¯¯¯¯¯¯¯|_|¯¯¯¯¯¯¯¯¯¯¯¯¯¯ ...
// DE ___________________________________________________ ...
//  V _____________|¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯ ...
//       |        ||
// DETIME^--------^|
//                 |
//                 VSYNC inactive
//
//
//InterruptT_VD   End  InterruptT_VD+1
//    |             |  |            End
//    v             v  v            v
//  H |_|¯¯¯¯¯¯¯¯¯¯¯¯¯¯|_|¯¯¯¯¯¯¯¯¯¯¯¯¯¯|_|¯¯¯¯¯¯¯¯¯¯¯¯¯¯ ...
// DE ____________________|¯¯¯¯¯¯¯¯|_______|¯¯¯¯¯¯¯¯|____ ...
//  V ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯ ...
//       |        ||      |        |
// DETIME^--------^|      ^--------^
//                 |
//                 dataEnable=TRUE
//
//InterruptV_COUNT  End
//    |             |
//    v             v
//  H |_|¯¯¯¯¯¯¯¯¯¯¯¯¯¯|_|¯¯¯¯¯¯¯¯¯¯¯¯¯¯|_|¯¯¯¯¯¯¯¯¯¯¯¯¯¯ ...
// DE ___|¯¯¯¯¯¯¯¯|______________________________________ ...
//  V ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯ ...
//       |        ||      |        |
// DETIME^--------^|      ^--------^
//                 |
//                 dataEnable=FALSE
//
//
// This isn't really a state, it's basically just:
// if(hsyncCount == NUM_HYSYNCS_PER_FRAME)
//    hsyncCount = 0;
//                    .....................................
//                    .                                   .
//InterruptT_DV   End .Interrupt0                         .
//    |             | .|                                  .
//    v             v .v                                  .
//  H |_|¯¯¯¯¯¯¯¯¯¯¯¯¯¯|_|¯¯¯¯¯¯¯¯¯¯¯¯¯¯|_|¯¯¯¯¯¯¯¯¯¯¯¯¯¯ .
// DE ___________________________________________________ .
//  V ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯|____________________ .
//       |        ||  .   |        |                      .
// DETIME^--------^|  .   ^--------^                      .
//                 |  .                                   .
//                 hsyncCount=0 (repeat from Interrupt0)  .
//                    .                                   .
//                    .....................................
//


// THESE STATES ARE NO LONGER ACCURATE
// ESPECIALLY THIS FIRST STATE.
// V Front Porch originally (and in these drawings) occurred at the end
// of the frame...
// More logically, it now starts at the beginning of the frame
// (Also removing this initial "glitch" of a single interrupt with
//  NothingActive)
//
// THESE STATES ARE NO LONGER ACCURATE (a/o 0.30, for certain)
// NOW to compare with LVDS states:
//
//Interrupt0       End
//    |             |  Interrupt1
//    v             v  v
//  H |_|¯¯¯¯¯¯¯¯¯¯¯¯¯¯|_|¯¯¯¯¯¯¯¯¯¯¯¯¯¯|_|¯¯¯¯¯¯¯¯¯¯¯¯¯¯ ...
// DE ___________________________________________________ ...
//  V ¯¯¯¯¯¯¯¯¯¯¯¯¯|_____________________________________ ...
//    ^ ^          ^   ^ ^
//    | |          |   | |
//    | |          |   | +--- V w/o H  \ These two toggle until
//    | |          |   +----- V w/ H   / next LCD state...
//    | |          |
//    | |          +--------- V w/o H > Intermediate change of state
//    | |
//    | +----- NothingActive  \ From Previous State (?)
//    +------- H_Only         /


//InterruptT_Vlow   End
//    |             |
//    v             v
//  H |_|¯¯¯¯¯¯¯¯¯¯¯¯¯¯|_|¯¯¯¯¯¯¯¯¯¯¯¯¯¯|_|¯¯¯¯¯¯¯¯¯¯¯¯¯¯ ...
// DE ___________________________________________________ ...
//  V _____________|¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯ ...
//    ^ ^          ^   ^ ^
//    | |          |   | |
//    | |          |   | +--- NothingActive  \ These two toggle until
//    | |          |   +----- H_Only         / next LCD state...
//    | |          |
//    | |          +--------- NothingActive > Intermediate change of state
//    | |
//    | +----- V w/o H   \ From previous state
//    +------- V w/ H    /


//InterruptT_VD   End  InterruptT_VD+1
//    |             |  |            End
//    v             v  v            v
//  H |_|¯¯¯¯¯¯¯¯¯¯¯¯¯¯|_|¯¯¯¯¯¯¯¯¯¯¯¯¯¯|_|¯¯¯¯¯¯¯¯¯¯¯¯¯¯ ...
// DE ____________________|¯¯¯¯¯¯¯¯|_______|¯¯¯¯¯¯¯¯|____ ...
//  V ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯ ...
//    ^ ^              ^ ^^        ^ 
//    | |              | ||        |
//    | |              | ||        +--- NothingActive  \  These four
//    | |              | |+------------ DE              | cycle until
//    | |              | +------------- NothingActive   | next LCD state...
//    | |              +--------------- H_Only  ^      /
//    | |                                       |
//    | +----- NothingActive  \ From previous   |
//    +------- H_Only         / state           +-This intermediate state
//                                                may not be necessary
//                                                (IAXG01 shows 160 dots!)



//InterruptV_COUNT  End
//    |             |
//    v             v
//  H |_|¯¯¯¯¯¯¯¯¯¯¯¯¯¯|_|¯¯¯¯¯¯¯¯¯¯¯¯¯¯|_|¯¯¯¯¯¯¯¯¯¯¯¯¯¯ ...
// DE ___|¯¯¯¯¯¯¯¯|______________________________________ ...
//  V ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯ ...
//    ^ ^^        ^    ^ ^
//    | ||        |    | |
//    | ||        |    | +--- NothingActive \  These two toggle
//    | ||        |    +----- H_Only        /  until next LCD state
//    | ||        |
//    | ||        +---- NothingActive \ 								//blah
//    | |+------------- DE             |  From previous state
//    | +-------------- NothingActive  |
//    +---------------- H_Only        /

// This isn't really a state, it's essentially just:
//  if(hsyncCount == NUM_HSYNCS_PER_FRAME)
//    hsyncCount = 0;
//                    .....................................
//                    .                                   .
//InterruptT_DV   End .Interrupt0                         .
//    |             | .|                                  .
//    v             v .v                                  .
//  H |_|¯¯¯¯¯¯¯¯¯¯¯¯¯¯|_|¯¯¯¯¯¯¯¯¯¯¯¯¯¯|_|¯¯¯¯¯¯¯¯¯¯¯¯¯¯ .
// DE ___________________________________________________ .
//  V ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯|____________________ .
//    ^ ^             .                                   .
//    | |             .....................................
//    | +--- NothingActive \											//blah
//    +----- H_Only        / From previous state...



//  LVDS State transitions:
//   1 H_Only
//   2 NothingActive
//
//   3 V w/o H
//   4 V w/ H
//    
//   (repeat 3,4)
//
//   5 V w/o H
//
//   6 NothingActive
//   7 H_Only
//
//   (repeat 6,7)
//
//   8 NothingActive
//
//   9 H_Only
//  10 NothingActive
//  11 DE
//  12 NothingActive
//
//  (repeat 9-12)
//
//  13 H_Only
//  14 NothingActive
//
//  (repeat 13-14)
//
//   These should be reviewed to determine which changes are necessary
//   between each state...
//   (the fewer changes, the less likely we'll glitch...?)

uint8_t dataEnable = 0;
uint8_t vSync = 0;
uint16_t hsyncCount = 0;

//frameCount=0 can be used in main to detect whether we've completed a 
//  a frame AND its FRAME_UPDATE_DELAY
#if (defined(FRAME_COUNT_TO_DELAY) && (FRAME_COUNT_TO_DELAY != 0))
#error "FRAME_COUNT_TO_DELAY isn't really an lcdStuff thing anymore, is it?"
uint8_t frameCount = 0;
#endif

void lcd_init(void)
{
	//Not sure modifying vSync randomly is safe...
	vSync = 0;
	//the rest should be...? assuming it occurs after lcd_update() completes
	// (thus leaving it in NADA-state)
	hsyncCount = 0;
	dataEnable = FALSE;
 	Nada_init();
}


#define TESTVALS	0
#define LTN 1
#define IDT 2


//TIMING:
//  Measurements below (in microseconds) are referring to preLVDS
//  There may be other such old-notes...


//        |<--T_Hhigh-->| |<-T_Hlow
//    __   _____________   ____________  3.6V
// H    |_|             |_|           _  0V
//
//    Data Enable
//       Only active when there's valid pixel data
//       (For a dual-pixel 1024x768 display:
//         for 512 of the 717 clocks in HSYNC-active)
//       Low during Vsync... (~720us)
//

//IDTech display: 136 dots
//2-136+
//IDTech, last used: 40
//LTN Last used 8
// LTN: 1072<=1344<=1500 clocks per line, 1024DE... Hsync unused
// SEE Hlow_Delay() notes if this value is large!

#ifndef H_LOW_DOTS
#if (DISPLAY == LTN)
 #define H_LOW_DOTS	8
#elif (DISPLAY == IDT)
 #define H_LOW_DOTS   40//8//1//40 //30 //2//136
#elif (DISPLAY == TESTVALS)
 #define H_LOW_DOTS	100//0 //LTN doesn't use H_LOW (DE-only)
							  // And having a value here increases codesize
#endif
#endif

//H is set low immediately upon entry of the timer interrupt
// its low-time is controlled via nops
// (so if HLow is long for a particular display, this might need revising)

#if((!defined(HLOW_DELAY_OVERRIDE) || !HLOW_DELAY_OVERRIDE) \
		                                         && !defined(Hlow_Delay))
 #define Hlow_Delay() \
	delay_Dots(H_LOW_DOTS)
#endif




//This was found expermentally with SwitchResX
//IDTech Last Used: 680
// (IDTech had some oddities regarding slow pixel-clocks, where pixels
//  would be repeated, regardless of the pixel-clock, thus, for slow clocks
//  it has the effect of *stretching* the image, thus 680 "pixels" across
//  instead of its 1024 *physical* pixels)
//LTN last used 1024
// Oddly: For the LTN, this doesn't seem to affect the horizontal
//        it does, however, affect the vertical(!)
// If I recall Correctly, this is used only for setting the timer interrupt
//  rate...
#ifndef DE_ACTIVE_DOTS
#if (DISPLAY == LTN)
 #define DE_ACTIVE_DOTS	1024
#elif (DISPLAY == IDT)
 #define DE_ACTIVE_DOTS	680 //1024 //800 //920 //943 //812 //472 //1024
#elif (DISPLAY == TESTVALS)
 #define DE_ACTIVE_DOTS	1024
#endif
#endif

//Horizontal Blank Time: HD_DOTS + H_LOW + DH_DOTS
//  90-320+
//Horizontal Total Time: 1206<=1344<=2047
// ... but Total = Blank + Pixels, so 90+1024 != 1206
// Excess waits should be thrown in DH (since that's between interrupts)
//T_HD is the time from H inactivated to DE activated
//aka H back-porch: 1-160+
//IDTech Last Used: 50
//LTN last used 5

#ifndef HD_DOTS
#if (DISPLAY == LTN)
 #define HD_DOTS 5
#elif (DISPLAY == IDT)
 #define HD_DOTS	50 //5 //0//20//5 //50 //30 //1 //160
#elif (DISPLAY == TESTVALS)
 #define HD_DOTS 50//5
#endif
#endif

#ifndef HD_Delay
 #define HD_Delay()	delay_Dots(HD_DOTS)
#endif

// I think this was just a mistake. This change to delay_Dots a/o 0.10ncf
// (untested in LCDdirectLVDS)
//delay_cyc(HD_DOTS)


// This is the number of CPU cycles between DE->L and Horiz->L
// This is just used in calculating the OCR value...
// aka H Front Porch: 0-24+
// IDTech Last Used: 24
// LTN last used 46
// 1072-1024-1-1 = 46
#ifndef DH_DOTS
#if (DISPLAY == LTN)
 #define DH_DOTS	46
#elif (DISPLAY == IDT)
 #define DH_DOTS	24 //46 //480//6//12//24 //46 //24 //122//30 //0 //24
#elif (DISPLAY == TESTVALS)
 #define DH_DOTS	0//1000//100//46
#endif
#endif

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
//LTN Frame: 772<=806<=1000 lines...

#ifndef T_VD
#if (DISPLAY == LTN)
 #define T_VD 3
#elif (DISPLAY == IDT)
 #define  T_VD 29 //3//29 //7 //29
#elif (DISPLAY == TESTVALS)
 #define T_VD 3
#endif
#endif

//aka V-sync front porch: 1-3+
// IDTech Last Used: 3
// LTN last used 3
#ifndef T_DV
#if (DISPLAY == LTN)
 #define T_DV 3
#elif (DISPLAY == IDT)
 #define T_DV 3//3 //1 //3
#elif (DISPLAY == TESTVALS)
 #define T_DV 3
#endif
#endif

//1-6+ (1 is used by the first switch-case...)
// IDTech Last Used: 6
// LTN Last used 16
// 772 - 768 - 1 - 1 = 2
#ifndef T_Vlow
#if (DISPLAY == LTN)
 #define T_Vlow 16
#elif (DISPLAY == IDT)
 #define T_Vlow	6 //16//6 //2 //6
#elif (DISPLAY == TESTVALS)
 #define T_Vlow 32
#endif
#endif

// IDTech Last Used: 768
#ifndef V_COUNT
#if (DISPLAY == LTN)
 #define V_COUNT	768
#elif (DISPLAY == IDT)
 #define V_COUNT	768
#elif (DISPLAY == TESTVALS)
// #if (LVDS_PRESCALER == 8)
  #define V_COUNT 768
// #else
//  #define V_COUNT	(768*2/3)
// #endif
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



#define HSYNC_COUNT_FRAME_COMPLETE  \
	(T_DV +(V_COUNT+T_VD+T_Vlow) + FRAME_UPDATE_DELAY)
// T_DV = 3
// V_COUNT = 768 
// T_VD = 3
// T_Vlow = 32
// FRAME_UPDATE_DELAY = 0
// TOTAL = 806

#if(FRAME_UPDATE_DELAY != 0)
 #error "FRAME_UPDATE_DELAY is no longer implemented..."
#endif


//#if(HSYNC_COUNT_FRAME_COMPLETE & 0x01)
// #warning "Total vertical lines might need to be even!"
 //This should be fixed...
//#endif


#define DOT_COUNT_PER_HSYNC \
	(H_LOW_DOTS + HD_DOTS + DE_ACTIVE_DOTS + DH_DOTS)
// H_LOW_DOTS = 100
// HD_DOTS = 50 
// DE_ACTIVE_DOTS = 1024
// DH_DOTS = 0
// TOTAL = 1174


//#define MAKELONG (0L)

#define DOT_COUNT_PER_FRAME \
 ((MAKELONG(HSYNC_COUNT_FRAME_COMPLETE)) * (MAKELONG(DOT_COUNT_PER_HSYNC)))
// TOTAL = 946,244

//Hlow_Delay 		H_LOW_DOTS
//HD_Delay			HD_DOTS	
//DE_ACTIVE_DOTS
//DH_DOTS



static __inline__ \
void loadData(uint16_t rowNum, uint8_t dataEnable) \
	  __attribute__((__always_inline__));



/*
static __inline__ \
void loadRow(uint16_t rowNum) \
	__attribute__((__always_inline__));
*/

//Returns TRUE when the frame is complete
// Should send nada (and maybe hsyncs?) thereafter(?)
// In sdramThing2.0, when this returns TRUE, the SDRAM loading is complete
// EXCEPT: the final HD_Dots' FR_READ/ACTIVATE commands will point to a
//  non-existent "next" Hsync... these commands need to be rewritten to
//  point to the "first" Hsync. This is handled in sdramThing2.0's code
// Used to be:
//SIGNAL(TIMER0_COMPA_vect)
//a/o v0.60: lcd_update() now returns the row-number (0 to V_COUNT-1)
//           Not *entirely* accurate, as it may be negative during VBLANK
//           OR returns LCD_FRAMECOMPLETE when that's the case.
// DOES THIS RETURN THE ROW THAT WAS DRAWN OR THE NEXT...?
#define LCD_FRAMECOMPLETE	INT16_MAX
int16_t lcd_update(void)
{
//	uint8_t frameComplete = FALSE;

#warning "this might be off by one, which would explain the weird first row"
	int16_t rowNum = hsyncCount - T_DV-T_VD-T_Vlow;
	//static uint8_t frameCount = 0;

#if 0 // (defined(SLOW_EVERYTHING_TEST) && SLOW_EVERYTHING_TEST)
	while(TCNT0L != 7){};
	//This won't work... who knows how many cycles interrupt-entry will be
	//Above should fix that...
	//I think this was just an indicator for scope triggering...
	// not relevent for 
	setpinPORT(PB2, PORTB);
	clrpinPORT(PB2, PORTB);
#endif

	//Hsync and Actually draw the pixels... (when DE is active)
	// This takes a LONG time and will occupy most of the CPU...
	//  ... IN THIS INTERRUPT
	//loadData(((hsyncCount-T_VD-T_Vlow)>>3)&31, dataEnable,colorOverride);
//	loadData(hsyncCount-T_DV-T_VD-T_Vlow, dataEnable);
	loadData(rowNum, dataEnable);

	hsyncCount++;

	switch(hsyncCount)
	{
		//V Front Porch, begins with initLCD

		//Vsync H->L (Begin V-Sync)
		case T_DV:
			dataEnable = FALSE;
			//clrpinPORT(VSYNC_PIN, VSYNC_PORT);
			vSync = TRUE;
			Vsync_fromNada();
			break;
		//Vsync L->H (Begin V Back Porch)
		case T_Vlow + T_DV:
		//	if(hfm_nextOutput(&modulator))
		//		colorOverride = 0xff;
		//	else
		//		colorOverride = 7;
			//setpinPORT(VSYNC_PIN, VSYNC_PORT);
			vSync = FALSE;
			Nada_fromVsync();
			break;
		//Start of frame (DE active)
		case T_VD +(T_Vlow + T_DV):
			dataEnable = TRUE;
			break;
		// All rows have been displayed
		// Return to V Front Porch
		case V_COUNT +(T_VD + T_Vlow + T_DV):
			dataEnable = FALSE;
		//	break;
		//Frame complete
		//case HSYNC_COUNT_FRAME_COMPLETE:
			//T_DV +(V_COUNT+T_VD+T_Vlow) + FRAME_UPDATE_DELAY:
			hsyncCount = 0;
#if 0 //(FRAME_COUNT_TO_DELAY != 0)
			//static uint8_t frameCount = 0;
			frameCount++;
			if(frameCount == FRAME_COUNT_TO_DELAY)
			{
				frameCount = 0;
				timer_compareMatchIntSetup(0, OUT_CHANNELA, FALSE);
				//timer_compareMatchIntDisable(0, OUT_CHANNELA);
			}
#endif
			
//#warning "NewNote: WTF, are we relying on loadData to have properly set the mode?"
// Yeah, guess that makes sense... Must be Nada...
			//frameComplete = TRUE;
			rowNum = LCD_FRAMECOMPLETE;
			break;
		default:
			break;
	}


//a/o 0.56ncf-1 (LCDdirectLVDS70)
// the call to loadRow is handled in the main project... and maybe it's
// best that way.

#if 0 //(defined(LOADROW) && LOADROW)
	//Load the next row into the row(seg)buffer
	//data is enabled after T_VD+T_Vlow...
	if((hsyncCount >= T_VD+T_Vlow) && (hsyncCount < T_VD+T_Vlow+V_COUNT))
		loadRow(hsyncCount - (T_VD+T_Vlow));
#endif

	//This note was written long before the above was commented-out...
	//So...?
	// Here is where loadRow used to be called
	// it has been moved to oldNotes.txt, but it would make more sense
	// to look at LCDdirectLVDS <25 or LCDdirect
	// loadRow used the time between DE inactive and the next Hsync
	// to load a row to a row-buffer (as opposed to a frame-buffer)
	// thus, DE could gain more pixels because there would be fewer
	// calculations to determine the memory location to write from
	// This effect has since been minimized
	// BUT loadRow might be necessary for program-memory-based images...

	return rowNum; //frameComplete;
}


//See LCDdirectLVDS for an example...
// basically it loads a single row into a buffer for the next dateEnable
//  (either the row-buffer or the row-segment-buffer) 
// This has been *greatly* reduced, is not at all functional, just
// to give an idea of loadRow's purpose.
#if 0 //(defined(LOADROW) && LOADROW)
void loadRow(uint16_t rowNum)
{
	//e.g....
	tet_drawRow(rbRowNum, rowBuffer);
	
	//and...	
	rowBuffer[i] = fb_to_rb(colorVal);
	
	//and...
	if((rbRowNum >= SMILE_TOP_ROW) &&
			(rbRowNum < (SMILE_HEIGHT+SMILE_TOP_ROW)))
	{
			for(i=0; i<FB_WIDTH; i++)
			{
         	temp=pgm_readImageByte(pimage, rbRowNum-SMILE_TOP_ROW, i);
				if(!(temp & 0xC0))
					rowBuffer[i+SMILE_SHIFT] = fb_to_rb(temp);
			}
	}

	life_drawRow(rbRowNum, &(rowBuffer[RB_WIDTH-LIFE_WIDTH]));

	//HORIZONTAL_COLOR_BARS
	for(i=0; i<RB_WIDTH; i++)
		rowBuffer[i] = fb_to_rb(rowNum);

	//Good for syncing to have white on both borders...
	newSeg(1, 0x06, (6<<4) | 3);
	
}
#endif //LOAD_ROW


//Including inactive pixel-clocks (timing)
// DotCount per row...
//  TOTAL_CYC_COUNT was used for the timer-interrupt in LCDdirectLVDS
//  This was DOTS_TO_CYC(DISPLAY_DOT_COUNT) + ROW_CALCULATION_CYCS
//  ROW_CALCULATION_CYCS was 50,000 (!!!)
//   DOTS_TO_CYC(dots), last-used, was dots*7 (LVDS_PRESCALER nullified /8)
//   So, the overall dot-count per row was... 
//     DISPLAY_DOT_COUNT + ROW_CALCULATION_DOTS
//     where ROW_CALCULATION_DOTS = 50,000cyc/7 = 7142 dots
//     So... the overall dot-count per row was *MUCH* longer than usual
//  Pretty certain there's not nearly enough memory to simulate that
//   (in sdramThing2.0)
//   But It does suggest that it would be worthwhile to try to stretch
//   the rows as much as possible...
//   During this time, NADA is sent normally, (V=H, H=H, DE=L)
//    Or Vsync (V=L, H=H, DE=L)
//   Maybe something could be simulated with PWM again, if necessary...
//   (Certainly not at full-speed)
#define DISPLAY_DOT_COUNT \
	(H_LOW_DOTS + HD_DOTS + DE_ACTIVE_DOTS + DH_DOTS)

#if 0

//#define T_Hlow_CYC 1//8 //(T_Halow_REAL*3)
#define T_Hlow_CYC DOTS_TO_CYC(H_LOW_DOTS)

#define T_DE_CYC	DOTS_TO_CYC(DE_ACTIVE_DOTS)

//#define T_HD_CYC 1//3//(T_HD_REAL*3)
#define T_HD_CYC	DOTS_TO_CYC(HD_DOTS)

#define T_DH_CYC	DOTS_TO_CYC(DH_DOTS)





void init_timer0Hsync(void)
{
	//T_HD + T_DH + T_low + T_DE
	// For the original system, 19.865us = 695 dot-clocks
	// We'd need to take advantage of the 16-bit timer
	//  OCR1A may not be written properly (with a temporary high-byte)
	//  ... not sure.
	// TIMER0 DOES NOT HAVE CTC MODE in 16-bit mode

// This is a hack, for testing syncability with long row-calculations
// before actually drawing it... e.g. loading from program memory
// This value needs to be larger than the calculation time
// otherwise, it seems, syncing isn't stable
// BUT THIS IS A HACK, has nothing to do with actual calculation time
// And for slow LVDS or whatnot, this may be unnecessarily HUGE

#define DISPLAY_CYC_COUNT	(DOTS_TO_CYC(DISPLAY_DOT_COUNT))
//	(T_Hlow_CYC + T_HD_CYC + T_DE_CYC + T_DH_CYC)

//NOTE that with longer ROW_CALCULATION_CYCS, lines can be repeated...
// apparently an effect of the display, definitely not a code-thing
// in which case the vertical resolution decreases
// though, this could be used to advantage... 
// (e.g. more processing time outside the drawing interrupts?)
// NOTE that if the value is too small and calculations take longer in some
// lines than others (?), those lines may repeat, or syncing could be off
#if (LVDS_PRESCALER == 8)
 //#define ROW_CALCULATION_DELAY 1//20
 #define ROW_CALCULATION_CYCS (50000) //0 //(70000) //(100000)
#else
 //#define ROW_CALCULATION_DELAY 9//7//5//2//1//10
 #define ROW_CALCULATION_CYCS	(8*DISPLAY_CYC_COUNT)
#endif

#define TOTAL_CYC_COUNT \
	(ROW_CALCULATION_CYCS + DISPLAY_CYC_COUNT)
//	(ROW_CALCULATION_DELAY * (T_Hlow_CYC + T_HD_CYC + T_DE_CYC + T_DH_CYC))
//#define TOTAL_CYC_COUNT 2000
#if (TOTAL_CYC_COUNT > 255)
 //Adding +7 causes it to round up (hopefully)
 #define TIMER0_TCNTS ((TOTAL_CYC_COUNT+7)/8)
 #define TIMER0_CLKDIV	CLKDIV8
 #warning "TIMER0_TCNTS = (TOTAL_CYC_COUNT+7)/8: Timer0 CLKDIV8"
 #if (TIMER0_TCNTS > 255)
  #warning "TIMER0_TCNTS = (TOTAL_CYC_COUNT+63)/64: Timer0 CLKDIV64"
  #define TIMER0_TCNTS ((TOTAL_CYC_COUNT+63)/64)
  #define TIMER0_CLKDIV CLKDIV64
  #if (TIMER0_TCNTS > 255)
	 #warning "TIMER0_TCNTS = (TOTAL_CYC_COUNT+255)/256: Timer0 CLKDIV256"
	 #define TIMER0_TCNTS ((TOTAL_CYC_COUNT+255)/256)
	 #define TIMER0_CLKDIV CLKDIV256
	 #if (TIMER0_TCNTS > 255)
		#warning "TIMER0_TCNTS = (TOTAL_CYC_COUNT+1023)/1024: Timer0 CLKDIV1024"
		#define TIMER0_TCNTS ((TOTAL_CYC_COUNT+1023)/1024)
		#define TIMER0_CLKDIV	CLKDIV1024
	 #endif
  #endif
 #endif
 #if (TIMER0_TCNTS == 0)
  #error "TIMER0_OCRVAL == 0"
 #endif
#else
 #define TIMER0_TCNTS TOTAL_CYC_COUNT
 #define TIMER0_CLKDIV CLKDIV1
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
 #if (((TIMER0_TCNTS + 1) % 7) == 0)
  #warning "(TIMER0_TCNTS + 1) % 7 == 0"
  #warning "   so... TIMER0_OCRVAL = TIMER0_TCNTS"
  #define TIMER0_OCRVAL (TIMER0_TCNTS)

 #elif (((TIMER0_TCNTS + 2) % 7) == 0)
  #warning "(TIMER0_TCNTS + 2) % 7 == 0"
  #warning "   so... TIMER0_OCRVAL = TIMER0_TCNTS + 1"
  #define TIMER0_OCRVAL (TIMER0_TCNTS + 1)

 #elif (((TIMER0_TCNTS + 3) % 7) == 0)
  #warning "(TIMER0_TCNTS + 3) % 7 == 0"
  #warning "   so... TIMER0_OCRVAL = TIMER0_TCNTS + 2"
  #define TIMER0_OCRVAL (TIMER0_TCNTS + 2)

 #elif (((TIMER0_TCNTS + 4) % 7) == 0)
  #warning "(TIMER0_TCNTS + 4) % 7 == 0"
  #warning "   so... TIMER0_OCRVAL = TIMER0_TCNTS + 3"
  #define TIMER0_OCRVAL (TIMER0_TCNTS + 3)

 #elif (((TIMER0_TCNTS + 5) % 7) == 0)
  #warning "(TIMER0_TCNTS + 5) % 7 == 0"
  #warning "   so... TIMER0_OCRVAL = TIMER0_TCNTS + 4"
  #define TIMER0_OCRVAL (TIMER0_TCNTS + 4)

 #elif (((TIMER0_TCNTS + 6) % 7) == 0)
  #warning "(TIMER0_TCNTS + 6) % 7 == 0"
  #warning "   so... TIMER0_OCRVAL = TIMER0_TCNTS + 5"
  #define TIMER0_OCRVAL (TIMER0_TCNTS + 5)

 #elif (((TIMER0_TCNTS) % 7) == 0)
  #warning "(TIMER0_TCNTS) % 7 == 0"
  #warning "   so... TIMER0_OCRVAL = TIMER0_TCNTS - 1"
  #define TIMER0_OCRVAL (TIMER0_TCNTS - 1)

 #else
  #error "WTF?"
 #endif
#else
 #define TIMER0_OCRVAL (TIMER0_TCNTS-1)
#endif

#if (TIMER0_OCRVAL > 255)
 #error "Despite all my efforts, TIMER0_OCRVAL is *still* > 255!"
#endif

   OCR0A = TIMER0_OCRVAL; //T_Hlow_CYC + T_HD_CYC + T_DE_CYC + T_DH_CYC;
	timer_setWGM(0, WGM_CLR_ON_COMPARE);
   timer_selectDivisor(0, TIMER0_CLKDIV); //CLKDIV1);
   timer_compareMatchIntEnable(0, OUT_CHANNELA);
}

#define fullBlue()	DEblue_init()
#endif


//Either use one of the ones here, or define your own...
// rowNum is the drawable row number on the LCD...
//static __inline__ void drawPix(uint16_t rowNum);

// This should have a uint16 rowNum!
// But, so far, the only cases which use all 768 rows are LCDdirectLVDS
// with RowSegBuffer, and sdramThing (how did this uint8_t thing not cause
// trouble there?!)
// RowSegBuffer doesn't pay attention to rowNum in drawPix...


static __inline__ \
void drawPix(uint16_t rowNum) \
	  __attribute__((__always_inline__));
#if 0
#if (!defined(ROW_SEG_BUFFER) || !ROW_SEG_BUFFER)
//drawPix loads each pixel individually... It doesn't use a for-loop
// since that would take away from actual pixel-drawing time...
// I think it's been removed... but here's a (non-functional) example...
void drawPix(uint8_t rowNum)
{
	writeColor(*(color+1));	 
	writeColor(*(color+2));	 
	writeColor(*(color+3)); 
	...
	writeColor(*(color+61));
	writeColor(*(color+62));
	writeColor(*(color+63));
	// WriteColor writes the pixel *after* the calculations...
	// thus the pixel appears basically after writeColor completes
	// These nops assure the 64th pixel is fully-displayed before exitting
	asm("nop");
	...
	asm("nop");

/*		reg[17] = colorBuffer[rowNum][17];                         
		writeColor(reg[17]);                         
		...
		reg[20] = colorBuffer[rowNum][20];                         
		writeColor(reg[20]);  
		
		//REPEATING to fill screen... (delayDots = 342 worked prior to this)
		reg[0] = colorBuffer[rowNum][0];
		writeColor(reg[0]);
		...
		reg[10] = colorBuffer[rowNum][10];
		writeColor(reg[10]);
*/
		//Display the rest as black...
		writeColor(0);
}
#else		//ROW_SEG_BUFFER
void drawPix(uint8_t rowNum)
{
	TCCR1A = ( (0<<COM1A1) | (1<<COM1A0)
	         | (0<<COM1B1) | (1<<COM1B0)
	         | (1<<PWM1A) | (1<<PWM1B) );

/* This bit was in loadData, just some notes, now...

 WTF, how did this work?! Am I not using drawPix?!
   Yes, but drawPix calls drawSegs, which doesn't have an argument
   because loadRow takes the row argument elsewhere, drawSegs just
   draws 'em
      //#warning "see 'shouldBe' here..."
      //Should be:
      rowNum = rowNum*FB_HEIGHT/V_COUNT;
      //rowNum = rowNum / (768/FB_HEIGHT);

	So, rowNum change left-over from the ol' FrameBuffer days had no effect
	on drawSegs.
*/



		drawSegs();

#endif
		//DE->Nada transition expects fullBlue...
		//Also helps to show the edge of the DE timing...

		//!!! Not sure what the state is at this point...
		// could be any DE+Blue level, or could be NADA...
		// Nada: DT1=3, still leaves one bit for clocking, might be OK
         
		//Among the things that don't make sense...
		// This appears to go into affect BEFORE delay_cyc (?)
		// as, without a pull-up resistor on the /OC1B output, 
		// green seems to be floating between the last pixel and the
		// delay_cyc (!)
		//Disable complementary-output for Green 
		//  (on /OC1B, where CLK is OC1B)
		// Since Nada, V, and H DT's might be bad for clocking.
		TCCR1A = ( (0<<COM1A1) | (1<<COM1A0)
         | (1<<COM1B1) | (0<<COM1B0)
         | (1<<PWM1A) | (1<<PWM1B) );

		fullBlue();
}
#endif //0


//
//          G) groovy little spiral...
//

#if 0 //(defined(CLOCK_INSENSITIVITY_TESTING) && CLOCK_INSENSITIVITY_TESTING)
		DEonly_fromNada();
		//Enable complementary-output for Green (on /OC1B, where CLK is OC1B)
		TCCR1A = ( (0<<COM1A1) | (1<<COM1A0)
				   | (0<<COM1B1) | (1<<COM1B0)
					| (1<<PWM1A) | (1<<PWM1B) );
#endif









/* These are old tests from above, under if(dataEnable)
   They should all be implemented in lcdStuff.c, but have not been
   retested since...
#if(defined(BLUE_DIAG_BAR) && BLUE_DIAG_BAR)
      uint16_t blueCyc = DOTS_TO_CYC(rowNum);
      uint16_t notBlueCyc = DOTS_TO_CYC(DE_ACTIVE_DOTS)-blueCyc;
      
      DEonly_fromNada();
      delay_cyc(notBlueCyc);
      DEblue_fromDEonly();
      delay_cyc(blueCyc);

      Nada_fromDEblue();
#elif(defined(BLUE_VERT_BAR) && BLUE_VERT_BAR)
      DEonly_fromNada();
      delay_cyc(DOTS_TO_CYC(DE_ACTIVE_DOTS)/3);
      DEblue_fromDEonly();
      delay_cyc(DOTS_TO_CYC(DE_ACTIVE_DOTS)*2/3);
      Nada_fromDEblue();
#elif(defined(DE_BLUE) && DE_BLUE)
      DEblue_fromNada();
      delay_cyc(DOTS_TO_CYC(DE_ACTIVE_DOTS));
      Nada_fromDEblue();
#else  //NOT BLUE_DIAG_BAR, BLUE_VERT_BAR, NOR DE_BLUE
 #if ( defined(COLOR_BAR_SCROLL) && COLOR_BAR_SCROLL)
      if(rowNum <= 256)
         rowNum=0;
      else
         rowNum=1;
 #else //NOT COLOR_BAR_SCROLL... drawPix...
*/



//a/o LCDdirectLVDS62:
// These BLUE_... tests are being reimplemented.
// They're *quite* handy for initial-testing of an untested display.
//
// Realistically, they should probably be less LVDS-specific
// (non-LVDS displays needn't be coded with these transition-specific
// functions like DEonly_fromNada() or DEblue_fromDEonly(), and certainly
// needn't be limited to blue/black testing. LVDS displays contain their
// syncing signals on the same wire as the blue pixel-data, thus these
// tests...)
// In the long-run, maybe they should be moved to an lvdsLCD commonCode
// and the transition-specific code removed from here, but I've yet to
// explore this.
// For non-LVDS displays, these should still be pretty good tests, and
// implementing transition-specific functions shouldn't be too difficult...
// e.g. "#define DEonly_fromNada()    setpinPORT(DE_PIN, DE_PORT)"
// or   "#define DEblue_fromDEonly()  BLUE_PORT = 0xff"
// (Actually, transition-specific functions make it faster regardless of
// the LVDS/non-LVDS nature of the display, as some things are redundant...
// e.g. if calls to DEonly_fromNada() were replaced with calls to DEonly(),
// it would require disabling H-sync and V-sync, in addition to enabling DE
// in every call. When we start getting into worrying about aligning pixels
// with the edge of the display and making them as skinny as possible, each
// cpu instruction counts.
// So, I guess, the only LVDS-specific stuff here is the blue-related
// transitions (and, more specifically, the fact that there are *only*
// blue-related transitions, as opposed to red and green)

//a/o sdramThing2.0v8 early... appears to be syncing on Blue signal
// thus we get diagonal data including Hsync (colored in red) along the
// as a diagonal stripe at the right
#if(defined(BLUE_DIAG_BAR) && BLUE_DIAG_BAR)
static __inline__ void drawPix(uint16_t rowNum)
{
		//uint16_t blueCyc = DOTS_TO_CYC(rowNum);
		//uint16_t notBlueCyc = DOTS_TO_CYC(DE_ACTIVE_DOTS)-blueCyc;
		uint16_t notBlueDots = rowNum;
		uint16_t blueDots = DE_ACTIVE_DOTS - notBlueDots;

		DEonly_fromNada();
		//a/o v0.70-1, swapping blue and not-blue...
		//This is a bit of a hack, since DEblue_fromNada doesn't exist...
		DEblue_fromDEonly();

		//delay_cyc(notBlueCyc);
		//delay_Dots(notBlueDots);
		DE_DotDelay(blueDots);
		DEonly_fromDEblue();
		//delay_cyc(blueCyc);
		//delay_Dots(blueDots);
		DE_DotDelay(notBlueDots);
		Nada_fromDEonly();
}
#endif


//a/o LCDdirectLVDS62: This is the same as above, but scrolls vertically
// one line per refresh... so it's easy to calculate the refresh-rate
#if(defined(BLUE_DIAG_BAR_SCROLL) && BLUE_DIAG_BAR_SCROLL)
static __inline__ void drawPix(uint16_t rowNum)
{
	static uint16_t rowOffset;
	if(rowNum == 0)
		rowOffset++;
		//uint16_t blueCyc = DOTS_TO_CYC(rowNum);
		//uint16_t notBlueCyc = DOTS_TO_CYC(DE_ACTIVE_DOTS)-blueCyc;
		uint16_t notBlueDots = (rowNum + rowOffset)%DE_ACTIVE_DOTS;
		uint16_t blueDots = DE_ACTIVE_DOTS - notBlueDots;

		DEonly_fromNada();
		DEblue_fromDEonly();
		//delay_cyc(notBlueCyc);
		//delay_Dots(notBlueDots);
		DE_DotDelay(blueDots);
		DEonly_fromDEblue();
		//delay_cyc(blueCyc);
		//delay_Dots(blueDots);
		DE_DotDelay(notBlueDots);
		Nada_fromDEonly();
}
#endif



//a/o LCDdirectLVDS62: This is the same as above, but alternates colors
//between each frame...
// (Careful, this one'll cause seizures!)
#if(defined(BLUE_DIAG_SCROLL_FLASH) && BLUE_DIAG_SCROLL_FLASH)
static __inline__ void drawPix(uint16_t rowNum)
{
	static uint16_t rowOffset;
	if(rowNum == 0)
		rowOffset++;
		//uint16_t blueCyc = DOTS_TO_CYC(rowNum);
		//uint16_t notBlueCyc = DOTS_TO_CYC(DE_ACTIVE_DOTS)-blueCyc;
		uint16_t blueDots = (rowNum + rowOffset)%DE_ACTIVE_DOTS;
		uint16_t notBlueDots = DE_ACTIVE_DOTS - blueDots;

#if(defined(BDSF_ALTERNATE_ROWS) && BDSF_ALTERNATE_ROWS)
	if(rowNum&0x01)
#else
	if(rowOffset&0x01)
#endif
	{
		DEonly_fromNada();
		//delay_cyc(notBlueCyc);
		//delay_Dots(notBlueDots);
		DE_DotDelay(notBlueDots);
		DEblue_fromDEonly();
		//delay_cyc(blueCyc);
		//delay_Dots(blueDots);
		DE_DotDelay(blueDots);
		Nada_fromDEblue();
	}
	else
	{
		DEblue_fromNada();
		//delay_cyc(notBlueCyc);
		//delay_Dots(notBlueDots);
		DE_DotDelay(notBlueDots);
		DEonly_fromDEblue();
		//delay_cyc(blueCyc);
		//delay_Dots(blueDots);
		DE_DotDelay(blueDots);
		Nada_fromDEonly();
	}
}
#endif


//a/o sdramThing2.0v8-31ish: LooksPromising, but delayed
// also LVDS bit-shifts causing vertical striping
#if(defined(BLUE_VERT_BAR) && BLUE_VERT_BAR)
static __inline__ void drawPix(uint16_t rowNum)
{
		DEonly_fromNada();
		//delay_cyc(DOTS_TO_CYC(DE_ACTIVE_DOTS)/3);
		//delay_Dots(DE_ACTIVE_DOTS/3);
		DE_DotDelay(DE_ACTIVE_DOTS/3);
		DEblue_fromDEonly();
		//delay_cyc(DOTS_TO_CYC(DE_ACTIVE_DOTS)*2/3);
		//delay_Dots(DE_ACTIVE_DOTS - DE_ACTIVE_DOTS/3);
		DE_DotDelay(DE_ACTIVE_DOTS - DE_ACTIVE_DOTS/3);
		Nada_fromDEblue();
}
#endif

//a/o sdramThing2.0v8-32 nogo... syncs at end of blue instead of Hsync
#if(defined(BLUE_VERT_BAR_REVERSED) && BLUE_VERT_BAR_REVERSED)
static __inline__ void drawPix(uint16_t rowNum)
{
		DEblue_fromNada();
		//delay_cyc(DOTS_TO_CYC(DE_ACTIVE_DOTS)/3);
		//delay_Dots(DE_ACTIVE_DOTS/3);
		DE_DotDelay(DE_ACTIVE_DOTS/3);
		DEonly_fromDEblue();
		//delay_cyc(DOTS_TO_CYC(DE_ACTIVE_DOTS)*2/3);
		//delay_Dots(DE_ACTIVE_DOTS - DE_ACTIVE_DOTS/3);
		DE_DotDelay(DE_ACTIVE_DOTS - DE_ACTIVE_DOTS/3);
		Nada_fromDEonly();
}
#endif


#if(defined(DE_BLUE) && DE_BLUE)
static __inline__ void drawPix(uint16_t rowNum)
{
		DEblue_fromNada();
		//delay_cyc(DOTS_TO_CYC(DE_ACTIVE_DOTS));
		//delay_Dots(DE_ACTIVE_DOTS);
		DE_DotDelay(DE_ACTIVE_DOTS);
		Nada_fromDEblue();
}
#endif

#if(defined(BLUE_BORDER) && (BLUE_BORDER!=0))
static __inline__ void drawPix(uint16_t rowNum)
{
		DEblue_fromNada();
		DE_DotDelay(BLUE_BORDER);
		DEonly_fromDEblue();
		DE_DotDelay(DE_ACTIVE_DOTS - BLUE_BORDER*2);
		DEblue_fromDEonly();
		DE_DotDelay(BLUE_BORDER);
		Nada_fromDEblue();
}
#endif

#if 0
//NOT BLUE_DIAG_BAR, BLUE_VERT_BAR, NOR DE_BLUE
 #if ( defined(COLOR_BAR_SCROLL) && COLOR_BAR_SCROLL)
		if(rowNum <= 256)
			rowNum=0;
		else
			rowNum=1;
 #else //NOT COLOR_BAR_SCROLL... drawPix...
		//#warning "see 'shouldBe' here..."
		//Should be:
		rowNum = rowNum*FB_HEIGHT/V_COUNT;
		//rowNum = rowNum / (768/FB_HEIGHT);
 #endif

		//DEonly_fromNada();
      drawPix(rowNum);
		//Nada_init();
 #warning "THIS ISN'T SAFE"
		Nada_fromDEonly();
	//Drawing Methods...

#if 0 //(defined(CLOCK_INSENSITIVITY_TESTING) && CLOCK_INSENSITIVITY_TESTING)
		//!!! Not sure what the state is at this point...
		// could be any DE+Blue level, or could be NADA...
		// Nada: DT1=3, still leaves one bit for clocking, might be OK

      //Disable complementary-output for Green 
		//  (on /OC1B, where CLK is OC1B)
		// Since Nada, V, and H DT's might be bad for clocking.
      TCCR1A = ( (0<<COM1A1) | (1<<COM1A0)
               | (1<<COM1B1) | (0<<COM1B0)
               | (1<<PWM1A) | (1<<PWM1B) );
#endif

		//colorOverride removed in LVDS, revisit LCDdirect50
#endif



// AND HERE!!!

// rowNum is only used when DE is active...
//  and should be 0 at *drawn row* 0
void loadData(uint16_t rowNum, uint8_t dataEnable)
{

#ifdef drawPixSetup
	drawPixSetup(rowNum);
#endif

	//H Active pulse...
   if(vSync)
   {
		//Vsync_fromNada() is called at the end of the last interupt
      VplusH_fromVsync();
      Hlow_Delay();
      Vsync_fromVplusH();
      HD_Delay();
   }
   else
   {
      Hsync_fromNada();
      Hlow_Delay();
      Nada_fromHsync();
      HD_Delay();
   }

   if(dataEnable)
   {
		//No DE_fromNada() in here, either...
		drawPix(rowNum);
		//#error "No Nada here?!"
		//Apparently it's in the drawPix functions themselves!
		// Don't forget it if implementing your own!
	}
#if ( (defined(LCDSTUFF_NOT_IN_TIMER) && LCDSTUFF_NOT_IN_TIMER) \
		|| (defined(LCDSTUFF_INCLUDE_NON_DE) && LCDSTUFF_INCLUDE_NON_DE) )
	else //Not DE (e.g. Vsync, etc)
	{
		//Need to load the timing info anyhow...
		// undrawn rows are supposed to be the same number of pixel clocks
		// (Though, that's probably not necessary, judging by the flexibility
		//  this display has demonstrated with LVDSdirect...)
		//delay_Dots(DE_ACTIVE_DOTS);
		//a/o v0.80: trying to figure out why DE_DotDelay() is necessary
		//rather than just using delay_Dots()
		// sdramThing3.0 uses write_Dots(), but writeDots has an argument
		// which takes into account the current mode, in order to position
		// the dots in memory properly...
		// So, delay_Dots() (if it even exists in sdramThing?) wouldn't work,
		// because it wouldn't take into account page-boundaries, etc.
		
		// DE_DotDelay() Does NOT change the current *mode* (e.g. hsync/vsync
		// /DE/Nada...)
		DE_DotDelay(DE_ACTIVE_DOTS);

 //This test in order to keep compatibility with the old method, which
 //isn't right (see notes) But worked. (DE-Only display...)
 // This whole section should be eliminated, until the else-case
 #if (!defined(LCDSTUFF_INCLUDE_NON_DE) || !LCDSTUFF_INCLUDE_NON_DE)
		//a/o v0.80: This probably isn't even necessary... Besides being
		//WRONG.

		//Well, the output data might not necessarily have been /blue/
		// but at least with the sdramThing2.0's Nada_fromDEblue() method
		// this should be fine (if not a tad overkill)
		Nada_fromDEblue();
		//Nada_init();
		//Actually, shouldn't it already be Nada, from before?
		// Wait a minute, none of this is right... right?
		// Shouldn't Vsync be active still?
 #else //LCDSTUF_INCLUDE_NON_DE (e.g. LCDdirectLVDSv81 with parallel LCD
		//The mode should be correct after exitting the vsync tests and
		//handling hsync, so there should be no need to do this:
		//if(vsync)
		//	Vsync_from
 #endif
	}

	//For writing to SDRAM, etc... (not usging timer)
	// DISPLAY_DOT_COUNT is the total number of dots per row
	// (in LVDSdirect, calculations would also occur after this...
	//  but that's not relevent here, now...)
	// DISPLAY_DOT_COUNT = (H_LOW_DOTS + HD_DOTS + DE_ACTIVE_DOTS + DH_DOTS)
	// H_LOW_DOTS was handled earlier via Hlow_delay()
	// HD_DOTS via HD_Delay()
	// DE_ACTIVE_DOTS in if (and else) above...
	// DH_DOTS was the down-time between timer-interrupts
	// But since we're not using timer interrupts, they need to be handled
	// explicitly...
	// (Note that with Timer Interrupts, and more importantly PWM-driving, 
	//  timing wasn't nearly so precise:
	//   calculation-time, etc, affected the number of cycles until the next
	//   delay, etc... e.g. Interrupt occurs, if/else for Vsync/Hsync
	//   So the actual Hsync/Vsync signals wouldn't be sent until the if
	//   statements were processed...
	//  If anything, this should show that the display is a lot less 
	//   sensitive to exact/stable timing than this method will provide
	//   However, it might mean that some delays might need to be increased)
	// Actually: After loadData was run, the timer interrupt would load
	//  the next mode, as appropriate, which would take effect immediately
	//  e.g. loadData -> Vsync_fromNada();
	//  So then, DH_DOTS would actually occur *after* Vsync_fromNada()
	//   meaning Vsync would be active during that time...
	//   But, these sorts of things (in the switch statement) only occur
	//   on non-drawing-lines right? More cause to believe the timing isn't
	//   excruciatingly sensitive...
	//   (Those cases occurred *without* DE-timing-mimicking implemented
	//    in this #if... so ... there.)
	
	//delay_Dots(DH_DOTS);
  #ifdef DH_Delay
	DH_Delay();
  #else
	delay_Dots(DH_DOTS);
  #endif

	//So, e.g. in sdramThing2.0, this function, via delays and DE,
	// should create *exactly* DISPLAY_DOT_COUNT "dots" per row... (no?)
#else
	//This is just for testing sdramThing2.0...
	//#error "WTF..."
#endif
}


//Who knows, this could be useful...
#if(defined(LCD_SMILEY) && LCD_SMILEY)
#define RED_IMAGEVAL_BIT 0
#define GREEN_IMAGEVAL_BIT 1
#define BLUE_IMAGEVAL_BIT 2

/*
#define _R	0x03
#define _O	0x07
#define _G 0x0C
#define _B 0x30
#define _V 0x13
#define _Y	0x0F
#define _C 0x3C
#define _M	0x33
#define _K	0x00
#define _W 0x3F
#define _r 0x01
#define _g 0x04
#define _b 0x10
#define _c 0x14
#define _m 0x11
#define _y 0x05
#define _k 0x15
#define Tr 0xC0	//Transparet
*/
uint8_t pgm_image1[256] PROGMEM =
{
	Tr,Tr,Tr,Tr,Tr,_K,_K,_K,_K,_K,_K,Tr,Tr,Tr,Tr,Tr,
	Tr,Tr,Tr,_K,_K,_Y,_Y,_Y,_Y,_Y,_Y,_K,_K,Tr,Tr,Tr,
	Tr,Tr,_K,_K,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_K,_K,Tr,Tr,
	Tr,_K,_K,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_K,_K,Tr,
	Tr,_K,_Y,_Y,_Y,_K,_Y,_Y,_Y,_Y,_K,_Y,_Y,_Y,_K,Tr,
	_K,_Y,_Y,_Y,_K,_K,_K,_Y,_Y,_K,_K,_K,_Y,_Y,_Y,_K,
	_K,_Y,_Y,_Y,_Y,_K,_Y,_Y,_Y,_Y,_K,_Y,_Y,_Y,_Y,_K,
	_K,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_K,
	_K,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_K,
	_K,_Y,_Y,_K,_K,_Y,_Y,_Y,_Y,_Y,_Y,_K,_K,_Y,_Y,_K,
	_K,_Y,_Y,_Y,_K,_K,_Y,_Y,_Y,_Y,_K,_K,_Y,_Y,_Y,_K,
	Tr,_K,_Y,_Y,_Y,_K,_K,_K,_K,_K,_K,_Y,_Y,_Y,_K,Tr,
	Tr,_K,_K,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_K,_K,Tr,
	Tr,Tr,_K,_K,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_K,_K,Tr,Tr,
	Tr,Tr,Tr,_K,_K,_Y,_Y,_Y,_Y,_Y,_Y,_K,_K,Tr,Tr,Tr,
	Tr,Tr,Tr,Tr,Tr,_K,_K,_K,_K,_K,_K,Tr,Tr,Tr,Tr,Tr
};

uint8_t pgm_image2[256] PROGMEM =
{
	Tr,Tr,Tr,Tr,Tr,_K,_K,_K,_K,_K,_K,Tr,Tr,Tr,Tr,Tr,
	Tr,Tr,Tr,_K,_K,_Y,_Y,_Y,_Y,_Y,_Y,_K,_K,Tr,Tr,Tr,
	Tr,Tr,_K,_K,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_K,_K,Tr,Tr,
	Tr,_K,_K,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_K,_K,Tr,
	Tr,_K,_Y,_Y,_Y,_K,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_K,Tr,
	_K,_Y,_Y,_Y,_K,_K,_K,_Y,_Y,_K,_Y,_Y,_Y,_Y,_Y,_K,
	_K,_Y,_Y,_Y,_Y,_K,_Y,_Y,_Y,_Y,_K,_K,_Y,_Y,_Y,_K,
	_K,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_K,
	_K,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_K,
	_K,_Y,_Y,_K,_K,_Y,_Y,_Y,_Y,_Y,_Y,_K,_K,_Y,_Y,_K,
	_K,_Y,_Y,_Y,_K,_K,_Y,_Y,_Y,_Y,_K,_K,_Y,_Y,_Y,_K,
	Tr,_K,_Y,_Y,_Y,_K,_K,_K,_K,_K,_K,_Y,_Y,_Y,_K,Tr,
	Tr,_K,_K,_Y,_Y,_Y,_K,_R,_K,_R,_K,_Y,_Y,_K,_K,Tr,
	Tr,Tr,_K,_K,_Y,_Y,_K,_R,_R,_R,_K,_Y,_K,_K,Tr,Tr,
	Tr,Tr,Tr,_K,_K,_Y,_Y,_K,_K,_K,_Y,_K,_K,Tr,Tr,Tr,
	Tr,Tr,Tr,Tr,Tr,_K,_K,_K,_K,_K,_K,Tr,Tr,Tr,Tr,Tr
};
#endif //SMILEY

/*
#if (IMAGE_WIDTH == 21)
uint8_t mainData[2][768>>5][IMAGE_WIDTH] =
{
	{
	{6,5,4,3,2,1,6,5,4,3,2,1,6,5,4,3,2,1,6,5,4},
	{5,4,3,2,1,6,0,0,0,0,0,0,0,0,0,2,1,6,5,4,3},
	{4,3,2,1,0,0,0,3,3,3,3,3,3,3,0,0,0,5,4,3,4},
	{3,2,1,0,0,3,3,3,3,3,3,3,3,3,3,3,0,0,3,2,1},
	{2,1,0,0,3,3,3,3,3,3,3,3,3,3,3,3,3,0,0,1,6},
	{1,0,0,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,0,0,5},
	{6,0,3,3,3,3,3,0,3,3,3,3,0,3,3,3,3,3,3,0,4},
	{0,0,3,3,3,3,0,0,0,3,3,0,0,0,3,3,3,3,3,0,0},
	{0,3,3,3,3,3,0,0,0,3,3,0,0,0,3,3,3,3,3,3,0},
	{0,3,3,3,3,3,3,0,3,3,3,3,0,3,3,3,3,3,3,3,0},
	{0,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,0},
	{0,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,0},
	{0,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,0},
	{0,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,0},
	{0,3,3,3,3,0,0,3,3,3,3,3,3,3,0,0,3,3,3,3,0},
	{0,0,3,3,3,3,0,0,3,3,3,3,3,0,0,3,3,3,3,0,0},
	{6,0,3,3,3,3,3,0,0,0,0,0,0,0,3,3,3,3,3,0,4},
	{5,0,0,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,0,0,3},
	{4,3,0,0,3,3,3,3,3,3,3,3,3,3,3,3,3,0,0,3,2},
	{3,2,6,0,0,3,3,3,3,3,3,3,3,3,3,3,0,0,3,2,1},
	{2,1,5,5,0,0,0,3,3,3,3,3,3,3,0,0,0,3,2,1,6},
	{1,6,3,4,3,2,0,0,0,0,0,0,0,0,0,4,3,2,1,6,5},
	{6,5,4,3,2,1,6,5,4,3,2,1,6,5,4,3,2,1,6,5,4},
	{5,4,3,2,1,6,5,4,3,2,1,6,5,4,3,2,1,6,5,4,3}
	},
	{	
	{5,4,3,2,1,6,5,4,3,2,1,6,5,4,3,2,1,6,5,4,3},
	{4,3,2,1,6,5,0,0,0,0,0,0,0,0,0,1,6,5,4,3,2},
	{3,2,1,6,0,0,0,3,3,3,3,3,3,3,0,0,0,4,3,2,1},
	{2,1,6,0,0,3,3,3,3,3,3,3,3,3,3,3,0,0,2,1,6},
	{1,6,0,0,3,3,3,3,3,3,3,3,3,3,3,3,3,0,0,6,5},
	{6,0,0,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,0,0,4},
	{5,0,3,3,3,3,3,0,3,3,3,3,3,3,3,3,3,3,3,0,3},
	{0,0,3,3,3,3,0,0,0,3,3,0,3,3,3,3,3,3,3,0,0},
	{0,3,3,3,3,3,0,0,0,3,3,3,0,0,0,3,3,3,3,3,0},
	{0,3,3,3,3,3,3,0,3,3,3,3,3,3,3,3,3,3,3,3,0},
	{0,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,0},
	{0,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,0},
	{0,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,0},
	{0,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,0},
	{0,3,3,3,3,0,0,3,3,3,3,3,3,3,0,0,3,3,3,3,0},
	{0,0,3,3,3,3,0,0,3,3,3,3,3,0,0,3,3,3,3,0,0},
	{5,0,3,3,3,3,3,0,0,0,0,0,0,0,3,3,3,3,3,0,3},
	{4,0,0,3,3,3,3,3,3,0,1,0,1,0,3,3,3,3,0,0,2},
	{3,2,0,0,3,3,3,3,3,0,1,1,1,0,3,3,3,0,0,2,1},
	{2,1,6,0,0,3,3,3,3,3,0,0,0,3,3,3,0,0,2,1,6},
	{1,6,5,4,0,0,0,3,3,3,3,3,3,3,0,0,0,2,1,6,5},
	{6,5,4,3,2,1,0,0,0,0,0,0,0,0,0,3,2,1,6,5,4},
	{5,4,3,2,1,6,5,4,3,2,1,6,5,4,3,2,1,6,5,4,3},
   {4,3,2,1,6,5,4,3,2,1,6,5,4,3,2,1,6,5,4,3,2}
	}
};
*/

//This is just here for an example, and has been greatly reduced...
// (probably not functional)
#if 0
int main(void)
{
	init_timer0Hsync();

	//This starts pretty late... watch out for WDT
	init_heartBeat();

	lvds_timerInit();

	// MUCH Of this is outside the screen...

	//NOT COLOR_BAR_SCROLL
	uint8_t row,col;
#if (defined(IMAGE_CHANGE) && IMAGE_CHANGE)
	uint32_t counter=0;
	uint8_t imageNum = 0;
	uint8_t colorShift = 0;
	while(1)
	{
 #if (defined(IMAGE_BUFFER) && IMAGE_BUFFER)
			for(row=0; row<FB_HEIGHT; row++)
	      for(col=0; col<FB_WIDTH; col++)
	      {
	         uint8_t imagePixel=pgm_readImageByte(pimage, row, col);


				if(imagePixel == Tr)
					frameBuffer[row][col] = colorShift+row+col;
				else
					frameBuffer[row][col] = imagePixel;	
			/*
	         setColor(
		         (getbit(RED_IMAGEVAL_BIT, imagePixel) ? 0x03 : 0),
	            (getbit(GREEN_IMAGEVAL_BIT, imagePixel) ? 0x0C : 0),
	            (getbit(BLUE_IMAGEVAL_BIT, imagePixel) ? 0x30 : 0),
	            r, c);
			*/
			}
			colorShift++;
	}
 #endif
		heartUpdate();
	}
#endif

}
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
 * /Users/meh/_avrProjects/LCDdirectLVDS/90-reGitting/_commonCode_localized/lcdStuff/0.80ncf/lcdStuff.c
 *
 *    (Wow, that's a lot longer than I'd hoped).
 *
 *    Enjoy!
 */

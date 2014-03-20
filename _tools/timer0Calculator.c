/* mehPL:
 *    This is Open Source, but NOT GPL. I call it mehPL.
 *    I'm not too fond of long licenses at the top of the file.
 *    Please see the bottom.
 *    Enjoy!
 */


#warning "PLL_SYSCLK is defined in the makefile... shouldn't be handled here explicitly"
#define PLL_SYSCLK	1

//NOT YET USED...
// (Well, only to calculate what the macros calculate on their own)

//Calculating the compare-match value for Timer0 takes a bit of math...
// it was originally implemented in macros, but the math has gotten too
// complicated to write without being able to see intermediate values
// for testing, at the very least.
// (Yeah, it *should* work, but it doesn't).
//So I'm opting for doing it in C, with printfs...

#include <stdio.h>
#include <inttypes.h>

#include "mainConfig.h"	//Necessary for LVDS_PRESCALER, 
								// and ALIGN_TIMER_WITH_PIXCLK, at least...
#include "LCDdefines.h"	// Contains T_HD_CYC, etc.



//Each row contains:
//  Horizontal Front Porch 	(T_DH)
//  Horizontal Sync 				(T_Hlow)
//  Horizontal Back Porch 		(T_HD)
//  DataEnable (pixels drawn)	(T_DE)

// So the timer interrupt should occur upon completing all these tasks
//  For our purposes, in the interrupt...
//  The Horizontal Front Porch (T_DH) occurs *after* T_DE completes
//    This is the time when execution exits the interrupt

// Unfortunately, (per an OLD note) Timer0 can't be used in 16-bit mode
// 	TIMER0 DOES NOT HAVE CTC MODE in 16-bit mode
// 	(So use the prescaler, with 8-bit mode.
//     Though, this limits the precision/resolution...)

uint32_t display_row_dots(void)
{
	//#define DISPLAY_CYC_COUNT \
	//  (T_Hlow_CYC + T_HD_CYC + T_DE_CYC + T_DH_CYC)

	return DH_DOTS + H_LOW_DOTS + DE_ACTIVE_DOTS + HD_DOTS;
}





// ROW_CALCULATION_CYCS:
// This is a hack, for testing syncability with long row-calculations
// before actually drawing it... e.g. loading from program memory
//
// a/o v59-20ish, this has been used for quite some time. It's no longer
// a test, though it's also not particularly scientific.
// E.G. RowSegBuffer uses this time to calculate and load the image-data
//  into the buffer, before displaying it at the next interrupt.
//  Furthermore, it seems this display isn't particularly sensitive to
//  rows of different durations... so this may be removable.
//
// If your display requires stable row-durations, then:
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

// I dunno where I came up with this logic... I think it's completely
// arbitrary.
#if (LVDS_PRESCALER == 8)
 //#define ROW_CALCULATION_DELAY 1//20
 #define ROW_CALCULATION_CYCS (50000UL) //0 //(70000) //(100000)
#else
 //#define ROW_CALCULATION_DELAY 9//7//5//2//1//10
 #define ROW_CALCULATION_CYCS   (8*DISPLAY_CYC_COUNT)
#endif



// Also, there's quite a bit of leeway here, as this is based on 
// ___CYC values, which are rounded from dot-values...
uint32_t total_cyc_count(void)
{

	//#define TOTAL_CYC_COUNT \
	//      ((0UL + ROW_CALCULATION_CYCS + DISPLAY_CYC_COUNT))

	return DOTS_TO_CYC(display_row_dots()) + ROW_CALCULATION_CYCS;
}


//Takes in a minimum value and rounds it up to the nearest multiple of
// roundTo... e.g. ROUND_TO(1,5)=5, ROUND_TO(13,5)=15
//NOTE: It only works if minVal and roundTo are integers!
#define ROUND_TO(minVal, roundTo) \
	((((minVal)+((roundTo)-1))/(roundTo))*(roundTo))



//Attempt to determine the minimum prescaler necessary
int determinePrescaler(void)
{
	//These are the prescalers available to Timer0
	// (Skipping prescaler CLKDIV1, since there's no way 1024 pixels
	//  could fit in 256 clock-cycles. Nevermind it makes math easier...)
	int prescaler[4] = {8,64,256,1024};

	printf("display_row_dots = %"PRIu32"\n", display_row_dots());
	printf("display_cyc = %"PRIu32"\n", DOTS_TO_CYC(display_row_dots()));
	printf("ROW_CALCULATION_CYCS = %"PRIu32"\n", ROW_CALCULATION_CYCS);
	printf("total_cyc_count = %"PRIu32"\n", total_cyc_count());

	int p;

	uint32_t ocrVal = 0xffff;

	for(p=0; ((p<4) && (ocrVal>255)); p++)
	{
		//Adding prescaler[p]-1 should cause rounding-up...
		// though with ROW_CALCULATION_CYCS being somewhat random, it
		// shouldn't be so important (to round-up)
		uint32_t ocrVal = (total_cyc_count() + prescaler[p]-1)/prescaler[p];

		printf("prescaler: %d\n", prescaler[p]);
		printf("  ocrVal = %"PRIu32"\n", ocrVal);

		//Attempt to align the timer-interrupt such that it always occurs
		// at the same point in a pixel-transmission...
		// Since all prescalers are divisible by 4, and 8 
		//   (the number of system clock cycles relative to the number of
		//    pixel clock cycles... erm...)
		//    cyc = dots * 7 * LVDS_PRESCALER / this
		//    where "this" is either 4 or 8 depending on whether the PLL
		//    clock is 4 or 8 times faster than the system-clock
		//    (configured elsewhere)
		//  ...we needn't worry about its divisibility there, only by 7
#warning "Do we need to consider the LVDS_PRESCALER???"
		//  Maybe that's why it didn't work as expected???
  		//#if(defined(ALIGN_TIMER_WITH_PIXCLK) && ALIGN_TIMER_WITH_PIXCLK)
			ocrVal = ROUND_TO(total_cyc_count(), prescaler[p]*7)/prescaler[p];
			printf("  ocrVal Aligned = %"PRIu32"\n", ocrVal);
		//#endif
	}

	//OCR Values of 1 -> counts two TCNTS, right?
	if((ocrVal < 255) && (ocrVal > 1))
		return 0;
	else
		return 1;

}

#if 0
#if (TOTAL_CYC_COUNT > 255)
 //Adding +7 causes it to round up (hopefully)
 #define TIMER0_TCNTS ((TOTAL_CYC_COUNT+7)/8)
 #define TIMER0_CLKDIV   CLKDIV8
 #warning "TIMER0_TCNTS = (TOTAL_CYC_COUNT+7)/8: Timer0 CLKDIV8"


 #if (TIMER0_TCNTS > 255)
  #warning "TIMER0_TCNTS = (TOTAL_CYC_COUNT+63)/64: Timer0 CLKDIV64"
  #define TIMER0_TCNTS ((TOTAL_CYC_COUNT+63)/64)
  #define TIMER0_CLKDIV CLKDIV64

  #if(defined(ALIGN_TIMER_WITH_PIXCLK) && ALIGN_TIMER_WITH_PIXCLK)
   //Attempting interrupt/pixel alignment...
   // this is where we're at, right now.
   // a/o v59-13ish
   #warning "THIS IS JUST A TEST FOR PIXEL-ALIGNING"
   //#define CLOCKCYCS (TIMER0_TCNTS * 64)
   //We know that CLOCKCYCS is divisible by four... (the system clock)
   // so we just need to round to the nearest 7...
   #define ROUND_TO(minVal, roundTo) \
      ((((minVal)+((roundTo)-1))/(roundTo))*(roundTo))

   #define TIMER0_TCNTS_A (ROUND_TO(TOTAL_CYC_COUNT, 64*7)/64)


   #if (TIMER0_TCNTS_A <= 255)
      #define TIMER0_TCNTS ((TIMER0_TCNTS_A))
   #else
      #warning "Rounded past 255"
   #endif
  #endif



  #if ((TIMER0_TCNTS > 255) || \
		 (defined(ALIGN_TIMER_WITH_PIXCLK) && (TIMER0_TCNTS_A > 255)))
    #warning "TIMER0_TCNTS = (TOTAL_CYC_COUNT+255)/256: Timer0 CLKDIV256"
    #define TIMER0_TCNTS ((TOTAL_CYC_COUNT+255)/256)
    #define TIMER0_CLKDIV CLKDIV256
   
    #if(defined(ALIGN_TIMER_WITH_PIXCLK) && ALIGN_TIMER_WITH_PIXCLK)
     //#define CLOCKCYCS (TIMER0_TCNTS * 256)
     #define TIMER0_TCNTS_B (ROUND_TO(TOTAL_CYC_COUNT, 256*7)/256)


     #if (TIMER0_TCNTS_B <= 255)
      #define TIMER0_TCNTS (TIMER0_TCNTS_B)
     #else
      #warning "Rounded past 255"
     #endif
    #endif


    #if ((TIMER0_TCNTS > 255) || \
			(defined(ALIGN_TIMER_WITH_PIXCLK) && (TIMER0_TCNTS_B > 255)))
      #warning "TIMER0_TCNTS = (TOTAL_CYC_COUNT+1023)/1024: Timer0 CLKDIV1024"
      #define TIMER0_TCNTS ((TOTAL_CYC_COUNT+1023)/1024)
      #define TIMER0_CLKDIV   CLKDIV1024

      #if (defined(ALIGN_TIMER_WITH_PIXCLK) && ALIGN_TIMER_WITH_PIXCLK)
       #define TIMER0_TCNTS_C (ROUND_TO(TOTAL_CYC_COUNT, 1024*7)/1024)


       #if (TIMER0_TCNTS_C <= 255)
        #define TIMER0_TCNTS (TIMER0_TCNTS_C)
       #else
        #error "Rounded past 255"
       #endif
      #endif
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

#endif //FALSE



//This is old... But maybe still usable.
// Or, maybe this'll be handled above...?
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


int main(void)
{
	return determinePrescaler();
}

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
 *    2) Please do not change/remove others' credit/licensing/copywrite 
 *         info, where noted. 
 *    3) If you find yourself profitting from my work, please send me a
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
 *    (Wow, that's a lot longer than I'd hoped).
 *
 *    Enjoy!
 */

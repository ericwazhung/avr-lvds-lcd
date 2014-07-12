/* mehPL:
 *    This is Open Source, but NOT GPL. I call it mehPL.
 *    I'm not too fond of long licenses at the top of the file.
 *    Please see the bottom.
 *    Enjoy!
 */




#ifndef __6BITPARALLEL_C__
#define __6BITPARALLEL_C__

#include "6bitParallel.h"


//onOff is TRUE for ON, FALSE for OFF (should be 0 or 1, nothing else)
void lcdInterface_pwmDotClockInit(uint8_t onOff)
{
	//Timer1 is used for the MCK (pixel/dot-clock)
	//FOR NOW: Assuming we're using a 16MHz system-clock
	// The dot-clock is supposed to be <= 4.2MHz, so let's go for 4MHz
	//
	// 16MHz/4MHz = 4
	// So set the timer to count from 0 to 3 and reset
	OCR1C = 3;

	// And we want the toggling to be centered:
	// TODO: Which phase is this, and how do we align it with data-changes?
	// As I recall, the change occurs after TCNT leaves the OCR value
	// (except for OCRVal == 0)
	// so an OCR-value of 1 causes:
	// TCNT    0   1   2   3   0 ....
	//      ___         _______
	//         |_______|       |___
	MCK_OCR = 1;

	//Set the Timer1 clock prescaler... (No prescaler == /1)
	writeMasked((1<<CS10),
			            ((1<<CS13) | (1<<CS12) | (1<<CS11) | (1<<CS10)),
							            TCCR1B);
	//Set the DeadTime prescaler (no prescaling, same speed as TCNT1)...
	// Allegedly this is prescaled from the PCK (or CK)
	//    NOT from the Timer1 prescaler...
	writeMasked(0,
			            ((1<<DTPS11) | (1<<DTPS10)),
							            TCCR1B);


	writeMasked(((0<<WGM11) | (0<<WGM10)), //FastPWM (combined with above)
	            ((1<<WGM11) | (1<<WGM10)), // (affects all PWM channels)
	            TCCR1D);

	//Forgot this... it was enableGreenMakeClockInsensitive-ish
	// from lvds.c
	writeMasked((onOff<<COM1B1) | (0<<COM1B0),
					(1<<COM1B1) | (1<<COM1B0),
					TCCR1A);
	
		
	writebit(PWM1B, TCCR1A, onOff);  //Enable PWM on OC1B for CLOCK

}


void lcdInterface_init(void)
{
	//Why wasn't this in lvds161?
  #if(defined(OSCCAL_VAL))
	OSCCAL = OSCCAL_VAL;
  #elif(defined(LVDS_MAXIMIZE_OSCCAL) && LVDS_MAXIMIZE_OSCCAL)
	#warning "LVDS_MAXIMIZE_OSCCAL is defined, without LVDS, should be OK"
   OSCCAL = OSCCAL_VAL;
  #endif

  #if(defined(LVDS_USE_PLL) && LVDS_USE_PLL)
	#error "LVDS_USE_PLL is defined, without LVDS... and parallel doesn't yet implement PLL"
	//There's more to it than just this... also need to consider the
	//prescaler, etc...
   pll_enable();
  #endif


  //Use PWM for MCK...
  #if (!defined(LCDINTERFACE_BITBANGED_DOTCLOCK) || \
		  !LCDINTERFACE_BITBANGED_DOTCLOCK)
	lcdInterface_pwmDotClockInit(TRUE);
  #endif //NOT: LCDINTERFACE_BITBANGED_DOTCLOCK (e.g. use PWM for MCK)
 //Otherwise we're bit-banging MCK

	setoutPORT(MCK_PIN, MCK_PORT); //+OC1B Clock (OC1B, not inverted)

	//Set the other pins as outputs, as well...
	setoutPORT(HSYNC_PIN, HSYNC_PORT);
	setoutPORT(VSYNC_PIN, VSYNC_PORT);
	setoutPORT(SD_PIN, SD_PORT);

	setPORToutMasked(RGB_PORT, (RED_MASK | GREEN_MASK | BLUE_MASK));

#warning "The ShutDown pin is completely unimplemented, but for now it seems to work..."
	setpinPORT(SD_PIN, SD_PORT);



	Nada_init();
	noGreen();
	noRed();
}




#if(defined(FRAMEBUFFER_TESTING) && FRAMEBUFFER_TESTING)


 #if (defined(LCDINTERFACE_BITBANGED_DOTCLOCK) && \
		  LCDINTERFACE_BITBANGED_DOTCLOCK)
 #warning "parallelLCD_writeColor() needn't be inline, but it probably is."
 #endif

void parallelLCD_writeColor(uint8_t includeDEinit, uint8_t includeDelay,
																		uint8_t colorVal)
{
#if(defined(ROW_BUFFER) && ROW_BUFFER)
 #error "parallelLCD_writeColor() has not yet implemented ROW_BUFFER"
#endif

	//colorVal is stored as a 6bit value, just as its written to the port
	// Brilliant, I tell you!

	//The parallel LCD interface doesn't (yet?) have DE
	// (The Sony display doesn't have DE at all, so it doesn't need to be
	// implemented at all)
	//if(includeDEinit)
	//{
	//
	//}

	//The remaining two bits on the port are unused, so we don't have to
	//worry about masking:
//WTF... this looks right, order of inclusions in main.c... WTF.
//#if (defined(BUMP_SENSOR_PORT_ON_RGB_PORT) && BUMP_SENSOR_PORT_ON_RGB_PORT)
//#error "OK"
#if(defined(OTHER_BITS_HIGH) && OTHER_BITS_HIGH)
	RGB_PORT = colorVal | ((1<<7) | (1<<6));
#else
	RGB_PORT = colorVal;
#endif

#if (defined(LCDINTERFACE_BITBANGED_DOTCLOCK) && \
		LCDINTERFACE_BITBANGED_DOTCLOCK)
	//THIS ASSUMES: F_CPU <= 16Mhz -> F_MCK <= 4MHz
	// This probably isn't particularly accurate...
	// e.g. it's 5 cycles, actually, since MCK and Data can't be written at
	// the same instruction (unless MCK was on the same port)
	asm("nop;");
	clrpinPORT(MCK_PIN, MCK_PORT);
	asm("nop;");
	setpinPORT(MCK_PIN, MCK_PORT);
#endif

	//For stretching across the display...
	if(includeDelay)
	{
#if (defined(LCDINTERFACE_BITBANGED_DOTCLOCK) && \
		LCDINTERFACE_BITBANGED_DOTCLOCK)
 //#error "Wait, we need delay_dots!"
		//One dot has been written, with clrpinPORT, above...
		delay_Dots(WRITE_COLOR_DOT_DELAY);
#else
		delay_cyc(WRITE_COLOR_DELAY);	
#endif
	}
}

#endif //FRAMEBUFFER_TESTING 


#endif //__6BITPARALLEL_C__


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
 * /Users/meh/_avrProjects/LCDdirectLVDS/93-checkingProcessAgain/_interfaces/6bitParallel.c
 *
 *    (Wow, that's a lot longer than I'd hoped).
 *
 *    Enjoy!
 */

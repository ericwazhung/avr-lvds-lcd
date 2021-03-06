/* mehPL:
 *    This is Open Source, but NOT GPL. I call it mehPL.
 *    I'm not too fond of long licenses at the top of the file.
 *    Please see the bottom.
 *    Enjoy!
 */



#ifndef __PLL_C__
#define __PLL_C__

#include _BITHANDLING_HEADER_
#include <util/delay.h> //For delay_us in pll_enable




#if(defined(__AVR_AT90PWM161__))
//PWM161 notes:
// The PWM161 has a PLL Multiplication-Factor which can be adjusted
//  4-8x the source-clock, with every step between
//  This could be AWESOME for LVDS, with a multiplcation-factor of 7...?
// "When Vcc is below 4.5V, maximum PLL-factor is 6"
//  we'll see. Best stick with 8 for now, since all the math relies on it.
//  But look here if it doesn't seem to be syncing...
#warning "PLL_MULT_FACTOR defaults to 8, which is allegedly too fast for 3.3V"
#define PLL_MULT_FACTOR	8
// "PLLF3 is used for debug purpose", its value is always 0
// The PLLF3:0 bits are used to set the multiplication-factor
//  They're shifted two bits left
//  And 2 is added to the value written to these bits to determine the
//  multiplication-factor...
#define PLL_MULT_MASK	(0x07<<2)

#define pll_setMult(factor) \
			writeMasked(((factor-2)<<2), PLL_MULT_MASK, PLLCSR)


// The pll_enable procedure isn't quite so clearly-defined...
//   there's no step-by-step procedure I've found so far.
//p271 Table 22-4 says the maximum PLL lock-in-time is 64us, so 100 should
//be fine.

#endif


//pll_enable does just what it says, using instructions from the ATtiny861
//   manual.
//   The PLL is used, in all cases, for the FPD-Link signals (generated by
//   Timer1's PWM). But, it may be prescaled back down to CPU-speeds or
//   even lower. Also, when PLL_SYSCLK is true, the PLL is actually used to
//   drive the CPU... I don't fully comprehend how this works, because the
//   pll_enable can't be called *before* the system boots... Anyways, It
//   seems to work, regardless, and calling pll_enable() in this mode
//   doesn't seem to cause any problems.


void pll_enable(void)
{
#if(defined(__AVR_AT90PWM161__))
	pll_setMult(PLL_MULT_FACTOR);
#endif
   //Stolen from threePinIDer109t:

   //Set Timer1 to use the "asynchronous clock source" (PLL at 64MHz)
   // With phase-correct PWM (256 steps up, then back down) and CLKDIV1
   // this is 64MHz/512=125kHz
   // The benefit of such high PWM frequency is the low RC values necessary
   //  for filtering to DC.
	// From the ATtiny861 manual:
	// "To change Timer/Counter1 to the async mode follow this procedure"
   // 1: Enable the PLL
   setbit(PLLE, PLLCSR);
   // 2: Wait 100us for the PLL to stabilize
   // (can't use dmsWait since the timer updating the dmsCount hasn't yet been started!)
   _delay_us(100);
//   dmsWait(1);
   // 3: Poll PLOCK until it is set...
   while(!getbit(PLOCK, PLLCSR))
   {
      asm("nop");
   }

//The PWM161 doesn't have a PCKE bit.
#if(!defined(__AVR_AT90PWM161__))
   // 4: Set the PCKE bit to enable async mode
   setbit(PCKE, PLLCSR);
#endif

}

#endif //__PLL_C__

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
 * /Users/meh/_avrProjects/LCDdirectLVDS/93-checkingProcessAgain/pll.c
 *
 *    (Wow, that's a lot longer than I'd hoped).
 *
 *    Enjoy!
 */

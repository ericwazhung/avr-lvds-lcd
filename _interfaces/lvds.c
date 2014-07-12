/* mehPL:
 *    This is Open Source, but NOT GPL. I call it mehPL.
 *    I'm not too fond of long licenses at the top of the file.
 *    Please see the bottom.
 *    Enjoy!
 */




#ifndef __LVDS_C__
#define __LVDS_C__

#include "lvds.h"
#include "pll.c"


void lvds_timerInit(void)
{
   //Timer1 is used for LVDS (in PLL clocking mode)
  
     //pll_enable();   
   
   //We want it to count 7 bits, 0-6 and reset at 7
   OCR1C = 6;

   //We want the clock to go low at TCNT=0 and high at TCNT=4
   CLOCK_OCR = 3; //2; //3;
   

// My 'scope is only 20MHz, and I'd rather be able to use the digital mode
// which is even slower...


//Overridden when SLOW_EVERYTHING_TEST is true...
//#define TOOFAST_TEST TRUE
#warning "HERE AND BELOW, doesn't OSCCAL have special write requirements?"
//OSCCAL = 0x00;
#if (defined(SLOW_EVERYTHING_TEST) && SLOW_EVERYTHING_TEST)
//FOR TESTING. This should slow the clock...
   // Gives roughly 4MHz...
   OSCCAL = 0x00; //0x80; 

   // This should divide the system clock by 256
   // does this affect the PLL? NO.
   // The PLL is clocked only by the RC Oscillator
   //   OSCCAL does affect it.
   // OPTIONS FOR SIMULATING FULL FUNCTIONALITY AT LOW SPEED:
   // Run PLL as normal
   //  Prescale Timer1 and DeadTimer equally
   //  Prescale System clock equally
   CLKPR = (1<<CLKPCE);
   CLKPR = (1<<CLKPS1) | (1<<CLKPS0);   //CLKDIV8
   //4MHz / 8 = .5Mhz...
   //PLL is 4MHz*8 = 32MHz
   // then TimerClockDivisor=8 gives 4MHz
   pll_enable();
   #define SLOW_LVDS_TEST TRUE
#elif (defined(OSCCAL_VAL))
   OSCCAL = OSCCAL_VAL;
#endif



#if (defined(SLOW_LVDS_TEST) && SLOW_LVDS_TEST)
   //This case doesn't really make sense without SLOW_EVERYTHING...
   //8x prescaler for Timer1
   #define CSBITS (1<<CS12)
   //8x prescaler for the dead-timer
   #define DTPSBITS ((1<<DTPS11) | (1<<DTPS10))
#elif (defined(LVDS_PRESCALER))
   //Timer1 on the Tiny861 uses a strange CLKDIV scheme...
   // (but it's nicer!)
   // The divisor is (1<<(csbits-1))
   // so a divisor of 1 = (1<<0) = (1<<(1-1)), (csbits = 0x1)
   // 256 = (1<<8) = (1<<(9-1)), (csbits = 0x9)
   // 512 = (1<<9) = (1<<(10-1)), (csbits = 0xA)
   // ...
   // (0x0 stops the timer)
   /*
      uint16_t divisor;
      uint8_t csbits = 0;
      for(divisor=CLKDIV; divisor != 0; divisor>>=1)
         csbits++;
         writeMasked(csbits, 0x0f, TCCR1B);
   */

//#if ((LVDS_PRESCALER != 64) && (LVDS_PRESCALER != 32) && \
//     (LVDS_PRESCALER != 16) && 
#if ((LVDS_PRESCALER != 8) && \
     (LVDS_PRESCALER != 4)  && (LVDS_PRESCALER != 2) && \
     (LVDS_PRESCALER != 1))
#error "LVDS_PRESCALER must be a power of 2, from 1 to 8"
#endif

   //Figured this out in cTools/dePower.c...
   //64 is overkill here, since the deadTimer prescaler only goes to 8...
#define divToCS(div) \
   ( (div == 64) ? 7 : (div == 32) ? 6 : (div == 16) ? 5 : (div == 8) ? 4 \
     : (div == 4) ? 3 : (div == 2) ? 2 : (div == 1) ? 1 : 0)


   //CSBITS (through PLL/8) (CS10 is bit 0)
   //CS12:10   CS12   CS11   CS10      PLL division
   //1         0      0      1         1
   //2         0      1      0         2
   //3         0      1      1         4
   //4         1      0      0         8
   #define CSBITS divToCS(LVDS_PRESCALER) //<<CS10 should be redundant
   //DTPSBITS (DTPS10 is bit 4)
   //DTPS11:10   DTPS11 DTPS10   PLL division
   //0         0      0         1
   //1         0      1         2
   //2         1      0         4
   //3         1      1         8
   #define DTPSBITS ((CSBITS-1)<<DTPS10)

   //Whoops! Forgot this (a/o 42-som'n WTF1pix)
   // so, it's promising to get single-pixel control...
   //  the result was kinda ugly, though... scroll was quite slow
   // Further, it was only drawing a certain number of pixels...
   //  (horizontally... pre LVDS_PRESCALER affecting DOTS_TO_CYC)
   //  so accessing *all* pixels in a row would make it even slower...
   // Further, it didn't seem to be paying attention to the fact that
   // the same pixels are drawn on multiple rows...
   // WTF? (the image was scaled, proportionately! 1pix x 1pix)
   pll_enable();

#else
   //No clock divisor
   #define CSBITS (1<<CS10)
   //No dead-timer divisor
   #define DTPSBITS 0
   pll_enable();
   //#warning "The PLL configuration code is not in here yet!"
#endif

   //Set the Timer1 clock prescaler...
   writeMasked(CSBITS, 
               ((1<<CS13) | (1<<CS12) | (1<<CS11) | (1<<CS10)),
               TCCR1B);

   //Set the DeadTime prescaler (no prescaling, same speed as TCNT1)...
   // Allegedly this is prescaled from the PCK (or CK)
   //    NOT from the Timer1 prescaler...
   writeMasked(DTPSBITS,
               ((1<<DTPS11) | (1<<DTPS10)),
               TCCR1B);



   //All LVDS modes (and signals) use FastPWM.. 
 
   //FastPWM
//Now Handled in lvds_xxxOnCompare():

   //These are also written below (excluding PWM1D)
   setbit(PWM1A, TCCR1A);  //Enable PWM on OC1A for DVH
                           //Need to do the same for other channels
  
   setbit(PWM1B, TCCR1A);  //Enable PWM on OC1B for CLOCK 

   setbit(PWM1D, TCCR1C);

   //PWM1D is not in TCCR1A...

   setoutPORT(PB1, PORTB);   //+OC1A, DVH/BLUE, MISO (usually heart)
//   setoutPORT(PB0, PORTB);   //-OC1A    MOSI unused
   setoutPORT(PB2, PORTB); //-OC1B, -GREEN    (INVERTED) SCK
   setoutPORT(PB3, PORTB); //+OC1B Clock (OC1B, not inverted)
   setoutPORT(PB5, PORTB); //+OC1D, RED


   writeMasked(((0<<WGM11) | (0<<WGM10)), //FastPWM (combined with above)
               ((1<<WGM11) | (1<<WGM10)), // (affects all PWM channels)
               TCCR1D);

   //OC1A is used, but /OC1A isn't
// Not Valid (deadTime only functions if in complementary-mode):
   // OTOH: dead-time is not necessary in the only case we need to switch
   // Most lvds states use clear on compare-match, set at 0
   // But there is one that uses the opposite
   // This can be toggled by a single bit-change
   //  COM1A1 = 1
   //  COM1A0 value indicates what happens on Compare-Match
   //                 or ! at BOTTOM
//#define lvds_clrOnCompare() clrbit(COM1A0, TCCR1A)
//#define lvds_setOnCompare() setbit(COM1A0, TCCR1A)
   //HOWEVER: the alternative is possible:
   // Use complementary (clear-on-compare)
   // and single-ended with set-on-compare
   // COM1A0 = 1
   // COM1A1 value indicates what happens on Compare-Match

/* These are three instructions apiece!
#define lvds_ComplementaryClrOnCompare() clrbit(COM1A1, TCCR1A)

#define lvds_setOnCompare() setbit(COM1A1, TCCR1A)
#define lvds_clrOnCompare() lvds_ComplementaryClrOnCompare()
*/

//Page 99:
// "In Fast PWM Mode ... when the COM1x1:0 bits are set to “01” ...
//  an user programmable Dead Time delay is inserted for 
//  these complementary output pairs (OC1x and OC1x)."

//Page 98:
// "The counter is loaded with a 4-bit DT1H or DT1L value from DT1 
//  I/O register, depending on the edge of the Waveform 
//  Output (OCW1x) when the dead time insertion is started."
// In other words, (as it appears from the diagram)
//  the dead-time value is loaded into the counter when the corresponding
//  edge in OCW1x is detected
//  So changing the value of DT1 affects the *next* corresponding edge 
//   (NOT if the dead timer is already running)

//Page 96:
// "The OCR1x Registers are double buffered when using any of the 
// Pulse Width Modulation (PWM) modes."
// "The double buffering synchronizes the update of the OCR1x 
// Compare Registers to either top or bottom of the counting sequence.
// The synchronization prevents the occurrence of odd-length, non-sym- 
// metrical PWM pulses, thereby making the output glitch-free."

//Page 100:
//"A change of the COM1x1:0 bits state will have effect 
// at the first Compare Match after the bits are written."
//Page 97: HAH!
//"Be aware that the COM1x1:0 bits are not double buffered 
// together with the compare value. 
// Changing the COM1x1:0 bits will take effect immediately."


//#define lvds_setOnCompare()   
//   TCCR1A = ( (1<<COM1A1) | (1<<COM1A0) 
//            | (0<<COM1B1) | (1<<COM1B0) 
//            | (1<<PWM1A) | (1<<PWM1B) )
//#define lvds_ComplementaryClrOnCompare() \ //
   //Do TCCR1C first, because it contains shadow-bits of TCCR1A that I
   // don't want to have to rewrite...
   TCCR1C = ( (1<<COM1D1) | (0<<COM1D0)
            | (1<<PWM1D) );
/* a/o v62-17: The notes are sparse as to why I disabled the Green signal
 * at all times, except during DE... As I recall, it was an experiment with
 * whether it was possible to get greater control over BLUE, by hacking the
 * clock signal during the DE period... e.g. since none of the V/H timings
 * are necessary during DE, maybe it was possible to have a clock that
 * wasn't perfectly as-spec'd... it didn't work. So, why not remove the
 * necessity for enabling/disabling the green signal...
 (Yes, it's that tightly-knit that affecting the clock affects green,
 affects blue... but there's some confusion in my mind as to why green
 couldn't've remained active anyhow...)
 Ahh, wait, something about when Complementary mode is enabled, then the
 clock is affected by Dead-Times... Still a bit vague.
*/
//	TCCR1A = ( (0<<COM1A1) | (1<<COM1A0) 
//            | (1<<COM1B1) | (0<<COM1B0) //Don't use complementary for CLK
//            | (1<<PWM1A) | (1<<PWM1B) );
	lvds_disableGreen_MakeClockInsensitiveToDT();
	//This was attempted in v66.51 (a/o v91)
	// And for some reason didn't work... It should, no? See notes re
	// FULL_INIT_TESTS and v66.51 in lvds.h
// lvds_enableGreen_MakeClockSensitiveToDT();


	
/* ... but this didn't work... nothing's showing up at all.
	TCCR1A = ( (0<<COM1A1) | (1<<COM1A0)
	         | (0<<COM1B1) | (1<<COM1B0)
	         | (1<<PWM1A) | (1<<PWM1B) );
*/

//#define lvds_clrOnCompare() lvds_ComplementaryClrOnCompare()

   // Enable Complementary-Mode (and thus the dead-timer)
   // This'll be changed as needed, but we need COM1A0 set prior to that

   //   lvds_clrOnCompare();
//   writeMasked(((0<<COM1A1) | (1<<COM1A0)), 
//               ((1<<COM1A1) | (1<<COM1A0)), 
//               TCCR1A);



   //THIS IS NOT THOROUGHLY THOUGHT-OUT...
   // as I recall, we need to use /OC1B for the clock output
   //  because it won't be affected by the deadtime...
//Now Handled in lvds_xxxOnCompare():
//   writeMasked(((0<<COM1B1) | (1<<COM1B0)),
//               ((1<<COM1B1) | (1<<COM1B0)),
//               TCCR1A);

   //Since PWM inversion affects all channels, great-pains were taken
   // to assure that it needn't be changed in any lvds state...
   // PWM inversion must be enabled:

   //inverted with PWM1X
//   setbit(PWM1X, TCCR1B);


//Another attempt (in v66.51, a/o v91):
	// Irrelevent (?) since Nada_init() overwrites it...
	// //Make sure Green is off...
	// DT1 = 3;



    Nada_init();
}




#endif //__LVDS_C__


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
 * /Users/meh/_avrProjects/LCDdirectLVDS/93-checkingProcessAgain/_interfaces/lvds.c
 *
 *    (Wow, that's a lot longer than I'd hoped).
 *
 *    Enjoy!
 */

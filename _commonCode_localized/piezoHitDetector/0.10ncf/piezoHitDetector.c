/* mehPL:
 *    This is Open Source, but NOT GPL. I call it mehPL.
 *    I'm not too fond of long licenses at the top of the file.
 *    Please see the bottom.
 *    Enjoy!
 */


//piezoHitDetector.c 0.10ncf

//Use a piezo-element connected to an ADC to detect physical hits on a
//project-box...
//Not particularly reliable, but works well enough when the sun's out


//0.10ncf Stolen from LCDdirectLVDS67, for the Mario Question Box...
//        See LCDdirectLVDS68 for implementation.






//   Piezo element
//   (contact-mic/
//    "hit-sensor")    3.3V 3.3V
//                      |    |
//                      \   _|__
//    ______            /    /\									//
//   /  __  \           \   /__\									//
//  |  /  \  |    ||    |    |
//  | |   o-------||----+----+-------> ADCin6
//  |  \__/  |    ||    |    |
//   \o_____/           \   _|__
//    |                 /    /\									//
//   GND                \   /__\									//
//                      |    |
//                     GND  GND
//
// AGAIN: This whole hit-sensor is completely hokey... the analog threshold
// values in main.c certainly would need to be changed according to your
// circuit. And, even after fine-tuning, mine acts differently in different
// temperatures, etc. It's *very* sensitive to electrical interference
// (ended up shielding *everything*). And, even viewing it on the 'scope,
// the actual "hit" is barely discernable from just regular noise in the
// circuit. Certainly the code could stand to be improved. You might be
// best-off replacing it with a button.



#include _ADC_HEADER_

void phd_init(void)
{
	adc_takeInput(6);
	adc_init();
	adc_select(6);

#warning "adc_startConversion has been moved... this is untested!"
	//In main, adc_startConversion() was run immediately before entering the
	//while(1) loop
	// I don't think it should matter, but we'll see.
	adc_startConversion();
}

//This is called in the main while(1) loop...
// Returns TRUE if a "hit" is detected.
uint8_t phd_update(void)
{
		static int32_t adcAvg = 0;
		static uint16_t adcVal;
		static uint8_t avgCount = 100;
		
		uint8_t retVal = FALSE;
		
		if(!adc_isBusy())
		{
			adcVal = adc_getValue();
			
			adc_startConversion();


			if(avgCount > 1)
				avgCount--;
			else if(avgCount == 1)
			{
				avgCount=0;
				adcAvg += adcVal;
			}
			else if(avgCount == 0)
			{
				//After the first case, the average should be somewhere
				//'round the ADC Val... then after subtracting the current
				//adcVal, we should be close to zero...
				adcAvg -= adcVal;
				{
					int32_t absDiff = adcAvg;
					if(absDiff < 0)
						absDiff = -absDiff;

					// At this point, absDiff is essentially the derivative
					// of the ADC measurements... (absoluted)
					// If there's no change, it will be near-zero
					// If there's a lot of change, it will be positive...
					// the ADC is ten-bits, so 3.3V/1024 = .003V per LSB
					// Of course, a spike isn't instantaneous...
					// .2V is measurable on the 'scope, currently...
#define ADC_DETECTION_THRESHOLD	15 //20 //38 //(1024*125/3300)
					if(absDiff > ADC_DETECTION_THRESHOLD)
						retVal = TRUE;
						//fbQuestion_hitDetected();
				}
				adcAvg = 0;
				avgCount=1;
			}

		}

		return retVal;
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
 * /Users/meh/_avrProjects/LCDdirectLVDS/90-reGitting/_commonCode_localized/piezoHitDetector/0.10ncf/piezoHitDetector.c
 *
 *    (Wow, that's a lot longer than I'd hoped).
 *
 *    Enjoy!
 */

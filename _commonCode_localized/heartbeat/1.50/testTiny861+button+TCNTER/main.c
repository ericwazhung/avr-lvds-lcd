/* mehPL:
 *    This is Open Source, but NOT GPL. I call it mehPL.
 *    I'm not too fond of long licenses at the top of the file.
 *    Please see the bottom.
 *    Enjoy!
 */


#include _HEARTBEAT_HEADER_

int main(void)
{

	/* Now in tcnter0.30
	//Initialize the counter for tcnter (see init_dmsTimer)
	//OCR0A = TCNTER_SOURCE_OVERFLOW_VAL;
	//??? What if the timer is 16-bit...?
	timer_setWGM(0, WGM_NORMAL);	//unnecessary?
	timer_selectDivisor(0, CLKDIV64);
	*/



	init_heartBeat();

	uint8_t heartBlink = 0;
	uint8_t buttonPressed = FALSE;

//	set_heartBlink(3);

	uint8_t loopCount = 0;

	while(1)
	{
		heartUpdate();

		//Intentionally allowed to wraparound from 255->0...
		loopCount++;

		if(loopCount == 0)
		{
			//Read the heartbeat's pin-value
			// doing-so briefly turns the pin into an input...
			// If, e.g. a momentary-pushbutton pulls the pin low,
			// heartPinInputPoll() will return 0
			// Otherwise, it's pulled high by internal pull-up resistor
		
			//As implemented here:
			//The pin is only polled once every 256 loops, so-as not to
			//interfere with the LED's brightness when the button is not
			//pressed. 
			//(Of course, pressing the button causes the LED to light-up at
			//full-brightness until it's released, since it's on the same pin)
			if(heartPinInputPoll())
			{
				//Has the button just been released?
				if(buttonPressed)
				{
					// Pressing the button once will cause it to blink once,
					// twice after the second press
					// and so-on, until 4, when it should go back to fading.
					heartBlink++;
					heartBlink %= 4;
	
					set_heartBlink(heartBlink);
				}
				
				buttonPressed = FALSE;
			}
			else
				buttonPressed = TRUE;
		}
	}

	return 0;
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
 * /Users/meh/_avrProjects/LCDdirectLVDS/93-checkingProcessAgain/_commonCode_localized/heartbeat/1.50/testTiny861+button+TCNTER/main.c
 *
 *    (Wow, that's a lot longer than I'd hoped).
 *
 *    Enjoy!
 */

/* mehPL:
 *    This is Open Source, but NOT GPL. I call it mehPL.
 *    I'm not too fond of long licenses at the top of the file.
 *    Please see the bottom.
 *    Enjoy!
 */


//Taken from stdinNonBlock
//  Taken from serIOtest20, and modified slightly

#include <stdio.h>
#include <errno.h>
#include <string.h> //needed for strerror()
#include <fcntl.h> //file control for open/read
#include _TCNTER_HEADER_


//These need to be global because they're used by polled_uar.c
// via #defines...
//uint8_t 
//tcnter_source_t 
// Must be greater than tcnter_source_t *here*
// because it will be wrapped-around mathematically below...
// NOCANDO... it's externed...
//uint16_t 
tcnter_source_t timerCounter = 0;


#define handleError(string, exit) \
	if(errno)   \
{\
	   printf( string " Error %d: '%s'\n", errno, strerror(errno)); \
	   if(exit) \
	      return 1; \
	   else \
			errno = 0; \
}

//errno = 0 seems to work, whereas clearerr(stdin) doesn't

int main(void)
{

	int stepSize = 1;
	int quit = 0;
	int timer = 0;

	// Set STDIN non-blocking (Still requires Return)
	int flags = fcntl(0, F_GETFL);
	flags |= O_NONBLOCK;
	fcntl(0, F_SETFL, flags);

	handleError("Couldn't open stdin.", 1);

	printf("---Press q and press return to quit---\n");
	printf("   Press 1-8 and press return to change the step-size\n");

	//POLLED_UAR Specific Stuff
	tcnter_init();


	while(!quit)
	{
		//This worked in 0.10-3! But now 0.10-4?!
//		handleError(" Unhandled Error.", 0);

		char kbChar;
		kbChar = getchar();

		//"Resource Temporarily Unavailable" isn't an error in this case
		if(errno == 35)
			errno = 0;
		else
		{
			switch(kbChar)
			{
				case 'q':
					quit = 1;
					break;
				case '1':
					stepSize = 1;
					break;
				case '2':
					stepSize = 2;
					break;
				case '3':
					stepSize = 3;
					break;
				case '4':
					stepSize = 4;
					break;
				case '5':
					stepSize = 5;
					break;
				case '6':
				   stepSize = 6;
				   break;
				case '7':
				   stepSize = 7;
					break;
				case '8':
					stepSize = 8;
					break;

				case '\n':
					break;
				//^^^ TO HERE
				default:
					printf("Received: '%c'", kbChar);
			}
		}


	//	tcnter_update();

		timer++;

		if(timer == 1000)
		{
			timerCounter+=stepSize;
			
			//since timerCounter is tcnter_source_t (uint8_t, here)
			// this needn't be done for wraparound after 0xff...
#if(TCNTER_SOURCE_OVERFLOW_VAL <= UINT8_MAX)
			if(timerCounter >= TCNTER_SOURCE_OVERFLOW_VAL)
				timerCounter -= TCNTER_SOURCE_OVERFLOW_VAL;
#elif (TCNTER_SOURCE_OVERFLOW_VAL > (UINT8_MAX+1))
 #error "wraparound math won't work here..."
#endif

			timer=0;

			if(   (timerCounter < 3) 
				|| (timerCounter > TCNTER_SOURCE_OVERFLOW_VAL - 3) )
			printf("TCNT: %d, tcnter=%d\n",timerCounter, tcnter_get());
		}

		tcnter_update();

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
 * /Users/meh/_avrProjects/LCDdirectLVDS/90-reGitting/_commonCode_localized/tcnter/0.30/testuint8Max/main.c
 *
 *    (Wow, that's a lot longer than I'd hoped).
 *
 *    Enjoy!
 */

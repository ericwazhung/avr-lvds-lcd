/* mehPL:
 *    This is Open Source, but NOT GPL. I call it mehPL.
 *    I'm not too fond of long licenses at the top of the file.
 *    Please see the bottom.
 *    Enjoy!
 */




#include <stdio.h>
#include <stdint.h>
#include <math.h>

#define PI		(double)(3.14159265358979)
#define QUADRANT	(PI/2)


#error "Make sure you select SINEMAX as desired"
// SINEMAX is the maximum, thus SINEDIST is 2*SINEMAX
#define SINEMAX		(INT8_MAX) //(INT16_MAX)

/* Sine(theta) will be the same as sin(theta)=[-1,1]
 *  with a magnitude of SINEMAX... Sine(theta) = [-SINEMAX, SINEMAX]
 *
 * Worst-case-scenario: distance=xmax-xmin is INT32_MAX (assumed per motorpoxn)
 * The formula for determining position (assuming x is 0-centered)
 *   determined by (0-centered means xmax = -xmin, and distance = 2*xmax)
 *   and   Sine(theta)/(SINEMAX)=1
 *   thus 
 *  Sine(theta)/SINEMAX * xmax gives the new coordinates
 * without using floats this is best handled by:
 *  Sine(theta) * xmax / SINEMAX
 *  BUT:
 *  SINEMAX * (INT32_MAX/2) requires an int64!
 * So... either use an int64 or use int16's and scale up
 * don't forget that the maximum amplitude should give xmax...
 * amplitude * Sine(theta) * xmax / AMPLITUDEMAX / SINEMAX
 * could do steps... with int64s as a first test...
 *  amplitude * Sine(theta) / AMPLITUDEMAX <= Sine(theta)
 */

int main(void)
{
	int32_t sine;
	int32_t position;
	double rads;
	double numSteps;
	double stepSize;
	int32_t value;
	int32_t steps;
	int32_t i;

	numSteps = 128;
	stepSize = QUADRANT / numSteps;

	//The array will be 129 values
	// covering sin(0) = 0 and sin(PI/2) = 1
	//  these values overlap on the axes (between quadrants)
	//  so choose one quadrant if it's on an axis, which gives 512 values...
	for(steps = 0; steps <= numSteps; steps++)
	{
		value = (int32_t)(lround((double)SINEMAX * (double)sin(rads)));	
		printf("%d, ",value);
//		for(i=0; i<value; i+=85899344)
//			printf("*");
		if(steps % 5 == 0)
			printf("\\\n");
		//printf("\n");

		rads += stepSize;
	}

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
 * /Users/meh/_avrProjects/LCDdirectLVDS/68-backToLTN/_commonCode_localized/sineTable/0.99/sineCreate/sineCreate.c
 *
 *    (Wow, that's a lot longer than I'd hoped).
 *
 *    Enjoy!
 */

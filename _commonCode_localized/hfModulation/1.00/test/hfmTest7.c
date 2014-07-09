/* mehPL:
 *    This is Open Source, but NOT GPL. I call it mehPL.
 *    I'm not too fond of long licenses at the top of the file.
 *    Please see the bottom.
 *    Enjoy!
 */


#include <stdio.h>
#define TRUE 1
#define FALSE 0
#include "../hfModulation.c"

int main(void)
{
	hfm_t hfmThing;

	hfm_setup(&hfmThing, 30, 50);

	int i;

	printf("Go!\n");
	printf("\n");

	int useSetPower;
	for(useSetPower=0; useSetPower<=1; useSetPower++)
	{
		if(useSetPower)
			printf("Using hfm_setPower on each change:\n");
		else
			printf("Using hfm_setup on each change:\n");


	int j;
	for(j=0; j<11; j++)
	{
		int k = (j > 5) ? 10-j : j;

		printf("%d/%d:|", k, 5);

		if(useSetPower)
			hfm_setPower(&hfmThing, k);
		else
			hfm_setup(&hfmThing, k, 5);

	for(i=0; i<=73; i++)
	{
		//int output = hfm_nextOutput(&hfmThing);

		//if(output)
		//	printf("#\n");
		//else
		//	printf("\n");

		//hfm_setPower&hfmThing, i);
		printf("%c", (hfm_nextOutput(&hfmThing) ? '#' : ' '));
	}
		printf("|\n");
	}
	}

	printf("\n");

	printf("Attempting a gradient with realtime hfm_setPower() calls\n");
	hfm_setup(&hfmThing, 0, 79);

	for(i=0; i<=79; i++)
	{
		
		hfm_setPower(&hfmThing, i);
		printf("%c", (hfm_nextOutput(&hfmThing) ? '#' : ' '));
		//hfm_setPower(&hfmThing, i);
	}
	printf("\n");


	printf("\n");

#define NUMSTEPS	16
	for(useSetPower = 0; useSetPower <= 1; useSetPower++)
	{

		printf("Attempting three short gradients with realtime hfm_setPower() calls\n");
		printf(" This sorta simulates a color-gradient on an LCD that has \n"
			 " a limited number of colors (e.g. 4 shades of red: 0,1,2,3)\n");
		if(useSetPower)
			printf(" Not calling hfmSetup between each gradient\n");
		else
			printf(" Calling hfmSetup between each gradient\n");

		//if(!useSetPower)
			hfm_setup(&hfmThing, 0, NUMSTEPS);

		for(i=0; i<=NUMSTEPS; i++)
		{
		
			hfm_setPower(&hfmThing, i);
			printf("%c", (hfm_nextOutput(&hfmThing) ? '.' : ' '));
			//hfm_setPower(&hfmThing, i);
		}

		//printf("|");
	
		if(!useSetPower)
			hfm_setup(&hfmThing, 0, NUMSTEPS);

		for(i=0; i<=NUMSTEPS; i++)
		{
		
			hfm_setPower(&hfmThing, i);
			printf("%c", (hfm_nextOutput(&hfmThing) ? 'o' : '.'));
			//hfm_setPower(&hfmThing, i);
		}

		//printf("|");
		if(!useSetPower)
			hfm_setup(&hfmThing, 0, NUMSTEPS);
		for(i=0; i<=NUMSTEPS; i++)
		{
		
			hfm_setPower(&hfmThing, i);
			printf("%c", (hfm_nextOutput(&hfmThing) ? 'O' : 'o'));
			//hfm_setPower(&hfmThing, i);
		}



		//printf("|<end\n");
		printf("\n");
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
 * /Users/meh/_avrProjects/LCDdirectLVDS/90-reGitting/_commonCode_localized/hfModulation/1.00/test/hfmTest7.c
 *
 *    (Wow, that's a lot longer than I'd hoped).
 *
 *    Enjoy!
 */

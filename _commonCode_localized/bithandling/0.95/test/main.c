/* mehPL:
 *    This is Open Source, but NOT GPL. I call it mehPL.
 *    I'm not too fond of long licenses at the top of the file.
 *    Please see the bottom.
 *    Enjoy!
 */


#include "../bithandling.h"
#include <stdio.h>
#include <inttypes.h>

#define HEX8	"0x%02"PRIx8



#ifndef _MMIO_BYTE
 //From avr/sfrdefs.h:
 #define _MMIO_BYTE(mem_addr) (*(volatile uint8_t *)(mem_addr))
 #define _SFR_IO8(io_addr) _MMIO_BYTE((io_addr) + __SFR_OFFSET)
 #define __SFR_OFFSET 0x00
 //From avr/iom8515.h:
 #define PORTA   _SFR_IO8(0x1B)
#else
 #error "WTF, MMIO_BYTE is defined?"
#endif



uint8_t portStuff[3];
#define FAKE_PORT _SFR_IO8((int)(&(portStuff[2])))
#define FAKE_DDR  _SFR_IO8((int)(&(portStuff[1])))
#define FAKE_PIN  _SFR_IO8((int)(&(portStuff[0])))

void printPortState(void);


void printBin8(uint8_t value)
{
	int i;
	for(i=7; i>=0; i--)
	{
		printf("%"PRIu8" ", getbit(i, value));
	}

	printf("\n");

}


int main(void)
{
	printf("\n");
	printf("This is hardly exhaustive...\n");


	printf("WriteMasked...\n");

	uint8_t orig = bin(0,0,0,0,1,1,1,1);
	uint8_t val  = bin(0,0,1,1,0,0,1,1);
	uint8_t mask = bin(0,1,0,1,0,1,0,1);
	uint8_t wm   = orig;

	writeMasked(val, mask, wm);

	printf("original: 0x%02"PRIx8" = ", orig); printBin8(orig);
	printf("value:    0x%02"PRIx8" = ", val); printBin8(val);
	printf("mask:     0x%02"PRIx8" = ", mask); printBin8(mask);
	printf("---------------------------------\n");
	printf("output:   0x%02"PRIx8" = ", wm); printBin8(wm);

	printf("\n");


	
	
	
	
	
	
	
	printf("&PORTA = "HEX8"\n"
		    "&DDR_FROM_PORT(PORTA) = "HEX8"\n"
			 "&PIN_FROM_PORT(PORTA) = "HEX8"\n",
			 (uint8_t)(&PORTA), (uint8_t)(&DDR_FROM_PORT(PORTA)), 
			 (uint8_t)(&PIN_FROM_PORT(PORTA)));


	printf("\n");


	FAKE_PORT = 0x55;

	printf("FAKE_PORT = 0x55; -> FAKE_PORT=="HEX8" ", FAKE_PORT);
	printBin8(FAKE_PORT);

	printf("(FAKE_PORT -> portStuff[2]=="HEX8")\n", portStuff[2]);
	
	DDR_FROM_PORT(FAKE_PORT)=0x44;
	PIN_FROM_PORT(FAKE_PORT)=0x33;

	printf("FAKE_DDR=="HEX8", portStuff[1]=="HEX8" ",
												FAKE_DDR, portStuff[1]);

	printBin8(FAKE_DDR);

	printf("FAKE_PIN=="HEX8", portStuff[0]=="HEX8" ",
												FAKE_PIN, portStuff[0]);
	printBin8(FAKE_PIN);


	printf("\n");
	printf("setPORToutMasked(FAKE_PORT, 0x3f):\n");
	setPORToutMasked(FAKE_PORT, 0x3f);
	printPortState();

	printf("\n");
	printf("setPORTinpuMasked(FAKE_PORT, 0x3f):\n");
	setPORTinpuMasked(FAKE_PORT, 0x3f);
	printPortState();

	printf("\n");
	printf("PORTin(FAKE_PORT)=="HEX8"\n", PORTin(FAKE_PORT));
	printPortState();

	printf("\n");
	printf("setPORTout(FAKE_PORT)\n");
	setPORTout(FAKE_PORT);
	printPortState();

	printf("\n");
	printf("setPORTinPU(FAKE_PORT)\n");
	setPORTinpu(FAKE_PORT);
	printPortState();
}

void printPortState(void) //uint8_t port[3])
{
	printf("FAKE_PORT=="HEX8" = ", FAKE_PORT); printBin8(FAKE_PORT);
	printf("FAKE_DDR=="HEX8"  = ", FAKE_DDR); printBin8(FAKE_DDR);
	printf("FAKE_PIN=="HEX8"  = ", FAKE_PIN); printBin8(FAKE_PIN);
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

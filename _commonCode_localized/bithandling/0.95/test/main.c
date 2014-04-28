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

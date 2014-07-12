/* mehPL:
 *    This is Open Source, but NOT GPL. I call it mehPL.
 *    I'm not too fond of long licenses at the top of the file.
 *    Please see the bottom.
 *    Enjoy!
 */












// STOLEN FROM glColorTest2 (v3's is no longer functional)

#define TO_GIMP_PALETTE TRUE


#ifdef __cplusplus
extern "C" {
//#include <cinttypes>
#else
//#include <inttypes.h>
#endif
	
//LATEST VERSION MOVED TO glColorTest
#include "../../../_commonCode/bithandling/0.94/bithandling.h"
//#include <cinttypes>

#ifndef NOLVDSMAIN
#include <stdio.h>
#include <inttypes.h>
#endif

//Don't count colors in this range...
#ifndef COLORMATCHRANGE
#define COLORMATCHRANGE 0//1//2
#endif

//0 or 1 (Not used, or used)
#ifndef HSYNC_USED
#define HSYNC_USED 0//1//0
#endif



#define OLDSTYLE
//The intermediate colors are indescernable (on my screen)
// (Note that even the two blues are indescernable...
// though their values are quite different, 47 vs 63!
// adjusting colorSync made the difference visible...)
#define FOUR_COLOR_LIMITED



#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif


uint8_t keepDups = FALSE;
uint8_t invertRed = 0;
uint8_t invertGreen = 0;
uint8_t invertBlue = 0;

// OCW1: General operation: Cleared on compare-match, Set when TCNT=0
//
// THIS DRAWING IS REVERSED HIGH/LOW
//    TCNT:     0    1    2    3    4    5    6
//                      OCB1 OCB2 OCB3 OCB4 OCB5
//                       v    v    v    v    v
// OCRB=0:     ____ ____ ____ ____ ____ ____ ____
//            \____\____X____X____X____X____X____/____ vvvvvv OCB>=6
//            ^    ^     ^    ^    ^    ^    ^
//            DT0  DT1  DT2  DT3  DT4  DT5  DT6
// Return Value is the "data" bits in order...
// such that tcnt 6 corresponds to bit6 (e.g. <<6)
// backwards from the TCNT listing above.
uint8_t dthBits(uint8_t dth, uint8_t ocr)
{
#if(!defined(TO_GIMP_PALETTE) || !TO_GIMP_PALETTE)
#ifndef NOLVDSMAIN
	printf("dth: ");
#endif
#endif
	if(ocr == 0)
	{
#if(!defined(TO_GIMP_PALETTE) || !TO_GIMP_PALETTE)
#ifndef NOLVDSMAIN
		printf("0 0 0 0 0 0 0 ");
#endif
#endif
		return 0x00;
	}

	uint8_t data[7] = {0,0,0,0,0,0,0};

	uint8_t tcnt=0;

	//Handling the deadtimer...
	//if(dth <= ocr+1)
		for(; (tcnt<7) && (tcnt<dth); tcnt++)
			data[tcnt] = 0;

	//Now the time between the deadtimer (or start) and OCR match
	for(;(tcnt<7)&&(tcnt<=ocr);tcnt++)
		data[tcnt] = 1;

	//Now the rest of the bits
	for(;tcnt<7;tcnt++)
		data[tcnt] = 0;

	uint8_t val = 0;

	//printf("dth: ");
	for(tcnt=0; tcnt<7; tcnt++)
	{
#if(!defined(TO_GIMP_PALETTE) || !TO_GIMP_PALETTE)
#ifndef NOLVDSMAIN
		printf("%"PRIu8" ", data[tcnt]);
#endif
#endif
		val |= (data[tcnt])<<tcnt;
	}
	//printf("\n");

	return val;	

}


// OCW1: Cleared on compare-match, Set when TCNT=0
//
// THIS DRAWING IS REVERSED HIGH/LOW
//    TCNT:     0    1    2    3    4    5    6
//                      OCB1 OCB2 OCB3 OCB4 OCB5
//                       v    v    v    v    v
// OCRB=0:     ____ ____ ____ ____ ____ ____ ____
//             ____ ____X____X____X____X____X____/____ vvvvvv OCB>=6
//                       ^    ^    ^    ^    ^    ^
//                  ??   1D0  1D1  1D2  1D3  1D4
//                            2D0  2D1  2D2  2D3
//                                 3D0  3D1  3D2
//                                      4D0  4D1
//                                           5D0
//
// SEE NOTES for dthBits regarding bit-order of the output...
// (tcnt 6 corresponds to bit6)
uint8_t dtlBits(uint8_t dtl, uint8_t ocr)
{
#if(!defined(TO_GIMP_PALETTE) || !TO_GIMP_PALETTE)
#ifndef NOLVDSMAIN
	printf("dtl: ");
#endif
#endif
	if(ocr == 0)
	{
#if(!defined(TO_GIMP_PALETTE) || !TO_GIMP_PALETTE)
#ifndef NOLVDSMAIN
		printf("0 0 0 0 0 0 0 ");
#endif
#endif
		return 0x00;
	}

	uint8_t tcnt;

	uint8_t data[7] = {0,0,0,0,0,0,0};

	for(tcnt=0; tcnt<ocr+dtl+1; tcnt++)
		data[tcnt] = 1;

	for( ; tcnt<7; tcnt++)
		data[tcnt] = 0;

	uint8_t val = 0;

	//printf("dtl: ");
	for(tcnt=0; tcnt<7; tcnt++)
	{
#if(!defined(TO_GIMP_PALETTE) || !TO_GIMP_PALETTE)
#ifndef NOLVDSMAIN
		printf("%"PRIu8" ", data[tcnt]);
#endif
#endif
		val |= (data[tcnt])<<tcnt;
	}
	//printf("\n");

	return val;
}


//calc___Olds's are for simulating the old color-scheme...
// This is based on timing which cannot be varied too dramatically...
// (e.g. there're only two shades of blue possible, otherwise the LVDS
//  clock would be affected). Currently it has 48 roughly-evenly-spaced
//  colors, covering the whole range from black to white...

// Blue is severely limited... changing DTH to anything other than 0
// affects the LVDS clock. Some displays may not care, but mine does.
// DTH *SHOULD BE* 0 in all calls to this function, unless you have a
// brilliant idea worth attempting...

//  TCNT:     |  0   1    2    3  | 4    5    6
//              B3   B2   DE   /V   /H   B5   B4
// "Blue/DVH" |____ ____ ____.____.____ ____ ____
//  OC1A:  D0>/ D1>/ D2>/           C4>\ C5>\ C6...
//            |¯¯¯¯ ¯¯¯¯                ¯¯¯¯ ¯¯¯¯
uint8_t calcBlueOld(uint8_t DTH, uint8_t OCRB)
{
#if(!defined(TO_GIMP_PALETTE) || !TO_GIMP_PALETTE)
#ifndef NOLVDSMAIN
	printf("b");
#endif
#endif
	uint8_t rawBits = dthBits(DTH, OCRB);

	uint8_t rawBit;
	uint8_t blue = 0;

	writebit(5, blue, getbit(5, rawBits));
	writebit(4, blue, getbit(6, rawBits));
	writebit(3, blue, getbit(0, rawBits));
	writebit(2, blue, getbit(1, rawBits));

	//invertBlue may not be possible, due to its effect on timing...
	// at the very-least the plausibility of it hasn't been explored
	// FURTHER, it may not be legit, since this only affects bits 5:2
	// and this math affects *all* bits
#warning "invertColors need to be revisited... their bits aren't all exclusive to the inverted channel! this might be fixed now..."
	if(invertBlue)
		return (~blue)&bin(0,0,1,1,1,1,0,0); //0x3f;
	else
		return blue;	

}

// Green is pretty limited, as it shares the same pwm channel as the clock
// its ocr never varies... OCRG *SHOULD BE* 3.

//  TCNT:     |  0   1    2    3  | 4    5    6
//  CLKideal: /¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯\______________/
//            | G2   G1   B1   B0 | G5   G4   G3
//  Green:    |                   |____ ____ ____
//  /OC1B     \    .    .    . d0>/ d1>/ d2>/ d3?\  //
//            |¯¯¯¯ ¯¯¯¯ ¯¯¯¯ ¯¯¯¯ ¯¯¯¯ ¯¯¯¯ ¯¯¯¯
uint8_t calcGreenOld(uint8_t DTL, uint8_t OCRG, uint8_t *blue)
{
#if(!defined(TO_GIMP_PALETTE) || !TO_GIMP_PALETTE)
#ifndef NOLVDSMAIN
	printf("g");
#endif
#endif
	//OCRG is constant, 3... otherwise the clock would be affected
	// Though, maybe for testing that should be a limit in the calling
   //	function, not here?
	uint8_t rawBits = dtlBits(DTL, OCRG);
	uint8_t green = 0;

	writebit(5, green, getbit(4, rawBits));
	writebit(4, green, getbit(5, rawBits));
	writebit(3, green, getbit(6, rawBits));

	//These should be irrelevent, but might be useful if using invertGreen.
	writebit(2, green, getbit(0, rawBits));
	writebit(1, green, getbit(1, rawBits));

	uint8_t blueTemp = 0;
	writebit(1, blueTemp, getbit(2, rawBits));
	writebit(0, blueTemp, getbit(3, rawBits));

	if(invertGreen)
	{
		*blue |= (~blueTemp)&bin4(1,1,0,0);
		return (~green)&bin(0,0,1,1,1,1,1,0);
	}
	else
	{
		*blue |= blueTemp;
		return green;
	}
}


//Again, DTH shouldn't be used, as it affects the clock...
// its value should be 0. FURTHER: it may be implemented on its own PWM
// channel, without complementary-output-mode
// thus DTH would have no effect. 

//  TCNT:     |  0   1    2    3  | 4    5    6
//            | R1   R0   G0   R5   R4   R3   R2
//  Red:      |____ ____ ____ ____ ____ ____ ____
//  OC1D   D0>/ D1>/ D2>/ C2>\ C3>\ C4>\ C5>\ C6...
//             ¯¯¯¯ ¯¯¯¯      ¯¯¯¯ ¯¯¯¯ ¯¯¯¯ ¯¯¯¯
uint8_t calcRedOld(uint8_t DTH, uint8_t OCRR, uint8_t *green)
{
#if(!defined(TO_GIMP_PALETTE) || !TO_GIMP_PALETTE)
#ifndef NOLVDSMAIN
	printf("r");
#endif
#endif
	uint8_t rawBits = dthBits(DTH, OCRR);
	uint8_t red = 0;

	writebit(5, red, getbit(3, rawBits));
	writebit(4, red, getbit(4, rawBits));
	writebit(3, red, getbit(5, rawBits));
	writebit(2, red, getbit(6, rawBits));

	//These shouldn't be relevent... but it shouldn't hurt...
	writebit(1, red, getbit(0, rawBits));
	writebit(0, red, getbit(1, rawBits));

	uint8_t greenTemp = 0;
	writebit(0, greenTemp, getbit(2, rawBits));

	if(invertRed)
	{
		*green |= (~greenTemp)&0x01;
		return (~red)&bin(0,0,1,1,1,1,1,1);
	}
	else
	{
		*green |= greenTemp;
		return red;
	}
}


//calc___New's are for the new color-scheme I've been considering...
// (actually, using the old timing-scheme, which was replaced when Green
//  and Red were implemented, in order to make the color-spectrum more
//  uniform)
// The benefit of the new color-scheme (should it be reimplemented)
// is a much broader range of colors... several color palettes are
// apparently possible. Though, there may be palettes which don't have
// key colors, such as black and white, or may have a strong preference
// toward shades of a particular color... (great for gradients, not so
// great for general-purpose)
// There're several ways to change the pallete, such as inverting the
// LVDS outputs, etc. So far none of this is implemented.


//  TCNT:     |  0   1    2  | 3    4    5    6
//              /H   B5   B4   B3   B2   DE   /V
uint8_t calcBlueNew(uint8_t DTH, uint8_t OCRB)
{
	uint8_t rawBits = dthBits(DTH, OCRB);

	uint8_t rawBit;
	uint8_t blue = 0;

	//Apparently worked...
	// I find the writebit method a little less confusing
	// though it is highly inefficient, I'm sure
//	for(rawBit=1; rawBit<5; rawBit++)
//		blue |= ((rawBits>>rawBit)&0x01)<<(6-rawBit);
	writebit(5, blue, getbit(1, rawBits));
	writebit(4, blue, getbit(2, rawBits));
	writebit(3, blue, getbit(3, rawBits));
	writebit(2, blue, getbit(4, rawBits));


	//invertBlue may not be possible, due to its effect on timing...
	// it hasn't been explored.
	if(invertBlue)
		return (~blue)&bin(0,0,1,1,1,1,0,0); //0x3f;
	else
		return blue;
}


//  TCNT:     |  0   1    2  | 3    4    5    6
//              R4   R3   R2   R1   R0   G0   R5
uint8_t calcRedNew(uint8_t dtl, uint8_t ocr, uint8_t* green)
{
	uint8_t rawBits = dtlBits(dtl, ocr);

	uint8_t rawBit;
	uint8_t red = 0;

//	for(rawBit=0; rawBit<=4; rawBit++)
//		red |= ((rawBits>>rawBit)&0x01)<<(4-rawBit);

//	red |= (rawBits&0x40)>>1;

	writebit(5, red, getbit(6, rawBits));
	writebit(4, red, getbit(0, rawBits));
	writebit(3, red, getbit(1, rawBits));
	writebit(2, red, getbit(2, rawBits));
	writebit(1, red, getbit(3, rawBits));
	writebit(0, red, getbit(4, rawBits));


	uint8_t greenTemp = 0;

	writebit(0, greenTemp, getbit(5, rawBits));

	//*green |= (rawBits&0x20)>>5;
	if(invertRed)
	{
		*green |= (~greenTemp)&0x01;
		//*green |= ((~rawBits)&0x20)>>5;
		return (~red)&0x3f;
	}
	else
	{
		*green |= greenTemp;
		//*green |= (rawBits&0x20)>>5;
		return red;
	}
}


//  TCNT:     |  0   1    2  | 3    4    5    6
//              G5   G4   G3   G2   G1   B1   B0

uint8_t calcGreenNew(uint8_t dth, uint8_t ocr, uint8_t* blue)
{
	uint8_t rawBits = dthBits(dth, ocr);

	uint8_t rawBit;
	uint8_t green = 0;

//	for(rawBit=0; rawBit<=4; rawBit++)
//		green |= ((rawBits>>rawBit)&0x01)<<(5-rawBit);
	writebit(5, green, getbit(0, rawBits));
	writebit(4, green, getbit(1, rawBits));
	writebit(3, green, getbit(2, rawBits));
	writebit(2, green, getbit(3, rawBits));
	writebit(1, green, getbit(4, rawBits));

	uint8_t blueTemp = 0;
	writebit(1, blueTemp, getbit(5, rawBits));
	writebit(0, blueTemp, getbit(6, rawBits));

	//*blue |= (rawBits&0x20)>>4;
	//*blue |= (rawBits&0x40)>>6;

	if(invertGreen)
	{
		*blue |= (~blueTemp)&0x03;
		//*blue |= ((~rawBits)&0x20)>>4;
		//*blue |= ((~rawBits)&0x40)>>6;
		return (~green)&bin(0,0,1,1,1,1,1,0);//0x3f;
	}
	else
	{
		//*blue |= (rawBits&0x20)>>4;
		//*blue |= (rawBits&0x40)>>6;
		*blue |= blueTemp;
		return green;
	}
}

typedef struct __BLAH_COLOR_THING_FOR_CPP__
{
	uint8_t red;
	uint8_t green;
	uint8_t blue;

	uint8_t dth;
	uint8_t dtl;
	uint8_t ocrr;
	uint8_t ocrg;
	uint8_t ocrb;

	uint8_t duplicate;	//True if an identical color was found elsewhere
} colorInfo_t;

//uint8_t color[8][8][8][8][3];

colorInfo_t colorList[32000];
uint16_t colorCount=0;
//uint8_t lColor[1000][3];


#ifdef NOLVDSMAIN
int initLColor(void)
{
#else
int main(void)
{
#if(!defined(TO_GIMP_PALETTE) || !TO_GIMP_PALETTE)
	printf("\n\nTCNT: 0 1 2 3 4 5 6       0 1 2 3 4 5 6       0 1 2 3 4 5 6"
			   "\n      -------------       -------------       -------------"
			   "\n   B: 3 2 x x x 5 4    G: 2 1 i o 5 4 3    R: 1 0 o 5 4 3 2"
				"\n      -------------       -------------       -------------"
				"\n");
#else
	printf("GIMP Palette\n"
			 "Name: LCDdirectLVDS_48c\n"
			 "Columns: 8\n"
			 "#\n");
	//Or it was just a mistype on my part...
	//		 "  0   0   0 NotAnOption\n");
			//Apparently Gimp throws black in even if it's not in the table
#endif

#endif
//	uint8_t DTH, OCRB;


//	for(OCRB=0; OCRB<7; OCRB++)
//	{
//		for(DTH=0; DTH<7; DTH++)
//			printf("DTH:%"PRIu8" OCRB:%"PRIu8" Green:%"PRIu8"\n", DTH, OCRB,
//					calcGreenNew(DTH, OCRB));
		//			calcBlueNew(DTH, OCRB));// & 0x1e);
		//Repurposing DTH as DTL...
		//for(DTH=0; DTH<7; DTH++)
		//	printf("DTL:%"PRIu8" OCR:%"PRIu8" Red:%"PRIu8"\n", DTH, OCRB,
		//			calcRedNew(DTH, OCRB));
//	}


	colorCount = 0;

	uint8_t DTH, OCRB, OCRRG, DTL;

//	uint8_t colorCount = 0;

	uint8_t blackFound = 0;

	//uint8_t color[8][8][8][8][3];

#ifndef OLDSTYLE
#define DTH_START HSYNC_USED
#define DTH_END	7

#define DTL_START 0
#define DTL_END	7

#define OCRB_START 0
#define OCRB_END   7

#define OCRRG_START	0
#define OCRRG_END		7
#else
#define DTH_START 0	//DTH is not used, since it affects the clock
#define DTH_END   0

#define DTL_START 0	//DTL is only used by green...
#define DTL_END   3

#define OCRB_START 4
#define OCRB_END   6

#define OCRRG_START  0	//OCRRG is actually only used for Red... OCR1D
#define OCRRG_END    7	//OCRG is exclusively 3 due to the clock
#endif

	for(DTH=DTH_START; DTH<=DTH_END; DTH++)
	{

		for(OCRB=OCRB_START; OCRB<=OCRB_END; OCRB++)
		{

			for(DTL=DTL_START; DTL<=DTL_END; DTL++)
			{
			
				for(OCRRG=OCRRG_START; OCRRG<=OCRRG_END; OCRRG++)
				{
					uint8_t green=0, blue=0, red=0; 
#ifndef OLDSTYLE
					blue = calcBlueNew(DTH, OCRB);
					green = calcGreenNew(DTH, OCRRG, &blue);
					red = calcRedNew(DTL, OCRRG, &green);
#else

#ifdef FOUR_COLOR_LIMITED
					switch(OCRRG)
					{
						case 1:
							OCRRG = 3;
							break;
						case 5:
							OCRRG = 6;
							break;
						default:
							break;
					}
#endif
					blue = calcBlueOld(DTH, OCRB);
					green = calcGreenOld(DTL, 3, &blue);
					red = calcRedOld(DTH, OCRRG, &green);

#warning "struct values, below, are not written properly... ocrr, ocrg..."
#endif

					//Check if it's already been found...
					uint16_t testNum;
					uint16_t colorDistance = COLORMATCHRANGE+1;

					for(testNum = 0; 
						(testNum < colorCount) && 
						(colorDistance > COLORMATCHRANGE); 
						testNum++)
					{
						colorDistance =
							abs(colorList[testNum].red - red) +
							abs(colorList[testNum].green - green) +
							abs(colorList[testNum].blue - blue);
					}

					if((colorDistance>COLORMATCHRANGE) || (keepDups))
					{
						colorInfo_t *c;
						c = &(colorList[colorCount]);

						c->red = red;
						c->green = green;
						c->blue = blue;
					
						c->dth = DTH;
						c->dtl = DTL;
						c->ocrr = OCRRG;
						c->ocrg = OCRRG;
						c->ocrb = OCRB;

						if(colorDistance<=COLORMATCHRANGE)
							c->duplicate = TRUE;
						else
							c->duplicate = FALSE;

						//colorCount++;

#if(!defined(TO_GIMP_PALETTE) || !TO_GIMP_PALETTE)
#ifndef NOLVDSMAIN
						printf("%3"PRIu16": %3"PRIu8",%3"PRIu8",%3"PRIu8"\n", 
							colorCount, red, green, blue);
#endif
#else				
						printf("%3"PRIu8" %3"PRIu8" %3"PRIu8
								" r%"PRIu8"g%"PRIu8"b%"PRIu8"\n",
							(uint8_t)(((uint16_t)red*255)/63), 
							(uint8_t)(((uint16_t)green*255)/63), 
							(uint8_t)(((uint16_t)blue*255)/63),

							(uint8_t)(((uint16_t)red*3)/63), 
							(uint8_t)(((uint16_t)green*3)/63), 
							(uint8_t)(((uint16_t)blue*3)/63)
							);
#endif
						colorCount++;
					}
#if(!defined(TO_GIMP_PALETTE) || !TO_GIMP_PALETTE)
#ifndef NOLVDSMAIN
					else
						printf(" Already Matched.\n");
#endif
#endif
				}
			}
		}
	}

	
}

#ifdef __cplusplus
}
#endif
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
 * /Users/meh/_avrProjects/LCDdirectLVDS/93-checkingProcessAgain/_tools/lvdsColorExperiments.c
 *
 *    (Wow, that's a lot longer than I'd hoped).
 *
 *    Enjoy!
 */

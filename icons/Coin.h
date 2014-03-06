#include "iconPacking.h"

#define COINR0 ROWPACK(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0)
#define COINR1 ROWPACK(0,0,0,0,0,0,0,2,2,0,0,0,0,0,0,0)
#define COINR2 ROWPACK(0,0,0,0,0,0,2,2,2,2,0,0,0,0,0,0)
#define COINR3 ROWPACK(0,0,0,0,0,2,2,2,2,2,2,0,0,0,0,0)
#define COINR4 ROWPACK(0,0,0,0,0,2,2,3,1,2,2,0,0,0,0,0)
#define COINR5 ROWPACK(0,0,0,0,2,2,3,2,2,1,2,2,0,0,0,0)
#define COINR6 ROWPACK(0,0,0,0,2,2,3,2,2,1,2,2,0,0,0,0)
#define COINR7 ROWPACK(0,0,0,0,2,2,3,2,2,1,2,2,0,0,0,0)
#define COINR8 ROWPACK(0,0,0,0,2,2,3,2,2,1,2,2,0,0,0,0)
#define COINR9 ROWPACK(0,0,0,0,2,2,3,2,2,1,2,2,0,0,0,0)
#define COINRA ROWPACK(0,0,0,0,2,2,3,2,2,1,2,2,0,0,0,0)
#define COINRB ROWPACK(0,0,0,0,0,2,2,3,1,2,2,0,0,0,0,0)
#define COINRC ROWPACK(0,0,0,0,0,2,2,2,2,2,2,0,0,0,0,0)
#define COINRD ROWPACK(0,0,0,0,0,0,2,2,2,2,0,0,0,0,0,0)
#define COINRE ROWPACK(0,0,0,0,0,0,0,2,2,0,0,0,0,0,0,0)
#define COINRF ROWPACK(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0)


#define COIN1R0 ROWPACK(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0)
#define COIN1R1 ROWPACK(0,0,0,0,0,0,0,2,1,0,0,0,0,0,0,0)
#define COIN1R2 ROWPACK(0,0,0,0,0,0,0,2,1,0,0,0,0,0,0,0)
#define COIN1R3 ROWPACK(0,0,0,0,0,0,2,1,1,1,0,0,0,0,0,0)
#define COIN1R4 ROWPACK(0,0,0,0,0,0,2,1,1,1,0,0,0,0,0,0)
#define COIN1R5 ROWPACK(0,0,0,0,0,0,2,1,1,1,0,0,0,0,0,0)
#define COIN1R6 ROWPACK(0,0,0,0,0,0,2,1,1,1,0,0,0,0,0,0)
#define COIN1R7 ROWPACK(0,0,0,0,0,0,3,1,1,1,0,0,0,0,0,0)
#define COIN1R8 ROWPACK(0,0,0,0,0,0,3,1,1,1,0,0,0,0,0,0)
#define COIN1R9 ROWPACK(0,0,0,0,0,0,2,1,1,1,0,0,0,0,0,0)
#define COIN1RA ROWPACK(0,0,0,0,0,0,2,1,1,1,0,0,0,0,0,0)
#define COIN1RB ROWPACK(0,0,0,0,0,0,2,1,1,1,0,0,0,0,0,0)
#define COIN1RC ROWPACK(0,0,0,0,0,0,2,1,1,1,0,0,0,0,0,0)
#define COIN1RD ROWPACK(0,0,0,0,0,0,0,2,1,0,0,0,0,0,0,0)
#define COIN1RE ROWPACK(0,0,0,0,0,0,0,2,1,0,0,0,0,0,0,0)
#define COIN1RF ROWPACK(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0)


#define COIN2R0 ROWPACK(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0)
#define COIN2R1 ROWPACK(0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0)
#define COIN2R2 ROWPACK(0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0)
#define COIN2R3 ROWPACK(0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0)
#define COIN2R4 ROWPACK(0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0)
#define COIN2R5 ROWPACK(0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0)
#define COIN2R6 ROWPACK(0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0)
#define COIN2R7 ROWPACK(0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0)
#define COIN2R8 ROWPACK(0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0)
#define COIN2R9 ROWPACK(0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0)
#define COIN2RA ROWPACK(0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0)
#define COIN2RB ROWPACK(0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0)
#define COIN2RC ROWPACK(0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0)
#define COIN2RD ROWPACK(0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0)
#define COIN2RE ROWPACK(0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0)
#define COIN2RF ROWPACK(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0)


//It looks to me like this is a direct flip of COIN1...
#define COIN3R0 ROWPACK(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0)
#define COIN3R1 ROWPACK(0,0,0,0,0,0,0,3,1,0,0,0,0,0,0,0)
#define COIN3R2 ROWPACK(0,0,0,0,0,0,0,3,1,0,0,0,0,0,0,0)
#define COIN3R3 ROWPACK(0,0,0,0,0,0,3,3,3,1,0,0,0,0,0,0)
#define COIN3R4 ROWPACK(0,0,0,0,0,0,3,3,3,1,0,0,0,0,0,0)
#define COIN3R5 ROWPACK(0,0,0,0,0,0,3,3,3,1,0,0,0,0,0,0)
#define COIN3R6 ROWPACK(0,0,0,0,0,0,3,3,3,1,0,0,0,0,0,0)
#define COIN3R7 ROWPACK(0,0,0,0,0,0,3,3,3,1,0,0,0,0,0,0)
#define COIN3R8 ROWPACK(0,0,0,0,0,0,3,3,3,1,0,0,0,0,0,0)
#define COIN3R9 ROWPACK(0,0,0,0,0,0,3,3,3,1,0,0,0,0,0,0)
#define COIN3RA ROWPACK(0,0,0,0,0,0,3,3,3,1,0,0,0,0,0,0)
#define COIN3RB ROWPACK(0,0,0,0,0,0,3,3,3,1,0,0,0,0,0,0)
#define COIN3RC ROWPACK(0,0,0,0,0,0,3,3,3,1,0,0,0,0,0,0)
#define COIN3RD ROWPACK(0,0,0,0,0,0,0,3,1,0,0,0,0,0,0,0)
#define COIN3RE ROWPACK(0,0,0,0,0,0,0,3,1,0,0,0,0,0,0,0)
#define COIN3RF ROWPACK(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0)

//37 (sky), 7, 47, 2

static uint8_t pgm_imageCOIN[ICON_PACKED_BYTES] PROGMEM =
   IMAGE_INIT(COIN);
   
static uint8_t pgm_imageCOIN1[ICON_PACKED_BYTES] PROGMEM =
	IMAGE_INIT(COIN1);

static uint8_t pgm_imageCOIN2[ICON_PACKED_BYTES] PROGMEM =
	IMAGE_INIT(COIN2);

static uint8_t pgm_imageCOIN3[ICON_PACKED_BYTES] PROGMEM =
	IMAGE_INIT(COIN3);

#define pgm_maskCOIN	NULL
//static uint8_t * pgm_maskCOIN; //[ICON_MASK_BYTES]; // PROGMEM =
//  MASK_INIT(GETNAMED(QUESTION));


#define NUMPALETTES_COIN 1

//gimpPixelValToLColor should probably be taken into account.
static uint8_t pgm_paletteCOIN[4*NUMPALETTES_COIN] PROGMEM =
 //  { 37, 7, 47, 2};
	{ 37, 2, 7, 47 };	//see Coin1.h for notes...
// 0 -> 0
// 1 -> 2
// 2 -> 3
// 3 -> 1

static sprite_t spriteCOIN =
      {pgm_imageCOIN, pgm_maskCOIN, pgm_paletteCOIN, NUMPALETTES_COIN};


uint8_t getRawPixelValCOIN(uint8_t spritePhase, uint8_t row, uint8_t col)
{
	uint8_t * p_image;

	//Is this better accomplished as a switch-statement or with math...?
	switch(spritePhase%8)
	{
		case 0:
		case 1:
//		case 2:
			p_image = pgm_imageCOIN;
			break;
		case 2:
		case 3:
			p_image = pgm_imageCOIN1;
			break;
		case 4:
		case 5:
			p_image = pgm_imageCOIN2;
			break;
		case 6:
		case 7:
		default:
			p_image = pgm_imageCOIN3;
			break;
	}	

   return
      (((pgm_read_byte((uint8_t *)(&(p_image[ \
               (row)*PACKED_BYTES_PER_ROW + (col)/PIXELS_PER_PACKAGE])))\
        )>>((col)%PIXELS_PER_PACKAGE)*(PACKED_BITS_PER_PIXEL))&0x03);
}  


#ifndef __1UP_H__
#define __1UP_H__

#include "iconPacking.h"




#define SHROOMR0 ROWPACK(0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0)
#define SHROOMR1 ROWPACK(0,0,0,0,0,1,1,1,1,2,2,0,0,0,0,0)
#define SHROOMR2 ROWPACK(0,0,0,0,1,1,1,1,2,2,2,2,0,0,0,0)
#define SHROOMR3 ROWPACK(0,0,0,1,1,1,1,1,2,2,2,2,2,0,0,0)
#define SHROOMR4 ROWPACK(0,0,1,1,1,1,1,1,1,2,2,2,1,1,0,0)
#define SHROOMR5 ROWPACK(0,1,1,2,2,2,1,1,1,1,1,1,1,1,1,0)
#define SHROOMR6 ROWPACK(0,1,2,2,2,2,2,1,1,1,1,1,1,1,1,0)
#define SHROOMR7 ROWPACK(1,1,2,2,2,2,2,1,1,1,1,1,2,2,1,1)
#define SHROOMR8 ROWPACK(1,1,2,2,2,2,2,1,1,1,1,1,2,2,2,1)
#define SHROOMR9 ROWPACK(1,1,1,2,2,2,1,1,1,1,1,1,1,2,2,1)
#define SHROOMRA ROWPACK(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1)
#define SHROOMRB ROWPACK(0,1,2,2,2,3,3,3,3,3,3,2,2,2,1,0)
#define SHROOMRC ROWPACK(0,0,0,0,3,3,3,3,3,3,3,3,0,0,0,0)
#define SHROOMRD ROWPACK(0,0,0,0,3,3,3,3,3,3,1,3,0,0,0,0)
#define SHROOMRE ROWPACK(0,0,0,0,3,3,3,3,3,3,1,3,0,0,0,0)
#define SHROOMRF ROWPACK(0,0,0,0,0,3,3,3,3,1,3,0,0,0,0,0)

static uint8_t pgm_imageSHROOM[ICON_PACKED_BYTES] PROGMEM = 
	IMAGE_INIT(SHROOM);



#define pgm_maskSHROOM	NULL
//static uint8_t pgm_maskSHROOM[ICON_MASK_BYTES]; // PROGMEM =
//	MASK_INIT(SHROOM);


#define NUMPALETTES_1UP	1

//1up Mapping: 0->37 (sky/mask) 1->7, 2->4 3->47
//gimpPixelValToLColor should probably be taken into account.
static uint8_t pgm_palette1UP[4*NUMPALETTES_1UP] PROGMEM = 
	{ 37, 7, 4, 47 };

static sprite_t sprite1UP = 
		{ pgm_imageSHROOM, pgm_maskSHROOM, pgm_palette1UP, NUMPALETTES_1UP};

#endif


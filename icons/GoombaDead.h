#include "iconPacking.h"

#define DEADGOOMBAR0 ROWPACK(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0)
#define DEADGOOMBAR1 ROWPACK(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0)
#define DEADGOOMBAR2 ROWPACK(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0)
#define DEADGOOMBAR3 ROWPACK(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0)
#define DEADGOOMBAR4 ROWPACK(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0)
#define DEADGOOMBAR5 ROWPACK(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0)
#define DEADGOOMBAR6 ROWPACK(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0)
#define DEADGOOMBAR7 ROWPACK(0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0)
#define DEADGOOMBAR8 ROWPACK(0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0)
#define DEADGOOMBAR9 ROWPACK(0,1,1,3,3,3,1,1,1,1,3,3,3,1,1,0)
#define DEADGOOMBARA ROWPACK(1,1,2,2,2,2,3,3,3,3,2,2,2,2,1,1)
#define DEADGOOMBARB ROWPACK(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1)
#define DEADGOOMBARC ROWPACK(0,0,0,2,2,2,2,2,2,2,2,2,2,0,0,0)
#define DEADGOOMBARD ROWPACK(0,0,0,2,2,2,2,2,2,2,2,2,2,0,0,0)
#define DEADGOOMBARE ROWPACK(0,0,3,3,3,3,0,0,0,0,3,3,3,3,3,0)
#define DEADGOOMBARF ROWPACK(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0)

static uint8_t pgm_imageDEADGOOMBA[ICON_PACKED_BYTES] PROGMEM =
   IMAGE_INIT(DEADGOOMBA);


#define pgm_maskDEADGOOMBA NULL
//static uint8_t * pgm_maskDEADGOOMBA; //[ICON_MASK_BYTES]; // PROGMEM =
//  MASK_INIT(GETNAMED(QUESTION));


#define NUMPALETTES_DEADGOOMBA 1

//gimpPixelValToLColor should probably be taken into account.
static uint8_t pgm_paletteDEADGOOMBA[4*NUMPALETTES_DEADGOOMBA] PROGMEM =
{
	//AGAIN: This is NOT a safe method, normally... (rgb8() here)
	37,
	//rgb8(141,157,255),
	rgb8(206, 28, 60),
	rgb8(255,157,190),
	rgb8(  0, 24, 60)
};

static sprite_t spriteDEADGOOMBA =
      {pgm_imageDEADGOOMBA, pgm_maskDEADGOOMBA, pgm_paletteDEADGOOMBA, 
			NUMPALETTES_DEADGOOMBA};


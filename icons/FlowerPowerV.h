#include "iconPacking.h"

#define FLOWERR0 ROWPACK(0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0)
#define FLOWERR1 ROWPACK(0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0)
#define FLOWERR2 ROWPACK(0,1,1,1,2,2,2,2,2,2,2,2,1,1,1,0)
#define FLOWERR3 ROWPACK(1,1,2,2,2,3,3,3,3,3,3,2,2,2,1,1)
#define FLOWERR4 ROWPACK(1,1,2,2,2,3,3,3,3,3,3,2,2,2,1,1)
#define FLOWERR5 ROWPACK(0,1,1,1,2,2,2,2,2,2,2,2,1,1,1,0)
#define FLOWERR6 ROWPACK(0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0)
#define FLOWERR7 ROWPACK(0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0)

#define FLOWERR8 ROWPACK(0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0)
#define FLOWERR9 ROWPACK(1,1,1,0,0,0,0,1,1,0,0,0,0,1,1,1)
#define FLOWERRA ROWPACK(0,1,1,1,0,0,0,1,1,0,0,0,1,1,1,0)
#define FLOWERRB ROWPACK(0,1,1,1,1,0,0,1,1,0,0,1,1,1,1,0)
#define FLOWERRC ROWPACK(0,0,1,1,1,1,0,1,1,0,1,1,1,1,0,0)
#define FLOWERRD ROWPACK(0,0,1,1,1,1,0,1,1,0,1,1,1,1,0,0)
#define FLOWERRE ROWPACK(0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0)
#define FLOWERRF ROWPACK(0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0)


//37, 47, 7, 4 (37, 4, unused, unused -> stem)
//37 (sky), 23 (outter Ring), 2 (inner ring), 0 middle
//37, 7, 5, 2

static uint8_t pgm_imageFLOWER[ICON_PACKED_BYTES] PROGMEM =
   IMAGE_INIT(FLOWER);
   

//Original plan was to use a mask, then that fourth color could be in the
// palette... but the new method is to switch palettes mid-drawing.
// so no mask necessary
#define pgm_maskFLOWER NULL
//static uint8_t pgm_maskFLOWER[ICON_MASK_BYTES]; // PROGMEM =
//  MASK_INIT(GETNAMED(FLOWER));


//Flower has two separate palettes during each draw...
// The first palette is, therefore, the one that remains the same for each
// other palette...
#define NUMPALETTES_FLOWER 4

#define FLOWER_PALETTE1_ROW	8

//gimpPixelValToLColor should probably be taken into account.
static uint8_t pgm_paletteFLOWER[4*NUMPALETTES_FLOWER] PROGMEM =
   { 
	  37,  4, 0, 0,
	  37, 47, 7, 4,
     37, 23, 2, 0,
     37,  7, 5, 2 };

static sprite_t spriteFLOWER =
      { pgm_imageFLOWER, pgm_maskFLOWER, pgm_paletteFLOWER, NUMPALETTES_FLOWER};


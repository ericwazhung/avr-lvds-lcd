#include "iconPacking.h"

#define COINR0 ROWPACK(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0)
#define COINR1 ROWPACK(0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0)
#define COINR2 ROWPACK(0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0)
#define COINR3 ROWPACK(0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0)
#define COINR4 ROWPACK(0,0,0,0,0,1,1,2,3,1,1,0,0,0,0,0)
#define COINR5 ROWPACK(0,0,0,0,1,1,2,1,1,3,1,1,0,0,0,0)
#define COINR6 ROWPACK(0,0,0,0,1,1,2,1,1,3,1,1,0,0,0,0)
#define COINR7 ROWPACK(0,0,0,0,1,1,2,1,1,3,1,1,0,0,0,0)
#define COINR8 ROWPACK(0,0,0,0,1,1,2,1,1,3,1,1,0,0,0,0)
#define COINR9 ROWPACK(0,0,0,0,1,1,2,1,1,3,1,1,0,0,0,0)
#define COINRA ROWPACK(0,0,0,0,1,1,2,1,1,3,1,1,0,0,0,0)
#define COINRB ROWPACK(0,0,0,0,0,1,1,2,3,1,1,0,0,0,0,0)
#define COINRC ROWPACK(0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0)
#define COINRD ROWPACK(0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0)
#define COINRE ROWPACK(0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0)
#define COINRF ROWPACK(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0)

//37 (sky), 7, 47, 2

static uint8_t pgm_imageCOIN[ICON_PACKED_BYTES] PROGMEM =
   IMAGE_INIT(COIN);
   

#define pgm_maskCOIN	NULL
//static uint8_t * pgm_maskCOIN; //[ICON_MASK_BYTES]; // PROGMEM =
//  MASK_INIT(GETNAMED(QUESTION));


#define NUMPALETTES_COIN 1

//gimpPixelValToLColor should probably be taken into account.
static uint8_t pgm_paletteCOIN[4*NUMPALETTES_COIN] PROGMEM =
   { 37, 7, 47, 2};

static sprite_t spriteCOIN =
      {pgm_imageCOIN, pgm_maskCOIN, pgm_paletteCOIN, NUMPALETTES_COIN};

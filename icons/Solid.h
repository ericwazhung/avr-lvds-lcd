#include "iconPacking.h"

#define SOLIDR0 ROWPACK(0,3,3,3,3,3,3,3,3,3,3,3,3,3,3,0)
#define SOLIDR1 ROWPACK(3,1,1,1,1,1,1,1,1,1,1,1,1,1,1,3)
#define SOLIDR2 ROWPACK(3,1,3,1,1,1,1,1,1,1,1,1,1,3,1,3)
#define SOLIDR3 ROWPACK(3,1,1,1,1,1,1,1,1,1,1,1,1,1,1,3)
#define SOLIDR4 ROWPACK(3,1,1,1,1,1,1,1,1,1,1,1,1,1,1,3)
#define SOLIDR5 ROWPACK(3,1,1,1,1,1,1,1,1,1,1,1,1,1,1,3)
#define SOLIDR6 ROWPACK(3,1,1,1,1,1,1,1,1,1,1,1,1,1,1,3)
#define SOLIDR7 ROWPACK(3,1,1,1,1,1,1,1,1,1,1,1,1,1,1,3)
#define SOLIDR8 ROWPACK(3,1,1,1,1,1,1,1,1,1,1,1,1,1,1,3)
#define SOLIDR9 ROWPACK(3,1,1,1,1,1,1,1,1,1,1,1,1,1,1,3)
#define SOLIDRA ROWPACK(3,1,1,1,1,1,1,1,1,1,1,1,1,1,1,3)
#define SOLIDRB ROWPACK(3,1,1,1,1,1,1,1,1,1,1,1,1,1,1,3)
#define SOLIDRC ROWPACK(3,1,1,1,1,1,1,1,1,1,1,1,1,1,1,3)
#define SOLIDRD ROWPACK(3,1,3,1,1,1,1,1,1,1,1,1,1,3,1,3)
#define SOLIDRE ROWPACK(3,1,1,1,1,1,1,1,1,1,1,1,1,1,1,3)
#define SOLIDRF ROWPACK(0,3,3,3,3,3,3,3,3,3,3,3,3,3,3,0)

//37 (sky), 2, not-used, 0

static uint8_t pgm_imageSOLID[ICON_PACKED_BYTES] PROGMEM =
   IMAGE_INIT(SOLID);


#define pgm_maskSOLID	NULL
//static uint8_t pgm_maskSOLID[ICON_MASK_BYTES]; // PROGMEM =
//  MASK_INIT(GETNAMED(QUESTION));


#define NUMPALETTES_SOLID 1

//gimpPixelValToLColor should probably be taken into account.
static uint8_t pgm_paletteSOLID[4*NUMPALETTES_SOLID] PROGMEM =
   { 37, 2, 0, 0}; 

static sprite_t spriteSOLID =
      {pgm_imageSOLID, pgm_maskSOLID, pgm_paletteSOLID, NUMPALETTES_SOLID};


#include "iconPacking.h"

#define STARR0 ROWPACK(0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0)
#define STARR1 ROWPACK(0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0)
#define STARR2 ROWPACK(0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0)
#define STARR3 ROWPACK(0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0)
#define STARR4 ROWPACK(0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0)
#define STARR5 ROWPACK(0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0)
#define STARR6 ROWPACK(0,1,1,1,1,1,2,1,1,2,1,1,1,1,1,0)
#define STARR7 ROWPACK(0,0,1,1,1,1,2,1,1,2,1,1,1,1,0,0)
#define STARR8 ROWPACK(0,0,0,1,1,1,2,1,1,2,1,1,1,0,0,0)
#define STARR9 ROWPACK(0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0)
#define STARRA ROWPACK(0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0)
#define STARRB ROWPACK(0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0)
#define STARRC ROWPACK(0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0)
#define STARRD ROWPACK(0,0,0,1,1,1,1,0,0,1,1,1,1,0,0,0)
#define STARRE ROWPACK(0,0,1,1,1,0,0,0,0,0,0,1,1,1,0,0)
#define STARRF ROWPACK(0,0,1,1,0,0,0,0,0,0,0,0,1,1,0,0)



//37, 7, 2, not used
//37, 2, 0, not used

static uint8_t pgm_imageSTAR[ICON_PACKED_BYTES] PROGMEM =
   IMAGE_INIT(STAR);


#define pgm_maskSTAR	NULL
//static uint8_t pgm_maskSTAR[ICON_MASK_BYTES]; // PROGMEM =
   // MASK_INIT(QUESTION);


#define NUMPALETTES_STAR 2

//gimpPixelValToLColor should probably be taken into account.
static uint8_t pgm_paletteSTAR[4*NUMPALETTES_STAR] PROGMEM =
   { 37, 7, 2, 0,
     37, 2, 0, 0 };

static sprite_t spriteSTAR =
      { pgm_imageSTAR, pgm_maskSTAR, pgm_paletteSTAR, NUMPALETTES_STAR};


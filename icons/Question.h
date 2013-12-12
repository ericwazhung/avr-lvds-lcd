#include "iconPacking.h"


#define QUESTIONR0 ROWPACK(0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0)
#define QUESTIONR1 ROWPACK(1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,3)
#define QUESTIONR2 ROWPACK(1,2,3,2,2,2,2,2,2,2,2,2,2,3,2,3)
#define QUESTIONR3 ROWPACK(1,2,2,2,2,1,1,1,1,1,2,2,2,2,2,3)
#define QUESTIONR4 ROWPACK(1,2,2,2,1,1,3,3,3,1,1,2,2,2,2,3)
#define QUESTIONR5 ROWPACK(1,2,2,2,1,1,3,2,2,1,1,3,2,2,2,3)
#define QUESTIONR6 ROWPACK(1,2,2,2,1,1,3,2,2,1,1,3,2,2,2,3)
#define QUESTIONR7 ROWPACK(1,2,2,2,2,3,3,2,1,1,1,3,2,2,2,3)
#define QUESTIONR8 ROWPACK(1,2,2,2,2,2,2,1,1,3,3,3,2,2,2,3)
#define QUESTIONR9 ROWPACK(1,2,2,2,2,2,2,1,1,3,2,2,2,2,2,3)
#define QUESTIONRA ROWPACK(1,2,2,2,2,2,2,2,3,3,2,2,2,2,2,3)
#define QUESTIONRB ROWPACK(1,2,2,2,2,2,2,1,1,2,2,2,2,2,2,3)
#define QUESTIONRC ROWPACK(1,2,2,2,2,2,2,1,1,3,2,2,2,2,2,3)
#define QUESTIONRD ROWPACK(1,2,3,2,2,2,2,2,3,3,2,2,2,3,2,3)
#define QUESTIONRE ROWPACK(1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,3)
#define QUESTIONRF ROWPACK(3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3)

// 37 (sky), 2 upper-left/Q, 7=inner, 0=Shadow/Screws 
//37, 2, 2, 0
//37, 2, 1, 0

static uint8_t pgm_imageQ[ICON_PACKED_BYTES] PROGMEM =
   IMAGE_INIT(QUESTION);


#define pgm_maskQ	NULL
//static uint8_t pgm_maskQ[ICON_MASK_BYTES]; // PROGMEM =
//	 MASK_INIT(GETNAMED(QUESTION));


#define NUMPALETTES_Q 3

//gimpPixelValToLColor should probably be taken into account.
static uint8_t pgm_paletteQ[4*NUMPALETTES_Q] PROGMEM =
   { 37, 2, 7, 0,   
	  37, 2, 2, 0,  
	  37, 2, 1, 0 };

static sprite_t spriteQ =
      { pgm_imageQ, pgm_maskQ, pgm_paletteQ, NUMPALETTES_Q};


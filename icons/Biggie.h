#include "iconPacking.h"

#include "1up.h" //has SHROOM in it...

#define NUMPALETTES_BIG 1

//1up Mapping: 0->37 (sky/mask) 1->7, 2->4 3->47

//0->37, 1->7, 2->2, 3->47
//gimpPixelValToLColor should probably be taken into account.
static uint8_t pgm_paletteBIG[4*NUMPALETTES_BIG] PROGMEM =
   { 37, 7, 2, 47 };

static sprite_t spriteBIG =
      { pgm_imageSHROOM, pgm_maskSHROOM, pgm_paletteBIG, NUMPALETTES_BIG};


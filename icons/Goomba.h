#include "iconPacking.h"

#define GOOMBAR0 ROWPACK(1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1)
#define GOOMBAR1 ROWPACK(1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1)
#define GOOMBAR2 ROWPACK(1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1)
#define GOOMBAR3 ROWPACK(1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1)
#define GOOMBAR4 ROWPACK(1,1,0,3,3,0,0,0,0,0,0,3,3,0,1,1)
#define GOOMBAR5 ROWPACK(1,0,0,0,2,3,0,0,0,0,3,2,0,0,0,1)
#define GOOMBAR6 ROWPACK(1,0,0,0,2,3,3,3,3,3,3,2,0,0,0,1)
#define GOOMBAR7 ROWPACK(0,0,0,0,2,3,2,0,0,2,3,2,0,0,0,0)
#define GOOMBAR8 ROWPACK(0,0,0,0,2,2,2,0,0,2,2,2,0,0,0,0)
#define GOOMBAR9 ROWPACK(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0)
#define GOOMBARA ROWPACK(1,0,0,0,0,2,2,2,2,2,2,0,0,0,0,1)
#define GOOMBARB ROWPACK(1,1,1,1,2,2,2,2,2,2,2,2,1,1,1,1)
#define GOOMBARC ROWPACK(1,1,1,1,2,2,2,2,2,2,2,2,3,3,1,1)
#define GOOMBARD ROWPACK(1,1,1,3,3,2,2,2,2,2,3,3,3,3,3,1)
#define GOOMBARE ROWPACK(1,1,1,3,3,3,2,2,2,3,3,3,3,3,3,1)
#define GOOMBARF ROWPACK(1,1,1,1,3,3,3,2,3,3,3,3,3,3,1,1)


static uint8_t pgm_imageGOOMBA[ICON_PACKED_BYTES] PROGMEM =
   IMAGE_INIT(GOOMBA);
   
   
#define pgm_maskGOOMBA NULL
//static uint8_t * pgm_maskGOOMBA; //[ICON_MASK_BYTES]; // PROGMEM =
//  MASK_INIT(GETNAMED(QUESTION));


#define NUMPALETTES_GOOMBA 1

//gimpPixelValToLColor should probably be taken into account.
static uint8_t pgm_paletteGOOMBA[4*NUMPALETTES_GOOMBA] PROGMEM =
{
	rgb8(206, 28, 60),
	rgb8(141,157,255),	//First encounter with rgb8 not working as expected
								//But I kinda like the badguy on a non-sky color
								// might leave this
								// ALSO forgot that these values don't take into
								// account that gimpColor is being modified...
								// (right?)
	rgb8(255,157,190),
	rgb8(  0, 24, 60)
		//Other colors look right, sos I'mma leave 'em
};

static sprite_t spriteGOOMBA =
      {pgm_imageGOOMBA, pgm_maskGOOMBA, pgm_paletteGOOMBA, NUMPALETTES_GOOMBA};


#include "iconPacking.h"



#define GOOMBAR0 ROWPACK(0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0)
#define GOOMBAR1 ROWPACK(0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0)
#define GOOMBAR2 ROWPACK(0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0)
#define GOOMBAR3 ROWPACK(0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0)
#define GOOMBAR4 ROWPACK(0,0,1,3,3,1,1,1,1,1,1,3,3,1,0,0)
#define GOOMBAR5 ROWPACK(0,1,1,1,2,3,1,1,1,1,3,2,1,1,1,0)
#define GOOMBAR6 ROWPACK(0,1,1,1,2,3,3,3,3,3,3,2,1,1,1,0)
#define GOOMBAR7 ROWPACK(1,1,1,1,2,3,2,1,1,2,3,2,1,1,1,1)
#define GOOMBAR8 ROWPACK(1,1,1,1,2,2,2,1,1,2,2,2,1,1,1,1)
#define GOOMBAR9 ROWPACK(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1)
#define GOOMBARA ROWPACK(0,1,1,1,1,2,2,2,2,2,2,1,1,1,1,0)
#define GOOMBARB ROWPACK(0,0,0,0,2,2,2,2,2,2,2,2,0,0,0,0)
#define GOOMBARC ROWPACK(0,0,0,0,2,2,2,2,2,2,2,2,3,3,0,0)
#define GOOMBARD ROWPACK(0,0,0,3,3,2,2,2,2,2,3,3,3,3,3,0)
#define GOOMBARE ROWPACK(0,0,0,3,3,3,2,2,2,3,3,3,3,3,3,0)
#define GOOMBARF ROWPACK(0,0,0,0,3,3,3,2,3,3,3,3,3,3,0,0)


static uint8_t pgm_imageGOOMBA[ICON_PACKED_BYTES] PROGMEM =
   IMAGE_INIT(GOOMBA);
   
   
#define pgm_maskGOOMBA NULL
//static uint8_t * pgm_maskGOOMBA; //[ICON_MASK_BYTES]; // PROGMEM =
//  MASK_INIT(GETNAMED(QUESTION));


#define NUMPALETTES_GOOMBA 6



#define GOOMBA_BODY	rgb8(206, 28, 60)
#define GOOMBA_BLACK	rgb8(  0, 24, 60)
#define GOOMBA_WHITE	rgb8(255,157,190)

#define rgb2(r,g,b) \
		(r | (g<<2) | (b<<4))
//gimpPixelValToLColor should probably be taken into account.
static uint8_t pgm_paletteGOOMBA[4*NUMPALETTES_GOOMBA] PROGMEM =
{
	//rgb8(141,157,255),	//First encounter with rgb8 not working as expected
								//But I kinda like the badguy on a non-sky color
								// might leave this
								// ALSO forgot that these values don't take into
								// account that gimpColor is being modified...
								// (right?)
	rgb2(0,1,0),
	GOOMBA_BODY, //rgb8(206, 28, 60),
	GOOMBA_WHITE, //rgb8(255,157,190),
	GOOMBA_BLACK, //rgb8(  0, 24, 60),
		//Other colors look right, sos I'mma leave 'em
	//Getting Dangerous
	rgb2(0,1,0), //0x0b, 					//yellow
	GOOMBA_BODY, //rgb8(206, 28, 60),
	GOOMBA_WHITE, //rgb8(255,157,190),
	GOOMBA_BLACK, //rgb8(  0, 24, 60),
	//Too Late
	rgb2(1,1,0), //0x0f,						//yellow-orange
	GOOMBA_BODY, //rgb8(206, 28, 60),
	GOOMBA_WHITE, //rgb8(255,157,190),
	GOOMBA_BLACK, //rgb8(  0, 24, 60)

	rgb2(3,1,0), //0x07,        		//orange
	GOOMBA_BODY, 
	GOOMBA_WHITE,
	GOOMBA_BLACK,

	rgb2(3,0,0), //0x03,        		//red
	rgb2(1,0,0), //GOOMBA_BODY, 
	rgb2(2,1,2), //GOOMBA_WHITE,
	GOOMBA_BLACK,

	0x00,        					//black
	0x20, //GOOMBA_BODY, 		//dark-blue
	0x01, //GOOMBA_WHITE,		//dark-brown
	0x20  //GOOMBA_BLACK,		//dark-blue

};

static sprite_t spriteGOOMBA =
      {pgm_imageGOOMBA, pgm_maskGOOMBA, pgm_paletteGOOMBA, NUMPALETTES_GOOMBA};


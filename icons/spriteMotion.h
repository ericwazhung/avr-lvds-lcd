/* mehPL:
 *    This is Open Source, but NOT GPL. I call it mehPL.
 *    I'm not too fond of long licenses at the top of the file.
 *    Please see the bottom.
 *    Enjoy!
 */




#ifndef __SPRITE_MOTION_H__
#define __SPRITE_MOTION_H__

#include "fb_question.h"
#include "iconPacking.h"

//motion can be either 0, 1, or 2 pixels
// 3 (DIR_TOGGLE) is a non-motion that merely tells the motion to change
// directions (for now, equivalent to a 0-motion)
#define DIR_TOGGLE   3
#define PACK_MOTION_BYTE(a,b,c,d) \
   ( ((a)&0x03) \
   | (((b)&0x03)<<2) \
   | (((c)&0x03)<<4) \
   | (((d)&0x03)<<6) )
#define MOTION_BYTES(motions) (((motions)+3)/4)


//a/o v83: Moving this to progmem...
//a/o v85: still hasn't been done...
// TODO: Move it to __flash
// would save 30Bytes of RAM
//Getting an inclusion-loop... moving this to iconPacking.h
//typedef struct _BLAH_MOTION_
//{
//   const uint8_t startPosition;
//   const uint8_t *motion;
//} motion_t;


typedef struct _BLAH_STATUS_
{
   //This'll have to be expanded for more motion-types (layer, etc)
   int8_t position[2];
   int8_t dir[2];
   uint8_t count;    //separate 'qCount' for each sprite... NYI
   const uint8_t *layer; //pointer to the first LAYER_BYTE in an array [1]
   const uint8_t *hFlip;   //pointer to the first FLIP_BYTE in an array [1]
   const __flash motion_t *motion; //pointer to the first motion_t in an array [2]
	const __flash sprite_t *sprite;
	uint8_t paletteNum; //Which palette should we draw with...?
} spriteState_t;


//spriteState_t selectedSpriteState; // =   { {0,0}, {1,1}, 0 };
//This is the "other" sprite being shown with the nowSprite...
// E.G. during "Reward" this is "SOLID"
// There aren't any other cases, currently.
// And if it's unused (e.g. during "Q" ...?)
//spriteState_t otherSpriteState; // =   { {0,0}, {1,1}, 0 };
//spriteState_t nextSpriteState; // =       { {0,0}, {1,1}, 0 };
//This is the sprite currently being handled...
// E.G. Q, or "Reward"
spriteState_t nowSpriteState;

//Camera isn't exactly a spriteState, but it uses many of the same
//variables...
spriteState_t cameraState;



//Flip is 1-bit per motion...
#define FLIP   1
#define NORM   0
//Not *certain* this should be reverseBin() vs bin(), but it should be
//right...
#define PACK_FLIP_BYTE(a,b,c,d,e,f,g,h) \
            reverseBin((a),(b),(c),(d),(e),(f),(g),(h))
#define FLIP_BYTES(motions)   ((motions+7)/8)

//a/o v83: Since Flips pack so nicely, it might be wise *not* to use
//progmem... doing-so here increased code-size by 10 bytes, and only
//decreased the RAM usage by 4...
// OTOH, I'm overflowing the stack.
// Also, it'll be more problematic when there's other Flip arrays
#define FLIP_IN_PROGMEM TRUE


#if(defined(FLIP_IN_PROGMEM) && FLIP_IN_PROGMEM)
#define FLIP_MEM  PROGMEM
#define readFlipByte pgm_read_byte
#else
#define FLIP_MEM  //leave it blank
#define readFlipByte *
#endif


#define GET_FLIP(pgm_array, motionIndex) \
   ( ((pgm_array) == NULL) ? NORM : \
   ((readFlipByte(&((pgm_array)[(motionIndex)/8])) >> ((motionIndex)%8)) & 0x01) )

#define NadaFlip NULL



//Layer is 1-bit per motion...
#define _BG 1
#define _FG 0
//Not *certain* this should be reverseBin() vs bin(), but it should be
//right...
#define PACK_LAYER_BYTE  PACK_FLIP_BYTE
#define LAYER_BYTES  FLIP_BYTES

//a/o v83: Since Flips pack so nicely, it might be wise *not* to use
//progmem... doing-so here increased code-size by 10 bytes, and only
//decreased the RAM usage by 4...
// OTOH, I'm overflowing the stack.
// Also, it'll be more problematic when there's other Flip arrays
#define LAYER_IN_PROGMEM   TRUE

#if(defined(LAYER_IN_PROGMEM) && LAYER_IN_PROGMEM)
#define LAYER_MEM PROGMEM
#define readLayerByte   pgm_read_byte
#else
#define LAYER_MEM //leave it blank
#define readLayerByte *
#endif

//This is roughly identical to GET_FLIP
// if NULL, it's always foreground...
#define GET_LAYER(pgm_array, motionIndex) \
   ( ((pgm_array) == NULL) ? _FG : \
   ((readFlipByte(&((pgm_array)[(motionIndex)/8])) >> ((motionIndex)%8)) & 0x01) )

#define NadaLayer NULL
#define TopLayer NULL


//Palette-Advance (PA) is 1-bit per advance...
#define PACK_PA_BYTE	PACK_FLIP_BYTE
#define PA_BYTES		FLIP_BYTES

#define GET_PALETTE_ADVANCE(pgm_array, motionIndex) \
	( ((pgm_array) == NULL) ? NORM : \
	((readFlipByte(&((pgm_array)[(motionIndex)/8])) >>((motionIndex)%8)) & 	0x01) )

#define NadaPaletteAdvance	NULL








#define MOTION_IN_PROGMEM  TRUE

#if(defined(MOTION_IN_PROGMEM) && MOTION_IN_PROGMEM)
#define MOTION_MEM   PROGMEM
#define readMotionByte  pgm_read_byte
#else
#define MOTION_MEM   //leave it blank
#define readMotionByte *
#endif



//Note that the motionIndex is that of the motion itself, NOT the byte it's
//stored in.
#define GET_MOTION(pgm_motionArray, motionIndex) \
   ( ((pgm_motionArray) == NULL) ? 0 : \
   (0x03&(readMotionByte(&(pgm_motionArray[(motionIndex)/4])) >> (2*((motionIndex)%4)))) )




const __flash motion_t NadaMotion[2] = { {0, NULL}, {0, NULL} };



/*
//This can also be used for repositioning the camera...
void fbQ_repositionSprite(spriteState_t *state);




//Draws a sprite on top of whatever's already there, at the position
//determined by its motionStuff... (repositionSprite() needs to be called
//prior)
//Eventually will return the rowNum for REFRESH_ON_CHANGE, etc...
uint8_t fbQ_overlaySprite(sprite_t *p_theSprite, spriteState_t *state);
*/




#endif

/* mehPL:
 *    I would love to believe in a world where licensing shouldn't be
 *    necessary; where people would respect others' work and wishes, 
 *    and give credit where it's due. 
 *    A world where those who find people's work useful would at least 
 *    send positive vibes--if not an email.
 *    A world where we wouldn't have to think about the potential
 *    legal-loopholes that others may take advantage of.
 *
 *    Until that world exists:
 *
 *    This software and associated hardware design is free to use,
 *    modify, and even redistribute, etc. with only a few exceptions
 *    I've thought-up as-yet (this list may be appended-to, hopefully it
 *    doesn't have to be):
 * 
 *    1) Please do not change/remove this licensing info.
 *    2) Please do not change/remove others' credit/licensing/copyright 
 *         info, where noted. 
 *    3) If you find yourself profiting from my work, please send me a
 *         beer, a trinket, or cash is always handy as well.
 *         (Please be considerate. E.G. if you've reposted my work on a
 *          revenue-making (ad-based) website, please think of the
 *          years and years of hard work that went into this!)
 *    4) If you *intend* to profit from my work, you must get my
 *         permission, first. 
 *    5) No permission is given for my work to be used in Military, NSA,
 *         or other creepy-ass purposes. No exceptions. And if there's 
 *         any question in your mind as to whether your project qualifies
 *         under this category, you must get my explicit permission.
 *
 *    The open-sourced project this originated from is ~98% the work of
 *    the original author, except where otherwise noted.
 *    That includes the "commonCode" and makefiles.
 *    Thanks, of course, should be given to those who worked on the tools
 *    I've used: avr-dude, avr-gcc, gnu-make, vim, usb-tiny, and 
 *    I'm certain many others. 
 *    And, as well, to the countless coders who've taken time to post
 *    solutions to issues I couldn't solve, all over the internets.
 *
 *
 *    I'd love to hear of how this is being used, suggestions for
 *    improvements, etc!
 *         
 *    The creator of the original code and original hardware can be
 *    contacted at:
 *
 *        EricWazHung At Gmail Dotcom
 *
 *    This code's origin (and latest versions) can be found at:
 *
 *        https://code.google.com/u/ericwazhung/
 *
 *    The site associated with the original open-sourced project is at:
 *
 *        https://sites.google.com/site/geekattempts/
 *
 *    If any of that ever changes, I will be sure to note it here, 
 *    and add a link at the pages above.
 *
 * This license added to the original file located at:
 * /Users/meh/_avrProjects/LCDdirectLVDS/90-reGitting/icons/spriteMotion.h
 *
 *    (Wow, that's a lot longer than I'd hoped).
 *
 *    Enjoy!
 */

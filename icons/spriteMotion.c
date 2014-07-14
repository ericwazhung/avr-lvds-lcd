/* mehPL:
 *    This is Open Source, but NOT GPL. I call it mehPL.
 *    I'm not too fond of long licenses at the top of the file.
 *    Please see the bottom.
 *    Enjoy!
 */






#ifndef __SPRITE_MOTION_C__
#define __SPRITE_MOTION_C__

//#include "../fb_question.h"
#include "spriteMotion.h"

#include "FlowerPowerV.h"	//Needed for flower-specific handling


//This is in ../fb_question.c
//It should probably be moved.
uint8_t getSpritePalette(const __flash sprite_t *p_thisSprite, uint8_t spritePhase,
		                                                 uint8_t spriteRow);


//This can also be used for repositioning the camera...
//THIS ASSUMES it's only called ONCE per count...
//Currently returns TRUE if a change has been detected
// (FOR REFRESH_ON_CHANGE)
uint8_t fbQ_repositionSprite(spriteState_t *state)
{
	//This isn't particularly-well-implemented
	// (for REFRESH_ON_CHANGE)
	// since, e.g., the camera could move along with the sprite...
	uint8_t changeDetected = FALSE;
   uint8_t axis;

   for(axis=0; axis<=1; axis++)
   {
  
	  /* Now handled in initSpriteStates (in fb_question.c)	
   if(state->count == 0)
   {
      //Need to have a starting-point...
      // Note that origin is lower-left, but nextSpritePosition is
      // upper-left... (change that, instead?)
      int8_t startPos = state->motion[axis].startPosition;
      
      if(axis == 0)
         startPos *= -1;

      state->position[axis] = startPos;
      state->dir[axis] = 1;
   }
	*/

   int8_t motion = GET_MOTION(state->motion[axis].motion, state->count);
   if(motion == DIR_TOGGLE)
   {
      state->dir[axis] = -state->dir[axis];
      motion = 0;
   }

   motion *= state->dir[axis];

   //Considering having LeafX be more like acceleration...?
   // e.g. motion: 1, 0, 0, DIR_TOGGLE -> motionX = 1 1 1 0(-)
   //(-= because we want +1 to shift right)
   if(axis == 0)
      motion *= (-1);

   state->position[axis] += motion;

	if(motion)
		changeDetected = TRUE;
   }

	//e.g. the camera...
	if(state->sprite != NULL)
	{
		uint8_t oldPaletteNum = state->paletteNum;

		uint8_t paletteAdvance = 
		  GET_PALETTE_ADVANCE(state->sprite->p_paletteAdvance, state->count);

		state->paletteNum += paletteAdvance;
		state->paletteNum %= state->sprite->numPalettes;

		if(state->paletteNum != oldPaletteNum)
			changeDetected = TRUE;
	}

	return changeDetected;
}




//Draws a sprite on top of whatever's already there, at the position
//determined by its motionStuff... (repositionSprite() needs to be called
//prior)
//Eventually will return the rowNum for REFRESH_ON_CHANGE, etc...
uint8_t fbQ_overlaySprite(const __flash sprite_t *p_theSprite, spriteState_t *state)     
																//int8_t *spritePosition)
{
   //uint8_t imageChangedTillRow=0;
	// doesn't really work any longer, since overlays happen multiplely
   uint8_t changeOccurred = FALSE;

   //It's too confusing with the camera having its own position, so e.g.
   //camCol=0, if it were used here, would NOT BE THE SAME as the column in
   //the "image" in which the camera's first column resides...
   // frameCol, instead, is the actual frame drawn...
   //int8_t camRow, camCol;
   int8_t frameRow, frameCol;

   //I can't recall how this used to work...
//   uint8_t paletteForColor = getSpritePalette(p_theSprite, 0, 0);
	uint8_t paletteForColor = 0;

	int8_t position[2] = {0,0};
	uint8_t * p_hFlip = NULL;
	uint8_t count = 0;

	//An unchanging sprite, e.g. SOLID in the background of a Reward...
	// (when NULL)
	if (state != NULL)
	{
		position[0] = state->position[0];
		position[1] = state->position[1];
		//hFlip = state->sprite->p_hFlip;	//6404
		p_hFlip = state->hFlip;					//6382
		count = state->count;
		paletteForColor = state->paletteNum;
	}

	uint8_t thisHFlip = GET_FLIP(p_hFlip, count);
	if(count > 0)
	{
		//Check if the flip-state has changed...
		if(GET_FLIP(p_hFlip, count-1) != thisHFlip)
			changeOccurred = TRUE;
	}
	else	//count 0, what to do...?
	{
		//Could always assume a change-occurred on count-0
		// but what about solid + reward in background?
		//Could compare to the last element in the hFlip array
		// in case of cycling...
		// (currently no sprites cycle hFlips)
		//I guess I'll just leave it alone.
	}

   for(frameRow = 0; frameRow<FB_HEIGHT; frameRow++)
   {
      int8_t spriteRow =
         (position[1] - cameraState.position[1]) + frameRow;
         
      //The sky's already been drawn...
      if((spriteRow >= FB_HEIGHT) || (spriteRow < 0))
         continue;
        
	  	if(p_theSprite == &spriteFLOWER)
		{	
			if(spriteRow >= 8)
				paletteForColor = NUMPALETTES_FLOWER-1;
		}

      //Don't need to do this if the row is not in the sprite...
      for(frameCol = 0; frameCol<FB_WIDTH; frameCol++)
      {
      
         int8_t spriteCol =
            (position[0] - cameraState.position[0]) + frameCol;
            
         if(thisHFlip) //GET_FLIP(p_hFlip, count))
            spriteCol=(FB_WIDTH-1)-spriteCol;
            
         //The sky's already been drawn...
         if((spriteCol >= FB_WIDTH) || (spriteCol < 0))
            continue;
           
		  //As it stands, a multi-image sprite is assumed to always
		  //cycle-through the images; one with each redraw...
         uint8_t colorData = getRawPixelVal(p_theSprite, spriteRow,
                                            spriteCol, 
											(state->count)%(p_theSprite->numImages));
                                                         
         //The sky is always, now, as I recall, 0...     
         // so use it as a mask.
         if(colorData == 0)
            continue; 



			colorData = rawPixValToGimpColorVal(colorData,
                                                p_theSprite,
                                                paletteForColor);
         colorData = gimpPixelValToLColor(colorData);
         
         frameBuffer[frameRow][frameCol] = colorData;
         
         //NEED TO STOP AT THE END OF THE MOTIONARRAY
      }  
      
      //For now we're just going to presume that the framebuffer 
      // has changed each time...
      //imageChangedTillRow = frameRow;
   }  
   
	//The only thing in here that should affect a graphical change is
	//hFlip...
   return changeOccurred; //imageChangedTillRow;
   
}  










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
 * /Users/meh/_avrProjects/LCDdirectLVDS/93-checkingProcessAgain/icons/spriteMotion.c
 *
 *    (Wow, that's a lot longer than I'd hoped).
 *
 *    Enjoy!
 */

/* mehPL:
 *    This is Open Source, but NOT GPL. I call it mehPL.
 *    I'm not too fond of long licenses at the top of the file.
 *    Please see the bottom.
 *    Enjoy!
 */












//fb_question is the good-ol' "Mario" Question-box, using a frameBuffer...
//  It was created after seg_question, which was created after a
//  frameBuffer-based method. Long convoluted story.

// Essentially, fb_question is the latest (a/o v66) version of the
// Mario-Question-Box, which has actually been installed at a somewhat
// permanent location for public-interaction. (and Seg_question has since
// been put on the back-burner).

// When the question-box is "hit" it displays a random sprite:
//  A Coin, 1-up, Biggie, Star, Flower, or possibly a Goomba
//  If you *hit* the goomba, it dies.

// As-implemented (in main), the ADC is used as the hit-detector. Connected
// to it is a piezo-element. Unfortunately, I'm certain, the
// threshold-values, etc. for this "hit-sensor" will have to be configured
// for your particular hardware... And, in fact, I've found that it's
// sensitive to electrical noise (e.g. from the backlight inverter) as well
// as things like temperature. It's kinda hokey, but it's also in main()
// and could easily be converted to a simple momentary-switch.






	//Start at brown, to match the SOLID color
#define Q_QSTART	7 //3

//a/o v62-21:
// this is stolen from seg_question.c and modified for the framebuffer...



//These are not included in NUM_ICONS
   #include "icons/Question.h"
   #include "icons/Solid.h"
   #include "icons/GoombaDead.h"

   //These are included in NUM_ICONS
   #include "icons/1up.h"
   #include "icons/Biggie.h"
   #include "icons/StarV.h"
   #include "icons/FlowerPowerV.h"
   #include "icons/Goomba.h"
   #include "icons/Coin.h"
	#include "icons/Leaf.h"
	#include "icons/Cloud.h"
	#include "icons/Mario.h"
	#include "icons/Luigi.h"
	#include "icons/MarioRuns.h"
	#include "icons/LuigiRuns.h"
//a/o v77
//APPARENTLY NUM_ICONS is no longer used.
//Excluding Solid and Question, etc...
//#define NUM_ICONS 6


//Allow the user to choose another winnings when the question-box is
//scrolling down to reveal it... for testing
//#define IMMEDIATE TRUE

//This is just a hack for testing BIGGIE with the Leaf tester, which was a
//hack in the first place.
//#define MOTION_BIGGIE TRUE	//RANDOM_OVERRIDE's gotta be 2
//These are from hitDetected()
//#define RANDOM_OVERRIDE 8//32//2//Biggie //5//Leaf  //4 //8 //0 //4

//Stalls at the first sprite (initilized in p_selectedSprite, below)
//#define STAY_ON_IT	TRUE

//Automatically hit after a few loops in the "Q" box
//#define AUTO_HIT	TRUE
//#define AUTO_KILL_GOOMBA	TRUE

//THESE DON'T BELONG HERE
// AND they shouldn't be "Q" specific, at all.
#define Q_WIDTH   16
#define Q_HEIGHT   16
#if((Q_WIDTH != FB_WIDTH) || (Q_HEIGHT != FB_HEIGHT))
#error "FB dims do not match Icon dims"
#endif

//this is just a counter to help throw in some additional randomness...
uint8_t callCount = 0;

uint8_t qCount = 0;
//sprite_t *p_selectedSprite = &spriteLEAF; //COIN; //GOOMBA; //FLOWER; //&spriteQ;

//a/o v84: nextSprite is hokily used as selectedSprite...
// This needs to be changed.
//sprite_t *p_nextSprite = &spriteLEAF; //NULL;
const __flash sprite_t *p_nowSprite = &spriteLEAF; //NULL;
//This indicates whether we're in a "Reward"
// e.g. to draw SOLID in the background
uint8_t nowIsReward = TRUE;


void initSpriteStates(const __flash sprite_t *newSprite);


void fbQuestion_hitDetected(void)
{

	if( (p_nowSprite == &spriteQ)
//#if (!defined(IMMEDIATE) || !IMMEDIATE)
//	    && (p_nextSprite == NULL)
//#endif
	  )
	{
		uint8_t hitReward;
		//Just trying to get some randomness up in hea
#if (defined(RANDOM_OVERRIDE))
		hitReward = RANDOM_OVERRIDE;
#elif (defined(HSYNC_TIMER_TCNT))
		static uint8_t lastReward;
		
		do
		{
			hitReward = (HSYNC_TIMER_TCNT + callCount + qCount)%12;
			//Don't allow double-goombas
		} while ((lastReward == 4) && (hitReward == 4));

		lastReward = hitReward;
#else
 #error "HSYNC_TIMER_TCNT should be available for randomness..."
#if 0
		//These guys don't have enough randomness to avoid call-count...
 #if (defined(__ADC_H__))
		hitReward = ((adc_getValue()*3+callCount+qCount))%8;
 #else
		hitReward = (callCount+qCount)%8;
 #endif
#endif
#endif

		switch(hitReward)
		{
			case 0:
				p_nowSprite = &spriteFLOWER;
				break;
			case 1:
				p_nowSprite = &sprite1UP;
				break;
			case 2:
				p_nowSprite = &spriteBIG;
				break;
			case 3:
				p_nowSprite = &spriteSTAR;
				break;
			case 4:
				p_nowSprite = &spriteGOOMBA;
				break;
			case 5:
				p_nowSprite = &spriteLEAF;
				break;
			case 6:
				p_nowSprite = &spriteCLOUD;
				break;
			case 7:
				p_nowSprite = &spriteMARIO;
				break;
			case 8:
				p_nowSprite = &spriteLUIGI;
				break;
//			case 9:
//				p_nowSprite = &spriteMARIORUNS;
				break;
			default:
				p_nowSprite = &spriteCOIN;
				break;
		}
		//qCount = 0;
		nowIsReward = TRUE;
		initSpriteStates(p_nowSprite);
	}
/* This was nice with the old scroll-up method, but with the new method,
 * it's too hokey... (can smash it before it's visible)
	else if( (p_nextSprite == &spriteGOOMBA)
		      && (qCount > 4) )
	{
		p_nextSprite = &spriteDEADGOOMBA;
	}
*/
	//As-is, the last frame (in blue, indicating death) is only visible
	//briefly before scroll-back-to-question starts...
	// so there's no reason to test for KILLABLE
	// since it can't be killed when it's scrolling.
	// There's probably a tiny fraction of a second window where it is
	// possible to hit it when it's blue, and kill it... but it's TINY.
	else if((p_nowSprite == &spriteGOOMBA)
				&& ((qCount > 16) && (qCount < 62)))
//				&& (p_nextSprite == NULL) )
//			   && ((qCount > 0) && (qCount < 16)) )
//			   && (qCount < GOOMBA_KILLABLE_QCOUNT) )
	{
		//qCount = 0;
		nowIsReward = TRUE;
		p_nowSprite = &spriteDEADGOOMBA;
		initSpriteStates(p_nowSprite);
	}

}

//If a spriteRow is outside the icon's dimensions, draws the sky-color
//Returns TRUE if a pixel has changed
// for redrawing...
uint8_t drawSpriteRow(const __flash sprite_t *p_thisSprite, uint8_t spritePhase,
													  int8_t spriteRow, 
													  uint8_t rowToDrawAt);



#if(defined(STAY_ON_IT) && STAY_ON_IT)
//Check for completion of one sprite, and setup the next...
void prepNextSprite(void)
{
}
#else
void prepNextSprite(void)
{
	//a/o v84:
	// The new plan is to have a defined path...
	// Q -> Hit -> "Reward" -> Solid -> Q
		
	// CURRENTLY: "Hit" is NYI so:
	// Q -> "Reward" -> Solid -> Q

	// ALSO: Goomba/GoombaDead needs to be reimplemented

	// Also, we've been using p_nextSprite as the current sprite for some
	// time. It should probably be renamed to selectedSprite, and the old
	// p_selectedSprite should probably be removed entirely.
	// IOW: There's no need for "nextSprite" in the new method.

	// Q Loops...
	if(p_nowSprite == &spriteQ)
	{
		//Do nothing for Q... until a hit arrives
		// qCount is reset by the caller, so it just loops
		// nowIsReward should already be FALSE
		
		// If AUTO_HIT is true, then "after" Question-box "completes"
		// act like a hit was detected...
	  #if(defined(AUTO_HIT) && AUTO_HIT)
		fbQuestion_hitDetected();
	  #else
		return;
     #endif
	}
	// Mario -> MarioRuns
	else if(p_nowSprite == &spriteMARIO)
	{
		nowIsReward = TRUE;
		p_nowSprite = &spriteMARIORUNS;
	}
	// Luigi -> LuigiRuns
	else if(p_nowSprite == &spriteLUIGI)
	{
		nowIsReward = TRUE;
		p_nowSprite = &spriteLUIGIRUNS;
	}
	// Solid -> Q
	else if(p_nowSprite == &spriteSOLID)
	{
		nowIsReward = FALSE;
		p_nowSprite = &spriteQ;
	}
	// "Reward" -> Solid
	else
	{
		nowIsReward = FALSE;
		p_nowSprite = &spriteSOLID;
	}	

	initSpriteStates(p_nowSprite);
}

void initSpriteStates(const __flash sprite_t *newSprite)
{
	qCount = 0;
	//.count is handled by qCount... but let's init it anyhow.
	
	p_nowSprite = newSprite;
	
	
	
	
	nowSpriteState.sprite = p_nowSprite;
	nowSpriteState.count = 0;
	nowSpriteState.hFlip = p_nowSprite->p_hFlip;
	nowSpriteState.motion = p_nowSprite->p_motion;
	//Origin is lower-left, but nextSpritePosition is upper-left...
	nowSpriteState.position[0] = -(nowSpriteState.motion[0].startPosition);
	nowSpriteState.position[1] = nowSpriteState.motion[1].startPosition;
	nowSpriteState.dir[0] = 1;
	nowSpriteState.dir[1] = 1;

	nowSpriteState.layer = p_nowSprite->p_layer;
	nowSpriteState.paletteNum = 0;


	//No sprite associated with the camera...
	cameraState.sprite = NULL;
	cameraState.count = 0;
	cameraState.hFlip = NadaFlip;
	cameraState.motion = p_nowSprite->p_camMotion;
	cameraState.position[0] = -(cameraState.motion[0].startPosition);
	cameraState.position[1] = cameraState.motion[1].startPosition;
	cameraState.dir[0] = 1;
	cameraState.dir[1] = 1;

	cameraState.layer = NadaLayer;
	cameraState.paletteNum = 0;


	//This is only used by the SOLID in the background for Reward...
	// So probably never changes...
	//otherSpriteState.hFlip = NadaFlip;
	//otherSpriteState.motion = NadaMotion;

#if 0
		//DEADGOOMBA -> Question-Box (directly)
		if(p_selectedSprite == &spriteDEADGOOMBA)
		{
			if(qCount >= 16)
			{
				qCount = Q_QSTART; //0;
				p_nextSprite = &spriteQ;
			}
		}
		//GOOMBA -> SOLID
		else if(p_selectedSprite == &spriteGOOMBA)
		{
			if(qCount >= GOOMBA_QCOUNT)
			{
				qCount = 0;
				p_nextSprite = &spriteSOLID;
			}
		}
		//COIN -> SOLID
		else if(p_selectedSprite == &spriteCOIN)
		{
			if(qCount >= 8)
			{
				qCount = 0;
				p_nextSprite = &spriteSOLID;
			}
		}
		//SOLID -> Question-Box
		else if(p_selectedSprite == &spriteSOLID)
		{
			if(qCount >= 8)
			{
				qCount = Q_QSTART; //0;
				p_selectedSprite = &spriteQ;
			}
		}
		//**OTHERS** (NOT Question-Box) -> SOLID
		else if(p_selectedSprite != &spriteQ)
		{
			if(qCount >= 16)
			{
				qCount = 0;
				//if(p_selectedSprite != &spriteSOLID)
				p_nextSprite = &spriteSOLID;
				//else
				//   p_selectedSprite = &spriteQ;
			}
		}
#endif //0
}
#endif //!STAY_ON_IT



// NEW MOTION-HANDLING...






//This is sort-of Motion-Handling, as well...
//Usually the sky...
void fbQ_drawBackground(spriteState_t *state)
{
	//I can't recall how this used to work...
	//uint8_t paletteForColor = getSpritePalette(&spriteLEAF, 0, 0);
	uint8_t paletteForColor = state->paletteNum;



	//probably should look into skyOverride
	// for now, use the sky-color of the leaf, which is at (0,0)
	//Fill the background with the sky-color...
//#define FBQ_SKYCOLOR_OVERRIDE _R
#if(defined(FBQ_SKYCOLOR_OVERRIDE))
	uint8_t skyColorData = FBQ_SKYCOLOR_OVERRIDE;
#else
	//uint8_t skyColorData = getRawPixelVal(, 0, 0);

	uint8_t skyColorData = rawPixValToGimpColorVal(0, 
														state->sprite,
														paletteForColor);
	skyColorData = gimpPixelValToLColor(skyColorData);
#endif


	int8_t camRow, camCol;

	for(camRow = 0; camRow<FB_HEIGHT; camRow++)
	{
		for(camCol = 0; camCol<FB_WIDTH; camCol++)
		{
#if(defined(FBQ_RAINBOW_SKY) && FBQ_RAINBOW_SKY)
			frameBuffer[camRow][camCol] = (camRow+camCol)&0x3f;//skyColorData;
#else
			frameBuffer[camRow][camCol] = skyColorData;
#endif

			//Should look into framebuffer-changed stuff, here...
		}
	}
}



//Returns the last image-row that has changed...
// (for refresh-on-change and/or partial-refresh)
// otherwise -1 if no change.
int8_t fbQuestion_update(void) //uint8_t triggerDetected)
{
	//last row that was changed...
	int8_t imageChangedTillRow = -1;

	//This is just used for helping to throw in some randomness for the next
	//sprite after a hit...
	callCount++;

//   uint8_t qRow;
	
	//Check if it's time to select the next sprite, and do-so according to
	//which sprite we're on and which should follow it.
	//Most of the time, a sprite is followed by SOLID, but there are
	//exceptions...
/*
	if(p_nextSprite == NULL)
	{
		prepNextSprite();
	}
*/

/*
	if(p_nextSprite != NULL)
	{
*/
		//a/o v84:
		//Is this necessary with motion handling, since the sky is drawn
		//separately???
		setSpriteSkyColorOverride(p_nowSprite);

		//This isn't right, doesn't transition at all
		// it's just for testing the motion-scheme
		//Testing the new transition-scheme...
/*
#if(defined(MOTION_BIGGIE) && MOTION_BIGGIE)
		if(p_nextSprite == &spriteBIG)
#else
		if(p_nextSprite == &spriteLEAF)
#endif
*/
		//if(p_selectedSprite == &spriteLEAF) //NoGo
		{
#if(defined(AUTO_KILL_GOOMBA) && AUTO_KILL_GOOMBA)
			if( (p_nowSprite == &spriteGOOMBA)
				 && (qCount >= 60) )
				fbQuestion_hitDetected();
#endif



			nowSpriteState.count = qCount;
			cameraState.count = qCount;
			//Override for now...
			//otherSpriteState.count=0;

			fbQ_drawBackground(&nowSpriteState);

			if(fbQ_repositionSprite(&nowSpriteState))
				imageChangedTillRow = FB_HEIGHT;

			if(fbQ_repositionSprite(&cameraState))
				imageChangedTillRow = FB_HEIGHT;
			// "otherSpriteState" needn't be repositioned, it's always
			// SOLID (if used, only during a Reward)

			if(nowIsReward)
			{
				//0 is foreground, so it should be drawn last
				if(!GET_LAYER(nowSpriteState.layer, nowSpriteState.count))
				{
					//imageChangedTillRow =
					//fbQ_overlaySprite(p_selectedSprite, selectedSpritePosition);
					if(fbQ_overlaySprite(&spriteSOLID, NULL))
						imageChangedTillRow = FB_HEIGHT;

					//imageChangedTillRow =
					if(fbQ_overlaySprite(p_nowSprite, &nowSpriteState))
						imageChangedTillRow = FB_HEIGHT;
				}
				else
				{
					//imageChangedTillRow =
					if(fbQ_overlaySprite(p_nowSprite, &nowSpriteState))
						imageChangedTillRow = FB_HEIGHT;
				
					//imageChangedTillRow =
					//fbQ_overlaySprite(p_selectedSprite, selectedSpritePosition);
					if(fbQ_overlaySprite(&spriteSOLID, NULL))
						imageChangedTillRow = FB_HEIGHT;
				}
			}
			else //Not a reward, so only one sprite is drawn...
			{
				//imageChangedTillRow =
					if(fbQ_overlaySprite(p_nowSprite, &nowSpriteState))
						imageChangedTillRow = FB_HEIGHT;
			}

			qCount++;
			//Could, e.g., test for a hard-coded value...
/*#if(defined(MOTION_BIGGIE) && MOTION_BIGGIE)
			if(qCount >= (sizeof(DefaultY)*4))
#else
			if(qCount >= (sizeof(LeafX)*4))
#endif
*/
			if(qCount >= p_nowSprite->totalCount)
			{
				qCount = 0;
				prepNextSprite();
/*
#define LEAF_REPEAT	TRUE
#if(!defined(LEAF_REPEAT) || !LEAF_REPEAT)
				p_selectedSprite = p_nextSprite;
				p_nextSprite = NULL;
#endif
*/
			}
			return imageChangedTillRow; 
		}
/*
		//Transitioning from the Question-box to the next sprite
		else if(p_selectedSprite == &spriteQ) 
		{
			uint8_t spriteRow;

			//Draw the top of the next sprite...
			for(qRow=0; qRow<qCount; qRow++)
			{
				spriteRow = qRow - (16 - qCount);
				if(drawSpriteRow(p_nextSprite, qCount, spriteRow, qRow))
					imageChangedTillRow = qRow;
			}
		
			//This is kinda hokey, but should work...
			// rather than displaying the "?" after it's been hit, it should
			// become solid... but in most cases using p_selectedSprite==SOLID
			// results in a transition straight to Q...
			// so rather than changing p_selectedSprite, let's just override
			// it here
			spriteRow = 0;
			for( ; qRow<Q_HEIGHT; qRow++)
			{
				if(drawSpriteRow(&spriteSOLID, qCount, spriteRow, qRow))
					imageChangedTillRow = qRow;

				spriteRow++;
			}
			
		}
		//Transitioning back from a sprite to the question-box/solid
		else
		{
			uint8_t displayRow;
			uint8_t spriteRow = qCount; 

			for(displayRow = 0; displayRow < (Q_HEIGHT-qCount); displayRow++)
			{
				if(drawSpriteRow(p_selectedSprite,qCount,spriteRow,displayRow))
					imageChangedTillRow = displayRow;

									//				displayRow+qCount, displayRow);
				spriteRow++;
			}
	
			spriteRow = 0;
			for( ; displayRow<Q_HEIGHT; displayRow++)
			{
				if(drawSpriteRow(p_nextSprite, qCount, spriteRow, displayRow))
					imageChangedTillRow = displayRow;

					//				displayRow-(Q_HEIGHT-1-qCount), displayRow);

				spriteRow++;
			}
		}

		//a/o v84: This bit's avoided with motionHandling due to return...
		qCount++;
		if(qCount >= Q_HEIGHT)
		{
			qCount = 0;
			p_selectedSprite = p_nextSprite;
			p_nextSprite = NULL;
		}

	} //end of nextSprite Handling...
	else //we only have a selectedSprite (nextSprite is NULL)
	{
		setSpriteSkyColorOverride(NULL);

		for(qRow=0; qRow<Q_HEIGHT; qRow++)
		{
			if(drawSpriteRow(p_selectedSprite, qCount, qRow, qRow))
				imageChangedTillRow = qRow; //TRUE;
		}

		qCount++;
		//the only case where qCount should cycle is spriteQ...
		// its value is %12, so could either reset it at 12, or this.. whatev
		// To avoid a glitch every 21 cycles.
		if(qCount >= ((255 / 12) * 12))
			qCount = 0;
	}
*/
	return imageChangedTillRow;
}

uint8_t getSpritePalette(const __flash sprite_t *p_thisSprite, uint8_t spritePhase,
																	uint8_t spriteRow)
{

	uint8_t thePalette;
	
	
	if(p_thisSprite == &spriteFLOWER)
	{
		if(spriteRow<FLOWER_PALETTE1_ROW)
			//data = getGimpColorVal(&spriteFLOWER,
			  thePalette = spritePhase%(spriteFLOWER.numPalettes-1) + 1; 
					 //spriteRow, qCol);
		else
			thePalette = 0;
			//data = getGimpColorVal(&spriteFLOWER, 0, spriteRow, qCol);
	}
	else if(p_thisSprite == &spriteGOOMBA)
	{
		//uint8_t gCol = qCol;
		//Goomba moves by flipping horizontally...
	//	if((spritePhase < 16) && (spritePhase & 0x01))
	//		theCol = 15-qCol;

		//uint8_t palette; // = qCount * p_thisSprite->numPalettes / 16;
		//if(palette >= p_thisSprite->numPalettes)
		//	palette = p_thisSprite->numPalettes-1;

			
		switch(spritePhase)
		{
				case 0:
				case 1:
				case 2:
				case 3:
					thePalette = 0;
					break;
				case 4:
				case 5:
				case 6:
					thePalette = 1;
					break;
				case 7:
				case 8:
				case 9:
					thePalette = 2;
					break;
				case 10:
				case 11:
				case 12:
					thePalette = 3;
					break;
				case 13:
				case 14:
				case 15:
					thePalette = 4;
					break;
				default:
					thePalette = p_thisSprite->numPalettes-1;
		}


		//data = getGimpColorVal(&spriteGOOMBA, palette, spriteRow, gCol);
	}
	else
	{
		//data = getGimpColorVal(p_thisSprite, 
		  thePalette = spritePhase%p_thisSprite->numPalettes;
						//spriteRow, qCol);
	}

	return thePalette;
}

//This does NOT handle COIN, see getRowPixelValCOIN()...
// vOffset is whether the sprite is above the frame-buffer or below...
// centered = 0
// so, e.g. vOffset = spriteRow - rowToDrawAt
uint8_t getSpritePhase(const __flash sprite_t *p_thisSprite, uint8_t qCount, int8_t
		vOffset)
{
	uint8_t spritePhase = qCount;

	if(p_thisSprite == &spriteQ)
	{
		switch(spritePhase%12)
		{
			case 0:
			case 1:
			case 2:

			case 9:
			case 10:
			case 11:
				spritePhase = 0; //yellow background
				break;
			case 3:
			case 4:

			case 7:
			case 8:
				spritePhase = 1; //red background
				break;
			case 5:
			case 6:

			default:
				spritePhase = 2; //brown background
				break;
		}

	}
	//else if(p_thisSprite == &spriteCOIN) //handled in getRowPixelValCOIN()
	else if(p_thisSprite == &spriteGOOMBA)
	{
		if (vOffset < 0) //(spriteRow < rowToDrawAt)
			spritePhase = 0;
		if (vOffset > 0) //(spriteRow > rowToDrawAt)
			spritePhase = GOOMBA_QCOUNT;
		else
			spritePhase /=3;

	}

	return spritePhase;

}

/*
//spritePhase replaces qCount... for determining e.g. the color palette and
//the motion of the goomba...
// This does *NOT* handle COIN... use getRowPixelValCOIN() instead.
uint8_t drawSpriteRow(const __flash sprite_t *p_thisSprite, uint8_t qCount,
															//uint8_t spritePhase,
													  int8_t spriteRow, 
													  uint8_t rowToDrawAt)
{
	uint8_t rowChanged = FALSE;
	uint8_t qCol;

	uint8_t spritePhase;

	spritePhase = getSpritePhase(p_thisSprite, qCount, 
												((int8_t)spriteRow - rowToDrawAt) );

	//This may get overridden (for the sky)
	uint8_t thePalette = getSpritePalette(p_thisSprite, spritePhase,
																				spriteRow);
	
	for(qCol=0; qCol<Q_WIDTH; qCol++)
   {

		// Should use readImageByte... but it's specific to FB_WIDTH
      //uint8_t data=
      //  pgm_read_byte((uint8_t *)(&((p_image)[(qRow)*Q_WIDTH+(qCol)])));
      uint8_t data;
		int8_t theCol = qCol;


		if(p_thisSprite == &spriteGOOMBA)
		{
			//uint8_t gCol = qCol;
			//Goomba moves by flipping horizontally...
			if((spritePhase < 16) && (spritePhase & 0x01))
				theCol = 15-qCol;
		}

		//Early test, just slide across the screen
		if(p_thisSprite == &spriteLEAF)
		{
			theCol = qCol-spritePhase;
		}


		//If rawPixVal == 0, then draw the sky-color...
		uint8_t rawPixVal;

		if((theCol >= FB_WIDTH) || (theCol < 0))
			rawPixVal = 0; 
		else if((spriteRow < 0) || (spriteRow >= Q_HEIGHT))
			rawPixVal = 0;
		else if(p_thisSprite == &spriteCOIN)
			rawPixVal = getRawPixelValCOIN(spritePhase, spriteRow, theCol);
		else
			rawPixVal = getRawPixelVal(p_thisSprite, spriteRow, theCol);


		const __flash sprite_t *p_spriteForColor = p_thisSprite;
		uint8_t paletteForColor = thePalette;

		if( (rawPixVal == 0) && (skyOverrideSprite != NULL) )
		{
			p_spriteForColor = skyOverrideSprite;
			int8_t vOffset = spriteRow - rowToDrawAt;

			if (skyOverrideSprite != p_thisSprite)
				vOffset = -vOffset;

			uint8_t spritePhaseForColor = 
						getSpritePhase(p_spriteForColor, qCount, vOffset);
									//				(spriteRow - rowToDrawAta)); 
			paletteForColor = getSpritePalette(p_spriteForColor, 
															spritePhaseForColor,
															spriteRow);
		}


		data = rawPixValToGimpColorVal(rawPixVal, p_spriteForColor,
																		paletteForColor); 
		
      data = gimpPixelValToLColor(data);

		if(frameBuffer[rowToDrawAt][qCol] != data)
		{
			rowChanged = TRUE;
			frameBuffer[rowToDrawAt][qCol] = data;
		}
	}

	return rowChanged;
}
*/

#if 0
   //Wow, is it really so smart as to recognize that rowNum is never >=
   // V_COUNT?! was 7684 when if(rowNum==0), then 7668 after this bug...
   //if(rowNum >= V_COUNT)
#if 0
	if(rowNum >= V_COUNT-1)
   {
      qCount++;
   }
#endif

	qCount++;
   //Wait, what?! We can't fit 1024 segments! But this sorta works
   // We're gtting syncing, a tiny bit of white-space on the right
   // which seems reasonable based on shotty math, 
   //                10     *  32   +  10   *    2       /   3 = 326
   // but 1024/3 is 341 (where's my math error?)
   //addSegfb(1024-Q_SEGLENGTH*Q_WIDTH-Q_WIDTH*Q_STRETCHTOP/Q_STRETCHDIV, _W);
   // But, nope, 341-... doesn't sync (vertical stretching again)
   // But, 342 does. Sweet. 1024/3=341.33333, actually.
   //addSegfb(342-Q_SEGLENGTH*Q_WIDTH-Q_WIDTH*Q_STRETCHTOP/Q_STRETCHDIV, _W);
   // Because it's not 10*2/3, it's 32*2/3=21.3...
   // which is exactly right
   // which then doesn't explain why it has whitespace and doesn't sync
   // without 342 above...
   // something to do with white being necessary?
   // Can't recall where 1024/3 was determined, maybe it varies
   // since the LVDS clock is 7 cycles...?
   //OK, so maybe we just need a white pixel at the end...
   // I mean, this works.
//   addSegfb(1,_W);


//   segTerminate();
}
#endif //0
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
 * /Users/meh/_avrProjects/LCDdirectLVDS/93-checkingProcessAgain/fb_question.c
 *
 *    (Wow, that's a lot longer than I'd hoped).
 *
 *    Enjoy!
 */

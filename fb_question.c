/* mehPL:
 *    This is Open Source, but NOT GPL. I call it mehPL.
 *    I'm not too fond of long licenses at the top of the file.
 *    Please see the bottom.
 *    Enjoy!
 */


	//Start at brown, to match the SOLID color
#define Q_QSTART	7 //3
#define GOOMBA_QCOUNT	(24*2)
//#define GOOMBA_QCOUNTSPERPHASE	3
//#define GOOMBA_KILLABLE_QCOUNT	(GOOMBA_QCOUNT-2)

//a/o v62-21:
// this is stolen from seg_question.c and modified for the framebuffer...


//Actually, this probably won't work right...
// because the rgb values aren't exact like that, right?
// e.g. black = 0,0,0 but it shows as 0,0,60 ish...
// then again, going in reverse like this might work fine...
// due to rounding (seems OK with this color-scheme)
// The idea wasn't so much to be able to input an 8-bit RGB value to get
// a close match (would probably want rounding up for that, not that it'd
// be even close)
// but to be able to read color-values from GIMP when using the 
//  LCDdirectLVDS color-palette... which isn't especially necessary since
//  now the colors are named with 3-based colors.
#define rgb8(r,g,b) \
      rgb((((r)*3)/255), (((g)*3)/255), (((b)*3)/255))


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

   //Excluding Solid and Question, etc...
//   #define NUM_ICONS 6


//Allow the user to choose another winnings when the question-box is
//scrolling down to reveal it... for testing
//#define IMMEDIATE TRUE

//#define RANDOM_OVERRIDE 4 //8 //0 //4

//Stalls at the first sprite (initilized in p_selectedSprite, below)
//#define STAY_ON_IT	TRUE

//Automatically hit after a few loops in the "Q" box
//#define AUTO_HIT	TRUE

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
sprite_t *p_selectedSprite = &spriteCOIN; //GOOMBA; //FLOWER; //&spriteQ;
sprite_t *p_nextSprite = NULL;

void fbQuestion_hitDetected(void)
{
	if( (p_selectedSprite == &spriteQ)
#if (!defined(IMMEDIATE) || !IMMEDIATE)
	    && (p_nextSprite == NULL)
#endif
	  )
	{
		qCount = 0;
#ifndef RANDOM_OVERRIDE
		//Just trying to get some randomness up in hea
		switch(((adc_getValue()*3+callCount+qCount))%8)
#else
		switch(RANDOM_OVERRIDE)
#endif
		{
			case 0:
				p_nextSprite = &spriteFLOWER;
				break;
			case 1:
				p_nextSprite = &sprite1UP;
				break;
			case 2:
				p_nextSprite = &spriteBIG;
				break;
			case 3:
				p_nextSprite = &spriteSTAR;
				break;
			case 4:
				p_nextSprite = &spriteGOOMBA;
				break;
			//The remaining are just for testing...
/*			case 5:
				p_selectedSprite = &spriteDEADGOOMBA;
				break;
			case 6:
				p_selectedSprite = &spriteQ;
				break;
			case 7:
				p_selectedSprite = &spriteSOLID;
				break;
*/			default:
				p_nextSprite = &spriteCOIN;
				break;
		}
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
	else if( (p_selectedSprite == &spriteGOOMBA)
				&& (p_nextSprite == NULL) )
//			   && ((qCount > 0) && (qCount < 16)) )
//			   && (qCount < GOOMBA_KILLABLE_QCOUNT) )
	{
		qCount = 0;
		p_selectedSprite = &spriteDEADGOOMBA;
	}

}

//If a spriteRow is outside the icon's dimensions, draws the sky-color
void drawSpriteRow(sprite_t *p_thisSprite, uint8_t spritePhase,
													  int8_t spriteRow, 
													  uint8_t rowToDrawAt);

void fbQuestion_update(void) //uint8_t triggerDetected)
{
	callCount++;
		//This should be an init-thing, I guess... it's only a 2-cycle
      // instruction, so it doesn't hurt too much to have it in the loop
      // BUT it does have to occur a while before the first getpinPORT
      // to allow time for the pull-ups to do their job
//      setinpuPORT(PB0, PORTB);


//	if(triggerDetected)
//		qCount++;

//   segClear();
   //Not sure why this is stretched without /2...
   // I think I ran into this elsewhere as well.
   //!!! INSTEAD: filled the rest of the row properly...
   // (wasn't filling up the entire row, due to rounding...
   //   result was the rows were carrying-over into the next)
   uint8_t qRow;// = (rowNum*Q_HEIGHT/(V_COUNT)); ///2));
   uint8_t qCol;
   
   //Guess white helps with sync...?
   // can't hurt.
   //addSegfb(3, _W);
   // Looks like some white is already being inserted (or syncing on end-
   // white?) about 3pixels wide.

   // 1024/3/32 = 10.67... so for every 3 segments, add 2
//#define Q_STRETCHTOP   1//0//28//2
//#define Q_STRETCHDIV 5//30//3
//   hfm_t colincrementer_hfm;
//   hfm_setup(&colincrementer_hfm, Q_STRETCHTOP, Q_STRETCHDIV);

	if(p_nextSprite == NULL)
	{
#if(!defined(STAY_ON_IT) || !STAY_ON_IT)
	if(p_selectedSprite == &spriteDEADGOOMBA)
	{
		if(qCount >= 16)
		{
			qCount = Q_QSTART; //0;
			p_nextSprite = &spriteQ;
		}
	}
	else if(p_selectedSprite == &spriteGOOMBA)
	{
		if(qCount >= GOOMBA_QCOUNT)
		{
			qCount = 0;
			p_nextSprite = &spriteSOLID;
		}
	}
	else if(p_selectedSprite == &spriteCOIN)
	{
		if(qCount >= 8)
		{
			qCount = 0;
			p_nextSprite = &spriteSOLID;
		}
	}
	else if(p_selectedSprite == &spriteSOLID)
	{
		if(qCount >= 8)
		{
			qCount = Q_QSTART; //0;
			p_selectedSprite = &spriteQ;
		}
	}
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
	//else is handled in the trigger-handler...
#if(defined(AUTO_HIT) && AUTO_HIT)
	else //spriteQ
	{
		if(qCount >= (12*5))
			fbQuestion_hitDetected();
	}
#endif
#endif //STAY_ON_IT
	}
	//There're two scrolling-cases
	// selected == Q && next == other
	// selected == other && (next == Q || next == SOLID)
	//
	//The first case shows the next sprite's top above the Q-box
	// the "window" is on the next sprite, but the Q box overlaps it
	// and the Q-box scrolls downward
	//      _________
	//  ^  |   ___   | \  <--Next
   //  |  |  /   \  |  | next sprite appears to scroll up
	//  |  ||¯¯¯¯¯¯¯||  | but really the overlapping selected-sprite scrolls
	//     || *   * || /  down...
	//      |   ?   |
	//      | *   * | <--Selected
	//       ¯¯¯¯¯¯¯
	//
	//The second case shows the two sprites atop each other
	// the "window" scrolls down the two
	//      _________
	//     |   ___   | <--Selected
	//   ..|../___\..|.. 
	// | : |   |_|   | : \  The window scrolls down, but the two sprites
	// | : |_________| :  | do not change position relative to each other
	// V : |         | : /
	//   ..|.*.....*.|..
	//     |    ?    |
	//     | *     * | <--Next
	//      ¯¯¯¯¯¯¯¯¯


	// NEW:
	//  The first case is kinda boring... it looks like the question-box is
	//  being pulled-away to reveal the item, rather than that the item is
	//  coming out of the question-box
	//  Instead, raise the item from the bottom (in the background) and
	//  lower the box... that way both are moving at the same time.
	//
	//       _____                               ___
	//      |     |     _____         __        /___\
	//      |  ?  | -> |     |  ->  _/__\_  ->   |_|
	//      |_____|    |  ?  |     |      |

	if(p_nextSprite != NULL)
	{

		setSpriteSkyColorOverride(p_selectedSprite);

		if(p_selectedSprite == &spriteQ) 
		{
			uint8_t spriteRow;

			//Draw the top of the next sprite...
			for(qRow=0; qRow<qCount; qRow++)
			{
				spriteRow = qRow - (16 - qCount);
				drawSpriteRow(p_nextSprite, qCount, spriteRow, qRow);
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
				drawSpriteRow(&spriteSOLID, qCount, spriteRow, qRow);
				spriteRow++;
			}
			
		}
		else
		{
			uint8_t displayRow;
			uint8_t spriteRow = qCount; 

			for(displayRow = 0; displayRow < (Q_HEIGHT-qCount); displayRow++)
			{
				drawSpriteRow(p_selectedSprite, qCount, spriteRow, displayRow);
									//				displayRow+qCount, displayRow);
				spriteRow++;
			}
	
			spriteRow = 0;
			for( ; displayRow<Q_HEIGHT; displayRow++)
			{
				drawSpriteRow(p_nextSprite, qCount, spriteRow, displayRow);
					//				displayRow-(Q_HEIGHT-1-qCount), displayRow);

				spriteRow++;
			}
		}

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
			drawSpriteRow(p_selectedSprite, qCount, qRow, qRow);

		qCount++;
		//the only case where qCount should cycle is spriteQ...
		// its value is %12, so could either reset it at 12, or this.. whatev
		// To avoid a glitch every 21 cycles.
		if(qCount >= ((255 / 12) * 12))
			qCount = 0;
	}

}

uint8_t getSpritePalette(sprite_t *p_thisSprite, uint8_t spritePhase,
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
uint8_t getSpritePhase(sprite_t *p_thisSprite, uint8_t qCount, int8_t
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


//spritePhase replaces qCount... for determining e.g. the color palette and
//the motion of the goomba...
// This does *NOT* handle COIN... use getRowPixelValCOIN() instead.
void drawSpriteRow(sprite_t *p_thisSprite, uint8_t qCount,
															//uint8_t spritePhase,
													  int8_t spriteRow, 
													  uint8_t rowToDrawAt)
{
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
		uint8_t theCol = qCol;


		if(p_thisSprite == &spriteGOOMBA)
		{
			//uint8_t gCol = qCol;
			//Goomba moves by flipping horizontally...
			if((spritePhase < 16) && (spritePhase & 0x01))
				theCol = 15-qCol;
		}



		uint8_t rawPixVal;

		if((spriteRow < 0) || (spriteRow >= Q_HEIGHT))
			rawPixVal = 0;
		else if(p_thisSprite == &spriteCOIN)
			rawPixVal = getRawPixelValCOIN(spritePhase, spriteRow, theCol);
		else
			rawPixVal = getRawPixelVal(p_thisSprite, spriteRow, theCol);


		sprite_t *p_spriteForColor = p_thisSprite;
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

		frameBuffer[rowToDrawAt][qCol] = data;
	}
}

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
 *    2) Please do not change/remove others' credit/licensing/copywrite 
 *         info, where noted. 
 *    3) If you find yourself profitting from my work, please send me a
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
 *    (Wow, that's a lot longer than I'd hoped).
 *
 *    Enjoy!
 */

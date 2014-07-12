/* mehPL:
 *    This is Open Source, but NOT GPL. I call it mehPL.
 *    I'm not too fond of long licenses at the top of the file.
 *    Please see the bottom.
 *    Enjoy!
 */













//seg_racer.c (uses rowSegBuffer):
//
//A Game! Ala "Racer" from the ol' TI-82 days...
// Use a potentiometer to try to keep the "car" on the race-track
// It gets harder the longer you stay on course!
// Demonstrates text-usage, using HFM to interpolate between distant points
// and more. Even at a refresh-rate of only 1/5th Hz, this game can get
// quite difficult.


#include _ADC_HEADER_
//   #include <stdio.h>
#include <stdlib.h>   //rand()


void racer_init(void)
{
	adc_takeInput(6);
	adc_init();
	adc_select(6);
}



//TODO: This has been implemented in commonCode's grabDecimal...
//For converting an integer to text-characters without using sprintf...
// Call once with the value to be written, and newVal TRUE
// then call repeatedly with the same value and newVal FALSE
// in order to grab each decimal digit in an entire uint32 from left to
// right.
//Returns 0-9 or -1 if we've reached 0...
int8_t grabDecimal_u32(uint32_t value, uint8_t newVal)
{
   static uint32_t divisor = 1000000000;

   static uint32_t thisVal;

   if(newVal)
   {
      divisor = 1000000000;
      thisVal = value;
   }

   uint8_t decimal = 0;



   if(divisor == 0)
      return -1;

   while(thisVal >= divisor)
   {
      decimal++;
      thisVal -= divisor;

   }




   divisor /= 10;

   return decimal;
}


//Including raceWidth, and maybe some buffer...
#define RACE_WIDTH_PSEGS   272


//Returns the number of psegs drawn...
uint8_t showScore(uint16_t rowNum, uint32_t score, uint8_t color)
{

   //The first few rows aren't displaying?
//      rowNum-=6;
      int8_t decimal;
      uint8_t scoreFound = FALSE;

      static uint32_t thisScore;

      if(rowNum == 0)
         thisScore = score; // goodCount;

      decimal = grabDecimal_u32(thisScore, 1);

      uint8_t col;

      uint8_t charRow = 0;

		uint8_t psegs = 0;

      for(col=0; col<NUM_PSEGS-3-RACE_WIDTH_PSEGS; col++)
      {
			psegs++;
//         uint8_t charRow = 0;
         
         if(col%8==0)
         {
            if((decimal != 0) && (decimal != -1))
               scoreFound = TRUE;

            if((decimal == -1) || (!scoreFound))
               charRow = 0;
            else
               charRow = getCharRow(decimal+'0', rowNum/2);
            decimal = grabDecimal_u32(thisScore, 0);
         }

         if(getbit(col%8, charRow))
            addSegfb(1,color);
         else
            addSegfb(1,_B);
      }

		return psegs;
}   



void racer_loadRow(uint16_t rowNum) 
{
  #define NEXT_COLOR   _R
  #define THIS_COLOR   _G
  #define OVERLAP_COLOR   _Y
  #define BG_COLOR   _C
  #define BG_BAD_COLOR   _M



   int16_t racerPseg = NUM_PSEGS/2 - 2;

   adc_startConversion();
   while(adc_isBusy())
      asm("nop;");
   racerPseg = (adc_getValue()>>2)+5;
   if(racerPseg > NUM_PSEGS-2)
      racerPseg = NUM_PSEGS-2;

   segClear();
   addSegfb(1, _W);
   static uint16_t raceWidth = 16;
   static uint8_t trackPosition = 0;
  #define TRACK_LENGTH   24   //24*32 = 768
  #define TRACK_ROWS   (V_COUNT/TRACK_LENGTH)
   static uint16_t raceTrack[TRACK_LENGTH*2];

   uint8_t bgColor = BG_COLOR;

   //racePseg += ((rand()&0xff0)>>4)*2/0xff - 1;

   uint8_t thisTrackRow = rowNum/TRACK_ROWS; //(V_COUNT/TRACK_LENGTH);
   uint8_t thisPosition = thisTrackRow + (trackPosition? TRACK_LENGTH : 0);
   uint8_t nextPosition = thisTrackRow + (trackPosition? 0 : TRACK_LENGTH);

   static hfm_t thisSmoothingHFM, nextSmoothingHFM;
   static uint8_t thisTrackGrowing, nextTrackGrowing;

/*
   static theta_t racerTheta = 0;
   if(racerTheta >= SINE_2PI)
      racerTheta -= SINE_2PI;

   racerPseg = sineRaw8(racerTheta) + -(INT8_MIN);
   racerTheta++;
*/

   uint8_t outOfTrack = FALSE;
   typedef struct _IST_BLAH_
   {
      uint16_t length;
      uint8_t color;
   }   intermediateSeg_t;

#define ISEGS_PER_WHATEVER   3
#define IS_SEGS (5*ISEGS_PER_WHATEVER)
   intermediateSeg_t is[IS_SEGS] = 
   { {0,_W}, {0,_W}, {0,_W}, {0,_W}, {0,_W},
     {0,_W}, {0,_W}, {0,_W}, {0,_W}, {0,_W},
     {0,_W}, {0,_W}, {0,_W}, {0,_W}, {0,_W} };

   uint8_t isPos = 0;

#define addSegIS(len, c) \
({\
    is[isPos].length = (len); \
    is[isPos].color = (c); \
    isPos+=ISEGS_PER_WHATEVER; \
    {}; \
})



   int8_t prevNextPosition;
   int8_t prevThisPosition;

   static uint16_t thisIS, nextIS;

   static uint8_t level = 1;


   if(rowNum%TRACK_ROWS == 0) //(V_COUNT/TRACK_LENGTH) == 0)
   {
      prevNextPosition = nextPosition - 1;
      if(prevNextPosition < 0)
         prevNextPosition = TRACK_LENGTH*2 - 1; //+prevNextPosition;

      prevThisPosition = thisPosition - 1;
      if(prevThisPosition < 0)
         prevThisPosition = TRACK_LENGTH*2 - 1; //+prevThisPosition;
      
      static theta_t racerTheta=0;

      int16_t nextTrackPseg;

      nextTrackPseg = sineRaw8(racerTheta) + -(INT8_MIN);
      racerTheta+=level;
      if(racerTheta >= SINE_2PI)
         racerTheta-=SINE_2PI;
/*
   int8_t randomVal = rand()&0x03;
   switch(randomVal)
   {
      case 1:
         if(nextTrackPseg < NUM_PSEGS-2-raceWidth)
            nextTrackPseg += 1;
         break;
      case 2:
         if(nextTrackPseg > 1)
            nextTrackPseg -= 1;
         break;
      default:
         break;
   }
*/


      raceTrack[nextPosition] = nextTrackPseg;

      thisIS = raceTrack[prevThisPosition];
      nextIS = raceTrack[prevNextPosition];
      uint8_t nextHFMdist, thisHFMdist;

      if(raceTrack[nextPosition] > nextIS)
      {
         nextTrackGrowing = TRUE;
         nextHFMdist = raceTrack[nextPosition] - nextIS;
      }
      else
      {
         nextTrackGrowing = FALSE;
         nextHFMdist = nextIS - raceTrack[nextPosition];
      }
      
      if(raceTrack[thisPosition] > thisIS)
      {
         thisTrackGrowing = TRUE;
         thisHFMdist = raceTrack[thisPosition] - thisIS;
      }
      else
      {
         thisTrackGrowing = FALSE;
         thisHFMdist = thisIS - raceTrack[thisPosition];
      }

      hfm_setup(&nextSmoothingHFM, nextHFMdist, TRACK_ROWS);

      hfm_setup(&thisSmoothingHFM, thisHFMdist, TRACK_ROWS);
   }


   if(thisTrackGrowing)//raceTrack[thisPosition] > raceTrack[prevThisPosition])
      thisIS += hfm_nextOutput(&thisSmoothingHFM);
   else
      thisIS -= hfm_nextOutput(&thisSmoothingHFM);

   if(nextTrackGrowing)//raceTrack[nextPosition] > raceTrack[prevNextPosition])
      nextIS += hfm_nextOutput(&nextSmoothingHFM);
   else
      nextIS -= hfm_nextOutput(&nextSmoothingHFM);

   if(thisIS < nextIS)
   {
      addSegIS(thisIS, bgColor);

      //Tracks overlap
      if(nextIS < thisIS + raceWidth)
      {
         addSegIS(nextIS - thisIS, THIS_COLOR);
         addSegIS(thisIS + raceWidth - nextIS, OVERLAP_COLOR);
         addSegIS(nextIS - thisIS, NEXT_COLOR);
         //outOfTrack=TRUE;
      }
      else
      {
         addSegIS(raceWidth, THIS_COLOR);
         addSegIS(nextIS - thisIS - raceWidth, bgColor);
         addSegIS(raceWidth, NEXT_COLOR);
      }

      //addSegIS(NUM_PSEGS-2-nextIS-raceWidth, bgColor);
      addSegIS(RACE_WIDTH_PSEGS-nextIS-raceWidth, bgColor);

   }
   else if(thisIS > nextIS)
   {
      addSegIS(nextIS, bgColor);

      //Tracks overlap
      if(thisIS < nextIS + raceWidth)
      {
         addSegIS(thisIS - nextIS, NEXT_COLOR);
         addSegIS(nextIS + raceWidth - thisIS, OVERLAP_COLOR);
         addSegIS(thisIS - nextIS, THIS_COLOR);
         //outOfTrack=TRUE;
      }
      else
      {
         addSegIS(raceWidth, NEXT_COLOR);
         addSegIS(thisIS - nextIS - raceWidth, bgColor);
         addSegIS(raceWidth, THIS_COLOR);
      }
   
      //addSegIS(NUM_PSEGS-2-thisIS-raceWidth, bgColor);
      addSegIS(RACE_WIDTH_PSEGS-thisIS-raceWidth, bgColor);
   }
   else //Positions equal
   {
      //outOfTrack=TRUE;
      addSegIS(thisIS, bgColor);

      addSegIS(raceWidth, OVERLAP_COLOR);
      //addSegIS(NUM_PSEGS-2-thisIS-raceWidth, bgColor);
      addSegIS(RACE_WIDTH_PSEGS-thisIS-raceWidth, bgColor);
   }
/*
addSegfb(raceWidth, _W);
   addSegfb(NUM_PSEGS-racePseg-raceWidth-2, _R);
*/
   uint8_t isSeg;
   uint16_t isSegTrackPos = 0;
   uint16_t oldTrackPos = 0;
   //uint8_t racerFound = FALSE;

   for(isSeg = 0; isSeg<IS_SEGS; isSeg++)
   {
      isSegTrackPos += is[isSeg].length;

      //if(!racerFound && 
      if((isSegTrackPos >= (uint16_t)racerPseg))
      {
         //racerFound = TRUE;
         uint16_t oldLength = is[isSeg].length;
         is[isSeg].length = racerPseg - oldTrackPos;
         is[isSeg+1].length = 1;
         is[isSeg+1].color = _K;
         //Not sure why this shouldn't be -1...
         is[isSeg+2].length = oldLength - is[isSeg].length; //- 1;
         is[isSeg+2].color = is[isSeg].color;
         if((is[isSeg].color != THIS_COLOR) && (is[isSeg].color != OVERLAP_COLOR))
            outOfTrack = TRUE;
         isSeg++;
         break;
      }
      oldTrackPos = isSegTrackPos;
   }

   
   for(isSeg = 0; isSeg<IS_SEGS; isSeg++)
     {
      if(is[isSeg].length == 0)
         continue;
      if(outOfTrack && (is[isSeg].color==BG_COLOR))
         addSegfb(is[isSeg].length, BG_BAD_COLOR);
      else
         addSegfb(is[isSeg].length, is[isSeg].color);
   }

   static uint32_t badCount=0;
   static uint32_t goodCount=0;

   if(outOfTrack)
      badCount++;
   else
      goodCount++;

   
   addSegfb(1, _B);


   uint8_t percentage = goodCount*100/(badCount+goodCount);
   static uint32_t lastLevelUpdate = 0;

   if((percentage > 50) && (goodCount - lastLevelUpdate > V_COUNT*3))
   {
      lastLevelUpdate = goodCount;
      level++;
   }

	uint8_t scorePsegs = 0;

   if((rowNum>=6) && (rowNum < 22))
      scorePsegs = showScore(rowNum-6, level, _C);

   else if((rowNum>=24) && (rowNum <40))
      scorePsegs = showScore(rowNum-24, goodCount, _G);
   else if((rowNum >= 42) && rowNum < 58)
      scorePsegs = showScore(rowNum-42, badCount, _R);
   else if((rowNum >= 60) && (rowNum < 76))
      scorePsegs = showScore(rowNum-60, percentage, _Y);

	//The addition of -scorePsegs, below, seems to have cleared up a weird
	// problem... the entire display looked right, except some of the
	// characters were printing incorrectly (as solid blocks, repeated first
	// row?)
	// Sometimes these extended-DE's (that extend past the screen)
	// cause weirdness like rows which print only half-way then repeat the 
	// previous row, for the second half.
	// Only odd thing remaining, is that it seems to be one pseg too short
	// (displaying a column of white next to the score)
	// Whatev.
	// Also, there's some oddities when viewing with PLL_SYSCLK
	// (weird green and magenta lines in the unwritten space)
	// Maybe related to similar issues..? ROW_CALC_DELAY too low?
	// Yeahp, ROW_CALCULATION_CYCS needs to be >20000, (currently 40000)
	//scorePsegs=0;
	// Actually, the column of white seems more related to calc-cycs too low
	// Actually, 40000 fixed both oddities, (and maybe woulda affected the
	// blockiness as well?)
	// Hard to test conclusively...
   //This should be varied according to the remaining space on the screen
   // in order to keep DE constant.
   // (it isn't currently, and in fact draws off-screen on showScore lines)
   addSegfb(NUM_PSEGS-2-RACE_WIDTH_PSEGS-scorePsegs, _B);

   addSegfb(1, _W);
   segTerminate();

   if(rowNum >= V_COUNT-1)
      trackPosition = !trackPosition;

}
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
 * /Users/meh/_avrProjects/LCDdirectLVDS/93-checkingProcessAgain/seg_racer.c
 *
 *    (Wow, that's a lot longer than I'd hoped).
 *
 *    Enjoy!
 */

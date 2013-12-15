
void segSine_loadRow(uint16_t rowNum) 
{
   static uint16_t lastThetaOffset = 0;
   static uint8_t frameCount = 0;

   if(rowNum == 0)
   {
      lastThetaOffset+=16;
      frameCount++;
   }
   //pgm_read_sine(sineTable[theta])

   segClear();
   //addSegfb(3,0x06, (6<<4) | 3);
//a/o v59-12ish addition of PLL_SYSCLK:
//Having a white border at the left-edge of the screen has been known to
// help syncing...
// Though it doesn't seem to be fixing the current problem.
// The problem is: Some rows are displaying long white-bars about half
//  of the displayed-area (~1/4 of the screen)
// I think I've seen this before.
#if(defined(PLL_SYSCLK) && PLL_SYSCLK)
   //This doesn't seem to matter...
   addSegfb(12, _W);
#else
   addSegfb(3, _W);
#endif

   uint8_t sineVal = (uint8_t)((int8_t)128 + 
         sineRaw8(rowNum+lastThetaOffset));

   uint8_t nextSineVal = (uint8_t)((int8_t)128 +
         sineRaw8(rowNum+1+lastThetaOffset));

	//WEIRD. gcc only started complaining about its being used uninitialized
	// a/o v60... I've been using it for ages!
   //uint8_t length; 
	uint8_t length = 1;

   //If the step-size is greater than one pixel, we'll see hashing
   // this should fix that.
   if(nextSineVal < sineVal)
   {
      length = (sineVal - nextSineVal);
      //This assures that the additional length of the white-segment
      // goes in the proper direction...
      sineVal-=length;//*4;
   }
   else if(nextSineVal > sineVal)
      length = (nextSineVal - sineVal);
   
	//This should be redundant a/o initializing...	
   //But we don't want zero-length white segments (where the sine values
   // are horizontal)
   //if(length == 0)
   //  length = 1;

   uint8_t rgbVal=0;
   uint8_t J=0;
   uint8_t j = frameCount;

#define WIDTH 5//3//4
   while(1)
   {
      J++;
      j++; // = J+frameCount;
      
      rgbVal = rgbGradient(j);

      if((uint16_t)J*(uint16_t)WIDTH>=sineVal)
         break;

      addSegfb(WIDTH, rgbVal);
   }

   J--;

   addSegfb(sineVal-(uint16_t)J*(uint16_t)WIDTH, rgbVal); 

   addSegfb(length, _W); 
   addSegfb(257-sineVal-length, rgbGradient(rowNum/8)); //rowNum/8);

   addSegfb(3, _W);

   //Draw a sideways character...
   {
      addSegfb(3, _R);

      uint8_t i;

      //Without + ' ' we get the same alphabet but shifted
      // starting with the lower-case letters which are shifted down a row
      // and some garbage in the top row...
      // now I get that I'm reading weird memory locations, but this
      // doesn't seem right. I should be accessing outside the array 
      // altogether, no?    
      char thisChar = (rowNum/8)%('~'-' '+1) + ' '; //pgm_read_byte(&(string[rowNum/8])); 
      //'A'; //string[rowNum/8];

      //Ironically, there are exactly the number of characters to be
      // displayed in 768 lines (plus one on the right, to center)
      for(i=8; i>0; i--)
      {
         if(getbit(rowNum%8, getCharRow(thisChar,i-1)))
            addSegfb(1, _Y); //0x06, (4<<4) | 3);
         else
            addSegfb(1, _R); //0x06, (4<<4) | 0);
      }

      addSegfb(3, _R); //0x06, (4<<4) | 0);

      addSegfb(1, _W); //0x04, (6<<4) | 3);

      //PGM_P charPtr = strchrnul_P(
      PGM_P tqbf = PSTR(" The Quick Brown Fox Jumped Over The Lazy Dog! \177"); 
      //, rowNum/8);

      strncpy_P(&thisChar, tqbf+(rowNum/8), 1);

      addSegfb(3, rgb(0,0,1));//_B); //0, (6<<4) | 0);

      if(rowNum/8 < strlen_P(tqbf))
      {
         for(i=8; i>0; i--)
         {
            if(getbit(rowNum%8, getCharRow(thisChar,i-1)))
               addSegfb(1, rgb(3,0,1));//_C); //0, (6<<4) | 3);
            else
               addSegfb(1, rgb(0,0,1));//_B); //0, (6<<4) | 0);
         }
      }
      else
         addSegfb(8, rgb(0,0,1));//_B); //0,(6<<4) | 0);

      addSegfb(3, rgb(0,0,1));//_B); //0,(6<<4)|0);

   }

   //newSeg(3,0x06,(4<<4) | 0);

   uint8_t i;
   for(i=0; i<64; i++)
      addSegfb(1, (i+rowNum/4)&0x3f);

   addSegfb(3, _W); //0x06, (6<<4) | 3);

   segTerminate();
}

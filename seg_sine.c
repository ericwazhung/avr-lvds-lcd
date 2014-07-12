/* mehPL:
 *    This is Open Source, but NOT GPL. I call it mehPL.
 *    I'm not too fond of long licenses at the top of the file.
 *    Please see the bottom.
 *    Enjoy!
 */











//seg_sine.c (uses rowSegBuffer):
//
//A very nice test-pattern... shows a sine-wave, the under-side of which
// is horizontal color-bars, above it is vertical color-bars. 
// Above that is two lines of text, showing all available characters
// And above that is color-patterns using up the remaining Row-Segments
// This is meant to be rotated 90-degrees
// Like SEG_HFM, it might be handy to adjust NUM_SEGMENTS for experimenting
// Note that color-segments which are too long to be stored in a single
// row-segment are automatically put into the next.
// And that existing segments are automatically stretched if the next-added
// segment is the same color.
// This displays all available colors and shows the resolution capabilities



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
 * /Users/meh/_avrProjects/LCDdirectLVDS/93-checkingProcessAgain/seg_sine.c
 *
 *    (Wow, that's a lot longer than I'd hoped).
 *
 *    Enjoy!
 */

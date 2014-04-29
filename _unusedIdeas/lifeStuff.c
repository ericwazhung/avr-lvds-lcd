/* mehPL:
 *    This is Open Source, but NOT GPL. I call it mehPL.
 *    I'm not too fond of long licenses at the top of the file.
 *    Please see the bottom.
 *    Enjoy!
 */






static __inline__ \
void life_drawRow(uint8_t rowNum, uint8_t rowBuffer[]) \
		__attribute__((__always_inline__));



#define LIFE_HEIGHT 15
#define LIFE_WIDTH  40
#define LIFE_WIDTH_BYTES   ((LIFE_WIDTH+7)/8)
#if (LIFE_WIDTH != 40)
#error "LIFE_WIDTH must be 40, or significant changes are necessary"
#endif

//This is for parsing a 40bit value into uint8_t's 
// suitable for array initialization
#define bin40 reverseBin40toU8s

//Gosper Glider Gun:
uint8_t lifeFrame[LIFE_HEIGHT][LIFE_WIDTH_BYTES] =
{
   bin40(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
   bin40(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
   bin40(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
   bin40(0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0),
   bin40(0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0),
   bin40(0,1,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
   bin40(0,1,1,0,0,0,0,0,0,0,0,1,0,0,0,1,0,1,1,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
   bin40(0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
   bin40(0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
   bin40(0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
   bin40(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
   {0,0,0,0,0},//A few extra rows for the glider to clear the others' paths
   {0,0,0,0,0},
   {0,0,0,0,0},
   {0,0,0,0,0}
};







//This assumes rowBuffer[] points to the first column we should draw
// it could be called as, e.g. life_drawRow(rbRowNum, &RealRowBuffer[5])
// to shift it right five pixels...
// rowNum corresponds to the row in drawable pixels
// (though this may change later... for, e.g. high vertical resolution)
void life_drawRow(uint8_t rowNum, uint8_t rowBuffer[])
{
   if(rowNum < LIFE_HEIGHT)
   {  
			uint8_t i;

			//for(i=RB_WIDTH-LIFE_WIDTH; i<RB_WIDTH; i++)
         for(i=0; i<LIFE_WIDTH; i++)
         {
            if(getbit(i&0x07, lifeFrame[rowNum][i>>3]))
               rowBuffer[i] = fb_to_rb(0);
         }
   }  

}




void lifeRound(void)
{
   //initImage = 0;
   //Making this static only so it will show up in the
   // memory-use listing (in AVR)...
   //+7 probably not necessary...
   static uint8_t oldFrameBits[LIFE_HEIGHT][LIFE_WIDTH_BYTES];
   
   uint8_t row;
   uint8_t col;
   
   for(row=0; row<LIFE_HEIGHT; row++)
   for(col=0; col<LIFE_WIDTH_BYTES; col++)
      oldFrameBits[row][col] = lifeFrame[row][col];
   //   writebit(col&0x07,oldFrameBits[row][col>>3],
   //       (lifeFrame[row][col] != 0x10)) 
   //      );
   
   
   for(row=0; row<LIFE_HEIGHT; row++)
   {
   for(col=0; col<LIFE_WIDTH; col++)
   {
      uint8_t nearSum=0;
      uint8_t rn, cn;
      //Test nearby pixels...
      for( rn = ( (row>0) ? (row-1) : 0 ) ;
           rn <= ( (row<LIFE_HEIGHT-1) ? (row+1) : (LIFE_HEIGHT-1) );
           rn++) 
      {    
         for( cn = ( (col>0) ? (col-1) : 0 ) ;
              cn <= ( (col<LIFE_WIDTH-1) ? (col+1) : (LIFE_WIDTH-1) );
              cn++) 
         {    
            if((cn != col) || (rn != row))
               nearSum += getbit(cn&0x07, oldFrameBits[rn][cn>>3]);
         }     
      }  
      
      //Live Cells:
      if(getbit(col&0x07,oldFrameBits[row][col>>3]))
      {
         if(nearSum < 2)
            clrbit(col&0x07, lifeFrame[row][col>>3]); //0x01; //DIM red = lonlyDead //_K;
         else if(nearSum > 3)
            clrbit(col&0x07, lifeFrame[row][col>>3]); //0x10; //DIM blue =overpopDead_K;
         else //2,3
         {
            setbit(col&0x07, lifeFrame[row][col>>3]); //redundant, should already be 1 
            /*
            if(nearSum == 2)
               frameBuffer[row][col] = 0x0C | 0x01; //Good, but a little lo
nely
            else
               frameBuffer[row][col] = 0x0C | 0x10; //_R;//oldFrameBits[row
][col];
            */
         }
      }  
      //Empty Cells:
      else if(nearSum == 3)
         setbit(col&0x07, lifeFrame[row][col>>3]);    //BIRTH!
      //   frameBuffer[row][col] = 0x0C | 0x01 | 0x10; //BIRTH! //_R;
      else
         clrbit(col&0x07, lifeFrame[row][col>>3]);
      // frameBuffer[row][col] = Tr; //_K;
   }  
   }  
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
 * /Users/meh/_avrProjects/LCDdirectLVDS/68-backToLTN/_unusedIdeas/lifeStuff.c
 *
 *    (Wow, that's a lot longer than I'd hoped).
 *
 *    Enjoy!
 */

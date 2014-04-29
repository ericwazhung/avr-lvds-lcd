/* mehPL:
 *    This is Open Source, but NOT GPL. I call it mehPL.
 *    I'm not too fond of long licenses at the top of the file.
 *    Please see the bottom.
 *    Enjoy!
 */






//Please see writeColor.c


#if(defined(FB_REFRESH_ON_CHANGE) && FB_REFRESH_ON_CHANGE)
extern volatile uint8_t updateFrame;
#endif

//drawPix from program memory
//a/0 v60: Again, unused for its original purpose, but tightly intertwined
// in old code that's still being (mis)used...
#define FB_WIDTH 16
#define FB_HEIGHT 16
uint8_t frameBuffer[FB_HEIGHT][FB_WIDTH];



//AHHH it wasn't IMAGE_BUFFER that's entangled, but IMAGE_CHANGE
// which is even weirder.

//This was #if'd into existence with IMAGE_BUFFER, but since it's only been
// used with frameBuffer, and since IMAGE_BUFFER is somewhat entangled with
// newer stuff, I'm putting it here and NOT including this file, (yet)
void setColor(uint8_t red, uint8_t green, uint8_t blue, 
               uint8_t row, uint8_t col)
{
   red = (red > 3) ? 3 : red;
   green = (green > 3) ? 3 : green;
   blue = (blue > 3) ? 3 : blue;

   frameBuffer[row][col] = red | ((green)<<2) | ((blue)<<4);
}




//Called as: pgm_readImageByte(pgm_image1, row, col)
#define pgm_readImageByte(image, row, col)   \
      pgm_read_byte((uint8_t *)(&((image)[(row)*FB_WIDTH+(col)])))




// This stuff was located in main() before the while loop...
// #if !COLOR_BAR_SCROLL... (which is *nearly every case* including 
//  RowSegBuffer, etc)
// it's long-since been commented-out...
void frameBufferInit(void)
{
	 //uint8_t frameBuffer[16][16];
/*   for(r=0; r<FB_HEIGHT; r++)
   {
      for(c=0; c<FB_WIDTH; c++)
      {
         uint8_t imagePixel=pgm_readImageByte(pgm_image1, r, c);

         setColor(
            (getbit(RED_IMAGEVAL_BIT, imagePixel) ? 0x03 : 0),
            (getbit(GREEN_IMAGEVAL_BIT, imagePixel) ? 0x0C : 0),
            (getbit(BLUE_IMAGEVAL_BIT, imagePixel) ? 0x30 : 0),
            r, c);
//         setBlue(
//               (
//   (getbit(BLUE_IMAGEVAL_BIT, imagePixel) ? (60<<2) : 0) |
//   (getbit(GREEN_IMAGEVAL_BIT, imagePixel) ? (1<<GREEN_BLUEVAL_BIT) : 0) |
//   (getbit(RED_IMAGEVAL_BIT, imagePixel) ? (1<<RED_BLUEVAL_BIT) : 0)
//               ), r, c);
      }
   }
*/

}


//#include "_options/smiley.c"


static __inline__
void frameBufferChange(uint8_t *bufferChanged, 
							uint8_t row, uint8_t col, uint8_t color)
	__attribute__((__always_inline__));


//This isn't especially necessary to write to the frameBuffer
// ...since it's global, it could be done directly
// but this allows for testing whether a change occurs
// in case it's desired to only refresh when that happens.
//Since it's always-inline, it should optimize-out a few things, e.g. the
//NULL-test, and even the write to bufferChanged, if it is null.
// so it should be pretty quick.
// It does NOT have a return-value, instead it only modifies bufferChanged
// IF there is a change, so e.g. it could be run in a for-loop
// and the end-value of bufferChanged will either be FALSE if no change, or
// TRUE if there was a change in any of the loops.
void frameBufferChange(uint8_t *bufferChanged, 
							uint8_t row, uint8_t col, uint8_t color)
{
	if(frameBuffer[(row)][(col)] != (color))
	{
		frameBuffer[(row)][(col)] = (color);
		
		if(bufferChanged != NULL)
			*bufferChanged = TRUE;
	}
}

#define frameBufferSet(row, col, color) \
				frameBufferChange(NULL, (row), (col), (color))


#if(defined(FB_SMILEY) && FB_SMILEY)
#include "_options/smiley.c"
#endif

// This code was in main...
// This isn't generalized enough to justify this function-name
// just not ready to delete it completely
// Returns TRUE if there's a change to the frameBuffer image
// (so a refresh can be scheduled, if so desired)

void frameBufferUpdate(void)
{
 #if(defined(FB_REFRESH_ON_CHANGE) && FB_REFRESH_ON_CHANGE)
      static uint8_t lastUpdated = FALSE;
      static dms4day_t fbLastUpdateTime = 0;

      //Because the dmsTimer is running somewhat arbitarily, this "200ms"
      // isn't particularly accurate...
      if(updateFrame)
         fbLastUpdateTime = dmsGetTime();
      else  //!updateFrame
      {
         if(dmsIsItTime(&fbLastUpdateTime, 100*DMS_MS))
         {
            if(fb_updater())
            {
               updateFrame = TRUE;
               lastUpdated = TRUE;
            }
         }
         //Attempt to get double-refresh on stationary images
         // to overcome the previous image
         else if(lastUpdated)
         {
            updateFrame = TRUE;
            lastUpdated = FALSE;
         }
      }
 #else
      if(isNewFrame())
      {
         fb_updater();
      }
 #endif

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
 * /Users/meh/_avrProjects/LCDdirectLVDS/68-backToLTN/_options/frameBuffer.c
 *
 *    (Wow, that's a lot longer than I'd hoped).
 *
 *    Enjoy!
 */

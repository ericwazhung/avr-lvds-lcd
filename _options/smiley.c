/* mehPL:
 *    This is Open Source, but NOT GPL. I call it mehPL.
 *    I'm not too fond of long licenses at the top of the file.
 *    Please see the bottom.
 *    Enjoy!
 */










//Moved Above
//Called as: pgm_readImageByte(pgm_image1, row, col)
//#define pgm_readImageByte(image, row, col)   
//   pgm_read_byte((uint8_t *)(&((image)[(row)*FB_WIDTH+(col)])))

#if( !defined(COLOR_BAR_SCROLL) || !COLOR_BAR_SCROLL)
#if ( (FB_WIDTH != 16) || (FB_HEIGHT != 16) )
 #error "image1 dimensions don't match FB_HEIGHT/WIDTH"
 #error "It's likely I just need to create a specific DRAWIMAGE definition and tests"
#endif

#define RED_IMAGEVAL_BIT 0
#define GREEN_IMAGEVAL_BIT 1
#define BLUE_IMAGEVAL_BIT 2



/*
#define _R   0x03
#define _O   0x07
#define _G 0x0C
#define _B 0x30
#define _V 0x13
#define _Y   0x0F
#define _C 0x3C
#define _M   0x33
#define _K   0x00
#define _W 0x3F
#define _r 0x01
#define _g 0x04
#define _b 0x10
#define _c 0x14
#define _m 0x11
#define _y 0x05
#define _k 0x15
#define Tr 0xC0   //Transparet
*/
#if (TRUE || (!defined(LIFE) || !LIFE))
const uint8_t pgm_image1[256] PROGMEM =
{
   Tr,Tr,Tr,Tr,Tr,_K,_K,_K,_K,_K,_K,Tr,Tr,Tr,Tr,Tr,
   Tr,Tr,Tr,_K,_K,_Y,_Y,_Y,_Y,_Y,_Y,_K,_K,Tr,Tr,Tr,
   Tr,Tr,_K,_K,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_K,_K,Tr,Tr,
   Tr,_K,_K,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_K,_K,Tr,
   Tr,_K,_Y,_Y,_Y,_K,_Y,_Y,_Y,_Y,_K,_Y,_Y,_Y,_K,Tr,
   _K,_Y,_Y,_Y,_K,_K,_K,_Y,_Y,_K,_K,_K,_Y,_Y,_Y,_K,
   _K,_Y,_Y,_Y,_Y,_K,_Y,_Y,_Y,_Y,_K,_Y,_Y,_Y,_Y,_K,
   _K,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_K,
   _K,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_K,
   _K,_Y,_Y,_K,_K,_Y,_Y,_Y,_Y,_Y,_Y,_K,_K,_Y,_Y,_K,
   _K,_Y,_Y,_Y,_K,_K,_Y,_Y,_Y,_Y,_K,_K,_Y,_Y,_Y,_K,
   Tr,_K,_Y,_Y,_Y,_K,_K,_K,_K,_K,_K,_Y,_Y,_Y,_K,Tr,
   Tr,_K,_K,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_K,_K,Tr,
   Tr,Tr,_K,_K,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_K,_K,Tr,Tr,
   Tr,Tr,Tr,_K,_K,_Y,_Y,_Y,_Y,_Y,_Y,_K,_K,Tr,Tr,Tr,
   Tr,Tr,Tr,Tr,Tr,_K,_K,_K,_K,_K,_K,Tr,Tr,Tr,Tr,Tr
};
#else
/*uint8_t pgm_image1[256] PROGMEM =
{  
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};
*/
/*
{  
   _K,_K,_K,_K,_K,_K,_K,_K,_K,_K,_K,_K,_K,_K,_K,_K,
   _K,_K,_K,_K,_K,_K,_K,_K,_K,_K,_K,_K,_K,_K,_K,_K,
   _K,_K,_K,_K,_K,_K,_K,_K,_K,_K,_K,_K,_K,_K,_K,_K,
   _K,_K,_K,_K,_K,_R,_R,_R,_K,_K,_K,_K,_K,_K,_K,_K,
   _K,_K,_K,_K,_K,_K,_K,_K,_K,_K,_K,_K,_K,_K,_K,_K,
   _K,_K,_K,_K,_K,_K,_K,_K,_K,_K,_K,_K,_K,_K,_K,_K,
   _K,_K,_K,_K,_K,_K,_K,_K,_K,_K,_K,_R,_R,_K,_K,_K,
   _K,_K,_K,_K,_K,_K,_K,_K,_K,_K,_K,_R,_R,_K,_K,_K,
   _K,_K,_K,_K,_R,_R,_R,_K,_K,_K,_K,_K,_K,_K,_K,_K,
   _K,_K,_K,_K,_K,_R,_R,_R,_K,_K,_K,_K,_K,_K,_K,_K,
   _K,_K,_K,_K,_K,_K,_K,_K,_K,_K,_K,_R,_R,_R,_K,_K,
   _K,_K,_K,_K,_K,_K,_K,_K,_K,_K,_K,_R,_K,_K,_K,_K,
   _K,_K,_K,_K,_K,_K,_K,_K,_K,_K,_K,_K,_R,_K,_K,_K,
   _K,_K,_K,_K,_K,_K,_K,_K,_K,_K,_K,_K,_K,_K,_K,_K,
   _K,_K,_K,_K,_K,_K,_K,_K,_K,_K,_K,_K,_K,_K,_K,_K,
   _K,_K,_K,_K,_K,_K,_K,_K,_K,_K,_K,_K,_K,_K,_K,_K
};
*/



#endif


const uint8_t pgm_image2[256] PROGMEM =
{
   Tr,Tr,Tr,Tr,Tr,_K,_K,_K,_K,_K,_K,Tr,Tr,Tr,Tr,Tr,
   Tr,Tr,Tr,_K,_K,_Y,_Y,_Y,_Y,_Y,_Y,_K,_K,Tr,Tr,Tr,
   Tr,Tr,_K,_K,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_K,_K,Tr,Tr,
   Tr,_K,_K,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_K,_K,Tr,
   Tr,_K,_Y,_Y,_Y,_K,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_K,Tr,
   _K,_Y,_Y,_Y,_K,_K,_K,_Y,_Y,_K,_Y,_Y,_Y,_Y,_Y,_K,
   _K,_Y,_Y,_Y,_Y,_K,_Y,_Y,_Y,_Y,_K,_K,_Y,_Y,_Y,_K,
   _K,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_K,
   _K,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_Y,_K,
   _K,_Y,_Y,_K,_K,_Y,_Y,_Y,_Y,_Y,_Y,_K,_K,_Y,_Y,_K,
   _K,_Y,_Y,_Y,_K,_K,_Y,_Y,_Y,_Y,_K,_K,_Y,_Y,_Y,_K,
   Tr,_K,_Y,_Y,_Y,_K,_K,_K,_K,_K,_K,_Y,_Y,_Y,_K,Tr,
   Tr,_K,_K,_Y,_Y,_Y,_K,_R,_K,_R,_K,_Y,_Y,_K,_K,Tr,
   Tr,Tr,_K,_K,_Y,_Y,_K,_R,_R,_R,_K,_Y,_K,_K,Tr,Tr,
   Tr,Tr,Tr,_K,_K,_Y,_Y,_K,_K,_K,_Y,_K,_K,Tr,Tr,Tr,
   Tr,Tr,Tr,Tr,Tr,_K,_K,_K,_K,_K,_K,Tr,Tr,Tr,Tr,Tr
};
#endif //COLOR_BAR_SCROLL

/*
#if (IMAGE_WIDTH == 21)
uint8_t mainData[2][768>>5][IMAGE_WIDTH] =
{
   {
   {6,5,4,3,2,1,6,5,4,3,2,1,6,5,4,3,2,1,6,5,4},
   {5,4,3,2,1,6,0,0,0,0,0,0,0,0,0,2,1,6,5,4,3},
   {4,3,2,1,0,0,0,3,3,3,3,3,3,3,0,0,0,5,4,3,4},
   {3,2,1,0,0,3,3,3,3,3,3,3,3,3,3,3,0,0,3,2,1},
   {2,1,0,0,3,3,3,3,3,3,3,3,3,3,3,3,3,0,0,1,6},
   {1,0,0,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,0,0,5},
   {6,0,3,3,3,3,3,0,3,3,3,3,0,3,3,3,3,3,3,0,4},
   {0,0,3,3,3,3,0,0,0,3,3,0,0,0,3,3,3,3,3,0,0},
   {0,3,3,3,3,3,0,0,0,3,3,0,0,0,3,3,3,3,3,3,0},
   {0,3,3,3,3,3,3,0,3,3,3,3,0,3,3,3,3,3,3,3,0},
   {0,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,0},
   {0,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,0},
   {0,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,0},
   {0,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,0},
   {0,3,3,3,3,0,0,3,3,3,3,3,3,3,0,0,3,3,3,3,0},
   {0,0,3,3,3,3,0,0,3,3,3,3,3,0,0,3,3,3,3,0,0},
   {6,0,3,3,3,3,3,0,0,0,0,0,0,0,3,3,3,3,3,0,4},
   {5,0,0,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,0,0,3},
   {4,3,0,0,3,3,3,3,3,3,3,3,3,3,3,3,3,0,0,3,2},
   {3,2,6,0,0,3,3,3,3,3,3,3,3,3,3,3,0,0,3,2,1},
   {2,1,5,5,0,0,0,3,3,3,3,3,3,3,0,0,0,3,2,1,6},
   {1,6,3,4,3,2,0,0,0,0,0,0,0,0,0,4,3,2,1,6,5},
   {6,5,4,3,2,1,6,5,4,3,2,1,6,5,4,3,2,1,6,5,4},
   {5,4,3,2,1,6,5,4,3,2,1,6,5,4,3,2,1,6,5,4,3}
   },
   {   
   {5,4,3,2,1,6,5,4,3,2,1,6,5,4,3,2,1,6,5,4,3},
   {4,3,2,1,6,5,0,0,0,0,0,0,0,0,0,1,6,5,4,3,2},
   {3,2,1,6,0,0,0,3,3,3,3,3,3,3,0,0,0,4,3,2,1},
   {2,1,6,0,0,3,3,3,3,3,3,3,3,3,3,3,0,0,2,1,6},
   {1,6,0,0,3,3,3,3,3,3,3,3,3,3,3,3,3,0,0,6,5},
   {6,0,0,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,0,0,4},
   {5,0,3,3,3,3,3,0,3,3,3,3,3,3,3,3,3,3,3,0,3},
   {0,0,3,3,3,3,0,0,0,3,3,0,3,3,3,3,3,3,3,0,0},
   {0,3,3,3,3,3,0,0,0,3,3,3,0,0,0,3,3,3,3,3,0},
   {0,3,3,3,3,3,3,0,3,3,3,3,3,3,3,3,3,3,3,3,0},
   {0,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,0},
   {0,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,0},
   {0,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,0},
   {0,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,0},
   {0,3,3,3,3,0,0,3,3,3,3,3,3,3,0,0,3,3,3,3,0},
   {0,0,3,3,3,3,0,0,3,3,3,3,3,0,0,3,3,3,3,0,0},
   {5,0,3,3,3,3,3,0,0,0,0,0,0,0,3,3,3,3,3,0,3},
   {4,0,0,3,3,3,3,3,3,0,1,0,1,0,3,3,3,3,0,0,2},
   {3,2,0,0,3,3,3,3,3,0,1,1,1,0,3,3,3,0,0,2,1},
   {2,1,6,0,0,3,3,3,3,3,0,0,0,3,3,3,0,0,2,1,6},
   {1,6,5,4,0,0,0,3,3,3,3,3,3,3,0,0,0,2,1,6,5},
   {6,5,4,3,2,1,0,0,0,0,0,0,0,0,0,3,2,1,6,5,4},
   {5,4,3,2,1,6,5,4,3,2,1,6,5,4,3,2,1,6,5,4,3},
   {4,3,2,1,6,5,4,3,2,1,6,5,4,3,2,1,6,5,4,3,2}
   }
};
#else
   #error
#endif
*/


//Call this often, and the framebuffer will alternate between two
//smiley-faces. Also, it places a gradient in the background, which scrolls
//over time.
//Returns TRUE if there's a change to the frameBuffer image
uint8_t smileyUpdate(void)
{
   uint8_t imageChanged = FALSE;

   //Here's where it alternated pimage to point to alternate images
   //This code worked alongside rowBuffer, via fb_to_rb, I guess
   // since tetUpdate, and hexColor, etc. were displayed alongside
   // at one point...
   // Far too convoluted to piece together in my mind anymore

  #if 0
   tetUpdate();
  #endif

   static const uint8_t *pimage = pgm_image1;
   static uint8_t imageNum = 0;
   static uint16_t updateCount = 0;
   static uint8_t colorShift = 0;

   //colorShift++;

   updateCount++;

   if(updateCount % 32 == 0)
      colorShift++;

   if(updateCount == 64) //1000)
   {
      updateCount = 0;

      if(imageNum == 0)
      {
         imageNum = 1;
         pimage = pgm_image1;
         //   tetColorScheme = 1;
      }
      else
      {
         //   hexColor++;
         //   hexColor&=0x3f;
         imageNum = 0;
         pimage = pgm_image2;
         //   tetColorScheme = 0;
      }
   }

   uint8_t row, col; //, colorShift=0;

   for(row=0; row<FB_HEIGHT; row++)
      for(col=0; col<FB_WIDTH; col++)
      {
         uint8_t imagePixel=pgm_readImageByte(pimage, row, col);

   
         if(imagePixel == Tr)
            frameBufferChange(&imageChanged, row, col,
                                          colorShift+row+col);
         else
            frameBufferChange(&imageChanged, row, col, imagePixel);
      }  

	return imageChanged;
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
 * /Users/meh/_avrProjects/LCDdirectLVDS/90-reGitting/_options/smiley.c
 *
 *    (Wow, that's a lot longer than I'd hoped).
 *
 *    Enjoy!
 */

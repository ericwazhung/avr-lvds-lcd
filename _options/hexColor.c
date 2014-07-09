/* mehPL:
 *    This is Open Source, but NOT GPL. I call it mehPL.
 *    I'm not too fond of long licenses at the top of the file.
 *    Please see the bottom.
 *    Enjoy!
 */







#if(defined(FRAMEBUFFER_TESTING) && FRAMEBUFFER_TESTING)
//__static __inline__
//This formatting stolen from tetStuff a/o v69... NYfullyI
 extern uint8_t frameBuffer[FB_HEIGHT][FB_WIDTH];
 #define hexColor_setPixel(col, color) \
	rowBuffer[(col)] = (color)

 #if(defined(FB_REFRESH_ON_CHANGE) && FB_REFRESH_ON_CHANGE)
  #error "hexColor doesn't yet have FB_REFRESH_ON_CHANGE implemented... disable it or code it :)"
#else
 #define hexColor_setPixel(col, color) \
	rowBuffer[(col)] = fb_to_rb(color)
#endif

uint8_t hexColor = 0;

static __inline__ \
void hexColor_drawRow(uint8_t rbRowNum, uint8_t rowBuffer[]) \
     __attribute__((__always_inline__));

//Takes a nibble and makes it a hex character...

#if(defined(FRAMEBUFFER_TESTING) && FRAMEBUFFER_TESTING)
 #define HEXCOLOR_TOPROW 0
#else
 #define HEXCOLOR_TOPROW 32
#endif

void hexColor_drawRow(uint8_t rbRowNum, uint8_t rowBuffer[])
{
//   static uint8_t color = 0;

   if( (rbRowNum >= HEXCOLOR_TOPROW) &&
       (rbRowNum < HEXCOLOR_TOPROW+8) )
   {
      uint8_t charRow1, charRow2;
      //uint8_t color;

      charRow1 = 
         getCharRow(nibbletochar((hexColor)>>4), rbRowNum-HEXCOLOR_TOPROW);
      charRow2 = 
         getCharRow(nibbletochar(hexColor&0x0f), rbRowNum-HEXCOLOR_TOPROW);

      uint8_t col;
      for(col=0; col<8; col++)
      {
         if(getbit(col, charRow1))
				hexColor_setPixel(col+1, hexColor);
            //rowBuffer[col+1] = fb_to_rb(hexColor);
         //else
         //   rowBuffer[col] = fb_to_rb(0x00);

         if(getbit(col, charRow2))
				hexColor_setPixel(col+8+1, hexColor);
            //rowBuffer[col+8+1] = fb_to_rb(hexColor);
      }
   }
}

#if(defined(FRAMEBUFFER_TESTING) && FRAMEBUFFER_TESTING)
void hexColor_update(void)
{
	uint8_t row;


//	static tcnter_t startTime = 0;

//	if(tcnter_isItTime(&startTime, 5*TCNTER_SEC))
	static dms4day_t startTime = 0;

	if(dmsIsItTime(&startTime, 5*DMS_SEC))
	{
		hexColor++;
		hexColor &= 0x3f;	//There are only two bits per R/G/B so 3f is the
								//largest color value...
								//(Technically, the other two bits are
								// alpha-values, in some modes, but that's not
								// really well implemented)
	}


	for(row=0; row<FB_HEIGHT; row++)
	{
		hexColor_drawRow(row, frameBuffer[row]);
	}
}
#else
 #error "hexColor NYreI for non FrameBuffer mode"
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
 * /Users/meh/_avrProjects/LCDdirectLVDS/90-reGitting/_options/hexColor.c
 *
 *    (Wow, that's a lot longer than I'd hoped).
 *
 *    Enjoy!
 */

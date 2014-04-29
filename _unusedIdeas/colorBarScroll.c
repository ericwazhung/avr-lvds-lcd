/* mehPL:
 *    This is Open Source, but NOT GPL. I call it mehPL.
 *    I'm not too fond of long licenses at the top of the file.
 *    Please see the bottom.
 *    Enjoy!
 */






//ColorBarScroll was an early-early test, I think before even LVDS...
// It's also highly-intertwined with PIXEL_SCROLL, IMAGE_CHANGE, SMILEY...
// which also are old...


//#define COLOR_BAR_SCROLL TRUE
#if (defined(COLOR_BAR_SCROLL) && COLOR_BAR_SCROLL)

//#warning "COLOR_BAR_SCROLL is broken a/o v29, possibly earlier"
// Probably just a timing issue...

//Optional, but only useful here...
 #define PIXEL_SCROLL TRUE

 //Handles all the displayable colors in a single row for scrolling...
 #define FB_WIDTH 64 //28
 //Two different arrangements of colors...
 #define FB_HEIGHT 2

#else //drawPix from program memory
 #define FB_WIDTH   16
 #define FB_HEIGHT 16
#endif



void colorBarScrollInit(void) {
   
/*   uint8_t frameBuffer[FB_HEIGHT][FB_WIDTH] = 
   { {
      (0<<2), 
         (0<<2) | (1<<RED_BLUEVAL_BIT), (0<<2) | (1<<GREEN_BLUEVAL_BIT),
         (1<<RED_BLUEVAL_BIT) | (0<<2) | (1<<GREEN_BLUEVAL_BIT),
      (4<<2), 
         (4<<2) | (1<<RED_BLUEVAL_BIT), (4<<2) | (1<<GREEN_BLUEVAL_BIT),
         (1<<RED_BLUEVAL_BIT) | (4<<2) | (1<<GREEN_BLUEVAL_BIT),
      (12<<2), 
         (12<<2) | (1<<RED_BLUEVAL_BIT), (12<<2) | (1<<GREEN_BLUEVAL_BIT),
         (1<<RED_BLUEVAL_BIT) | (12<<2) | (1<<GREEN_BLUEVAL_BIT),
      (32<<2), 
         (32<<2) | (1<<RED_BLUEVAL_BIT), (32<<2) | (1<<GREEN_BLUEVAL_BIT),
         (1<<RED_BLUEVAL_BIT) | (32<<2) | (1<<GREEN_BLUEVAL_BIT),
      (36<<2), 
         (36<<2) | (1<<RED_BLUEVAL_BIT), (36<<2) | (1<<GREEN_BLUEVAL_BIT),
         (1<<RED_BLUEVAL_BIT) | (36<<2) | (1<<GREEN_BLUEVAL_BIT),
      (44<<2), 
         (44<<2) | (1<<RED_BLUEVAL_BIT), (44<<2) | (1<<GREEN_BLUEVAL_BIT),
         (1<<RED_BLUEVAL_BIT) | (44<<2) | (1<<GREEN_BLUEVAL_BIT),
      (60<<2),
           (60<<2) | (1<<RED_BLUEVAL_BIT), (60<<2) | (1<<GREEN_BLUEVAL_BIT),
         (1<<RED_BLUEVAL_BIT) | (60<<2) | (1<<GREEN_BLUEVAL_BIT)
     },
     {
      (0<<2) | (1<<RED_BLUEVAL_BIT), (4<<2) | (1<<RED_BLUEVAL_BIT),
        (12<<2) | (1<<RED_BLUEVAL_BIT), (32<<2) | (1<<RED_BLUEVAL_BIT),
        (36<<2) | (1<<RED_BLUEVAL_BIT), (44<<2) | (1<<RED_BLUEVAL_BIT),
      (60<<2) | (1<<RED_BLUEVAL_BIT),
      
      (0<<2) | (1<<GREEN_BLUEVAL_BIT), (4<<2) | (1<<GREEN_BLUEVAL_BIT), 
      (12<<2) | (1<<GREEN_BLUEVAL_BIT), (32<<2) | (1<<GREEN_BLUEVAL_BIT), 
      (36<<2)| (1<<GREEN_BLUEVAL_BIT), (44<<2) | (1<<GREEN_BLUEVAL_BIT),
      (60<<2) | (1<<GREEN_BLUEVAL_BIT), 
      
      (0<<2), (4<<2), (12<<2),(32<<2), (36<<2), (44<<2), (60<<2),

      (0<<2) | (1<<RED_BLUEVAL_BIT) | (1<<GREEN_BLUEVAL_BIT),
        (4<<2) | (1<<RED_BLUEVAL_BIT) | (1<<GREEN_BLUEVAL_BIT),
      (12<<2) | (1<<RED_BLUEVAL_BIT) | (1<<GREEN_BLUEVAL_BIT),
        (32<<2) | (1<<RED_BLUEVAL_BIT) | (1<<GREEN_BLUEVAL_BIT),
      (36<<2) | (1<<RED_BLUEVAL_BIT) | (1<<GREEN_BLUEVAL_BIT),
        (44<<2) | (1<<RED_BLUEVAL_BIT) | (1<<GREEN_BLUEVAL_BIT),
      (60<<2) | (1<<RED_BLUEVAL_BIT) | (1<<GREEN_BLUEVAL_BIT)

       }
   };
*/


   //Init:
   uint8_t i;
   for(i=0; i<FB_WIDTH; i++)
   {
      uint8_t j=FB_WIDTH-1-i;
      setColor((i&0x03), (i&0x0C)>>2, (i&0x30)>>4, 0, i);
      setColor((i&0x30)>>4, (i&0x0C)>>2, (i&0x03), 1, i);
//      setBlue(frameBuffer[0][i], 0, i);
//      setBlue(frameBuffer[1][i], 1, i);
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
 * /Users/meh/_avrProjects/LCDdirectLVDS/68-backToLTN/_unusedIdeas/colorBarScroll.c
 *
 *    (Wow, that's a lot longer than I'd hoped).
 *
 *    Enjoy!
 */

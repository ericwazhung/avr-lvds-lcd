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

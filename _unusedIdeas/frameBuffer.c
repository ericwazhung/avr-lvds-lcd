

//AHHH it wasn't IMAGE_BUFFER that's entangled, but IMAGE_CHANGE
// which is even weirder.

//This was #if'd into existence with IMAGE_BUFFER, but since it's only been
// used with frameBuffer, and since IMAGE_BUFFER is somewhat entangled with
// newer stuff, I'm putting it here and NOT including this file, (yet)
#if(defined(IMAGE_BUFFER) && IMAGE_BUFFER)
void setColor(uint8_t red, uint8_t green, uint8_t blue, 
               uint8_t row, uint8_t col)
{
   red = (red > 3) ? 3 : red;
   green = (green > 3) ? 3 : green;
   blue = (blue > 3) ? 3 : blue;

   frameBuffer[row][col] = red | ((green)<<2) | ((blue)<<4);
}
#endif



//drawPix from program memory
//a/0 v60: Again, unused for its original purpose, but tightly intertwined
// in old code that's still being (mis)used...
 #define FB_WIDTH 16
 #define FB_HEIGHT 16

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


#if(defined(IMAGE_BUFFER) && IMAGE_BUFFER)
uint8_t frameBuffer[FB_HEIGHT][FB_WIDTH];
#endif


// This code was in main...
// This isn't generalized enough to justify this function-name
// just not ready to delete it completely
void frameBufferUpdate(void)
{
        #if (defined(IMAGE_BUFFER) && IMAGE_BUFFER)
         uint8_t *pimage;
        #endif

			//Here's where it alternated pimage to point to alternate images
			//This code worked alongside rowBuffer, via fb_to_rb, I guess
			// since tetUpdate, and hexColor, etc. were displayed alongside
			// at one point...
			// Far too convoluted to piece together in my mind anymore

        #if 0
         tetUpdate();

         if(imageNum == 0)
         {
            imageNum = 1;
            pimage = pgm_image1;
            tetColorScheme = 1;
         }
         else
         {
            hexColor++;
            hexColor&=0x3f;
            imageNum = 0;
            pimage = pgm_image2;
            tetColorScheme = 0;
         }
        #endif //0



        #if (defined(IMAGE_BUFFER) && IMAGE_BUFFER)
         for(row=0; row<FB_HEIGHT; row++)
         for(col=0; col<FB_WIDTH; col++)
         {
            uint8_t imagePixel=pgm_readImageByte(pimage, row, col);


            if(imagePixel == Tr)
               frameBuffer[row][col] = colorShift+row+col;
            else
               frameBuffer[row][col] = imagePixel;
         /*
            setColor(
               (getbit(RED_IMAGEVAL_BIT, imagePixel) ? 0x03 : 0),
               (getbit(GREEN_IMAGEVAL_BIT, imagePixel) ? 0x0C : 0),
               (getbit(BLUE_IMAGEVAL_BIT, imagePixel) ? 0x30 : 0),
               r, c);
         */
         }
         colorShift++;
        #endif
}







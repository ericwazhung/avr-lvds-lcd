uint8_t hexColor = 0;

static __inline__ \
void hexColor_drawRow(uint8_t rbRowNum, uint8_t rowBuffer[]) \
     __attribute__((__always_inline__));

//Takes a nibble and makes it a hex character...

#define HEXCOLOR_TOPROW 32
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
            rowBuffer[col+1] = fb_to_rb(hexColor);
         //else
         //   rowBuffer[col] = fb_to_rb(0x00);

         if(getbit(col, charRow2))
            rowBuffer[col+8+1] = fb_to_rb(hexColor);
      }
   }
}

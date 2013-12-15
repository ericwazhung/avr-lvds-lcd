//These are only used in drawPix...
//This probably has no purpose outside of COLOR_BAR_SCROLL below
// Actually, it's kinda funny watcching the top of the smiley's
// head scroll... like he's getting a brain transplant
//#define PIXEL_SCROLL TRUE




void pixelScroll_main(void)
{
   uint32_t counter=0;

   while(1)
   {
      if(counter==0)
      {
         uint8_t firstCol[2];
         firstCol[0] = frameBuffer[0][0];
         firstCol[1] = frameBuffer[1][0];

         uint8_t i;
         for(i=0; i<FB_WIDTH-1; i++)
         {
            frameBuffer[0][i] = frameBuffer[0][i+1];
            frameBuffer[1][i] = frameBuffer[1][i+1];
//            setBlue(frameBuffer[0][i], 0, i);
//            setBlue(frameBuffer[1][i], 1, i);
         }

         frameBuffer[0][i] = firstCol[0];
         frameBuffer[1][i] = firstCol[1];
//         setBlue(frameBuffer[0][i], 0, i);
//         setBlue(frameBuffer[1][i], 1, i);
      }
      counter+=16384;
      heartUpdate();
   }
}



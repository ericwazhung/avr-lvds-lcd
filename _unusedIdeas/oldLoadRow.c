//This was in loadRow()...

#if 0
   uint8_t i=0;

//   if((rowNum < 0) || (rowNum >= V_COUNT))
//      return;

   //THIS DOES NOT MAKE AN OUNCE OF SENSE TO ME:
   // why do I need *2 here?! Without it, the image is stretched and cut
   // in half vertically!
   // Apparently has to do with delayed Hsyncs due to calculations...?
   // LIFE with delay = 10 causes 2/3 frame at *2
   // The screen itself appears to be repeating rows...
#if(!defined(HORIZONTAL_COLOR_BARS) || !HORIZONTAL_COLOR_BARS)
 #if(LVDS_PRESCALER == 8)
  #define ROW_SCALE 1
 #else
  #define ROW_SCALE 2
 #endif
   uint8_t rbRowNum = ((uint32_t)rowNum)*RB_HEIGHT*ROW_SCALE/V_COUNT;

   //Old note pre tet_drawRow... (code deleted a/o v45)   
   //WITHOUT THIS: syncing seems OK
   // But this is necessary for the right-border...
   // Adding it causes syncing problems at _b and _c -> _W
   // TODO: Look into sync-problems above!!!
   //  (revisit 41-1)
   //i++;

   //Clear the rowBuffer, it will be reloaded, where appropriate, after
   // This isn't particularly speed-efficient, since most pixels will 
   // be written twice... but it should make things more plug-n-play...
   for(i=0; i<RB_WIDTH; i++)
      rowBuffer[i]=fb_to_rb(0);


   tet_drawRow(rbRowNum, rowBuffer);


   hexColor_drawRow(rbRowNum, rowBuffer);

   
   i=63-40;

   static uint16_t colorShift = 0;

   //!!! This *should* only increment once per frame (RIGHT?)
   // BUT, set colorShift>>5 to colorShift>>3 or even >>0
   // and it seems this is not true... WTF, again?
   if(rbRowNum == 0)
      colorShift++;

   //Just fill up the remaining pixels with color...
   // i-- to overwrite the blank column in the characters
   // Leaving one column of black to check into syncing problems
   // with LVDS_PRESCALER=8...
   // apparently, the garbage-lines are related to the last color drawn(?!)
   for( ;i<RB_WIDTH;i++)
   {
         uint8_t colorVal = (i + rbRowNum + (colorShift>>5)) & 0x3f;

         //We don't want Black, since it's used for LIFE...
         if(colorVal==0)
            colorVal = 0x15;

         //Only draw colors where there isn't already data
         // (e.g. LIFE or the SMILEY)
         // Actually, this can't work, since Black is a possibility...
         //if(!rowBuffer[i])
            rowBuffer[i] = fb_to_rb(colorVal);
   }

/* This is now handled in the rowSegBuffer loading...
   I guess that doesn't fix it for the old non-row-seg case...

   //Apparently *black* causes issues... as well as some dark colors
   // (certain shades of blue?)
   // White, as the last pixel, causes syncing to work fine, ish
   //And adding a black column doesn't do anything?!
   // either way, thsi should be redundant due to rowBuffer clearing
   // and RB_WIDTH-2 in the color-filling
   // WTF?
   //rowBuffer[RB_WIDTH-2] = fb_to_rb(_K);
   rowBuffer[RB_WIDTH-1] = fb_to_rb(_W);
*/

   //Draw the smiley on top of the color pixels
#define SMILE_TOP_ROW (20)
#define SMILE_HEIGHT FB_HEIGHT
#define SMILE_SHIFT   (24+(40-FB_WIDTH)/2)
   if((rbRowNum >= SMILE_TOP_ROW) &&
         (rbRowNum < (SMILE_HEIGHT+SMILE_TOP_ROW)))
   {
         for(i=0; i<FB_WIDTH; i++)
         {
            uint8_t temp;
         #if(defined(IMAGE_BUFFER) && IMAGE_BUFFER)
            temp=frameBuffer[rbRowNum-SMILE_TOP_ROW][i];
         #else
            temp=pgm_readImageByte(pimage, rbRowNum-SMILE_TOP_ROW, i);
         #endif

            if(!(temp & 0xC0))
               rowBuffer[i+SMILE_SHIFT] = fb_to_rb(temp);
         }
   }


   #if(defined(LIFE) && LIFE)
   //This has to be called *after* the colored background is drawn...
   // (that could be changed... with _Tr?)
   life_drawRow(rbRowNum, &(rowBuffer[RB_WIDTH-LIFE_WIDTH]));
   #endif //LIFE


#else //HORIZONTAL_COLOR_BARS
   for(i=0; i<RB_WIDTH; i++)
      rowBuffer[i] = fb_to_rb(rowNum);
#endif


#endif //0

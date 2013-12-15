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



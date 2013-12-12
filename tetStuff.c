#if (TRUE)

//in main...
uint8_t fb_to_rb(uint8_t fbColor);

// Tetris:
// 10 across
// 18 up
// 7 pieces (colors = 3 bits each)
// Using 4 bits, for easy-access: 90 bytes
// Using 3 bits: 67.5 bytes
// How about a happy-medium... use 32-bits for a row...
#define TET_HEIGHT	18
#define TET_WIDTH		10
#define TET_TOP_ROW  (0)

extern uint16_t pgm_tetPiece[7][4];
#define bin4x4(b15,b14,b13,b12,\
		         b11,b10, b9, b8,\
		          b7, b6, b5, b4,\
                b3, b2, b1, b0)\
         ((bin(b15,b14,b13,b12,b11,b10, b9, b8) << 8) | \
			          (bin( b7, b6, b5, b4, b3, b2, b1, b0)))

#define getTetPieceBrick(piece, orientation, row, col) \
	   getbit((row*4+col), pgm_getTetPiece(piece,orientation))

#define pgm_getTetPiece(piece,orientation) \
	   ((uint16_t)pgm_read_word(&(pgm_tetPiece[piece][orientation])))

uint8_t getTetBrick(uint8_t row, uint8_t col);
void setTetBrick(uint8_t row, uint8_t col, uint8_t val);
uint8_t tetBrick_to_fb(uint8_t tetBrick);
	
//For now, let's just test that it works...
// (This could be implemented with bitfields?)
uint32_t tetRow[TET_HEIGHT];

void tetInit(uint8_t filledRows)
{
	uint8_t row;

	//Clear the board...
	for(row=0; row<TET_HEIGHT; row++)
		tetRow[row]=0;
#if (TRUE) //Load random-ish "bricks" for testing...
	uint8_t col;
	//Now, leaving the first several rows blank for piece-testing...
	for(row=TET_HEIGHT-filledRows; row<TET_HEIGHT; row++)
		for(col=0; col<TET_WIDTH; col++)
			setTetBrick(row, col, (row+col)&0x07);

#endif
#if (FALSE) //Just throw some random pieces in for testing... Dun Woik
	row = 0;
	uint8_t col;
	uint8_t piece = 0;
	uint8_t pieceRow;

	for(col=0; col<TET_WIDTH; col++)
	{
		piece = col/4;

		for(pieceRow=0; pieceRow<4; pieceRow++)
			setTetBrick(pieceRow + row, col, (row+col) & 0x07);
				  	//(getTetPieceBrick(piece,0,pieceRow,col)) ? piece : 0);

		row+=4;

	}
#endif
}


//Also determines color (pieceNum+1)...
uint8_t pieceNum = 0;
uint8_t pieceOrientation = 0;
//Upper-Left corner
uint8_t pieceTopRow = 0;
uint8_t pieceLeftCol = 0;
#define PIECE_HEIGHT 4
#define PIECE_WIDTH	4
#define NUM_PIECES	7
#define PIECE_STARTCOL 3
#define PIECE_STARTROW 0


static __inline__ \
void tet_drawRow(uint8_t rowNum, uint8_t rowBuffer[]) \
	  __attribute__((__always_inline__));
//This assumes rowBuffer[] points to the first column we should draw
// it could be called as, e.g. tet_drawRow(rbRowNum, &RealRowBuffer[5])
// to shift it right five pixels...
// rowNum corresponds to the row in drawable pixels
// (though this may change later... for, e.g. high vertical resolution)
// (e.g. for brick separators...)
void tet_drawRow(uint8_t rowNum, uint8_t rowBuffer[])
{
	uint8_t i; 

	if( (rowNum >= TET_TOP_ROW) &&
		 (rowNum < TET_TOP_ROW+TET_HEIGHT) )
	{
		//left border:
		rowBuffer[0]=fb_to_rb(_W);

		//Draw active (and inactive) bricks
		for(i=1; i<=TET_WIDTH; i++)
		{
			rowBuffer[i]=
				fb_to_rb(tetBrick_to_fb(
							getTetBrick(rowNum-TET_TOP_ROW,i-1)));
		}

		//right border:
		rowBuffer[i]=fb_to_rb(_W);

		//SEE NOTE in main.c re: _b and _c -> _W syncing...
	}
	//bottom border:
	else if(rowNum == TET_TOP_ROW+TET_HEIGHT)
		for(i=0; i<=TET_WIDTH+1; i++)
			rowBuffer[i]=fb_to_rb(_W);
//	else
//		return;


	//Active (falling) piece:

	if((rowNum >= pieceTopRow) &&
		(rowNum < pieceTopRow+PIECE_HEIGHT) )
	{
		uint8_t pieceRowNum = rowNum - pieceTopRow;

		for(i=0; i<PIECE_WIDTH; i++)
		{
			uint8_t pieceBrick = 
					getTetPieceBrick(pieceNum, pieceOrientation, 
										  pieceRowNum, i);

			//Only draw piece pixels (not black/empty pixels)
			// (we don't want to overwrite nearby bricks)
			if(pieceBrick)
			//+1 for the left border...
				rowBuffer[i+1+pieceLeftCol] =
						fb_to_rb(tetBrick_to_fb(pieceNum+1));
		}
	}




	//Draw "TETRIS" vertically...
#define CHARACTER_START_COL   14 //FB_WIDTH
   //i=FB_WIDTH;
   i=CHARACTER_START_COL;

   uint8_t charRow;
   uint8_t color;
   char character;
   switch(rowNum/8)
   {
         case 0:
            //charRow = getCharRow('T', rowNum%8);
            character = 'T';
            color = _R;
            break;
         case 1:
            //charRow = getCharRow('E', rowNum%8);
            character = 'E';
            color = _O;
            break;
         case 2:
            //charRow = getCharRow('T', rowNum%8);
            character = 'T';
            color = _Y;
            break;
         case 3:
            { 
               uint8_t z, temp;
               
               //Reverse the "R" to make it look Russian
               // And shift it one left...
               temp = getCharRow('R', rowNum%8);
               
               for(z=0; z<8; z++)
               {
                  writebit(7-z, charRow, getbit(z,temp));
               }  
               //Characters are reversed... bit 0 is far-left
               //So to shift a character left, I must shift its bits right
               // (right?)
               charRow >>= 1;
               color = _G; 
            }  
            break;
         case 4:
            //charRow = getCharRow('I', rowNum%8);
            character = 'I';
            color = _C; 
            break;
         case 5:
            //charRow = getCharRow('S', rowNum%8);
            character = 'S';
            color = _B; 
            break;
         default:
            character = ' ';
            //charRow = 0;
            break;
   }     

	//Load charRow for all cases except the backwards "R"
	// which is handled above...      
   if(rowNum/8 != 3)
         charRow = getCharRow(character, rowNum%8);

   i++;

   uint8_t j;
   for(j=0; j<8; j++)
         if(getbit(j, charRow))
            //frameBuffer[rowNum][j+i+1] = 0x25;
            rowBuffer[j+i] = fb_to_rb(color); //0x25);
         else
            //frameBuffer[rowNum][j+i+1] = 0x01;
            rowBuffer[j+i] = fb_to_rb(0x00);


}

void tetUpdate(void)
{
	//For now, we're just experimenting with the different pieces/
	// orientations/positions...	
	static uint8_t callCount = 0;
	static uint8_t newPieceHitCount=0;

	uint8_t new_pieceOrientation;
	uint8_t new_pieceTopRow;
	uint8_t new_pieceLeftCol;
//	uint8_t new_pieceNum;

	uint8_t pieceRow;
	uint8_t pieceCol;

	uint8_t noChange = FALSE;
	uint8_t settle = FALSE;

	if(newPieceHitCount == 5)
	{
		newPieceHitCount = 0;
		tetInit(0);
	}

	new_pieceOrientation = pieceOrientation;
	new_pieceLeftCol = pieceLeftCol;
	new_pieceTopRow = pieceTopRow;

	if(callCount%4 == 2)
   {
		new_pieceOrientation=pieceOrientation+1;
      if (new_pieceOrientation==4)
			new_pieceOrientation=0;
	}

      if(callCount%4==0)
      {  
         new_pieceLeftCol=pieceLeftCol+1;
		}

		if(callCount%4==3)
		{
			new_pieceTopRow=pieceTopRow+1;
      }

	callCount++;





	//Check for collision in the new piece position/orientation...
	//Starting from the bottom... (intentional wraparound)
	for(pieceRow = 3; (pieceRow < 128) && !settle && !noChange; pieceRow--)
		for(pieceCol = 0; pieceCol < 4; pieceCol++)
		{
			//This isn't particularly efficient...
			// rereading each time...
			uint8_t pieceBrick =
						getTetPieceBrick(pieceNum, new_pieceOrientation,
											  pieceRow, pieceCol);

			uint8_t boardRow = new_pieceTopRow + pieceRow;

			uint8_t boardCol = new_pieceLeftCol + pieceCol;

			uint8_t boardBrick = getTetBrick(boardRow, boardCol);


			if(pieceBrick)
			{
				if(boardRow >= TET_HEIGHT)
				{
					noChange = TRUE;
					settle = TRUE;
					break;
				}

				//There's some plausibility for some oddities here...
				// revisit 41-15.zip
				if(boardCol >= TET_WIDTH)
				{
				   noChange = TRUE;
				   break;
				}


				if(boardBrick)
				{
					//Must be a settle...
					if((new_pieceOrientation == pieceOrientation) &&
						(new_pieceLeftCol == pieceLeftCol))
						settle = TRUE;

					noChange = TRUE;
					break;
				}
			}
		}





	if(!noChange)
	{
		pieceOrientation = new_pieceOrientation;
		pieceLeftCol = new_pieceLeftCol;
		pieceTopRow = new_pieceTopRow;
	}

	if(settle)
	{
		// Check if we've hit on a new piece...
		if((pieceTopRow == PIECE_STARTROW) &&
		   (pieceLeftCol == PIECE_STARTCOL) &&
		   (pieceOrientation == 0))
		   newPieceHitCount++;


		//Load the piece into the board...
		for(pieceRow = 0; pieceRow < 4; pieceRow++)
			for(pieceCol = 0; pieceCol < 4; pieceCol++)
			{
				if(getTetPieceBrick(pieceNum, pieceOrientation,
										  pieceRow, pieceCol))
				setTetBrick(pieceTopRow+pieceRow,
								pieceLeftCol+pieceCol,
								pieceNum+1);
			}


		//Load a new piece...
		pieceNum++;
		if(pieceNum==NUM_PIECES)
			pieceNum = 0;
		pieceLeftCol = PIECE_STARTCOL;
		pieceTopRow = PIECE_STARTROW;
		pieceOrientation = 0;
	}

}

//returns a 3-bit value
// 0 corresponds to no brick
// 1-7 corresponds to the type of piece the brick came from...
uint8_t getTetBrick(uint8_t row, uint8_t col)
{
	//uint8_t tetCol = col*3;
//	uint8_t tetMask = '\007';
	
	return ((uint8_t)(tetRow[row] >> (3*col))) & '\007';

}


//val should only be 0-7
// 0 corresponds to no brick
// 1-7 corresponds to the type of piece the brick came from...
void setTetBrick(uint8_t row, uint8_t col, uint8_t val)
{
	//Clear the old value...
	tetRow[row] &= ~(((uint32_t)'\007')<<(3*col));
	//Write the new value...
	tetRow[row] |= (((uint32_t)val)<<(3*col));
}

uint8_t tetColorScheme = 0;

uint8_t tetBrick_to_fb(uint8_t tetBrick)
{
	if(!tetBrick)
		return 0;

	switch(tetColorScheme)
	{
		case 0:
		case 1:
			switch(tetBrick)
			{
				case 1:
					return _R;
				case 2:
					return _O;
				case 3:
					return _Y;
				case 4:
					return _G;
				case 5:
					return _C;
				case 6:
					return _B;
				case 7:
					return _M;
				default:	//WTF???
					return _W;
			}
			break;
		default: //Shouldn't happen...
			// was case1 but these colors are ugly
			// and cause syncing problems...
			switch(tetBrick)
			{
				case 1:
					return _r;
				case 2:
					return _g;
				case 3:
					return _b;
				case 4:
					return _c;
				case 5:
					return _m;
				case 6:
					return _y;
				case 7:
					return _k;
				default:	//WTF
					return _W;
			}
//		default:
//			return 0;
	}

}

//Pieces are four bricks apiece:
// (funny I never made that connection before)
//
//  ####  ##    ##    #    #       #   ##
//         ##  ##    ###   ###   ###   ##
//
//  (2)  (2)   (2)   (4)   (4)   (4)   (1) ...(orientations)
//
// I don't know how they're supposed to rotate...
//  it seems some rotate around a specific brick,
//     some around a vertex (e.g. the 2x2 block)
//     and some just alternate between two orientations (-_)
// worst-case scenario, each piece could be described by a 4x4 grid
// (probably easiest, actually)... two bytes per orientation

uint16_t pgm_tetPiece[NUM_PIECES][4] PROGMEM =
{
	{
		bin4x4(0,0,0,0,
				 0,0,0,0,
				 1,1,1,1,
				 0,0,0,0),

		bin4x4(0,0,1,0,
				 0,0,1,0,
				 0,0,1,0,
				 0,0,1,0),

		bin4x4(0,0,0,0,
      		 0,0,0,0,
      		 1,1,1,1,
      		 0,0,0,0),

		bin4x4(0,0,1,0,
		       0,0,1,0,
		       0,0,1,0,
		       0,0,1,0)
	},
	{
		bin4x4(0,0,0,0,
				 1,1,0,0,
				 0,1,1,0,
				 0,0,0,0),

		bin4x4(0,0,0,0,
				 0,0,1,0,
				 0,1,1,0,
				 0,1,0,0),

		bin4x4(0,0,0,0,
      		 1,1,0,0,
     		    0,1,1,0,
      		 0,0,0,0),

		bin4x4(0,0,0,0,
      		 0,0,1,0,
     		    0,1,1,0,
      		 0,1,0,0)
	},
	{
		bin4x4(0,0,0,0,
				 0,1,1,0,
				 1,1,0,0,
				 0,0,0,0),

		bin4x4(1,0,0,0,
				 1,1,0,0,
				 0,1,0,0,
				 0,0,0,0),

      bin4x4(0,0,0,0,
             0,1,1,0,
             1,1,0,0,
             0,0,0,0),

      bin4x4(1,0,0,0,
             1,1,0,0,
             0,1,0,0,
             0,0,0,0)
	},
	{
		bin4x4(0,0,0,0,
				 0,1,0,0,
				 1,1,1,0,
				 0,0,0,0),

		bin4x4(0,0,0,0,
				 0,1,0,0,
				 1,1,0,0,
				 0,1,0,0),

		bin4x4(0,0,0,0,
				 0,0,0,0,
				 1,1,1,0,
				 0,1,0,0),

		bin4x4(0,0,0,0,
				 0,1,0,0,
				 0,1,1,0,
				 0,1,0,0)
	},
	{
		bin4x4(0,0,0,0,
				 1,0,0,0,
				 1,1,1,0,
				 0,0,0,0),

		bin4x4(0,0,0,0,
				 0,1,0,0,
				 0,1,0,0,
				 1,1,0,0),

		bin4x4(0,0,0,0,
				 0,0,0,0,
				 1,1,1,0,
				 0,0,1,0),

		bin4x4(0,0,0,0,
				 0,1,1,0,
				 0,1,0,0,
				 0,1,0,0)
	},
	{
		bin4x4(0,0,0,0,
				 0,0,1,0,
				 1,1,1,0,
				 0,0,0,0),

		bin4x4(0,0,0,0,
				 1,1,0,0,
				 0,1,0,0,
				 0,1,0,0),

		bin4x4(0,0,0,0,
				 0,0,0,0,
				 1,1,1,0,
				 1,0,0,0),

		bin4x4(0,0,0,0,
				 0,1,0,0,
				 0,1,0,0,
				 0,1,1,0)
	},
	{
		bin4x4(0,0,0,0,
				 0,1,1,0,
				 0,1,1,0,
				 0,0,0,0),

      bin4x4(0,0,0,0,
             0,1,1,0,
             0,1,1,0,
             0,0,0,0),

      bin4x4(0,0,0,0,
             0,1,1,0,
             0,1,1,0,
             0,0,0,0),

      bin4x4(0,0,0,0,
             0,1,1,0,
             0,1,1,0,
             0,0,0,0)
	}				 
};

#endif //FALSE

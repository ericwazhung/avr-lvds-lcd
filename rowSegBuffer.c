#include "rowSegBuffer.h"

// Dunno why this was originally in main() upon init...
// mighta just been a quick/early test:
// But it might be a good starting-point to see how to use these functions
// (for now)
/*
   segPosition=0;
   newSeg(32, 0x06, (6<<4) | 3);
   newSeg(16, 0x00, (4<<4) | 0);
   newSeg(8, 0x06, (4<<4) | 0);
   newSeg(32, 0x00, (4<<4) | 3);
   newSeg(32, 0x06, (6<<4) | 3);
   segTerminate();
*/



// Was thinking about loading the last segment as black, but that doesn't
// work if the last segment isn't read, due to length=0...
seg_t rowSegBuffer[NUM_SEGMENTS+1];/*=
{
	{(4<<SEG_LENGTH_SHIFT) | 0x06, (6<<4) | 3},  //White
   {(8<<SEG_LENGTH_SHIFT) | 0x00, (4<<4) | 0},  //Black
	{(16<<SEG_LENGTH_SHIFT)| 0x06, (4<<4) | 0},  //Red
	{(32<<SEG_LENGTH_SHIFT) | 0x06, (6<<4) | 3}, //White
	{0,(4<<4)}                                   //Terminate
};
*/
/*=
{
	{0x10,4,6},
	{0x32,4,8},
	{0x03,5,2},
	{0x00,0,0}
};
*/


//Outside of rbpix_to_seg, this corresponds with the last-written segment
uint8_t segPosition = 0;


uint8_t newSeg(uint16_t length, uint8_t red, uint8_t green_blue);
uint8_t stretchSeg(uint16_t additionalLength);

void segClear(void)
{
	segPosition = 0;
	//The important part is the null-length
	// but let's load white just in case
	rowSegBuffer[segPosition].red_length = 6;
	rowSegBuffer[segPosition].green_blue = (6<<4) | 3;
}


void segTerminate(void)
{

	//Add a new NULL-SEGMENT
	//This might appear as a stretched last pixel/segment...
	if((segPosition < NUM_SEGMENTS-1) 
		&& (rowSegBuffer[segPosition].red_length & SEG_LENGTH_MASK))
	{
		segPosition++;

		rowSegBuffer[segPosition].red_length 
			= rowSegBuffer[segPosition-1].red_length & RED_MASK;
		rowSegBuffer[segPosition].green_blue
			= rowSegBuffer[segPosition-1].green_blue;
	}
	//We're already at the last segment, so we have to nullify it
	//This might terminate the last pixel early, but is necessary.
	else if(segPosition == NUM_SEGMENTS-1)
	{
		//Apparently white doesn't cause sync problems... so load it white
		rowSegBuffer[segPosition].red_length = fbRed_to_seg(_W);
		rowSegBuffer[segPosition].green_blue = 
										fbBlue_to_seg(_W) | fbGreen_to_seg(_W);
	}

	//If we got here, it's already been terminated...
	//else...
}


//This is just an intermediate test, for now...
// for reloading the rowbuffer to a seg-buffer.
//This could easily be revised (and generalized) to be...
// addSegment or addPixel
void rbpix_to_seg(uint8_t rbValue, uint8_t col, uint8_t length)
{
	static uint8_t last_rbValue = 0;
	//uint8_t length = 1;

	if(col == 0)
	{
		last_rbValue = 0;
		segClear();
	}
	//Same color as last pixel...
	else if(segPosition && (rbValue == last_rbValue))
	{
		stretchSeg(length);
		return;
	}

	uint8_t green = rbValue & 0x03;
	uint8_t red = (rbValue & 0x1C) >> 2;
	uint8_t blue = rbValue >> 5;

	newSeg(length, red, (blue<<4) | green);

	last_rbValue = rbValue;
}

//Call this when we already know the color in the last segment is the same
// so we wish to stretch the segment by additionalLength
// these values are real-lengths, not as shifted for storage...
// Return TRUE if the stretch was truncated...
// THIS DOES NOT TEST DISPLAY_WIDTH!
// DON'T FORGET to make additionalLength square before the call
// if necesary....
uint8_t stretchSeg(uint16_t additionalLength)
{
		uint8_t oldLength=
			(rowSegBuffer[segPosition].red_length >> SEG_LENGTH_SHIFT);

		if(!additionalLength)
			return FALSE;

		//We can stretch the current segment
		if(oldLength < SEG_MAXLENGTH)
		{
			uint8_t maxStretch = SEG_MAXLENGTH - oldLength;

			// Will the entire addition fit in the previous segment?
			if(additionalLength <= maxStretch)
			{
				rowSegBuffer[segPosition].red_length += 
					(additionalLength << SEG_LENGTH_SHIFT);

				//Then we're done.
			//	additionalLength = 0;
			//	break;
				return FALSE;
			}
			// Nope... stretch it all the way, then later we'll make another
			else
			{
				uint8_t red = 
					rowSegBuffer[segPosition].red_length & RED_MASK;

				rowSegBuffer[segPosition].red_length =
					red | (SEG_MAXLENGTH << SEG_LENGTH_SHIFT);

				additionalLength -= maxStretch;
			}
		}

		//The current segment has been stretched and there's more remaining
		// or it was already maxed-out...

		// We shouldn't ever get here if additionalLength = 0...

		//But we should test to make sure there's still lenth after
		// the stretch...
		if(additionalLength)
		{
			uint8_t red =
				rowSegBuffer[segPosition].red_length & RED_MASK;

			uint8_t green_blue =
				rowSegBuffer[segPosition].green_blue;

			uint8_t retVal;

			//Create new segment(s) with the remaining length
			retVal = newSeg(additionalLength, red, green_blue);

			//Then we're done...
			//additionalLength = 0;
			//break;

			return retVal;
		}	

		//We shouldn't ever get here...
		// Well, maybe... if additionalLength == maxStretch...?
		return FALSE;
}

//This should be like newSeg, but also checks whether it can stretch
// the last segment...
uint8_t addSeg(uint16_t length, uint8_t red, uint8_t green_blue)
{

	//Currently not smart enough to stretch the first segment...
	if(segPosition == 0)
		return newSeg(length, red, green_blue);

	//Can we stretch?
	if(((rowSegBuffer[segPosition].red_length & RED_MASK) == red)
		&& (rowSegBuffer[segPosition].green_blue == green_blue))
			return stretchSeg(length);
	else
		return newSeg(length, red, green_blue);

}

//This assumes you intend to create a new segment
// it does *not* check whether the previous segment could be stretched
// (it in fact, is called by stretchSeg as necessary)
// Returns TRUE if it had to be truncated
//  (out of segment memory, past the display edge...)
// length is the real length, not shifted for storage...
// THIS DOES NOT squarify, so adjust length as necessary before calling
uint8_t newSeg(uint16_t length, uint8_t red, uint8_t green_blue)
{

	//Recursion would be simpler, but we've got a limited stack.
	while(length >= SEG_MAXLENGTH)
	{
		// No more segments available...	
		// The last segment must have a length of 0
		if(segPosition == NUM_SEGMENTS-1)
		{
			segTerminate();
			return TRUE;
		}

		// Not yet sure how to handle display-width...
		//if(segTotal > DISPLAY_WIDTH)
		//  at least draw as much of the segment as possible...
		//  then segTerminate();
		//  and return TRUE

		// If we've gotten this far, then we can add the segment
	
		//Check if the segment buffer is cleared...
		// then DON'T increment segPosition...
		//if(!((segPosition == 0) && 
		//  !(rowSegBuffer[segPosition].red_length & SEG_LENGTH_MASK)))
		if(segPosition || 
			((rowSegBuffer[segPosition].red_length & SEG_LENGTH_MASK) != 0))
				segPosition++;


		//Create a maxlength segment
		rowSegBuffer[segPosition].red_length =
			red | (SEG_MAXLENGTH<<SEG_LENGTH_SHIFT);

		rowSegBuffer[segPosition].green_blue = green_blue;

		length -= SEG_MAXLENGTH;

	}

	//We can fit whatever's left (or the whole thing) into a single segment
	// Unless, of course, there's nothing left...
	if(length > 0) //< SEG_MAXLENGTH)//Guaranteed now...
	{
		//Make sure there's another segment to write to!
		if(segPosition == NUM_SEGMENTS-1)
		{
			segTerminate();
			return TRUE;
		}

		//if(!((segPosition == 0) &&
		//  !(rowSegBuffer[segPosition].red_length & SEG_LENGTH_MASK)))
		if(segPosition || 
	     ((rowSegBuffer[segPosition].red_length & SEG_LENGTH_MASK) != 0))
  				segPosition++;


		rowSegBuffer[segPosition].red_length = 
			red | (length<<SEG_LENGTH_SHIFT);

		rowSegBuffer[segPosition].green_blue = green_blue;
	}

	return FALSE;

}


static __inline__ \
void drawSegs(void) \
	__attribute__((__always_inline__));


void drawSegs(void)
{
#warning "This might need to be modified such that DE is active *with* the first pixel... and complementary outputs on the clock pin, as well"
	//Declared volatile *just in case* I decide to use it after the asm...
	// (as modified by the asm)
	volatile uint8_t *byteptr = &(rowSegBuffer[0].red_length);

	//These should only be used in the asm...
	// declaring them here, instead of using specific registers in asm
	// should help assure that those registers aren't clobbered
	// if used elsewhere
	//volatile uint8_t r=0, rl=0, g=0, b=0;

	//uint8_t r, rl, g, b;

	//Basic idea:

	//do{
	//  red_length = rowSegBuffer[n].red_length;
	//  green      = rowSegBuffer[n].green;
	//  blue       = rowSegBuffer[n].blue;
	//  
	//  OCR1D = red_length&0x0f;
	//  DT1   = green;
	//  OCR1A = blue;
	//
	//  red_length &= 0xf0;
	//  if(!red_length)
	//     break;
	//
	//  while(red_length)
	//		red_length -= 0x10;
	// } while(1);

	//This looks like it should work...
	// but there are some oddities
	// in this case, registers are loaded such that andi can't be used
	// but and can... thus it's a few instructions larger...
	/*
	do
	{
		rl = *(byteptr++);
		r = rl;
		r &= 0x0f;
		g = *(byteptr++);
		b = *(byteptr++);

		//just testing pushes and pops...
		// apparently the optimizer realizes which registers are in use
		// in the (only) call to drawSegs() and doesn't bother to push the
		// the rest...
//		drawSegs();
		OCR1D = r;
		DT1 = g;
		OCR1A = b;

		rl &= 0xf0;

		if(!rl)
			break;

		while(rl)
			rl -= 0x10;
	} while(1);
*/

	//LoadNext:
	//  ld   Rredlength, Z+	(Red|length)								2
	//  ld   Rgreen, Z+	(Green)											2
	//  ld   Rblue, Z+	(Blue)											2
	//	 mov  Rred, Rredlength												1
	//  andi Rred, 0x0f				//Remove length from Rred		1
	//  out  OCR1D, Rred														1
	//  out  DT1, Rgreen														1
	//  out  OCR1A, Rblue													1

	//  andi Rredlength, 0xf0		//Remove red from length		1
	//  breq Done						// if length==0, we're done	1 2

	//Counter:
	//  subi Rlength, 0x10			//Subtract 0x10 each time...	1
	//  breq LoadNext					// if length==0, next segment	1 2
	//  rjmp Counter					// else, next counter loop    2

	//Done:

	// Thus, minimum inter-pixel time is 16 clock cycles
	// (14 for the last one)
	// This cannot be reuced by using a separate byte for red and length
	// because the mov and andi in LoadNext would be replaced by a ld

	// To make the counter take longer between each count,
	// decrement it by a smaller number...

//	drawSegs();
/*
__asm__ __volatile__
	(
	"LoadNext_%=: \n\t"
	 	"ld %1, %a0+ ; \n\t"		//%a1 refers to XYZ whichever is selected
		"mov %2, %1 ; \n\t"		// Load R (+length)
		"andi %2, 0x0f ; \n\t"	//Remove length from Rred
		"ld %3, %a0+ ; \n\t"		//Load G	
		"ld %4, %a0+ ; \n\t"		//Load B
		"out %5, %2 ; \n\t"	// Write OCR1D (Red)
		"out %6, %3 ; \n\t"		// Write DT1 (Green)
		"out %7, %4 ; \n\t"	// Write OCR1A (Blue)

		"andi %1, 0xf0 ; \n\t"	// Remove red from length
		"breq Done_%= ; \n\t"		// if length==0 from the start, we're done

	"Counter_%=: \n\t"
		"subi %1, 0x10 ; \n\t"	//Decrement the counter by 0x10
		"breq LoadNext_%= ; \n\t"	// if we've decremented to 0, next segment

	"Done_%=: \n\t"
		:								//No output register...
		: "e" (byteptr),			//byteptr: %0, r/w and assigned to X,Y,orZ
		  "d"  (rl),				//rl: %1, andi and subi will be used
		  "d"  (r),					//r: %2, andi will be used
		  "r"  (g),					//g: %3
		  "r"  (b),					//b: %4
		  "I"  (_SFR_IO_ADDR(OCR1D)), //OCR1D: %5
		  "I"  (_SFR_IO_ADDR(DT1)),	//DT1: %6
		  "I"  (_SFR_IO_ADDR(OCR1A))	//OCR1A %7
	);
*/

__asm__ __volatile__
   (
   "LoadNext_%=: \n\t"
      "ld r20, %a0+ ; \n\t"    //%a0 refers to XYZ whichever is selected
      "mov r21, r20 ; \n\t"     // Load R (+length)
//      "andi r21, 0x0f ; \n\t"  //Remove length from Rred
		"andi r21, %5 ; \n\t"
      "ld r22, %a0+ ; \n\t"    //Load G 
#if (defined(GB_COMBINED) && GB_COMBINED)
		"mov r23, r22 ; \n\t"	 //copy G(+B) to B
		"andi r22, 0x0f ; \n\t"	 // strip B from G
		"swap r23 ; \n\t"			 // swap B and G nibbles in B
		"andi r23, 0x0f \n\t"	 // strip G from B
#else
		"ld r23, %a0+ ; \n\t"    //Load B
#endif
		"out %1, r21 ; \n\t"  // Write OCR1D (Red)
      "out %2, r22 ; \n\t"     // Write DT1 (Green)
      "out %3, r23 ; \n\t"  // Write OCR1A (Blue)

//      "andi r20, 0xf0 ; \n\t"  // Remove red from length
      "andi r20, %4 ; \n\t"
		"breq Done_%= ; \n\t"      // if length==0 from the start, we're done
#if (defined(GB_COMBINED) && GB_COMBINED)
#warning "This probably isn't necessary anymore..."
		// What was it for? Maybe because I was thinking about
		// using a smaller count-increment (for non-square pixels?)
		// than the minimum count step (given shifting)?
		// e.g. if the count is stored in the upper 4 bits, then the
		//  smallest count step would be 0x10 (1 count)
		//  I think the divisible-by-four aspect, below, was due to my
		//  possibly wanting to set the counter to count by 0x04 instead of 
		//  0x10... then each count-step would result in 4 counts
		//  this could avoid all those nops below... (but now it makes less
		//  sense for non-square pixels)
		// I don't remember if the above is why I added these two nops...

		//Bump it to a round-number for counting...
		// 18 cyc -> 20 cyc... 20 is divisible by 4
		"nop ; \n\t"
		"nop ; \n\t"
#endif

   "Counter_%=: \n\t"
      "subi r20, %6 ; \n\t"  //Decrement the counter by 0x10...
      "breq LoadNext_%= ; \n\t"  // if we've decremented to 0, next segment
#if (defined(SQUARE_SEGMENTS) && SQUARE_SEGMENTS)
		//Match a single-pixel-width...
		//(18cyc w/o GB_COMBINED)
 #if (defined(GB_COMBINED) && GB_COMBINED)
		"nop ; \n\t"
		"nop ; \n\t"
 #endif
		"nop ; \n\t"		// if length = 0x10 this will be avoided
		"nop ; \n\t"		// for each step of 0x10 > 0x10
		"nop ; \n\t"		//   this will match a pixel-width
		"nop ; \n\t"
		"nop ; \n\t"
		"nop ; \n\t"
		"nop ; \n\t"
		"nop ; \n\t"
		"nop ; \n\t"
		"nop ; \n\t"
		"nop ; \n\t"
		"nop ; \n\t"
		"nop ; \n\t"
		"nop ; \n\t"
#endif
		"rjmp Counter_%= ; \n\t"

   "Done_%=: \n\t"
      :                       //No output register...
		// Input values and registers...
		// Their values are set-up by C and the preprocessor 
		// before entering the asm function
		// Only byteptr is actually in a register, and its value is changed
		//  in this asm function (incremented with each use)
		//  I'm still not wrapping my head around the whole read/write vs.
		//  input/output options here... clearly the asm is writing to
		//  byteptr, so maybe it should be in the clobber list?
		//  or maybe by putting it here (only), it would be automatically
		//  assumed to be clobbered, if it were used after the asm function?
		//  It shouldn't matter, here anyways, since it's not used after.
		//  Though, if it were to be, we'd probably need to specify somehow
		//  whether we want to reuse the variable name "byteptr" after this
		//  to refer to the value set up before this, or to the value this
		//  will write to it... It's all very confusing.
      : "e" (byteptr),        //byteptr: %0, r/w and assigned to X,Y,orZ
        "I"  (_SFR_IO_ADDR(OCR1D)), //OCR1D: %1
        "I"  (_SFR_IO_ADDR(DT1)),   //DT1: %2
        "I"  (_SFR_IO_ADDR(OCR1A)), //OCR1A %3
		  "M"  (SEG_LENGTH_MASK),		//%4
		  "M"  (RED_MASK),				//%5 RED_MASK....
		  "M"  (SEG_COUNT_1)				//%6
		//Indicate (to C) which registers are going to be clobbered
		// these registers are only used internal to the asm function
		// their values are unused on entry and exit.
		// Having them listed here should tell C to either not use them
		// outside of here, or to push and pop them around this... right?
	  : "r20", //rl
		  "r21",	//r
		  "r22",	//g
        "r23"	//b
   );


//	rl++;

}


//Note that rowNum is not used, AND it's a uint8_t! There's some note about
// this in lcdStuff.c, now. Not particularly helpful, but gives enough
// history I might be able to piece this stuff together and clean it up.
void rsb_drawPix(uint8_t rowNum)
{
   //a/o v59-12ish: WTF, no comment about this?!
   // I believe this is to enable Green's output
   // which was disabled prior because...?
   TCCR1A = ( (0<<COM1A1) | (1<<COM1A0)
            | (0<<COM1B1) | (1<<COM1B0)
            | (1<<PWM1A) | (1<<PWM1B) );

      drawSegs();

#define COLORS_WRITTEN   64


//#error "should add SEG_STRETCH here..."
#if (ROW_COMPLETION_DELAY > 0)
//      delay_cyc(DOTS_TO_CYC(DE_ACTIVE_DOTS) -60 // - 68)// - 60
//            - WRITE_COLOR_CYCS*COLORS_WRITTEN);
      delay_cyc(ROW_COMPLETION_DELAY);
//      asm("nop;");
//      asm("nop;");
//      asm("nop;");
//      asm("nop;");
//      asm("nop;");
//      asm("nop;");
//      asm("nop;");
//      asm("nop;");
      
#else
#warning "ROW_COMPLETION_DELAY <= 0"
#endif

      //Just for testing...
      // Actually, it's quite handy, because it shows where drawSegs has
      // completed... (I thought it stretched to the end of DE, but nope)
      // The "bug" with PLL_SYSCLK's white bars now appears to be quite
      // apparently due to carry-over from a previous line
      // rather than an Hsync problem, as now it appears cyan.
      OCR1D = 0;

      //DE->Nada transition expects fullBlue...
      //Also helps to show the edge of the DE timing...

      //!!! Not sure what the state is at this point...
      // could be any DE+Blue level, or could be NADA...
      // Nada: DT1=3, still leaves one bit for clocking, might be OK
         
      //Among the things that don't make sense...
      // This appears to go into affect BEFORE delay_cyc (?)
      // as, without a pull-up resistor on the /OC1B output, 
      // green seems to be floating between the last pixel and the
      // delay_cyc (!)
      //Disable complementary-output for Green 
      //  (on /OC1B, where CLK is OC1B)
      // Since Nada, V, and H DT's might be bad for clocking.
      TCCR1A = ( (0<<COM1A1) | (1<<COM1A0)
         | (1<<COM1B1) | (0<<COM1B0)
         | (1<<PWM1A) | (1<<PWM1B) );

      fullBlue();
      Nada_fromDEonly();
}





#include "delayCyc.h"

//This'll optimze-out in some cases.
// The timings and instructions used are probably specific to the
// instruction-set in the MCU, the GCC version, optimization-level, etc.
// a/o LCDdirectLVDS11 HLow_delay() this is how it's compiling...

// The actual number of cycs will probably be a few higher...
// (or who knows, if things optimze-out, e.g. small ~0-8 numCycs)
// Due to init, and division...
// Using rounding-up as well...

// the argument is int32_t to allow for negative value testing
// BUT: numCyc is only tested against a uint16_t...
//  the greatest value (?) is (UINT16_MAX - 7)
// (maybe it'd make more sense to do the math outside the delay_cyc call
// and allow the preprocessor to change it to 0, instead of doing it here.

//This should probably be reimplemented using _delay_loop_1/2() from 
// util/delay_basic.h
//  1 uses three cycles per count, counts from 1-256, 
//      256 counts: _delay_loop_1(0)
//  2 uses four, counts from 1-65536, 65536 is passed as 0

//#define DELAY_CYC_DELAY_LOOP TRUE//FALSE

#if(defined(DELAY_CYC_DELAY_LOOP) && DELAY_CYC_DELAY_LOOP)
#warning "This has only been tested with v54+... rowSegBuffer, etc."
void delay_cyc(int32_t numCyc)
{
   if(numCyc <= 0)
      return;

   uint16_t numLoops = (numCyc+3)/4;

   _delay_loop_2(numLoops);

/*   uint32_t delayLoops = (numCyc+3)/4;

   do
   {
      delayLoops--;
//      _delay_loop_2(delayLoops&0xffff);

//      delayLoops -= (delayLoops&0xffff);
   } while(delayLoops);
*/
/*
   //Since this is inline, AND it's only called with values computed
   // at compile time, only one of these should be compiled in...
   // as necessary...
   // If called without precomputed value (i.e. a variable)????
   // Maybe I should only use delay_loop_2...
   if(numCyc < 256*3)
      _delay_loop_1(numCyc/&0xff);

   else
*/
}

#else
void delay_cyc(int32_t numCyc)
{

   //This shouldn't happen often, but some delay_cyc() calls use math
   // to calculate the number of cycles, and it could be negative
   if(numCyc < 0)
      return;
                                 //#Clocks
   //Two instructions:
   // ldi r24, 0                  //1          i=0
   // ldi r25, 0                  //1
   uint16_t i;


   //Loop instructions:
   //nop                           //1
   //adiw  r24, 0x01   ; 1         //2          i++
   //cpi   r24, 0x77   ; 119      //1          i<numCyc (119 in this case)
   //cpc   r25, r1               //1          Apparently it's also testing
                                 //           the high byte is 0
   //brne  .-10        ;         //"1/2"      return to nop
                                 //            if I understand, this is two
                                 //            cyc when branching
                                 //            or one if not (when complete)

   //READ THIS:
   // Current Compilation Settings: A/O v18:
   //    THIS LOOP WILL BE UNROLLED if numLoops <= 5!!!
   //    Which then turns into numLoops*2 cycles (instead of numLoops*8)
   // 
   // Here's an attempted hack...
   //  it *should* optimize the test out in either case, so it's like a 
   //  preprocessing directive...
	uint8_t loopRemainder = numCyc & 0x07;
	uint16_t numLoops = (((uint16_t)(numCyc)+7)>>3);


   // HACK ATTEMPT 2: THATS A LOT OF CODE.
      switch(numLoops)
      {
         case 5:
            asm("nop");
            asm("nop");
            asm("nop");
            asm("nop");
            asm("nop");
            asm("nop");
            asm("nop");
            asm("nop");
         case 4:
            asm("nop");
            asm("nop");
            asm("nop");
            asm("nop");
            asm("nop");
            asm("nop");
            asm("nop");
            asm("nop");
         case 3:
            asm("nop");
            asm("nop");
            asm("nop");
            asm("nop");
            asm("nop");
            asm("nop");
            asm("nop");
            asm("nop");
         case 2:
            asm("nop");
            asm("nop");
            asm("nop");
            asm("nop");
            asm("nop");
            asm("nop");
            asm("nop");
            asm("nop");
         case 1:
            asm("nop");
            asm("nop");
            asm("nop");
            asm("nop");
            asm("nop");
            asm("nop");
            asm("nop");
            asm("nop");
            return;
            break;
         default:
            break;
      }
   // THUS:
   // Each loop is 7 cycles, make it 8 by adding an extra nop and we can
   // use >> instead of / for calculations...
   // +7 assures rounding-up...
#warning "This loop seems to be optimizing out!"
   //for(i=0; i<((numCyc+7)>>3); i++)
   for(i=0; i<numLoops; i++)
   {
      //THIS IS A HACK DUE TO OPTIMIZATION, see above
      // It will NOT likely be happy with different versions of gcc...
   /*   // NOGO: Apparently it won't expand the loop if this is part of it
      // so then we have 5 loops AND 8 instructions /within/ the loop
      // AND the comparison overhead!`
      if(numLoops <= 5)
      {
         asm("nop");
         asm("nop");
      //   asm("nop");
      //   asm("nop");
      //   asm("nop");
      //   asm("nop");
      }
      else
      {
         asm("nop");
         asm("nop");
      }
   */
      //Apparently this loop will optimize-out without this:
      // Obviously, one instruction each...
      asm("nop");
      asm("nop");
   }

	//This addition a/o v62-20
	// When delay_cyc is called with a constant-value, it should optimize
	// out to merely the number of nops... but if it's non-constant, then
	// this will slow things a bit... UNTESTED.
	// (Actually, briefly tested with BLUE_DIAG_BAR_SCROLL and seems to
	// work, but not highly precise, as expected)

	//Realistically, this should probably be done in assembly, the whole
	//thing...
	// but doing-so might make optimization more difficult...?
	switch(loopRemainder)
	{
		case 7:
			asm("nop");
		case 6:
			asm("nop");
		case 5:
			asm("nop");
		case 4:
			asm("nop");
		case 3:
			asm("nop");
		case 2:
			asm("nop");
		case 1:
			asm("nop");
		default:
			break;
	}

}
#endif

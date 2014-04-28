/* mehPL:
 *    This is Open Source, but NOT GPL. I call it mehPL.
 *    I'm not too fond of long licenses at the top of the file.
 *    Please see the bottom.
 *    Enjoy!
 */


//writeColor() is a hokey method to draw individual pixels from a buffer,
//either a row-buffer or a frame-buffer.
// I say "hokey" because the associated drawPix() function has individual
// calls to writeColor(), rather than putting them in a for-loop... in
// order to save instruction-cycles, and increase resolution...

//As-Implemented
//Frame-Buffer:
// The pixels' color-data are stored in this buffer, two bits each R,G,B.
// Thus, it's easy to write a color-value, but all processing to convert
// that color-value into register-settings is done *during* the drawing...
// So, it's pretty slow to process each "pixel", and thus each pixel in the
// buffer is stretched horizontally across *several* physical LCD pixels
//Row-Buffer:
// This should be called "Row-Settings-Buffer" or something similar...
// The pixels' color-data is stored as packed register-values to be written
// at the time of drawing. The buffer itself does *not* consist of raw
// color-values, so it is a bit more difficult to explain.
// It still takes several instructions to load the registers for each
// color, so it still stretches each "pixel" across several LCD pixels.
// But it's significantly faster, so the horizontal resolution is higher.


//For Red and Green (NOT Blue) This enables four shades, instead of three
// (including black)
// Doing so increases pixel-processing time, thus the pixel-widths
// (thus decreasing resolution)
// each color takes 9 cycles to process in three-shade mode
// or 12 cycles for red and green, plus 9 for blue in four-shade mode
// a/o v59: I don't think this does anything in ROW_SEG_BUFFER
#define FOUR_SHADES TRUE

#if(defined(FOUR_SHADES) && FOUR_SHADES)
 //a/o v60
 //I can't find this anywhere else... Might not be looking hard enough
 #define	NUM_COLORS	(4*4*3) //48
#else
 #define NUM_COLORS (3*3*3) //27
#endif



#if(!defined(ROW_BUFFER) || !ROW_BUFFER)
#include "_options/frameBuffer.c"
#endif




// LVDS/FPD-Link timing:

//            |<--- (LCDdirectLVDS: "pixel") --->|
//  Timer1:   |<-- One Timer1 Cycle (OCR1C=6) -->|
//  TCNT:     |  0   1    2    3    4    5    6  |  0   1    2    3    
//            |____.____.____.____               |____.____.____.____
//  RXclk+:   /         |         \    .    .    /         |         \ //
//            |         |          ¯¯¯¯ ¯¯¯¯ ¯¯¯¯|         |
// One Pixel: |         |<--- One FPD-Link Pixel Cycle --->|
//            |                                  |
// "Blue/DVH" |____ ____v____ ____ ____v____ ____|____ ____
//  RXin2:    X B3 X B2 X DE X /V X /H X B5 X B4 X B3 X B2 X ...
//            |¯¯¯¯ ¯¯¯¯^¯¯¯¯ ¯¯¯¯ ¯¯¯¯^¯¯¯¯ ¯¯¯¯|¯¯¯¯ ¯¯¯¯
//            |         |<--Not Blue-->|         |
//            |                                  |
// "Green"    |____ ____v____ ____v____ ____ ____|____ ____
//  RXin1:    X G2 X G1 X B1 X B0 X G5 X G4 X G3 X G2 X G1 X ...
//            |¯¯¯¯ ¯¯¯¯^¯¯¯¯ ¯¯¯¯^¯¯¯¯ ¯¯¯¯ ¯¯¯¯|¯¯¯¯ ¯¯¯¯ 
//            |         |<------->|-Not Green    |
//
// "Red"      |____ ____v____v____ ____ ____ ____|____ ____
//  RXin0:    X R1 X R0 X G0 X R5 X R4 X R3 X R2 X R1 X R0 X ...
//            |¯¯¯¯ ¯¯¯¯^¯¯¯¯^¯¯¯¯ ¯¯¯¯ ¯¯¯¯ ¯¯¯¯ ¯¯¯¯ ¯¯¯¯
//            |         |<-->|-Not Red
//
//   Of course: The Not Green/Red bits above are low-bits and
//              basically have little/no visible effect
//
//




//#if(!defined(ROW_SEG_BUFFER) || !ROW_SEG_BUFFER)

#if(defined(__AVR_AT90PWM161__))
 #define WRITE_COLOR_CYCS 56
 //This value determined from .lss
 // probably not 100% accurate.
 //And kinda surprising it's so much more than the others...
#else
 #if(defined(ROW_BUFFER) && ROW_BUFFER)
 //THIS IS JUST AN ESTIMATE
  #define WRITE_COLOR_CYCS   (13)
 #elif(defined(FOUR_SHADES) && FOUR_SHADES)
  // Roughly...
  #define WRITE_COLOR_CYCS   (12*2+9+3)
 #else
  // Roughly...
  #define WRITE_COLOR_CYCS   (9*3+3)
 #endif
#endif

// DE_CYC = FB_WIDTH * ( WRITE_COLOR_CYCS + WRITE_COLOR_DELAY )
// DE_CYC/FB_WIDTH = WRITE_COLOR_CYCS + WRITE_COLOR_DELAY
// WRITE_COLOR_DELAY = DE_CYC/FB_WIDTH - WRITE_COLOR_CYCS
// Not sure why -4 is necessary... overhead in delayCyc?
#define WRITE_COLOR_DELAY \
	( DOTS_TO_CYC(DE_ACTIVE_DOTS) / FB_WIDTH - WRITE_COLOR_CYCS-4)
#if((WRITE_COLOR_DELAY < 0) || (WRITE_COLOR_DELAY >127))
 #define WRITE_COLOR_DELAY 0
 #warning "problem here..."
// #error "problem here..."
#endif

//load a color-value from the frame/row buffer and write the registers




static __inline__ 
void writeColor(uint8_t includeDelay, uint8_t colorVal) 
	__attribute__((__always_inline__));

#if(defined(__AVR_AT90PWM161__))

 void writeColor(uint8_t includeDelay, uint8_t colorVal)
 {
	uint8_t redVal = colorVal & 0x03;
	uint8_t greenVal = (colorVal >> 2) & 0x03;
	uint8_t blueVal = (colorVal >> 4) & 0x03;

	setRed4(redVal);
	setGreen4(greenVal);
	setBlue4(blueVal);

	if(includeDelay)
		delay_cyc(WRITE_COLOR_DELAY);		

 }
#if 0
 void writeColor(uint8_t includeDelay, uint8_t colorVal)
 {

#if(defined(ROW_BUFFER) && ROW_BUFFER)
	#error "ROW_BUFFER is NYI for the PWM161"
#endif


	 //This is pieced-together from the ATTiny861's writeColor()
	 // and memory...

	 //The basic idea is to set a color in a specific/non-varying number of
	 //instruction-cycles.
	 // Thus, we can't use "if" statements, etc, without paying attention to
	 // the actual assembly output.
	 // Additionally, it would be nice to know exactly how many cycles the
	 // entire function-call takes, so we can determine the number of pixels
	 // that can fit in a DE-active period... 
	 // (How many writeColor() calls can fit horizontally)
	 // Further still, this function is "always inline"
	 // which means that it will be optimized in each usage...
	 // which means that it might compile differently in different calls
	 // (e.g. if one call has a constant-argument and another has a
	 // variable)
	 // which is another reason to use inline-assembly, to assure it's the
	 // exact same number of clock-cycles each time.
	 // That's reserved for later...

	 // FURTHER STILL:
	 // the optimizer reorganizes things... duh.
	 // Which means that even though these calculations are listed at the
	 // top of the function...
	 // their actual instructions may not occur until later.
	 // The write to OCR-registers would ideally occur simultaneously
	 // such that the red/green/blue edges would align
	 // Without doing this in ASM, we can't guarantee (and I've seen
	 // first-hand) that it can't happen that one OCR is written, the next
	 // is calculated, *then* the next is written.

	 //First, unpack the colors:
	 uint8_t redVal = colorVal&0x03;
	 //uint8_t greenVal = (colorVal>>2)&0x03;
	 //uint8_t blueVal = (colorVal>>4)&0x03;
	 //The above is optimized below by combining two of the
	 //blue-shifts... It should be smaller
	 uint8_t bgTemp = colorVal>>2;
	 uint8_t greenVal = bgTemp&0x03;
	 uint8_t blueVal = (bgTemp>>2)&0x03;

	 //Now prep some variables for the register-values
	 // the actual registers will be written all at once at the end

	 //see setRed4() and setGreen4()...
	 uint8_t redOCR_val   = (redVal + 2);
	 uint8_t greenOCR_val = (greenVal + 3);

	 //Blue is more difficult, it requires a test
	 // which, when compiled, would likely result in two branches that take
	 // different numbers of clock-cycles
	 uint8_t blueOCR_val = (blueVal + 4);

	 // So far, none of the above code has any branches (right?)
	 // So it should be a constant number of clock-cycles
	 // (unless an inline-call has a constant as an argument!)

	 //The following is nothing more than 
	 // if (blueVal == 3) 
	 //     blueOCR_val=8;


	 //This is NOT AT ALL optimized:
	 // But it is guaranteed to be exactly 3 cycles *in each branch*
__asm__ __volatile__
	(                                                        // cycles
	 "cpi   %1, 3 ; \n\t"				// (blueVal == 3) ?     //    1
	 "brne  nothingToDo_%= ; \n\t"	// N: skip next line    // N:2  Y:1
	 "ldi   %0, 6 ; \n\t"            //   blueOCR_val=8      //   .    1
  "nothingToDo_%=: \n\t"            // just a jump-to label //   0    0
    : "=r" (blueOCR_val) //blueOCR_val is an output-value, "%0"
	 : "r" (blueVal)		//blueVal is an input-value, "%1"
	);

	//Now all the OCR_val variables are set
	// write them to the registers
	// Locking/unlocking of the PSCs can't occur simultaneously,
	// so it's likely there will be a pixel or two of difference between the
	// transition from one value of blue to the next vs red/green's
	// transition
	lockPSC2();
	lockPSC0();
//		OCR2RA = blueOCR_val;
//		OCR0SA = redOCR_val;
//		OCR0SB = greenOCR_val;
//"When addressing the I/O Registers as data space using LD and ST
// instructions, 0x20 must be added to these addresses"
//"For the Extended I/O space from 0x60-0xFF in SRAM, only the ST... and
//LD... instructions can be used"
// So, rather than using _SFR_IO_ADDR(), use _SFR_MEM_ADDR()
		__asm__ __volatile__
		(
		 "sts %3, %0; \n\t"
		 "sts %4, %1; \n\t"
		 "sts %5, %2; \n\t"
		 :
		 : "r" (blueOCR_val),	//"%0"
		   "r" (redOCR_val),		//"%1"
			"r" (greenOCR_val),	//"%2"
			"M" (_SFR_MEM_ADDR(OCR2RAL)), //"%3" //0x2e (0x4e)
			"M" (_SFR_MEM_ADDR(OCR0SAL)), //"%4"	//(0x60)
			"M" (_SFR_MEM_ADDR(OCR0SBL)) //"%5"  //0x22 (0x42)

		);
	unlockPSC0();
	unlockPSC2();


	//"Attempt to stretch across the full screen"
	// Not sure if/where this is used... 
	if(includeDelay)
		delay_cyc(WRITE_COLOR_DELAY);
 }
#endif



#endif










/*
static __inline__ \
void writeColor(uint8_t includeDelay, uint8_t colorVal) \
     __attribute__((__always_inline__));
*/



//This drawPix was developed before RowSegBuffer
// there are two versions included: RowBuffer and FrameBuffer
// which function almost identically as far as this function's concerned

// After writeColor()s are called, the remaining is (a/o v60) identical
// to rsb_drawPix, as it was a result of an:
//#if(!ROW_SEG_BUFFER)
// void drawPix(uint8_t rowNum)
// {
//  	do writeColorStuff...
//#else
// void drawPix(uint8_t rowNum)
// {
//		do rowSegBufferStuff...
//#endif
//    do remaining Stuff...
// }

// But nonRSB stuff hasn't been tested in quite some time...

void nonRSB_drawPix(uint16_t rowNum)
{
   //uint8_t *setting = &(settingBuffer[rowNum][0]);
#if(defined(ROW_BUFFER) && ROW_BUFFER)
   uint8_t *color = &(rowBuffer[0]);
#else
	rowNum = rowNum*FB_HEIGHT / V_COUNT;
//	rowNum &= 0x0f;
	uint8_t *color = &(frameBuffer[rowNum][0]);
#endif

	//a/o v67:
	// WTF... this didn't work without DEonly_init();
	// despite the fact that writeColor() on the 161 sets DE?!
	// AND it worked on the Tiny861 without this?!
	// No... writeColor doesn't change one register, which needs to be set
	// for blue...
	DEonly_init();
	//lockPSC2();
	//OCR2SA=1;	//WTF, 0 doesn't work?!
	//OCR2RA=4;
	//unlockPSC2();
   /*
      DEonly_fromNada();
      //Enable complementary-output for Green (on /OC1B, where CLK is OC1B)
      TCCR1A = ( (0<<COM1A1) | (1<<COM1A0)
               | (0<<COM1B1) | (1<<COM1B0)
               | (1<<PWM1A) | (1<<PWM1B) );
   */
      //The Greenish-bar on the left is due to the time it takes to execute
      // the first writeColor (since its value is only written at the END)
      // Thus the greenish-bar is about one write-color wide...

   //Judging by some weird experiences re v21/22,
   // it's not entirely likely this will be predictable
   // it may try to recalculate the Z register between writeBlues...
   // hopefully not, for now. I should probably assemblify this
      writeColor(FALSE, *(color+0));

		lvds_enableGreen_MakeClockSensitiveToDT();

		//Because includeDelay is FALSE, above, do it here...
		// The intention being to get enableGreen as soon after the
		// register-settings as possible.
		delay_cyc(WRITE_COLOR_DELAY);
		
      //Moving this here not only removes (most of) the green bar
      // but also seems to make the pixel edges significantly sharper
      // (v29 has ~1/8in of noise, v30 has ~1pixel noise at the right edge)
//      TCCR1A = ( (0<<COM1A1) | (1<<COM1A0)
//               | (0<<COM1B1) | (1<<COM1B0)
//               | (1<<PWM1A) | (1<<PWM1B) );

      writeColor(TRUE, *(color+1));    
      writeColor(TRUE, *(color+2));    
      writeColor(TRUE, *(color+3)); 
      writeColor(TRUE, *(color+4));    
      writeColor(TRUE, *(color+5));                
      writeColor(TRUE, *(color+6));  
      writeColor(TRUE, *(color+7));                         
      writeColor(TRUE, *(color+8));                         
		writeColor(TRUE, *(color+9));                         
      writeColor(TRUE, *(color+10));                         
      writeColor(TRUE, *(color+11));                         
      writeColor(TRUE, *(color+12));                         
      writeColor(TRUE, *(color+13));                         
      writeColor(TRUE, *(color+14));                         
      writeColor(TRUE, *(color+15));   
#define COLORS_WRITTEN   16      
#if ( (defined(COLOR_BAR_SCROLL) && COLOR_BAR_SCROLL) \
   || (defined(ROW_BUFFER) && (ROW_BUFFER)) )
      writeColor(TRUE, *(color+16));
      writeColor(TRUE, *(color+17));
      writeColor(TRUE, *(color+18));
      writeColor(TRUE, *(color+19));
writeColor(TRUE, *(color+20));
writeColor(TRUE, *(color+21));
writeColor(TRUE, *(color+22));
writeColor(TRUE, *(color+23));
writeColor(TRUE, *(color+24));
writeColor(TRUE, *(color+25));
writeColor(TRUE, *(color+26));
writeColor(TRUE, *(color+27));
#define COLORS_WRITTEN   28
#if (defined(ROW_BUFFER) && (ROW_BUFFER))
writeColor(TRUE, *(color+28));
writeColor(TRUE, *(color+29));
writeColor(TRUE, *(color+30));
writeColor(TRUE, *(color+31));
//Some sort of syncing problem after 32... (?)

writeColor(TRUE, *(color+32));
writeColor(TRUE, *(color+33));
writeColor(TRUE, *(color+34));
writeColor(TRUE, *(color+35));
writeColor(TRUE, *(color+36));
writeColor(TRUE, *(color+37));
writeColor(TRUE, *(color+38));
writeColor(TRUE, *(color+39));
writeColor(TRUE, *(color+40));
writeColor(TRUE, *(color+41));
writeColor(TRUE, *(color+42));
writeColor(TRUE, *(color+43));
writeColor(TRUE, *(color+44));
writeColor(TRUE, *(color+45));
writeColor(TRUE, *(color+46));
writeColor(TRUE, *(color+47));
writeColor(TRUE, *(color+48));
writeColor(TRUE, *(color+49));
writeColor(TRUE, *(color+50));
writeColor(TRUE, *(color+51));
writeColor(TRUE, *(color+52));
writeColor(TRUE, *(color+53));
writeColor(TRUE, *(color+54));
writeColor(TRUE, *(color+55));
writeColor(TRUE, *(color+56));
writeColor(TRUE, *(color+57));
writeColor(TRUE, *(color+58));
writeColor(TRUE, *(color+59));
writeColor(TRUE, *(color+60));
writeColor(TRUE, *(color+61));
writeColor(TRUE, *(color+62));
writeColor(TRUE, *(color+63));
// WriteColor writes the pixel *after* the calculations...
// thus the pixel appears basically after writeColor completes
// These nops assure the 64th pixel is fully-displayed before exitting
// (Not sure how the other following instructions apply to this)
// The number of nops was found experimentally...
asm("nop");
asm("nop");
asm("nop");
asm("nop");
asm("nop");
asm("nop");
asm("nop");
asm("nop");
//count "0" below, as well..
#define COLORS_WRITTEN 65
#endif //ROW_BUFFER
#else
//   writeColor(0);
#endif //COLOR_BARS || ROW_BUFFER
/*      reg[17] = colorBuffer[rowNum][17];                         
      writeColor(reg[17]);                         
      ...
      reg[20] = colorBuffer[rowNum][20];                         
      writeColor(reg[20]);  
      
      //REPEATING to fill screen... (delayDots = 342 worked prior to this)
      reg[0] = colorBuffer[rowNum][0];
      writeColor(reg[0]);
      ...
      reg[10] = colorBuffer[rowNum][10];
      writeColor(reg[10]);
*/
      //Display the rest as black...
      writeColor(FALSE, 0);
		//Do it as quickly as possible.
		// (No, this cuts off the last pixel...)
		//OCR1D = 0;	//Red Off
		//DT1 = 0;		//Green Off
		//OCR1A = 4;  //Blue Off

		//writeColor(0xff);
        //delay_Dots(500);//142); //Don't want to disable DE too early...   
      //900 leaves a buffer for various calculations while also showing
      // a blue bar at the right-side...
      //LTN Last Used 900
      // -68 is from 900's intent, IIRC
      //  seems arbitrary, but its value (especially if too small)
      // causes blank lines... (?!)
      // -60 makes more sense for a delay (was the original post-900)
      //  (outside DOTS_TO_CYC because it's for cycles used for calcs...
      // -68 worked for LVDS_PRE=2
      // -60 for 1
      // 4 doesn't work... blue-lines

//a/o v60
//From Here Down, everything has only been tested recently with rowSegBuf
// I did a nice #if-#else scheme which makes this redundant
// with what's in rsb_drawPix()...

// a/o v59-12ish... ROW_COMPLETION_DELAY uses were already commented-out
// BUT WHY WAS IT REMOVED?! Seems to help, now.
// 
//   Some Experimenting has led to the conclusion:
//   DE's active-duration needn't be exact. In fact, it can be *way* off
//     White is shown between the end of drawSegs, and cyan is shown after
//   ROW_COMPLETION_DELAY (which, for now, is constant, regardless of how
//    many pixels were drawn)
//   Almost immediately after the ROW_COMPLETION_DELAY (when it turns cyan)
//    DE is disabled
//    Yet the remainder of the screen still fills with cyan.
//   THUS: Disabling DE before the end of the screen appears to have the
//    effect of either not being acknowledged, or of repeating the last
//    color (untested)
//   Also, DE durations that are *longer* than the screen, seem to be 
//    absorbed by nonexistent pixels to the left...
//    (setting ROW_COMPLETION_DELAY==65535 unreasonably high,
//         just shows white at the right side, and still syncs)
//   Now, the original problem was that there seemed to be some carry-over
//   which maybe due to DEs that are EXTRAORDINARILY long?
//   NO!
//   Actually, it appears to be due to DEs that are TOO SHORT (?)
//      (setting ROW_COMPLETION_DELAY to 0 causes the problem again)
//   Doesn't appear to be *entirely* scientific, as using SEG_SINE
//    would suggest that these (now cyan) bars would appear at the troughs
//    in the diagonal-color-stripes at the top...
//    they seem, instead to be somewhat random, though maybe more common
//      at those locations.
//   But Wait! Setting ROW_COMPLETION_DELAY to 1 fixes it again.
//    realistically, that should be nothing more than a single nop; no?
//    (Maybe not, with a few cycles to entry, and minimum execution times)
//    a handful of nops does the trick, as well.
//    So is it a problem with too short a DE, or is it a matter of
//    e.g. the last segment drawn is setting new values that might only
//    be *completely transmitted* after a full PWM cycle...
//    So maybe somehow that last transaction is being interrupted
//     by the TCCR1A settings, or new values...
//    Plausible.
//
// FURTHER. Lest it be revisited. It was noted elsewhere that I thought
// this display was NOT DE-Only. In fact, the datasheet specifically says
// "DE-Only Mode"




/*#define ROW_COMPLETION_DELAY \
      (DOTS_TO_CYC(DE_ACTIVE_DOTS) -60  \
       - WRITE_COLOR_CYCS * COLORS_WRITTEN)
*/
//I think -60 was an arbitrary value just chosen to compensate for
//calculations/instructions...
// -90 has been found to be right at the edge of the screen, now...
// ish. maybe it's not accurate since delay_loop_2 is four instructions per
// loop...?
#define ROW_COMPLETION_DELAY \
      (DOTS_TO_CYC(DE_ACTIVE_DOTS) - 90  \
       - (WRITE_COLOR_CYCS + WRITE_COLOR_DELAY) * COLORS_WRITTEN)
//#define ROW_COMPLETION_DELAY 512 //1 //65535//512




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
#define ROW_COMPLETION_DELAY 0
#endif

		//a/o v62: (Original notes removed)
		//OCR1D controls RED... >=6 is full-red
		// Setting this here indicates where the drawing has completed
		// This is handy for determining timing, stretching, etc...
		//OCR1D = 6; //0;
		fullRed();

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
//		TCCR1A = ( (0<<COM1A1) | (1<<COM1A0)
//         | (1<<COM1B1) | (0<<COM1B0)
//         | (1<<PWM1A) | (1<<PWM1B) );

		lvds_disableGreen_MakeClockInsensitiveToDT();

      //fullBlue();
      //Nada_fromDEonly();
		Nada_init();
}



#if(!defined(__AVR_AT90PWM161__))
void writeColor(uint8_t includeDelay, uint8_t colorVal)
{
//#warning "I'm absolutely certain this'll need to be revised, probably asm"
   //   Red: (+OC1D => RX0+)
   //    Off (0/63): OCR1D = 0
   //    35/63:      OCR1D = 3
   //    63/63:      OCR1D >= 6

/* No Shit: This compiles to a 16-bit test!
   switch((uint8_t)(colorVal & (uint8_t)0x03))
   {
      case (uint8_t)0:
         OCR1D = 0;
         break;
      case (uint8_t)1:
         OCR1D = 3;
         break;
      case (uint8_t)2:
      default:
         OCR1D = 6;
         break;
   }
*/

#if(defined(ROW_BUFFER) && ROW_BUFFER)
   // In this case, colorVal is actually settingVal...
   // Between LDI, these instructions, and OCR/DT register writes
   // this is 14 cycles... or 16 pixels...

   //                              //ldi (colorVal) (2 cyc)
   //Red: (temp)
   uint8_t ocrd = colorVal >> 2;   //mov, shl, shl
   //Green:
   uint8_t dt = colorVal & 0x03; //andi
   //Blue:
   uint8_t ocra = ocrd >> 3;      //mov, shl, shl, shl
   //And red...
   ocrd &= 0x07;                  //andi
                                 //out OCRD, out DT, out OCRA

#else //NOT ROW_BUFFER (FRAMEBUFFER)

//a/o v67: I didn't make very clear notes here, but have been using it for
//quite some time. As I recall, the purpose of moving this to asm, instead
//of just using switch() statements, is in order to assure that regardless
//of the color-value, the same number of instruction-cycles will be
//executed... (otherwise, the color-value might affect the width of a
//pixel)

//   uint8_t redVal; // = colorVal & 0x03;
   uint8_t ocrd;

/*
   if(redVal == 0x00)
      ocrd = 0;
   else if(redVal == 0x01)
      ocrd = 3;
   else //2, 3
      ocrd = 6;
*/
#if(defined(FOUR_SHADES) && FOUR_SHADES)
 // "nop; nop; nop;" compiles to just a single nop! 
 //"\n\t" or maybe the space is necessary
 #define FOUR_SHADES_NOPS "nop ; \n\t nop ; \n\t nop ; \n\t"
#else
 #define FOUR_SHADES_NOPS "\n\t"
#endif

   //Each branch is 9 cycles... (12 with FOUR_SHADES)
__asm__ __volatile__
   ( "mov    %0, %1    ; \n\t"  // ocrd (redVal) = colorVal           //1
     "andi   %0, 0x03  ; \n\t"  // ocrd = ocrd & 0x03                 //1
     "brne   red1tst_%=; \n\t"  // if(ocrd != 0x00) jump to red1test  //1`2
     "ldi   %0, 0x00   ; \n\t"  // (ocrd==0x00) add some delays       //1 .
     "nop            ; \n\t"    //                                    //1 .
     "nop            ; \n\t"    //                                    //1 .
     "nop            ; \n\t"    //                                    //1 .
     FOUR_SHADES_NOPS           //                                    //N .
     "rjmp  end_%=   ; \n\t"    //   jump to the end                  //2 .
                                // (ocrd_reg = redVal_reg = 0)            .
   "red1tst_%=:"                //"%=" is a unique identifier for this asm.
                                //  invocation, so the label won't be     .
                                //  mistaken from another invocation      .
     "cpi   %0, 0x01   ; \n\t"  // if(ocrd-0x01 != 0)               //  1
     "brne   red23_%=   ; \n\t" //   jump to red=2,3                //  1`2
     FOUR_SHADES_NOPS           //                                  //  N .
     "ldi   %0, 0x03   ; \n\t"  // else ocrd = 0x03                 //  1 .
     "rjmp   end_%=   ; \n\t"   //      jump to the end             //  2 .
   "red23_%=:"                                                      //    .
#if (defined(FOUR_SHADES) && FOUR_SHADES)                           //   /.
     "cpi   %0, 0x02 ; \n\t"   // if(ocrd-0x02 !=0)               //( . 1
     "brne  red3_%=   ; \n\t"  //      jump to red=3              //( . 1`2
     "ldi   %0, 0x04 ; \n\t"   // else ocrd=4                     //( . 1 .
     "rjmp  end_%=   ; \n\t"   //      jump to the end            //( . 2 .
   "red3_%=:"                                                     //( .   /
#endif                                                            //(  \ /
     "ldi   %0, 0x06   ; \n\t"   // ocrd = 0x06                   //    1
     "nop            ; \n\t"  // one delay...                     //    1
  "end_%=:"

     : "=r" (ocrd)      //Output only "%0"
     : "r"  (colorVal)  //colorVal is "%1"
     //,  "d0"  (ocrd)     //ocrd is also used for andi, and is %2
   );


//   OCR1D = ocrd;


   //   Green: (/OC1B => RX1-)          (B1,0 Active, as well as G2,1)
   //    Off (6/63): DTL1 = 0
   //    38-39/63:      DTL1 = 1
   //    62-63/63:      DTL1 = 3
/*   switch(colorVal & 0x0C)
   {
      case 0x00:
         DT1 = 0;
         break;
      case 0x04:
         DT1 = 1;
         break;
      case 0x08:
      default:
         DT1 = 3;
         break;
   }
*/
//   uint8_t greenVal = colorVal & 0x0C;
   uint8_t dt;
/*   if(greenVal == 0x00)
      dt=0;
   else if(greenVal == 0x04)
      dt=1;
   else //0x06, 0x0C
      dt=3;
*/
   //Each branch is 9 cycles... (12 with FOUR_SHADES)
__asm__ __volatile__
   ( "mov   %0, %1   ; \n\t"  // dt (greenVal) = colorVal           //1
     "andi  %0, 0x0C ; \n\t"  // dt = dt & 0x0C                     //1
     "brne  grn4tst_%=; \n\t" // if(dt != 0x00) jump to grn4test    //1`2
     "ldi   %0, 0x00 ; \n\t"  // (dt==0x00) add some delays         //1 .
     "nop            ; \n\t"  //                                    //1 .
     "nop            ; \n\t"  //                                    //1 .
     "nop            ; \n\t"  //                                    //1 .
     FOUR_SHADES_NOPS         //                                    //N .
     "rjmp  end_%=   ; \n\t"  //   jump to the end                  //2 .
   "grn4tst_%=:"              //"%=" is a unique identifier for this asm.
                              //  invocation, so the label won't be     .
                              //  mistaken from another invocation      .
     "cpi   %0, 0x04 ; \n\t"  // if(dt-0x04 != 0)                   //  1
     "brne  grn8C_%= ; \n\t"  //   jump to green=8,C                //  1`2
     "ldi   %0, 0x01 ; \n\t"  // else dt = 0x01                     //  1 .
     FOUR_SHADES_NOPS         //                                    //  N .
     "rjmp  end_%=   ; \n\t"  //      jump to the end               //  2 .
   "grn8C_%=:"                                                      //    .
#if (defined(FOUR_SHADES) && FOUR_SHADES)                           //   /.
     "cpi   %0, 0x08 ; \n\t"  // if(dt-0x08 !=0)                  //( . 1
     "brne  grn3_%=  ; \n\t"  //    jump to green=3               //( . 1`2
     "ldi   %0, 0x02 ; \n\t"  // else dt=2                        //( . 1 .
     "rjmp  end_%=   ; \n\t"  //      jump to the end             //( . 2 .
   "grn3_%=:"                                                     //( .   /
#endif                                                            //(  \ /
     "ldi   %0, 0x03 ; \n\t"  // dt = 0x03                        //    1
     "nop            ; \n\t"  // one delay...                     //    1
   "end_%=:"

     : "=r" (dt)      //Output only "%0"
     : "r"  (colorVal)  //colorVal is "%1"
     //,  "d0"  (ocrd)     //ocrd is also used for andi, and is %2
   );



//   DT1 = dt;
   //   Blue: (+OC1A => RX2+)               (B3,2 Active from here down)
   //    Off (15/63):  OCR1A=4
   //    47/63:        OCR1A=5
   //    63/63:        OCR1A=6
/*   switch(colorVal & 0x30)
   {
      case 0x00:
         OCR1A = 4;
         break;
      case 0x10:
         OCR1A = 5;
         break;
      case 0x20:
      default:
         OCR1A = 6;
         break;
   }
*/
//   uint8_t blueVal = colorVal & 0x30;
   uint8_t ocra;
/*   if(blueVal == 0x00)
      ocra=4;
   else if(blueVal == 0x10)
      ocra=5;
   else //0x20, 0x30
      ocra=6;
*/

   //Each branch is 9 cycles...
__asm__ __volatile__
   ( "mov   %0, %1   ; \n\t"  // ocra (blueVal) = colorVal          //1
     "andi  %0, 0x30 ; \n\t"  // ocra = ocra & 0x30                 //1
     "brne  blu1tst_%=; \n\t" // if(ocra != 0x00) jump to red1test  //1`2
     "ldi   %0, 0x04 ; \n\t"  // (ocra==0x00) add some delays       //1 .
     "nop            ; \n\t"  //                                    //1 .
     "nop            ; \n\t"  //                                    //1 .
     "nop            ; \n\t"  //                                    //1 .
     "rjmp  end_%=   ; \n\t"  //   jump to the end                  //2 .
                              // (ocra_reg = blueVal_reg = 0)            .
   "blu1tst_%=:"              //"%=" is a unique identifier for this asm.
                              //  invocation, so the label won't be     .
                              //  mistaken from another invocation      .
     "cpi   %0, 0x10 ; \n\t"  // if(ocra-0x10 != 0)                 //  1
     "brne  blu23_%= ; \n\t"  //   jump to red=2,3                  //  1`2
     "ldi   %0, 0x05 ; \n\t"  // else ocra = 0x05                   //  1 .
     "rjmp  end_%=   ; \n\t"  //      jump to the end               //  2 .
   "blu23_%=:"                                                      //    .
     "ldi   %0, 0x06 ; \n\t"  // ocra = 0x06                        //    1
     "nop            ; \n\t"  // one delay...                       //    1
   "end_%=:"

     : "=r" (ocra)      //Output only "%0"
     : "r"  (colorVal)  //colorVal is "%1"
     //,  "d0"  (ocra)     //ocra is also used for andi, and is %2
   );

#endif //SETTING vs. FRAMEBUFFER

   DT1 = dt;
   OCR1D = ocrd;
   OCR1A = ocra;

	if(includeDelay)
		//Attempt to stretch across the full screen...
		delay_cyc(WRITE_COLOR_DELAY);
}
#endif //AT90PWM161
//#endif //!ROW_SEG_BUFFER



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
 *    2) Please do not change/remove others' credit/licensing/copywrite 
 *         info, where noted. 
 *    3) If you find yourself profitting from my work, please send me a
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
 *    (Wow, that's a lot longer than I'd hoped).
 *
 *    Enjoy!
 */

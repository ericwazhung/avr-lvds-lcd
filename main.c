#include "projInfo.h"   //Don't include in main.h 'cause that's included in other .c's?
#include "main.h"
#include <util/delay.h> //For delay_us in pll_enable
#include <avr/pgmspace.h>
#include _TIMERCOMMON_HEADER_
//#include "../../../_commonCode/charBitmap/0.10/charBitmap.h"
#include _CHARBITMAP_HEADER_
#include "fbColors.h"

#include _SINETABLE_HEADER_



#include "mainConfig.h"


#include "delayCyc.h"

#if (defined(ROW_SEG_BUFFER) && ROW_SEG_BUFFER)
	 #include "rowSegBuffer.h"
#endif

//Nope, can't be removed yet...
//#define ROW_BUFFER FALSE
//#define LOADROW TRUE
#if (defined(ROW_BUFFER) && ROW_BUFFER)
 #include "rowBuffer.c"
#endif

#if (defined(SEG_TET) && SEG_TET)
 #include "tetStuff.c"
#endif




// a/o v59:
//PSEGS are the "pixel segments" addressable in RowSegmentBuffer
// e.g. one could load a single row-segment NUM_PSEGS long, and it would 
// reach from one edge of the screen to the other
// or any number <=NUM_SEGMENTS of row segments whose lengths all add up to
// <=NUM_PSEGS. (It's possible to extend beyond NUM_PSEGS, which would draw
// beyond the edge of the screen, but it may affect the screen's ability 
// to sync properly)
// This value isn't exactly adjustable, it's a characteristic of the
// ratio of the pixel-clock and the system-clock, along with the number of
// system clock cycles used to draw each pixel.
// This value was determined by experimentation with the very specific 
// configuration currently in use, though it could probably be calculated 
// from other #defines elsewhere.
// (and probably *should* be, at some point, especially with 
//  experimentations with PLLSYSCLK, LVDS_PRESCALER, SLOW_LVDS, etc...)
// For now, changing this doesn't really do anything except scale some 
// drawings horizontally (E.G. SEG_RACER)
//These notes were determined long ago when developing SEG_RACER:
// Wasn't sure where 357 came from... and now I'm wondering where the
// notes I wrote regarding it disappeared to...
// Anyhow, from line 72, if ROW_SEG_BUFFER && !LVDS_PRESCALER
// "ROW_SEG_BUFFER uses 20cyc/pixel" (doesn't match the math I've 
// elsewhere) 1024/20*7 = 358.4... so maybe that's something to do with it
// TODO: If it was 21cyc/pixel, wouldn't we have perfect alignment with
//       color-transitions and segment widths?
//#define NUM_PSEGS	(1024/3+16) //357
#define NUM_PSEGS (1024/3+16)


#include "delayCyc.c"
#include "lcdDefines.h"
//Yes, I know #including C files is bad-practice...
// and yes, this has to be here... for now.
#if(defined(SEG_RACER) && SEG_RACER)
 #include "seg_racer.c"
#endif





// a/o v60, this note is OLD, and actually is the whole point behind
// ROW_SEG_BUFFER's current implementation.
// TODO: Between OSCCAL_VAL and FRAME_UPDATE_DELAY
//  it seems the speed (at least for the LTN) isn't so important after all
//  This could be used to my advantage...
//  Maybe bump OSCCAL_VAL all the way up, but use a slower "bit-rate"
//  Thus, pixels could be skinnier, thus higher-resolution.
//   Or more instructions could be used for the same resolution
//   allowing, e.g. sprites from program-memory, without necessitating a
//   large (or any?) frame-buffer...


//This used to be handled by TOOFAST_TEST, but now can be configured here
// 0xff sets the processor to run as fast as possible 
//  (~16MHz? ~128MHz bit-rate)
// 0x00 sets it as slow as possible (~4MHz, ~32MHz bit-rate)
//  good for testing whether odd colors
//  or bad syncing is due to bits being shifted, etc. 
//  Maybe due to slow "LVDS" conversion chips, or whatnot.
//   This seems to be the case right now... 0xff: black is appearing green
//   There should be *some* green in it, but not this much.
//   Likely due to different propagation delays between my under-driven
//    74LS86's (running red and green), from 3.3V!
//    and the happier and newer 74AHC XOR and OR currently driving the
//    clock and DVH/Blue signals
//  Highly dependent on the screen itself, some may not run at all with
//  such slow bitrates... heck, some may not run at all even at the fastest
// If SLOW_EVERYTHING_TEST is true, this value is overridden
// LTN last used 0x20
// a/o v59:
// 0xD8 is the lowest value that syncs reliably, a few glitches from time
// to time.
// The effect of lower values isn't *quite* what I was expecting, but
// definitely suggests that using ROW_SEG_BUFFER at this bit/refresh-rate
// is dang-near right at the minimum required for this display.
// This might be why the other (same model) display didn't work, but its
// flakeyness at 0xff appeared different than this one's at lower OSCCALs
// --Later note... actually retrying the "flakey" display with the same
//   configuration. Actually, yes, the problem looks dang-near identical to
//   the "reliable" display's being run at OSCCAL_VALs lower than 0xD8
//   Both displays are of the same manufacturer/model.
//    "reliable" has the chip: LXD91810 VS252AG
//    "flakey"   has the chip: LXD91810 VS242AC
//   The only bit I don't get is that I'm dang-near certain I had both
//   displays working at one point, and from the same code...
//   but this OSCCAL value is the maximum, and the only other way to speed
//   up the bit-rate changes the resolution; which I haven't done
//   for quite some time.
//   Also different about this test than when I originally determined it
//   "flakey": I've since switched the 74LS00 and LS32 out for LS86's
//   (These are wired such that one chip is responsible for the + side of
//    every LVDS signal, and one chip is responsible for the - side.
//    The best-practice, probably, would be to have the + and - sides of
//    an LVDS signal on the same chip. It's also plausible that my test was
//    so long ago that it was on the old board which had this configuration
//    but I doubt that, since that board was so hokily-implemented (with
//    chip-clips and grabbers all over the place) that I can't imagine it
//    was ever running again after all those months of disuse. Though...
//    if that *was* the case... it could also be that I'm using a different
//    ATtiny861... in which case, maybe the RC oscillator and/or PLL aren't
//    capable of running as fast... 
//    Not that I have a ton of these things lying around (three total, in
//    various projects already)
//    BUT the end-result may be that I just happen to have the one display
//    and ATtiny861 on the planet that will work together. That'd be lame.)
#define OSCCAL_VAL	0xff//0xDB//0xDC//0xE0//0//0xff//0x20//0xff //0x00





//a/o v60: These are old experiments...
// they *are useful* for testing new displays...
// but I can't recall off-hand how to use them instead of (or with?)
// rowSegBuffer...
// They're defined and better-described in lcdStuff.c...
//These three are mutually-exclusive (BLUE_VERT_BAR overrides DE_BLUE)
// (BLUE_DIAG_BAR overrides both...)
//#define BLUE_DIAG_BAR TRUE
//A single black bar, followed by a single blue bar... ~1/3 and 2/3's width
//#define BLUE_VERT_BAR TRUE
//#define DE_BLUE TRUE
// OLD NOTE:
// NONE SELECTED DOESN'T SYNC!!! DE not detected???



// Default is drawPix, from program memory...
// a/o v60: drawPix() has since become the de-facto function for drawing
// a row of pixels, regardless of the method.
// It's defined as necessary...



//This is where COLOR_BAR_SCROLL and PIXEL_SCROLL were, in part.





#include "pll.c"

#include "lvds.c"




static __inline__ \
void loadRow(uint16_t rowNum) \
	__attribute__((__always_inline__));


#include _LCDSTUFF_CFILE_
//#include "../../../_commonCode/lcdStuff/0.50ncf/lcdStuff.c"




//Nearly everything display-related happens in this interrupt
// It even calls the functions that load (and calculate!) the data for the
// next row.
// So basically, the entire project is running via timer-interrupt.
SIGNAL(TIMER0_COMPA_vect)
{
	static uint8_t frameCount = 0;

	//Could insert a delay of sorts for scopability...
	// (otherwise there's not much guarantee that register-assignments
	//  later will align with the LVDS frame... though ALIGN should help)
	// see scopeHsync.c


	if(updateLCD())
	{
		frameCount++;
		//updateLCD() returns TRUE when the frame is complete
		// which can be used for whatever purposes 
		// (e.g. FRAME_COUNT_TO_DELAY)
	}

#if(defined(LOADROW) && LOADROW)
	//Load the next row into the row(seg)buffer
	//data is enabled after T_VD+T_Vlow...
	//a/o v60: Rereading this is confusing...
	// Basically, there's no reason to call loadRow for display-lines
	// which don't display data (e.g. V-Sync)
	if((hsyncCount >= T_VD+T_Vlow) && (hsyncCount < T_VD+T_Vlow+V_COUNT))
		loadRow(hsyncCount - (T_VD+T_Vlow));
#endif
	// a/o v60: This is a funny old note... I can't recall a time when
	// loadRow was written, then removed, then added again. Weird:
	// Here is where loadRow used to be called
	// it has been moved to oldNotes.txt, but it would make more sense
	// to look at LCDdirectLVDS <25 or LCDdirect
	// loadRow used the time between DE inactive and the next Hsync
	// to load a row to a row-buffer (as opposed to a frame-buffer)
	// thus, DE could gain more pixels because there would be fewer
	// calculations to determine the memory location to write from
	// This effect has since been minimized
	// BUT loadRow might be necessary for program-memory-based images...
}



#if(defined(SEG_HFM) && SEG_HFM)
	#include "seg_hfm.c"
#endif

#if(defined(SEG_QUESTION) && SEG_QUESTION)
	#include "seg_question.c"
#endif

#if(defined(SEG_SINE) && SEG_SINE)
	#include "seg_sine.c"
#endif

#if(defined(LOADROW) && LOADROW)
// whatever this means... it's OLD.
//#warning "loadRow is currently in an intermediate phase..."
void loadRow(uint16_t rowNum)
{


#if(defined(ROW_SEG_BUFFER) && ROW_SEG_BUFFER)
 #if (defined(SEG_RACER) && SEG_RACER)
	racer_loadRow(rowNum);
 #elif (defined(SEG_HFM) && SEG_HFM)
 	segHFM_loadRow(rowNum);
 #elif (defined(SEG_QUESTION) && SEG_QUESTION)
 	segQuestion_loadRow(rowNum);
 #elif (defined(SEG_SINE) && SEG_SINE)
 	segSine_loadRow(rowNum);
 #elif (defined(SEG_LINE) && SEG_LINE)
	//Left here for an example...
	// Draws a diagonal line 
	//OLD NOTE:
	//syncing issues due to recursion overflowing the stack???
	// Apparently was
	   
	//Isn't BLAH = 1 necessary so we don't get a row with no data?
	// *looks* like it's working, but I dunno...
	#define BLAH 0

	segClear();
	newSeg(3,0x06, (6<<4) | 3);									//W
	newSeg((rowNum&0xff) | BLAH, 0x06, (4<<4) | 0);				//R
	newSeg(1,0x06, (6<<4) | 3);									//W
	newSeg((255-(rowNum&0xff)) | BLAH, 0x06, (4<<4) | 0 );	//R
	newSeg(3,0x06, (6<<4) | 3);									//W
	segTerminate();

 #elif (defined(SEG_TET) && SEG_TET)
	//This isn't particularly functional, anymore
	// it used to be an intermediate stage between rowBuffer and 
	//  rowSegBuffer... Left here for an example of how it could be done...
  #if(!defined(SEG_STRETCH))
	#define SEG_STRETCH 5 //(((NUM_PSEGS-6)+RB_WIDTH-1)/RB_WIDTH)
  #endif
	//3-5 = white + cyan
	//6 = letters alternating with above
	//7-9 = ditto, stretched

#define TET_VSTRETCH	16
	if(rowNum == 0)
	{
		//Probably not best to put this here, as we're still in the interrupt
		// extra-long calculations might cause syncing issues with displays
		// that require rows to be a constant time
		tetUpdate();
	}
	if(rowNum % TET_VSTRETCH == 0)
	{
		uint8_t i;
		for(i=0; i<RB_WIDTH; i++)
			rowBuffer[i] = fb_to_rb(_K);
		//	rowBuffer[i] = fb_to_rb((i+rowNum/TET_VSTRETCH)&0x3f);
		
		tet_drawRow(rowNum/TET_VSTRETCH, rowBuffer);
	}
	segClear();

	//Good for syncing to have white on both borders...
	newSeg(3, 0x06, (6<<4) | 3);
	
	uint16_t i;
	//TET_OVERLAY alternates rows between the TETRIS board and
	// an rgb "gradient" The only purpose is to test the idea of
	// using the high vertical-resolution to essentially increase the
	// number of colors available to a low-resolution image
	// sort of like "dithering" in the ol' days of 256 colors in Windows
	// but easy to implement in this case with very little overhead 
	// row-by-row
	// The effect, from a ways back, looks like the colors are blended
	// (like the Text and Game-board are translucent)
#define TET_OVERLAY TRUE

	//TET_GRADIENT takes that a step further and attempts to create a
	// gradient between each color in that rgb-gradient.
	// It's not as pretty as I'd hoped... though it makes sense...
	// first we're alternating between TET's color and the background color
	// then the background color is (roughly) alternating between a couple
	// colors, several rows between...
	// so if a color is stretched 16 rows vertically, that only leaves 8
	// rows for the color-gradient... might work better over more gradual
	// color-changes
#define TET_GRADIENT TRUE

#if (defined(TET_OVERLAY) && TET_OVERLAY)
	if(rowNum & 0x01)
	{
#endif
	for(i=0; i<RB_WIDTH; i++)
	{
		//i+1 because we don't want to overwrite the white border...
		rbpix_to_seg(rowBuffer[i], i+1, SEG_STRETCH);
	}

#if (defined(TET_OVERLAY) && TET_OVERLAY)
	}
	else
	{
		#define ROWS_PER 64//(V_COUNT/NUM_COLORS)

		uint8_t fbColor;

#if (defined(TET_GRADIENT) && TET_GRADIENT)
		static hfm_t hfmGradient;
		if(rowNum%ROWS_PER == 0)
			hfm_setup(&hfmGradient, 0, ROWS_PER/2-4);

		//It's worth it to experiment with changing the range of the 
		// hfmGradient, ('-4' above and '+4' below).
		// to try to avoid one or two stray spikes
		// (e.g. power = 1, maxPower = 15, there'll be one bright row
		//  and 14 dark, it sticks out like a sore-thumb)
		//  These values are experimental, and entirely dependent on the
		//  values used...

		hfm_setPower(&hfmGradient, (rowNum/2)%(ROWS_PER/2)+4);

		if(hfm_nextOutput(&hfmGradient))
			fbColor = rgbGradient(ROWS_PER-1 - rowNum/(ROWS_PER)+16-1);
		else
#endif
			fbColor = rgbGradient(ROWS_PER-1 - rowNum/(ROWS_PER)+16);

		addSegfb(RB_WIDTH*SEG_STRETCH, (fbColor));
		//newSeg(SEG_STRETCH, fb_to_seg((rowNum*64/768)&0x3f));

	}
#endif
	//white...
	newSeg(3, 0x06, (6<<4) | 3);
	segTerminate();
 #elif (defined(SEG_GRADIENT) && SEG_GRADIENT)
	static hfm_t hfmGradient;
#define ROWS_PER	(255)	//(V_COUNT/3) == 256

	if(rowNum%ROWS_PER == 0)
		hfm_setup(&hfmGradient, 0, ROWS_PER);

	uint8_t color;

	uint16_t power;

	if(rowNum < ROWS_PER/16)
		rowNum = 0;
	else
		rowNum -= ROWS_PER/16;

	//Since there are four shades, there will be three gradients...
	if(rowNum < ROWS_PER)
	{
		//The ROWS_PER/16 stuff is to help alleviate sharp color-spikes
		// which occur early-on... see the explanation in SEG_TET.
		color = 0;
		//hfm_setPower(&hfmGradient, 
		power = (rowNum); // + ROWS_PER/16);
	}
	else if(rowNum < ROWS_PER*2)
	{
		color = 1;
		//hfm_setPower(&hfmGradient, 
		power = rowNum-(ROWS_PER); // + ROWS_PER/16;
	}
	else
	{
		color = 2;
		//hfm_setPower(&hfmGradient, 
		power = rowNum-(ROWS_PER*2); // + ROWS_PER/16;
	}


	if(power < ROWS_PER/16)
		power = 0;

	hfm_setPower(&hfmGradient, (power <= 255) ? power : 255);

	if(hfm_nextOutput(&hfmGradient))
		color++;

	color |= color<<2 | color<<4;

	segClear();
	addSegfb(3, _W);
	addSegfb(NUM_PSEGS-6, color); 
	addSegfb(3, _W);
	segTerminate();
 #elif(defined(SEG_GRADIENT2) && SEG_GRADIENT2)
#error "Heh, never did implement this..."
 #else
	#error "Gotta select a SEG_... option, or create your own"
 #endif //SEG_ selection
#endif //ROW_SEG_BUFFER
}
#endif //LOAD_ROW

#include "timer0Stuff.c"



#if (defined(ROW_SEG_BUFFER) && ROW_SEG_BUFFER)
void drawPix(uint8_t rowNum)
{
	//Note that rowNum isn't really used here...
	// and it's only a uint8_t!
	rsb_drawPix(rowNum);
}
#else
#include "nonRSB_drawPix.c"
void drawPix(uint8_t rowNum)
{
	//This hasn't been used in quite some time... 
	// it may not work at all anymore.
	nonRSB_drawPix();
}
#endif



int main(void)
{

#if(defined(SEG_RACER) && SEG_RACER)
	racer_init();
#endif

#if(defined(SEG_TET) && SEG_TET)
	tetInit(3);
#endif
	init_timer0Hsync();

	//This starts pretty late... watch out for WDT
	init_heartBeat();

	setHeartRate(0);

	lvds_timerInit();


	// MUCH Of this is outside the screen...


	uint8_t imageNum = 0;
	uint8_t colorShift = 0;
	while(1)
	{
		heartUpdate();
	}

}

#include "rowSegBuffer.c"

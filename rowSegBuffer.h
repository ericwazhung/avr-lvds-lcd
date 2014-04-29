/* mehPL:
 *    This is Open Source, but NOT GPL. I call it mehPL.
 *    I'm not too fond of long licenses at the top of the file.
 *    Please see the bottom.
 *    Enjoy!
 */






// Please see rowSegBuffer.c for a functionality-description


#ifndef __ROW_SEG_BUFFER_H__
#define __ROW_SEG_BUFFER_H__

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




//a/o v62: This has been determined in drawSeg()...
#define DRAWSEG_CPU_CYCS_PER_PSEG 20


#ifndef NUM_SEGMENTS	
//128 works with SEG_SINE, shows just a tiny bit of early-termination
// This value can be bumped down or up as memory allows...
// (E.G. If your project has high memory-requirements, but few changes
//  in color horizontally, then reduce this...)
// SEG_RACER uses quite a bit of memory for the course, and requires this
// to be low, like 68...
	#define NUM_SEGMENTS	60//64//128//96//68//128 //68 //128//68
#endif

//Green and blue are in the same byte...
// I don't think the code will compile anymore without this.
// It increases the pixel calculation from 18 cycles to 20 cycles
#define GB_COMBINED TRUE

// SQUARE_SEGMENTS inserts nops in the counter-loop
// such that each loop takes the equivalent time as the first load/write
//  of the values...
// Then, each incremet of length is equivalent to one drawable-pixel
// (Later, maybe, setting this FALSE would allow for LCD pixel-resolution
//  down to a single pixel, with LVDS_PRESCALER, etc. BUT, the minimum 
//  width of a segment would be longer, due to higher calculation time
//  at the beginning)
// Another benefit of SQUARE_SEGMENTS is that more distance can be
//  covered in the same count... (fewer segments necessary in memory for a
//  long color-segment)
// Another "benefit" is that all count values can be used,
//  (less testing is necessary to make sure a pixel can be added)
//  (meh...)
#define SQUARE_SEGMENTS TRUE

//Segments have four values:
// segment length
// red	(OCR)
// green	(DT)
// blue	(OCR)
//
// To save space, and no additional instruction cycles are needed,
//  combine red and length
//  red_length bit:  7 6 5 4 3 2 1 0
//                   \__ __/ \__ __/
//								V       V
//                      |       Red OCR value
//                      segLength

// NOW:
//  red_length bit:  7 6 5 4 3 2 1 0
//                   \___ ___/ \_ _/
//                       V       V
//                       |       Red OCR value
//                       segLength
// RISK:
//  At one point I was considering doubling OCR/DT values
//   in able to get higher resolution...
//   (more CPU cycles per pixel clock)
//  That would require a significant overhaul
//   and LVDS_PRESCALER has already helped...
#define SEG_LENGTH_BITS 5

//#if (defined(SEG_LENGTH_BITS))
 #define SEG_LENGTH_MASK 	((UPPER_BIT_MASK8(SEG_LENGTH_BITS)))
//&0xff is necessary for an assembly operand to be 8-bits
 #define RED_MASK 			(((~(SEG_LENGTH_MASK))&0xff))
 #define SEG_COUNT_1			((UPPER_BIT_MASK8__COUNT_1(SEG_LENGTH_BITS)))
 #define SEG_LENGTH_SHIFT	((8-(SEG_LENGTH_BITS)))
//#else
// #define SEG_LENGTH_MASK (0xf0)
// #define RED_MASK (~(SEG_LENGTH_MASK))
//#endif


//This seems backwards, but it's not. See SQUARE_SEGMENTS notes, above
#if (defined(SQUARE_SEGMENTS) && SQUARE_SEGMENTS)
 #define COUNT_INCREMENT SEG_COUNT_1 //0x10
 //This is the actual length, not as shifted for storage
 #define SEG_MAXLENGTH	(SEG_LENGTH_MASK >> SEG_LENGTH_SHIFT)
#else
#error "This probably isn't implemented anymore..."
 #if (defined(GB_COMBINED) && GB_COMBINED)
  #define COUNT_INCREMENT (5*SEG_COUNT_1) //0x50
 #else
  #define COUNT_INCREMENT (4*SEG_COUNT_1) //0x40
 #endif
#endif


// Similar *might* be possible with green and blue
// IF we can switch DE/Blue to /OC1A
//           switch Green to OC1B
//           use DTH for green -> mov, andi, andi
// Not sure if this is possible
// Otherwise, sharing a byte for green/blue introduces
//            mov, andi, and four lsrs
//            mov, andi, swap, andi... maybe not so bad...?
//                       only two additional cycles (since ld is two)
// the benefit with red_length is killing two birds with one stone
//   andi both &='s AND tests for 0...
typedef struct _NONAME_
{
	uint8_t red_length;	// segLength<<3 | OCR1D
#if (defined(GB_COMBINED) && GB_COMBINED)
	uint8_t green_blue;	// blue<<4 | green
#else
	uint8_t green;			//DT1(Low nibble)
	uint8_t blue;			//OCR1A
#endif
} seg_t;


extern seg_t rowSegBuffer[NUM_SEGMENTS+1];


//See rowBuffer.c for an explanation...
// The first three will probably seldom be used
// The last....
#define fbBlue_to_seg(fbColor) \
		(((((fbColor) & 0x30) | 0x40)))
		//((((fbColor & 0x30) >> 4) | 0x04)<<4) //fixed
//	   (((fbColor & 0x30) << 1) | 0x40)

#define fbGreen_to_seg(fbColor) \
	   (((fbColor) & 0x0C) >> 2)

#define fbRed_to_seg(fbColor) \
	   ((((fbColor) & 0x03) << 1) | ((fbColor) & 0x01))

//This gives 'red, green_blue' pairs for arguments to newSeg, etc.
// It's kinda hokey to call a three-argument function with *apparently*
// only two arguments, but this'll be used probably more often than
// the actual function-call...
// use newSegfb(length, fbColor) 
//    instead of newSeg(length, fb_to_seg(fbColor))
// it's just a macro, but it makes more sense
#define fb_to_seg(fbColor) \
  fbRed_to_seg(fbColor), (fbBlue_to_seg(fbColor) | fbGreen_to_seg(fbColor))

//Outside of rbpix_to_seg, this corresponds with the last-written segment
extern uint8_t segPosition;


uint8_t newSeg(uint16_t length, uint8_t red, uint8_t green_blue);
uint8_t stretchSeg(uint16_t additionalLength);

void segClear(void);

#define rbpix_to_segTerminate segTerminate

void segTerminate(void);


#define newSegfb(length, fbColor) newSeg((length), fb_to_seg(fbColor))
#define addSegfb(length, fbColor) addSeg((length), fb_to_seg(fbColor))

//This is just an intermediate test, for now...
// for reloading the rowbuffer to a seg-buffer.
//This could easily be revised (and generalized) to be...
// addSegment or addPixel
void rbpix_to_seg(uint8_t rbValue, uint8_t col, uint8_t length);

//Call this when we already know the color in the last segment is the same
// so we wish to stretch the segment by additionalLength
// these values are real-lengths, not as shifted for storage...
// Return TRUE if the stretch was truncated...
// THIS DOES NOT TEST DISPLAY_WIDTH!
// DON'T FORGET to make additionalLength square before the call
// if necesary....
uint8_t stretchSeg(uint16_t additionalLength);

//This should be like newSeg, but also checks whether it can stretch
// the last segment...
uint8_t addSeg(uint16_t length, uint8_t red, uint8_t green_blue);

//This assumes you intend to create a new segment
// it does *not* check whether the previous segment could be stretched
// (it in fact, is called by stretchSeg as necessary)
// Returns TRUE if it had to be truncated
//  (out of segment memory, past the display edge...)
// length is the real length, not shifted for storage...
// THIS DOES NOT squarify, so adjust length as necessary before calling
uint8_t newSeg(uint16_t length, uint8_t red, uint8_t green_blue);


static __inline__ \
void drawSegs(void) \
	__attribute__((__always_inline__));



//Note that rowNum is not used, AND it's a uint8_t! There's some note about
// this in lcdStuff.c, now. Not particularly helpful, but gives enough
// history I might be able to piece this stuff together and clean it up.
void rsb_drawPix(uint8_t rowNum);

#if (defined(GB_COMBINED) && GB_COMBINED)
 #define WRITE_COLOR_CYCS 20
#else
 #define WRITE_COLOR_CYCS 16
#endif


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
#define ROW_COMPLETION_DELAY 512 //1 //65535//512


#endif
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
 *    2) Please do not change/remove others' credit/licensing/copyright 
 *         info, where noted. 
 *    3) If you find yourself profiting from my work, please send me a
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
 * This license added to the original file located at:
 * /Users/meh/_avrProjects/LCDdirectLVDS/68-backToLTN/rowSegBuffer.h
 *
 *    (Wow, that's a lot longer than I'd hoped).
 *
 *    Enjoy!
 */

/* mehPL:
 *    This is Open Source, but NOT GPL. I call it mehPL.
 *    I'm not too fond of long licenses at the top of the file.
 *    Please see the bottom.
 *    Enjoy!
 */




//This file is to be #included in <project>/mainConfig.h 
// when ROWBUFFER_TESTING is true...

//Modify (comment/uncomment) options here as you wish.

#ifndef __ROWBUFFER_TESTING_H__
#define __ROWBUFFER_TESTING_H__



//This file is used for configuring the project at a high-level...
// Most configuration-options are simplified, here, by uncommenting various
// options.
//
// RowBuffer functions by calculating each row during the
// horizontal-front-porch... thus, whereas a 16x16 framebuffer could store
// 256 pixels' worth of data, now that same 256 pixels' worth of data can
// be applied to *each row* (regardless of how many rows are on the
// display)
//
// This method may not work with most displays... So far it's only been
// tested-functional with the Samsung LTN...
//
// The problem is, quite a bit of processing is necessary to calculate,
// e.g. 256 pixels, and those calculations must be done during *each* row.
// Some displays don't particularly like long horizontal-blanking periods.
// Others don't like when that blanking-period varies from row-to-row
// (as it would when one row is easier to calculate than the next)
// Additionally, it reduces the refresh-rate *dramatically*
// (A very similar method is used in rowSegBuffer, and at one point the
// refresh-rate dropped to 1/5th Hz... yes, that's 5 seconds to redraw the
// entire display).
// 
// a/o v90:
// CURRENTLY: there's really no testing-method to see about a display's
// compatibility with row[Seg]Buffer... it's kinda all-or-nothing.
// FURTHER: It hasn't been developed for quite some time, nor even
// tested... At one point it was *heavily* developed, though, so the code
// exists, and may be worth experimenting with.







//START HERE.


//This should be an option in mainConfig.h...
//Do not uncomment it here. It's left here purely for 
//documentation-purposes...
// //#define ROWBUFFER_TESTING	TRUE



//THE REMAINDER IS STRAIGHT FROM mainConfig.h:

//The row-buffer method increases the resolution dramatically, when
//compared to the frame-buffer method... all that memory which was used fo
//a 2D image can now be dedicated to a single row! (e.g. a 16x16 frame
//buffer method requires 256 Bytes of RAM, a 24x24 frame buffer requires
//576 bytes (more than the ATTiny861 has available).
//
//Instead, calculate each row immediately before drawing it (during the
//LCD's "Horizontal Front Porch").
// Thus, that i.e. 576 bytes originally used for an entire frame can now b
// dedicated to each row. Further, since each row on the display itself ha
// to be drawn individually, it can increase the vertical resolution to 
// that of the display (768, in my case).
//
//The "row-buffer" method is somewhat poorly-named.
// It might be more appropriate to call it a "row-settings buffer", or
// something similar, as each byte is not an RGB color-value (as in frame
// buffer) but a packed representation of the actual register-values that
// generate the desired color. This reduces calculations during the actual
// row-drawing process, allowing the fewest number of calculations per
// "pixel" by pushing the vast majority of calculations *before* the 
// drawing process.
// Fewer calculations per pixel => skinnier pixels => more pixels can be
// squished in horizontally.
// 
//BUT: Since this requires recalculating *each row* *each time it's drawn*
// we start to run into various limitations:
//   The refresh-rate is limited by all that calculation time.
//   Some displays will not sync with long delays between rows.
//   Some displays seem to require *stable* delays between rows
//      so even if some rows require very few cycles to calculate, the
//      delay between each row needs to be kept somewhat constant
//      (time for another timer and/or repurpose of the hsync timer?)
//      Further, visually it's more comfortable (?) if the refresh doesn't 
//      vary in speed on each row (due to varying calculation times).
//
//As it stands, only the LTN display has been tested with the row-buffer
// It has been found quite stable. The Chi Mei display was tested with the
// Row-Segment-Buffer, and found incompatible, so likely would be with the
// row-buffer as well. Though further experimenting would be worthwhile,
// that display has since been made-use-of rather permanently.

//So, where's this leave us...?
// The resolution, using row-buffer, is not as much limited by memory, as
// it is by the actual number of cpu instructions necessary to load the
// registers from the packed data vs. the display's pixel-clock (the LVDS
// prescaler). There are various ways to optimize it based on needs: e.g.
// with the PWM161's 1K of RAM, value-packing may not be necessary, then
// unpacking isn't either, making each pixel require even fewer CPU
// cycles. But doing so requires three bytes per pixel (three times the
// memory!)
//
// FOR NOW: value-packing is de-facto, as this was developed with only 512B
// of RAM.
//
// Also, changing the lvds-prescaler could increase resolution at the cost
// of refresh-speed, if the display can handle slower rates.
// 
//
//a/o v70, the rowBuffer hasn't been implemented for quite some time, and
//not at all on the PWM161, so there's some work ahead.
// Getting there...
//
//More contemplations:
// If a project is designed with drawRow() functions, rather than
// drawFrame, it's easy enough to port either way, as can be seen in
// tetStuff, etc.
// e.g. a drawRow function might contain:
//   void drawRow(uint16_t rowNum, uint8_t *rowBuffer) {
//     for(i=0; i<ROW_WIDTH; i++) rowBuffer[i] = _W; // (white)
//   }
//
// using frameBuffer is as simple as having a function to loop through all
// the rows calling drawRow as:
//    drawRow(rowNum, frameBuffer[rowNum])
// Note that frameBuffer is two dimensional, but given to drawRow with a
// specific row-index, so appears to drawRow as a rowBuffer.
//
// The only other change is to set rowBuffer[i] = fb_to_rb(_W);
//   instead of merely rowBuffer[i] = _W;
//
// fb_to_rb converts a frameBuffer color value (RGB) to a packed byte to
// store in the row(settings)buffer
//
//


#define ROW_BUFFER TRUE

//These can be varied for testing...
#define LVDS_PRESCALER 1
#define LVDS_PRESCALER_ERROR_OVERRIDE TRUE

//Interesting... this seems to be problematic now.
#define ROW_CALCULATION_CYCS 0 //4000

//Try out the 




//.... yep, that's where I left-off in mainConfig.




#endif //__FRAMEBUFFER_TESTING_H__


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
 * /Users/meh/_avrProjects/LCDdirectLVDS/90-reGitting/mainConfig.h
 *
 *    (Wow, that's a lot longer than I'd hoped).
 *
 *    Enjoy!
 */

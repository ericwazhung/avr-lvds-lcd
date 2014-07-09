/* mehPL:
 *    This is Open Source, but NOT GPL. I call it mehPL.
 *    I'm not too fond of long licenses at the top of the file.
 *    Please see the bottom.
 *    Enjoy!
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

void nonRSB_drawPix(uint8_t rowNum)
{
   //uint8_t *setting = &(settingBuffer[rowNum][0]);
#if(defined(ROW_BUFFER) && ROW_BUFFER)
   uint8_t *color = &(rowBuffer[0]);
#else
   uint8_t *color = &(frameBuffer[rowNum][0]);
#endif
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
      writeColor(*(color+0));

      //Moving this here not only removes (most of) the green bar
      // but also seems to make the pixel edges significantly sharper
      // (v29 has ~1/8in of noise, v30 has ~1pixel noise at the right edge)
      TCCR1A = ( (0<<COM1A1) | (1<<COM1A0)
               | (0<<COM1B1) | (1<<COM1B0)
               | (1<<PWM1A) | (1<<PWM1B) );

      writeColor(*(color+1));    
      writeColor(*(color+2));    
      writeColor(*(color+3)); 
      writeColor(*(color+4));    
      writeColor(*(color+5));                
      writeColor(*(color+6));  
      writeColor(*(color+7));                         
      writeColor(*(color+8));                         
      writeColor(*(color+9));                         
      writeColor(*(color+10));                         
      writeColor(*(color+11));                         
      writeColor(*(color+12));                         
      writeColor(*(color+13));                         
      writeColor(*(color+14));                         
      writeColor(*(color+15));   
#define COLORS_WRITTEN   16      
#if ( (defined(COLOR_BAR_SCROLL) && COLOR_BAR_SCROLL) \
   || (defined(ROW_BUFFER) && (ROW_BUFFER)) )
      writeColor(*(color+16));
      writeColor(*(color+17));
      writeColor(*(color+18));
      writeColor(*(color+19));
writeColor(*(color+20));
writeColor(*(color+21));
writeColor(*(color+22));
writeColor(*(color+23));
writeColor(*(color+24));
writeColor(*(color+25));
writeColor(*(color+26));
writeColor(*(color+27));
#define COLORS_WRITTEN   28
#if (defined(ROW_BUFFER) && (ROW_BUFFER))
writeColor(*(color+28));
writeColor(*(color+29));
writeColor(*(color+30));
writeColor(*(color+31));
//Some sort of syncing problem after 32... (?)

writeColor(*(color+32));
writeColor(*(color+33));
writeColor(*(color+34));
writeColor(*(color+35));
writeColor(*(color+36));
writeColor(*(color+37));
writeColor(*(color+38));
writeColor(*(color+39));
writeColor(*(color+40));
writeColor(*(color+41));
writeColor(*(color+42));
writeColor(*(color+43));
writeColor(*(color+44));
writeColor(*(color+45));
writeColor(*(color+46));
writeColor(*(color+47));
writeColor(*(color+48));
writeColor(*(color+49));
writeColor(*(color+50));
writeColor(*(color+51));
writeColor(*(color+52));
writeColor(*(color+53));
writeColor(*(color+54));
writeColor(*(color+55));
writeColor(*(color+56));
writeColor(*(color+57));
writeColor(*(color+58));
writeColor(*(color+59));
writeColor(*(color+60));
writeColor(*(color+61));
writeColor(*(color+62));
writeColor(*(color+63));
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
      writeColor(0);
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
#define ROW_COMPLETION_DELAY 512 //1 //65535//512

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
}
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
 * /Users/meh/_avrProjects/LCDdirectLVDS/90-reGitting/_unusedIdeas/nonRSB_drawPix.c.movedToWriteColor.c
 *
 *    (Wow, that's a lot longer than I'd hoped).
 *
 *    Enjoy!
 */

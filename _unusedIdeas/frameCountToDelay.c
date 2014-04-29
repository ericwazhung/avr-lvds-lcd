/* mehPL:
 *    This is Open Source, but NOT GPL. I call it mehPL.
 *    I'm not too fond of long licenses at the top of the file.
 *    Please see the bottom.
 *    Enjoy!
 */






//New Idea:
// Disable the Hsync timer interrupt after the frame completes
//  and reenable elsewhere...
//  (It seems pixels changing from one shade to another take an
//   intermediate step halfway between)
//  (Thus, two updates then a pause might clear that up)
//  (Duh... it doesn't clear that up, because the fastest refresh rate
//   is the same...)
//  (but it is an interesting effect)
//  (Double-Duh... I was using a slow refresh with FRAME_UPDATE_DELAY)
//  Reworded: Having several frame refreshes before a pause helps to
//  "solidify" the color changes... especially the sharp changes 
//  (e.g. yellow<->black)
//  (Side Note: I saw this effect on a regular (NON-LVDS) TFT display 
//   in LCDdirect that was only connected such that R, G, and B were each
//   either full-off or full-on, i.e. all Red bits were connected to a
//   single output pin (full on when 1, full off when 0).
//   I hadn't attributed it to improper refresh synchronization
//   as the refresh rate was too fast to see... That's a handy way to get
//   intermediate shades/colors without refresh-flicker from pulsing them!)
//  connected with each color 
//  0 means no delay (except FRAME_UPDATE_DELAY) via interrupt disabling
//  If you WANT this effect (for intermediate shades?) 
//   set FRAME_COUNT_TO_DELAY to 1
//  Then, each change of color will (apparently) result in a value halfway
//   between the old color and the new color
//  Not sure whether it's an electrical problem, but these half-shades 
//   appear dithered, sometimes ugilly, sometimes it's nice. I guess it
//   depends on the goal...
// a/o v59: I believe this doesn't work with ROW_SEG_BUFFER...
//#define FRAME_COUNT_TO_DELAY 8//2//7//1//5//2//3




//Implementation:

/* From main loop:

   //If I recall correctly, the idea is to test whether the timer is
   // running...
   //e.g. if(!getbit(OCIE0A, T0_TIMSK)) (this needs to be verified)

   //then to run calculations, if desired, or set up some other sort of
   // delay before:
#if (FRAME_COUNT_TO_DELAY != 0)
 timer_compareMatchIntEnable(0, OUT_CHANNELA);
#endif //FRAME_COUNT_TO_DELAY

   // (It's documented better elsewhere... but basically:
   //  do several frame refreshes, then pause.... allows for some
   //  useful redraw characteristics)
 */

/* From SIGNAL(Timer0):
   within if(updateLCD())
   {
#if (FRAME_COUNT_TO_DELAY != 0)
#error "FRAME_COUNT_TO_DELAY can't be enabled without code to restart the timer in the main() loop. It's been removed."
         //static uint8_t frameCount = 0;
         frameCount++;
         if(frameCount == FRAME_COUNT_TO_DELAY)
         {
            frameCount = 0;
            timer_compareMatchIntSetup(0, OUT_CHANNELA, FALSE);
            //timer_compareMatchIntDisable(0, OUT_CHANNELA);
         }
#endif
   }
*/
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
 * /Users/meh/_avrProjects/LCDdirectLVDS/68-backToLTN/_unusedIdeas/frameCountToDelay.c
 *
 *    (Wow, that's a lot longer than I'd hoped).
 *
 *    Enjoy!
 */

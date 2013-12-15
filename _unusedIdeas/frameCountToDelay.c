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

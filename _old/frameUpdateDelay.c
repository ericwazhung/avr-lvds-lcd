//This would normally be 0 for the highest-speed frame-refresh possible
// However, increasing this value can slow the frame-refresh rate
//   if desired
// e.g. 50000 causes it to refresh ~ every few seconds
//   (durn-near the limit for how long the LCD will retain an image)
// The benefit being that it doesn't flicker at annoying rates for
//  stationary images
//  worth exploring...
// This may be highly dependent on the screen capabilities...
//   of course, this sorta thing is undocumented
//   it just has to be experimented with
//   Further, it may be quite hard on the Liquid Crystals, since they'd 
//   likely be driven DC... not sure what the long term effects are.
// This could be a hell of a lot more sophisticated...
// e.g. refresh after a frameBuffer change
//      refresh based on a seconds-timer
//      ...
// AS-IS: Does refreshes completely independent of the frame-buffer
//      if it's too slow, some images may be missed completely.
//      This is not true for FRAME_SYNC TRUE (below)
//      (and it's not really useful anymore with FRAME_SYNC)
// a/0 v59: This is no longer used (!?) I coulda sworn it was still
//          responsible for modifying the timer's interrupt frequency
//          This was also used in an attempt to make the Horizontal Back
//          Porch constant regardless of row calculation-time
//          So, either, the HBP is long enough for the current math, or
//          the display is not sensitive to varying HBPs between rows
//          (see also lcdStuff.c)
//#define FRAME_UPDATE_DELAY 0//5000//50000 //2000 //5000 //20000 //50000

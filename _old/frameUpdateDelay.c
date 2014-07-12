/* mehPL:
 *    This is Open Source, but NOT GPL. I call it mehPL.
 *    I'm not too fond of long licenses at the top of the file.
 *    Please see the bottom.
 *    Enjoy!
 */












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
 * /Users/meh/_avrProjects/LCDdirectLVDS/93-checkingProcessAgain/_old/frameUpdateDelay.c
 *
 *    (Wow, that's a lot longer than I'd hoped).
 *
 *    Enjoy!
 */

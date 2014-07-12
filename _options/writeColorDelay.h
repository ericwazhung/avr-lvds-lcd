/* mehPL:
 *    This is Open Source, but NOT GPL. I call it mehPL.
 *    I'm not too fond of long licenses at the top of the file.
 *    Please see the bottom.
 *    Enjoy!
 */








#ifndef __WRITECOLORDELAY_H__
#define __WRITECOLORDELAY_H__

//writeColorDelay.h is just for calculating the delay necessary to stretch
//each call to "writeColor()" such that the frameBuffer/rowBuffer will fill
//the display...
//
// It's probably hokey.
//
// It's now separated from writeColor.c because e.g.
// _interfaces/6bitParallel.c makes reference to it.



#ifndef DOTS_TO_CYC
 #error "DOTS_TO_CYC not defined here"
#endif

#ifndef DE_ACTIVE_DOTS
 #error "DE_ACTIVE_DOTS not defined here"
#endif

#ifndef FB_WIDTH
 #error "FB_WIDTH not defined here"
#endif

#ifndef WRITE_COLOR_CYCS
 #error "WRITE_COLOR_CYCS not defined here"
#endif






//WRITE_COLOR_DELAY attempts to stretch each pixel such that the total
//number of horizontal pixels (e.g. FB_WIDTH) will stretch across the
//screen. It takes into account the number of cycles it takes to extract
//the color register-values from the buffer, and load them into the
//registers. It's entirely plausible it will be negative, if e.g.
//FB_WIDTH * WRITE_COLOR_CYCS > DE_CYC
//(i.e. you've chosen an FB_WIDTH that's too large for the screen)
//The math is loose, e.g.:
// WRITE_COLOR_CYCS is, unless done entirely in assembly, usually an
//  estimate 
// There's no rounding, and uses integer division...
// There's nothing stopping you from setting an FB_WIDTH too large...
//So it's likely the resulting image will be skinnier (or wider?) than the
// display...
//Calcs:
// DE_CYC = FB_WIDTH * ( WRITE_COLOR_CYCS + WRITE_COLOR_DELAY )
// DE_CYC/FB_WIDTH = WRITE_COLOR_CYCS + WRITE_COLOR_DELAY
// WRITE_COLOR_DELAY = DE_CYC/FB_WIDTH - WRITE_COLOR_CYCS
// Not sure why -4 is necessary... overhead in delayCyc?
#define WRITE_COLOR_DELAY_PRETEST \
	( DOTS_TO_CYC(DE_ACTIVE_DOTS) / FB_WIDTH - WRITE_COLOR_CYCS-4)

#if(WRITE_COLOR_DELAY_PRETEST < 0)
 #define WRITE_COLOR_DELAY 0
 #warning "WRITE_COLOR_DELAY < 0, your FB_WIDTH is possibly too large for the screen..."
// #error "problem here..."
#elif(WRITE_COLOR_DELAY_PRETEST >127)
 #define WRITE_COLOR_DELAY 127
 #warning "WRITE_COLOR_DELAY > 127... Tiny FB_WIDTH? HUGE screen? Have fun!"
#else
 #define WRITE_COLOR_DELAY (WRITE_COLOR_DELAY_PRETEST)
#endif


#endif //__WRITECOLORDELAY_H__

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
 * /Users/meh/_avrProjects/LCDdirectLVDS/93-checkingProcessAgain/_options/writeColorDelay.h
 *
 *    (Wow, that's a lot longer than I'd hoped).
 *
 *    Enjoy!
 */

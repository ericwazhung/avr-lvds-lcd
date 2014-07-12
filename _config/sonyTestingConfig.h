/* mehPL:
 *    This is Open Source, but NOT GPL. I call it mehPL.
 *    I'm not too fond of long licenses at the top of the file.
 *    Please see the bottom.
 *    Enjoy!
 */


#ifndef __SONY_TESTING_CONFIG_H__
#define __SONY_TESTING_CONFIG_H__

//This configuration is for a Sony LCD used as a Mario Question-Box which
//happens to have a window that's slightly smaller than the display...





//This is an attempt at moving calculations *before* Hsync, so as to work
//with a non-DE display... had a *minor* effect...
//Interestingly, without this, the codesize increased from 6888 to 6906
// (No visible effect, with bitbanged)
#define WC_SETUP TRUE




//So far, this is only implemented with the Sony parallel-LCD
// It should be *really easy* to implement with other displays/interfaces
// Just grep this.
// If the display is partially covered by display-housing, then we'd like
// to fit the frame-buffer into the *visible* portion
// This'll likely be something *near* but *smaller than* DE_ACTIVE_DOTS
//(This assumes the covered-up portion is at the right of the display)
#define VISIBLE_ROW_DOTS  225
//Override the default WHITE at the end of each row with BLACK...
#define FB_END_OF_ROW_COLOR  _K
//Later, maybe, _LCD_INTERFACE_CFILE should be handled in
// _displays/sonyACX705AKM.h?
// (Though, technically, the display *could* be interfaced via an FPD-Link
//  receiver chip, using _interfaces/lvds.c... Unlikely, but maybe useful
//  to keep these things separate for future displays/interfaces?)
#define _LCD_SPECIFIC_HEADER_ "_displays/sonyACX705AKM.h"
#define _LCD_INTERFACE_HEADER_ "_interfaces/6bitParallel.h"
#define _LCD_INTERFACE_CFILE_ "_interfaces/6bitParallel.c"
//Since the MISO pin is available for the heart, it's used instead of the
//default assigned by the makefile
#define _HEART_PINNUM_ PB1
#define _HEART_PINPORT_   PORTB




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
 * /Users/meh/_avrProjects/LCDdirectLVDS/93-checkingProcessAgain/_config/sonyTestingConfig.h
 *
 *    (Wow, that's a lot longer than I'd hoped).
 *
 *    Enjoy!
 */

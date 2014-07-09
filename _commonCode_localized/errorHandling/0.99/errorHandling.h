/* mehPL:
 *    This is Open Source, but NOT GPL. I call it mehPL.
 *    I'm not too fond of long licenses at the top of the file.
 *    Please see the bottom.
 *    Enjoy!
 */


//errorHandling .99

//0.99 now returns num in the high nibble, and the function's return value
//     in the low nibble...
//0.98 first version

#ifndef __ERRORHANDLING_H__
#define __ERRORHANDLING_H__

//If __ERR_DISABLE__ is true, save a few bytes by not testing...
#ifndef __ERR_DISABLE__
 #define __ERR_DISABLE__ FALSE
#endif


#if !(__ERR_DISABLE__)
 #define ERR_HANDLE(function)				\
 {											\
	if(uint8_t err_val = (function))		\
		return err_val;						\
 }

 #define ERR_HANDLE_NUM(num, function)    \
 {                                        \
	uint8_t err_val = (function);          \
	if(err_val)                            \
		return ((num<<4) | err_val);        \
 }
#else
 #define ERR_HANDLE(function)			(function)
 #define ERR_HANDLE_NUM(num, function)	(function)
#endif

//Problem 1:
// if(test)
//   ERR_HANDLE_NUM(2, fn());
// else...
//
//becomes
// if(test)
// {
//   if((fn()))
//		return 2;
// }; <<<<-----
// else...
//BUT:
// the semicolon closes the if statement, and else appears from nowhere...
//SO:
//Either:
//enclose the macro in brackets...
// if(test)
// {
//   ERR_...
// }
//becomes
// if(test)
// {
//   {
//     if(fn())
//   };
// }
//OR:
//don't use a semicolon... 
// if(test)
//   ERR_HANDLE(fn())
//either way's hokey...
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
 * /Users/meh/_avrProjects/LCDdirectLVDS/90-reGitting/_commonCode_localized/errorHandling/0.99/errorHandling.h
 *
 *    (Wow, that's a lot longer than I'd hoped).
 *
 *    Enjoy!
 */

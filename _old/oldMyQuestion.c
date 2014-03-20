/* mehPL:
 *    This is Open Source, but NOT GPL. I call it mehPL.
 *    I'm not too fond of long licenses at the top of the file.
 *    Please see the bottom.
 *    Enjoy!
 */


#if(defined(MY_QUESTION) && MY_QUESTION)
	#define Q_WIDTH	32
	#define Q_HEIGHT	24

	//If this isn't static, and inside a function, then it will load to the
	// *data* section... rather than text...
	// Interesting "feature" that it can be made static here and clear that
	static uint8_t pgm_imageQ[Q_WIDTH*Q_HEIGHT] PROGMEM =
	{
		2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,0,
		2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,0,
		2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,0,
		2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,0,
		2,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,2,0,
		2,2,2,2,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,2,2,2,0,
		2,2,2,2,0,1,2,2,2,0,1,1,8,8,8,8,8,8,8,1,1,1,1,2,2,2,0,2,2,2,2,0,
		2,2,2,2,0,1,2,2,2,0,1,1,8,8,8,8,8,8,8,8,1,1,1,2,2,2,0,2,2,2,2,0,
		2,2,2,2,0,1,2,2,2,0,1,8,8,8,8,8,8,8,8,8,1,1,1,2,2,2,0,2,2,2,2,0,
		2,2,2,2,0,1,0,0,0,1,1,8,8,8,1,1,1,8,8,8,1,1,1,0,0,0,1,2,2,2,2,0,
		2,2,2,2,0,1,1,1,1,1,1,8,8,8,1,1,1,8,8,8,1,1,1,1,1,1,1,2,2,2,2,0,
		2,2,2,2,0,1,1,1,1,1,1,1,1,1,1,1,8,8,8,8,1,1,1,1,1,1,1,2,2,2,2,0,
		2,2,2,2,0,1,1,1,1,1,1,1,1,1,8,8,8,8,8,1,1,1,1,1,1,1,1,2,2,2,2,0,
		2,2,2,2,0,1,1,1,1,1,1,1,1,1,8,8,8,8,8,1,1,1,1,1,1,1,1,2,2,2,2,0,
		2,2,2,2,0,1,1,1,1,1,1,1,1,1,8,8,8,1,1,1,1,1,1,1,1,1,1,2,2,2,2,0,
		2,2,2,2,0,1,2,2,2,0,1,1,1,1,1,1,1,1,1,1,1,1,1,2,2,2,0,2,2,2,2,0,
		2,2,2,2,0,1,2,2,2,0,1,1,1,1,8,8,8,1,1,1,1,1,1,2,2,2,0,2,2,2,2,0,
		2,2,2,2,0,1,2,2,2,0,1,1,1,1,8,8,8,1,1,1,1,1,1,2,2,2,0,2,2,2,2,0,
		2,2,2,2,0,1,0,0,0,1,1,1,1,1,8,8,8,1,1,1,1,1,1,0,0,0,1,2,2,2,2,0,
		2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,0,
		2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,0,
		2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,0,
		2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	};
#endif
// Stuff here...
#if(defined(MY_QUESTION) && MY_QUESTION)
		qRow = (rowNum*Q_HEIGHT/(V_COUNT)); ///2));
	
		// Should use readImageByte... but it's specific to FB_WIDTH
		uint8_t data=
		  pgm_read_byte((uint8_t *)(&((pgm_imageQ)[(qRow)*Q_WIDTH+(qCol)])));


		if(data == 8)
		{
			data = qCount%3;
		}


		switch(data | ((qCount&0x01)<<3))
		{
			case 0:
				data = rgb(2,1,0);
				break;
			case 1:
				data = rgb(2,2,0);
				break;
			case 2:
				data = rgb(3,2,0);
				break;
			case 8|0:
				data = rgb8(206,157,60);
				break;
			case 8|1:
				data = rgb8(255,222,60);
				break;
			case 8|2:
				data = rgb8(255,157,60);
				break;
			default:
				data = rgb(3,3,3);
				break;
		}
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
 *    2) Please do not change/remove others' credit/licensing/copywrite 
 *         info, where noted. 
 *    3) If you find yourself profitting from my work, please send me a
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
 *    (Wow, that's a lot longer than I'd hoped).
 *
 *    Enjoy!
 */

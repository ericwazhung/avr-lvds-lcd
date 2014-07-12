/* mehPL:
 *    This is Open Source, but NOT GPL. I call it mehPL.
 *    I'm not too fond of long licenses at the top of the file.
 *    Please see the bottom.
 *    Enjoy!
 */






//Test '__flash' and its functionality with structures...
// Looks perfect.
//#define TEST__FLASH TRUE


#if(defined(TEST__FLASH) && TEST__FLASH)
#undef FRAMEBUFFER_TESTING
//const __flash uint8_t flashArray[] = { 0, 1, 2, 3 };
const uint8_t flashArray[4] PROGMEM = { 0, 1, 2, 3 };


typedef struct __BLAH__
{
	//const __flash uint8_t *flArray; //Pointer to the first element in an array
	const uint8_t *flArray; //Pointer to the first element in an array
} flashStruct_t;

typedef struct __BLAH2__
{
	const __flash flashStruct_t * flStruct;
} flStruct2_t;

//"Initializer Element is not computable at load time"
// (if the struct doesn't use '__flash' in its element description
const __flash flashStruct_t flashStruct = { flashArray };


const flStruct2_t flStruct2 = { &flashStruct };

//This 'const' refers to the constant-ness of the data being pointed-to
// NOT the constantness of the pointer itself.
const flStruct2_t *p_flStruct2 = &flStruct2;
#endif


void test__flash(void)
{
#if(defined(TEST__FLASH) && TEST__FLASH)
	fb_timer_t startTime = 0;

	uint8_t count = 0;

	while(1)
	{
		if(fb_isItTime(&startTime, 10000*MAIN_MS))
		{
			//set_heartBlink(p_flStruct2->flStruct->flArray[count]);
			set_heartBlink(
					pgm_read_byte(&(p_flStruct2->flStruct->flArray[count])));

			count++;
			if(count >= sizeof(flashArray))
				count = 0;
		}
		
		
		heartUpdate();
	}
#endif
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
 * /Users/meh/_avrProjects/LCDdirectLVDS/93-checkingProcessAgain/test__flash.c
 *
 *    (Wow, that's a lot longer than I'd hoped).
 *
 *    Enjoy!
 */

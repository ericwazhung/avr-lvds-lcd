/* mehPL:
 *    This is Open Source, but NOT GPL. I call it mehPL.
 *    I'm not too fond of long licenses at the top of the file.
 *    Please see the bottom.
 *    Enjoy!
 */










// THIS IS JUST A COPY OF the following with everything removed but
//  updateLCD
// and with loadData with colorOverride added so we don't have to modify
// loadData yet...
//lcdStuff 0.50ncf

//Returns TRUE when the frame is complete
// Should send nada (and maybe hsyncs?) thereafter(?)
// In sdramThing2.0, when this returns TRUE, the SDRAM loading is complete
// EXCEPT: the final HD_Dots' FR_READ/ACTIVATE commands will point to a
//  non-existent "next" Hsync... these commands need to be rewritten to
//  point to the "first" Hsync. This is handled in sdramThing2.0's code
// Used to be:
//SIGNAL(TIMER0_COMPA_vect)
uint8_t updateLCD(void)
{
	uint8_t frameComplete = FALSE;

	//static uint8_t frameCount = 0;

#if 0 // (defined(SLOW_EVERYTHING_TEST) && SLOW_EVERYTHING_TEST)
	while(TCNT0L != 7){};
	//This won't work... who knows how many cycles interrupt-entry will be
	//Above should fix that...
	//I think this was just an indicator for scope triggering...
	// not relevent for 
	setpinPORT(PB2, PORTB);
	clrpinPORT(PB2, PORTB);
#endif

	//Hsync and Actually draw the pixels... (when DE is active)
	// This takes a LONG time and will occupy most of the CPU...
	//  ... IN THIS INTERRUPT
	//loadData(((hsyncCount-T_VD-T_Vlow)>>3)&31, dataEnable,colorOverride);
	loadData(hsyncCount-T_DV-T_VD-T_Vlow, dataEnable);

	hsyncCount++;

	switch(hsyncCount)
	{
		//V Front Porch, begins with initLCD

		//Vsync H->L (Begin V-Sync)
		case T_DV:
			dataEnable = FALSE;
			//clrpinPORT(VSYNC_PIN, VSYNC_PORT);
			vSync = TRUE;
			Vsync_fromNada();
			break;
		//Vsync L->H (Begin V Back Porch)
		case T_Vlow + T_DV:
		//	if(hfm_nextOutput(&modulator))
		//		colorOverride = 0xff;
		//	else
		//		colorOverride = 7;
			//setpinPORT(VSYNC_PIN, VSYNC_PORT);
			vSync = FALSE;
			Nada_fromVsync();
			break;
		//Start of frame (DE active)
		case T_VD +(T_Vlow + T_DV):
			dataEnable = TRUE;
			break;
		// All rows have been displayed
		// Return to V Front Porch
		case V_COUNT +(T_VD + T_Vlow + T_DV):
			dataEnable = FALSE;
		//	break;
		//Frame complete
		//case HSYNC_COUNT_FRAME_COMPLETE:
			//T_DV +(V_COUNT+T_VD+T_Vlow) + FRAME_UPDATE_DELAY:
			hsyncCount = 0;
#if 0 //(FRAME_COUNT_TO_DELAY != 0)
			//static uint8_t frameCount = 0;
			frameCount++;
			if(frameCount == FRAME_COUNT_TO_DELAY)
			{
				frameCount = 0;
				timer_compareMatchIntSetup(0, OUT_CHANNELA, FALSE);
				//timer_compareMatchIntDisable(0, OUT_CHANNELA);
			}
#endif
			
//#warning "NewNote: WTF, are we relying on loadData to have properly set the mode?"
// Yeah, guess that makes sense... Must be Nada...
			frameComplete = TRUE;
			break;
		default:
			break;
	}


#if 0 // (defined(LOADROW) && LOADROW)
	//Load the next row into the row(seg)buffer
	//data is enabled after T_VD+T_Vlow...
	if((hsyncCount >= T_VD+T_Vlow) && (hsyncCount < T_VD+T_Vlow+V_COUNT))
		loadRow(hsyncCount - (T_VD+T_Vlow));
#endif

	//This note was written long before the above was commented-out...
	//So...?
	// Here is where loadRow used to be called
	// it has been moved to oldNotes.txt, but it would make more sense
	// to look at LCDdirectLVDS <25 or LCDdirect
	// loadRow used the time between DE inactive and the next Hsync
	// to load a row to a row-buffer (as opposed to a frame-buffer)
	// thus, DE could gain more pixels because there would be fewer
	// calculations to determine the memory location to write from
	// This effect has since been minimized
	// BUT loadRow might be necessary for program-memory-based images...

	return frameComplete;
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
 * /Users/meh/_avrProjects/LCDdirectLVDS/90-reGitting/_old/lcdUpdate.c
 *
 *    (Wow, that's a lot longer than I'd hoped).
 *
 *    Enjoy!
 */

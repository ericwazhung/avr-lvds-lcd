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


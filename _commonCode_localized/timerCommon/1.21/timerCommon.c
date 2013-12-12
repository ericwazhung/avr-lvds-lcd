//timerCommon 1.20-1
#include "timerCommon.h"


#if defined(TCCR0)
 #define T0_CSReg	TCCR0
#endif
//Seems like there'd be no case where TCCR0 AND TCCR0A would both exist, but just in case...
//Allow multiple-definition (don't use elif) so a warning will be shown...
#if defined(TCCR0B)
 #define T0_CSReg	TCCR0B
#endif

#if defined(TCCR1)
 //Doesn't seem likely, but who knows with other procs...
 #define T1_CSReg	TCCR1
#endif
#if defined(TCCR1B)
 #define T1_CSReg	TCCR1B
#endif

#if defined(TCCR2)
 #define T2_CSReg	TCCR2
#endif
#if defined(TCCR2B)
 #define T2_CSReg	TCCR2B
#endif

#if defined(TCCR3)
 //Also doesn't seem likely, but who knows...
 #define T3_CSReg	TCCR3
#endif
#if defined(TCCR3B)
 #define T3_CSReg	TCCR3B
#endif

#if defined(T3_CSReg) && !defined(CSOBS)
 #warning "Timer3 on this device is not necessarily properly implemented!"
#endif

//!!! THESE DO NOTTTT Correspond to the values in TinyX61...
//    (well, DIV1 does...)
//Internal use only... Actual value to be written to the _CSReg
#define CSTYP_DIV1		0x01
#define CSTYP_DIV8		0x02
#define CSTYP_DIV64		0x03
#define CSTYP_DIV256	0x04
#define CSTYP_DIV1024	0x05
//Internal use only... Actual value to be written to the _CSReg
#if defined(CSALT)
 #define CSALT_DIV1		0x01
 #define CSALT_DIV8		0x02
 #define CSALT_DIV32	0x03
 #define CSALT_DIV64	0x04
 #define CSALT_DIV128	0x05
 #define CSALT_DIV256	0x06
 #define CSALT_DIV1024	0x07
#endif
//Internal use only... Actual value to be written to the _CSReg
//These values are extensions of CSTYP...
#if defined(CSOBS)
 #define CSOBS_DIV16	0x06
 #define CSOBS_DIV32	0x07
#endif

//All Clock Select bits appear to be located in the first three bits
#define CSMASK			0x07

//Roughly equivalent to the ol' Init_timerX(clkDiv, wgm)
//Return 0 if no error...
#if (!defined(TIMER_INIT_UNUSED) || !TIMER_INIT_UNUSED)
uint8_t timer_init(uint8_t timerNum, uint8_t clockDiv, uint8_t wgm)
{
	//set the waveform mode
	ERR_HANDLE_NUM(1,				\
		timer_setWGM(timerNum, wgm)	);

	//select the clock
	ERR_HANDLE_NUM(2,							\
		timer_selectDivisor(timerNum, clockDiv)	);
		
	return 0;
}
#endif

#if (!defined(TIMER_SETOUTPUTMODES_UNUSED) || !TIMER_SETOUTPUTMODES_UNUSED)
//timer_setOutputMode() no longer exists, the output channel is now a mask
uint8_t timer_setOutputModes(uint8_t timerNum, uint8_t outputChannels, \
										uint8_t outputMode)
{
	uint8_t channelNums;

	//This is not be-all-end-all testing, but helps...
	if(timerNum >= MAXTIMERS)
		return 1;
	if(outputChannels >= 0x0f) //MAXCHANNELSPERTIMER)
		return 2;

	//For the sake of our brute-force switch statement...
	//Combine the timer number and channel into a single variable...
	//The high nibble contains the timer number, 
	// the low contains the channel numbers as a mask (A=0x1,B=0x2,C=0x3...)
	channelNums = (timerNum<<4) | outputChannels;
	
	uint8_t i;

	//Handle each output in the mask...
	for(i=1; i<=8; i<<=1)
	{
		uint8_t channelNum = channelNums & (0xf0 | i);

		//Don't bother with the switch statement if the for loop is testing
		// a channel that's not being enabled (in the mask)
		if((channelNum & 0x0f) == 0)
			continue;


		switch(channelNum)
		{	
			//Timer 0
		  #if defined(T0_COMReg)
		   #if defined(OUT_OC0)
			case OUT_OC0:
				writeMasked((outputMode<<COM_SHIFT), COM_MASK, T0_COMReg);
				break;
		   #endif
		   #if defined(OUT_OC0A)
			case OUT_OC0A:
				writeMasked((outputMode<<COMA_SHIFT), COMA_MASK, T0_COMReg);
				break;
		   #endif
		   #if defined(OUT_OC0B)
			case OUT_OC0B:
				writeMasked((outputMode<<COMB_SHIFT), COMB_MASK, T0_COMReg);
				break;
		   #endif
		  #endif
			//Timer 1
		  #if defined(T1_COMReg)
		   #if defined(OUT_OC1)
			case OUT_OC1:
				writeMasked((outputMode<<COM_SHIFT), COM_MASK, T1_COMReg);
				break;
		   #endif
		   #if defined(OUT_OC1A)
			case OUT_OC1A:
				writeMasked((outputMode<<COMA_SHIFT), COMA_MASK, T1_COMReg);
				break;
		   #endif
		   #if defined(OUT_OC1B)
			case OUT_OC1B:
				writeMasked((outputMode<<COMB_SHIFT), COMB_MASK, T1_COMReg);
				break;
		   #endif
			#if defined(OUT_OC1D) //so far only on the Tiny861...
			case OUT_OC1D:
				writeMasked((outputMode<<COMD_SHIFT), COMD_MASK, T1_COMDReg);
				break;
			#endif
		  #endif
			//Timer 2
		  #if defined(T2_COMReg)
		   #if defined(OUT_OC2)
			case OUT_OC2:
				writeMasked((outputMode<<COM_SHIFT), COM_MASK, T2_COMReg);
				break;
		   #endif
		   #if defined(OUT_OC2A)
			case OUT_OC2A:
				writeMasked((outputMode<<COMA_SHIFT), COMA_MASK, T2_COMReg);
				break;
		   #endif
		   #if defined(OUT_OC2B)
			case OUT_OC2B:
				writeMasked((outputMode<<COMB_SHIFT), COMB_MASK, T2_COMReg);
				break;
		   #endif
		  #endif
			//Timer 3
		  #if defined(T3_COMReg)
		   #if defined(OUT_OC3)
			case OUT_OC3:
				writeMasked((outputMode<<COM_SHIFT), COM_MASK, T3_COMReg);
				break;
		   #endif
		   #if defined(OUT_OC3A)
			case OUT_OC3A:
				writeMasked((outputMode<<COMA_SHIFT), COMA_MASK, T3_COMReg);
				break;
		   #endif
		   #if defined(OUT_OC3B)
			case OUT_OC3B:
				writeMasked((outputMode<<COMB_SHIFT), COMB_MASK, T3_COMReg);
				break;
		   #endif
		  #endif
			//The selected timer and channel combination doesn't exist...
			default:
				return 3;
				break;
		}
	}
	return 0;
}
#endif

//Selecting a clock source is independent of the number of channels... 
//Returns 0 unless an error occurs...
#if (!defined(TIMER_SELECTDIVISOR_UNUSED) || !TIMER_SELECTDIVISOR_UNUSED)
uint8_t timer_selectDivisor(uint8_t timerNum, uint8_t clockDiv)
{
	uint8_t csbits;
	
  #if defined(CSOBS)
	if(timerNum == 0 || timerNum == 1 || timerNum == 3)//IS_CSTYP(timerNum) || IS_CSOBS(timerNum))
  #else
	if(IS_CSTYP(timerNum))
  #endif
	{
		switch(clockDiv)
		{
			case CLKDIV1:
				csbits = CSTYP_DIV1;	
				break;
			case CLKDIV8:
				csbits = CSTYP_DIV8;
				break;
			case CLKDIV64:
				csbits = CSTYP_DIV64;	
				break;
			case CLKDIV256:
				csbits = CSTYP_DIV256;	
				break;
			case CLKDIV1024:
				csbits = CSTYP_DIV1024; 
				break;
			default:
			  #if defined(CSOBS)
				if(IS_CSOBS(timerNum))	
				{
					if(clockDiv == CLKDIV16)	
						csbits = CSOBS_DIV16;
					else if(clockDiv == CLKDIV32)
						csbits = CSOBS_DIV32;
					else
						return 1;	
				}
				else
					return 1;
			  #else
				return 1;
			  #endif
				break;
		}
	}
  #if defined(CSALT)
	else if(IS_CSALT(timerNum)) 
	{
		switch(clockDiv)	
		{
			case CLKDIV1:
				csbits = CSALT_DIV1;
				break;
			case CLKDIV8:
				csbits = CSALT_DIV8;
				break;
			case CLKDIV32:
				csbits = CSALT_DIV32;
				break;
			case CLKDIV64:
				csbits = CSALT_DIV64;
				break;
			case CLKDIV128:
				csbits = CSALT_DIV128;
				break;
			case CLKDIV256:
				csbits = CSALT_DIV256;
				break;
			case CLKDIV1024:
				csbits = CSALT_DIV1024;
				break;
			default:
				return 1;	
				break;
		}
	}
  #endif
	else 
		return 1;	

	switch(timerNum)
	{
		//Assume Timer0 and Timer1 exist (see setWGM)
		case 0:
			writeMasked(csbits, CSMASK, T0_CSReg);
			break;
		case 1:
			writeMasked(csbits, CSMASK, T1_CSReg);
			break;
		//From here, only compile for timers that exist as expected...
		#if defined(T2_CSReg)
		case 2:
			writeMasked(csbits, CSMASK, T2_CSReg);
			break;
		#endif
		#if defined(T3_CSReg)
		case 3:
			writeMasked(csbits, CSMASK, T3_CSReg);
			break;
		#endif
		default:
			return 1;
			break;
	}
	
	return 0;
}
#endif

//Set the Waveform Generation Mode...
// Four are universal, 6 are almost... 16bit counters are all the same
// For now, just implement the universal ones...
// Returns 0 if no error
#if(!defined(TIMER_SETWGM_UNUSED) || !TIMER_SETWGM_UNUSED)
uint8_t timer_setWGM(uint8_t timerNum, uint8_t wgm)
{
	uint8_t wgmLb = getbit(0, wgm);
	uint8_t wgmHb = getbit(1, wgm);
	
	//make the whole byte reflect the bit, so it can be used in writeMasked... 
	//This could probably be more efficient if using writeBit instead...
	// Oddly only minor effect to .stab but not to .text or .data after 
	// other previous changes (below)...
//	wgmLb *= 0xff;
//	wgmHb *= 0xff;

	//Make sure the chosen WGM will be written to the WGM bits properly, as implemented below...
	// Only modes 0-3 are implemented...
	if(wgm > 0x03)
		return 1;
	
	switch(timerNum)
	{
		//Assuming every AVR has a Timer0... 
		//(an error will occur otherwise, and this can be updated accordingly)
		case 0:
#ifdef _AVR_IOTNx61_H_
			//TinyX61s' Timer0 is an exception...
			// It only has WGM00, which serves the same functionality as
			// most others' WGM01 (when their WGM00 == 0)
			// (No PWM)
			if(wgmLb)
				return 1;

			//Whatever .stab is, this single change reduces it by 36Bytes
			writebit(WGM00, T0_WGMReg, wgmHb);
//			writeMasked(wgmHb, (1<<WGM00), T0_WGMReg);
#else
			//This seems to be the "universal" WGM Timer0 settings
			writebit(WGM00, T0_WGMReg, wgmLb);
			writebit(WGM01, T0_WGMReg, wgmHb);
//			writeMasked(wgmLb, (1<<WGM00), T0_WGMReg);
//			writeMasked(wgmHb, (1<<WGM01), T0_WGMReg);
#endif
			break;
		//Also Timer1, since all devices I've explored have both T0 and T1...
		case 1:
#ifdef _AVR_IOTNx61_H_
			//TinyX61s' Timer1 is also an exception...
			// WGM_NORMAL and WGM_CLR_ON_COMPARE are identical...
			//  comparing to OCR1C
			//  AND only possible when PWM1x is zero...
			// AGAIN: ONLY THE "UNIVERSAL" MODES are currently implemented
			// This has several fancy features.

//   Replacing the switch statement below with this didn't save any space
//     and, frankly, the switch statement is more intuitive
//			wgmLb=0;
//			wgmHb=0;
//			if(wgm == WGM_PHASE_PWM)
//				wgmLb = 0xff;

			switch(wgm)
			{
//   Moving this case with default saved 10 text and 4 data bytes!
//				case WGM_FAST_PWM:
//					wgmLb = 0;
//					wgmHb = 0;
//					break;
				case WGM_PHASE_PWM:
					wgmLb = 1; //0xff;
					wgmHb = 0;
					break;
				case WGM_FAST_PWM:
					// This one really has bits = 0 0
				case WGM_NORMAL:
				case WGM_CLR_ON_COMPARE:
					//For these modes, the WGM bits are Don't Cares
					// ("Normal" counting is determined by PWM1x = 0)
				default:
					//default shouldn't happen...
					wgmLb = 0;
					wgmHb = 0;
					break;
			}

			//NOTE: TinyX61s' WGM bits are both in TCCR1D (very unusual)
			writebit(WGM10, TCCR1D, wgmLb);
			writebit(WGM11, TCCR1D, wgmHb);
//			writeMasked(wgmLb, (1<<WGM10), TCCR1D);
//			writeMasked(wgmHb, (1<<WGM11), TCCR1D);
#elif (!defined(WGM10) || !defined(WGM12) || !defined(TCCR1A) \
		|| !defined(TCCR1B))   //defined(_AVR_IOTNx5_H_)
 #warning "This device's Timer1 doesn't appear to have (normal) WGM modes."
 #warning " Or, at least, I haven't figured out how to implement them yet"
 #warning " calls to timer_setWGM(1,...) will error"
			return 1;
#else
			//The "Universal" WGM settings for Timer1 are set here...
			writebit(WGM10, TCCR1A, wgmLb);
			writebit(WGM12, TCCR1B, wgmHb);
//			writeMasked(wgmLb, (1<<WGM10), TCCR1A);
//			writeMasked(wgmHb, (1<<WGM12), TCCR1B);
#endif
			break;
	  //From here, only compile for timers that exist...
	  //at least "exist" as expected...
	  #if defined(T2_WGMReg)
		case 2:
			writebit(WGM20, T2_WGMReg, wgmLb);
			writebit(WGM21, T2_WGMReg, wgmHb);
//			writeMasked(wgmLb, (1<<WGM20), T2_WGMReg);
//			writeMasked(wgmHb, (1<<WGM21), T2_WGMReg);
			break;
	  #endif
	  #if defined(TCCR3A)
		case 3:
			writebit(WGM30, TCCR3A, wgmLb);
			writebit(WGM32, TCCR3B, wgmHb);
//			writeMasked(wgmLb, (1<<WGM30), TCCR3A);
//			writeMasked(wgmHb, (1<<WGM32), TCCR3B);
			break;
	  #endif
		default:
			return 1;
			break;
	}
	
	return 0;
}
#endif

//timer_intEnable(type) could be created, assuming similarities between OCIEn/OCFn and TOIEn/TOVn amongst all timers on all chips
// Looks complicated since OCIEA/B for certain timers, and offsets don't seem particularly consistent... (just looking at 8515)
//uint8_t timer_compareMatchIntEnable(uint8_t timerNum, uint8_t outputChannel)
//!!! This ONLY works with OUT_CHANNEL_A/B
// Maybe it should be modified for OUT_A | OUT_B as above...?
#if (!defined(TIMER_COMPAREMATCHINTSETUP_UNUSED) \
		      || !TIMER_COMPAREMATCHINTSETUP_UNUSED)
uint8_t timer_compareMatchIntSetup(uint8_t timerNum, uint8_t outputChannel,\
	  												uint8_t enable)
{
	uint8_t channelNum;

	if(timerNum >= MAXTIMERS)
		return 1;

	if(outputChannel >= 2) //MAXCHANNELSPERTIMER)
		return 2;

	//writebit (used later) does not currently handle TRUE and FALSE properly, just 0 and 1...
	// this should be fixed now...
	if(enable)
		enable = 1;
	
	//For the sake of our brute-force switch statement...
	//Combine the timer number and channel into a single variable...
	//The high nibble contains the timer number, and the low contains the channel number (A=0,B=1)
	channelNum = (timerNum<<4) + outputChannel;

	//Don't clear interrupts before disabling this one, or that defeats the purpose!
	// But do clear so we don't get unexpected overflows on init
	if(enable)
		cli();
	
	switch(channelNum)
	{
	  #if defined(T0_TIMSK)
		//Case 0x00 is either OC0 or OC0A...
	   #if defined(OCIE0)
		case 0x00:
			//Enable the overflow interrupt (don't forget to create a handler!)
//			setbit(OCIE0, T0_TIMSK);
			writebit(OCIE0, T0_TIMSK, enable);
			//Clear the overflow flag (OCD)...
			setbit(OCF0, T0_TIFR);
			break;
	   #endif
	   #if defined(OCIE0A)
		case 0x00:
//			setbit(OCIE0A, T0_TIMSK);
			writebit(OCIE0A, T0_TIMSK, enable);
			// My OCD bit me in the ass, this was set to TIMSK by mistake(?)
			setbit(OCF0A, T0_TIFR);
			break;
	   #endif
	   #if defined(OCIE0B)
		case 0x01:
//			setbit(OCIE0B, T0_TIMSK);
			writebit(OCIE0B, T0_TIMSK, enable);
			// As was this...
			setbit(OCF0B, T0_TIFR);
			break;
	   #endif
	  #endif
	  #if defined(T1_TIMSK)
	   #if defined(OCIE1)
		case 0x10:
//			setbit(OCIE1, T1_TIMSK);
			writebit(OCIE1, T1_TIMSK, enable);
			setbit(OCF1, T1_TIFR);
			break;
	   #endif
	   #if defined(OCIE1A)
		case 0x10:
//			setbit(OCIE1A, T1_TIMSK);
			writebit(OCIE1A, T1_TIMSK, enable);
			setbit(OCF1A, T1_TIFR);
			break;
	   #endif
	   #if defined(OCIE1B)
		case 0x11:
//			setbit(OCIE1B, T1_TIMSK);
			writebit(OCIE1B, T1_TIMSK, enable);
			setbit(OCF1B, T1_TIFR);
			break;
	   #endif
	  #endif
	  #if defined(T2_TIMSK)
	   #if defined(OCIE2)
		case 0x20:
//			setbit(OCIE2, T2_TIMSK);
			writebit(OCIE2, T2_TIMSK, enable);
			setbit(OCF2, T2_TIFR);
			break;
	   #endif
	   #if defined(OCIE2A)
		case 0x20:
//			setbit(OCIE2A, T2_TIMSK);
			writebit(OCIE2A, T2_TIMSK, enable);
			setbit(OCF2A, T2_TIFR);
			break;
	   #endif
	   #if defined(OCIE2B)
		case 0x21:
//			setbit(OCIE2B, T2_TIMSK);
			writebit(OCIE2B, T2_TIMSK, enable);
			setbit(OCF2B, T2_TIFR);
			break;
	   #endif
	  #endif
	  #if defined(T3_TIMSK)
	   #if defined(OCIE3)
		case 0x30:
//			setbit(OCIE3, T3_TIMSK);
			writebit(OCIE3, T3_TIMSK, enable);
			setbit(OCF3, T3_TIFR);
			break;
	   #endif
	   #if defined(OCIE3A)
	    case 0x30:
//			setbit(OCIE3A, T3_TIMSK);
			writebit(OCIE3A, T3_TIMSK, enable);
			setbit(OCF3A, T3_TIFR);
			break;
	   #endif
	   #if defined(OCIE3B)
		case 0x31:
//			setbit(OCIE3B, T3_TIMSK);
			writebit(OCIE3B, T3_TIMSK, enable);
			setbit(OCF3B, T3_TIFR);
			break;
	   #endif
	  #endif
		default:
			return 3;
			break;
	}
	
	//Don't reenable interrupts here if disabling... just leave them as they were....
	// but do enable for init...
	if(enable)
		sei();
	
	return 0;
}
#endif

#if (!defined(TIMER_OVERFLOWINTENABLE_UNUSED) \
		      || !TIMER_OVERFLOWINTENABLE_UNUSED)
uint8_t timer_overflowIntEnable(uint8_t timerNum)
{
	cli();
	
	switch(timerNum)
	{
		#if defined(T0_TIMSK)
		case 0:
			//Enable the overflow interrupt (don't forget to create a handler!)
			setbit(TOIE0, T0_TIMSK);
			//Clear the overflow flag (OCD)...
			setbit(TOV0, T0_TIFR);
			break; 
		#endif
		#if defined(T1_TIMSK)
		case 1:
			setbit(TOIE1, T1_TIMSK);
			setbit(TOV1, T1_TIFR);
			break;
		#endif
		#if defined(T2_TIMSK)
		case 2:
			setbit(TOIE2, T2_TIMSK);
			setbit(TOV2, T2_TIFR);
			break;
		#endif
		#if defined(T3_TIMSK)
		case 3:
			setbit(TOIE3, T3_TIMSK);
			setbit(TOV3, T3_TIFR);
			break;
		#endif
		default:
			return 1;
	}
	
	sei();
	
	return 0;
}
#endif

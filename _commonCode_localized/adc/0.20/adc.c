/* mehPL:
 *    This is Open Source, but NOT GPL. I call it mehPL.
 *    I'm not too fond of long licenses at the top of the file.
 *    Please see the bottom.
 *    Enjoy!
 */






#include "adc.h"

#if(!(defined(ADC_SUM_REMOVED) && ADC_SUM_REMOVED))
uint32_t adcSum[NUMADCS];
#endif

void adc_init(void)
{
	//Initialize to external reference voltage on AREF
	// Do NOT change this if AREF is connected to ANYTHING other than a capacitor
	// (i.e. a 3.3V voltage regulator output)
	//Technically, these values are reset-defaults, but just make sure
	//!!!According to attiny861, these values set VCC as the voltage ref
	//   NOT an external reference. So the above note seems odd...
	// This IS what I want on the Tiny861 threePinIDer...
	clrbit(REFS0, ADMUX);
	clrbit(REFS1, ADMUX);
	
	//Use right-adjusted output
	// so values read from the 2-byte register are:
	//	8-bits in the Low reg, and 2 in the High reg
	clrbit(ADLAR, ADMUX);

	//Enable the ADC
	// (see description for noise cancelling techniques)
	setbit(ADEN, ADCSRA);
	
	//Don't use Auto Triggering
	// (@@@ could use a timer, etc.)
	clrbit(ADATE, ADCSRA);
	
	//Clear the interrupt-flag to be used as a conversion-complete indicator
	// (not currently using the interrupt)
	// "This bit is set when an ADC conversion completes and the Data Registers are updated."
	// "ADIF is cleared by writing a logical one to the flag."
	// " BEWARE that if doing a Read-Modify-Write on ADCSRA, a pending interrupt can be disabled. "
	//   because the flag may be 1 when read, and re-writing 1 clears it.
	//   ???could get around this by using a special set/clrbit instruction which would always write 0
	//   to this bit...
	setbit(ADIF, ADCSRA);
	
	//make sure the interrupt is not enabled...
	clrbit(ADIE, ADCSRA);
	
	//Set the conversion clock prescaler
	// "By default, the successive approximation circuitry requires an input
	//  clock frequency between 50kHz and 200 kHz to get maximum resolution. 
	//  If a lower resolution than 10 bits is needed, the input clock frequency 
	//  to the ADC can be higher than 200 kHz to get a higher sample rate."
	// For now, let's go for higher-precision... 
	// 16MHz / 200kHz = 80
	// our best-bet for precision is 128
	adc_setPrescaler(ADPS_128);
		
	//Select GND as the first ADC source...
	adc_select(ADC_SELECT_GND);
	
	//Start a conversion so it will be initialized for the next...
	//"The first conversion after ADSC has been written 
	// after the ADC has been enabled ... 
	// will take 25 ADC clock cycles instead of the normal 13. 
	// This first conversion performs initialization of the ADC. "
	adc_startConversion();

	//Wait for the initialization to complete for lockstepping...
	while(adc_isBusy())
	{}
}


//adcNum 0-7 directly selects an input. 
//	Other values are defined in Table 21-4 (old note, 644?)
//adcNum 0-10 directly selects an input on the TinyX61
//(This doesn't currently work with differential inputs...)
void adc_select(uint8_t adcNum)
{
	//"If a different data channel is selected while a conversion is in progress,
	// the ADC will finish the current conversion before performing the channel change. "
	//  though, it is not safe to do this immediately after a startConversion,
	//  since the ADC clock is significantly slower than the uC
	// see Figure 21.5
	//"Note that the conversion starts on the following rising ADC clock edge after 
	// ADSC is written. The user is thus advised not to write new channel or reference
	// selection values to ADMUX until one ADC clock cycle after ADSC is written. 

	writeMasked(adcNum, ADC_SELECT_MASK, ADMUX);
}

//Return TRUE on error (if there was a previous conversion in progress...)
uint8_t adc_startConversion(void)
{
	//Check if a conversion is already in progress...
	if(adc_isBusy())
		return TRUE;

	//"The channel and reference selection is continuously updated 
	// until a conversion is started. Once the conversion starts, the channel and 
	// reference selection is locked to ensure a sufficient sampling time for the ADC."
	
	//"In Single Conversion mode, write this bit to one to start each conversion."
	// BEWARE: calling this will CLEAR the conversion-complete indicator
	setbit(ADSC, ADCSRA);
	return FALSE;
}

//Return the last value read by the ADC
// check if adc_isBusy is true before reading, in lockstep...
// Since ADLAR is cleared, it will be the low ten bits 
uint16_t adc_getValue(void)
{
	return ADC;
	//Note that ADC is a SFR_MEM16 read to ADCH and ADCL, defined in iomxx4.h (not in the datasheet)
	//reading both values takes longer and some other things mentioned in the manual
	// (i.e. it might be more efficient to only read one byte if acceptable)
	// unless only reading ADCH: "ADCL must be read first, then ADCH"
	//!!! Might want to verify this occurs properly in the assembly code
	
}


//In free-running mode, this will always return TRUE
// so adc_getValue can be called at any time and used to grab the most recent conversion
// (which might be one behind if it's called right at the end of one)
uint8_t adc_isBusy(void)
{
	//"This bit stays high as long as the conversion is in progress and 
	// will be cleared by hardware when the conversion is completed."
	// "ADSC can also be used to determine if a conversion is in progress. 
	//  The ADSC bit will be read as one during a conversion, 
	//  independently of how the conversion was started."
	//  actually, it would return true /always/ during free-running mode

	//Check state-machine... might be better to use ADIF...?
	return getbit(ADSC, ADCSRA);
}

void adc_setPrescaler(uint8_t adps)
{
	//This will clear the conversion-complete flag!
	writeMasked(adps, ADPS_MASK, ADCSRA);
}

//Disable the digital input circuitry for the selected channel
void adc_takeInput(uint8_t inputNum)
{
	//"The corresponding PIN Register bit will always read as zero when this bit is set"

	uint8_t bitNum = inputNum;

#ifdef _AVR_IOTNx61_H_
	//ATTINYx61's have AREFD at bit 3, rather than ADC3D
	// the following ADC inputs are shifted 
	if(inputNum > 2)
		bitNum = inputNum + 1;

	//Analog inputs 7-10 are selected in DIDR1 and shifted...
	// 7 is bit 4, 10 is bit 7...
	//CURRENTLY ONLY: channels 0-7 are usable in this version
	// THIS NOTE SHOULD BE INVALID AS OF 0.19.
	if(inputNum >= 7)
	{
		bitNum = inputNum - 3;
		setbit(bitNum, DIDR1);
		return;
	}
#endif

	if(bitNum <= 7)
		setbit(bitNum, DIDR0);
}




#if(!(defined(ADC_SUM_REMOVED) && ADC_SUM_REMOVED))
uint16_t adcSumChannelMask = 0;
uint16_t adcSumNumSamples = 1;


void adc_sumSetup(uint16_t numSamples, uint16_t channelMask)
{
	adcSumChannelMask = channelMask;
	adcSumNumSamples = numSamples;
}


//Return TRUE when there is an updated value
uint8_t adc_sumUpdate(void)
{
	static uint8_t state = 0;
	static uint8_t adcInNum = LASTADC; //Start with channel 0; state 0 advances 7 to 0...
	
	//sampleNum is 1-based, initializing with 0 advances to 1
	static uint16_t sampleNum = 0;
	uint8_t retVal = FALSE;
	static uint32_t adcSumVal[NUMADCS];

		switch(state)
		{
			//Select the next input...
			// advance to the next state only if it's in the mask
			case 0:
				//select the next input...
				adcInNum++;

				//If we're at the last (possible) input, select the first
				if(adcInNum > LASTADC)
				{
					adcInNum = 0;
					sampleNum++;
					
					//If we've taken enough samples, write them for external access
					if(sampleNum > adcSumNumSamples)
					{
						uint8_t i;
						
						sampleNum = 1;

						//Write the samples for external access
						// and clear the sum for the next samples
						for(i = 0; i<NUMADCS; i++)
						{
							adcSum[i] = adcSumVal[i];
							adcSumVal[i] = 0;
						}
						
						retVal = TRUE;
					}

//					adcInNum = 0;
				}
								
				//Only advance to the next state if this adcInNum is enabled
				// Otherwise repeat this state and increment until one is found
				if(getbit(adcInNum, adcSumChannelMask))
					state++;
				
				break;
			//Select the channel and start the conversion
			case 1:
				adc_select(adcInNum);
				
				if(adc_startConversion())
				{
					//Returns TRUE if a conversion is already running
					//This should never happen with lockstepping...
//					setHeartRate(16);
					break;
				}
				
				state++;
				break;
			//Wait until the ADC conversion is completed...
			case 2:
				if(!adc_isBusy())
					state++;
				break;
			case 3:
				//Make sure the data has been transferred to the register
				//Wouldn't it make sense to put this in isBusy?
				if(getbit(ADIF, ADCSRA))
				{
					setbit(ADIF, ADCSRA);
					adcSumVal[adcInNum] += adc_getValue();
					state++;
				}
				break;
			default:
				state = 0;
				break;
		}

	return retVal;
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
 * /Users/meh/_avrProjects/LCDdirectLVDS/68-backToLTN/_commonCode_localized/adc/0.20/adc.c
 *
 *    (Wow, that's a lot longer than I'd hoped).
 *
 *    Enjoy!
 */

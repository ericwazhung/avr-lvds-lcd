/* mehPL:
 *    This is Open Source, but NOT GPL. I call it mehPL.
 *    I'm not too fond of long licenses at the top of the file.
 *    Please see the bottom.
 *    Enjoy!
 */



#ifndef __LVDS161_C__
#define __LVDS161_C__

#include _BITHANDLING_HEADER_
#include "lvds161.h"
#include "pll.c"





void lvds_timerInit(void)
{

  #if(defined(LVDS_MAXIMIZE_OSCCAL) && LVDS_MAXIMIZE_OSCCAL)
	OSCCAL = OSCCAL_VAL;
  #endif

  #if(defined(LVDS_USE_PLL) && LVDS_USE_PLL)
	pll_enable();
  #endif

	PSOC2 = (0<<POS23)	//PSCOUT23 outputs Waveform A if 1 or B if 0
			| (0<<POS22)	//PSCOUT22 outputs Waveform B if 1 or A if 0
							//Depending on whether POEN2C/D are 1...
			// PSYnC21:20 are for synchronizing the ADC...
			| (0<<POEN2D)	//PSCOUT23 unused if 0, PSC output if 1
			| (1<<POEN2B)	//PSCOUT21 unused if 0, PSC output if 1
			| (0<<POEN2C)	//PSCOUT22 ''
			| (1<<POEN2A)  //PSCOUT20 ''
			;

#warning "NOTE! These are 12-bit registers, requiring complex loading"
#warning "since we only need them to count to 7, it might be better to use"
#warning "low bytes, only!"

	//OCR2SA, OCR2RA	-> DVH/Blue
	//OCR2SB, OCR2RB	-> LVDS Clock
	// For one-ramp mode, OCR2RB determines the number of cycles (right?)
	// In order to make this work, the LVDS-CLK signal should be on this
	// channel...

	OCR2RB = 6;	//Count to 6, from 0, then reset the count
	OCR2SB = 3;	// Toggle the lvds-clock high at edge 3 (0->3 = 4 bits off)
					//                                      (4->6 = 3 bits on)
	//The above two won't change.
	//THIS IS INVERTED.
	// The outputs of the LVDS driver chips need to be swapped
	// (RXclk-/+)

	//OCR2SA/RA are used for DVH/Blue, and should be handled by this:
#if(!defined(PWM_TESTING) || !PWM_TESTING)
	Nada_init();
#else
	//These are just arbitrary values for 'scoping...
	OCR2SA = 0;
	OCR2RA = 8;
	// EXPERIMENTATION NOTES:
	// S   R
	// 0   0   signal is always low
	// 1   0   signal goes low for bit 1 (appears to wrap! Awesome)
	// 0   6   signal goes low for bit 0 (ditto)
	// 0   7   signal is always low (???? Weird?)
	//           Tested twice, yep. Weird
	// 7   0   signal is always low (expected)
	// 0   1   signal goes high for bit 1
	// 6   6   signal	is always low
	// 6   5   signal is low for bit 6
	// 1   1   always low
	// 2   1   low for bit 2
	// Appears to be no way to set *all* bits ??? NO, see below.
	// fff fff always low
	// 0   fff always high (!)
	//         may be hokey, due to the counter overflowing once before
	//         the OCRRB value being recognized?
	//         ALSO the output is fuzzy (weird?)
	// fff 0   always low
	// 0   ff  always high (!) (same as 0xfff)
	// 0   0x70 always high (!) (same)
   // 0   8   always high (!)
   // 0   7   recheck -- Yep, always low.
	// Appears that the TCNT value might jump briefly to 7 before resetting
	// to 0
	//So... apparently any combination can be created.
	// 5   0   bits 6 and 0 are high
	// 0   5   bits 6 and 0 are low
#endif

	//Also, forgot about FlankWidthModulation (?)

	PCNF2 = (0<<PFIFTY2)	//"Fifty-percent waveforms" unused
			| (0<<PALOCK2)	// AutoLock: Assures simultaneous register updates
			| (0<<PLOCK2)  // Lock: Similar
//TODO: Probably gonna have to use one of the LOCKs later.
			| (0<<PMODE21)	// \  0 0 -> One Ramp mode
			| (0<<PMODE20) // /
			| (1<<POP2)    // PSC outputs are active-high
			| (PCLKSELn<<PCLKSEL2) 
								// 0 selects "slow clock input" 1 selects fast
								// 1=CLK_PLL 0=CLK_I/O
			| (0<<POME2)   // 1 enables the Output Matrix, 0 disables
			;

	//PCNFE2 = //PASDLK22:20 for Analog Sync, unused...
				//PBFM21 for Flank Width Modulation, unused...
				//PELEV2A1 for Input Select, unused...
				//PELEV2B1 ''
				//PISEL2A1 ''
				//PISEL2B1 ''

	//PASDLY2 Analog Synchronization Delay Register

#if(LVDS_PRESCALER == 1)
 #define PPREn1	0
 #define PPREn0	0
#elif (LVDS_PRESCALER == 4)
 #define PPREn1	0
 #define PPREn0	1
#if 0
	These values only work with PSC2:
#elif (LVDS_PRESCALER == 16)
 #define PPREn1	1
 #define PPREn0	0
#elif (LVDS_PRESCALER == 64)
 #define PPREn1	1
 #define PPREn0	1
#endif
#if 0
	These values only work with PCSR:
#elif (LVDS_PRESCALER == 32)
 #define PPREn1	1
 #define PPREn0	0
#elif (LVDS_PRESCALER == 256)
 #define PPREn1	1
 #define PPREn0	1
#endif
//If values other than 1 or 4 are needed, there'll have to be some math
//done to determine whether to use the PLL or the system-clock
// and it may not even be possible, who knows.
#else
 #error "This LVDS_PRESCALER value won't work with the PWM161"
#endif
#if 0
 THIS HAS BEEN MOVED to the end of the function
	In order to synchronize the two PSCs

	PCTL2 = (PPREn1<<PPRE21) // \ Prescaler selection (0 0 = no divider)
			| (PPREn0<<PPRE20) // /
			//PBFM20 for Flank Width Modulation, unused...
			| (0<<PAOC2B) //"Fault Input selected to block _ can act directly
			| (0<<PAOC2A) // to PSCOUT2x outputs" ???
//TODO: These are related to PSC Clock Sources?! "page 133"
			//PARUN2 for synchronizing with other PSC's... but there aren't
						//any that it can sync with (PSCR doesn't), right?
			| (0<<PCCYC2) // Used during halting... which mightn't be used
			| (1<<PRUN2)  // START THE PSC
			;
#endif
	//A bunch of input configuration registers and capture registers

	POM2 = 0;	//PSC2 Output Matrix (unused)

	//PIM2: PSC2 Interrupt Mask Register, unused
	//PIFR2: PSC2 Interrupt Flag Register, unused

	//NOTE: PSC2/PSCR I/O can be set to defaults on reset, via fuses


//These are for PSCR and closely match PSC2:
	PSOC0 = (0<<PISEL0A1)	//PSC Input A Select (along with PISEL0A0)
			| (0<<PISEL0B1)	//PSC Input B Select (along with PISEL0B0)
				//These two might need to be changed for syncing...?
			//PSYNC01:00 are for synchronizing the ADC
			//Bit 3 is reserved
			| (1<<POEN0B)		//PSCOUT01 is connected to PSCR B output
			//Bit 1 is reserved
			| (1<<POEN0A)		//PSCOUT00 is connected to PSCR A output
			;

	//OCR0SA, OCR0RA	-> "Red"
	//OCR0SB, OCR0RB	-> "Green"
	// For now, I've been assuming that "green" was connected to channel B
	// Not sure why, exactly... maybe because it's similar to the case where
	// green is tied to the LVDS-CLK signal in the ATTiny861 configuration
	// Changing this to Red might allow for more colors 
	//  (since the fixed "starting-point" is R2 vs G3)	
	// But for now, I'll leave it with the written assumptions.
	//
	// OCR0RB determines the reset value of the counter, in one-ramp mode...
	// So its value should not change (regardless of whether it's Green or
	// Red)
	OCR0RB = 6; // count from 0->6, then reset

	//See notes re: Red OUTPUT INVERTED, green-matched, above.
	// Using R=5, we should be able to get away with simple math
	// see also setRed4()
	OCR0RA = 5;

	//These should handle configuring reasonable values for OCR0SA/RA/SB
#if(!defined(PWM_TESTING) || !PWM_TESTING)
	noGreen();
	noRed();
#else
	OCR0SB = 3;
	OCR0SA = 5;
	OCR0RA = 6;
#endif

	//See PCNF2, above...
	PCNF0 = (0<<PFIFTY0)
			| (0<<PALOCK0)
			| (0<<PLOCK0)
//TODO: Look into LOCKs
			| (0<<PMODE01)	//	\ 0 0 -> One Ramp Mode
			| (0<<PMODE00)	//	/
			| (0<<POP0)		// PSC outputs, active-LOW
								// PSCR works with ALL OUTPUTS INVERTED
								// (for Red/Green)
			| (PCLKSELn<<PCLKSEL0)
			//bit 0 is reserved
			;

////////lss stops following here...

	//The PCTLn registers actually start the PSC...
	// In order to synchronize the two separate PSC systems, the number of
	// instructions between the start of PSC2 and PSCR has to be precise
	// That's handled in assembly, below, but the values can be calculated
	// ahead-of-time.
	uint8_t pctl2_val;

	//PCTL2
	pctl2_val = 
		     (PPREn1<<PPRE21) // \ Prescaler selection (0 0 = no divider)
			| (PPREn0<<PPRE20) // /
			//PBFM20 for Flank Width Modulation, unused...
			| (0<<PAOC2B) //"Fault Input selected to block _ can act directly
			| (0<<PAOC2A) // to PSCOUT2x outputs" ???
//TODO: These are related to PSC Clock Sources?! "page 133"
			//PARUN2 for synchronizing with other PSC's... but there aren't
						//any that it can sync with (PSCR doesn't), right?
			| (0<<PCCYC2) // Used during halting... which mightn't be used
			| (1<<PRUN2)  // START THE PSC
			;


	uint8_t pctl0_val;

	//PCTL0 =
	pctl0_val =	
	        (PPREn1<<PPRE01) // \ Prescaler Selection (0 0 = no dividor)
			| (PPREn0<<PPRE00) // /
			//PBFM01 for flank-width-modulation, unused
			// PAOC0B/A for "fault input" tied to PSCOUT's... (?)
			//PBMF00 combines with PBFM01, above
			| (0<<PCCYC0) // Used during halting... mightn't get used
			| (1<<PRUN0)	// START the PSCR
			;

//This value should work with LVDS_PRESCALER=1 and USE_PLL
// As well as LVDS_PRESCALER=1 and NOT USE_PLL
//It's, roughly something to do with 8 being the PLL_MULT_FACTOR
// and 7 being the number of counts before a reset (bits)
// 8*7 = 56, which is a least-common-multiple
// then subtract one cycle because of the actual out instruction...
// So, *maybe* it's as general as ((PLL_MULT_FACTOR * LVDS_NUM_BITS) - 1)
// But then also need to take into account the LVDS_PRESCALER and whatnot.
// And PLL_SYSCLK

#if(defined(PLL_SYSCLK) && PLL_SYSCLK)
 //Not sure this math is right...
 // with PLL_SYSCLK we have SYS_CLK = PLL_CLK/4 (not /2)
 // But it did clear-up the yellow-tint on black...
 // Further, shouldn't it have the same effect? 55 would just double the
 // number of nops (which would be redundant)... ish.
 #define PSC_SYNC_CYCLES	((4*7)-1)
#else
 #define PSC_SYNC_CYCLES	55
#endif


#if ((LVDS_PRESCALER != 1) || (PLL_MULT_FACTOR != 8))
 #error "PSC_SYNC_CYCLES needs to be calculated!"
#endif

#if(PSC_SYNC_CYCLES > 127)
 #error "PSC_SYNC_CYCLES must be <= 127..."
#endif

//..... lss resumes here...
//Which is OK, since pctln_vals are constants...

//	uint8_t pctl2_val = 0x3f;
//	uint8_t pctl0_val = 0x7f;
	//delayCyc_setup(asdf, (int8_t)PSC_SYNC_CYCLES);
	delayCyc_setup(asdf, (int8_t)(PSC_SYNC_CYCLES));

	__asm__ __volatile__
	(
		"delayCycs_asmInit asdf; \n\t"
	 	"out %0, %1 ; \n\t"				//Start the PSC2
		"delayCycs asdf; \n\t"			//Delay for a specific number of cycles
		"out %2, %3 ; \n\t"				//Start the PSCR
		:
		: "I" (_SFR_IO_ADDR(PCTL2)),	//PCTL2 register
		  "d" (pctl2_val),				//value to write to PCTL2
		  "I" (_SFR_IO_ADDR(PCTL0)),	//PCTL0 register
		  "d" (pctl0_val),				//value to write to PCTL0
		  DC_VARIABLES_FOR_ASM(asdf)	// this is a list of variables used by
		 										// delayCyc... see delayCyc/test/...
	);

	//Ignoring the Input Control registers for now...
//TODO: Look into these for syncing PSCR and PSC2
	// PFRC0A/B, PICR0

	//PIM0: Interrupt Mask Register, unused
	//PIFR0: Interrupt Flag Register, unused


}


#endif //__LVDS161_C__

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
 * /Users/meh/_avrProjects/LCDdirectLVDS/93-checkingProcessAgain/_interfaces/lvds161.c
 *
 *    (Wow, that's a lot longer than I'd hoped).
 *
 *    Enjoy!
 */

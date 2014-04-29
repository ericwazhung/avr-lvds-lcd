/* mehPL:
 *    This is Open Source, but NOT GPL. I call it mehPL.
 *    I'm not too fond of long licenses at the top of the file.
 *    Please see the bottom.
 *    Enjoy!
 */




#ifndef __DMS_EXTERNAL_H__
#define __DMS_EXTERNAL_H__
extern volatile uint32_t dmsCount;	//deci-milliseconds

#if (!defined(DMS_FRAC_UNUSED) || !DMS_FRAC_UNUSED)
extern volatile uint8_t dmsFrac;
#endif

//These WERE specific to externalUpdate, 
// however they can also be used for dmsFrac with timer0 interrupt
// (Along with dmsFrac, below)
// Also with nonAVR code, where us is read directly...
//   these defaults are changed in init functions
//   (defaults correspond to us-reading)
// a/o 1.12: Apparently these were a remnant of pre-xyt times...
//           Though, as stated above, they are usable by dmsFrac
//           So remove them if dmsFrac is disabled!
#if (!defined(DMS_FRAC_UNUSED) || !DMS_FRAC_UNUSED)
extern uint8_t dmsNumUpdates; 
extern uint8_t dmsIncrementSize;
#endif

//Fractional part of the last-read dmsTime...
// This'll likely be handled differently in different cases
// for now it's only implemented in NON-AVR code or AVR code based on
// timer interrupt 0 (untested)
//volatile uint8_t dmsFrac = 0;


//DMS timer is updated spurradically by an externally-defined source...
extern xyt_t dmsLine;

//__inline__ prefix might be unnecessary since the prototype includes it
__inline__ \
void init_dmsExternalUpdate(uint32_t numUpdates, uint32_t incrementSize)
{
#if (!defined(DMS_FRAC_UNUSED) || !DMS_FRAC_UNUSED)

//This is the result of 1.12-1:
// which was developed with FRAC_UNUSED...
// an easy option would be to step back to 1.12
// OR replace dmsNumUpdates and dmsIncrementSize with uint32_t's.
// Alternatively (memory limited?) do some divisions here...
#error "DMS_FRAC was unused a/o the creation of this version"
#error "numUpdates and/or incrementSize > UINT8_MAX will be destroyed!"
#error "It's an easy fix, see notes in this file"

	dmsIncrementSize = incrementSize;
	//one less than the actual count since it counts from 0...
	dmsNumUpdates = numUpdates;
#endif

//#warning "xyt requires incrementSize < numUpdates!"
	xyt_init(&dmsLine, incrementSize, numUpdates);
}

//__inline__ prefix might be unnecessary since the prototype includes it
__inline__ \
void dms_update(void)
{
//	static uint8_t counter = 0;
//	static dms4day_t lastTime;

//	lastTime = dmsCount;

	dmsCount=xyt_nextOutput(&dmsLine);	
  #if (!defined(DMS_FRAC_UNUSED) || !DMS_FRAC_UNUSED)
	dmsFrac =xyt_remainder(&dmsLine);
  #endif
//	if(dmsCount != lastTime)
//		dmsFrac=0;
	//???What happens with odd fractions...
	// e.g. 2/5, would give frac=1,2 in one case, and just 1 in the next
//	else
//		dmsFrac++;
/*
	//counter++;

	// '>' Just in case update is called before init...
	//@@@ Could use an xyT here?
	//    e.g. numUpdates=4, incSize=3
	//    would work better with dmsGetFrac...
	if(counter >= dmsNumUpdates)
	{
		counter = 0;
		dmsCount+=dmsIncrementSize;
	}
	else
		counter++;

	dmsFrac = counter;
*/
}

__inline__ \
dms4day_t dmsGetTime(void)
{
	return dmsCount;
}

#else //This has aleady been included...
 #warning "really?"
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
 * /Users/meh/_avrProjects/LCDdirectLVDS/68-backToLTN/_commonCode_localized/dmsTimer/1.13/dmsExternal.h
 *
 *    (Wow, that's a lot longer than I'd hoped).
 *
 *    Enjoy!
 */

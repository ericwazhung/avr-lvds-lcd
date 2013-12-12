//high frequency modulation .93-2
//.93-2 adding this note:
//    TODO: Don't reset the running/desiredSums in setPower...
//          So setPower could be called repeatedly with the same value
//.93-1 adding HFMODULATION_REMOVED for size-testing
//.93 adjustable divisor... no longer limited to 256 steps
//    adding _HFMODULATION_HEADER_ to makefile
//    removal of stepNumber
//.92 added makefile, updated bithandling...
//.91cf moved to _common from MotorDriver11
//.91 stepNum implemented
//.90 Stolen from heartbeat .95

#ifndef __HFMODULATION_H__
#define __HFMODULATION_H__

#include <avr/io.h>
//Updated for/from 24Scam...
//#include "../../bithandling/0.80cf/bithandling.h"
//#include "../../bithandling/0.93/bithandling.h"

/*
	if power=102 (2 * 255/5)
		
	Cycle Entry -> Exit:
	stepNum desiredSum	runningSum	Output	(Avg)
	0 -> 1	0->102	 >	0 	-> 255	ON		(255)
	1 -> 2	102->204 <	255 -> 255	OFF		(127.5)
	2 -> 3	204->306 >	255	-> 510	ON		(170)
	3 -> 4	306->408 <	510	-> 510	OFF		(127.5)
	4 -> 0	408->510->0 510->510->0	OFF		(102)
	REPEAT
*/
typedef volatile struct
{
	uint16_t runningSum;	//in place of average...
	uint16_t desiredSum;	//in place of average...
	uint8_t	 maxPower;		//AKA power divisor... (usually 255) 
//	uint8_t  stepNumber;	//number of the current step... resets to 0 when the average output power = the desired. e.g. power=51, stepNum:0-4
	uint8_t  power;			//Desired power; 0-maxPower -> 0-100% -> 0-1 -> 0/maxPower - maxPower/maxPower
	// POWER is best NOT Read, since power>maxPower is set to maxPower
}  hfm_t;

#if (!defined(__HFMODULATION_REMOVED__) || !__HFMODULATION_REMOVED__)
void hfm_setup(hfm_t *modulator, uint8_t power, uint8_t maxPower);

void hfm_setPower(hfm_t	*modulator, uint8_t power);

//Returns 0 if the "output" should be "off" in this cycle
//		  1 if the "output" should be "on"  in this cycle
uint8_t hfm_nextOutput(hfm_t *modulator);
#else
#define hfm_setup(a,b,c) (0)
#define hfm_setPower(a,b) (0)
#define hfm_nextOutput(a) (0)
#endif
#endif

//high frequency modulation .94

#include "hfModulation.h"

//OLD:100% = 255, 0% = 0...
///volatile uint8_t  newDesired = 0;
// 100% = maxPower, 0% = 0... 



/* Basic algorithm concept:
 *	///increment numSteps (no longer necessary)
 * 
 *	- if the desired value is equal to the current average
 *		- restart (repeat the same pattern again, the overall average will then be the same...)
 *		- numSteps = 0
 *		- runningSum = 0
 * 
 * 	- if the desired (desiredSum) value is greater than the current "average" (runningSum) value  ///(runningSum/numSteps)
 *		///- Increment numSteps
 *		- turn on the output
 *		- add the maximum value to the sum to be averaged (runningSum)...
 *  - if the desired value is less than the current average
 *		///- Increment numSteps
 *		- turn off the output
 *		- add nothing to the runningSum
 *
 *
 *	--CAN BE OPTIMIZED for no divisions and no multiplications (?!)
 *	
 *	--NOT PERFECTLY ACCURATE unless called by timer interrupts...
 */

//Mainly just for setting up the maxPower (divisor)
// [can] use hfm_setPower otherwise...
// can't tell how to get away without using hfm_setup once
void hfm_setup(hfm_t *modulator, uint8_t power, uint8_t maxPower)
{
	modulator->desiredSum = 0;
	modulator->maxPower = maxPower;
	hfm_setPower(modulator, power);
}

//For the most part, an hfm will have the same divisor all the time... (255)
// This needs to be set, initially! 
//   either init it in the hfm variable declaration
//   or use hfm_setup (ideal)
// however, sometimes it needs to be variable, thus hfm_setup (above)
void hfm_setPower(hfm_t	*modulator, uint8_t power)
{
	//Don't allow power values > 1, otherwise who knows what'll happen...
	//(This is probably unnecessary, but whatev)
	//It's not, actually, if power's greater than maxPower, the values never reset
	// This is trouble, e.g. in heartbeat, where modulator.power is read
	if(power <= (modulator->maxPower))
		modulator->power = power;
	else
		modulator->power = modulator->maxPower;
		
//	modulator->stepNumber = 0;

//0.94 This is just a test... It should be OK, but it hasn't been
//     thoroughly-thought-out... (removal of these)
// CAUSES BADNESS.
//0.94-2 this should no longer be necessary... (runningSum is no more)
//	modulator->runningSum = 0;
	//0.94-2 should also no longer be necessary:
//	modulator->desiredSum = 0;
}

//Returns 0 if the "output" should be "off" in this cycle
//		  1 if the "output" should be "on"  in this cycle
uint8_t hfm_nextOutput(hfm_t *modulator)
{
	uint8_t toReturn;
	
	//The first time, this will be == desired*1... 
	// (and runningSum will be zero)
	//(modulator->desiredSum) += (modulator->power);


	//The first time, since runningsum == 0,
	//	if desiredSum == 0 we else...
	//		clr == good
	// if desiredSum > 0 we if...
	//		set == good
	
	// IF desired == 255, we want it never to clr...
	//		but running will be 0 the first time
	//		and we'll therefore if... set == good
	//		and later we'll runningSum == desiredSum (above) 
	//    and reset and if again
	if(modulator->desiredSum > 0)
	{
		(modulator->desiredSum) -= (modulator->maxPower); //0xff;
		toReturn = TRUE;
	}
	else	//avg == desired is handled at the start... (except 0-case which works here too)
	{
		//Don't modify runningSum, wait until desiredSum catches up...
		toReturn = FALSE;
	}
	
	(modulator->desiredSum) += (modulator->power);

	
	
//	(modulator->stepNumber)++;
/*
	//Restart the cycle if the output average == the desired
	if(modulator->runningSum == modulator->desiredSum)
	{
		//Handled below...
//		modulator->stepNumber = 0;
		modulator->runningSum = 0;
		modulator->desiredSum = 0;
	}
*/	
	return toReturn;
}



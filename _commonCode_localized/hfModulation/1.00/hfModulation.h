//high frequency modulation 1.00-4
//1.00-4 Looks like the gradient works... a little more testing.
//1.00-3 HOLY SHIT...
//       v0.94-1.00 were completely broken.
//       Revisiting 0.93-1.00 for fixing...
//-------------
	//high frequency modulation .94-3
	//.94-3: HOLY SHIT
	//       .94 was broken, completely. Only discovered in 1.00.
	//       adding tests and fixing.
	//       TODO: Fix requires setup to set desiredSum to zero
	//             This implies there may be setPower changes which may not
	//             be effective!
	//             (This particular instance might be due to being run on a
	//              PC where new variables aren't inited to 0...
	//              Since, in most AVR cases, the hfm_t variable will be
	//              static and thus initted to 0, no?)
//-------------
//1.00-2 Looking into increasing-power HFM
//       a/o LCDdirectLVDS-60 for possibly doing gradients by
//        rows alternating between one shade and another
//        e.g. a gradient from 0 to 1 across 16 pixels...
//          Rather than specifying a certain number of pixels to set the
//          power (e.g. 1/4 power for 4 pixels, 1/2 for the next four,
//                 3/4 for the next, and 1 for the final)
//          Maybe we can call hfm_setup 16 times each time with increasing
//          power...? (Relies on .93-2's TODO, which is no longer 
//            functional???)
//        Creating a test application (Didn't I do this already?!)
//1.00-1 Adding This Note:
//         Apparently this is called "Pulse-Density Modulation"
//         or "Sigma-delta modulation"?
//        see: http://en.wikipedia.org/wiki/Pulse_density_modulation
//1.00  (same as .94-2, but it's a big change so I want a new number)
//      Also (not noted previously, .94-2 did away with runningSum
//      so now it should be smaller if not faster
//.94-2 NOW: setPower can be called repeatedly with the same or similar 
//           values without resetting the count
//           (e.g. setPower could be run before every nextOutput)
//    .94/.94-1 changes (.93-2 todo) caused BADNESS
//    Complete redesign necessary:
//      Instead of working with sums, work with deltas...
//.94-1 Making computer-compatible...
//      complains about avr/io.h, is that even necessary?
//.94 Looking into .93-2 todo
//    Revised 102/255 example below
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

#include <stdint.h>
//Not necessary anymore...
//#include <avr/io.h>
//Updated for/from 24Scam...
//#include "../../bithandling/0.80cf/bithandling.h"
//#include "../../bithandling/0.93/bithandling.h"

/*
	if power=102 (2 * 255/5) ==> 2/5
		
	DesiredSum is always incremented by desired (power)
	   (currently, it's incremented immediately on entry)
	RunningSum is only incremented when desiredSum > runningSum (on entry)

	         --entry--                         --exit--
	step     desS  runS   desS<?>runS  output  
	0        102   0		    >         on      (runS -> 255)
	1        204   255       <         off     (runS = no change)
	2        306   255       >         on      (runS -> 510)
	3        408   510       <         off     (runS = no change)
	4        510   510       ==        off     (runS = no change)
  (5)      (102) (0)       (>)       (on)     (...)
	(repeats)

	As can be seen, in five cycles, the output is on for two (2/5 = 102/255)


   Prior to 0.94:
   A change of power (desired) would reset both desired and running sums
	(so if power is updated often enough at small and/or equal values, 
	  it may never light an LED!)

	Two considerations:
	 1) Don't reset desired/running sums when equal (step 5, above)
	    (use wraparound/overflow integer math?)
		 This seems difficult... and doesn't really serve a purpose
		 I thought it'd save a step, but the math is more complicated
		 e.g. power 2/3, desired 0-3 desired/runningSum 0-7:
		 	2 > 0 on
			4 > 3 on
			6 = 6 off
			0 ? 6 ?????
			There must be a way (using subtraction? differentials?)
			but it's not relevent to this, is it?
    2) Removal of reset during setPower...
	 	 Not thoroughly thought-out... then again neither was reset 
		 	apparently



	NOW: (.94-2)

	step   desS      output  desS+=des - (on ? 255 : 0)
	0      102  > 0  on      desS+=102-255 = -51
	1      -51  < 0  off     desS+=102 = 51
	2      51   > 0  on      desS+=102-255 = -102
	3      -102 < 0  off     desS+=102 = 0
	4      0    = 0  off     desS+=102 = 102
	(5 same as step 0)

 
	NOW: (1.00)	power=3, maxPower=5
	desS			output	desS += power - (on ? maxPower : 0)
	0		=0		off		desS += 3 - 0 = 3
	3		>0		on			desS += 3 - 5 = 6 - 5 = 1
	1		>0		on			desS += 3 - 5 = 4 - 5 = -1
	-1		<0		off		desS += 3 - 0 = 2
	2		>0		on			desS += 3 - 5 = 5 - 5 = 0
	----------------
	0	repeat
 
 
 
*/


// Basic usage:
// 	hfm_t whatever;
//    hfm_setup(&whatever, power, maxPower);
//
//    while(1)
//       ledVal = hfm_nextOutput(&whatever);
//
// CAN ALSO BE USED FOR:
//    e.g. a rise/run sorta thing, like xyTracker, but in 8-bit
//         recently used example: Stretching 512 rows across 768
//    hfm_setup(&stretcher, 64, 96); //64*8=512, 96*8=768
//    ...
//    for(row=0; row<768; row++)
//      outputRowSequentially(hfm_nextOutput(&stretcher));
//
//   where outputRowSequentially's argument is:
//      TRUE: output the next row
//      FALSE: output the same row as was last output



typedef volatile struct
{
//	uint16_t runningSum;	//in place of average...
	int16_t desiredSum;	//in place of average...
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

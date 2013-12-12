//adc 0.20-1

//0.20-1 notes
//       Was easier than I thought... adding usage notes below
//0.20 attempting to use for LCDdirectLVDS_56-racer
//0.19-1 adding this note:
//       commonFiled adc not used on latest projects... 
//			(e.g. threePinIDer, audioThing)
//       it's not interrupt-based at all, and there are so many options
//       free-running, muxing, etc... too crazy to implement in one go
//0.19 adding support for ADCs > 7 (tiny861)
//0.18-2 DIDR (digital input disable) on tinyX61 is odd... fix
//0.18-1 prepping for attiny861
//0.18 (eight-teen) prepping for ~/_commonCode/
//     mainly: ../../_common/bithandling.... is no longer valid.
//     (for threePinIDer1: _BITHANDLING_HEADER_ is now valid)
//     APPARENTLY: ../../_common was relative to the project, 
//       so it was never really valid!
//0.10-1 (ten) adding 1284p
//0.10 stolen from startingpoint 324pADC+AVG2+GOTO+LED and modified for CFs

//Basic procedure:
// (No interrupts, not free-running, etc)
// take input (ADC6 == "6") \ These could be combined into init
// init                     / with channelMask as an argument...
//
// Select channel (6)		\ These could be combined into a single
// start conversion        | blocking function...
// wait until not-busy     |
// read                    /
// repeat
//
// adcSum is a special-case that (as I recall) averages samples
// and automatically cycles through selected channels...
// its use is unnecessary e.g. CFLAGS+=-D'ADC_SUM_REMOVED=TRUE'

#ifndef __ADC_H__
#define __ADC_H__

#include <avr/io.h>

#if(defined(_AVR_IOMXX4_H_) || defined(__AVR_ATmega1284P__))
 #define NUMADCS 8
//Fall through
#elif(defined(_AVR_IOTNx61_H_))
// #warning "ATtinyX61's ADCs 7-10 are not implemented THIS SHOULD BE INVALID"
 #define NUMADCS 11
 //And fall through...
#else
 #error "this MCU not yet supported"
#endif

#define LASTADC (NUMADCS - 1)
//#include "../../_common/bithandling/0.94/bithandling.h"
#ifndef _BITHANDLING_HEADER_
 #include "../../bithandling/0.94/bithandling.h"
#else
 #include _BITHANDLING_HEADER_
#endif

//Bits in the ADMUX register for selecting an ADC channel
// Also used to select gain and/on differential channels
// For the TinyX61, this only covers ADMUX4..0
//   (ADMUX5 is in another register, but is currently unused)
#define ADC_SELECT_MASK	0x1f
#define ADC_SELECT_GND	ADC_SELECT_MASK

//Returned by adc_getValue when a conversion is still in progress
// or if no conversion was started
// OR if the interrupt-flag was somehow accidentally cleared during read-modify-write of ADCSRA
// (trying to watch these carefully)
#define ADC_NODATA	0xffff

//The last three bits in ADCSRA are the prescaler select bits...
// The prescaler divides the CPU clock by 2^ADPS_VAL...
// except that ADPS_VAL = 0 is the same as ADPS_VAL = 1
#define ADPS_MASK	0x07
#define ADPS_128	ADPS_MASK




void adc_takeInput(uint8_t inputNum);
void adc_init(void);

void adc_select(uint8_t adcNum);
uint8_t adc_startConversion(void);
uint8_t adc_isBusy(void);
uint16_t adc_getValue(void);


#if(!(defined(ADC_SUM_REMOVED) && ADC_SUM_REMOVED))
extern uint32_t adcSum[NUMADCS];
uint8_t adc_sumUpdate(void);
void adc_sumSetup(uint16_t numSamples, uint16_t channelMask);
#endif


//Mostly internal...
void adc_setPrescaler(uint8_t adps);


#endif

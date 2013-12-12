//sineTable.h 0.99-3
//.99-3 Looking into note from .99-2 a/o glColor18bit-1
//.99-2 Adding this note:
//       TODO: Isn't arrays' definitions incompatible with non AVRs?
//         PROGMEM should be undefined, no?
//         did I change it?
//.99-1 Adding this note:
//       TODO: Compare sineRaw8() to threePinIDer109's sineRaw127...
//        (sineRaw8 was developed independently)
//.99   adding sineRaw8()
//			Must add: -D'SINE_RAW8=TRUE' to CFLAGS
//       (since code-size was a factor in recent programs)
//       (This'll probably be removed in 1.00)
//       TODO: It probably makes more sense for it to be Unsigned (?)
//.98-5 adding theta_t
//      minor bug-fix re: DISABLEDOUBLESCALE disabling sineRaw by mistake
//.98-4 adding SINE_TABLE_ONLY
//.98-3 No shit, this was loading to the SRAM!!!
//      NOT! Apparently the 256ish byte increase in .data
//       was due to the use of doubles in sineScaled...
//       Also ~2000 bytes in program space
//      NOW: SINE_DISABLEDOUBLESCALE can be defined TRUE in makefile
//.98-2 adding ROUNDED values (for int16)
//.98-1 getting rid of __COMPILING_AVR_LIBC__ for __AVR_ARCH__
//.98 for glTest, attempting to make generic enough for non AVRs...
//.95-2 minor cleanup (unused variables)
//.95-1 using -floats- doubles! (esp in sineScaledRT)
//.95 fixed sineScaledRT doesn't take negative amplitude values
//		REMOVING sineSetup, amplitude, etc...
//.93-2 axisShift in sineScaledRT
//.93-1 adding SINE_PI_2 and SINE_3PI_2
//.93 adding sineScaledRT
// sine function with table-lookup and int32 output...
//.91-1 signed int16 theta, SINE_FULLCIRCLE=(2*Pi)
//.91 need to shrink the necessary memory...
//		use 16bits instead
//  Other Options to consider later:
//		calculate on the fly (.4ms per calc... .8 with amplitude)
//		sineSetup with numSteps (64, skip every-other?)
//  switch ifndef from SINES_H to SINETABLE_H
//.90 first commonFiled version, added sineSetup and sineScaled from LCDSineTest5
//sines .90 first version, in krautPoxn 41b.2

#ifndef __SINETABLE_H__
#define __SINETABLE_H__

#include <inttypes.h>

#if defined(__AVR_ARCH__) //__COMPILING_AVR_LIBC__)
#   include <avr/pgmspace.h>
#else
//#   error "WTF"
#   define prog_int16_t	const int16_t
#   define prog_int32_t const int32_t
//  called as: (wow, this works?!)
// pgm_read_sine(sineTable[SINE_SIZE2-theta])
// where pgm_read_sine(arrayItem) = pgm_read_word(&arrayItem)
#   define pgm_read_word(pArrayItem)		(*(pArrayItem))
#   define pgm_read_dword(pArrayItem)	(*(pArrayItem))
#	define PROGMEM 
#endif


#ifndef _SINE_TYPE_
#   define _SINE_TYPE_ 16
#   warning "Defaulting to sine-type 16-bit, define SINE_TYPE in makefile to override"
#endif

typedef int16_t theta_t;

//theta is a value between 0 and 511 (representing an angle of 0-364.som'n deg)
//   rollover/wraparound should work fine... (e.g. Sine(coord_t))
//output is a value between -INT32_MAX and INT32_MAX (representing sin values -1 to 1)
// Internal
//int32_t Sine(uint16_t theta);

//Necessary for sineTable[SINE_SIZE]
//SINE_SIZE covers 90 degrees (PI/2) + 1 (for 0?)
#define SINE_SIZE	((theta_t)129)
#define SINE_SIZE2	((SINE_SIZE-1)*2)
#define SINE_SIZE3	((SINE_SIZE-1)*3)
//Necessary for detecting whether the whole circle has been completed...
#define SINE_SIZE4	((SINE_SIZE-1)*4)
#define SINE_PI		SINE_SIZE2
#define SINE_2PI	SINE_SIZE4

//PI/2 (90deg)
#define SINE_PI_2	(SINE_SIZE-1)
//3PI/2 (270deg)
#define SINE_3PI_2	((SINE_SIZE-1)*3)
//512
#define SINE_FULLCIRCLE SINE_2PI

#if (_SINE_TYPE_ == 16)
 #define SINE_MAX INT16_MAX
 typedef int16_t	sine_t;
//a multiplication is still done with an int32, via axis_t
// so mult_t should still be int64
 typedef int32_t	mult_t;
// #error "mult_t = int32 means that axisMax can't be larger than 65536!!" 
//#warning "mult_t is int64!!!!"
// typedef int64_t mult_t;
 
//Actually, this shouldn't be true... the multiplication occurs AFTER xmax is shifted
// typedef int64_t mult_t;
#elif (_SINE_TYPE_ == 32)
 #define SINE_MAX INT32_MAX
 typedef int32_t	sine_t;
 typedef int64_t	mult_t;
#else
 #error
#endif

typedef int32_t	axis_t;

 
// Let's just assume everything is in sine_t... << and >> as appropriate
//#define AMPLITUDE_MAX	SINE_MAX

//An amplitude of 1 is AMPLITUDE_MAX


//Get the scaled sine value from theta and a prior call to sineSetup...
//	360deg = (SINE_SIZE-1)*4 (i.e. 512)

#if (!defined(SINE_TABLE_ONLY) || !SINE_TABLE_ONLY)
sine_t sineRaw(theta_t theta);

#if ( defined(SINE_RAW8) && SINE_RAW8 )
int8_t sineRaw8(theta_t theta);
#endif

 #if (!defined(SINE_DISABLEDOUBLESCALE) || !SINE_DISABLEDOUBLESCALE)
axis_t sineScaled(theta_t theta, axis_t axisAmplitude);
 #endif
#endif
#endif


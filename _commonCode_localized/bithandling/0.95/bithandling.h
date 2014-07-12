/* mehPL:
 *    This is Open Source, but NOT GPL. I call it mehPL.
 *    I'm not too fond of long licenses at the top of the file.
 *    Please see the bottom.
 *    Enjoy!
 */


//bitHandling 0.95-6
//TODO: Don't Forget .94-18!
//TODO: and .94_...-5!
//
//.95-6  I think it's time to remove the message about macros and () and {}
//       and also setPORT___'s' being untested
//.95-5  adding ZERO8_FN()
//.95-4  adding ROUND_UP_TO()
//.95-3  adding CLI_SAFE() -> SEI_RESTORE()
//.95-2  Adding MAKEFLOAT()
//.95-1	Adding this note: BV(thing) is old from libc_dep (and before)
//       This is defined as _BV(thing), which apparently is defined
//       somewhere. This is equivalent to (1<<thing), as I recall.
//       This note, a/o LCDRevisited2012-25
//.95 - It's time to quit typing "_parenExperiment"...
//      ALSO adding nibbletochar()
//.94_...-5 AH-HAH! The "proper" way is as in info cpp 3.10.3:
//          do{ .... } while(0)
//          I think my method's more elegant, have yet to find an issue
//             well, the additional {}; is a bit of a nuisance...
//          But that method would be guaranteed
//          And, apparently, the *only* case where it's a problem to not
//          use one of these methods is an else case...
//          which generates an error, anyhow
//          Unless there's a hokey-case, like
//					if (something)
//						if (something else)
//							DOSOMETHING();
//						else
//                   ...
//          Which I'd *never* code anyhow... where SHOULD that else belong?//      AH HAH 2.0!
//        "The `({ ... })' notation produces a compound statement that acts
//			  as an expression. Its value is the value of its last statement."
//         --info cpp 3.10.4

//.94_...-4
//			TRUNCATE_U8(thing) -> (thing&0xff)
//.94_parenExperiment-3
//       DUH, MAKELONG should look like a function...
//         e.g. (MAKELONG(ONE) * MAKELONG(ANOTHER))
//.94_parenExperiment-2
//       Adding MAKELONG
//         e.g. when preprocessed math needs to be done on two constants
//         neither of which are long, but the result of the math
//         requires LONG types, it would complain "integer overflow in
//         expression" (a/o sdramThing2.0-6)
//         So MAKELONG is used as such:
//            #define ONE 65535
//            #define ANOTHER 65535
//            #define ONEANOTHER ((ONE+MAKELONG)*(ANOTHER+MAKELONG))
//            (It might make more sense to use (ONE*MAKELONG)... we'll see)
//			Moving SFR register notes to sfrNotes.txt
//.94_parenExperiment-1
//       Apparently the ({}) block returns the value of the last... item
//       see cTools/bracketsInDefinesTest3.c
//       So adding {}; at the end prevents the return. LOL
//       This is friggin' hilarious.
//.94_parenExperiment Looking into {} in #defines causing issues with
//       if(...)
//          DOSOMETHING;
//       else
//          whatever...
//       Where DOSOMETHING -> #define DOSOMETHING {...}
//       Resulting in:
//       if(...)
//          {...}; <--- That semicolon kills the if-statement
//       else           So else doesn't refer to anything (syntax error)
//          whatever... 
//       So change #define DOSOMETHING {...} to ({...})
//       See cTools/bracketsInDefinesTest.c
//     Interesting effects...
//       setPORTinpu() is now acting like a function, ish...
//       "returning" a value... i.e. reading the port just written into
//       r24. Yet r24 is unused... 
//       so it's kinda like having, essentially, "PORTA;" as a line...
//       which doesn't do anything, but the compiler might not know that
//       (e.g. reading some SFRs might cause their values to change?)
//       (e.g.notQuite: reading an Interrupt SFR would clear the interrupt
//        flags)
//       (run an lssDiff between sdramThing2.0-4 and 4bh)
//			otherwise, the code seems identical... just a few extra
//       (unnecessary) instructions
//     Thing is: 
//       I feel like there *could* be a case where the old {} method would
//       cause unnoticed errors. I can't think of such a case.
//     Further, is it really logical that a ({}) block has a "return"
//       value?
//
//.94-20 Adding SHIFT_LEFT_ONES() and toBinString() (first function!)
//       a/o sdramThing19
//.94-19 (was there a 18?)
//       set entire port to output...
//       PIN/DDR_FROM_PORT(PORTx)
//       setPORT___[Masked]() ('___' = out/in/inpu)
//       PORTin()
//       a/o sdramThing3
//.94-18 TODO IMPORTANT TODO:
//       How to handle {} #defines which might be used after an if
//       if statement??? e.g.
//     < if(somethingBigger) >
//         if(something)
//           setinpuPORT(...);
//         else
//           clrpinPORT(...);
//         Without somethingBigger, else isn't associated with anything!
//         Because if(something) { };
//          (apparently the ; closes the if)
//         So then, ... what...?
//         With somethingBigger, we should *always* use brackets, anyhow
//         Because it's *REALLY* hard to read and totally unpredictable
//         And the only other case I can think of is the case I ran into
//         Where else is disassociated altogether, resulting in an error
//         So maybe it's not such a big deal anyhow.
//      Who knows, though... there could be other such cases where ;-ing
//        is a bad idea.
//.94-17 increasing DESHIFT()'s capability for at least u16 
//       for audioThing16's BLOCKSIZE_SHIFT
//.94-16 Adding This TODO:
//         Add extractBitsFromU8Array() (from cTools, and/or audioThingv10)
//.94-15 adding DESHIFT() (from cTools/dePower2.c)
//.94-14 (apparently the numbering was off?)
//       adding writepinPORT()
//.94-12 adding UPPER_BIT_MASK8(numBits)
//       and    UPPER_BIT_MASK8__COUNT_1(numBits)
//.94-11 no changes, but this is post-JiggyFour crash and recovery
//.94-8 fixing reverseBin40toU8s
//.94-7 adding bin32 and bin40toU8s
//      adding include <stdint.h>
//      adding reversebin()
//.94-6 adding TOBCD(dec99)
//.94-5 adding shiftRightSigned
//.94-4 compilation notes...
//.94-3 cleanup and adding note/code re: bit-reversal
//      but it needs to be in a function.
//.94-2 adding writebit(bitNum, byte, value) AND FIX
//.94-1 adding togglepinPORT
//.94 added __PORT macros, __PIN's stupid
//TODO: writebit(value, bitNum, bitwiseByte)
//.93-2 added writeMaskedVar (untested)
//.93-1 __Var notes. Added __PIN macros...
//.93 parened getbit's args... added writeMasked, stolen from .80 (should make a new version, but it's a simple change and I'm lazy)
//.92(?)cf moved to _common from 24scam5
//.92 Added puWait for pull-up input capacitance handling...
//.91? Added var-based bithandling for ports
//.90 From 2OctStuff/5
#ifndef __BITHANDLING_H__
#define __BITHANDLING_H__

#include <stdint.h>


//developed for HEART_REMOVED a/o heartbeat1.30-7(?)
// this is to replace a function which returns a uint8_t with 0
// Done this way to handle multiple cases e.g.:
// #if (! HEART_REMOVED)
// uint8_t heartUpdate(void)
// #else
// #define heartUpdate()	ZERO8_FN()
// #endif
// 
// And in main:
//  heartUpdate();
// OR
//  if(heartUpdate())
//   ...
// see cTools/unusedMacroTest.c
//  this avoids the warnings:
//   "statment with no effect"
//   "unused variable"
//   "uninitialized variable"
//   and doesn't choke if a return-value is necessary.
//   And I'm pretty certain it should completely optimize out to nothing
#define ZERO8_FN()	({uint8_t a=0; a;})




#ifdef __AVR_ARCH__

//If only used once, can call this as:
// uint8_t CLI_SAFE(uint8_name);
// ...
// SEI_RESTORE(uint8_name);
// ...
// //But can also be reused:
// CLI_SAFE(uint8_name);
// ...
// SEI_RESTORE(uint8_name);
#define CLI_SAFE(uint8_name) (uint8_name) = (SREG & (1<<7)); cli()

#define SEI_RESTORE(uint8_name) if(uint8_name) sei()
#endif



//Takes a value and rounds it up such that it is divisible by roundTo...
// e.g. minVal=5, roundTo=7 -> 7
//      minVal=15, roundTo=7 -> 21
#define ROUND_UP_TO(minVal, roundTo) \
	   ((((minVal)+((roundTo)-1))/(roundTo))*(roundTo))




//Takes a 2^n value and determines how many shifts are necessary to create
// it... e.g. 128 = (1<<7) so DESHIFT(128) = 7
// Many prescaler bits are configured this way... 
//    e.g. ATtiny861's ADC Prescaler
//         ATtiny861's Timer1 is similar, but needs a little more math
//         e.g. #define TC1_PS_BITS (DESHIFT(128) + 1) UNCHECKED
//         Normal Timers may be similar as well, with a shift/division?
//         e.g. ATtiny861's Timer0... well, not quite.
// NOTE: It only works for values 1, 2, 4, 8, 16, 32, 64, 128 so far
//       e.g.: DESHIFT(255)==7==DESHIFT(128),  DESHIFT(63)==DESHIFT(32)
//  AND: It's probably NOT WISE to use this with variables
//       e.g. #define ADC_PS_BITS DESHIFT(128)
//            (ADC_PS cannot be 1, but that's not DESHIFT's fault...)
// There must be a better way to do this... obviously a for loop would work
//  but this is for #define's and things that I don't want recalculated
// TODO?: Return a 16-bit value, shifted so no-zeros are on the right
//        with upper 8-bits as the result?
//        e.g. DESHIFT(0x50) -> 0x0504, (0x05<<4)
//             DESHIFT(0x0C) -> 0x0302, (0x03<<2)
//             DESHIFT(128)  -> 0x0107, (0x01<<7)
//        Then could do: 
//          #define ADCPS DESHIFT(ADCDIV)
//          #if ((ADCPS & 0xfe00) || (ADCPS & 0xf8)) //ADCPS_BITS<=0x07
//           #error "Invalid ADCDIV"
//          #else
//           #define ADCPS_BITS (ADCPS & 0xff)
//          #endif
#define DESHIFT(n) \
	( (n & (1<<15)) ? 15 : (n & (1<<14)) ? 14 : (n & (1<<13)) ? 13 : \
	  (n & (1<<12)) ? 12 : (n & (1<<11)) ? 11 : (n & (1<<10)) ? 10 : \
	  (n & (1<<9))  ?  9 : (n & (1<<8))  ?  8 : \
	  (n & 128) ? 7 : (n & 64) ? 6 : (n & 32) ? 5 : (n & 16) ? 4 : \
	  (n & 8) ? 3 : (n & 4) ? 2 : (n & 2) ? 1 : (n & 1) ? 0 : 0 )


//Wait for the input capacitance to be overcome by the pull-ups
//	experiments showed 2us, which would be a count of 32, why minimize...?
//  Also: Pull-ups = 50k (max) (no info on input capacitance...)
//  T=RC 2us/50k = 40pF (seems extreme, but... maybe also due to PCB...?)
#define puWait()							\
({											\
	uint8_t puCount;						\
	for(puCount=0; puCount<100; puCount++)	\
		asm("nop");							\
	{}; \
})

//Convert e.g. 30 to 0x30 for display purposes...
#define TOBCD(dec99)   \
	      ((((dec99)/10)<<4) | ((dec99)%10))

//This is risky, as, as far as I'm aware, shifting of signed values
// has undefined behavior
// Experimentally (see cTools/shiftSigned2.c and threePinIDer81ib)
//    it works as expected-ish:
// This is essentially like dividing by 2^shift

static __inline__ \
int32_t shiftRightI32(int32_t value, uint8_t shift) \
			__attribute__((__always_inline__));

__inline__ \
int32_t shiftRightI32(int32_t value, uint8_t shift)
{
	//negative values shifted-right past their value
	// give -1... e.g. -4 >> 10 = -1 (rather than 0)
	if((value < 0)
		// > since we're working with negative values...
	   && (value > -(((int32_t)1<<shift))))
		  return 0;
	else
		return (value>>shift);
}


//Taken and modified from Atmel's AppNote AVR307 (USI-UART)
// Pretty groovy looking, but haven't looked into how it works, yet...
//Can I do this in a header...? (with return? NOPE.)
// This is pretty long, it should probably be a function, realistically...
/*#define reverseByte(byte) \
{									\
	uint8_t x=(byte);			\
	x = ((x >> 1) & 0x55) | ((x << 1) & 0xaa);	\
	x = ((x >> 2) & 0x33) | ((x << 2) & 0xcc);	\
	x = ((x >> 4) & 0x0f) | ((x << 4) & 0xf0);	\
	return x;	\
}*/


/* Convert 8 bits to a byte */
//reverseBin is for graphics-type-stuff...
// e.g. we're storing characters left-to-right starting from bit0
// (since reading/addressing starts at 0 and starts fom the left)
#define reverseBin(b0,b1,b2,b3,b4,b5,b6,b7) \
					bin(b7,b6,b5,b4,b3,b2,b1,b0)
#define bin(b7,b6,b5,b4,b3,b2,b1,b0) \
	(((((((((((((((b7) << 1) \
					| (b6)) << 1) \
				   | (b5)) << 1) \
				   | (b4)) << 1) \
			      | (b3)) << 1) \
		         | (b2)) << 1) \
		         | (b1)) << 1) \
	            | (b0))
#define bin4(b3,b2,b1,b0) \
	((((((((((((((0 << 1) \
					| 0) << 1) \
				   | 0) << 1) \
				   | 0) << 1) \
			      | b3) << 1) \
		         | b2) << 1) \
		         | b1) << 1) \
	            | b0)
#define bin32(b31,b30,b29,b28,b27,b26,b25,b24,b23,b22,b21,b20,\
		b19,b18,b17,b16,b15,b14,b13,b12,b11,b10, \
		b9,b8,b7,b6,b5,b4,b3,b2,b1,b0) \
	   ( ((uint32_t)bin(b31,b30,b29,b28,b27,b26,b25,b24)<<24) \
		| ((uint32_t)bin(b23,b22,b21,b20,b19,b18,b17,b16)<<16) \
		| ((uint32_t)bin(b15,b14,b13,b12,b11,b10,b9,b8)<<8) \
		| ((uint32_t)bin(b7,b6,b5,b4,b3,b2,b1,b0)) )

//This is for parsing a 40-bit value into uint8_t's
//  suitable for array initialization
// Probably won't be used... see below...
#define bin40toU8s(b39,b38,b37,b36,b35,b34,b33,b32,b31,b30, \
		                   b29,b28,b27,b26,b25,b24,b23,b22,b21,b20, \
		                   b19,b18,b17,b16,b15,b14,b13,b12,b11,b10, \
		                    b9, b8, b7, b6, b5, b4, b3, b2, b1, b0) \
					{ bin( b7, b6, b5, b4, b3, b2, b1, b0), \
			        bin(b15,b14,b13,b12,b11,b10, b9, b8), \
			        bin(b23,b22,b21,b20,b19,b18,b17,b16), \
			        bin(b31,b30,b29,b28,b27,b26,b25,b24), \
			        bin(b39,b38,b37,b36,b35,b34,b33,b32) \
			      }
//For graphics:
#define reverseBin40toU8s( b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, \
								  b10,b11,b12,b13,b14,b15,b16,b17,b18,b19, \
								  b20,b21,b22,b23,b24,b25,b26,b27,b28,b29, \
								  b30,b31,b32,b33,b34,b35,b36,b37,b38,b39) \
					{ bin( b7, b6, b5, b4, b3, b2, b1, b0), \
					  bin(b15,b14,b13,b12,b11,b10, b9, b8), \
					  bin(b23,b22,b21,b20,b19,b18,b17,b16), \
					  bin(b31,b30,b29,b28,b27,b26,b25,b24), \
					  bin(b39,b38,b37,b36,b35,b34,b33,b32) \
					}


//Wow... apparently it's smart enough to optimize this to a cbi if possible!
#define setbit(bitNum, bitwiseByte) \
	(bitwiseByte = ((bitwiseByte) | (1 << (bitNum))))
//Wow... apparently it's smart enough to optimize this to a cbi if possible!
#define clrbit(bitNum, bitwiseByte) \
	(bitwiseByte = ((bitwiseByte) & (~(1 << (bitNum)))))

#define togglebit(bitNum, bitwiseByte) \
	bitwiseByte = ((bitwiseByte) & ~(1<<(bitNum))) \
               | (~((bitwiseByte) & (1<<(bitNum))) & (1<<(bitNum)))

//value is only predictable with 0 and 1, other values though "true" may not work... should create a wrapper?
#define writebit(bitNum, bitwiseByte, value)	\
	((value) ? setbit((bitNum),(bitwiseByte)):clrbit((bitNum),(bitwiseByte)))
//(bitwiseByte = ((bitwiseByte) | ((value & 0x01) << (bitNum))))


//??? Would this be more efficient if ((bitwiseByte >> bitNum) & 0x01)?
//#define getbit(bitNum, bitwiseByte) (((1 << (bitNum)) & (bitwiseByte)) >> (bitNum))
//!!! HOLY FUCK 70 bytes?! (WRT PE0,PINE...)
#define getbit(bitNum, bitwiseByte) \
	(((bitwiseByte) >> (bitNum)) & 0x01)

/* Added 10/5/03 */
#define IN		0	/* Set a bit on a port as an input */
#define OUT		1	/* Set a bit on a port as an output */
#define ALLINS	0x00	//Written to DDRx
#define ALLOUTS	0xff	//Written to DDRx
#define ALLUP	0xff	//Written to PORTx (while an input)

#define setout(bitNum, portdir) \
	setbit(bitNum, portdir)	/* set a bit on a port as an output */

#define setin(bitNum, portdir) \
	clrbit(bitNum, portdir) /* set a bit on a port as an input */

#define setpu(bitNum, port)	\
	setbit(bitNum, port)		/* enable the pull-up on the port */

//NOTE!!! Mask macros won't work with PA0, etc...
#define setinMask(mask, portdir)	\
	((portdir) &= ~(mask))	/* set bits as inputs */

#define setoutMask(mask, portdir) \
	((portdir) |= (mask))	/* set bits as outputs */

#define setpuMask(mask, port)	\
	((port) |= (mask))		/* set pull-ups on the port */


#define setoutS(pin)	\
	(setbit(pin.pinNumber, (*(pin.ddr))))

/* Set the pin as an input AND pull it up */
#define setinPU(pin)	\
	clrbit(pin.pinNumber, (*(pin.ddr))); \
	setbit(pin.pinNumber, (*(pin.port)))

	
#define getpin(pindesc)	\
	getbit(pindesc.pinNumber, (*(pindesc.pin)))

//Added 6-4-08
//where the port address is given by a variable pointing to the port's PIN

//e.g.
// volatile uint8_t* heartPIN;
// ...
// INIT_HEARTBEAT(pin, ...) init_heartBeat((&(pin)), ...)
// init_heartBeat(volatile uint8_t* pin) {
//		heartPIN = pin;
//
//		setoutVar(...,heartPIN);
//	}

//	thus setoutVar acts on a ...volatile uint8_t*...
//  thus heartPIN !!!= PINx
//  heartPIN == &(PINx)
//  so these ..Var-macros don't work directly with PINx, but a variable pointing to it...
#define PORTOFFSET	2
#define DDROFFSET	1
#define setoutVar(bitNum, bitPIN)	setout((bitNum), (*((bitPIN)+DDROFFSET)))
#define setinVar(bitNum, bitPIN)	setin((bitNum), (*((bitPIN)+DDROFFSET)))
#define setpuVar(bitNum, bitPIN)	setpu((bitNum), (*((bitPIN)+PORTOFFSET)))

#define setpinVar(bitNum, bitPIN)	setbit((bitNum), (*((bitPIN)+PORTOFFSET)))
#define clrpinVar(bitNum, bitPIN)	clrbit((bitNum), (*((bitPIN)+PORTOFFSET)))

#define getpinVar(bitNum, bitPIN)	getbit((bitNum), (*(bitPIN)))
#define writeMaskedVar(val, mask, bitPIN, portOffset)	\
									writeMasked((val),(mask),(*((bitPIN)+(portOffset))))
#define	setinpuVar(bitNum, bitPin)	\
({									\
	setinVar((bitNum), (bitPin));	\
	setpuVar((bitNum), (bitPin));	\
	{}; \
})

//These ...PIN macros work directly with PINx...
// might make more sense to work with PORTx 
// and shift the offsets accordingly...
// See below for ...PORT macros...

//OR _SFR_IO8 (from sfr_defs)
//#define _SFR_IO8(io_addr) ((io_addr) + __SFR_OFFSET)
//#define _SFR_IO8(io_addr) _MMIO_BYTE((io_addr) + 0x20)
//#define _MMIO_BYTE(mem_addr) (*(volatile uint8_t *)(mem_addr))
//@@@#warning "xxxPIN macros should be gotten rid of... setpinPIN doesn't make sense!"
//one instruction: sbi... mainly (&(PINx) + ___OFFSET) is preprocessed
#define setoutPIN(bitNum, PINx)	 \
	setout((bitNum), _MMIO_BYTE(&(PINx) + DDROFFSET))
//one instruction: cbi
#define setinPIN(bitNum, PINx)	\
	setin((bitNum), _MMIO_BYTE(&(PINx) + DDROFFSET))
//one instruction: sbi
#define setpuPIN(bitNum, PINx)	\
	setpu((bitNum), _MMIO_BYTE(&(PINx) + PORTOFFSET))

#define setpinPIN(bitNum, PINx)	\
	setbit((bitNum), _MMIO_BYTE(&(PINx) + PORTOFFSET))
//one instruction: cbi
#define clrpinPIN(bitNum, PINx)	\
	clrbit((bitNum), _MMIO_BYTE(&(PINx) + PORTOFFSET))

#define togglepinPIN(bitNum, PINx)	\
	togglebit((bitNum), _MMIO_BYTE(&(PINx) + PORTOFFSET))

#define getpinPIN(bitNum, PINx)	\
	getbit((bitNum), (PINx))

#define setinpuPIN(bitNum, PINx)	\
({									\
	setinPIN((bitNum), (PINx));		\
	setpuPIN((bitNum), (PINx));		\
	{}; \
})

// setout(PB6, PORTA)
//Relative PIN
//#define PORTOFFSET	2
//#define DDROFFSET	1
//#define PINOFFSET 0
//then NEGATIVE relative PORT:
#define PORTPORTOFFSET	0
#define DDRPORTOFFSET	1
#define PINPORTOFFSET	2
//Most seem to compile as a single instruction!

#define setoutPORT(Pxn, PORTx)	\
	setout((Pxn), _MMIO_BYTE(&(PORTx) - DDRPORTOFFSET))

#define setinPORT(Pxn, PORTx)		\
	setin((Pxn),  _MMIO_BYTE(&(PORTx) - DDRPORTOFFSET))

#define setpuPORT(Pxn, PORTx)		\
	setpu((Pxn),  _MMIO_BYTE(&(PORTx) - PORTPORTOFFSET))

#define setpinPORT(Pxn, PORTx)	\
	setbit((Pxn), _MMIO_BYTE(&(PORTx) - PORTPORTOFFSET))

#define clrpinPORT(Pxn, PORTx)	\
	clrbit((Pxn), _MMIO_BYTE(&(PORTx) - PORTPORTOFFSET))

#define getpinPORT(Pxn, PORTx)	\
	getbit((Pxn), _MMIO_BYTE(&(PORTx) - PINPORTOFFSET))

#define togglepinPORT(Pxn, PORTx)	\
	togglebit((Pxn), _MMIO_BYTE(&(PORTx) - PORTPORTOFFSET))

#define writepinPORT(Pxn, PORTx, value) \
	   ((value) ? setpinPORT((Pxn),(PORTx)):clrpinPORT((Pxn),(PORTx)))

//Time to remove this message... It's useful, but it's also
//deeply-ingrained in my brain, and the error it generates is obvious
//and it doesn't seem to have any odd consequences in other cases.
//#warning "TODO: (IMPORTANT?) Using this as a response to an if-statement closes the statement, due to the '};' so the associated else isn't associated! THIS COULD BE A PROBLEM IN MULTIPLE PLACES. WHICH IS WHY I HAVE THIS IN THE BITHANDLING HEADER TO REMIND ME"
// SEE: cTools/bracketsInDefinesTest.c
// also: cTools/unusedMacroTest.c

#define setinpuPORT(Pxn, PORTx)		\
({									\
	setinPORT((Pxn), (PORTx));		\
	setpuPORT((Pxn), (PORTx));		\
	{}; \
})


#define PIN_FROM_PORT(PORTx) \
	((_MMIO_BYTE(&(PORTx) - PINPORTOFFSET)))

#define DDR_FROM_PORT(PORTx) \
	((_MMIO_BYTE(&(PORTx) - DDRPORTOFFSET)))


// Let's do the same for Pulled up vs not pulled up



// For an entire port:
//#warning "setPORT___'s are untested..."
//I think these are pretty well verified by now...
#define setPORTout(PORTx)		(DDR_FROM_PORT(PORTx) = 0xff)

//NOTE: This does NOT affect the pull-up state (which is determined
// by the value already on PORTx)
#define setPORTin(PORTx)		(DDR_FROM_PORT(PORTx) = 0x00)

#define clrPORTpu(PORTx)		((PORTx) = 0x00)

#define setPORTpu(PORTx) 		((PORTx) = 0xff)

#define setPORTinpu(PORTx) \
({\
	setPORTin(PORTx); \
	setPORTpu(PORTx); \
	{};\
})

//Equivalent to PINx (from PORTx)
#define PORTin(PORTx) 			(PIN_FROM_PORT(PORTx))


#define setPORToutMasked(PORTx, Mask)	\
	writeMasked(0xff, Mask, DDR_FROM_PORT(PORTx))

#define setPORTinMasked(PORTx, Mask) \
	writeMasked(0x00, Mask, DDR_FROM_PORT(PORTx))

#define setPORTpuMasked(PORTx, Mask) \
	writeMasked(0xff, Mask, PORTx)

#define setPORTinpuMasked(PORTx, Mask) \
({ \
	setPORTinMasked(PORTx, Mask); \
	setPORTpuMasked(PORTx, Mask); \
	{};\
})







/* Get the value of a certain bit */
#define VAL(bitnum)	(1 << bitnum)

//(variable & ~mask)	clears the bits to be written
//(value & mask) 		assures value doesn't overwrite masked bits

#if (__ERR_WARN__)
 #warning "if an interrupt modifies variable, we could screw up the variable!"
#endif


#define writeMasked(value, mask, variable)	\
	(variable) = (((variable) & ~(mask)) | ((value) & (mask)))


#define TRUE	1
#define FALSE	0


#define PINTOPORT(PINx)	(_MMIO_BYTE(&(PINx) + PORTOFFSET))

// Creates a mask with upperBits bits...
// e.g. UPPER_BIT_MASK(4)=0xf0 = 1111 0000
//      UPPER_BIT_MASK(5)=0xf8 = 1111 1000
// &0xff is necessary for passing this in as an assembly argument
// otherwise, I guess, it assumes it's a 16-bit value
// (casting didn't fix it)
#define UPPER_BIT_MASK8(upperBits)	\
	(((0xff)<<(8-(upperBits)))&0xff)
// Creates a single count in the upper-bit-mask...
// e.g. ...COUNT_1(4) =   0x10 = 0001 0000
//      ...COUNT_1(5) =   0x08 = 0000 1000
#define UPPER_BIT_MASK8__COUNT_1(upperBits)	\
	(((0x01)<<(8-(upperBits)))&0xff)


//Say you want to find the largest value storable in an addressing-scheme
// this for sdramThing19
// e.g. 0b 1011 1111 1111 1111 is the highest address
// you know 0b10<<14, but want, essentially, 0b10<<(1, 14)
#define SHIFT_LEFT_ONES(value, shift) \
   ((((value)+1)<<(shift))-1)


//Since this is a function, we don't want it compiled everywhere
// or anywhere unless we're using it...
// and since it's in a header that's included everywhere... ugh
// I think what should be done is #define INCLUDE_TOBINSTRING
// in the file which uses it before any includes (bithandling.h
// might be included in another include elsewhere)
// This is hokey, but mainly I just want it in commonCode for later
// example in cTools/shiftOnes.c
#if(defined(INCLUDE_TOBINSTRING) && INCLUDE_TOBINSTRING)
void toBinString(char* stringOut, uint8_t length, int32_t value)
{
   int8_t i;

   for(i=length-1; i>=0; i--)
   {
		if((i&0x03) == 0x03)
		{
			*stringOut = ' ';
			stringOut++;
		}
		*stringOut = (value & (1<<i) ) ? '1' : '0';
      stringOut++;
      *stringOut = '\0';
   }
}
#endif


//See explanation in version-notes 0.94_parenExperiment-2
// Note also that according to K&R this will force the other operands
// to become long as well..
// e.g. the result of (1 + MAKELONG) is a LONG 
#define MAKELONG(value)	((value) + 0L)

#define MAKEFLOAT(value) ((value)*1.0)

//Not really sure how I feel about this... technically it doesn't change it
// to u8 math... but it does assure that ... yeah, not sure how I feel...
// a/o sdramThing: #define BLAH (~(1<<blah)) 
//	uint8_t blah2 = BLAH; resulted in "large integer implicitly truncated
// to unsigned type"
#define TRUNCATE_U8(value) ((value) & 0xff)


#define nibbletochar(val)  \
	   (((val)<=9) ? ((val) + '0') : ((val) + ('A' - 10)))



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
 * /Users/meh/_avrProjects/LCDdirectLVDS/93-checkingProcessAgain/_commonCode_localized/bithandling/0.95/bithandling.h
 *
 *    (Wow, that's a lot longer than I'd hoped).
 *
 *    Enjoy!
 */

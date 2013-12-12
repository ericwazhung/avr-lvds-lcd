//bitHandling 0.94-20
//TODO: Don't Forget .94-18!
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
{											\
	uint8_t puCount;						\
	for(puCount=0; puCount<100; puCount++)	\
		asm("nop");							\
}

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
{									\
	setinVar((bitNum), (bitPin));	\
	setpuVar((bitNum), (bitPin));	\
}

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
{									\
	setinPIN((bitNum), (PINx));		\
	setpuPIN((bitNum), (PINx));		\
}

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

#warning "TODO: (IMPORTANT?) Using this as a response to an if-statement closes the statement, due to the '};' so the associated else isn't associated! THIS COULD BE A PROBLEM IN MULTIPLE PLACES. WHICH IS WHY I HAVE THIS IN THE BITHANDLING HEADER TO REMIND ME"
#define setinpuPORT(Pxn, PORTx)		\
{									\
	setinPORT((Pxn), (PORTx));		\
	setpuPORT((Pxn), (PORTx));		\
}


#define PIN_FROM_PORT(PORTx) \
	((_MMIO_BYTE(&(PORTx) - PINPORTOFFSET)))

#define DDR_FROM_PORT(PORTx) \
	((_MMIO_BYTE(&(PORTx) - DDRPORTOFFSET)))


// Let's do the same for Pulled up vs not pulled up



// For an entire port:
#warning "setPORT___'s are untested..."
#define setPORTout(PORTx)		(DDR_FROM_PORT(PORTx) = 0xff)

//NOTE: This does NOT affect the pull-up state (which is determined
// by the value already on PORTx)
#define setPORTin(PORTx)		(DDR_FROM_PORT(PORTx) = 0x00)

#define clrPORTpu(PORTx)		((PORTx) = 0x00)

#define setPORTpu(PORTx) 		((PORTx) = 0xff)

#define setPORTinpu(PORTx) \
{\
	setPORTin(PORTx); \
	setPORTpu(PORTx); \
}

//Equivalent to PINx (from PORTx)
#define PORTin(PORTx) 			(PIN_FROM_PORT(PORTx))


#define setPORToutMasked(PORTx, Mask)	\
	writeMasked(0xff, Mask, DDR_FROM_PORT(PORTx))

#define setPORTinMasked(PORTx, Mask) \
	writeMasked(0x00, Mask, DDR_FROM_PORT(PORTx))

#define setPORTpuMasked(PORTx, Mask) \
	writeMasked(0xff, Mask, PORTx)

#define setPORTinpuMasked(PORTx, Mask) \
{ \
	setPORTinMasked(PORTx, Mask); \
	setPORTpuMasked(PORTx, Mask); \
}







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







/* Notes re: PORT,DDR,PIN register locations:

All Explored MCUs have the same register-order... PINx DDRx PORTx
so offsets are the same for all, and only one is necessary for #defines...
Since PORT makes the most sense when working with a pin on a port
 eventually it'd be nice to switch this all up:

getPin, setPin, clrPin, setPinPU, setPinOut, setPinIn or som'n...

Could maybe do #defines such as PinB7 = PB7, PORTB
so getPin could be as simple as getPin(PinB7)
besides being quicker to type and visually, it'd also relieve some risk of port mixup getPin(PB7,PORTD)

m168
0x0B (0x2B) PORTD	PORTD7 PORTD6 PORTD5 PORTD4 PORTD3 PORTD2 PORTD1 PORTD0 89 
0x0A (0x2A) DDRD	DDD7 DDD6 DDD5 DDD4 DDD3 DDD2 DDD1 DDD0 89 
0x09 (0x29) PIND	PIND7 PIND6 PIND5 PIND4 PIND3 PIND2 PIND1 PIND0 89 
0x08 (0x28) PORTC	Ð PORTC6 PORTC5 PORTC4 PORTC3 PORTC2 PORTC1 PORTC0 88 
0x07 (0x27) DDRC	Ð DDC6 DDC5 DDC4 DDC3 DDC2 DDC1 DDC0 88 
0x06 (0x26) PINC	Ð PINC6 PINC5 PINC4 PINC3 PINC2 PINC1 PINC0 88 
0x05 (0x25) PORTB	PORTB7 PORTB6 PORTB5 PORTB4 PORTB3 PORTB2 PORTB1 PORTB0 88 
0x04 (0x24) DDRB	DDB7 DDB6 DDB5 DDB4 DDB3 DDB2 DDB1 DDB0 88 
0x03 (0x23) PINB	PINB7 PINB6 PINB5 PINB4 PINB3 PINB2 PINB1 PINB0 88 


m8515
$1B ($3B) PORTA		PORTA7 PORTA6 PORTA5 PORTA4 PORTA3 PORTA2 PORTA1 PORTA0 74 
$1A ($3A) DDRA		DDA7 DDA6 DDA5 DDA4 DDA3 DDA2 DDA1 DDA0 74 
$19 ($39) PINA		PINA7 PINA6 PINA5 PINA4 PINA3 PINA2 PINA1 PINA0 74 
$18 ($38) PORTB		PORTB7 PORTB6 PORTB5 PORTB4 PORTB3 PORTB2 PORTB1 PORTB0 74 
$17 ($37) DDRB		DDB7 DDB6 DDB5 DDB4 DDB3 DDB2 DDB1 DDB0 74 
$16 ($36) PINB		PINB7 PINB6 PINB5 PINB4 PINB3 PINB2 PINB1 PINB0 74 
$15 ($35) PORTC		PORTC7 PORTC6 PORTC5 PORTC4 PORTC3 PORTC2 PORTC1 PORTC0 74 
$14 ($34) DDRC		DDC7 DDC6 DDC5 DDC4 DDC3 DDC2 DDC1 DDC0 74 
$13 ($33) PINC		PINC7 PINC6 PINC5 PINC4 PINC3 PINC2 PINC1 PINC0 75 
$12 ($32) PORTD		PORTD7 PORTD6 PORTD5 PORTD4 PORTD3 PORTD2 PORTD1 PORTD0 75 
$11 ($31) DDRD		DDD7 DDD6 DDD5 DDD4 DDD3 DDD2 DDD1 DDD0 75 
$10 ($30) PIND		PIND7 PIND6 PIND5 PIND4 PIND3 PIND2 PIND1 PIND0 75 
--
$07 ($27) PORTE		- - - - - PORTE2 PORTE1 PORTE0 75 
$06 ($26) DDRE		- - - - - DDE2 DDE1 DDE0 75 
$05 ($25) PINE		- - - - - PINE2 PINE1 PINE0 75 

mxx4P
0x0B (0x2B) PORTD	PORTD7 PORTD6 PORTD5 PORTD4 PORTD3 PORTD2 PORTD1 PORTD0 91 
0x0A (0x2A) DDRD	DDD7 DDD6 DDD5 DDD4 DDD3 DDD2 DDD1 DDD0 91 
0x09 (0x29) PIND	PIND7 PIND6 PIND5 PIND4 PIND3 PIND2 PIND1 PIND0 91 
0x08 (0x28) PORTC	PORTC7 PORTC6 PORTC5 PORTC4 PORTC3 PORTC2 PORTC1 PORTC0 91 
0x07 (0x27) DDRC	DDC7 DDC6 DDC5 DDC4 DDC3 DDC2 DDC1 DDC0 91 
0x06 (0x26) PINC	PINC7 PINC6 PINC5 PINC4 PINC3 PINC2 PINC1 PINC0 91 
0x05 (0x25) PORTB	PORTB7 PORTB6 PORTB5 PORTB4 PORTB3 PORTB2 PORTB1 PORTB0 90 
0x04 (0x24) DDRB	DDB7 DDB6 DDB5 DDB4 DDB3 DDB2 DDB1 DDB0 90 
0x03 (0x23) PINB	PINB7 PINB6 PINB5 PINB4 PINB3 PINB2 PINB1 PINB0 90 
0x02 (0x22) PORTA	PORTA7 PORTA6 PORTA5 PORTA4 PORTA3 PORTA2 PORTA1 PORTA0 90 
0x01 (0x21) DDRA	DDA7 DDA6 DDA5 DDA4 DDA3 DDA2 DDA1 DDA0 90 
0x00 (0x20) PINA	PINA7 PINA6 PINA5 PINA4 PINA3 PINA2 PINA1 PINA0 90 

m162
0x1B (0x3B) PORTA	PORTA7 PORTA6 PORTA5 PORTA4 PORTA3 PORTA2 PORTA1 PORTA0 81 
0x1A (0x3A) DDRA	DDA7 DDA6 DDA5 DDA4 DDA3 DDA2 DDA1 DDA0 81 
0x19 (0x39) PINA	PINA7 PINA6 PINA5 PINA4 PINA3 PINA2 PINA1 PINA0 81 
0x18 (0x38) PORTB	PORTB7 PORTB6 PORTB5 PORTB4 PORTB3 PORTB2 PORTB1 PORTB0 81 
0x17 (0x37) DDRB	DDB7 DDB6 DDB5 DDB4 DDB3 DDB2 DDB1 DDB0 81 
0x16 (0x36) PINB	PINB7 PINB6 PINB5 PINB4 PINB3 PINB2 PINB1 PINB0 81 
0x15 (0x35) PORTC	PORTC7 PORTC6 PORTC5 PORTC4 PORTC3 PORTC2 PORTC1 PORTC0 81 
0x14 (0x34) DDRC	DDC7 DDC6 DDC5 DDC4 DDC3 DDC2 DDC1 DDC0 81 
0x13 (0x33) PINC	PINC7 PINC6 PINC5 PINC4 PINC3 PINC2 PINC1 PINC0 82 
0x12 (0x32) PORTD	PORTD7 PORTD6 PORTD5 PORTD4 PORTD3 PORTD2 PORTD1 PORTD0 82 
0x11 (0x31) DDRD	DDD7 DDD6 DDD5 DDD4 DDD3 DDD2 DDD1 DDD0 82 
0x10 (0x30) PIND	PIND7 PIND6 PIND5 PIND4 PIND3 PIND2 PIND1 PIND0 82 
--
0x07 (0x27) PORTE	Ð Ð Ð Ð Ð PORTE2 PORTE1 PORTE0 82 
0x06 (0x26) DDRE	Ð Ð Ð Ð Ð DDE2 DDE1 DDE0 82 
0x05 (0x25) PINE	Ð Ð Ð Ð Ð PINE2 PINE1 PINE0 82 
*/

#endif

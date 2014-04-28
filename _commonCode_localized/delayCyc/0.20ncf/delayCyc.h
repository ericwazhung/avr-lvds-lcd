#ifndef __DELAYCYC_H__
#define __DELAYCYC_H__

#ifndef _STRINGIFY_HEADER_
 #include "../../__std_wrappers/stringify/0.10/stringify.h"
#else
 #include _STRINGIFY_HEADER_
#endif

//delayCyc.h 0.20ncf (AVR)

//0.20ncf-2 changing stringify include to _STRINGIFY_HEADER_

//0.20ncf-1 Discovered in LCDdirectLVDSv67:
//          warning: optimization may eliminate reads and/or writes to
//          register variables [-Wvolatile-register-var]
//          Looking into this...
//          TODO: THIS HAS NOT BEEN TESTED since "volatile register"
//                has been replaced with just "register"
//0.20ncf - LCDdirectLVDS has its own delayCyc which is entirely unrelated
//          But its conditional-inclusion test is identical to this one's
//          (wouldn't it make more sense to change it, than this?)
//          (yes, but now it's delay_cyc.c/h which still makes sense to
//          have a conditional-inclusion test for __DELAY_CYC_H__... and
//          realistically since this is delayCyc, it should be
//          __DELAYCYC_H__)
//0.10ncf-4 - a/o LCDdirectLVDS67, needing delayCyc for synchronizing the
//            two PSCs... upgraded to avr-gcc4.8, test still compiles, but
//            in LCDdirectLVDS it complains that delayCyc_setup is an
//            implicity declaration?!
//            First, making sure this is getting included:
//#error "yep"
//            Nope... because LCDdirectLVDS has the old delayCyc.c/h
//            which has absolutely nothing to do with this delayCyc.

//0.10ncf-3ish - Got it working...
//               Implemented a test
//               Added a shitton of notes
//               Forgot to mention:
//                  delayCycs can not delay less than 2 cycles
//                  AND it's important to remember that each delay is *in
//                  addition to* whatever time it takes to process the
//                  surrounding assembly code (e.g. setting a pin high,
//                  delayCycs 2, then setting that pin low results in the
//                  pin being high for *3* cycles, because it goes high
//                  immediately before the first delay clock, there's a
//                  second delay clock, and then the pin-high cycle takes
//                  another clock before its value is written at the end of
//                  the cycle.
//0.10ncf-1 - Still going at it...
//            So far, have looked into using GCC's extension which allows
//            taking the address of a goto-label. Would be nice, but
//            doesn't play nicely with assembly...
//            The actual delayCycs routine will be in assembly, and further
//            will be embedded *in* a block of surrounding assembly...
//            Thus, wrapping it with C-labels is a no-go.
//            Having separate asm() blocks (e.g. one to set a pin low, one
//            call to delayCycs, and a final to set the pin high) is
//            non-ideal, as optimizing may well move code around such that
//            some C code (assigning variables, etc) may actually occur
//            between two (code-wise) sequential blocks of assembly.
//            Instead of using C labels, switching over to asm labels
//            and a minor block of ASM code, in a C-style macro, to grab a
//            label from assembly and put it in a C-variable.
//            loadZwithLabel()
//            Testing and development is going on in sdramThing3.0-0.7
//0.10ncf - This is the first version in _commonCode, but it's not
//          common-filed (no delayCyc.mk).
//          THIS IS NOT the same as delayCyc.c in LCDdirectLVDS
//          This is an entirely different branch, written entirely
//          separately.


// The basic idea:
//
// This was developed for sdramThing3.0, as a means to write multiple
// memory-locations sequentially with the same data-value...
// The thing is, in order to write multiple memory-locations into the
// SDRAM, we need to hold a pin at a certain level for an exact number of 
// clock cycles.
//
// This can really only be done in assembly.

// There are three basic parts, here...
//
// In sdramThing, quite a bit needs to be done in assembly, in addition to
// this simple delay, so it'd be ideal to try to make this somehow modular.

// In assembly the best resolution I can get using loops is 3 cycles per
// loop, so there's a bit of extra code necessary to get higher resolution.
// As a result, the actual loop-counter (iterator) value does *not* match
// the requested number of cycles to delay, a bit of math is required to
// figure out how to initialize the loop-counter, etc.
// That math could easily be handled in C, as long as we're certain it will
// run *before* the assembly code that eventually calls the loop.
// Further, this puts a lower-limit on the number of clock-cycles that the
// loop will require, even if the number of loops is zero.
// So, to have small-value cycle-counts, 

// So this is a multi-step process with a few branches:
//
// Entry
//  v
//  |
//  +--> Can the requested number of cycles be handled by the loop?
// / \ .
// Y N
// | |
// | +-> Determine how many NOPs are necessary
// | +-> Set the jump-address appropriately
// | |
// +---> Calculate the initial value for the loop-counter
// +---> Set the jump-address to the loop-counter code
// | |
// \ /
//  |
//  +--> Do project-specific stuff in assembly, etc.
//  |      ( E.G. set up the data on one port, then set the WRITE pin low )
//  +--> Jump to the previously-initialized jump-address
//  .      (NOPs or Loop)      
//  .
// / \ .
// N  L
// .   \ .
// .    \ ,
// .   ->+--> Decrement Loop-Counter by number of cycles per loop
// .  /  |      (loopCounter -= 3 )
// . |   +--> Is the loop complete (loopCounter < 0) ?
// . |  / \ .
// . |  N Y
// .  \_/ |
// .      +--> Cycles Remaining == 0 (loopCounter == -3) ?
// .     / \ .
// .    /   \ .
// .   Y     N
// .   |     |
// .   |     +--> Cycles Remaining == 1 (loopCounter == -2) ?
// .    \   / \ .
// .     \ Y   N (Cycles Remaining == 2, loopCounter == -1)
// .      \|   |
// .       \   +--> NOP
// .       |\  |
// .       | \ |
// .        \ \|
// .         \ +--> NOP
// .          \|
// .           +--> Jump to end of NOP list
// .           .
// .Cycles == 8.       
// .\          .
// . \         .
// .\ +--> NOP .
// . \|        .
// .\ +--> NOP .
// . \|        .
// .\ +--> NOP .
// . \|        .
// .\ +--> NOP .
// . \|        .
// .\ +--> NOP .
// . \|        .
// .\ +--> NOP .
// . \|        .
// .\ +--> NOP .
// . \|        .
// .\ +--> NOP .
// . \|        .
// |  +--> NOP .
// | /      .
// |/     .
// |    .
// |  .
// |.
// v
// +--> Do more project-specific stuff in assembly, etc.
// |      ( E.G. set the WRITE pin high)
//


// Probably need to tell it which variable we're using...
// so it knows not to attempt to push/pop it
// and it'd be more intuitive if this had a name more appropriate
// e.g. loadJumpToWithLabel()
// or   loadWithLabel(variable, label)
// even though variable would always have to be in the Z register...
#define loadZwithLabel(label,instanceName) \
	__asm__ __volatile__ \
	( \
	  "ldi r30, lo8(pm(" STRINGIFY(label) ")) ; \n\t" \
	  "ldi r31, hi8(pm(" STRINGIFY(label) ")) ; \n\t" \
	  : \
	  : "z" (PASTE(dc_jumpTo_,instanceName)) \
	)

#define DELAY_MIN_CYC	2
#define LOOP_MIN_CYC		6
//cycles must be >= 2 (DELAY_MIN_CYC)


#if FALSE
//This value doesn't correspond *directly* to the number of cycles to delay
//In general, loops repeat as long as it's positive
// when it goes negative the looping stops
// Its (negative) value, then, represents the number of remaining cycles
// (e.g. the loop itself takes 3 cycles per loop, so there may be 0, 1, or
// 2 cycles remaining. These are represented by loopCount == -3, -2, -1, 
// respectively)


//These two variables should be defined in the calling function:
//int8_t loopCounter;
//void *jumpTo register asm("Z");
// DO NOT mess this one up... asm("Z") is REQUIRED.

//Would be nice to have a function like delayCyc_setup()
//its declaration would be something like:
// But there's a difficulty here:
// In order to use jumpTo, the goto-labels must be within the same function
//SEE THE #define BELOW
// This is all irrelevent a/o loadZwithLabel()

// So we should be able to use this now....
// EXCEPT, that if delayCyc was used in more than one location...
// we'd need different labels... and we'd need STRINGIFY for that...
// so maybe we can't use this still.
//THIS IS A BIT OUTDATED, the #defined macro is more current
void delayCyc_setup(int8_t cycles, int8_t *loopCounter, void *jumpTo)
{
	if (cycles >= DELAY_MIN_CYC + LOOP_MIN_CYC)
	{
		*loopCounter = cycles - DELAY_MIN_CYC - LOOP_MIN_CYC;
		//taking the address of a goto-label is a GCC extension
		//jumpTo = &&loop;
		// also, it only works with C labels, and we need an assembly label
		//Instead, this fills Z with the address at dc_loop
		//(which assigns the value to jumpTo)
		loadZwithLabel(dc_loop);
	}
	else	//ASSUMING cycles >= DELAY_MIN_CYC
	{
		//Use a value that won't be misinterpretted as valid during testing
		loopCounter = (INT8_MIN);
		//jumpTo = &&done - (cycles);
		loadZwithLabel(dc_done);
		jumpTo -= (cycles - DELAY_MIN_CYC);

		//Running from this jumpTo address is essentially the same as 
		//executing the corresponding number of NOPs...
		//The actual instructions themselves serve no purpose, in this case.
	
		//Each instruction executes in a single-cycle as long as:
	  	//	each comparison must fail so that
	  	//	each branch instruction does *not* branch 
		// (thus taking a single instruction-cycle)
		//Should it enter *at* a branch-instruction, 
		// the branch-test must fail
		// Each branch is 'breq' (branch *if* the last instruction which sets
		// the SREG's Z(ero) bit is true)
		// So set the Z-bit FALSE
		// (And don't execute any functions between here and the actual
		// delay-execution that would change the Z-bit!)
		asm("bclr 1	; \n\t");
	}
}
#endif //FALSE (This is implemented in the macro, below)

//This is a huge block of code which declares variables, etc... that will
// be used later.
// So it must be on its own line NOT inside an if{} block, etc.
// This is the code-only from the delayCyc_setup() function, which
// describes the code in better detail.
#if 0
#define delayCyc_setup(cycles) \
	int8_t dc_loopCounter; \
	void *dc_jumpTo register asm("Z"); \
	if ((cycles) >= DELAY_MIN_CYC + LOOP_MIN_CYC) \
	{ \
		dc_loopCounter = (cycles) - DELAY_MIN_CYC - LOOP_MIN_CYC; \
		loadZwithLabel(dc_loop); \
	} \
	else \
	{ \
		dc_loopCounter = (INT8_MIN); \
		loadZwithLabel(dc_done); \
		dc_jumpTo -= (cycles); \
		asm("bclr 1	; \n\t"); \
	}
#endif //0

#define DC_LC_REG	r16	//must be a "d" register for subi
#define DC_LC_REG_Q STRINGIFY(DC_LC_REG)

#define DC_JT_REG r30	//must be the "Z" register, so don't change this
#define DC_JT_REG_Q STRINGIFY(DC_JT_REG)

//instanceName is anything to identify this instance separately from any
//other... e.g. if delayCyc is used in sdram_read() as well as
//sdram_write(), they should have separate instanceNames...
// (instanceName could well be 'sdram_read' and 'sdram_write' 
//  *without* quotes.)
// for dc_jumpTo_... asm("Z") doesn't work, so asm("r30") I guess...

//ERRORS:
//  r30/31 not being pushed?!
//     Some registers are *supposed* to be backed-up by the caller NOT the
//     callee... these are amongst those.
//  where's the -= cycles!?
//		 Fixed... they were being reused, because I didn't inform C that they
//		 were being used in the ASM blocks.

//TODO: better handling for out-of-range values?
//      e.g. < 2 -> 2
//           > (what?) -> max...
//cycles must be int8_t, >=2, and <= 127(?)
// This MUST be called before EACH call to delayCycs
// (no getting away with one setup and then looping the asm block)
// (because loopCounter needs to be set-up... TODO: maybe add another
// variable and use delayCycs_asmInit to reset loopCounter?)
// Not sure it's possible without requiring another *specific* register
// It's pretty seldom it'd be needed, anyhow, right?
// TODO: There can only be *one* delayCyc call per function!
//       since variables are defined which use specific registers.
//       (Z would *always* have to be used, without some extra do-dads
//        possibly handled in delayCycs_asmInit)


//a/o v0.20-1: Not sure why I had 'volatile register'
// but gcc-4.8 complains about it.
// http://stackoverflow.com/questions/18097463/why-doesnt-gcc-handle-volatile-register
//
// I don't think it's necessary...
#define VOLATILE_ISH
//#define VOLATILE_ISH	volatile

#define delayCyc_setup(instanceName, cycles) \
	VOLATILE_ISH register int8_t PASTE(dc_loopCounter_,instanceName) \
																		asm(DC_LC_REG_Q); \
	VOLATILE_ISH register void *PASTE(dc_jumpTo_,instanceName) \
																		asm(DC_JT_REG_Q); \
	if ((int8_t)((cycles)) >= DELAY_MIN_CYC + LOOP_MIN_CYC) \
	{ \
		PASTE(dc_loopCounter_,instanceName) = \
					((int8_t)(cycles)) - DELAY_MIN_CYC - LOOP_MIN_CYC; \
		loadZwithLabel(PASTE(dc_loop_,instanceName),instanceName); \
	} \
	else \
	{ \
		PASTE(dc_loopCounter_,instanceName) = (INT8_MIN); \
		loadZwithLabel(PASTE(dc_done_,instanceName),instanceName); \
		PASTE(dc_jumpTo_,instanceName) -= (((int8_t)(cycles)) \
															- DELAY_MIN_CYC); \
	}
/*#define delayCyc_setup(instanceName, cycles) \
	register int8_t PASTE(dc_loopCounter_,instanceName) asm(DC_LC_REG_Q); \
	register void *PASTE(dc_jumpTo_,instanceName) asm(DC_JT_REG_Q); \
	if ((cycles) >= DELAY_MIN_CYC + LOOP_MIN_CYC) \
	{ \
		PASTE(dc_loopCounter_,instanceName) = \
					(cycles) - DELAY_MIN_CYC - LOOP_MIN_CYC; \
		loadZwithLabel(PASTE(dc_loop_,instanceName)); \
	} \
	else \
	{ \
		PASTE(dc_loopCounter_,instanceName) = (INT8_MIN); \
		loadZwithLabel(PASTE(dc_done_,instanceName)); \
		PASTE(dc_jumpTo_,instanceName) -= (cycles); \
	}
*/

//Interestingly, the bclr 1 is being replaced by clz, smart
// re: using bclr 1 (clz) in delayCyc_setup() (now removed):
// This STILL isn't safe... even if delayCyc_setup() is called
// *immediately* before the asm block containing delayCycs, the optimizer
// could (did) reorganize the delayCyc_setup() code with some from before.
//
// so the asm setup... needs to be set at the beginning of the asm block
// Since subi runs *before* any tests, if necessary, clz is OK regardless
// of whether the loop is executed or not.

//This can go wherever in the ASM block containing delayCycs, but *before*
//delayCycs, and *after* any math that may be done in the block (which
//would overwrite the Z-flag.
// This should be exactly one cycle, if doing cycle-counting in the
// asm-block for offset-values 
// e.g. 
// sdram_write(column): startingCol=(column - numCyclesTilActualWrite); ...
asm
(
 ".macro delayCycs_asmInit instanceName \n\t"
 "dc_asm_init_\\instanceName\\(): \n\t"
 	"clz ; \n\t"
 "dc_asm_init_exit_\\instanceName\\(): \n\t"
 ".endm \n\t"
);






//This will be included in the calling asm block, directly
// so, in C-terms, this is essentially static __inline__ or maybe even a
// preprocessor macro
//__asm__ __volatile__ is a nogo at this (top/global) level
//#error "Wait a minute, something's missing here... no jump to jumpTo???"
//Might be questionable, anyhow, since my asm block seems to be doing some
//math (which would overwrite clz)
// or it's reorganizing some code...
// 
//#define DC_NOPS_ONLY	TRUE
#if(!defined(DC_NOPS_ONLY) || !DC_NOPS_ONLY)
asm
(
".macro delayCycs instanceName \n\t"
 "delayCycs_\\instanceName\\(): \n\t"
"ijmp ; \n\t" 		//Jump as appropriate
							// either to the start of the loop
							// or somewhere near the end (essentially NOPs)
//The loop itself, takes three clock cycles each loop, two when finished
 "dc_loop_\\instanceName\\():	\n\t"
	//	"subi %\[dc_loopCounter\], 3		; \n\t"
	// NOGO, apparently the macro is not inserted word-for-word(?)
	// But we know which register it is, so maybe we can do it manually
	//	"subi %3, 3		; \n\t"
	//	"subi r16, 3		; \n\t"
	"subi " DC_LC_REG_Q ", 3		; \n\t"
	"brge dc_loop_\\instanceName	; \n\t"
//The loop has completed, handle the remaining clock cycles
//Don't really need this, it's just useful to see in the assembly listing
"dc_loopRemainder_\\instanceName\\(): \n\t"
	//	"cpi %[dc_loopCounter], -3		; \n\t"
	//	"cpi r16, -3		; \n\t"
	"cpi " DC_LC_REG_Q ", -3		; \n\t"
	//	"breq dc_done1_%=	; \n\t"
	"breq dc_done1_\\instanceName	; \n\t"
	//	"cpi %[dc_loopCounter], -2		; \n\t"
	//	"cpi r16, -2		; \n\t"
	"cpi " DC_LC_REG_Q ", -2		; \n\t"
	"breq dc_done_\\instanceName	; \n\t"
	"nop				; \n\t"
//This is only used internally...
 "dc_done1_\\instanceName\\():		\n\t"
	"nop				; \n\t"
 "dc_done_\\instanceName\\():	\n\t"
".endm \n\t"
//NoCanDo
//:
//:[dc_loopCounter] "d" (PASTE(dc_loopCounter_,instanceName))
);
#else
asm
(
".macro delayCycs instanceName \n\t"
 "delayCycs_\\instanceName\\(): \n\t"
"ijmp ; \n\t" 		//Jump as appropriate
							// either to the start of the loop
							// or somewhere near the end (essentially NOPs)
//The loop itself, takes three clock cycles each loop, two when finished
 "dc_loop_\\instanceName\\():	\n\t"
	//	"subi %\[dc_loopCounter\], 3		; \n\t"
	// NOGO, apparently the macro is not inserted word-for-word(?)
	// But we know which register it is, so maybe we can do it manually
	//	"subi %3, 3		; \n\t"
	//	"subi r16, 3		; \n\t"
"nop ; \n\t" //	"subi " DC_LC_REG_Q ", 3		; \n\t"
"nop ; \n\t" //	"brge dc_loop_\\instanceName	; \n\t"
//The loop has completed, handle the remaining clock cycles
//Don't really need this, it's just useful to see in the assembly listing
"dc_loopRemainder_\\instanceName\\(): \n\t"
	//	"cpi %[dc_loopCounter], -3		; \n\t"
	//	"cpi r16, -3		; \n\t"
"nop ; \n\t" //	"cpi " DC_LC_REG_Q ", -3		; \n\t"
	//	"breq dc_done1_%=	; \n\t"
"nop ; \n\t" //	"breq dc_done1_\\instanceName	; \n\t"
	//	"cpi %[dc_loopCounter], -2		; \n\t"
	//	"cpi r16, -2		; \n\t"
"nop ; \n\t" //	"cpi " DC_LC_REG_Q ", -2		; \n\t"
"nop ; \n\t" //	"breq dc_done_\\instanceName	; \n\t"
	"nop				; \n\t"
//This is only used internally...
 "dc_done1_\\instanceName\\():		\n\t"
	"nop				; \n\t"
 "dc_done_\\instanceName\\():	\n\t"
".endm \n\t"
//NoCanDo
//:
//:[dc_loopCounter] "d" (PASTE(dc_loopCounter_,instanceName))
);
#endif
//This isn't necessary (or usable) since %... isn't working within a .macro
// HOWEVER, it might be necessary to tell C that the variable is used so it
// doesn't get mangled by C before/after
/*#define DC_LOOPCOUNTER_FOR_ASM(instanceName) \
	[dc_loopCounter] "d" (PASTE(dc_loopCounter_,instanceName))
*/
#define DC_VARIABLES_FOR_ASM(instanceName) \
	[dc_jumpTo] "z" (PASTE(dc_jumpTo_,instanceName)), \
	[dc_loopCounter] "d" (PASTE(dc_loopCounter_,instanceName))


// Basic Usage:
// void callingFunction(int8_t cycles, ...)
// {
// 	 set up variables, do math, etc...
// --> delayCyc_setup(somethingUnique1, cycles);
//
//     __asm__ __volatile__ (
// --->  "delayCycs_asmInit somethingUnique1 ; \n\t"
//       //do stuff
//       "out 0x18, r1 ; \n\t" //set pin active
// --->  "delayCycs somethingUnique1 ; \n\t"
//       "out 0x18, r2 ; \n\t" //set pin inactive
//       //more stuff
//			: ...
//       : ...
// --->    DC_VARIABLES_FOR_ASM(somethingUnique1)
//     )
//    ...
// }
//
// Allegedly somethingUnique1, here, could be __FUNCTION__ (GCC) or
// __func__ (C99), as long as there's only one delayCyc in the function
// Rather, since it's in quotes, it should be:
// ...wait... __FUNCTION__ must already be quoted. :/
// Yeah, NOGO:
// delayCyc_setup(__FUNCTION__, cycles)
// "delayCycs " STRINGIFY(__FUNCTION__) " ; \n\t"
// DC_LOOPCOUNTER_FOR_ASM(__FUNCTION__)
// Also nogo: What if the function is inline? Then we'd have multiple
// definitions 


/*
re: void *jumpTo=&&goto_label;

" You may not use this mechanism to jump to code in a different function.
  If you do that, totally unpredictable things will happen.  The best way
  to avoid this is to store the label address only in automatic variables
  and never pass it as an argument."

  Totally Unpredictable Things,
  like the registers have been set-up for one function, then jumping to
  some random point in another function where its registers may clobber
  and/or be uninitialized... ? Where return may be called but we've only
  actually CALLed the first function, so we're returning not to the first
  function but *from* it, even though the "return" was in the second
  function...?

  These all seem relatively predictable, and can easily be avoided by
  knowing that the other "function" is not being called as a function at 
  all, but a jump-point which will jump back to a known point in the 
  calling function...

  Or is the problem bigger; due to the possibility of labels existing in
  different functions with the same name, (still, we're talking a
  program-memory address... not a *name*)
*/


#endif

//heartbeat 1.21-8
// TODO: REVISIT Switch (see 1.10-1/1.10)
// TODO: Revisit whether dms6sec_t is safe everywhere (especially blink)
//       Change Blink so upper nibble is first?
//       (currently blinks lower first, so 0x23 blinks 3 times, then 2)
//1.21-1 Enabling HEART_REMOVED as TRUE causes the code not to be added to
//       SRC, which is the idea. BUT, it causes it to not be included in
//       make localize... here's a hack to fix that... using COM_HEADERS
//1.21-7 _BITHANDLING_HEADER_ wasn't used!
//1.21-6 adding this TODO: create a halt-function for errors, with blinks?
//			see audioThing-v4
//       What about a timer-interrupt-function for heartUpdates until
//       the system's booted...?
//        (maybe just for debugging... a CFLAG?)

//1.21-5 Input Polling Delay for pull-up...
//1.21-4 HeartBlink only runs once (when No DMS?) (Fixed)
//1.21-3 Shouldn't HFModulation be included in the ifndef? Maybe not.
//#warning "UNTESTED change re: HFModulation versioning"
//       NOGO, reverted, with notes in .mk
//1.21-2 HFModulation version overridable
//1.21-1 remove TCOMDIR in favor of COMDIR (forgot to back up 1.21)
//1.21 updating to latest dmsTimer... 
//1.20-1 Adding __HEART_REMOVED__:
//     In Makefile:
//      HEART_REMOVED = TRUE
//        will not add heartbeat.c to SRC and will add:
//        CFLAGS += -D'__HEART_REMOVED__=TRUE'
//      removes all heart-code quickly for code-size-testing...
//1.20 Adding HEARTPIN and HEARTPINPIN options
//     so compilation doesn't make them variables...
//		 In Makefile:
//			CFLAGS += -D'HEARTPIN_HARDCODED=TRUE'
//			CFLAGS += -D'HEART_PINNUM=PA3'
//			CFLAGS += -D'HEART_PINPORT=PORTA'
//			CFLAGS += -D'HEART_LEDCONNECTION=LED_TIED_HIGH'
//     This is a significant improvement...
//       threePinIDer51i (with 1.12-5) was 81000cyc/5sec, 7896B
//       threePinIDer51ib (with 1.20) is 103000cyc/5sec, 7650B
//       and four bytes freed from .bss
//1.12-5 HEART_DMS=FALSE doesn't seem to clear all references...
//1.12-4 attemping to save some codespace for threePinIDer49i
//       adding _UNUSED conditional compilation...
//1.12-3 wow, dmsTimer.h was hardcoded to v1.00! switching to 1.10
//1.12-2 switching to dmsTimer 1.10 (from 1.00)
//1.12-1 adding wdt_disable at start of heart_init
//1.12 adding error-blinking... I thought I had this before!
//1.10-1 #include "_HFMODULATION_HEADER_" notes... 
//       LED/Input notes re: LED tied to GND added to 1.10:
//       NOT CERTAIN CODE HANDLES ALL CASES PROPERLY
//1.10 LED/Input switching revisited for CS71518/9 (with inverter) 
//           --24Scam16
//   0.99.5 worked with CS71518/9 for PS24-6
//     revision to generalize screwed it up (musta been 1.00-4)
//    "Float" refers to "input mode, pulled-up" unless noted.
//		Without Inverter, LED Tied High (LED_DIRECT_HIGH/LED_TIED_HIGH): 
//						uC Out >---|<|---vvv--->+5V
//			LED On		L		   
//			LED Off		Float   (do not use H in case a switch is connected!)
//			Float		LED will PULL UP the uC input 
//						*The input switch is ONLY MOMENTARY (i.e. nanotracker) otherwise the LED will be stuck ON
//						*The input switch must be tied DIRECTLY LOW (no resistors)
//						*The LED Off state MUST BE FLOAT (NEVER USE HIGH as an OUTPUT)
//          Switch:
//						uC I/O (L/F) >-+-|<|---vvv--->+5V
//                                     |   
//                                     |   _|_
//                                     +--o   o------>GND
//          Status: Has this been coded? Technically, it's the same L/F as the withInverter case...
//		With Inverter (LED_INVERTED):
//						uC Out >---|>o---|<|---vvv--->+5V
//			LED On		H (Float, with internal pull-ups)			
//			LED Off		L
//			Float		Switch (through resistor) determines LED state
//          Switch: (From memory, not verified... check 24Scam16?)
//						uC I/O (L/F(H)) >-+-|>o---|<|---vvv--->+5V
//                                        |   
//                                        |   _|_     v--Resistor not necessary if L/F is used exclusively (it's NOT)
//                                        +--o   o---vvv--->GND
//          Status: Think it works, not verified. Check 24Scam16...
//      Without Inverter, LED Tied Low (LED_TIED_LOW):
//                      uC Out >--vvv---|>|---->GND
//          LED On      H
//          LED Off     L (Float, with internal pull-ups may light dimly)
//          Float       Value will likely be pulled low through LED, even with pull-ups
//                        though value may not be low-enough.
//          Switch: Not possible tied-low... maybe possible tied-high
//					  but again LED may not pull low enough.
//			Compatibility: (L/F) will not work. (LF)/H is necessary
//          Status: Unknown
//                
//  Current Status: (Not using H/L or On/Off for states, due to inversion, etc.)
//          State1: LED_DIRECT_HIGH -> input, pulled-up
//                  LED_INVERTED    -> output, High 
//          State2:
//                  LED_DIRECT_HIGH == LED_INVERTED -> Low, output
//          
//1.00-10 removing obnoxious warning... 
//1.00-9 revising dmsUpdate's functionPointer for clarity/portability...
//		(1.00-8 not archived, see krautPoxn 117 tarball...)
//1.00-8 using dms6sec_t...
//1.00-7 HFModulation .93 (for adjustable divisor)
//1.00-6 adding _HEARTBEAT_HEADER_ to makefile
//1.00-5 bithandling .94
//1.00-4 LED / INPUT switching...
//		Input is only usable when pulled LOW, do not pull it high... 
//			(maybe different if the LED is tied low, but code would probably have to change)
//		This is only useful if the input usage is temporary since it forces the LED state
//			i.e. money-trigger, start-button, configuration-button, etc...
//		This will also work with non-temporary input if an inverter AND resistors are used
//			i.e. configuration DIP switch
//		Without inverter, LED tied high:
//			LED-ON		(DDR = H-out,	PORT = L)
//			LED-OFF/IN	(DDR = L-in,	PORT = H-pull-up)
//				(If IN is tied LOW, the LED will be ON)
//		With inverter, LED tied high, no resistors:
//			LED-OFF		(DDR = H-out,	PORT = L)
//			LED-ON/IN	(DDR = L-in,	PORT = H-pull-up)
//				(If IN is tied LOW, the LED will be OFF)
//		NOTE: this will NOT work with a directly-connected (not buffered/inverted) LED tied low
//1.00-3 adding dmsWait with heartUpdate...
//1.00-2 adding getHeartRate
//1.00-1 adding Brown-Out-Reset indication...
//1.00 adding watchdog timer... dms cleanup/update
//.99.6 latest dmsTimer, with DMS_INCLUDED, etc...
//.99.5 heartSharing (switch to an input and poll)
//.99.4 heartRate added, more DMS define fixes...
//.99.3 HEARTDMS fixes... added __HEARTBEAT_H__ definition
//.99.2-1 added __HEART_DMSNOINIT__
//.99.2 makefile changes...?
//.99.1 makefile added, made compatible with latest dmsTimer, bithandling. 
//      Changed HEART_DMS define, and made it configurable via project's makefile
//.99cf moved to _common from 24scam5
#if (__TODO_WARN__)
#warning ".99cf may actually be older in some ways than 0.97zcf"
#endif
//.99 setClear
//.98 /actual/ variable-based pin..
//.97 heartStepTime variable
//.96 using hfModulation
//.95 Heart Smoothing via adjustable-frequency PWM, completely different algorithm... needs cleanup
//.91b	never so-titled, but updated for uint8_t output from heartUpdate...
//.91 added HEART2X for heart rate doubling
//.90 added heart for heart clearing

#ifndef __HEARTBEAT_H__
#define __HEARTBEAT_H__

#include <avr/io.h>
#include <avr/interrupt.h>	//for sei and cli, making the heartcount update atomic (could do this other ways...?)

#ifndef _BITHANDLING_HEADER_
 #error "Not gonna letcha get away with this... _BITHANDLING_HEADER_ is not defined"
 #include "../../bithandling/0.94/bithandling.h"
#else
 #include _BITHANDLING_HEADER_
#endif
//#include "../../hfModulation/0.92.2/hfModulation.h"

//Use of _HFMODULATION_HEADER_ seems dangerous...
//   HFMODULATION_LIB = $(TCOMDIR)/hfModulation/0.93/hfModulation
//   CFLAGS+= -D'_HFMODULATION_HEADER_ = "$(HFMODULATION_LIB).h"'
//       TCOMDIR = ../../_common WOULD screw it up, IF it were referenced from here...
//       because it's done in CFLAGS, it's referenced from the compiling directory (not from here)
//       (Frankly, it's just luck it worked out this way... I don't think I was paying /that/ close attention while writing this)
//     So then, why did I come to looking into this...?
//       trying to compile an application outside the Trimpin folder... 
//         'make clean' reported errors here, likely due to TCOMDIR = ~/Trimpin/_common
//             In file included from /Users/meh/Trimpin/_common/heartbeat/1.00/heartbeat.c:2:
//             /Users/meh/Trimpin/_common/heartbeat/1.00/heartbeat.h:57:31: 
//             error: ~/Trimpin/_common/hfModulation/0.93/hfModulation.h: No such file or directory
//          due to the use of "~" it seems. (TCOMDIR=/Users/meh/Trimpin/_common works)
//          
#include _HFMODULATION_HEADER_
#include <avr/wdt.h>

//#define _HEART_DMS_

#ifndef _HEART_DMS_
#error "_HEART_DMS_ not defined. Make sure your project makefile has 'CFLAGS +=' everywhere..."
#endif

#if (_HEART_DMS_)
//#include "../../dmsTimer/1.00/dmsTimer.h"
#include _DMSTIMER_HEADER_


//Note that this is NOT calling int heartUpdate() but void heartupdate()
#if defined(dmsWait)
 #undef dmsWait
 #define dmsWait(time)	dmsWaitFn((time), &heartupdate)
#endif

#endif
 
//Number of loops before stepping to the next brightness level... (only if NODMS)
#define HEART_ITERATIONCOUNT	400


//If using dmsTimer, this is the time in dms between each step...
// else this is the number of heartUpdates between each step
extern uint16_t heartStepTime;

//For init: See 1.10 notes...
//These two are the originals... for backwards compatibility (heh, all of a half a version?)
#define LED_DIRECT_HIGH		FALSE	//(Currently an output L or an input, pulled up)
#define LED_INVERTED		TRUE	//(Currently always an output, H/L)

//These make a little more sense
#define LED_TIED_HIGH		LED_DIRECT_HIGH
#define LED_TIED_LOW		LED_INVERTED	//Tied Low works with Inverted's functionality (always an output, H/L)
#define LED_BUFFERED		LED_INVERTED

//Amount of time between each level of brightness in deci-milliseconds
//There are 512 steps per bright/dim cycle...(?)
//7.8125ms = 128 steps in 1 second, whole bright/dim sequence in 4
//No real reason to make this externed... initialized in declaration in heartbeat.c
#define HEARTSTEPTIME	(78*2)	
//#define HEART_MAXVAL 0xff
//#define HEART_MAXBRIGHT	(HEART_MAXVAL*2/3)
//#define HEART_MAXBRIGHT	(0xff)
#define HEART_MAXBRIGHT	(0xff)

/* testing of hfm with divisor
//  heart getNextDesired doesn't work well with different divisors
//  if MAXBRIGHT != maxPower... since power is read from the hfm_t in getNext
#define HEARTSTEPTIME	(78*256)	
#define HEART_MAXBRIGHT	(0x4)
*/
//Is this the number of iterations per 
//#define HEARTCOUNTMAX	((uint32_t)0x20000l) 
//#define HEARTNUMSTEPS	0x80l		//Number of PWM value steps between full-off and full-on


//#define HEARTNUMCOUNTSPERSTEP	(HEARTCOUNTMAX/2/HEARTNUMSTEPS)					
//#define HEARTNUMCOUNTSPERPWMDIVISION	(HEARTNUMCOUNTSPERSTEP/HEARTNUMSTEPS)	

/*
//LATER MAYBE
//heartSet can be set to any of these values from external functions and will be updated in the next heartUpdate()
//Requires fewer instructions from external functions...
#define HEARTDONTSET	0xff
#define HEART2X			0xf0	//beat twice as fast
#define HEARTRESET		0
void heartMode(uint8_t mode);
*/

//#define HEARTFULLOFF	0
//#define HEARTHALFBRIGHT	1
//#define HEARTFULLON		2
//#define HEARTHALFDIM	3
//#define HEARTJERK		0xf0

//heartSet can be set to 0-4 to set the heart to either full
//extern volatile uint8_t	heartSet;


#if (!defined(HEARTPIN_HARDCODED) || !HEARTPIN_HARDCODED)
//e.g. PINA
extern volatile uint8_t* heartPIN;
//extern volatile uint8_t* heartPORT;
//e.g. PA7
extern uint8_t heartBeatPin;

//Takes arguments in the form... (&(DDRD)) (see INIT_HEARTBEAT() below)
//MUST BE CALLED WITHIN THE FIRST 15ms of boot so WDT won't get stuck in a reset-loop!!!
// HEART UPDATE MUST ALSO BE CALLED BEFORE WDT runs out!!!!
#if (!defined(__HEART_REMOVED__) || !__HEART_REMOVED__)
void init_heartBeat(volatile uint8_t* pin, uint8_t pinName, 
							uint8_t ledConnectionType);
#else
#define init_heartbeat(a, b, c) (0)
#endif

//INIT_HEARTBEAT(HEARTPIN,HEARTBEAT);
//MUST BE CALLED WITHIN THE FIRST 15ms of boot 
// so WDT won't get stuck in a reset-loop!!!
#define INIT_HEARTBEAT(pin,pinName,ledConnType) \
	init_heartBeat((&(pin)),(pinName),(ledConnType))

#else
#if (!defined(__HEART_REMOVED__) || !__HEART_REMOVED__)
void init_heartBeat(void);
#else
#define init_heartBeat() (0)
#endif
#endif

#if (!defined(__HEART_REMOVED__) || !__HEART_REMOVED__)
void heartClear(void);
#else
#define heartClear() (0)
#endif

//!!!
// IF this is called from anywhere besides internal to heatbeat.c/h
// WDT indication will NOT function
#if (!defined(__HEART_REMOVED__) || !__HEART_REMOVED__)
void setHeartRate(uint8_t rate);
#else
#define setHeartRate(a) (0)
#endif

#if (!defined(HEART_GETRATE_UNUSED) || !HEART_GETRATE_UNUSED)
//Get the current heart rate (especially useful for keeping error indication...)
#if (!defined(__HEART_REMOVED__) || !__HEART_REMOVED__)
uint8_t getHeartRate(void);
#else
#define getHeartRate() (0)
#endif
#endif

//If this is set to 0, the heart fades as usual
// if non-zero the heart blinks as follows:
//  say value=0xXY, it will blink Y times, then X times, then repeat
// (for error indication)
#if (!defined(__HEART_REMOVED__) || !__HEART_REMOVED__)
void set_heartBlink(uint8_t value);
#else
#define set_heartBlink(a) (0)
#endif

//OLD:Returns TRUE if the LED is active, FALSE if the LED is inactive...
//Returns TRUE when the pin is an input and pulled-up. LED active-level depends on wiring/inverter
#if (!defined(__HEART_REMOVED__) || !__HEART_REMOVED__)
uint8_t heartUpdate(void);
#else
#define heartUpdate() (0)
#endif

#if (!defined(DMS_WAITFN_UNUSED) || !DMS_WAITFN_UNUSED)
//NoReturn for dmsWait...
#if (!defined(__HEART_REMOVED__) || !__HEART_REMOVED__)
void heartupdate(void);
#else
#define heartupdate() (0)
#endif
#endif

//Poll the heartbeat pin an input...
//Returns the actual value of the pin
//!!! This should NOT be called in a typical while... updateEverything loop 
//    otherwise it will constantly interfere with the heartbeat
//  Ideally, this would be called once every... 256? heartUpdates to minimize visual distortion
//	 Maybe during getNextDesired?
//Meh, the actual input poll takes all of... 25ish instructions, the loop itself probably takes significantly longer
//This could be more sophisticated to be tied together with heart being an input...
// Except that testing would be slowed due to FULLBRIGHT...
#if (!defined(HEART_INPUTPOLLING_UNUSED) || !HEART_INPUTPOLLING_UNUSED)
#if (!defined(__HEART_REMOVED__) || !__HEART_REMOVED__)
uint8_t heartPinInputPoll(void);
#else
#define heartPinInputPoll() (0)
#endif
#endif

#if (!defined(__HEART_REMOVED__) || !__HEART_REMOVED__)
uint8_t getNextDesired(void);
uint8_t updateOutput(void);
#else
#define getNextDesired() (0)
#define updateOutput() (0)
#endif

#if defined(MCUSR)
 #define MCUStatReg MCUSR
#elif defined(MCUCSR)
//8515...
 #define MCUStatReg MCUCSR
#endif


#endif

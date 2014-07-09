/* mehPL:
 *    This is Open Source, but NOT GPL. I call it mehPL.
 *    I'm not too fond of long licenses at the top of the file.
 *    Please see the bottom.
 *    Enjoy!
 */


//heartbeat 1.20
#include "heartbeat.h"


//So far this is only used with HEART_DMS to disallow cumulative-error in
//heart-updates... 
// This should also be used with HEART_TCNTER, soon (if not already)

//It shouldn't be necessary to be TRUE in the vast-majority of cases
// Setting this true bumped the test program from 1550 to 1572 bytes...
#define HEART_PRECISE   FALSE

#if(_HEART_TCNTER_)

#include _TCNTER_HEADER_

// Now defined in tcnter.h
// #ifndef TCNTER_MS
//  #warning "TCNTER_MS is completely arbitrary"
//  #define TCNTER_MS 100
// #endif

//HEART_TCNTER_UPDATES_AND_INIT can be CFLAG +='d...
// But if it's not, we're going to default to TRUE
// If your project uses tcnter_update() in the main-while loop, then set
// this FALSE (don't forget tcnter_init() in main)
// e.g. if tcnter is used by multiple things, like polled_uar/t
// (It could be left for heartUpdate() to handle, still... but as it
// stands, HEART_REMOVED would destroy that.)
 #ifndef HEART_TCNTER_UPDATES_AND_INIT
  #define HEART_TCNTER_UPDATES_AND_INIT TRUE
 #endif

//100% = 255, 0% = 0...
//volatile uint8_t  newDesired = 0;

//Could be uint8 for dms and uint16 for nondms
tcnter_t heartStepTime = HEARTSTEPTIME;
#else
uint16_t heartStepTime = HEARTSTEPTIME;
#endif

//volatile uint8_t  heartSet = HEARTDONTSET;

#if (!defined(HEARTPIN_HARDCODED) || !HEARTPIN_HARDCODED)
//#define LED_DIRECT_HIGH
//#define LED_INVERTED
uint8_t ledConnection;

volatile uint8_t*	heartPIN;
uint8_t heartBeatPin;
#endif

//shift the update-period by heartRate to increase the rate
// this is a right-shift for calculation efficiency
// so rates are 0, 1, 2, 3 -> PERIOD, PERIOD/2, PERIOD/4, PERIOD/8...
//uint8_t heartRate = 0;

hfm_t heartModulator;

/*
void heartMode(uint8_t mode)
{
	switch(mode)
	{
		case HEART_2X:
			heartRate = 1;
			break;
		case HEART_4X:
			heartRate 
	
	}
	heartSet = mode;
}
*/

#if((defined(_HEART_DMS_) && _HEART_DMS_))
#warning "Relevant Everywhere: dms6sec_t used in heartbeat, yet a full cycle is 8 seconds, and nevermind blinks! This might need changing!"
#endif


#if((defined(_HEART_DMS_) && _HEART_DMS_) || \
	 (defined(_HEART_TCNTER_) && _HEART_TCNTER_))
	#define HEART_USES_TIMER	TRUE
#else
	#define HEART_USES_TIMER	FALSE
//	#define HEARTSTEPTIME	(HEART_ITERATIONCOUNT)
#endif


void setHeartRate(uint8_t rate)
{
//	heartRate = rate;
	
	if(rate == 0)
		rate = 1;
		
    heartStepTime = (HEARTSTEPTIME/rate);
}

#if (!defined(HEART_GETRATE_UNUSED) || !HEART_GETRATE_UNUSED)
//This function takes about 20 bytes
uint8_t getHeartRate(void)
{
	 return (uint8_t)(HEARTSTEPTIME/heartStepTime);
}
#endif

#if(defined(_HEART_DMS_) && _HEART_DMS_)
 #if (!defined(DMS_WAITFN_UNUSED) || !DMS_WAITFN_UNUSED)
// This only saves 4 bytes...
void heartupdate(void)
{
	heartUpdate();
}
 #endif
#endif

void heartClear(void)
{
	hfm_setPower(&heartModulator, 0);
}


#if (!defined(HEARTPIN_HARDCODED) || !HEARTPIN_HARDCODED)
//THIS USES THE WDT NOW!
// AFTER init_heartBeat, the wdt must be reset often enough!!! this can be accomplished via heartUpdate...
void init_heartBeat(volatile uint8_t* pin, uint8_t pinName, uint8_t ledConnectionType)
{
//	heartDDR = ddr;
	heartPIN = pin;
	heartBeatPin = pinName;
	ledConnection = ledConnectionType;

//	setoutVar(heartBeatPin, heartPIN);
//	setout(PD6, DDRD);

	//Set the port first, so there's not a short 
	//(there may be an instant of floating on the input of an inverter)
	//PORT = L
	clrpinVar(heartBeatPin, heartPIN);
	//DDR = H,out
	setoutVar(heartBeatPin, heartPIN);
#else
void init_heartBeat(void)
{
	clrpinPORT(HEART_PINNUM, HEART_PINPORT);
	setoutPORT(HEART_PINNUM, HEART_PINPORT);
#endif



//	hfm_setPower(&heartModulator, 0);
	hfm_setup(&heartModulator, 0, HEART_MAXBRIGHT);

	setHeartRate(0);
	
#if (_HEART_DMS_)
 #if (defined(_DMS_EXTERNALUPDATE_) && _DMS_EXTERNALUPDATE_)
	//dmsTimer should be initialized elsewhere based on the rate of dmsUpdate calls...
 #else 
	init_dmsTimer();
 #endif

#elif(defined(HEART_TCNTER_UPDATES_AND_INIT) && \
		HEART_TCNTER_UPDATES_AND_INIT)
	tcnter_init();
#endif


#if (!defined(_WDT_DISABLE_) || !_WDT_DISABLE_)
	// IF the reset occurred, the WDTimer is reset to 16ms!
	// IF wdt_reset() isn't called before then, it will get stuck resetting!
	//!!!! This can happen even if the WDT is NEVER ENABLED
	// esp. if the bit is set during a brown-out...
	wdt_reset();
	// So also disable it (and reenable it if we want it)
	wdt_disable();

	//Check if there was a watchdog reset...
	// IF the reset occurred, the WDTimer is reset to 16ms!
	// IF wdt_reset() isn't called before then, it will get stuck resetting!
	//!!!! This can happen even if the WDT is NEVER ENABLED
	// esp. if the bit is set during a brown-out...
	//Could also check ! Power-On, and/or brownout... 
	if(getbit(WDRF, MCUStatReg))
	{
		setHeartRate(32);
/*
		
		//HALT HERE FOR DEBUGGING!!!
		// INDICATE it on the HeartBeat!
#warning "watchdog should NOT hang the system when distributed... this is DEBUGGING ONLY"
		while(1)
		{
			heartUpdate();
		}
*/
	}
	if(!getbit(PORF, MCUStatReg))
	{
		//It seems unusual that a brown-out wouldn't occur on power-up... takes time to charge caps, etc.
		//only check this if PowerOnReset was already cleared from a previous run
		if(getbit(BORF, MCUStatReg))
		{
			setHeartRate(16);
		}
	}
	//Clear the reset flags so we can see next time...
//	MCUSR = 0x0f;
	MCUStatReg = 0;
	
	//It's kinda handy to have the WDT run for a while so we can tell it crashed...
 #if defined(_WDTO_USER_)
	wdt_enable(_WDTO_USER_);
 #else
  #if defined(WDTO_4S)
	wdt_enable(WDTO_4S);
  #else
	wdt_enable(WDTO_1S);
  #endif
 #endif
#else
 #warning "WDT DISABLED"
#endif
}

//Increment or decrement the heart brightness as appropriate
// this is called periodically by heartUpdate...
// This function takes about 32 bytes
// and inlining it saved 6, wee!
static __inline__ uint8_t getNextDesired(void)
{
	//Don't set abs(direction) > 1 or we'll have wraparound errors!
	static int8_t direction = +1;
	int16_t desired;


//Somehow it seems the program gets stuck with the heartbeat at half-brightness...
//So main is still running, but the heart isn't incrementing!
// So do the watchdog reset here...

//!!! Check This with WDT...
//#warning "at what heart_reset() rate will getNextDesired never be called?"
#if (!defined(_WDT_DISABLE_) || !_WDT_DISABLE_)
	//Woot! Saved two bytes!
	wdt_reset();
#endif

	desired = (uint8_t)(heartModulator.power);

	desired += direction;
	
	//Slightly slower not to have these internal to above, but also safer...
	
	if(desired >= HEART_MAXBRIGHT)
	{
		desired = HEART_MAXBRIGHT;
		direction = -1;
	}	

	if(desired <= 0)
	{
		desired = 0;
		direction = +1;
	}
	
	return (uint8_t)desired;
}

#if (!defined(HEART_BLINK_UNUSED) || !HEART_BLINK_UNUSED)
//Used for error-indication...
// when 0, the heart fades as usual
// when non-zero the heart blinks as follows:
//  say heartBlink is 0xXY, it will blink Y times first, then X times
//  then repeat...
// (actually, I think that's backwards, a remnant of times past... I think
// it's now X times first, then Y...)
uint8_t heartBlink = 0;
//#define TOGGLETIME	(250*DMS_MS)

void set_heartBlink(uint8_t count)
{
	heartBlink = count;
}

//Called by heartUpdate... used internally, only
#if (_HEART_DMS_)
 #define heart_isItTime(a,b,c)	dmsIsItTime6secV2((a),(b),(c))
 #define TOGGLETIME   (250*DMS_MS)
//This function takes about 288 bytes
 #warning "This may get cut, if the blink-rate is longer than 6sec!"
static __inline__
uint8_t blinkHeart(void)
		__attribute__((__always_inline__));

uint8_t blinkHeart(void) 
{
	static dms6sec_t lastToggleTime = 0;

#elif (defined(_HEART_TCNTER_) && _HEART_TCNTER_)
 #define heart_isItTime(a,b,c)	tcnter_isItTimeV2((a),(b),(c))
 #define TOGGLETIME (250*TCNTER_MS)

static __inline__
uint8_t blinkHeart(void)
		__attribute__((__always_inline__));

uint8_t blinkHeart(void)
{
	static myTcnter_t lastToggleTime = 0;
#else
 #define TOGGLETIME   (2500)
	
uint8_t blinkHeart(uint32_t currentTime)
{
	static uint32_t lastToggleTime = 0;
#endif

	static uint8_t toggleCount = 0;

	//This is not THE high nibble
	// This is True or False depending on 
	//  whether thisNibble is the high nibble
	static uint8_t highNibble = 0;
	

	uint8_t thisNibble = heartBlink;

	//If both nibbles are filled, process them as specified
	if((heartBlink & 0xf0) && (heartBlink & 0x0f))
	{	//Fall through and toggle...
	}
	//Otherwise just process the one that's filled
	// to avoid long delays and hopefully make it easier to determine which
	// nibble is filled...
	else if(heartBlink & 0x0f)
		highNibble = 0;
	else if(heartBlink & 0xf0)
		highNibble = 1;

	//Fill the appropriate nibble for blinking...
	if(highNibble)
		thisNibble = (thisNibble & 0xf0) >> 4;
	else
		thisNibble = (thisNibble & 0x0f);

#if(HEART_USES_TIMER)
	//Blink the proper number of times
	if((toggleCount>>1) < thisNibble)
	{
		if(heart_isItTime(&lastToggleTime, TOGGLETIME, HEART_PRECISE))
			toggleCount++;
	}
	else if(heart_isItTime(&lastToggleTime, 
						((((uint32_t)TOGGLETIME) * 8)<<(highNibble)),
						HEART_PRECISE))
	{
		toggleCount = 0;
		//Only valid if both nibbles are filled (see above)
		highNibble = !highNibble;
	}
#else
	//Blink the proper number of times
	if((toggleCount>>1) < thisNibble)
	{
		if(currentTime - lastToggleTime > TOGGLETIME)
		{
			lastToggleTime = currentTime;
			toggleCount++;
		}
	}
	//Wait for a while
//Has been working for quite some time, so removing this warning...
//#warning "There may be some integer promotion issues here..."
	else if(currentTime - lastToggleTime > 
						(((uint32_t)(TOGGLETIME * 8))<<(highNibble)))
	{
		toggleCount = 0;
		lastToggleTime = currentTime;
		//Only valid if both nibbles are filled (see above)
		highNibble = !highNibble;
	}
#endif
	//(actually, the new value, at this point... but will be the last soon)
	return !getbit(0, toggleCount);
}
#endif

//This function takes about 186 bytes
// Really?! Or is hfm not compiled if it's not used?
uint8_t heartUpdate(void)
{	
//	static uint32_t lastTime = 0;
#if (	HEART_USES_TIMER )
 #if (_HEART_DMS_)
	static dms6sec_t lastTime = 0;
 #elif ( _HEART_TCNTER_ )
	static myTcnter_t lastTime = 0;

  #if (HEART_TCNTER_UPDATES_AND_INIT)
	tcnter_update();
  #endif
 #else
  #error "NYI"
 #endif
//	dms6sec_t currentTime;
	
//	currentTime = dmsGetTime();

	if(heart_isItTime(&lastTime, heartStepTime, HEART_PRECISE))
		hfm_setPower(&heartModulator, getNextDesired());

	//HEARTSTEPTIME isn't 100% accurate, especially if this isn't accessed often enough...
#else
	static uint32_t lastTime = 0;
	
	static uint32_t currentTime = 0;
	currentTime++;

//	if(currentTime - lastTime > (HEART_ITERATIONCOUNT>>heartRate))
	//heartStepTime is number of updates, no corelation with actual seconds..
//#warning "heartStepTime without dms is untested"

	if((currentTime - lastTime) > heartStepTime)
	{
		lastTime = currentTime;

		hfm_setPower(&heartModulator, getNextDesired());
	}
#endif

//		Without inverter, LED tied high:
//			LED-ON		(DDR = H-out,	PORT = L)
//			LED-OFF/IN	(DDR = L-in,	PORT = H-pull-up)
//				(If IN is tied LOW, the LED will be ON)
//		With inverter, LED tied high, no resistors:
//			LED-OFF		(DDR = H-out,	PORT = L)
//			LED-ON/IN	(DDR = L-in,	PORT = H-pull-up)
//				(If IN is tied LOW, the LED will be OFF)
	//determine whether the LED should be on or off based on the HFM
	// return TRUE if the pin is available as an input
	uint8_t ledVal;

#if (!defined(HEART_BLINK_UNUSED) || !HEART_BLINK_UNUSED)
	if(heartBlink)
	{
 #if (HEART_USES_TIMER)
		ledVal = blinkHeart();
 #else
		ledVal = blinkHeart(currentTime);
 #endif
		//ledVal = blinkHeart((dms6sec_t)currentTime);
	}
	else
#endif
		ledVal = hfm_nextOutput(&heartModulator);

	if(ledVal)
	{
#if (!defined(HEARTPIN_HARDCODED) || !HEARTPIN_HARDCODED)
		if(ledConnection == LED_DIRECT_HIGH)
		{
			//Set the direction first, so there's not a short short with the input switch
			//DDR = L,in
			setinVar(heartBeatPin, heartPIN);
			//PORT = H,pull-up
			setpuVar(heartBeatPin, heartPIN);
		}
		else if(ledConnection == LED_INVERTED)
		{
			//Shouldn't be necessary, but might as well...
			setoutVar(heartBeatPin, heartPIN);
			setpinVar(heartBeatPin, heartPIN);
		}
#else
 #if (HEART_LEDCONNECTION == LED_DIRECT_HIGH)
		setinPORT(HEART_PINNUM, HEART_PINPORT);
		setpuPORT(HEART_PINNUM, HEART_PINPORT);
 #elif (HEART_LEDCONNECTION == LED_INVERTED)
		setoutPORT(HEART_PINNUM, HEART_PINPORT);
		setpinPORT(HEART_PINNUM, HEART_PINPORT);
 #else
	#error "Not Handled..."
 #endif
#endif
		return TRUE;
	}
	else
	{
#if (!defined(HEARTPIN_HARDCODED) || !HEARTPIN_HARDCODED)
		//This should be the same for either case...
		
		//Set the port first, so there's not a short 
		//(there may be an instant of floating on the input of an inverter)
		//PORT = L
		clrpinVar(heartBeatPin, heartPIN);
		//DDR = H,out
		setoutVar(heartBeatPin, heartPIN);
#else
		clrpinPORT(HEART_PINNUM, HEART_PINPORT);
		setoutPORT(HEART_PINNUM, HEART_PINPORT);
#endif
		return FALSE;
	}
}

#if (!defined(HEART_INPUTPOLLING_UNUSED) || !HEART_INPUTPOLLING_UNUSED)
// This value's arbitrary... works with tabletBluetoother 3...
// This function takes about 20 bytes
// HEART_PULLUP_DELAY was originally 3, but doesn't seem to be enough for
// the pwm161 running with CLKDIV8 disabled.
#ifndef HEART_PULLUP_DELAY
#define HEART_PULLUP_DELAY 255
#warning "HEART_PULLUP_DELAY is unset, defaulting to a large value"
#warning " You can add CFLAGS += -D'HEART_PULLUP_DELAY=<value>' to your makefile"
#endif

uint8_t heartPinInputPoll(void)
{
	uint8_t pinState;
	
#if (!defined(HEARTPIN_HARDCODED) || !HEARTPIN_HARDCODED)
	//Make the pin an input, pulled-up
	//OLD:Set the pullup first so it doesn't take so long to raise...
	//Set input first so we don't short the output (PORT=H,pu) to ground
	setinVar(heartBeatPin, heartPIN);
	//PORT = H, pull-up
	setpuVar(heartBeatPin, heartPIN);

	//Insert a delay to allow the pull-up to rise...
	uint8_t delay; 
	for(delay = 0; delay < HEART_PULLUP_DELAY; delay++)
	{  
	   //Necessary or this'll be optimised-out
	   asm("nop;");
	}

	pinState = getpinVar(heartBeatPin, heartPIN);
#else
	setinPORT(HEART_PINNUM, HEART_PINPORT);
	setpuPORT(HEART_PINNUM, HEART_PINPORT);

	//Insert a delay to allow the pull-up to rise...
	uint8_t delay;
	for(delay = 0; delay < HEART_PULLUP_DELAY; delay++)
	{
		//Necessary or this'll be optimised-out
		asm("nop;");
	}

	pinState = getpinPORT(HEART_PINNUM, HEART_PINPORT);
#endif

	//Leave the pinstate to be updated with next heartUpdate
	// to assure valid/safe values of PORT and DDR...
	//Return the pin to the heartbeat
//	setoutVar(heartBeatPin, heartPIN);

	return pinState;
}
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
 * /Users/meh/_avrProjects/LCDdirectLVDS/90-reGitting/_commonCode_localized/heartbeat/1.50/heartbeat.c
 *
 *    (Wow, that's a lot longer than I'd hoped).
 *
 *    Enjoy!
 */

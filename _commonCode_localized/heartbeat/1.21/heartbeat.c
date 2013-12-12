//heartbeat 1.20
#include "heartbeat.h"


//100% = 255, 0% = 0...
//volatile uint8_t  newDesired = 0;

//Could be uint8 for dms and uint16 for nondms
uint16_t heartStepTime = HEARTSTEPTIME;


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

void setHeartRate(uint8_t rate)
{
//	heartRate = rate;
	
	if(rate == 0)
		rate = 1;
		
	#if (_HEART_DMS_)

#warning "Relevant Everywhere: dms6sec_t used in heartbeat, yet a full cycle is 8 seconds, and nevermind blinks! This might need changing!"
    heartStepTime = (HEARTSTEPTIME/rate);
	#else
	 heartStepTime = (HEART_ITERATIONCOUNT/rate);
	#endif
}

#if (!defined(HEART_GETRATE_UNUSED) || !HEART_GETRATE_UNUSED)
uint8_t getHeartRate(void)
{
	#if (_HEART_DMS_)
	 return HEARTSTEPTIME/heartStepTime;
	#else
	 return HEART_ITERATIONCOUNT/heartStepTime;
	#endif
}
#endif

#if (!defined(DMS_WAITFN_UNUSED) || !DMS_WAITFN_UNUSED)
void heartupdate(void)
{
	heartUpdate();
}
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

	// IF the reset occurred, the WDTimer is reset to 16ms!
	// IF wdt_reset() isn't called before then, it will get stuck resetting!
	//!!!! This can happen even if the WDT is NEVER ENABLED
	// esp. if the bit is set during a brown-out...
	wdt_reset();
	// So also disable it (and reenable it if we want it)
	wdt_disable();

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
	wdt_reset();
	wdt_disable();
	clrpinPORT(HEART_PINNUM, HEART_PINPORT);
	setoutPORT(HEART_PINNUM, HEART_PINPORT);
#endif



//	hfm_setPower(&heartModulator, 0);
	hfm_setup(&heartModulator, 0, HEART_MAXBRIGHT);

	setHeartRate(0);
	
#if (_HEART_DMS_)
 #if (_DMS_EXTERNALUPDATE_)
	//dmsTimer should be initialized elsewhere based on the rate of dmsUpdate calls...
 #else 
	init_dmsTimer();
 #endif

#endif

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
#if !defined(_WDT_DISABLE_)
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
uint8_t getNextDesired(void)
{
	//Don't set abs(direction) > 1 or we'll have wraparound errors!
	static int8_t direction = +1;
	int16_t desired;


//Somehow it seems the program gets stuck with the heartbeat at half-brightness...
//So main is still running, but the heart isn't incrementing!
// So do the watchdog reset here...

//!!! Check This with WDT...
//#warning "at what heart_reset() rate will getNextDesired never be called?"
	wdt_reset();

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

//Used for error-indication...
// when 0, the heart fades as usual
// when non-zero the heart blinks as follows:
//  say heartBlink is 0xXY, it will blink Y times first, then X times
//  then repeat...
uint8_t heartBlink = 0;
//#define TOGGLETIME	(250*DMS_MS)

void set_heartBlink(uint8_t count)
{
	heartBlink = count;
}

//Called by heartUpdate... used internally, only
#if (_HEART_DMS_)
#define TOGGLETIME   (250*DMS_MS)

#warning "This may get cut, if the blink-rate is longer than 6sec!"
uint8_t blinkHeart(dms6sec_t currentTime)
{
	static dms6sec_t lastToggleTime = 0;
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
#warning "There may be some integer promotion issues here..."
	else if(currentTime - lastToggleTime > 
						(((uint32_t)(TOGGLETIME * 8))<<(highNibble)))
	{
		toggleCount = 0;
		lastToggleTime = currentTime;
		//Only valid if both nibbles are filled (see above)
		highNibble = !highNibble;
	}

	//(actually, the new value, at this point... but will be the last soon)
	return !getbit(0, toggleCount);
}


uint8_t heartUpdate(void)
{	
//	static uint32_t lastTime = 0;
	
#if (_HEART_DMS_)
	static dms6sec_t lastTime = 0;
	
//	uint32_t currentTime;
	dms6sec_t currentTime;
	
	currentTime = dmsGetTime();
//	currentTime = dmsGet6sec();
	
	//HEARTSTEPTIME isn't 100% accurate, especially if this isn't accessed often enough...
	//heartStepTime is in dms...
//	if((currentTime - lastTime) > ((dms6sec_t)heartStepTime))
//	{
#else
	static uint32_t lastTime = 0;
	
	static uint32_t currentTime = 0;
	currentTime++;

//	if(currentTime - lastTime > (HEART_ITERATIONCOUNT>>heartRate))
	//heartStepTime is number of updates, no corelation with actual seconds..
//#warning "heartStepTime without dms is untested"
#endif

	if((currentTime - lastTime) > heartStepTime)
	{
		lastTime = currentTime;

		hfm_setPower(&heartModulator, getNextDesired());
	}


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

	if(heartBlink)
		ledVal = blinkHeart(currentTime);
		//ledVal = blinkHeart((dms6sec_t)currentTime);
	else
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
#define HEART_PULLUP_DELAY 3
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



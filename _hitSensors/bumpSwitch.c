/* mehPL:
 *    This is Open Source, but NOT GPL. I call it mehPL.
 *    I'm not too fond of long licenses at the top of the file.
 *    Please see the bottom.
 *    Enjoy!
 */






//a/o v82:

//The new idea for a bump-switch/hit-detector is simply a metal ball in a
//cage of pins...

// In its resting-state the ball will make contact between two pins
// when bumped sufficiently it will briefly break contact as the ball rolls
// around to find a new resting-position between two contacts.

// This has some inherent difficulties:
// *The metal ball may become corroded over time and lose conductiveness...
// * (dirt?)
// *There may be some physical orientations of the "sensor" that make no 
//  contact or require a much harder hit than desired...
// *BOUNCE!!!

// so far, this is only used with _interfaces/6bitParallel.c
// Since it's placed on the RGB port, we need to make sure writeColor
// doesn't interfere with the pull-up
#define BUMP_SENSOR_PORT_ON_RGB_PORT TRUE

#define BUMP_SENSOR_PIN PA7
#define BUMP_SENSOR_PORT PORTA
#define BUMP_SENSOR_PCINT	PCINT7
#define BUMP_SENSOR_PCIE	PCIE1
#define BUMP_SENSOR_PCMSK	PCMSK0
#define BUMP_SENSOR_NOTMY_PCMSK	PCMSK1

//TODO:
// CHECK: Is interrupt being missed due to most time spent in
// non-interruptable hsync interrupt?



#include "fb_question.h"

void hitSensor_init(void)
{
	
	//Enable the pin as an input, pulled-up
	setinpuPORT(BUMP_SENSOR_PIN, BUMP_SENSOR_PORT);

	//The default PCMSK value is NON-ZERO!
	//Since the PCINT_vect is (so far) exclusively used for the bump-sensor
	// disable all the other PCINTs
	//Enable the corresponding Pin-Change pin...
	//setbit(BUMP_SENSOR_PCINT, BUMP_SENSOR_PCMSK);
	BUMP_SENSOR_PCMSK = (1<<BUMP_SENSOR_PCINT);

	//The other PCMSK register also has defaults with certain pins enabled
	// and stupidly (?) the PCMSK# doesn't correspond to the PCIE#
	BUMP_SENSOR_NOTMY_PCMSK = 0;

	//Clear the Pin-Change interrupt (probably not necessary)
	setbit(PCIF, GIFR);

	//Enable the Pin-Change interrupt
	// (They are *all* disabled by default)
	setbit(BUMP_SENSOR_PCIE, GIMSK);

}


SIGNAL(PCINT_vect)
{
	fbQuestion_hitDetected();	

	//Not certain this is the best way to handle this...
	//BOUNCE is INHERENT
	//So clear the interrupt again, even though it was cleared upon entry of
	//the interrupt-vector
	// (Will this prevent reentry to this interrupt if the bounce occurred
	// before it exitted and sei()ed at the end?
	setbit(PCIF, GIFR);
}

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
 * /Users/meh/_avrProjects/LCDdirectLVDS/93-checkingProcessAgain/_hitSensors/bumpSwitch.c
 *
 *    (Wow, that's a lot longer than I'd hoped).
 *
 *    Enjoy!
 */

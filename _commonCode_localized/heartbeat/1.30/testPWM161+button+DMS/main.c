#include _HEARTBEAT_HEADER_

void main(void)
{

	init_heartBeat();

	uint8_t heartBlink = 0;
	uint8_t buttonPressed = FALSE;

//	set_heartBlink(3);

	uint8_t loopCount = 0;

	while(1)
	{
		heartUpdate();

		//Intentionally allowed to wraparound from 255->0...
		loopCount++;

		if(loopCount == 0)
		{
			//Read the heartbeat's pin-value
			// doing-so briefly turns the pin into an input...
			// If, e.g. a momentary-pushbutton pulls the pin low,
			// heartPinInputPoll() will return 0
			// Otherwise, it's pulled high by internal pull-up resistor
		
			//As implemented here:
			//The pin is only polled once every 256 loops, so-as not to
			//interfere with the LED's brightness when the button is not
			//pressed. 
			//(Of course, pressing the button causes the LED to light-up at
			//full-brightness until it's released, since it's on the same pin)
			if(heartPinInputPoll())
			{
				//Has the button just been released?
				if(buttonPressed)
				{
					// Pressing the button once will cause it to blink once,
					// twice after the second press
					// and so-on, until 4, when it should go back to fading.
					heartBlink++;
					heartBlink %= 4;
	
					set_heartBlink(heartBlink);
				}
				
				buttonPressed = FALSE;
			}
			else
				buttonPressed = TRUE;
		}
	}
}


#include <stdio.h>
#include <stdint.h>
#include <math.h>

#define PI		(double)(3.14159265358979)
#define QUADRANT	(PI/2)
// SINEMAX is the maximum, thus SINEDIST is 2*SINEMAX
#define SINEMAX		(INT16_MAX)

/* Sine(theta) will be the same as sin(theta)=[-1,1]
 *  with a magnitude of SINEMAX... Sine(theta) = [-SINEMAX, SINEMAX]
 *
 * Worst-case-scenario: distance=xmax-xmin is INT32_MAX (assumed per motorpoxn)
 * The formula for determining position (assuming x is 0-centered)
 *   determined by (0-centered means xmax = -xmin, and distance = 2*xmax)
 *   and   Sine(theta)/(SINEMAX)=1
 *   thus 
 *  Sine(theta)/SINEMAX * xmax gives the new coordinates
 * without using floats this is best handled by:
 *  Sine(theta) * xmax / SINEMAX
 *  BUT:
 *  SINEMAX * (INT32_MAX/2) requires an int64!
 * So... either use an int64 or use int16's and scale up
 * don't forget that the maximum amplitude should give xmax...
 * amplitude * Sine(theta) * xmax / AMPLITUDEMAX / SINEMAX
 * could do steps... with int64s as a first test...
 *  amplitude * Sine(theta) / AMPLITUDEMAX <= Sine(theta)
 */

int main(void)
{
	int32_t sine;
	int32_t position;
	double rads;
	double numSteps;
	double stepSize;
	int32_t value;
	int32_t steps;
	int32_t i;

	numSteps = 128;
	stepSize = QUADRANT / numSteps;

	//The array will be 129 values
	// covering sin(0) = 0 and sin(PI/2) = 1
	//  these values overlap on the axes (between quadrants)
	//  so choose one quadrant if it's on an axis, which gives 512 values...
	for(steps = 0; steps <= numSteps; steps++)
	{
		value = (int32_t)(lround((double)SINEMAX * (double)sin(rads)));	
		printf("%d, ",value);
//		for(i=0; i<value; i+=85899344)
//			printf("*");
		if(steps % 5 == 0)
			printf("\\\n");
		//printf("\n");

		rads += stepSize;
	}

}


//Should be in sineTable.h anyhow.
//#include <avr/pgmspace.h>
#include "sineTable.h"

#if (_SINE_TYPE_ == 8)
int8_t sineTable[129] PROGMEM = {
	0, \
	2, 3, 5, 6, 8, \
	9, 11, 12, 14, 16, \
	17, 19, 20, 22, 23, \
	25, 26, 28, 29, 31, \
	32, 34, 35, 37, 38, \
	40, 41, 43, 44, 46, \
	47, 49, 50, 51, 53, \
	54, 56, 57, 58, 60, \
	61, 63, 64, 65, 67, \
	68, 69, 71, 72, 73, \
	74, 76, 77, 78, 79, \
	81, 82, 83, 84, 85, \
	86, 88, 89, 90, 91, \
	92, 93, 94, 95, 96, \
	97, 98, 99, 100, 101, \
	102, 103, 104, 105, 106, \
	106, 107, 108, 109, 110, \
	111, 111, 112, 113, 113, \
	114, 115, 115, 116, 117, \
	117, 118, 118, 119, 120, \
	120, 121, 121, 122, 122, \
	122, 123, 123, 124, 124, \
	124, 125, 125, 125, 125, \
	126, 126, 126, 126, 126, \
	127, 127, 127, 127, 127, \
	127, 127, 127
};

#define pgm_read_sine8(variable) \
     ((int8_t)pgm_read_byte((int8_t *)(&(variable))))


#define SINE_DISABLE_SINERAW	TRUE
#define SINE_RAW8	TRUE

#elif (_SINE_TYPE_ == 16)

// WHOOPS! ... actually the nearly exactly 256byte data section increase
//   was due to double in sineScaled... (?)
//   (then why is the .data section so small as to not include this table?)
// prog_int16_t sineTable[] = { 
 theta_t sineTable[129] PROGMEM = {
	 0, 402, 804, 1206, 1608, 2009, \
	 2410, 2811, 3212, 3612, 4011, \
	 4410, 4808, 5205, 5602, 5998, \
	 6393, 6786, 7179, 7571, 7962, \
	 8351, 8739, 9126, 9512, 9896, \
	 10278, 10659, 11039, 11417, 11793, \
	 12167, 12539, 12910, 13279, 13645, \
	 14010, 14372, 14732, 15090, 15446, \
	 15800, 16151, 16499, 16846, 17189, \
	 17530, 17869, 18204, 18537, 18868, \
	 19195, 19519, 19841, 20159, 20475, \
	 20787, 21096, 21403, 21705, 22005, \
	 22301, 22594, 22884, 23170, 23452, \
	 23731, 24007, 24279, 24547, 24811, \
	 25072, 25329, 25582, 25832, 26077, \
	 26319, 26556, 26790, 27019, 27245, \
	 27466, 27683, 27896, 28105, 28310, \
	 28510, 28706, 28898, 29085, 29268, \
	 29447, 29621, 29791, 29956, 30117, \
	 30273, 30424, 30571, 30714, 30852, \
	 30985, 31113, 31237, 31356, 31470, \
	 31580, 31685, 31785, 31880, 31971, \
	 32057, 32137, 32213, 32285, 32351, \
	 32412, 32469, 32521, 32567, 32609, \
	 32646, 32678, 32705, 32728, 32745, \
	 32757, 32765, 32767};

/* NOT ROUNDED:
	0, 402, 804, 1206, 1607, 2009, \
	2410, 2811, 3211, 3611, 4011, \
	4409, 4807, 5205, 5601, 5997, \
	6392, 6786, 7179, 7571, 7961, \
	8351, 8739, 9126, 9511, 9895, \
	10278, 10659, 11038, 11416, 11792, \
	12166, 12539, 12909, 13278, 13645, \
	14009, 14372, 14732, 15090, 15446, \
	15799, 16150, 16499, 16845, 17189, \
	17530, 17868, 18204, 18537, 18867, \
	19194, 19519, 19840, 20159, 20474, \
	20787, 21096, 21402, 21705, 22004, \
	22301, 22594, 22883, 23169, 23452, \
	23731, 24006, 24278, 24546, 24811, \
	25072, 25329, 25582, 25831, 26077, \
	26318, 26556, 26789, 27019, 27244, \
	27466, 27683, 27896, 28105, 28309, \
	28510, 28706, 28897, 29085, 29268, \
	29446, 29621, 29790, 29955, 30116, \
	30272, 30424, 30571, 30713, 30851, \
	30984, 31113, 31236, 31356, 31470, \
	31580, 31684, 31785, 31880, 31970, \
	32056, 32137, 32213, 32284, 32350, \
	32412, 32468, 32520, 32567, 32609, \
	32646, 32678, 32705, 32727, 32744, \
	32757, 32764, 32767};
*/
 #define pgm_read_sine(variable)	pgm_read_word(&(variable))

//Read the upper byte...
 #define pgm_read_sine8(variable) \
		((int8_t)pgm_read_byte((int8_t *)(&(variable))+1))

#elif (_SINE_TYPE_ == 32)

 //Should be 129 values... addresses 0-128
// prog_int32_t sineTable[] = { 
 int32_t sineTable[] PROGMEM = {
	0, 26352927, 52701886, 79042909, 105372028, 131685278, \
	157978697, 184248325, 210490205, 236700387, 262874923, \
	289009870, 315101294, 341145265, 367137860, 393075166, \
	418953276, 444768293, 470516330, 496193509, 521795962, \
	547319836, 572761285, 598116478, 623381597, 648552837, \
	673626407, 698598532, 723465451, 748223418, 772868705, \
	797397601, 821806412, 846091463, 870249094, 894275670, \
	918167571, 941921199, 965532977, 988999350, 1012316783, \
	1035481765, 1058490807, 1081340444, 1104027236, 1126547764, \
	1148898639, 1171076494, 1193077990, 1214899812, 1236538674, \
	1257991319, 1279254514, 1300325059, 1321199779, 1341875532, \
	1362349203, 1382617709, 1402677998, 1422527049, 1442161873, \
	1461579512, 1480777043, 1499751575, 1518500249, 1537020242, \
	1555308767, 1573363067, 1591180424, 1608758156, 1626093615, \
	1643184190, 1660027307, 1676620430, 1692961061, 1709046738, \
	1724875039, 1740443579, 1755750016, 1770792043, 1785567395, \
	1800073847, 1814309215, 1828271354, 1841958163, 1855367579, \
	1868497584, 1881346200, 1893911493, 1906191569, 1918184579, \
	1929888719, 1941302224, 1952423376, 1963250500, 1973781966, \
	1984016187, 1993951623, 2003586778, 2012920199, 2021950482, \
	2030676267, 2039096240, 2047209132, 2055013722, 2062508834, \
	2069693340, 2076566158, 2083126253, 2089372636, 2095304368, \
	2100920555, 2106220350, 2111202957, 2115867624, 2120213650, \
	2124240379, 2127947205, 2131333570, 2134398964, 2137142926, \
	2139565041, 2141664947, 2143442325, 2144896908, 2146028478, \
	2146836865, 2147321945, 2147483647};

 #define pgm_read_sine(variable)	pgm_read_dword(&(variable))

#else
 #error "SINE_TYPE not defined or not handled"
#endif

#if (!defined(SINE_TABLE_ONLY) || !SINE_TABLE_ONLY)

//Takes theta of any value in theta_t's range, and returns a theta
// that fits in the first quadrant (for reading from the table)
// negative values returned here are to be absoluted, first
// but they indicate that the resulting sine value is to be negated
theta_t quadrantizeTheta(theta_t theta)
{

	theta %= SINE_2PI;

	if(theta < 0)
		theta = SINE_2PI + theta;

//For more accurate realtimeishness, it might be worthwhile to reverse this order...
	
	//in the first quadrant, just take the sine value from the table
	if(theta < SINE_SIZE-1) //128)
	{
		//theta=theta;
	}	
	//in the second quadrant, mirror theta and subtract the offset
	else if(theta < SINE_SIZE2) //256)
	{
		theta=SINE_SIZE2-theta;
		//return pgm_read_sine(sineTable[SINE_SIZE2-theta]); //256-theta]);
	}
	//in the third quadrant, mirror the value and subtract the offset
	else if(theta < SINE_SIZE3) //384)
	{
		theta=-(theta-SINE_SIZE2);
		//negative=TRUE;
		//return -(sine_t)pgm_read_sine(sineTable[theta-SINE_SIZE2]); //256]);
	}
	//in the fourth quadrant, mirror both theta and the value and subtract the offset...
	else
	{
		theta=-(SINE_SIZE4-theta);
		//negative=TRUE;
		//return -(sine_t)pgm_read_sine(sineTable[SINE_SIZE4-theta]); //512-theta]);
	}

	return theta;
}

#if(!defined(SINE_DISABLE_SINERAW) || !SINE_DISABLE_SINERAW)
//Stores the scaled sine-values... so calculations can be executed initially
//  and retrieval can be quite quick...
//  These values are scaled to match the axes AND amplitude
//int32_t sineTable[129];

// [I think] Use pgm_read_dword(address_short) unless it's far... we'll have to determine that...
// probably OK since it's in the same file... who knows.. .data vs .text?


//theta is a value between 0 and 511 (representing an angle of 0-359.som'n deg)
//   rollover/wraparound should work fine... (e.g. Sine(coord_t))
//output is a value between -INT32_MAX and INT32_MAX (representing sin values -1 to 1)
sine_t sineRaw(theta_t theta)
{
	uint8_t negative=FALSE;

	theta = quadrantizeTheta(theta);
/*
//	theta %= 512;
	theta %= SINE_2PI;

	if(theta < 0)
		theta = SINE_2PI + theta;

//For more accurate realtimeishness, it might be worthwhile to reverse this order...
	
	//in the first quadrant, just take the sine value from the table
	if(theta < SINE_SIZE-1) //128)
	{
		//theta = theta;
		//return pgm_read_sine(sineTable[theta]);
	}	
	//in the second quadrant, mirror theta and subtract the offset
	else if(theta < SINE_SIZE2) //256)
	{
		theta=SINE_SIZE2-theta;
		//return pgm_read_sine(sineTable[SINE_SIZE2-theta]); //256-theta]);
	}
	//in the third quadrant, mirror the value and subtract the offset
	else if(theta < SINE_SIZE3) //384)
	{
		theta=theta-SINE_SIZE2;
		negative=TRUE;
		//return -(sine_t)pgm_read_sine(sineTable[theta-SINE_SIZE2]); //256]);
	}
	//in the fourth quadrant, mirror both theta and the value and subtract the offset...
	else
	{
		theta=SINE_SIZE4-theta;
		negative=TRUE;
		//return -(sine_t)pgm_read_sine(sineTable[SINE_SIZE4-theta]); //512-theta]);
	}

	//This reduced code-size from 3860 to 3838
*/
	//adding quadrantizeTheta increased it to 3844
	// but should be dramatic in combination with sineRaw8
	if(theta < 0)
	{
		theta = -theta;
		negative=TRUE;
	}

	sine_t sine=pgm_read_sine(sineTable[theta]);
	
	if(negative)
		sine = -sine;
	return sine;
}
#endif

#if ( defined(SINE_RAW8) && SINE_RAW8 )
int8_t sineRaw8(theta_t theta)
{
	/*
	theta %= SINE_2PI;

	if(theta < 0)
		theta = SINE_2PI + theta;

	if(theta < SINE_SIZE-1)
		return pgm_read_sine8(sineTable[theta]);
   else if(theta < SINE_SIZE2) //256)
		return pgm_read_sine8(sineTable[SINE_SIZE2-theta]); //256-theta]);
	else if(theta < SINE_SIZE3) //384)
		return -(int8_t)pgm_read_sine8(sineTable[theta-SINE_SIZE2]); //256]);
	else
	   return -(int8_t)pgm_read_sine8(sineTable[SINE_SIZE4-theta]); 
	*/

	//And doing this here, reduced from 3844 to 3770
	uint8_t negative = FALSE;
	theta=quadrantizeTheta(theta);

	if(theta < 0)
	{
		theta = -theta;
		negative = TRUE;
	}

	int8_t sine=pgm_read_sine8(sineTable[theta]);

	if(negative)
		sine = -sine;
	return sine;
}
#endif




//amplitude is a value from 0 to SINE_MAX (the ratio amplitude/SINE_MAX <= 1)
//axisMax could be larger than SINE_MAX, and should now be capable signed...
#if (!defined(SINE_DISABLEDOUBLESCALE) || !SINE_DISABLEDOUBLESCALE)
axis_t sineScaled(theta_t theta, axis_t axisAmplitude)
{
//	uint16_t shift = 0;
	axis_t temp;
//	int8_t axisSign = 1;
//	axis_t axisShifted = axisAmplitude;

/*
	//Necessary for shift...
	//@@@ There's got to be a better way...
	if(axisAmplitude < 0)
	{
		axisSign = -1;
		axisShifted = -axisShifted;
	}
	
#warning "Massive precision loss... could cause issues!!!"
	//Scale the axis if necessary
	while(axisShifted > (axis_t)SINE_MAX)
	{
		shift++;
		axisShifted = (axisShifted>>1);
	}
*/
	//Gives a value between +-axisMax, division by SINE_MAX gives the ratio of -1<=SINE/SINE_MAX<=1
//	temp = ( ((mult_t)( (mult_t)sineRaw(theta) * (mult_t)axisShifted )) / ((mult_t)SINE_MAX));
	temp = ( ((double)( (double)sineRaw(theta) * (double)axisAmplitude )) / ((double)SINE_MAX));
/*	
	//Necessary to handle shifting separately based on which sign sineRaw returned...
	if(temp < 0)
	{
		//Make temp positive (remove the sine bit)
		temp = -temp;
		//Do the shift
		temp = temp << shift;
		//Return the sign bit...
		temp = -temp;
	}
	else
	{
		temp = temp << shift;
	}
	
	if(axisSign > 0)
		return temp;
	else
		return -temp;
*/
	return temp;
}
#endif
#endif


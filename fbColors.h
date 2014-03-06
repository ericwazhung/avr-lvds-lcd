// 7 6 5 4 3 2 1 0
// a a b b g g r r
//             0 0 = 0x00
//             0 1 = 0x01
//             1 0 = 0x02
//             1 1 = 0x03
//         0 0     = 0x00
//         0 1     = 0x04
//         1 0     = 0x08
//         1 1     = 0x0C
//     0 0         = 0x00
//     0 1         = 0x10
//     1 0         = 0x20
//     1 1         = 0x30


//Takes three values, from 0-3 each...
#define rgb(r,g,b) (((r)&0x03) | (((g)&0x03)<<2) | (((b)&0x03)<<4))

/*  rgb8: (from  seg_question + rgb()
((((((r)*3)/255))&0x03) | ((((((g)*3)/255))&0x03)<<2) | ((((((b)*3)/255))&0x03)<<4))
*/

#define _R  0x03	//0x03						00 00 11
#define _O  0x07	//0x03 | 0x10				00 01 11
#define _Y  0x0F  //0x03 | 0x0C           00 11 11
#define _G 0x0C	//       0x0C				00 11 00
#define _C 0x3C   //       0x0C | 0x30    11 11 00
#define _B 0x30	//              0x30		11 00 00
#define _V 0x13   //0x03 |        0x10		01 00 11
#define _M  0x33	//0x03 |			 0x30		11 00 11
#define _K  0x00	//								00 00 00
#define _W 0x3F	//0x03 | 0x0C | 0x30		11 11 11
#define _r 0x01	//0x01						00 00 01
#define _g 0x04	//			0x04				00 01 00
#define _b 0x10	//					 0x10		01 00 00
#define _c 0x14	//			0x04	 0x10		01 01 00
#define _m 0x11	//0x01 |			 0x10		01 00 01
#define _y 0x05	//0x01 | 0x04				00 01 01
#define _k 0x15	//0x01 | 0x04 | 0x10		01 01 01
#define Tr 0xC0   //Transparent


//Creates a "gradient" effect, where no color-change is greater than 1
// for each change in value
// except at the crossover from full-blue back to 0... (0x30)
static __inline__ \
uint8_t rgbGradient(uint8_t value) \
	__attribute__((__always_inline__));

uint8_t rgbGradient(uint8_t value)
{
	while(value > 0x2f)
		value -= 0x30;

	uint8_t R = ((value)&0x03); 
	uint8_t r = ((value)&0x04) ? (3-R) : R ; 
	uint8_t G = ((value)&0x0C)>>2; 
	uint8_t g = ((value)&0x10) ? (3-G) : G ; 
	uint8_t B = ((value)&0x30) >> 4; 
	uint8_t b = ((value)&0x40) ? (3-B) : B ;

	return rgb(r,g,b);
}


/* mehPL:
 *    This is Open Source, but NOT GPL. I call it mehPL.
 *    I'm not too fond of long licenses at the top of the file.
 *    Please see the bottom.
 *    Enjoy!
 */










//fbColors refers to the color-values stored in, e.g. the frameBuffer
//        (if used). 
//        Basically, it's just 4-shades for each of R, G, and B.
//        ( Technically, B can only display 3 shades, due to the LVDS
//          implementation. )
//        (It's essentially storing RGB as 2 bits each, 64 colors, total)
//        (The alpha-channel isn't really implemented, in the vast majority
//        of cases)

// This also contains a "gradient" for drawing all the colors without sharp
// changes in color... (each adjacent color should only be one shade of
// difference).

//fbColors is used even when the frameBuffer isn't used... there're
//conversion-schemes all over the place to convert these RGB values to 
//other buffer-types.



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

// CAPS == Full-brightness
// lower-case == dim
#define _R  0x03	//0x03						00 00 11		RED
#define _O  0x07	//0x03 | 0x10				00 01 11		ORANGE
#define _Y  0x0F  //0x03 | 0x0C           00 11 11		YELLOW
#define _G 0x0C	//       0x0C				00 11 00		GREEN
#define _C 0x3C   //       0x0C | 0x30    11 11 00		CYAN
#define _B 0x30	//              0x30		11 00 00		BLUE
#define _V 0x13   //0x03 |        0x10		01 00 11		VIOLET
#define _M  0x33	//0x03 |			 0x30		11 00 11		MAGENTA
#define _K  0x00	//								00 00 00		BLACK
#define _W 0x3F	//0x03 | 0x0C | 0x30		11 11 11		WHITE
#define _r 0x01	//0x01						00 00 01		red
#define _g 0x04	//			0x04				00 01 00		green
#define _b 0x10	//					 0x10		01 00 00		blue
#define _c 0x14	//			0x04	 0x10		01 01 00		cyan
#define _m 0x11	//0x01 |			 0x10		01 00 01		magenta
#define _y 0x05	//0x01 | 0x04				00 01 01		yellow
#define _k 0x15	//0x01 | 0x04 | 0x10		01 01 01		black
#define Tr 0xC0   //Transparent
// Other colors exist, but aren't named...

//The idea, here, was to be able to initialize arrays with images that
//could be discerned visually... but the ',_' in the initializers made it
//difficult to see


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
 * /Users/meh/_avrProjects/LCDdirectLVDS/90-reGitting/fbColors.h
 *
 *    (Wow, that's a lot longer than I'd hoped).
 *
 *    Enjoy!
 */

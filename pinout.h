// Huh, I thought there was a pinout file already... I guess this isn't
// actually used as a header file, but might be soon.
//
// So, here I am trying to piece it all together from the code, and the PCB


/*Just ignore this section.
// Best to probably ignore the HEART pin-stuff in the makefile...
// It's a bit confusing because:
// A) The heart software has been removed (HEART_REMOVED=TRUE)
//    (to save codespace)
// B) The heart isn't soldered up on the latest board
// C) "QUESTION" mode uses PB0 pulled-low as a button-input
//    GENERALLY, I use the heart pin as an input, for such purposes...
// D) GENERALLY, I use the heart pin on the programming-header
//    as my programmer has a button on that pin for exactly these purposes
*/



//From main.c:
//   It's probably best to use two XORs from the same chip for a single
//    LVDS channel, since different chips may have slightly different
//    characteristics. 
//
//   The entire circuit, thus, requires TWO 74LS86's 
//    (four XORs apiece, two per LVDS channel, 8-total)
//
//                                                              __________
//              VCC3V3   VCC3V3                                | LCD
//                |        |                                   |
//                +---\ \¯¯¯-_       100ohm OPTIONAL See below |
//                     | |    ¯-                               |
//                     | | XOR   >---/\/\/\-------> RXinN/clk- |---+
//   AVR               | |    _-                               |   |
//   OC1x >----+------/ /___-¯                                 |   |
//   output    |                                               |   \ 100ohm
//             |                                               |   / built-
//             |                                               |   \ in to 
//             `------\ \¯¯¯-_       100ohm OPTIONAL See below |   / LCD
//                     | |    ¯-                               |   |
//                     | | XOR   >---/\/\/\-------> RXinN/clk+ |---+
//                     | |    _-                               |
//                +---/ /___-¯                                 |
//                |        |
//               GND      GND
// The XOR outputs are wired directly to the LCD's LVDS inputs
// It's explained elsewhere, but these particular LS chips underdriven at
//  3.3V and overloaded with 100ohms (in the LCD) causes damned-near 
//  perfect voltage-levels for LVDS.
//  IN OTHER WORDS: As-Is, in my setup, with LS's from 1980, the 100ohm
//   resistors are *not* used. And I have not tested use of them.
//
// IT IS WISE TO TEST THE VOLTAGE OUTPUT LEVELS OF YOUR XOR CHIPS
//  BEFORE CONNECTING THE LCD by connecting a 100ohm resistor
//  between the + and - XOR outputs.
//  Newer chips probably have higher drive ability, so this is wise even
//  with 74LS chips newer than 1980 (the ones I used)
//  I suppose if the voltage is too high, then you could use a resistor on
//  each output (in series with the LCD inputs, as shown)... 
//  Ideally the voltage across that resistor should be no more than +/- 1V
//  probably 100ohms on each pin?
//  No promises, here...

// OC1x outputs:
//  These might be listed differently in various places... I went through
//  many iterations, trying to find the most useful/versatile. Latest is:
//   (This has been pretty-well verified)
//   PB3   OC1B -> LVDS-CLK
//   PB2  /OC1B -> LVDS-"Green" (RXin1)
//   PB5   OC1D -> LVDS-"Red"   (RXin0)
//   PB1   OC1A -> LVDS-"D/V/H/Blue"  (RXin2)

// GREEN needs to be inverted... this is easily done by swapping the 
//  RXin1+ and RXin1- out of the respective XORs to the LCD


/* From lvds_timerInit() in main.c:
   setoutPORT(PB1, PORTB); //+OC1A, DVH/BLUE, MISO (usually heart)
   setoutPORT(PB2, PORTB); //-OC1B, -GREEN    (INVERTED) SCK
   setoutPORT(PB3, PORTB); //+OC1B Clock (OC1B, not inverted)
   setoutPORT(PB5, PORTB); //+OC1D, RED
*/



//Programming Header SIP (yours may vary)
// 1  GND
// 2  V+
// 3  SCK   PB2   
// 4  MOSI  PB0   
// 5  /RST
// 6  MISO  PB1   (Usually Heart)


//  TO LCD Buffers
//   |                       ATtiny861
//   V                       ____________________
//                         |         |_|        |
//         Button / MOSI --|  1 PB0      PA0 20 |-- 
// DVHBlue (OC1A) / MISO --|  2 PB1      PA1 19 |-- 
// -Green (/OC1B) / SCK  --|  3 PB2      PA2 18 |-- 
// Clock   (OC1B)  --------|  4 PB3      PA3 17 |-- 
//                   VCC --|  5 VCC     AGND 16 |-- GND
//                   GND --|  6 GND     AVCC 15 |-- VCC
//                       --|  7 PB4      PA4 14 |--       VCC
//   Red   (OC1D)  ----- --|  8 PB5      PA5 13 |--        |
//                       --|  9 PB6      PA6 12 |--        \  Potentiometer
//                /Reset --| 10 PB7      PA7 11 |-- <----->/  5k
//                         |____________________|          \  for "Racer"
//                                                         |  control
//                                                        GND


// Button is used in "Mario"/"QUESTION" mode. Shorting the pin to ground
// momentarily causes the image to change.

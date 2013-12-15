// Looking back into other ways of getting more colors...
// Considering switching the TCNT to start at the FALLING edge of clock


//Current, a/o early v54:
//                           OCR1B=3
//  TCNT:     |  0   1    2    3  | 4    5    6
//
//  CLKideal: /¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯\______________/
//
//            |____ ____ ____.____v              |
//  CLK:   D0>/ D1>/ D2>/         \    .    .    /
//  OC1B      |¯¯¯¯ ¯¯¯¯          |¯¯¯¯ ¯¯¯¯ ¯¯¯¯|
//            |                   |
//            | G2   G1   B1   B0 | G5   G4   G3
//  Green:    |                   |____ ____ ____
//  /OC1B     \    .    .    . d0>/ d1>/ d2>/ d3?\  //
//            |¯¯¯¯ ¯¯¯¯ ¯¯¯¯ ¯¯¯¯ ¯¯¯¯ ¯¯¯¯ ¯¯¯¯
//            |
//            | R1   R0   G0   R5   R4   R3   R2
//  Red:      |____ ____ ____ ____ ____ ____ ____
//  OC1D   D0>/ D1>/ D2>/ C2>\ C3>\ C4>\ C5>\ C6...
//             ¯¯¯¯ ¯¯¯¯      ¯¯¯¯ ¯¯¯¯ ¯¯¯¯ ¯¯¯¯
//             ____ ____ ____ ____ ____ ____ ____
//  /OC1D     \ ^OCR1D>=6^ L>/  M>/  N>/  O>/ P?>...
//             ¯¯¯¯ ¯¯¯¯ ¯¯¯¯ ¯¯¯¯ ¯¯¯¯ ¯¯¯¯ ¯¯¯¯
//
//              B3   B2   DE   /V   /H   B5   B4
// "Blue/DVH" |____ ____ ____.____.____ ____ ____
//  OC1A:  D0>/ D1>/ D2>/           C4>\ C5>\ C6...
//            |¯¯¯¯ ¯¯¯¯                ¯¯¯¯ ¯¯¯¯
//             ____ ____ ____ ____ ____ ____ ____
//  /OC1A:    \  ^^^--OCR1A>=6--^^^  X>/  Y>/ Z?>\  //
//  (usable?)  ¯¯¯¯ ¯¯¯¯ ¯¯¯¯ ¯¯¯¯ ¯¯¯¯ ¯¯¯¯ ¯¯¯¯
//                 X: OCR1A=4, DTL=0
//                 Y: OCR1A=4, DTL=1; OCR1A=5, DTL=0
//                 Z: OCR1A=4, DTL=2; OCR1A=5, DTL=1; OCR1A>=6


//Considering revising it to use the other edge...
// (This was the old method, before adding Red and Green)
// But something doesn't seem right... how would Nada be possible?!
//  use DTH for TCNT0 delayed output from H to L
//  use OCR for controlling L to H
//
// Blue/DE/V/H were controllable...
//                v---Clock Edge, separation between "pixels" transmitted
// Cases:   DE /V  /H  Blues                    +OC1out
//          H   H   H  H/L     DE Active        DT>=1, OCR<=3
//          L   H   H  X       Nada Active      DT=1, OCR=4
//          L   L   H  X       Vsync without H  DT=1, OCR=6 NO! No /H=H???
//          L   L   L  X       Vsync with H		DT=0, OCR=0
//          L   H   L  X       Hsync only			DT=0, OCR=4

// If clock is single-ended (not affected by DTH/L)
//  Then the clock's other OCR output cannot be used by other LVDS channels
//  DTL(H?) is required for DE-mode (Hsync inactive)
//    which affects the other OCR channel
//      (This note long after last update to this file...)
//			Might make more sense to use Red on the affected channel
//       Since R4 = H => 16/63 = quite dim
//         (If such that R4 is *low* then black could be achievable as well
//          as 48/63rds ~Full Bright)
//    
// And a LOT of other assumptions, not looked into...

//                      OCR1B=2
//  TCNT:     |  0   1    2  | 3    4    5    6
//
//  CLKideal: \______________/¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯\
//
//              /H   B5   B4   B3   B2   DE   /V
// "Blue/DVH" |____ ____ ____ ____ ____ ____.____.
//  OC1A:     |    \    X    X    X    /
//            |     ¯¯¯¯ ¯¯¯¯ ¯¯¯¯ ¯¯¯¯
//               DT1  DT2  DT3  DT4
//                 v____/____/____/____
//     OR:     ____/____ ____ ____ ____\____.____.
//    DE:       ^       \    \    \    ^
//             C0       C1   C2   C3   C4
//
//   Possibly: Use Fixed (large) DT
//             Then OCR is exclusively responsible for Blue's intensity
//             No... C is for Falling edge...
//             So then, DT must be always 1
//             Then other colors aren't dependent on blue...
//
//             (These notes and the following diagrams new, untested)
//             (DT > 4, for here, plausible, despite DE==Active, 
//              assuming OCRBLUE <= DT...)
//              ALSO: DT1 *HERE* would be overridden similarly if
//              OCR*HERE* > 6 or < DT1 ?
//              Would allow for special cases of full/no red/green)
//
//
//               DT1  DT2  DT3  DT4  DT5  DT6
//                 v____/____/____/____/____/____
//             ____/____ ____ ____ ____ ____ ____
//              ^       \    \    \    \    \
//             C0       C1   C2   C3   C4   C5
//             
//              R4   R3   R2   R1   R0   G0   R5
//              G5   G4   G3   G2   G1   B1   B0
//
//              C0
//             ____ ____ ____ ____ ____ ____ ____
//            /____ ____ ____ ____ ____ ____ ____
//                 \    \
//                

//
//		Some sign-stuff is in order, as this is getting a bit confusing
//
// E.G. OCR1C = 5 ("TOP")
//      OCR1A = 3
//      DT1H = 1
//      DT1L = 2
//
//
// TCNT 
//   0    1    2    3    4    5    6    0    1    2    3    4    5    6
// .                              ____.                              ____.
// |               ____.----'¯¯¯¯'    |               ____.----'¯¯¯¯'    |
// |____.----'¯¯¯¯'                   |____.----'¯¯¯¯'                   |
//
// OCW1A                ____.____.____                     ____.____.____
// \____.____.____.____/              \____.____.____.____/              \
//                     ^              ^
// OC1A   (DT1H = 1)-->|<-->|         |
//                     |    |____.____v                         ____.____
// \____.____.____.____.____/         \____.____.____.____.____/         \
//                     |              |
// /OC1A               |              |<------->|<--(DT1L = 2)
//            ____.____v              |         |____.____
// .____.____/         \____.____.____.____.____/         \____.____.____.
//                     |              |
//                     ^              ^
//                   These edges always aligned
//                   Regardless of DT/OCR values

//   With these arbitrarily-chosen values, it's difficult to see the
//   relationship between OC1A, /OC1A, and OCW1A
//   (which is exemplary of how it can be finnaggled!)
//   Note /OC1A FALLS as a result of OCW1A's Rising edge, thus it's the
//   "inverted" output. Likewise, OC1A RISES as a result of OCW1A's RISING
//   edge (though, delayed by DT1H) thus it's the "non-inverted" output
//   Note also, the output of OCW1A changes *after* a compare-match
//   (as it moves away from, or exits the matching TCNT value)
//  DUH: TCNT goes from 0 to 6, no?... 
//  Also consider special cases: 
//       OCR1A=0 means OCW1A is always high
//       OCR1A=1 means OCW1A is high AFTER TCNT=1, (TCNT>=2)
//       Thus, there is no case where OCW1A becomes high during TCNT=1
//       OC1A and /OC1A are entirely dependent on OCW1A:
//         If OCW1A doesn't change levels, then DT values have no effect
//
//
//    (Back to old notes)
//
//            /    /    /    /    \    \    \
//              R4   R3   R2   R1   R0   G0   R5
//            
//
//              G5   G4   G3   G2   G1   B1   B0
//                  ____ ____ ____ ____
//            .____/    \____\____\____\____.____  \   Invert maybe?
//              DT1^  C1^  C2^  C3^  C4^            >  For G5=fullBright
//      OCR=0 .____.____.____.____.____.____.____  /   but G5 never off!
//
//             ____ ____ ____ ____ ____ ____ ____
//            /____\____\____\____\____\____\____X
//              ^  ^    ^    ^    ^
//           C0,d0 |    |    |    |
//                 |    |    |    +--C0,d4???;C1,d2;C2,d1;C3,d0
//                 |    |    +--C0,d3???;C1,d1;C2,d0
//                 |    +--C0,d2???; C1,d0
//                 +--C0,d1???
//
//     So, possible combos:
//       DT1, BC0: B5 B4 B3 B2 
//       DT1, BC1:    B4 B3 B2
//       DT1, BC2:       B3 B2
//       DT1, BC3:          B2
//
//    DT1, C0, d0:              G5 G4 G3 G2 G1 (B1 B0)  
//    DT1, C0, d1:
//    DT1, C1, d0:              G5    G3 G2 G1
//    DT1, C

//
//  Wearing down... since R would be highly dependent on G.
//  Also, G can't have G5 variable... (no black, or no white!)
//
//  With a DE-only display, H is irrelevent... DT could vary to 0
//  

//look at cTools/lvdsColorExperiments.c...
// latest is glColorTest/ shows (all?) possible color-pallete combinations
// Most are quite... pastel. And consist of a wide range of similar colors
//  but few *different* colors.
// There are *more* colors, and the palletes
// could be switched rather easily by attaching the level-choosing XOR
// inputs to a pin on the MCU...
// There's a lot of contemplations here...
// *how to implement regular color-selection... can it still be stored
//  in as few (packed) bytes? (actually, maybe more can, since each 4bit
//  value could control a possible of 16 colors... interesting...
// (THIS COULD BE USED NOW, why limit to four-colors???)
// (...because the only other possible colors in the current mode are
//  indescernable)
// *how to implement pallete-switching (row-by-row would be easy, but
//  otherwise would require modifying the color-writing functions which are
//  rather optimized... 
// I guess it depends on your needs, more colors and fewer pixels...?

/* mehPL:
 *    This is Open Source, but NOT GPL. I call it mehPL.
 *    I'm not too fond of long licenses at the top of the file.
 *    Please see the bottom.
 *    Enjoy!
 */


#if (defined(REMOVE_DC) && REMOVE_DC)
#error "REMOVE_DC hasn't been tested since lcdStuff, or long prior"
 #define Nada_init()         { DT1=(3<<4); OCR1A=5; }

 //Unused, normally...
 #define Vsync_init()       { DT1=(4<<4);  OCR1A=5; }
 #define VplusH_init()      { DT1=0;       OCR1A=1; }
 #define Hsync_init()       { DT1=(3<<4);  OCR1A=3; }
 #define DEonly_init()      { DT1=(2<<4);  OCR1A=4; }
 #define DEblue_init()       { DT1=0;       OCR1A=5; }

#else //!REMOVE_DC
//Nada init
 #define Nada_init()         { DT1=(3<<4); OCR1A=4; }

//Unused, normally...
 #define Vsync_init()         { DT1=(4<<4);   OCR1A=4; }
 #define VplusH_init()      { DT1=0;         OCR1A=0; }
 #define Hsync_init()         { DT1=(3<<4);   OCR1A=3; }
// #define DE_init()            { DT1=(2<<4);   OCR1A=4; } //...SHOULD BE
 #define DEonly_init()         { DT1=(2<<4);  OCR1A=4; }
 #define DEblue_init()         { DT1=0;       OCR1A=6; }
//#define DE_init()            { DT1=(1<<4);   OCR1A=2; } //Works with DE_ONLY
 //#define DE_init()            { DT1=0;         OCR1A=0xff; }   //DE_BLUE
#endif //REMOVE_DC



//Display is DE-Only (doesn't use H/Vsync)
// Shouldn't be necessary to select this if it is,
// since DE timing is the same either way
// but I want to test whether a single-bit is being detected
// (e.g. maybe the rise/fall-times of the output aren't fast enough for a
//  single bit, which might explain why the other display didn't work)
#if (defined(DE_ONLY_DISPLAY) && DE_ONLY_DISPLAY)
#define Vsync_fromNada()      Nada_init()
#define VplusH_fromVsync()    Nada_init()
#define Vsync_fromVplusH()    Nada_init()
#define Nada_fromVsync()      Nada_init()
#define Hsync_fromNada()      Nada_init()
#define Nada_fromHsync()      Nada_init()
#define DEonly_fromNada()     DE_init()
#define DEblue_fromDEonly()   DE_init()
#define Nada_fromDEblue()      Nada_init()
#define DEonly_fromDEblue()	DE_init()
 #if(DE_BLUE)
  #warning "DE_BLUE is true, but not implemented with DE_ONLY_DISPLAY"
  #warning "...The display should be blank"
 #endif
//Use full initialization of each LVDS state
// (might not be good during transitions, but should guarantee
//  steady-state, in case my transitions aren't correct...)
#elif (defined(FULL_INIT_TESTS) && FULL_INIT_TESTS)
#define Vsync_fromNada()      Vsync_init()
#define VplusH_fromVsync()      VplusH_init()
#define Vsync_fromVplusH()      Vsync_init()
#define Nada_fromVsync()      Nada_init()
#define Hsync_fromNada()      Hsync_init()
#define Nada_fromHsync()      Nada_init()
#define DEonly_fromNada()      DEonly_init()
#define DEblue_fromDEonly()   DEblue_init()
#define Nada_fromDEonly()      Nada_init()
#define Nada_fromDEblue()      Nada_init()
#define DEonly_fromDEblue()	DEonly_init()

#else   //NOT FULL_INIT_TESTS
//#define Vsync_fromNada()   { DT1=(4<<4); }
#define Vsync_fromNada()   { DT1=(4<<4); OCR1A=5; } //Leave two bits high
                                                    //for easy-scoping
                  //Three would be nicer, but I'm pretty sure OCR=TOP=ON
#define VplusH_fromVsync() { OCR1A=0; }
//#define Vsync_fromVplusH() { OCR1A=4; }
#define Vsync_fromVplusH() { OCR1A=5; }            //Extra bits for scoping
//#define Nada_fromVsync()   { DT1=(3<<4); }
#define Nada_fromVsync()   { DT1=(3<<4); OCR1A=4; } //scoping...
#define Hsync_fromNada()   { OCR1A=3; }
#define Nada_fromHsync()   { OCR1A=4; }

#define Nada_fromDEblue()   { DT1=(3<<4); OCR1A=4; }
#define Nada_fromDEonly()   { DT1=(3<<4); }
#define DEonly_fromNada()   { DT1=(2<<4); }
#define DEblue_fromNada()   { OCR1A=0xff; }
#define DEblue_fromDEonly() { OCR1A=0xff; }

//a/o v62: This hadn't existed previously, necessary for
//BLUE_VERT_BAR_REVERSED
#warning "This is just a guess, piecing together Nada_fromDEblue and whatnot"
//But it seems to work... 
#define DEonly_fromDEblue() { OCR1A=4; }

/*
#if (!defined(DE_BLUE) || !DE_BLUE)
 //Nada -> DE
 //#define DE_fromNada()      { DT1=(1<<4); }   //DT1 = (1<<4);
 #define DE_fromNada()      { DT1=(2<<4); }
 //DE -> Nada
 //#define Nada_fromDE()      { DT1=(2<<4); }   //DT1 = (2<<4);
 #define Nada_fromDE()      { DT1=(3<<4); }
#else
 //Nada -> DE
 //#define DE_fromNada()      { OCR1A=0xff; }   //DT1 = (1<<4);
 #define DE_fromNada()      { OCR1A=0xff; }
 //DE -> Nada
 //#define Nada_fromDE()      { OCR1A=3; }   //DT1 = (2<<4);
 #define Nada_fromDE()      { OCR1A=4; }
#endif
*/
#endif //End Of FULL_INIT_TESTS


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
 * /Users/meh/_avrProjects/LCDdirectLVDS/93-checkingProcessAgain/_interfaces/lvdsTransitionsOnly.h
 *
 *    (Wow, that's a lot longer than I'd hoped).
 *
 *    Enjoy!
 */

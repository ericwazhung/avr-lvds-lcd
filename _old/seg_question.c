/* mehPL:
 *    This is Open Source, but NOT GPL. I call it mehPL.
 *    I'm not too fond of long licenses at the top of the file.
 *    Please see the bottom.
 *    Enjoy!
 */










//seg_question.c (uses rowSegBuffer):
//Displays a Question-Mark box, ala Mario-Brothers. Press the button and
// receive a reward (and occasional goomba)
// Demonstrates usage of program-memory-based images in rowSegBuffer... 
// (16x16 pixels WOO!)
// This is much less functional than FB_QUESTION


void segQuestion_loadRow(uint16_t rowNum) 
{
//Actually, this probably won't work right...
// because the rgb values aren't exact like that, right?
// e.g. black = 0,0,0 but it shows as 0,0,60 ish...
// then again, going in reverse like this might work fine...
// due to rounding (seems OK with this color-scheme)
// The idea wasn't so much to be able to input an 8-bit RGB value to get
// a close match (would probably want rounding up for that, not that it'd
// be even close)
// but to be able to read color-values from GIMP when using the 
//  LCDdirectLVDS color-palette... which isn't especially necessary since
//  now the colors are named with 3-based colors.
#define rgb8(r,g,b) \
      rgb((((r)*3)/255), (((g)*3)/255), (((b)*3)/255))



//THESE DON'T BELONG HERE
// AND they shouldn't be "Q" specific, at all.
#define Q_WIDTH   16
#define Q_HEIGHT   16
      //This should be an init-thing, I guess... it's only a 2-cycle
      // instruction, so it doesn't hurt too much to have it in the loop
      // BUT it does have to occur a while before the first getpinPORT
      // to allow time for the pull-ups to do their job
      setinpuPORT(PB0, PORTB);

   static uint8_t qCount = 0;

   segClear();
   //Not sure why this is stretched without /2...
   // I think I ran into this elsewhere as well.
   //!!! INSTEAD: filled the rest of the row properly...
   // (wasn't filling up the entire row, due to rounding...
   //   result was the rows were carrying-over into the next)
   uint8_t qRow;// = (rowNum*Q_HEIGHT/(V_COUNT)); ///2));
   uint8_t qCol;
   
   //Guess white helps with sync...?
   // can't hurt.
   //addSegfb(3, _W);
   // Looks like some white is already being inserted (or syncing on end-
   // white?) about 3pixels wide.

   // 1024/3/32 = 10.67... so for every 3 segments, add 2
#define Q_STRETCHTOP   1//0//28//2
#define Q_STRETCHDIV 5//30//3
   hfm_t colincrementer_hfm;
   hfm_setup(&colincrementer_hfm, Q_STRETCHTOP, Q_STRETCHDIV);

   for(qCol=0; qCol<Q_WIDTH; qCol++)
   {
      //Not sure where this is defined, if it is...
      // each "segment length" appears to be 3 physical pixels
      //+1 is a result of random experiments with stretch
      // resulting in a stretch larger than 1
      //#define Q_SEGLENGTH   (1024/3/Q_WIDTH+1)
		#define Q_SEGLENGTH (NUM_PSEGS / Q_WIDTH)

   //These are not included in NUM_ICONS
   #include "icons/Question.h"
   #include "icons/Solid.h"
   #include "icons/GoombaDead.h"

   //These are included in NUM_ICONS
   #include "icons/1up.h"
   #include "icons/Biggie.h"
   #include "icons/StarV.h"
   #include "icons/FlowerPowerV.h"
   #include "icons/Goomba.h"
   #include "icons/Coin.h"

   //Excluding Solid and Question, etc...
   #define NUM_ICONS 6
      /*
      static uint8_t iconSelection = 0;
      static uint8_t * icon[NUM_ICONS] = { pgm_imageUP, pgm_imageCOIN};
      static uint8_t iconFrames[NUM_ICONS] = {NUM_UPS, NUM_COINS};

      static uint8_t * p_image = pgm_imageUP; // = icon[0]; //(pgm_imageFLOWER);
      static uint8_t NumFramesInImage = NUM_UPS; // = NUM_FLOWERS;
      */
      //For changing images...
      static sprite_t *p_selectedSprite = &spriteQ;
      static uint16_t buttonPressed = FALSE;

#define DEBUGGING_PRESSIMMEDIATE TRUE

#if(!defined(DEBUGGING_PRESSIMMEDIATE) || !DEBUGGING_PRESSIMMEDIATE)
      if((p_selectedSprite == &spriteQ) 
            || (p_selectedSprite == &spriteGOOMBA))
#else
   #warning "Debugging mode... players can change their result!"
#endif
      if(!getpinPORT(PB0, PORTB))
      {
         if(!buttonPressed)
         {
            buttonPressed = rowNum;
            if(buttonPressed == 0)
               buttonPressed = 1;
         }
         else if((rowNum-buttonPressed) >= 64)
            buttonPressed += 0x4000;
      }


#define REALLY_IMMEDIATE   TRUE
#if (defined(REALLY_IMMEDIATE) && REALLY_IMMEDIATE)
      if((rowNum == 0) || (buttonPressed&0x8000))
#else
      if(rowNum==0)
#endif
      {
         if(!(buttonPressed&0x8000) && !((p_selectedSprite == &spriteQ) ||
                                    (p_selectedSprite == &spriteGOOMBA)))
            buttonPressed = FALSE;

         if(buttonPressed)
         {
            if(p_selectedSprite == &spriteGOOMBA)
            {
               if(qCount == 1)
                  p_selectedSprite = &spriteDEADGOOMBA;

            }
            else
            {
               //Duh...
               // But still, 255 % 3 should give a value from 0 to 2
               //buttonPressed=FALSE;
               qCount = 0;
               //static uint8_t iconSelection;
               //iconSelection++;
               //iconSelection%=NUM_ICONS;
               //NumFramesInImage = iconFrames[iconSelection];
//#define RANDOMSPRITE_OVERRIDE 0
#if(defined(RANDOMSPRITE_OVERRIDE))
               switch(RANDOMSPRITE_OVERRIDE)
#else
               switch ((buttonPressed-1)%(NUM_ICONS*2))
#endif
               {
                  case 0:
                     p_selectedSprite = &spriteFLOWER;
                     break;
                  case 1:
                     p_selectedSprite = &sprite1UP;
                     break;
                  case 2:
                     p_selectedSprite = &spriteBIG;
                     break;
                  case 3:
                     p_selectedSprite = &spriteSTAR;
                     break;
                  case 4:
                     p_selectedSprite = &spriteGOOMBA;
                     break;
                  default:
                     p_selectedSprite = &spriteCOIN;
                     break;
               }
            }

            //buttonPressed=FALSE;
         }
         if(p_selectedSprite == &spriteCOIN)
         {
            if(qCount >= 2)
               p_selectedSprite = &spriteSOLID;
         }
         else if(p_selectedSprite != &spriteQ)
         {
            if(qCount >= 4)
            {
               qCount = 0;
               if(p_selectedSprite != &spriteSOLID)
                  p_selectedSprite = &spriteSOLID;
               else
                  p_selectedSprite = &spriteQ;
            }
         }

         buttonPressed = FALSE;
      }

      qRow = (rowNum*ICON_HEIGHT/(V_COUNT)); ///2));
      



//      p_image = (icon[iconSelection] + 
//                  (qCount%NumFramesInImage)*ICON_HEIGHT*ICON_WIDTH);

      // Should use readImageByte... but it's specific to FB_WIDTH
      //uint8_t data=
      //  pgm_read_byte((uint8_t *)(&((p_image)[(qRow)*Q_WIDTH+(qCol)])));
      uint8_t data;
     
      if(p_selectedSprite == &spriteFLOWER)
      {
         if(qRow<FLOWER_PALETTE1_ROW)
            data = getGimpColorVal(&spriteFLOWER,
                     qCount%(spriteFLOWER.numPalettes-1) + 1, qRow, qCol);
         else
            data = getGimpColorVal(&spriteFLOWER,0,qRow, qCol);
      }
      else if(p_selectedSprite == &spriteGOOMBA)
      {
         uint8_t gCol = qCol;
         //Goomba moves by flipping horizontally...
         if(qCount & 0x01)
            gCol = 15-qCol;

         data = getGimpColorVal(&spriteGOOMBA, 0, qRow, gCol);
      }
      else
      {
         data = getGimpColorVal(p_selectedSprite, 
                     qCount%p_selectedSprite->numPalettes, qRow, qCol);
      }
      data = gimpPixelValToLColor(data);

      addSegfb(Q_SEGLENGTH + hfm_nextOutput(&colincrementer_hfm), data);
   }


   //Wow, is it really so smart as to recognize that rowNum is never >=
   // V_COUNT?! was 7684 when if(rowNum==0), then 7668 after this bug...
   //if(rowNum >= V_COUNT)
   if(rowNum >= V_COUNT-1)
   {
      qCount++;
   }


   //Wait, what?! We can't fit 1024 segments! But this sorta works
   // We're gtting syncing, a tiny bit of white-space on the right
   // which seems reasonable based on shotty math, 
   //                10     *  32   +  10   *    2       /   3 = 326
   // but 1024/3 is 341 (where's my math error?)
   //addSegfb(1024-Q_SEGLENGTH*Q_WIDTH-Q_WIDTH*Q_STRETCHTOP/Q_STRETCHDIV, _W);
   // But, nope, 341-... doesn't sync (vertical stretching again)
   // But, 342 does. Sweet. 1024/3=341.33333, actually.
   //addSegfb(342-Q_SEGLENGTH*Q_WIDTH-Q_WIDTH*Q_STRETCHTOP/Q_STRETCHDIV, _W);
   // Because it's not 10*2/3, it's 32*2/3=21.3...
   // which is exactly right
   // which then doesn't explain why it has whitespace and doesn't sync
   // without 342 above...
   // something to do with white being necessary?
   // Can't recall where 1024/3 was determined, maybe it varies
   // since the LVDS clock is 7 cycles...?
   //OK, so maybe we just need a white pixel at the end...
   // I mean, this works.
   addSegfb(1,_W);


   segTerminate();
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
 * /Users/meh/_avrProjects/LCDdirectLVDS/90-reGitting/_old/seg_question.c
 *
 *    (Wow, that's a lot longer than I'd hoped).
 *
 *    Enjoy!
 */

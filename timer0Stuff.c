#include "timer0Stuff.h"

//Testing with values returned by timer0Calculator
//#define TIMER0_OCRVAL	65

void init_timer0Hsync(void)
{
   OCR0A = TIMER0_OCRVAL; //T_Hlow_CYC + T_HD_CYC + T_DE_CYC + T_DH_CYC;
   timer_setWGM(0, WGM_CLR_ON_COMPARE);
   timer_selectDivisor(0, TIMER0_CLKDIV); //CLKDIV1);
   timer_compareMatchIntEnable(0, OUT_CHANNELA);
}



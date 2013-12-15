void pll_enable(void)
{
   //Stolen from threePinIDer109t:

   //Set Timer1 to use the "asynchronous clock source" (PLL at 64MHz)
   // With phase-correct PWM (256 steps up, then back down) and CLKDIV1
   // this is 64MHz/512=125kHz
   // The benefit of such high PWM frequency is the low RC values necessary
   //  for filtering to DC.
   // "To change Timer/Counter1 to the async mode follow this procedure"
   // 1: Enable the PLL
   setbit(PLLE, PLLCSR);
   // 2: Wait 100us for the PLL to stabilize
   // (can't use dmsWait since the timer updating the dmsCount hasn't yet been started!)
   _delay_us(100);
//   dmsWait(1);
   // 3: Poll PLOCK until it is set...
   while(!getbit(PLOCK, PLLCSR))
   {
      asm("nop");
   }
   // 4: Set the PCKE bit to enable async mode
   setbit(PCKE, PLLCSR);

}

// This bit was in SIGNAL(TIMER0...)
// right at the beginning...

#if (defined(SLOW_EVERYTHING_TEST) && SLOW_EVERYTHING_TEST)
   while(TCNT0L != 7){};
   //This won't work... who knows how many cycles interrupt-entry will be
   //Above should fix that...
   //I believe this was only so I'd have something for the 'scope to
   // trigger on... such that the pixel-clock would align with the
   // register-assignments... Not sure it ever worked.
#error "These shouldn't be used anymore, since PB2 is GREEN"
   setpinPORT(PB2, PORTB);
   clrpinPORT(PB2, PORTB);
#endif


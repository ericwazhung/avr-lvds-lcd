//Auto-generated by makefile

#ifndef __PROJINFO_H__
#define __PROJINFO_H__
#include <inttypes.h>

#if (defined(_PROJINFO_OVERRIDE_) && _PROJINFO_OVERRIDE_)
 const uint8_t __attribute__ ((progmem)) \
   header[] = "";
#elif (defined(PROJINFO_SHORT) && PROJINFO_SHORT)
 const uint8_t __attribute__ ((progmem)) \
   header[] = "heartbeatTesttestPWM161+button 2014-04-22 01:35:50";
#else //projInfo Not Shortened nor overridden
 const uint8_t __attribute__ ((progmem)) \
   header0[] = " /Users/meh/_commonCode/heartbeat/1.30/testPWM161+button ";
 const uint8_t __attribute__ ((progmem)) \
   header1[] = " Tue Apr 22 01:35:50 PDT 2014 ";
 const uint8_t __attribute__ ((progmem)) \
   headerOpt[] = "  ";
#endif

//For internal use...
//Currently only usable in main.c
#define PROJ_VER testPWM161+button
#define COMPILE_YEAR 2014

#endif


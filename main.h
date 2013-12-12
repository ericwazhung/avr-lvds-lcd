#ifndef __MAIN_H__
#define __MAIN_H__

#include <avr/io.h>
#include <inttypes.h>
#include <stdlib.h>

/* Interrupt Handling */
#include <avr/interrupt.h>

#include _BITHANDLING_HEADER_
//#include _SINETABLE_HEADER_
//#include _SINETRAVEL_HEADER_
//#include _GOTORAMPED_HEADER_
//#include _UART_OUT_HEADER_
//#include _UART_IN_HEADER_
#include _HEARTBEAT_HEADER_
//#include _HPGLTEXT_HEADER_

//These are also included in limits.c... kinda hokey
//#ifdef _KC644_
// #include "xyKC644.h"
//#else
// #include "xyKrautBoard.h"
//#endif


#endif


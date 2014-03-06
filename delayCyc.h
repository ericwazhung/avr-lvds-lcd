#ifndef __DELAY_CYC_H__
#define __DELAY_CYC_H__

//This should probably be reimplemented using _delay_loop_1/2() from 
// util/delay_basic.h
//  1 uses three cycles per count, counts from 1-256, 
//      256 counts: _delay_loop_1(0)
//  2 uses four, counts from 1-65536, 65536 is passed as 0


//This isn't valid, who knows how many instructions the for loop takes
static __inline__ \
void delay_cyc(int32_t numCyc) \
     __attribute__((__always_inline__));

#ifndef DELAY_CYC_DELAY_LOOP
#define DELAY_CYC_DELAY_LOOP TRUE//FALSE
#endif

#endif

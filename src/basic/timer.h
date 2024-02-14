
#include "peripheral.h"
#ifndef TIMER_H
#define TIMER_H

#define SYSTEM_TIMER_BASE (SYSTEM_TIMER_OFFSET + PERIPHERAL_BASE)

void timer_init(void);

void timer_set(unsigned int usecs);

void udelay(unsigned int usecs);

typedef struct {
    unsigned int timer0_matched: 1;
    unsigned int timer1_matched: 1;
    unsigned int timer2_matched: 1;
    unsigned int timer3_matched: 1;
    unsigned int reserved: 28;
} timer_control_reg_t;

typedef struct {
    timer_control_reg_t control;
    unsigned int counter_low;
    unsigned int counter_high;
    unsigned int timer0;
    unsigned int timer1;
    unsigned int timer2;
    unsigned int timer3;
} timer_registers_t;

#endif
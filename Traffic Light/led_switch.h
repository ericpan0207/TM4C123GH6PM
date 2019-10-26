#ifndef LED_SWITCH_H
#define LED_SWITCH_H

// p340 in data sheet
#define RCGCGPIO *((unsigned int*) 0x400FE608)
// p663 in data sheet
#define GPIODIR_PORT_F_APB *((unsigned int*) 0x40025400)
// p683 in data sheet
#define GPIODEN_PORT_F_APB *((unsigned int*) 0x4002551C)
// p662 in data sheet
#define GPIODATA_PORT_F_APB *((unsigned int*) 0x400253FC)
// p684 in data sheet
#define GPIOLOCK_PORT_F_APB *((unsigned int*) 0x40025520)
// p685 in data sheet
#define GPIOCR_PORT_F_APB *((unsigned int*) 0x40025524)
// p677 in data sheet
#define GPIOPUR_PORT_F *((unsigned int*) 0x40025510)

// Port F Interrupts
// p664 in data sheet
#define GPIOIS_PORT_F_APB *((unsigned int*) 0x40025404)
// p665 in data sheet
#define GPIOIBE_PORT_F_APB *((unsigned int*) 0x40025408)
// p666 in data sheet
#define GPIOIEV_PORT_F_APB *((unsigned int*) 0x4002540C)
// p667 in data sheet
#define GPIOIM_PORT_F_APB *((unsigned int*) 0x40025410)
// p668 in data sheet
#define GPIOICR_PORT_F_APB *((unsigned int*) 0x4002541C)

// Port F1, F2, F3 respectively
#define RED 0x2
#define BLUE 0x4
#define GREEN 0x8

#endif // LED_SWITCH_H

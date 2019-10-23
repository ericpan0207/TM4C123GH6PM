// p659 GPIO ports, p660 register map
// p340 in data sheet
#ifndef TRAFFIC_LIGHT_H
#define TRAFFIC_LIGHT_H

#define RCGCGPIO *((unsigned int*) 0x400FE608)
// p689 
#define GPIOPCTL_PORT_A_APB *((unsigned int*) 0x4000452C)
// p687
#define GPIOAMSEL_PORT_A_APB *((unsigned int*) 0x40004528)
// p663
#define GPIODIR_PORT_A_APB *((unsigned int*) 0x40004400)
// p671
#define GPIOAFSEL_PORT_A_APB *((unsigned int*) 0x40004420)
// p683
#define GPIODEN_PORT_A_APB *((unsigned int*) 0x4000451C)
// p684
#define GPIOLOCK_PORT_A_APB *((unsigned int*) 0x40004520)
// p685
#define GPIOCR_PORT_A_APB *((unsigned int*) 0x40004524)
// p662
#define GPIODATA_PORT_A_APB *((unsigned int*) 0x400043FC)

// Port A Interrupts
// p664
#define GPIOIS_PORT_A_APB *((unsigned int*) 0x40004404)
// p665
#define GPIOIBE_PORT_A_APB *((unsigned int*) 0x40004408)
// p666
#define GPIOIEV_PORT_A_APB *((unsigned int*) 0x4000440C)
// p667
#define GPIOIM_PORT_A_APB *((unsigned int*) 0x40004410)
// p668 
#define GPIOICR_PORT_A_APB *((unsigned int*) 0x4000441C)


#endif // TRAFFIC_LIGHT_H
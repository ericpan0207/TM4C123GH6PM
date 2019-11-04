#ifndef UART_H
#define UART_H
// UART register map p904

// p344
#define RCGCUART *((unsigned int*) 0x400FE618)
// p918
#define UARTCTL_0 *((unsigned int*) 0x4000C030)
// p914
#define UARTIBRD_0 *((unsigned int*) 0x4000C024)
// p915
#define UARTFBRD_0 *((unsigned int*) 0x4000C028)
// p939
#define UARTCC *((unsigned int*) 0x4000CFC8)
// p916
#define UARTLCRH *((unsigned int*) 0x4000C02C)
// p918
#define UARTCTL *((unsigned int*) 0x4000C030)

// p683
#define GPIODEN_PORT_A_APB *((unsigned int*) 0x4000451C)
// p673
#define GPIOAFSEL_PORT_A_APB *((unsigned int*) 0x40004420)
// p911
#define UARTFR *((unsigned int*) 0x4000C018)
// p906
#define UARTDR *((unsigned int*) 0x4000C000)

#endif
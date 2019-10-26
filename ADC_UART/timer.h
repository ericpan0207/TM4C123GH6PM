// p726 Timer registers map
#ifndef TIMER_H
#define TIMER_H

// p338 in data sheet
#define RCGCTIMER *((unsigned int*) 0x400FE604)
// p737
#define GPTMCTL_0 *((unsigned int*) 0x4003000C)
// p728
#define GPTMCFG_0 *((unsigned int*) 0x40030000)
// p729
#define GPTMTAMR_0 *((unsigned int*) 0x40030004)
// p756
#define GPTMTAILR_0 *((unsigned int*) 0x40030028)
// p750
#define GPTMRIS_0 *((unsigned int*) 0x4003001C)
// p754
#define GPTMICR_0 *((unsigned int*) 0x40030024)
// p744
#define GPTMIMR_0 *((unsigned int*) 0x40030018)
// p142
#define EN0 *((unsigned int*) 0xE000E100)
#define EN1 *((unsigned int*) 0xE000E104)

#endif
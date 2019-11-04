#ifndef ADC_H
#define ADC_H
// p818 ADC Register map

// p254
#define RCC *((unsigned int*) 0x400FE060)
#define RCC2 *((unsigned int*) 0x400FE070)
// p244
#define RIS *((unsigned int*) 0x400FE050)

// p352
#define RCGCADC *((unsigned int*) 0x400FE638)
// p821
#define ADCACTSS_0 *((unsigned int*) 0x40038000)
// p833
#define ADCEMUX_0 *((unsigned int*) 0x40038014)
// p851 (not used)
#define ADCSSMUX_0 *((unsigned int*) 0x40038040)
// p876
#define ADCSSCTL3 *((unsigned int*) 0x400380A4)
// p825
#define ADCIM_0 *((unsigned int*) 0x40038008)
// p845
#define ADCPSSI_0 *((unsigned int*) 0x40038028)
// p823
#define ADCRIS_0 *((unsigned int*) 0x40038004)
// p860
#define ADCSSFIFO3_0 *((unsigned int*) 0x400380A8)
// p829
#define ADCISC_0 *((unsigned int*) 0x4003800C)

#endif
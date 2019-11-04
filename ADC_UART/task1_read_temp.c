#include "timer.h"
#include "led_switch.h"
#include "adc.h"
#include "uart.h"
#include <stdio.h>

// Configure clock speed and timer (Lab 3, Task 1)
void Port_F_Handler(void) {
  // Checks if Port F4 is pressed, Change to 80 Mhz clock and update Timer
  if (((GPIODATA_PORT_F_APB >> 4) & 0x1)  == 0) {
    Timer0_Init(80, 1, 1);
    PLL_Init();
    
  }
  // Checks if Port F0 is pressed, Change to 4 Mhz clock and update Timer
  if ((GPIODATA_PORT_F_APB & 0x1) == 0) {
    Timer0_Init(4, 1, 1);
    PLL_Init_2();
  }
  GPIOICR_PORT_F_APB |= 0x11;
}

char temperature[4];

// Lights up LED according to temperature readings from ADC
void ADC0_Handler(void) {
  ADCPSSI_0 |= 0x8;          // Begin sampling SS3
  while (!ADCRIS_0 & 0x8) {} // Monitor SS3 Interrupt status  
  
  // Convert Voltage to temperature
  int x = ADCSSFIFO3_0;
  float vResult = x * 3.3 / 4096.0;
  float temp = (2.7 - vResult) * 75 - 55;
  printf("%f\n", temp);
  
  // Convert temperature float value to string value
  sprintf(temperature, "%0.2f\n\r", temp);
  
  // Write temperature string to UART while the transmitter is not full
  while (!(UARTFR & 0x20)) {
    for(int i = 0; temperature[i] != '\0'; ++i) {
      UARTDR |= temperature[i];
    }
  }
  
  if (temp < 17) {
    GPIODATA_PORT_F_APB = RED;   // Red
  } else if (temp < 19) {
    GPIODATA_PORT_F_APB = BLUE;  // Blue
  } else if (temp < 21) {
    GPIODATA_PORT_F_APB = 0x6;   // Violet
  } else if (temp < 23) {
    GPIODATA_PORT_F_APB = GREEN; // Green
  } else if (temp < 25) {
    GPIODATA_PORT_F_APB = 0xA;   // Yellow
  } else if (temp < 27) {
    GPIODATA_PORT_F_APB = 0xC;   // Light Blue 
  } else {
    GPIODATA_PORT_F_APB = 0xE;   // White
  }
  
  ADCISC_0 |= 0x8;              // Clear Interrupt Flag for SS3
}
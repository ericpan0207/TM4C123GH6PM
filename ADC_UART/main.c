#include "timer.h"
#include "led_switch.h"

int main()
{ 
  PortF_Init();                    // Enable switches and LED
  board_switch_interrupt_init();   // Enable interrupts for switches
  Timer0_Init(4, 1, 1);            // Configure and enable a 1 second timer based on 4 Mhz clock 
  PLL_Init_2();                    // Initialize 4 Mhz clock
  ADC_Init();                      // Initialize ADC
  UART_Init();                     // Initialize UART
  
  while (1) {}
  return 0;
}

#include "timer.h"
#include "led_switch.h"

// TODO: Should trigger the ADC
// Do this first
void Timer_0A_Handler(void) {
  static int i = 0;
  if (i % 2 == 0) {
    GPIODATA_PORT_F_APB = BLUE;
  } else {
    GPIODATA_PORT_F_APB = 0x0;
  }
  i++;
  GPTMICR_0 |= 0x1;
}

// TODO: Change to modify clock speed when button pressed (Later)
// Controls the timer from a user switch (Lab 2, Task 2.2)
void Port_F_Handler(void) {
  // Checks if Port F4 is pressed, if it is turn on RED led
  if (((GPIODATA_PORT_F_APB >> 4) & 0x1)  == 0) {
    GPTMCTL_0 |= 0x1;           // Enable Timer 1
  }
  // Checks if Port F0 is pressed, if it is turn on GREEN led
  if ((GPIODATA_PORT_F_APB & 0x1) == 0) {
    GPTMCTL_0 &= ~0x1;          // Disable Timer 1
    GPIODATA_PORT_F_APB = RED;
  }
  GPIOICR_PORT_F_APB |= 0x11;
}

// Lab 2, Task 2.2
void blink_blue_interrupt(void) {
  PortF_Init();
  board_switch_interrupt_init();
  Timer0_Init(1, 1);             // Start 1 second Timer for blinking blue led
  
  while (1) {}
} 
#include "led_switch.h"
#include "timer.h"
#include <stdint.h>

// Lab 2, Task 1.1
void cycle_through_colors(void) {
  enable_GPIO();
  GPIODEN_PORT_F_APB = RED | BLUE | GREEN; // Enable digital pins PF1, PF2, PF3
  
  enable_timer_0(1, 0);
  int i = 0;
  
  // Set ports f1-f3 on and off depending on which color to display
  while (1) {
    if (GPTMRIS_0 & 0x1) {
      GPIODATA_PORT_F_APB = i;
      GPTMICR_0 |= 0x1;        // Reset Timer
      i = (i + 2) % 16;
    }
  }
}

// Lab 1 : Task 1
void turn_on_led_with_switch(void) {
  enable_GPIO();
  board_switch_init();

  while (1) {
    // Checks if Port F4 is pressed, if it is turn on RED led
    if (((GPIODATA_PORT_F_APB >> 4) & 0x1)  == 0) {
      GPIODATA_PORT_F_APB = RED;
    }
    // Checks if Port F0 is pressed, if it is turn on GREEN led
    if ((GPIODATA_PORT_F_APB & 0x1) == 0) {
      GPIODATA_PORT_F_APB = GREEN;
    }
    // otherwise no light is on by default
    GPIODATA_PORT_F_APB = 0x0;
  }
} 
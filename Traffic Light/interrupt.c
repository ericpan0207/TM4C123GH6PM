#include "led_switch.h"
#include "timer.h"
#include "traffic_light.h"
#include <stdio.h>

// Cycles through all colors on the tiva board
void Timer_0A_Handler(void) {
  static int i = 0;
  GPIODATA_PORT_F_APB = i;
  i = (i + 2) % 16;
  GPTMICR_0 |= 0x1;         // Clear Interrupt flag
  
}

// Lab 2, Task 2.1
void cycle_through_colors_using_interrupt(void) {
  enable_GPIO();
  // Enable digital pins PF1, PF2, PF3
  GPIODEN_PORT_F_APB = RED | BLUE | GREEN;
  
  enable_timer_0(1, 1);
  while (1) {}
}

// Blinks a blue LED
void Timer_1A_Handler(void) {
  static int i = 0;
  if (i % 2 == 0) {
    GPIODATA_PORT_F_APB = BLUE;
  } else {
    GPIODATA_PORT_F_APB = 0x0;
  }
  i++;
  GPTMICR_1 |= 0x1;
}

// Controls the timer from a uswer switch (Lab 2, Task 2.2)
void Port_F_Handler(void) {
  // Checks if Port F4 is pressed, if it is turn on RED led
  if (((GPIODATA_PORT_F_APB >> 4) & 0x1)  == 0) {
    GPTMCTL_1 |= 0x1;           // Enable Timer 1
  }
  // Checks if Port F0 is pressed, if it is turn on GREEN led
  if ((GPIODATA_PORT_F_APB & 0x1) == 0) {
    GPTMCTL_1 &= ~0x1;          // Disable Timer 1
    GPIODATA_PORT_F_APB = RED;
  }
  GPIOICR_PORT_F_APB |= 0x11;
}

// Lab 2, Task 2.2
void blink_blue_interrupt(void) {
  enable_GPIO();
  board_switch_init();
  board_switch_interrupt_init();
  enable_timer_1(1, 1);             // Start 1 second Timer for blinking blue led
  
  while (1) {}
} 

int start_button = 0;               // Keep track of if start button was pushed
int pedestrian_button = 0;          // Keep track of if pedestrian button was pushed

// Interrupt for Traffic Light: Default state once system is started
void Timer_2A_Handler(void) {
  setup_timer_2(5, 1);        // Set timer to 5 seconds
  // Alternate between Red and Green
  if (is_on('R')) {
    LED_on('G');
  } else {
    LED_on('R');
  }
  GPTMICR_2|= 0x1;            // Clear Interrupt Flag  

}

// Interrupt for external switches
void Port_A_Handler(void) {
  static int i = 0;                 // Keep track of start/stop button presses
  if (start_button) {
      if (i % 2 == 0) {
        setup_timer_2(0, 1);        // Initialize timer 2A
        GPTMCTL_2 &= ~0x1;          // Enable Timer 2A
      } else {
        GPTMCTL_2 &= ~0x1;          // Disable Timer 2A
        LED_off();
      }
    i++;
    start_button = 0;
  } else if (pedestrian_button && is_on('G')) { // Warning state
    LED_on('Y');
    GPTMCTL_2 &= ~0x1;              // Disable Timer 2A
    enable_timer_0(5, 0);           // Start a 5 second timer
    GPTMICR_0 |= 0x1;               // Reset Timer
    while (!(GPTMRIS_0 & 0x1)) {}   // Wait 5 seconds
    LED_on('R');
  }
  GPIOICR_PORT_A_APB |= 0x18;       // Clear Interrupt Flag for both switches
}

// Lab 2, Task 2.3 
void run_traffic_interrupt(void) {
  traffic_light_init();
  traffic_light_interrupt_init();

  while(1) {
    start_button = switch_input('S');
    pedestrian_button = switch_input('P');
  }
}
#include "traffic_light.h"
#include "timer.h"
#include <stdint.h>
 
// Turn on LED connected to specific pin and turn off everything else
// color : 'R' for red
//       : 'Y' for yellow
//       : 'G' for green
// Note: will turn on everything if color is not any of the above
void LED_on(char color) {
  if (color == 'R') {
    GPIODATA_PORT_A_APB = 0x20;  
  } else if (color == 'Y') {
    GPIODATA_PORT_A_APB = 0x40;
  } else if (color == 'G') {
    GPIODATA_PORT_A_APB = 0x80;
  } else {   
    GPIODATA_PORT_A_APB |= 0xE0;  
  }
}

// Turn off LED connected to PA5, PA6, PA7
void LED_off(void) {
  GPIODATA_PORT_A_APB &= ~0xE0;
}

// Checks if a switch is pressed for at least 2 seconds
// button : 'S' for start/stop button
//          'P' for pedestrian button
unsigned long switch_input(char button) {
  // PA3 : 0x8
  // PA4 : 0x10
  enable_timer_1(2, 0);
  GPTMICR_1 |= 0x1; // Reset Timer
  if (button == 'S' && GPIODATA_PORT_A_APB & 0x10) {
    while (!(GPTMRIS_1 & 0x1)) {
      if (!(GPIODATA_PORT_A_APB & 0x10)) {
        return 0;
      }
    }
    return (GPIODATA_PORT_A_APB & 0x10); // 0x10 (pressed) or 0 (not pressed) 
  } else if (button == 'P' && GPIODATA_PORT_A_APB & 0x8) {
    while (!(GPTMRIS_1 & 0x1)) {
      if (!(GPIODATA_PORT_A_APB & 0x08)) {
        return 0;
      }
    }
    return (GPIODATA_PORT_A_APB & 0x8); // 0x8 (pressed) or 0 (not pressed) 
  }
  return 0;
}

// Checks if a specific led is on
// color : 'G' for green
//       : 'R' for red
//       : 'Y' for yellow
// Returns >0 if true, 0 otherwise
unsigned long is_on(char color) {
  if (color == 'G') {
    return (GPIODATA_PORT_A_APB & 0x80); // 0x80 (true) or 0 (false) 
  } else if (color == 'R') {
    return (GPIODATA_PORT_A_APB & 0x20); // 0x10 (true) or 0 (false) 
  } else if (color == 'Y') {
    return (GPIODATA_PORT_A_APB & 0x40); // 0x40 (true) or 0 (false)
  }
  return 0;
}

// Start/stop button control
// pedestrian : whether or not the pedestrian button has been pressed
//            : 0 for no, 1 for yes
int start_stop_system(int pedestrian) {
  static unsigned int count = 0;
  // Start/Stop Button
  if (switch_input('S')) {
    count++;
    // Every odd presses, Red led will light up signifying system start
    if (count % 2 != 0) {
      LED_on('R');
      return 0;
    } else {
      LED_off();
    }
  }
  return pedestrian;
}

// Stop State of the traffic light
// Will switch LED to green after 5 seconds if pedestrian button hasn't been pressed
int stop_state(int pedestrian) {
  enable_timer_0(5, 0);
  GPTMICR_0 |= 0x1; // Reset Timer
  while (!(GPTMRIS_0 & 0x1)) {
    pedestrian = start_stop_system(pedestrian);
  }
  if (is_on('R')) {
    LED_on('G');
  }
  return pedestrian;
}

// Go State of the traffic light
// Checks whether pedestrian button is pressed during a 5 second window
// if true: switch to warning state and then stop state
//   false: switch to stop state
int go_state(int pedestrian) {
  // Reset 5 second timer and check if pedestrian button is pressed
  GPTMICR_0 |= 0x1;
  while (!(GPTMRIS_0 & 0x1)) {
    // Pedestrian button
    if (switch_input('P')) {
      pedestrian = 1;
      LED_on('Y');
      enable_timer_0(5, 0);
    }
    pedestrian = start_stop_system(pedestrian);
  }
  LED_on('R');
  return pedestrian;
}

void run_traffic(void) {
  traffic_light_init();
  enable_timer_0(5, 0);

  volatile unsigned int pedestrian = 0;
  
  while(1) {
    pedestrian = start_stop_system(pedestrian);
    // Alternate between stop and go state, taking Pedestrian button into consideration
    if (is_on('R') && !pedestrian) {
      pedestrian = stop_state(pedestrian);
    } else if (is_on('G')) {
      pedestrian = go_state(pedestrian);
    }
  }
}
               

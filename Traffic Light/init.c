#include "led_switch.h"
#include "traffic_light.h"
#include "timer.h"

// Enable Port F and RGB leds 
void enable_GPIO(void) {  
  RCGCGPIO = 0x20;                         // Enable and provide a clock to GPIO Port F 
  GPIODIR_PORT_F_APB = RED | GREEN | BLUE; // Set direction of port for RBG LEDs to output
}

// Enables pins 4 and 0 on the board
void board_switch_init(void) {
  GPIOLOCK_PORT_F_APB = 0x4C4F434B; // Enable write access to the GPIOCR register
  GPIOCR_PORT_F_APB = 0x11;         // Enable write to pins 4 and 0 (sw1 and sw2 respectively)
  GPIODEN_PORT_F_APB = 0x1F;        // Enable digital pins PF0, PF1, PF2, PF3, PF4
  GPIOPUR_PORT_F = 0x11;            // Enable pull up register for pins 4 and 0
}

// Enables interrupts for switches on the tiva board
void board_switch_interrupt_init() {
  GPIOIS_PORT_F_APB &= ~0x11;       // Edge-sensitive interrupt for pins 4 and 0
  GPIOIBE_PORT_F_APB &= ~0x11;      // Pass Interrupt control to GPIOIEV register
  GPIOIEV_PORT_F_APB &= ~0x11;      // Low level edge trigger for pins 4 and 0
  GPIOIM_PORT_F_APB |= 0x11;        // Allow interrupt for pins 4 and 0
  EN0 |= 0x40000000;                // Enable Interrupt for port F
}

void enable_port_a(void) {  
  volatile unsigned long delay;  
  RCGCGPIO |= 0x1;  // Enable and provide a clock to GPIO Port A  
  delay = RCGCGPIO; // Allow time for clock to start
}

void LED_init(void) {    
  GPIOPCTL_PORT_A_APB &= ~0xFFF00000; // Regular GPIO for ports PA5, PA6, PA7
  GPIOAMSEL_PORT_A_APB &= ~0xE0;      // Disable analog of PA5, PA6, PA7  
  GPIODIR_PORT_A_APB |= 0xE0;         // set PA5, PA6, PA7 to output
  GPIOAFSEL_PORT_A_APB &= ~0xE0;      // Regular port function for PA5, PA6, PA7
  GPIODEN_PORT_A_APB |= 0xE0;         // Enable digital output for PA5, PA6, PA7
}

void switch_init(void) {  
  GPIOAMSEL_PORT_A_APB &= ~0x18;      // Disable analog of PA3, PA4
  GPIOPCTL_PORT_A_APB &= ~0x000FF00;  // Regular GPIO for ports PA3, PA4
  GPIODIR_PORT_A_APB &= ~0x18;        // Set PA2, PA3 to input
  GPIOAFSEL_PORT_A_APB &= ~0x18;      // Regular port function for PA3, PA4
  GPIODEN_PORT_A_APB |= 0x18;         // Enable digital output for PA4, PA4
}

void traffic_light_init(void) {
  enable_port_a();
  LED_init();
  switch_init();
}

void traffic_light_interrupt_init(void) {
  GPIOIS_PORT_A_APB &= ~0x18;       // Edge-sensitive interrupt for pins 3 and 4
  GPIOIBE_PORT_A_APB &= ~0x18;      // Pass Interrupt control to GPIOIEV register
  GPIOIEV_PORT_A_APB &= ~0x18;      // Low level edge trigger for pins 3 and 4
  GPIOIM_PORT_A_APB |= 0x18;        // Allow interrupt for pins 3 and 4
  EN0 |= 0x1;                       // Enable Interrupt for Port A
}

void enable_timer_0(int second, int interrupt) {
  RCGCTIMER |= 0x1;        // Enable Timer 0  
  GPTMCTL_0 &= ~0x1;       // Disable Timer A
  GPTMCFG_0 = 0x0;         // Select 32-bit mode
  GPTMTAMR_0 |= 0x2;       // Periodic Mode
  GPTMTAMR_0 &= ~(1 << 4); // Count Down Mode
  GPTMTAILR_0 = 16000000 * second;  // 1 Hz blink rate
  if (interrupt) {
    GPTMIMR_0 |= 0x1;      // Enable Interrupt
    EN0 |= 0x80000;
  }
  GPTMCTL_0 |= 0x1;        // Enable Timer A
}

void enable_timer_1(int second, int interrupt) {
  RCGCTIMER |= 0x2;
  GPTMCTL_1 &= ~0x1;
  GPTMCFG_1 = 0x0;         
  GPTMTAMR_1 |= 0x2;       
  GPTMTAMR_1 &= ~(1 << 4);
  GPTMTAILR_1 = 16000000 * second;
  if (interrupt) {
    GPTMIMR_1 |= 0x1;      // Enable Interrupt
    EN0 |= 0x200000;
  }
  GPTMCTL_1 |= 0x1;
}

void setup_timer_2(int second, int interrupt) {
  RCGCTIMER |= 0x4;
  GPTMCTL_2 &= ~0x1;
  GPTMCFG_2 = 0x0;         
  GPTMTAMR_2 |= 0x2;       
  GPTMTAMR_2 &= ~(1 << 4);
  GPTMTAILR_2 = 16000000 * second;
  if (interrupt) {
    GPTMIMR_2 |= 0x1;      // Enable Interrupt
    EN0 |= 0x800000;
  }
  GPTMCTL_2 |= 0x1;  
}

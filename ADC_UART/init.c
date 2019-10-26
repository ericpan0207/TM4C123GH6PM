#include "led_switch.h"
#include "timer.h"

// Configure GPIO Port F pins (board LEDs + switches)
void PortF_Init(void) {
  RCGCGPIO = 0x20;                         // Enable and provide a clock to GPIO Port F 
  GPIODIR_PORT_F_APB = RED | GREEN | BLUE; // Set direction of port for RBG LEDs to output
  GPIOLOCK_PORT_F_APB = 0x4C4F434B;        // Enable write access to the GPIOCR register
  GPIOCR_PORT_F_APB = 0x11;                // Enable write to pins 4 and 0 (sw1 and sw2 respectively)
  GPIODEN_PORT_F_APB = 0x1F;               // Enable digital pins PF0, PF1, PF2, PF3, PF4
  GPIOPUR_PORT_F = 0x11;                   // Enable pull up register for pins 4 and 0
}

// Enables interrupts for switches on the board
void board_switch_interrupt_init(void) {
  GPIOIS_PORT_F_APB &= ~0x11;       // Edge-sensitive interrupt for pins 4 and 0
  GPIOIBE_PORT_F_APB &= ~0x11;      // Pass Interrupt control to GPIOIEV register
  GPIOIEV_PORT_F_APB &= ~0x11;      // Low level edge trigger for pins 4 and 0
  GPIOIM_PORT_F_APB |= 0x11;        // Allow interrupt for pins 4 and 0
  EN0 |= 0x40000000;                // Enable Interrupt for port F
}

// Initialize a timer
// Timer will count down for [seconds] number of seconds
// interrupt : 1 to enable interrupt
//           : 0 to not use interrupt
void Timer0_Init(int seconds, int interrupt) {
  RCGCTIMER |= 0x1;        // Enable Timer 0  
  GPTMCTL_0 &= ~0x1;       // Disable Timer A
  GPTMCFG_0 = 0x0;         // Select 32-bit mode
  GPTMTAMR_0 |= 0x2;       // Periodic Mode
  GPTMTAMR_0 &= ~(1 << 4); // Count Down Mode
  GPTMTAILR_0 = 16000000 * seconds;  // 1 Hz blink rate
  if (interrupt) {
    GPTMIMR_0 |= 0x1;      // Enable Interrupt
    EN0 |= 0x80000;
  }
  GPTMCTL_0 |= 0x1;        // Enable Timer A
}



void PLL_Init(void) {
  
}

void ADC_Init(void) {
  
}
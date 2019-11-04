#include "led_switch.h"
#include "timer.h"
#include "adc.h"
#include "uart.h"
#include <stdio.h>

void delay(void) {
  for (int i = 0; i < 1000000; i++) {}
}

// Configure GPIO Port F pins (board LEDs + switches)
void PortF_Init(void) {
  RCGCGPIO = 0x20;                         // Enable and provide a clock to GPIO Port F 
  delay();
  GPIODIR_PORT_F_APB = RED | GREEN | BLUE; // Set direction of port for RBG LEDs to output
  GPIOLOCK_PORT_F_APB = 0x4C4F434B;        // Enable write access to the GPIOCR register
  GPIOCR_PORT_F_APB = 0x11;                // Enable write to pins 4 and 0 (sw1 and sw2 respectively)
  GPIODEN_PORT_F_APB = 0xFF;               // Enable digital pins PF0-PF7
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
// Speed of clock in Mhz
// Timer will count down for [seconds] number of seconds
// interrupt : 1 to enable interrupt
//           : 0 to not use interrupt
void Timer0_Init(int sys_clock, int seconds, int interrupt) {
  RCGCTIMER |= 0x1;        // Enable Timer 0  
  delay();
  GPTMCTL_0 &= ~0x1;       // Disable Timer A
  GPTMCFG_0 = 0x0;         // Select 32-bit mode
  GPTMTAMR_0 |= 0x2;       // Periodic Mode
  GPTMTAMR_0 &= ~(1 << 4); // Count Down Mode
  GPTMTAILR_0 = sys_clock * 1000000 * seconds;  // 1 Hz blink rate
  if (interrupt) {
    GPTMIMR_0 |= 0x1;      // Enable Interrupt
    EN0 |= 0x20000;
  }
  GPTMCTL_0 |= 0x20;       // Enable ADC Trigger 
  GPTMCTL_0 |= 0x1;        // Enable Timer A
}

// Configure a 80 Mhz clock using PLL
void PLL_Init(void) {
  // "Disabling" PLL so we can configure
  RCC |= 0x800;             // PLL Bypass 
  RCC &= ~400000;           // Clear Usesys bit
  
  RCC &= ~0x7C0;            // Clear XTAL bits
  RCC |= 0x540;             // Set XTAL to 16 Mhz clock
  RCC &= ~0x30;             // Set OSCSRC to Main oscillator
  RCC &= ~0x2000;           // Activate PLL (PWRDN)
  RCC &= ~0x7800000;        // clear sysdiv 
  RCC |= 2000000;           // set sysdiv value to 4 to get 80 Mhz
  RCC |= 400000;            // Use sys div enabled
  
  RCC2 &= ~0x70;            // Set OSCSRC2 to Main oscillator
  RCC2 &= ~0x2000;          // Activate PLL (PWRDN2)
  RCC2 |= 40000000;         // Use 400 Mhz PLL output
  RCC2 &= ~0x1FC00000;      // Clear sysdiv2
  RCC2 |= 1000000;          // sysdiv2 of value: 4 to get 80 Mhz
  RCC2 |= 0x80000000;       // Use RCC2 register fields (USERCC2)
  
  while (!RIS & 0x40) {}    // Wait for PLL to lock,    need to clear?
  RCC2 &= ~0x800;           // Enable PLL (Bypass2)
}

// Configure a 4 Mhz clock using PLL
void PLL_Init_2(void) {
  // "Disabling" PLL so we can configure
  RCC |= 0x800;             // PLL Bypass 
  RCC &= ~400000;           // Clear Usesys bit
  
  RCC &= ~0x7C0;            // Clear XTAL bits
  RCC |= 0x540;             // Set XTAL to 16 Mhz clock
  RCC &= ~0x30;             // Set OSCSRC to Main oscillator
  RCC &= ~0x2000;           // Activate PLL (PWRDN)
  RCC &= ~0x7800000;        // clear sysdiv 
  RCC |= 2000000;           // set sysdiv value to 4 to get 80 Mhz
  RCC |= 400000;            // Use sys div enabled
  
  RCC2 &= ~0x70;            // Set OSCSRC2 to Main oscillator
  RCC2 &= ~0x2000;          // Activate PLL (PWRDN2)
  RCC2 |= 40000000;         // Use 400 Mhz PLL output
  RCC2 &= ~0x1FC00000;      // Clear sysdiv2
  RCC2 |= 0x18C00000;       // sysdiv2 of value: 99 to get 4 Mhz
  RCC2 |= 0x80000000;       // Use RCC2 register fields (USERCC2)
  
  while (!RIS & 0x40) {}    // Wait for PLL to lock
  RCC2 &= ~0x800;           // Enable PLL (Bypass2)
}

void ADC_Init(void) {
  RCGCADC |= 0x1;           // Enable and provide clock to ADC module 0
  delay();
  ADCACTSS_0 &= ~0x8;       // Disable Sample Sequencer 3       
  ADCEMUX_0 |= 0x5000;      // Timer Trigger for Sample Sequencer 3
  ADCSSCTL3 |= 0xE;         // Temp sensor, Interrupt enable for SS3
  ADCIM_0 |= 0x8;           // SS3 interrupt sent to interrupt controller
  ADCACTSS_0 |= 0x8;        // Enable SS3
}

void UART_Init(void) {
  RCGCUART |= 0x1;          // Enable UART module 0
  delay();
  RCGCGPIO |= 0x1;          // Enable Port A
  UARTCTL_0 &= ~0x1;        // Disable UART0
  UARTIBRD_0 |= 0x68;       // Set Baud Rate (integer portion)
  UARTFBRD_0 |= 0xB;        // Set Baud Rate (fraction portion)
  UARTCC |= 0x5;            // Use PIOSC clock
  UARTLCRH |= 0x70;         // Configure line control
  UARTCTL |= 0x301;         // Enable Transmit and Recieve for UART
  GPIODEN_PORT_A_APB |= 0x3; // Enable digital pins 0 and 1 
  GPIOAFSEL_PORT_A_APB |= 0x3; // Alt function for pins 0 and 1
}

#include "timer.h"
#include "led_switch.h"

int main()
{
  //blink_blue_interrupt();   The en0 bit for the interrupt was changed to use adc handler instead of timer handler
  
  
  PortF_Init();
  board_switch_interrupt_init();
  Timer0_Init(4, 1, 1);  
  PLL_Init_2();
  ADC_Init();
  
  int i = 0;
  while (1) {
    if (GPTMRIS_0 & 0x1) {
      GPIODATA_PORT_F_APB = i;
      GPTMICR_0 |= 0x1;        // Reset Timer
      i = (i + 2) % 16;
    }
  
  }
  return 0;
}

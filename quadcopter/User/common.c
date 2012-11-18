#include "stm32f10x.h"
#include "hardware_conf.h"
#include "common.h"

void delay_ms(uint32_t ms)
{
  SysTick->LOAD  = (SystemCoreClock / 1000) * ms - 1;      /* set reload register */
  SysTick->VAL   = 0;                                          /* Load the SysTick Counter Value */
  SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk |
                   SysTick_CTRL_ENABLE_Msk;                    /* Enable SysTick IRQ and SysTick Timer */
  
   while(!((SysTick->CTRL)&SysTick_CTRL_COUNTFLAG_Msk));
  
  SysTick->CTRL=0x00;
}

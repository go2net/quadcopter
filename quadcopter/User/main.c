
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "common.h"
#include "CC2520.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void RCC_Configuration(void);
void LED_Configuration(void);
/* Private functions ---------------------------------------------------------*/

int main(void)
{
  /* Add your application code here */
  RCC_Configuration();
  LED_Configuration();
  CC2520_ClkOut();
    
  /* Infinite loop */
  while (1)
  {    
  }
}

void RCC_Configuration(void)
{
  /* GPIOA, GPIOB, GPIOC, GPIOD and AFIO clocks enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB|
                         RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD|
						 RCC_APB2Periph_AFIO, ENABLE);
  
  GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
}

void LED_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
        GPIO_InitStructure.GPIO_Pin = LED_Red_Pin;
	GPIO_Init(LED_Red_Port, &GPIO_InitStructure);
        
        GPIO_InitStructure.GPIO_Pin = LED_Green_Pin;
	GPIO_Init(LED_Green_Port, &GPIO_InitStructure);
        
        GPIO_InitStructure.GPIO_Pin = LED_Blue_Pin;
	GPIO_Init(LED_Blue_Port, &GPIO_InitStructure);
}
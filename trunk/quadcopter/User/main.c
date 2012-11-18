
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "common.h"
#include "CC2520.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
TIM_OCInitTypeDef  TIM_OCInitStructure;
uint16_t CCR1_Val = 2047;
uint16_t CCR2_Val = 1535;
uint16_t CCR3_Val = 1023;
uint16_t CCR4_Val = 511;
/* Private function prototypes -----------------------------------------------*/
void System_Init(void);
void RCC_Configuration(void);
void LED_Configuration(void);
void NVIC_Configuration(void);
void PWM_Configuration(void);
/* Private functions ---------------------------------------------------------*/

int main(void)
{
  System_Init(); 
  PWM_Configuration();
  //IIC_Configuration();
  
  
  

  
  
  
  /* Infinite loop */
  while (1)
  {   
    LED_Red_Reset();
    delay_ms(230);
    delay_ms(230);
    LED_Red_Set();
    LED_Green_Reset();
    delay_ms(230);
    delay_ms(230);    
    LED_Green_Set();
    LED_Blue_Reset();
    delay_ms(230);
    delay_ms(230);    
    LED_Blue_Set();

  }
}

void System_Init(void)
{
  /* GPIOA, GPIOB, GPIOC, GPIOD and AFIO clocks enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB|
                         RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD|
						 RCC_APB2Periph_AFIO, ENABLE);
  GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
  LED_Configuration();
  CC2520_ClkOut();
  RCC_Configuration();
  NVIC_Configuration();
}

void RCC_Configuration(void)
{
  ErrorStatus HSEStartUpStatus;
  /* Enable HSE */  
  RCC_HSEConfig(RCC_HSE_Bypass); 
  /* Wait till HSE is ready */
  HSEStartUpStatus = RCC_WaitForHSEStartUp();  
  if (HSEStartUpStatus == SUCCESS)
  {  
    /* Enable Prefetch Buffer */
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
    /* Flash 2 wait state */
    FLASH_SetLatency(FLASH_Latency_2); 
    /* HCLK = SYSCLK */
    RCC_HCLKConfig(RCC_SYSCLK_Div1);  
    /* PCLK2 = HCLK */
    RCC_PCLK2Config(RCC_HCLK_Div1); 
    /* PCLK1 = HCLK/2 */
    RCC_PCLK1Config(RCC_HCLK_Div2);    
    /* PLLCLK = 8MHz * 9 = 72 MHz */
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
    /* Enable PLL */ 
    RCC_PLLCmd(ENABLE);  
    /* Wait till PLL is ready */
    while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET){}
    /* Select PLL as system clock source */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
    /* Wait till PLL is used as system clock source */
    while(RCC_GetSYSCLKSource() != 0x08){}   

    SystemCoreClockUpdate();
    /* Enable Clock Security System(CSS): this will generate an NMI exception
     when HSE clock fails */
    RCC_ClockSecuritySystemCmd(ENABLE);   
  }
}

/**
  * @brief  Configures Vector Table base location.
  * @param  None
  * @retval None
  */

void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Enable and configure RCC global IRQ channel */
  NVIC_InitStructure.NVIC_IRQChannel = RCC_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

void PWM_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Pin = Motor_1_Pin | Motor_2_Pin |
                                Motor_3_Pin | Motor_4_Pin;
  GPIO_Init(Motor_Port, &GPIO_InitStructure); 
  
  /* TIM4 Configuration ---------------------------------------------------
   TIM4CLK = 36 MHz, Prescaler = 0, TIM4 counter clock = 36 MHz
   TIM4 frequency = TIM4CLK/(TIM4_Period + 1) = 8.785 KHz
  - TIM4 Channel1 = TIM4->CCR1 / (TIM4_Period + 1) = 50% 
  - TIM4 Channel2 = TIM4->CCR2 / (TIM4_Period + 1) = 37.5% 
  - TIM4 Channel3 = TIM4->CCR3 / (TIM4_Period + 1) = 25%
  - TIM4 Channel4 = TIM4->CCR4 / (TIM4_Period + 1) = 12.5% 
  ----------------------------------------------------------------------- */

  /* Time Base configuration */
  TIM_TimeBaseStructure.TIM_Prescaler = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseStructure.TIM_Period = 4095;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

  /* Channel 1, 2,3 and 4 Configuration in PWM mode */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
  TIM_OCInitStructure.TIM_Pulse = CCR1_Val;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
  TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
  TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;

  TIM_OC1Init(TIM4, &TIM_OCInitStructure);

  TIM_OCInitStructure.TIM_Pulse = CCR2_Val;
  TIM_OC2Init(TIM4, &TIM_OCInitStructure);

  TIM_OCInitStructure.TIM_Pulse = CCR3_Val;
  TIM_OC3Init(TIM4, &TIM_OCInitStructure);

  TIM_OCInitStructure.TIM_Pulse = CCR4_Val;
  TIM_OC4Init(TIM4, &TIM_OCInitStructure);

  /* TIM4 counter enable */
  TIM_Cmd(TIM4, ENABLE);

  /* TIM4 Main Output Enable */
  TIM_CtrlPWMOutputs(TIM4, ENABLE);    
}

void LED_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  //LED Off
  LED_Red_Set();
  LED_Green_Set();
  LED_Blue_Set();

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
  GPIO_InitStructure.GPIO_Pin = LED_Red_Pin;
  GPIO_Init(LED_Red_Port, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = LED_Green_Pin;
  GPIO_Init(LED_Green_Port, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = LED_Blue_Pin;
  GPIO_Init(LED_Blue_Port, &GPIO_InitStructure);
}


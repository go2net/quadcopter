#ifndef __HARDWARE_CONF_H
#define __HARDWARE_CONF_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
   
#define LED_Red_Pin GPIO_Pin_13
#define LED_Red_Port GPIOC
#define LED_Red_Set() {LED_Red_Port->BSRR = LED_Red_Pin;}
#define LED_Red_Reset() {LED_Red_Port->BRR = LED_Red_Pin;}

#define LED_Green_Pin GPIO_Pin_2
#define LED_Green_Port GPIOA
#define LED_Green_Set() {LED_Green_Port->BSRR = LED_Green_Pin;}
#define LED_Green_Reset() {LED_Green_Port->BRR = LED_Green_Pin;}

#define LED_Blue_Pin GPIO_Pin_7
#define LED_Blue_Port GPIOA
#define LED_Blue_Set() {LED_Blue_Port->BSRR = LED_Blue_Pin;}
#define LED_Blue_Reset() {LED_Blue_Port->BRR = LED_Blue_Pin;}   

/****************************   CC2520   **************************************/
   
#define CC2520_REGEN_Pin GPIO_Pin_8
#define CC2520_REGEN_Port GPIOA   
#define CC2520_REGEN_Set() {CC2520_REGEN_Port->BSRR = CC2520_REGEN_Pin;}
#define CC2520_REGEN_Reset() {CC2520_REGEN_Port->BRR = CC2520_REGEN_Pin;}

#define CC2520_nRST_Pin GPIO_Pin_5
#define CC2520_nRST_Port GPIOC
#define CC2520_nRST_Set() {CC2520_nRST_Port->BSRR = CC2520_nRST_Pin;}
#define CC2520_nRST_Reset() {CC2520_nRST_Port->BRR = CC2520_nRST_Pin;}   

#define CC2520_nCS_Pin GPIO_Pin_12
#define CC2520_nCS_Port GPIOB 
#define CC2520_nCS_Set() {CC2520_nCS_Port->BSRR = CC2520_nCS_Pin;}
#define CC2520_nCS_Reset() {CC2520_nCS_Port->BRR = CC2520_nCS_Pin;}   
 
#define CC2520_SCLK_Pin GPIO_Pin_13
#define CC2520_SCLK_Port GPIOB      
   
#define CC2520_MISO_Pin GPIO_Pin_14
#define CC2520_MISO_Port GPIOB    
#define CC2520_MISO_Set() {CC2520_MISO_Port->BSRR = CC2520_MISO_Pin;}
#define CC2520_MISO_Reset() {CC2520_MISO_Port->BRR = CC2520_MISO_Pin;}

#define CC2520_MOSI_Pin GPIO_Pin_15
#define CC2520_MOSI_Port GPIOB     
 
#define CC2520_SPI_Port GPIOB
#define CC2520_SPI_SPIn SPI2   
   
   
   
   
#ifdef __cplusplus
}
#endif

#endif /* __HARDWARE_CONF_H */
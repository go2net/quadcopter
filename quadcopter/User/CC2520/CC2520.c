#include "stm32f10x.h"
#include "common.h"
#include "CC2520.h"

/*  
   IMPORTANT NOTE:
   ============== 
   1. After each device reset the HSI is used as System clock source.

   2. Please make sure that the selected System clock doesn't exceed your device's
      maximum frequency.
      
   3. If none of the define below is enabled, the HSI is used as System clock
    source.

   4. The System clock configuration functions provided within this file assume that:
        - For Low, Medium and High density Value line devices an external 8MHz 
          crystal is used to drive the System clock.
        - For Low, Medium and High density devices an external 8MHz crystal is
          used to drive the System clock.
        - For Connectivity line devices an external 25MHz crystal is used to drive
          the System clock.
     If you are using different crystal you have to adapt those functions accordingly.
    */

/***********************************************************************************
* LOCAL FUNCTIONS
*/

static uint8_t CC2520_SPI_TXRX(uint8_t Data)
{
  while((CC2520_SPI_SPIn->SR & SPI_I2S_FLAG_TXE) != SPI_I2S_FLAG_TXE); 
  CC2520_SPI_SPIn->DR = Data;
  while((CC2520_SPI_SPIn->SR & SPI_I2S_FLAG_RXNE) != SPI_I2S_FLAG_RXNE); 
  return CC2520_SPI_SPIn->DR;
}

/***********************************************************************************
* GLOBAL FUNCTIONS
*/

void CC2520_ClkOut(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;  
  
  //MISO As Input Pin
  //The SO pin should not be left floating while in LPM1 or LPM2, 
  //as this will result in higher current consumption than necessary
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Pin = CC2520_MISO_Pin;
  GPIO_Init(CC2520_MISO_Port, &GPIO_InitStructure);  
  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  //nCS as Output Pin
  CC2520_nCS_Set(); 
  GPIO_InitStructure.GPIO_Pin = CC2520_nCS_Pin;
  GPIO_Init(CC2520_nCS_Port, &GPIO_InitStructure); 
  //REGEN as Output Pin
  GPIO_InitStructure.GPIO_Pin = CC2520_REGEN_Pin;
  GPIO_Init(CC2520_REGEN_Port, &GPIO_InitStructure);
  //nRST as Output Pin
  GPIO_InitStructure.GPIO_Pin = CC2520_nRST_Pin;
  GPIO_Init(CC2520_nRST_Port, &GPIO_InitStructure);
  
  /**********Change from LPM2 mode to Active mode**********/  
  //Set VREG_EN = 1
  CC2520_REGEN_Set();
  //Set RESETn = 0
  CC2520_nRST_Reset();
  delay_ms(1);
  //Set RESETn = 1
  CC2520_nRST_Set();
  delay_ms(1);
  //Set CSn = 0
  CC2520_nCS_Reset();
  //Wait until SO = 1
  while(!(CC2520_MISO_Port->IDR & CC2520_MISO_Pin));
  //Set CSn = 1
  CC2520_nCS_Set();  
  /********************************************************/ 
  
  CC2520_SPI_Init();
  CC2520_MEMWR8(CC2520_EXTCLOCK, 0x3C); 
    
}

/***********************************************************************************
* @fn      CC2520_SPI_Init
*
* @brief   Init SPI
*
* @param  none
*         none
*
* @return  none
*/
void CC2520_SPI_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;  
  SPI_InitTypeDef  SPI_InitStructure;   
  
  /* SPI2 Periph clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
  
  //Config SPI Ports
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Pin = CC2520_SCLK_Pin | 
                                CC2520_MISO_Pin | CC2520_MOSI_Pin;
  GPIO_Init(CC2520_SPI_Port, &GPIO_InitStructure);  
  
  //Config SPI
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(SPI2, &SPI_InitStructure);
  
  /* Enable SPI2 */
  SPI_Cmd(SPI2, ENABLE);    
}

/***********************************************************************************
* @fn      CC2520_MEMWR8
*
* @brief   Write memory 8 bits
*
* @param   uint16 addr
*          uint8 value
*
* @return  uint8 - status byte
*/
uint8_t CC2520_MEMWR8(uint16_t addr, uint8_t value)
{
    uint8_t s;
    CC2520_nCS_Reset();
    s = CC2520_SPI_TXRX(CC2520_INS_MEMWR | HI_UINT16(addr));
    CC2520_SPI_TXRX(LO_UINT16(addr));
    CC2520_SPI_TXRX(value);
    CC2520_nCS_Set(); 
    return s;
}


/***********************************************************************************
* @fn      CC2520_REGWR8
*
* @brief   Write one register byte
*
* @param  uint8 addr - address
*         uint8 value
*
* @return  none
*/
void CC2520_REGWR8(uint8_t addr, uint8_t value)
{
    CC2520_nCS_Reset();
    CC2520_SPI_TXRX(CC2520_INS_REGWR | addr);
    CC2520_SPI_TXRX(value);
    CC2520_nCS_Set(); 
}

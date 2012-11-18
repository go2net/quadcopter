#include "MS5611.h"
#include "IIC.h"

static struct MS5611_Coff
{
  uint16_t MS5611_Coff[6];
  uint16_t MS5611_CRC;
}imCoff;

static void MS5611_Send_Command(uint8_t cmd)
{
  /* Send STRAT condition */
  IIC_Start();
  /* Test on EV5 and clear it */
  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT)); 
  /* Send device address for write */
  IIC_Send_Byte((MS5611_ADDRESS | MS5611_IIC_WRITE));
  /* Test on EV6 and clear it */
  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
  /* Send Command */
  IIC_Send_Byte(cmd);   
  /* Test on EV8 and clear it */
  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED)); 
  /* Send STOP condition */  
  IIC_Stop();
}

static void MS5611_Read_Bytes(uint8_t* pBuffer, uint32_t NumByteToRead)
{
  /* Send STRAT condition */
  IIC_Start();
  /* Test on EV5 and clear it */
  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT)); 
  /* Send device address for write */
  IIC_Send_Byte((MS5611_ADDRESS | MS5611_IIC_READ));
  /* Test on EV6 and clear it */
  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

  while(NumByteToRead)  
  {
    if(NumByteToRead == 1)
    {
      /* Disable Acknowledgement */
      I2C_AcknowledgeConfig(I2C2, DISABLE);      
      /* Send STOP Condition */
      IIC_Stop();
    }
    /* Test on EV7 and clear it */
    if(I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED))  
    {      
      /* Read a byte from the EEPROM */
      *pBuffer = IIC_Receive_Byte();
      /* Point to the next location where the byte read will be saved */
      pBuffer++;     
      /* Decrement the read bytes counter */
      NumByteToRead--;        
    }   
  }  
  I2C_AcknowledgeConfig(I2C2, ENABLE);
}

void MS5611_Read_Coefficients(void)
{
  uint8_t data[2];
  uint32_t i;
  MS5611_Send_Command(MS5611_READ_PROM_Coff1);
  for(i=0;i<6;i++)
  {
    MS5611_Read_Bytes(data, 2);
    imCoff.MS5611_Coff[i] = ((uint16_t)(data[0])<<8) | (uint16_t)(data[1]);
  }
  MS5611_Read_Bytes(data, 2);
  imCoff.MS5611_CRC = (data[1])|0x07;
}


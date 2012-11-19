#include "MS5611.h"
#include "IIC.h"

MS5611_Data_Union MS5611_Data;

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
  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));

  while(NumByteToRead)  
  {
    if(NumByteToRead == 1)
    {
      /* Disable Acknowledgement */
      I2C_AcknowledgeConfig(I2C2, DISABLE);      
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
  /* Send STOP Condition */
  IIC_Stop();
}

static void MS5611_Read_Coefficients(void)
{
  uint8_t data[2];
  uint32_t i;
  
  for(i=0;i<7;i++)
  {
    MS5611_Send_Command(MS5611_READ_PROM_Coff(i+1));
    MS5611_Read_Bytes(data, 2);
    MS5611_Data.MS5611_PROM.MS5611_Coff[i] = ((uint16_t)(data[0])<<8) | (uint16_t)(data[1]);
  }
}

void MS5611_Init(void)
{
  MS5611_Reset();
  delay_ms(10);
  MS5611_Read_Coefficients(); 
}


uint32_t MS5611_ADC_Read(void)
{
  uint8_t ADC_Result[3];
  uint32_t result = 0;
  
  MS5611_Send_Command(MS5611_READ_ADC); 
  MS5611_Read_Bytes(ADC_Result, 3);
  
  result |= (((uint32_t)(ADC_Result[0]))<<16);
  result |= (((uint32_t)(ADC_Result[1]))<<8);
  result |= ((uint32_t)(ADC_Result[2]));
  
  return result;
}

int32_t Pressure;
int32_t Temperature;
static int32_t dT;
static int64_t OFF;
static int64_t SENS;

static int32_t MS5611_Calculate_Temperature(void)
{
  int32_t Temp;
  
  dT = (int32_t)MS5611_Data.MS5611_Data.Temp_ADC - (int32_t)(((uint32_t)MS5611_Data.MS5611_Data.MS5611_TREF) << 8);  
  Temp = (2000 + (((int64_t)dT * (int64_t)MS5611_Data.MS5611_Data.MS5611_TEMPSENS) >> 23));
  return Temp;
}

static int32_t MS5611_Calculate_Pressure(void)
{
  int32_t pres;
  
  OFF = (((int64_t)MS5611_Data.MS5611_Data.MS5611_OFF_T1) << 16) + ((((int64_t)MS5611_Data.MS5611_Data.MS5611_TCO) * dT) >> 7);
  SENS = (((int64_t)MS5611_Data.MS5611_Data.MS5611_SENS_T1) << 15) + ((((int64_t)MS5611_Data.MS5611_Data.MS5611_TCS) * dT) >> 8);
  pres = (((((int64_t)MS5611_Data.MS5611_Data.Pres_ADC) * SENS) >> 21) - OFF) >> 15;
  return pres;
}

static void MS5611_Temperature_Compensation(int32_t* t)
{
  int32_t Temp,T2,OFF2,SENS2;
  
  Temp = *t;
  if(Temp<2000)
  {
      T2= (dT >> 15) * (dT >> 15)/2;
      OFF2 = 5 * (Temp - 2000) * (Temp - 2000) / 2;
      SENS2 = OFF2 >> 1;
      if(Temp < -1500)
      {
        OFF2 += 7 * (Temp + 1500) * (Temp + 1500);
        SENS2 += 11 * (Temp + 1500) * (Temp + 1500) / 2; 
      }
      Temp -= T2;
      OFF -= OFF2;
      SENS -= SENS2;
  }
  *t = Temp;
}

void MS5611_Get_TP(void)
{
  MS5611_Conversion_Temperature4096();
  delay_ms(10);
  MS5611_Data.MS5611_PROM.Temp_ADC = MS5611_ADC_Read();
  MS5611_Conversion_Pressure4096();
  delay_ms(10);
  MS5611_Data.MS5611_PROM.Pres_ADC = MS5611_ADC_Read();
  Temperature = MS5611_Calculate_Temperature();
  Pressure = MS5611_Calculate_Pressure();
  //MS5611_Temperature_Compensation(&Temperature);
}
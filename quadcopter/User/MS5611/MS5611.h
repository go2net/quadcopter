#ifndef __MS5611_H
#define __MS5611_H

#ifdef __cplusplus
 extern "C" {
#endif

/***********************************************************************************
* INCLUDES
*/   

#include "stm32f10x.h"
#include "common.h"
   
/***********************************************************************************
* CONSTANTS AND DEFINES
*/
// Address
#define MS5611_ADDRESS                  0xEE//0xEC
   

// Commands 
#define MS5611_READ_ADC                 0x00
#define MS5611_RESET                    0x1E
#define MS5611_D1_CONVERSION_256        0x40
#define MS5611_D1_CONVERSION_512        0x42
#define MS5611_D1_CONVERSION_1024       0x44
#define MS5611_D1_CONVERSION_2048       0x46
#define MS5611_D1_CONVERSION_4096       0x48
#define MS5611_D2_CONVERSION_256        0x50
#define MS5611_D2_CONVERSION_512        0x52
#define MS5611_D2_CONVERSION_1024       0x54
#define MS5611_D2_CONVERSION_2048       0x56
#define MS5611_D2_CONVERSION_4096       0x58
#define MS5611_READ_PROM_RESERVED       0xA0
#define MS5611_READ_PROM_Coff(x)        (0xA0|((x)<<1))
#define MS5611_READ_PROM_Coff1          (0xA0|(0x01<<1))
#define MS5611_READ_PROM_Coff2          (0xA0|(0x02<<1))
#define MS5611_READ_PROM_Coff3          (0xA0|(0x03<<1))
#define MS5611_READ_PROM_Coff4          (0xA0|(0x04<<1))
#define MS5611_READ_PROM_Coff5          (0xA0|(0x05<<1))
#define MS5611_READ_PROM_Coff6          (0xA0|(0x06<<1))
#define MS5611_READ_PROM_CRC            (0xA0|(0x07<<1))

   
#define MS5611_IIC_WRITE                0x00
#define MS5611_IIC_READ                 0x01   
   
   

#define MS5611_Reset()                          {MS5611_Send_Command(MS5611_RESET);}   
#define MS5611_Conversion_Pressure256()         {MS5611_Send_Command(MS5611_D1_CONVERSION_256);} 
#define MS5611_Conversion_Pressure512()         {MS5611_Send_Command(MS5611_D1_CONVERSION_512);}
#define MS5611_Conversion_Pressure1024()        {MS5611_Send_Command(MS5611_D1_CONVERSION_1024);}
#define MS5611_Conversion_Pressure2048()        {MS5611_Send_Command(MS5611_D1_CONVERSION_2048);}
#define MS5611_Conversion_Pressure4096()        {MS5611_Send_Command(MS5611_D1_CONVERSION_4096);}   
#define MS5611_Conversion_Temperature256()      {MS5611_Send_Command(MS5611_D2_CONVERSION_256);}
#define MS5611_Conversion_Temperature512()      {MS5611_Send_Command(MS5611_D2_CONVERSION_512);}
#define MS5611_Conversion_Temperature1024()     {MS5611_Send_Command(MS5611_D2_CONVERSION_1024);}
#define MS5611_Conversion_Temperature2048()     {MS5611_Send_Command(MS5611_D2_CONVERSION_2048);}
#define MS5611_Conversion_Temperature4096()     {MS5611_Send_Command(MS5611_D2_CONVERSION_4096);} 

   
   
typedef struct 
{
  uint32_t Pres_ADC;
  uint32_t Temp_ADC;
  uint16_t MS5611_Coff[7];
} MS5611_PROM_Struct;   

typedef struct 
{
  uint32_t Pres_ADC;
  uint32_t Temp_ADC;
  uint16_t MS5611_SENS_T1;
  uint16_t MS5611_OFF_T1;
  uint16_t MS5611_TCS;
  uint16_t MS5611_TCO;
  uint16_t MS5611_TREF;
  uint16_t MS5611_TEMPSENS;
  uint16_t MS5611_CRC;
} MS5611_Data_Struct;

typedef union
{
  MS5611_PROM_Struct MS5611_PROM;
  MS5611_Data_Struct MS5611_Data;
} MS5611_Data_Union,*pMS5611_Data_Union;

extern int32_t Pressure;
extern int32_t Temperature;
void MS5611_Init(void);
void MS5611_Get_TP(void);

#ifdef __cplusplus
}
#endif

#endif /* __MS5611_H */   
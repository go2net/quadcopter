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
#define MS5611_ADDRESS                  0xEC
   

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
  uint32_t Pressure;
  uint32_t Temperature;
  uint16_t MS5611_Coff[6];
  uint16_t MS5611_CRC;
} MS5611_Data_Struct , *pMS5611_Data_Struct;   
   

#ifdef __cplusplus
}
#endif

#endif /* __MS5611_H */   
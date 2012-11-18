#include "IIC.h"


void IIC_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  I2C_InitTypeDef  I2C_InitStructure;
  
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Pin = IIC_SDA_Pin |IIC_SCL_Pin;
  GPIO_Init(IIC_Port, &GPIO_InitStructure);   
  
  I2C_Cmd(I2C2, ENABLE);
    
  /* I2C2 configuration ------------------------------------------------------*/
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
  I2C_InitStructure.I2C_OwnAddress1 = 0x1C;
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
  I2C_InitStructure.I2C_ClockSpeed = 400000;

  I2C_Init(I2C2, &I2C_InitStructure);
  
}
					 				     
void IIC_Write_Byte(uint8_t dev, uint8_t reg, uint8_t data)
{
  /* Send STRAT condition */
  IIC_Start();
  /* Test on EV5 and clear it */
  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT)); 
  /* Send device address for write */
  IIC_Send_Byte(dev);
  /* Test on EV6 and clear it */
  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
  
  IIC_Send_Byte(reg);   
  /* Test on EV8 and clear it */
  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
  
  IIC_Send_Byte(data);   
  /* Test on EV8 and clear it */
  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));  
  /* Send STOP condition */  
  IIC_Stop();
}	  

   

uint8_t IIC_Read_Byte(uint8_t dev, uint8_t reg)
{
  /* While the bus is busy */
  while(I2C_GetFlagStatus(I2C2, I2C_FLAG_BUSY));
  /* Send STRAT condition */
  IIC_Start();
  /* Test on EV5 and clear it */
  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT)); 
  /* Send device address for write */
  IIC_Send_Byte(dev);
  /* Test on EV6 and clear it */
  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
  
  
  
  return 0;
}


//unsigned char I2C_ReadOneByte(unsigned char I2C_Addr,unsigned char addr)
//{
//	unsigned char res=0;
//	
//	IIC_Start();	
//	IIC_Send_Byte(I2C_Addr);	   //发送写命令
//	res++;
//	IIC_Wait_Ack();
//	IIC_Send_Byte(addr); res++;  //发送地址
//	IIC_Wait_Ack();	  
//	//IIC_Stop();//产生一个停止条件	
//	IIC_Start();
//	IIC_Send_Byte(I2C_Addr+1); res++;          //进入接收模式			   
//	IIC_Wait_Ack();
//	res=IIC_Read_Byte(0);	   
//    IIC_Stop();//产生一个停止条件
//
//	return res;
//}



//u8 IICreadBytes(u8 dev, u8 reg, u8 length, u8 *data){
//    u8 count = 0;
//	
//	IIC_Start();
//	IIC_Send_Byte(dev);	   //发送写命令
//	IIC_Wait_Ack();
//	IIC_Send_Byte(reg);   //发送地址
//    IIC_Wait_Ack();	  
//	IIC_Start();
//	IIC_Send_Byte(dev+1);  //进入接收模式	
//	IIC_Wait_Ack();
//	
//    for(count=0;count<length;count++){
//		 
//		 if(count!=length-1)data[count]=IIC_Read_Byte(1);  //带ACK的读数据
//		 	else  data[count]=IIC_Read_Byte(0);	 //最后一个字节NACK
//	}
//    IIC_Stop();//产生一个停止条件
//    return count;
//}


//uint8_t IICwriteBytes(uint8_t dev, u8 reg, uint8_t length, uint8_t* data)
//{  
// 	uint8_t count = 0;
//	IIC_Start();
//	IIC_Send_Byte(dev);	   //发送写命令
//	IIC_Wait_Ack();
//	IIC_Send_Byte(reg);   //发送地址
//    IIC_Wait_Ack();	  
//	for(count=0;count<length;count++){
//		IIC_Send_Byte(data[count]); 
//		IIC_Wait_Ack(); 
//	 }
//	IIC_Stop();//产生一个停止条件
//
//    return 1; //status == 0;
//}


//u8 IICreadByte(u8 dev, u8 reg, u8 *data){
//	*data=I2C_ReadOneByte(dev, reg);
//    return 1;
//}

 


//u8 IICwriteBits(u8 dev,u8 reg,u8 bitStart,u8 length,u8 data)
//{
//
//    u8 b;
//    if (IICreadByte(dev, reg, &b) != 0) {
//        u8 mask = (0xFF << (bitStart + 1)) | 0xFF >> ((8 - bitStart) + length - 1);
//        data <<= (8 - length);
//        data >>= (7 - bitStart);
//        b &= mask;
//        b |= data;
//        return IICwriteByte(dev, reg, b);
//    } else {
//        return 0;
//    }
//}


//u8 IICwriteBit(u8 dev, u8 reg, u8 bitNum, u8 data){
//    u8 b;
//    IICreadByte(dev, reg, &b);
//    b = (data != 0) ? (b | (1 << bitNum)) : (b & ~(1 << bitNum));
//    return IICwriteByte(dev, reg, b);
//}
#ifndef __IIC_H
#define __IIC_H

#include "main.h"

#define SCL_GPIO_CLK_ENABLE()              __HAL_RCC_GPIOA_CLK_ENABLE()
#define SDA_GPIO_CLK_ENABLE()              __HAL_RCC_GPIOA_CLK_ENABLE()
#define SCL_Pin GPIO_PIN_4
#define SCL_GPIO_Port GPIOA
#define SDA_Pin GPIO_PIN_0
#define SDA_GPIO_Port GPIOA

#define SDA_DATA(a)	(a?HAL_GPIO_WritePin(SDA_GPIO_Port,SDA_Pin,GPIO_PIN_SET):HAL_GPIO_WritePin(SDA_GPIO_Port,SDA_Pin,GPIO_PIN_RESET))
#define SCL_OUT(a)	(a?HAL_GPIO_WritePin(SCL_GPIO_Port,SCL_Pin,GPIO_PIN_SET):HAL_GPIO_WritePin(SCL_GPIO_Port,SCL_Pin,GPIO_PIN_RESET))												
							
#define SDA_IO_IN   HAL_GPIO_ReadPin(SDA_GPIO_Port,SDA_Pin)

//IIC所有操作函数
void IIC_GPIO_Init(void);                //初始化IIC的IO口	
void SDA_OUT(void);
void SDA_IN(void);			 
void IIC_Start(void);				//发送IIC开始信号
void IIC_Stop(void);	  			//发送IIC停止信号
void IIC_Send_Byte(uint8_t txd);			//IIC发送一个字节
uint8_t IIC_Read_Byte(unsigned char ack);//IIC读取一个字节
uint8_t IIC_Wait_Ack(void); 				//IIC等待ACK信号
void IIC_Ack(void);					//IIC发送ACK信号
void IIC_NAck(void);				//IIC不发送ACK信号

#endif


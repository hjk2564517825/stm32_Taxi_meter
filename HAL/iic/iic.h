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

//IIC���в�������
void IIC_GPIO_Init(void);                //��ʼ��IIC��IO��	
void SDA_OUT(void);
void SDA_IN(void);			 
void IIC_Start(void);				//����IIC��ʼ�ź�
void IIC_Stop(void);	  			//����IICֹͣ�ź�
void IIC_Send_Byte(uint8_t txd);			//IIC����һ���ֽ�
uint8_t IIC_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
uint8_t IIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void IIC_Ack(void);					//IIC����ACK�ź�
void IIC_NAck(void);				//IIC������ACK�ź�

#endif


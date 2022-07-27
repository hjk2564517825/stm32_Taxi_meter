#include "iic.h"
#include "delay.h"
 
 
//初始化IIC
void IIC_GPIO_Init(void)
{		

	SCL_GPIO_CLK_ENABLE();
	SDA_GPIO_CLK_ENABLE();

	GPIO_InitTypeDef GPIO_InitStruct = {0};

  /*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = SCL_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(SCL_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = SDA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(SDA_GPIO_Port, &GPIO_InitStruct);
}

//SDA PB7
//SCL PB6

void SDA_OUT(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	
	GPIO_InitStruct.Pin = SDA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(SDA_GPIO_Port, &GPIO_InitStruct);
}

void SDA_IN(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	
	GPIO_InitStruct.Pin = SDA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(SDA_GPIO_Port, &GPIO_InitStruct);
}

//产生IIC起始信号
void IIC_Start(void)
{
	SDA_OUT();     	//sda线输出
	SDA_DATA(1);	
	SCL_OUT(1);  	  
	delay_us(20);

	SDA_DATA(0);	//START:when CLK is high,DATA change form high to low 
	delay_us(20);
	SCL_OUT(0); 	//钳住I2C总线，准备发送或接收数据 
}	
  
//产生IIC停止信号
void IIC_Stop(void)
{
	SDA_OUT();		//sda线输出
	SCL_OUT(0);  
	SDA_DATA(0);	//STOP:when CLK is high DATA change form low to high
 	delay_us(20);
 	SCL_OUT(1);  
	SDA_DATA(1);	//发送I2C总线结束信号
	delay_us(20);
}  

//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
uint8_t IIC_Wait_Ack(void)
{
	uint8_t ucErrTime=0;
	SDA_IN();      		//SDA设置为输入  
	SDA_DATA(1);
	delay_us(10);
	SCL_OUT(1);  	   
	delay_us(10);
	while(SDA_IO_IN)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
//			printf("no ack!\r\n");
			IIC_Stop();
			return 1;
		}
	}
	SCL_OUT(0);  		//时钟输出0 	
	return 0;  
} 

//产生ACK应答
void IIC_Ack(void)
{
	SCL_OUT(0);  	
	SDA_OUT();
	SDA_DATA(0);
	delay_us(20);
	SCL_OUT(1); 
	delay_us(20);
	SCL_OUT(0); 
} 

//不产生ACK应答		    
void IIC_NAck(void)
{
	SCL_OUT(0); 
	SDA_OUT();
	SDA_DATA(1);
	delay_us(20);
	SCL_OUT(1); 
	delay_us(20);
	SCL_OUT(0); 
}		
			 				     
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
void IIC_Send_Byte(uint8_t txd)
{                        
    uint8_t t;   
		SDA_OUT(); 
		SCL_OUT(0); 	    	//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
			if((txd&0x80)>>7)
				SDA_DATA(1); 
			else
				SDA_DATA(0); 
			txd<<=1; 	  
			delay_us(20);   
			SCL_OUT(1); 
			delay_us(20); 
			SCL_OUT(0); 	
			delay_us(20);
    }	
		delay_us(20);
		
} 	    

//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
uint8_t IIC_Read_Byte(uint8_t ack)
{
		uint8_t i;
		uint16_t receive=0;
		SDA_IN();//SDA设置为输入
    for(i=0;i<8;i++ )
	{
        SCL_OUT(0);  
        delay_us(20);
        SCL_OUT(1); 
        receive<<=1;
        if(SDA_IO_IN)	
					receive++;   
				delay_us(20); 
				//printf("%d,%d ",i,receive);
    }		
		//printf("\n");
    if (!ack)
        IIC_NAck();//发送nACK
    else
        IIC_Ack(); //发送ACK   
    return receive;
}

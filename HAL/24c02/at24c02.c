/**********************************
包含头文件
**********************************/
#include "at24c02.h"
#include "iic.h"

/****
*******向AT24C02的地址address中写入一个字节的数据info
*****/
void At24c02_Write_Add(uchar address,uchar info)
{
    IIC_Start(); 
    IIC_Send_Byte(0xa0);
    IIC_NAck(); 
    IIC_Send_Byte(address);
    IIC_NAck(); 
    IIC_Send_Byte(info);
    IIC_NAck(); 
    IIC_Stop();
}

/****
*******从AT24C02的地址address中读取一个字节的数据
*****/
uchar At24c02_Read_Add(uchar address)
{
    uchar i;
    IIC_Start(); 
    IIC_Send_Byte(0xa0);
    IIC_NAck(); 
    IIC_Send_Byte(address);
    IIC_NAck(); 
		HAL_Delay(1);
    IIC_Start();
    IIC_Send_Byte(0xa1); 
    IIC_NAck();
    i=IIC_Read_Byte(0); 
    IIC_Stop();
    return(i);
}


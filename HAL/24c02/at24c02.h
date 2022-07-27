/**********************************
AT24C02
**********************************/

#ifndef _AT24C02_H_
#define _AT24C02_H_

#ifndef uchar
#define uchar unsigned char
#endif
//#define 

/**********************************
包含头文件
**********************************/
#include "main.h"

/**********************************
函数声明
**********************************/
void At24c02_Write_Add(uchar address,uchar info);			//At24c02写入数据函数
uchar At24c02_Read_Add(uchar address);								//At24c02读取数据函数



#endif

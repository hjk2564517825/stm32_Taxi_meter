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
����ͷ�ļ�
**********************************/
#include "main.h"

/**********************************
��������
**********************************/
void At24c02_Write_Add(uchar address,uchar info);			//At24c02д�����ݺ���
uchar At24c02_Read_Add(uchar address);								//At24c02��ȡ���ݺ���



#endif

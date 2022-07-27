/**********************************
按键头文件
**********************************/

#ifndef _KEY_H_
#define _KEY_H_


/**********************************
包含头文件
**********************************/
#include "main.h"


/**********************************
PIN口定义
**********************************/
#define K1 HAL_GPIO_ReadPin(K1_GPIO_Port,K1_Pin)
#define K2 HAL_GPIO_ReadPin(K2_GPIO_Port,K2_Pin)
#define K3 HAL_GPIO_ReadPin(K3_GPIO_Port,K3_Pin)
#define K4 HAL_GPIO_ReadPin(K4_GPIO_Port,K4_Pin)
#define K5 HAL_GPIO_ReadPin(K5_GPIO_Port,K5_Pin)
#define K6 HAL_GPIO_ReadPin(K6_GPIO_Port,K6_Pin)
#define K7 HAL_GPIO_ReadPin(K7_GPIO_Port,K7_Pin)

/**********************************
函数声明
**********************************/
uint8_t Chiclet_Keyboard_Scan(void);			//独立按键扫描函数


#endif


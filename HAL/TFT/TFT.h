#ifndef __TFT__H__
#define __TFT__H__

#include "main.h"
#include "string.h"

#define uchar unsigned char
#define uint unsigned int


//#define USE_LANDSCAPE//测试程序横竖屏切换，横屏请开启此宏，竖屏请屏蔽

//---------------------------液晶屏接线说明-------------------------------------//
//sbit bl        =P2^4;//接模块BL引脚
//sbit scl       =P2^5;//接模块CLK引脚
//sbit sda       =P2^3;//接模块DIN/MOSI引脚
//sbit rs        =P2^2;//接模块D/C引脚
//sbit cs        =P2^0;//接模块CE引脚
//sbit reset     =P2^1;//接模块RST引脚
/* 宏定义 -------------------------------------------------------------------*/
/***********************   TFT 连接引脚定义  **************************/
#define TFT_BL_GPIO_CLK_ENABLE()              __HAL_RCC_GPIOB_CLK_ENABLE()
#define TFT_SCL_GPIO_CLK_ENABLE()             __HAL_RCC_GPIOB_CLK_ENABLE()
#define TFT_SDA_GPIO_CLK_ENABLE()             __HAL_RCC_GPIOB_CLK_ENABLE()
#define TFT_RS_GPIO_CLK_ENABLE()              __HAL_RCC_GPIOB_CLK_ENABLE()
#define TFT_RST_GPIO_CLK_ENABLE()             __HAL_RCC_GPIOB_CLK_ENABLE()
#define TFT_CS_GPIO_CLK_ENABLE()              __HAL_RCC_GPIOB_CLK_ENABLE()

#define TFT_BL_Port														GPIOB
#define TFT_SCL_Port													GPIOB
#define TFT_SDA_Port													GPIOB
#define TFT_RS_Port														GPIOB
#define TFT_RST_Port													GPIOB
#define TFT_CS_Port														GPIOB

#define TFT_BL_Pin														GPIO_PIN_8
#define TFT_SCL_Pin														GPIO_PIN_6
#define TFT_SDA_Pin														GPIO_PIN_7
#define TFT_RS_Pin														GPIO_PIN_11
#define TFT_RST_Pin														GPIO_PIN_12
#define TFT_CS_Pin														GPIO_PIN_13

#define TFT_BL(a) (a?HAL_GPIO_WritePin(TFT_BL_Port,TFT_BL_Pin,GPIO_PIN_SET):HAL_GPIO_WritePin(TFT_BL_Port,TFT_BL_Pin,GPIO_PIN_RESET))
#define TFT_SCL(a) (a?HAL_GPIO_WritePin(TFT_SCL_Port,TFT_SCL_Pin,GPIO_PIN_SET):HAL_GPIO_WritePin(TFT_SCL_Port,TFT_SCL_Pin,GPIO_PIN_RESET))
#define TFT_SDA(a) (a?HAL_GPIO_WritePin(TFT_SDA_Port,TFT_SDA_Pin,GPIO_PIN_SET):HAL_GPIO_WritePin(TFT_SDA_Port,TFT_SDA_Pin,GPIO_PIN_RESET))
#define TFT_RS(a) (a?HAL_GPIO_WritePin(TFT_RS_Port,TFT_RS_Pin,GPIO_PIN_SET):HAL_GPIO_WritePin(TFT_RS_Port,TFT_RS_Pin,GPIO_PIN_RESET))
#define TFT_RST(a) (a?HAL_GPIO_WritePin(TFT_RST_Port,TFT_RST_Pin,GPIO_PIN_SET):HAL_GPIO_WritePin(TFT_RST_Port,TFT_RST_Pin,GPIO_PIN_RESET))
#define TFT_CS(a) (a?HAL_GPIO_WritePin(TFT_CS_Port,TFT_CS_Pin,GPIO_PIN_SET):HAL_GPIO_WritePin(TFT_CS_Port,TFT_CS_Pin,GPIO_PIN_RESET))

//定义常用颜色
#define RED  		0xf800
#define GREEN		0x07e0 
#define BLUE 		0x001f
#define WHITE		0xffff
#define BLACK		0x0000
#define YELLOW  0xFFE0
#define GRAY0   0xEF7D   
#define GRAY1   0x8410      	
#define GRAY2   0x4208  

void TFT_GPIO_Init(void);
void  SPI_WriteData(uchar Data);
void  Lcd_WriteIndex(uchar Data);
void  Lcd_WriteData(uchar Data);
void  LCD_WriteData_16Bit(unsigned int Data);
void Reset(void);
void lcd_initial(void);
void Lcd_SetRegion(unsigned int x_start,unsigned int y_start,unsigned int x_end,unsigned int y_end);
void PutPixel(uint x_start,uint y_start,uint color);
void dsp_single_colour(int color);
void Display_ASCII8X16(uint x0,uint y0,uchar *s);
void Display_ASCII8X16_Color(uint x0,uint y0,uchar *s,uint forecolor,uint backgroundcolor);

void Fast_DrawFont_GBK16(uint x, uint y, uint fc, uint bc, uchar *s);
void Gui_DrawFont_GBK16(uint x, uint y, uint fc, uint bc, uchar *s);
void Lcd_DrawLine(uint x,uint y,uint color);//画个固定长度的线

#endif

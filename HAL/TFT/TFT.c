#include "TFT.h"
#include "TFT_Font.h"
#include "delay.h"

//引脚初始化
void TFT_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
	
	/* 引脚时钟使能 */
	TFT_BL_GPIO_CLK_ENABLE();
	TFT_SCL_GPIO_CLK_ENABLE();
	TFT_SDA_GPIO_CLK_ENABLE();
	TFT_RS_GPIO_CLK_ENABLE();
	TFT_RST_GPIO_CLK_ENABLE();
	TFT_CS_GPIO_CLK_ENABLE();
	
  /* LED引脚初始化 */
  GPIO_InitStruct.Pin   = TFT_BL_Pin;
  GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull  = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(TFT_BL_Port, &GPIO_InitStruct);
  /* SCL引脚初始化 */
  GPIO_InitStruct.Pin   = TFT_SCL_Pin;
  GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull  = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(TFT_SCL_Port, &GPIO_InitStruct);
  /* SDA引脚初始化 */
  GPIO_InitStruct.Pin   = TFT_SDA_Pin;
  GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull  = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(TFT_SDA_Port, &GPIO_InitStruct);
  /* RS引脚初始化 */
  GPIO_InitStruct.Pin   = TFT_RS_Pin;
  GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull  = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(TFT_RS_Port, &GPIO_InitStruct);
  /* RST引脚初始化 */
  GPIO_InitStruct.Pin   = TFT_RST_Pin;
  GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull  = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(TFT_RST_Port, &GPIO_InitStruct);
  /* CS引脚初始化 */
  GPIO_InitStruct.Pin   = TFT_CS_Pin;
  GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull  = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(TFT_CS_Port, &GPIO_InitStruct);
}

//向SPI总线传输一个8位数据
void  SPI_WriteData(uchar Data)
{
	unsigned char i=0;
	for(i=8;i>0;i--)
	{
		if(Data&0x80)	
		TFT_SDA(1); //输出数据
		else TFT_SDA(0);

		TFT_SCL(0);
		TFT_SCL(1);
		Data<<=1;
	}
}
//向液晶屏写一个8位指令
void  Lcd_WriteIndex(uchar Data)
{
		TFT_CS(0);
		TFT_RS(0);
		SPI_WriteData(Data); 		
		TFT_CS(1);
}
//向液晶屏写一个8位数据
void  Lcd_WriteData(uchar Data)
{	
		TFT_CS(0);
		TFT_RS(1);
		SPI_WriteData(Data); 	
		TFT_CS(1);
}
//向液晶屏写一个16位数据
void  LCD_WriteData_16Bit(unsigned int Data)
{
	TFT_CS(0);
	TFT_RS(1);
	SPI_WriteData(Data>>8); 	//写入高8位数据
	SPI_WriteData(Data); 			//写入低8位数据
	TFT_CS(1);
}

void Reset(void)
{
    TFT_RST(0);
    HAL_Delay(100);
    TFT_RST(1);
    HAL_Delay(100);
}
//////////////////////////////////////////////////////////////////////////////////////////////
//液晶屏初始化 for S6D02A1
void lcd_initial(void)
{	Reset();//Reset before LCD Init.
		
	//LCD Init For 1.44Inch LCD Panel with ST7735R.
	Lcd_WriteIndex(0x11);//Sleep exit 
	HAL_Delay (120);
		
	//ST7735R Frame Rate
	Lcd_WriteIndex(0xB1); 
	Lcd_WriteData(0x01); 
	Lcd_WriteData(0x2C); 
	Lcd_WriteData(0x2D); 

	Lcd_WriteIndex(0xB2); 
	Lcd_WriteData(0x01); 
	Lcd_WriteData(0x2C); 
	Lcd_WriteData(0x2D); 

	Lcd_WriteIndex(0xB3); 
	Lcd_WriteData(0x01); 
	Lcd_WriteData(0x2C); 
	Lcd_WriteData(0x2D); 
	Lcd_WriteData(0x01); 
	Lcd_WriteData(0x2C); 
	Lcd_WriteData(0x2D); 
	
	Lcd_WriteIndex(0xB4); //Column inversion 
	Lcd_WriteData(0x07); 
	
	//ST7735R Power Sequence
	Lcd_WriteIndex(0xC0); 
	Lcd_WriteData(0xA2); 
	Lcd_WriteData(0x02); 
	Lcd_WriteData(0x84); 
	Lcd_WriteIndex(0xC1); 
	Lcd_WriteData(0xC5); 

	Lcd_WriteIndex(0xC2); 
	Lcd_WriteData(0x0A); 
	Lcd_WriteData(0x00); 

	Lcd_WriteIndex(0xC3); 
	Lcd_WriteData(0x8A); 
	Lcd_WriteData(0x2A); 
	Lcd_WriteIndex(0xC4); 
	Lcd_WriteData(0x8A); 
	Lcd_WriteData(0xEE); 
	
	Lcd_WriteIndex(0xC5); //VCOM 
	Lcd_WriteData(0x0E); 
	
	Lcd_WriteIndex(0x36); //MX, MY, RGB mode 
#ifdef USE_LANDSCAPE
	Lcd_WriteData(0xA8); //竖屏C8 横屏08 A8
#else
	Lcd_WriteData(0xC8); //竖屏C8 横屏08 A8
#endif		
	//ST7735R Gamma Sequence
	Lcd_WriteIndex(0xe0); 
	Lcd_WriteData(0x0f); 
	Lcd_WriteData(0x1a); 
	Lcd_WriteData(0x0f); 
	Lcd_WriteData(0x18); 
	Lcd_WriteData(0x2f); 
	Lcd_WriteData(0x28); 
	Lcd_WriteData(0x20); 
	Lcd_WriteData(0x22); 
	Lcd_WriteData(0x1f); 
	Lcd_WriteData(0x1b); 
	Lcd_WriteData(0x23); 
	Lcd_WriteData(0x37); 
	Lcd_WriteData(0x00); 	
	Lcd_WriteData(0x07); 
	Lcd_WriteData(0x02); 
	Lcd_WriteData(0x10); 

	Lcd_WriteIndex(0xe1); 
	Lcd_WriteData(0x0f); 
	Lcd_WriteData(0x1b); 
	Lcd_WriteData(0x0f); 
	Lcd_WriteData(0x17); 
	Lcd_WriteData(0x33); 
	Lcd_WriteData(0x2c); 
	Lcd_WriteData(0x29); 
	Lcd_WriteData(0x2e); 
	Lcd_WriteData(0x30); 
	Lcd_WriteData(0x30); 
	Lcd_WriteData(0x39); 
	Lcd_WriteData(0x3f); 
	Lcd_WriteData(0x00); 
	Lcd_WriteData(0x07); 
	Lcd_WriteData(0x03); 
	Lcd_WriteData(0x10);  
	
	Lcd_WriteIndex(0x2a);
	Lcd_WriteData(0x00);
	Lcd_WriteData(0x00+2);
	Lcd_WriteData(0x00);
	Lcd_WriteData(0x80+2);

	Lcd_WriteIndex(0x2b);
	Lcd_WriteData(0x00);
	Lcd_WriteData(0x00+3);
	Lcd_WriteData(0x00);
	Lcd_WriteData(0x80+3);
	
	Lcd_WriteIndex(0xF0); //Enable test command  
	Lcd_WriteData(0x01); 
	Lcd_WriteIndex(0xF6); //Disable ram power save mode 
	Lcd_WriteData(0x00); 
	
	Lcd_WriteIndex(0x3A); //65k mode 
	Lcd_WriteData(0x05); 
	
	
	Lcd_WriteIndex(0x29);//Display on
	TFT_BL(1);
}



/*************************************************
函数名：LCD_Set_Region
功能：设置lcd显示区域，在此区域写点数据自动换行
入口参数：xy起点和终点
返回值：无
*************************************************/
void Lcd_SetRegion(unsigned int x_start,unsigned int y_start,unsigned int x_end,unsigned int y_end)
{	
#ifdef USE_LANDSCAPE//使用横屏模式
	Lcd_WriteIndex(0x2a);
	Lcd_WriteData(0x00);
	Lcd_WriteData(x_start+3);
	Lcd_WriteData(0x00);
	Lcd_WriteData(x_end+3);

	Lcd_WriteIndex(0x2b);
	Lcd_WriteData(0x00);
	Lcd_WriteData(y_start+2);
	Lcd_WriteData(0x00);
	Lcd_WriteData(y_end+2);

#else//竖屏模式	
	Lcd_WriteIndex(0x2a);
	Lcd_WriteData(0x00);
	Lcd_WriteData(x_start+2);
	Lcd_WriteData(0x00);
	Lcd_WriteData(x_end+2);

	Lcd_WriteIndex(0x2b);
	Lcd_WriteData(0x00);
	Lcd_WriteData(y_start+3);
	Lcd_WriteData(0x00);
	Lcd_WriteData(y_end+3);	
#endif
	Lcd_WriteIndex(0x2c);
}


void PutPixel(uint x_start,uint y_start,uint color)
{
	Lcd_SetRegion(x_start,y_start,x_start+1,y_start+1);
	LCD_WriteData_16Bit(color);
	
}



void dsp_single_colour(int color)
{
 	uchar i,j;
	Lcd_SetRegion(0,0,128-1,128-1);
 	for (i=0;i<128;i++)
    	for (j=0;j<128;j++)
        	LCD_WriteData_16Bit(color);
}


//显示一个英文字符,带前景色和背景色设定
void Display_ASCII8X16_Color(uint x0,uint y0,uchar *s,uint forecolor,uint backgroundcolor)
{
	int x,y;
	
	unsigned int qm;	
	while(*s != '\0')
	{
		qm = *(s) * 16;
		Lcd_SetRegion(x0,y0,x0+8-1,y0+16-1);
		for(x=0;x<16;x++)
		{
			for(y=0;y<8;y++)
			{
				if(Zk_ASCII8X16[qm+x]&(0x80>>y)) LCD_WriteData_16Bit(forecolor);
				else LCD_WriteData_16Bit(backgroundcolor);
			}
		}
		s++;
		x0+=8;
	}     	
}

//显示一个英文字符
void Display_ASCII8X16(uint x0,uint y0,uchar *s)
{
	int i,j,k,x,y,xx;
	
	unsigned char qm;
	
	long int ulOffset;
	
	char  ywbuf[32],temp[2];
	
	for(i = 0; i<strlen((char*)s);i++)
	{
		if(((unsigned char)(*(s+i))) >= 161)
		{
			temp[0] = *(s+i);
			temp[1] = '\0';
			return;
		}
		else
		{
			qm = *(s+i);
			
			ulOffset = (long int)(qm) * 16;
			
			for (j = 0; j < 16; j ++)
			{
				ywbuf[j]=Zk_ASCII8X16[ulOffset+j];
			}
			
			for(y = 0;y < 16;y++)
			{
				for(x=0;x<8;x++) 
				{
					k=x % 8;
					
					if(ywbuf[y]&(0x80 >> k))
					{
						xx=x0+x+i*8;
						PutPixel(xx,y+y0,RED);
					}
//					else	  //会不会背景色覆盖未填充区域
//					{
//						xx=x0+x+i*8;
//						PutPixel(xx,y+y0,WHITE);	
//					}
		
				}
			}
		
		}
	}     	
}

//采用设置窗口方法填充字库数据，相比Gui_DrawFont_GBK16更快速
void Fast_DrawFont_GBK16(uint x, uint y, uint fc, uint bc, uchar *s)
{
	uchar ii,iii,m;
//	u16 x0=x,x1=x;
//	u16 y1=y;
	while(*s != '\0')
	{
		for(m=0;m<50;m++)
		{
			if((hsk_16x16[m].hsk_2[0] == *s) && (hsk_16x16[m].hsk_2[1] == *(s+1)))
			{
				Lcd_SetRegion(x,y,x+16-1,y+16-1);
				for(ii=0;ii<16*2;ii++)
				{
					for(iii=0;iii<8;iii++)
					{
						if(hsk_16x16[m].hsk_32[ii]&(0x80>>iii))	LCD_WriteData_16Bit(fc);
						else if (fc!=bc) LCD_WriteData_16Bit(bc);
					}
				}
			}
		}
		x+=16;
		s += 2;
	}
}

void Lcd_DrawLine(uint x,uint y,uint color)//画个固定长度的线
{
	uchar j,k;
	Lcd_SetRegion(x,y,x+15,y+2);
	for(j=0;j<16;j++)
	{
		for(k=0;k<2;k++)
		{
			LCD_WriteData_16Bit(color);
		}
	}
}

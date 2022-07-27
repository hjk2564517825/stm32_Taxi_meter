/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "rtc.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "TFT.h"
#include "at24c02.h"
#include "iic.h"
//#include "ds1302.h"
#include "stdio.h"
#include "key.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

uint8_t USART1_TX_BUF[200]; //����
#define u1_printf(...)  HAL_UART_Transmit(&huart1,USART1_TX_BUF,sprintf((char *)USART1_TX_BUF,__VA_ARGS__),0xffff)
																					
uint8_t USART2_TX_BUF[200]; //��������
#define u2_printf(...)  HAL_UART_Transmit(&huart2,USART2_TX_BUF,sprintf((char *)USART2_TX_BUF,__VA_ARGS__),0xffff)

#define Motor_IN1(a) (a?HAL_GPIO_WritePin(Motor_IN1_GPIO_Port,Motor_IN1_Pin,GPIO_PIN_SET):HAL_GPIO_WritePin(Motor_IN1_GPIO_Port,Motor_IN1_Pin,GPIO_PIN_RESET))
#define Motor_IN2(a) (a?HAL_GPIO_WritePin(Motor_IN2_GPIO_Port,Motor_IN2_Pin,GPIO_PIN_SET):HAL_GPIO_WritePin(Motor_IN2_GPIO_Port,Motor_IN2_Pin,GPIO_PIN_RESET))

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/**********************************
��������
**********************************/
uchar key_num = 0;										//������ֵ
uchar flag_display = 0;								//��ʾ��־λ

char Dis_Buf[15];											//��ʾ�洢��������
uint16_t Motor_Speed,Motor_Num;				//������ٱ���
uint8_t Motor_PWM;										//������ٱ���

uint32_t price;												//�ܼ�
uint32_t km;													//������
uint32_t wait_sec;										//�ȴ�ʱ�����־λ
uint8_t sec;													//���־λ
uint8_t jilu[10][10];									//���10�μ�¼����
uint8_t jilu_num;											//��¼

RTC_TimeTypeDef RTC_Time;							//RTCʱ��ṹ��
uint8_t time_set;											//ʱ�����ñ���

/* ��ַ0--Сʱ   ��ַ1--����   ��ַ2--��   ��ַ3--����   ��ַ4--��������λʮλ   ��ַ5--��������λС��λ
��ַ6--�ȴ�ʱ�����   ��ַ7--�ȴ�ʱ����   ��ַ8--�۸��λʮλ   ��ַ9--�۸��λС��λ */
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/**********************************
��������
**********************************/
void Key_function(void);							//��������
void Monitor_function(void);					//��⺯��
void Display_function(void);					//��ʾ����
void LCD_Dis_Init(void);							//��ʾ���ݳ�ʼ������
void LCD_Clean_Half(void);						//�����

/****
*******��������
*****/
void Key_function(void)
{
	uint8_t j,k;
	key_num = Chiclet_Keyboard_Scan();	//����ɨ��
	if(key_num != 0)										//�а�������
	{
		switch(key_num)
		{
			case 1:													//����1���������
				if(Motor_PWM < 10) Motor_PWM+=4;
			break;

			case 2:													//����2���������
				if(Motor_PWM > 0) Motor_PWM-=4;
			break;

			case 3:													//����3���鿴��¼
				if(flag_display==0 && time_set==0)
				{
					flag_display = 3;
					jilu_num = 0;
					dsp_single_colour(WHITE);
					LCD_Dis_Init();
				}
				else if(flag_display == 3)
				{
					jilu_num++;
					if(jilu_num >= 10)
					{
						flag_display=0;
						dsp_single_colour(WHITE);
						LCD_Dis_Init();
					}
				}
			break;
			
			case 4:													//����4���л���ͬ״̬���ճ����пͣ�
				if(Motor_Speed==0 && time_set==0)					//ֻ���ڳ�ͣ������ʱ������л�
				{
					if(flag_display == 0)
					{
						flag_display = 1;
						LCD_Clean_Half();
						Fast_DrawFont_GBK16(32,40,BLACK,WHITE,"��ӭ����");
						Fast_DrawFont_GBK16(0,60,BLACK,WHITE,"�ϳ���ϵ�ð�ȫ��");
						u2_printf("��ӭ���٣��ϳ���ϵ�ð�ȫ��");
						HAL_Delay(1000);
						km=0;
						wait_sec=0;
					}
					else if(flag_display == 1)
					{
						flag_display = 2;
						u2_printf("���γ���������Ϊ%d%.2d.%d���۸�Ϊ%.2d.%dԪ",km/100000,km/1000%100,km/100%10,price/10,price%10);
						u2_printf("�˿���������³�����ӭ�´γ���");
						/* �����γ˳���¼�����24C02 */
						for(j=9;j>0;j--)										//��֮ǰ10�μ�¼����
						{
							for(k=0;k<10;k++)
							{
								jilu[j][k] = jilu[j-1][k];
							}
						}
						jilu[0][0] = RTC_Time.Hours;											//��¼���γ˳���Ϣ--Сʱ
						jilu[0][1] = RTC_Time.Minutes;										//��¼���γ˳���Ϣ--����
						jilu[0][2] = RTC_Time.Seconds;										//��¼���γ˳���Ϣ--��
						jilu[0][4] = km/100/100;													//��¼���γ˳���Ϣ--����������λʮλ��
						jilu[0][5] = km/100%100;													//��¼���γ˳���Ϣ--����������λС��λ��
						jilu[0][6] = wait_sec/60;													//��¼���γ˳���Ϣ--�ȴ�ʱ�䣨���ӣ�
						jilu[0][7] = wait_sec%60;													//��¼���γ˳���Ϣ--�ȴ�ʱ�䣨�룩
						jilu[0][8] = price/100;														//��¼���γ˳���Ϣ--�۸񣨰�λʮλ��
						jilu[0][9] = price%100;														//��¼���γ˳���Ϣ--�۸񣨸�λС��λ��
						for(j=0;j<10;j++)																	//�����μ�¼����24C02
						{
							for(k=0;k<10;k++)
							{
								At24c02_Write_Add(j*10+k,jilu[j][k]);
								HAL_Delay(2);
							}
						}
					}
					else if(flag_display == 2)
					{
						flag_display = 0;
					}
					if(flag_display < 2)
					{
						LCD_Clean_Half();
						LCD_Dis_Init();
					}
				}
			break;
				
			case 5:													//����5������RTCʱ��
				if(flag_display == 0)					//����ʱ������ģʽ
				{
					time_set++;
					if(time_set >= 4)						//�����õ�ʱ�����RTC��
					{
						HAL_RTC_SetTime(&hrtc,&RTC_Time,RTC_FORMAT_BIN);
						time_set = 0;
					}
				}
			break;

			case 6:													//����6��ʱ���
				switch(time_set)
				{
					case 1:
						RTC_Time.Hours++;
						if(RTC_Time.Hours >= 24) RTC_Time.Hours = 0;
					break;
					
					case 2:
						RTC_Time.Minutes++;
						if(RTC_Time.Minutes >= 60) RTC_Time.Minutes = 0;
					break;

					case 3:
						RTC_Time.Seconds++;
						if(RTC_Time.Seconds >= 60) RTC_Time.Seconds = 0;
					break;
				}
			break;

			case 7:													//����7��ʱ���
				switch(time_set)
				{
					case 1:
						RTC_Time.Hours--;
						if(RTC_Time.Hours >= 24) RTC_Time.Hours = 23;
					break;
					
					case 2:
						RTC_Time.Minutes--;
						if(RTC_Time.Minutes >= 60) RTC_Time.Minutes = 59;
					break;

					case 3:
						RTC_Time.Seconds--;
						if(RTC_Time.Seconds >= 60) RTC_Time.Seconds = 59;
					break;
				}
			break;
		}
	}
}

/****
*******��⺯��
*****/
void Monitor_function(void)
{
	if(time_set == 0)
		HAL_RTC_GetTime(&hrtc,&RTC_Time,RTC_FORMAT_BIN);//��ȡ��ǰRTCʱ��
	
	if(sec == 1)																//ÿ������������һ����Ϣ
	{
		sec=0;
		if(flag_display == 0) u1_printf("3�����𲽼�11Ԫ\r\n����ÿ����2.4Ԫ\r\nÿ������Ӱ�һ����Ʒ�\r\n");//���������շѱ�׼
		else if(flag_display==1 || flag_display==2)
		{
			if(flag_display == 1) u1_printf("�ٶȣ�%.2dkm/h\r\n",Motor_Speed);
			u1_printf("��������%d%.2d.%dkm\r\n",km/100000,km/1000%100,km/100%10);
			u1_printf("�ȴ�ʱ�䣺%.2d:%.2d\r\n",wait_sec/60,wait_sec%60);
			u1_printf("�۸�%d.%dRMB\r\n",price/10,price%10);
		}
		else if(flag_display == 3)
		{
			u1_printf("��%d�μ�¼\r\n",jilu_num + 1);
			u1_printf("ʱ�䣺%.2d:%.2d:%.2d \r\n",jilu[jilu_num][0],jilu[jilu_num][1],jilu[jilu_num][2]);
			u1_printf("��������%.2d%d.%dkm\r\n",jilu[jilu_num][4],jilu[jilu_num][5]/10%10,jilu[jilu_num][5]%10);
			u1_printf("�ȴ�ʱ�䣺%.2d:%.2d\r\n",jilu[jilu_num][6],jilu[jilu_num][7]);
			u1_printf("�۸�%d%d.%dRMB\r\n",jilu[jilu_num][8],jilu[jilu_num][9]/10%10,jilu[jilu_num][9]%10);
		}
	}
}

/****
*******��ʾ����
*****/
void Display_function(void)
{
	if(flag_display==0 || flag_display==1)
	{
		/* ��ʾСʱ�����ӡ���		*/
		sprintf(Dis_Buf,"%.2d:%.2d:%.2d",RTC_Time.Hours,RTC_Time.Minutes,RTC_Time.Seconds);
		Display_ASCII8X16_Color(32,16,(uchar *)Dis_Buf,BLACK,WHITE);
		
		if(time_set == 1) Lcd_DrawLine(32,30,BLACK);
		else if(time_set == 2) Lcd_DrawLine(32+3*8,30,BLACK);
		else if(time_set == 3) Lcd_DrawLine(32+6*8,30,BLACK);
	}
	
	if(flag_display == 1)
	{
		/* ��ʾ������ */
		sprintf(Dis_Buf,"%d%.2d.%dkm",km/100000,km/1000%100,km/100%10);
		Display_ASCII8X16_Color(70,60,(uchar *)Dis_Buf,BLACK,WHITE);
		/* ��ʾ�ٶ� */
		sprintf(Dis_Buf,"%.2dkm/h ",Motor_Speed);
		Display_ASCII8X16_Color(70,40,(uchar *)Dis_Buf,BLACK,WHITE);
		/* ��ʾ�ȴ�ʱ�� */
		sprintf(Dis_Buf,"%.2d:%.2d ",wait_sec/60,wait_sec%60);
		Display_ASCII8X16_Color(70,80,(uchar *)Dis_Buf,BLACK,WHITE);
		/* ��ʾ�۸� */
		if(km <= 3000) price = 110;																			//3km���ڼ۸�ȫ��11Ԫ
		else price = (wait_sec*24/300) + ((km-3000)*24/1000) + 110;			//3km�۸����
		sprintf(Dis_Buf,"%d.%dRMB ",price/10,price%10);
		Display_ASCII8X16_Color(70,100,(uchar *)Dis_Buf,BLACK,WHITE);
	}
	else if(flag_display == 3)																				//��ʾ��¼
	{
		sprintf(Dis_Buf,"%d  ",jilu_num + 1);
		Display_ASCII8X16_Color(0,0,(uchar *)Dis_Buf,BLACK,WHITE);
		/* ��ʾСʱ�����ӡ���		*/
		sprintf(Dis_Buf,"%.2d:%.2d:%.2d",jilu[jilu_num][0],jilu[jilu_num][1],jilu[jilu_num][2]);
		Display_ASCII8X16_Color(32,16,(uchar *)Dis_Buf,BLACK,WHITE);
		
		/* ��ʾ������ */
		sprintf(Dis_Buf,"%.2d%d.%dkm",jilu[jilu_num][4],jilu[jilu_num][5]/10%10,jilu[jilu_num][5]%10);
		Display_ASCII8X16_Color(70,60,(uchar *)Dis_Buf,BLACK,WHITE);
		/* ��ʾ�ȴ�ʱ�� */
		sprintf(Dis_Buf,"%.2d:%.2d ",jilu[jilu_num][6],jilu[jilu_num][7]);
		Display_ASCII8X16_Color(70,80,(uchar *)Dis_Buf,BLACK,WHITE);
		/* ��ʾ�۸� */
		sprintf(Dis_Buf,"%d%d.%dRMB ",jilu[jilu_num][8],jilu[jilu_num][9]/10%10,jilu[jilu_num][9]%10);
		Display_ASCII8X16_Color(70,100,(uchar *)Dis_Buf,BLACK,WHITE);
	}
}

/****
*******��ʾ���ݳ�ʼ������
*****/
void LCD_Dis_Init(void)
{
	switch(flag_display)												//���ݲ�ͬ����ʾ��־λ����ʾ��ͬ�Ľ�������
	{
		case 0:																		//����0
			/* ��ʾ�۸� */
			Display_ASCII8X16_Color(0,40,"3",BLACK,WHITE);
			Fast_DrawFont_GBK16(8,40,BLACK,WHITE,"�����𲽼�");
			Display_ASCII8X16_Color(6*16-8,40,"11",BLACK,WHITE);
			Fast_DrawFont_GBK16(7*16-8,40,BLACK,WHITE,"Ԫ");
			Fast_DrawFont_GBK16(0,60,BLACK,WHITE,"����ÿ����");
			Display_ASCII8X16_Color(5*16,60,"2.4",BLACK,WHITE);
			Fast_DrawFont_GBK16(7*16-8,60,BLACK,WHITE,"Ԫ");
			Fast_DrawFont_GBK16(24,80,BLACK,WHITE,"ÿ�������");
			Fast_DrawFont_GBK16(16,100,BLACK,WHITE,"��һ����Ʒ�");
		break;

		case 1:																		//����1
			Fast_DrawFont_GBK16(8,60,BLACK,WHITE,"������");
			Fast_DrawFont_GBK16(16,40,BLACK,WHITE,"�ٶ�");
			Fast_DrawFont_GBK16(0,80,BLACK,WHITE,"�ȴ�ʱ��");
			Fast_DrawFont_GBK16(16,100,BLACK,WHITE,"�۸�");
		break;

		case 3:																		//����3
			Fast_DrawFont_GBK16(8,60,BLACK,WHITE,"������");
			Fast_DrawFont_GBK16(0,80,BLACK,WHITE,"�ȴ�ʱ��");
			Fast_DrawFont_GBK16(16,100,BLACK,WHITE,"�۸�");
		break;
		
		default:
		break;
	}
}

void LCD_Clean_Half(void)//TFT���°���
{
	uint8_t j,k;
	Lcd_SetRegion(0,40,127,127);
	for(j=0;j<88;j++)
	{
		for(k=0;k<128;k++)
		{
			LCD_WriteData_16Bit(WHITE);
		}
	}
}

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

	uint8_t j,k;
	
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM1_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_RTC_Init();
  /* USER CODE BEGIN 2 */

	TFT_GPIO_Init();											//TFT��Ļ��س�ʼ��
	lcd_initial();
	dsp_single_colour(WHITE);
	
	HAL_TIM_Base_Start_IT(&htim1);				//ʹ�ܶ�ʱ��1�ж�		
	IIC_GPIO_Init();											//IIC��س�ʼ��

	for(j=0;j<10;j++)											//��ʮ�μ�¼
	{
		for(k=0;k<10;k++)
		{
			jilu[j][k] = At24c02_Read_Add(j*10+k);
			HAL_Delay(1);
		}
	}
	LCD_Dis_Init();											//��ʾ���ݳ�ʼ��
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1)
  {
		Key_function();										//��������
		Monitor_function();								//��⺯��
		Display_function();								//��ʾ����
		
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL16;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

uint8_t i;
uint16_t time,k;
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)//��ʱ���жϻص�����
{
	/* ������� */
	time++;
	if(time >= 3000)
	{
		time=0;
		sec=1;
		Motor_Speed = Motor_Num/20;
		if(flag_display == 1) km += ((Motor_Speed*1000)/3600)*3;
		Motor_Num = 0;
	}
	/* �ȴ�ʱ����� */
	if(flag_display==1 && Motor_Speed==0)
	{
		k++;
		if(k>=1000)
		{
			k=0;
			wait_sec++;
		}
	}
	
	/* ������� */
	i++;
	if(i >= 12) i=0;//ΪPWM�źŲ���һ������
	
	if(i < Motor_PWM) Motor_IN2(1);//����PWM�ź�
	else Motor_IN2(0);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)//�ⲿ�жϻص�����
{
	if(GPIO_Pin == GPIO_PIN_5)	//�ж��Ƿ���GPIO5�����ж�
	{
		Motor_Num++;
	}
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

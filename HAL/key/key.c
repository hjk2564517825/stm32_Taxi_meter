/**********************************
����ͷ�ļ�
**********************************/
#include "key.h"


/**********************************
��������
**********************************/
uint8_t chiclet_keyboard_num = 0;				//��ֵ����
uint8_t key_bz;


/**********************************
��������
**********************************/
/****
*******��������ɨ�躯��
*******����ֵ����ֵ
*****/
uint8_t Chiclet_Keyboard_Scan(void)
{
	if(K1==1 && K2==1 && K3==1 && K4==1 && K5==1 && K6==1 && K7==1) key_bz=1;
	
	if(K1==0 && key_bz==1)						//����K1Ϊ�͵�ƽ
	{
		key_bz=0;
		return 1;												//���ؼ�ֵ
	}

	if(K2==0 && key_bz==1)						//����K2Ϊ�͵�ƽ
	{
		key_bz=0;
		return 2;												//���ؼ�ֵ
	}

	if(K3==0 && key_bz==1)						//����K3Ϊ�͵�ƽ
	{
		key_bz=0;
		return 3;												//���ؼ�ֵ
	}
	
	if(K4==0 && key_bz==1)						//����K4Ϊ�͵�ƽ
	{
		key_bz=0;
		return 4;												//���ؼ�ֵ
	}

	if(K5==0 && key_bz==1)						//����K5Ϊ�͵�ƽ
	{
		key_bz=0;
		return 5;												//���ؼ�ֵ
	}
	
	if(K6==0 && key_bz==1)						//����K6Ϊ�͵�ƽ
	{
		key_bz=0;
		return 6;												//���ؼ�ֵ
	}
	
	if(K7==0 && key_bz==1)						//����K7Ϊ�͵�ƽ
	{
		key_bz=0;
		return 7;												//���ؼ�ֵ
	}
	
	return 0;													//û�а������£�����0
}
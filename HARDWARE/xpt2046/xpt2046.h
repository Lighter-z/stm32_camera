 
 
#ifndef XPT2046_H
#define XPT2046_H
#include "mcusys.h"

#define TP_PEN  	PCin(1)  	//PEN
#define TP_OUT 		PCin(3)   	//MISO

#define TP_DIN 		PCout(7)  	//MOSI
#define TP_CLK 		PCout(5)  	//SCK
#define TP_CS  		PCout(6)  	//CS  



#define xpt_flash_sector  200   //����У׼���� ��Ӧ�ⲿflash������200 1����4k�ֽ� ��Ӧ��ַΪ200*4096
//#define	xpt2046_CS_0  GPIO_ResetBits(GPIOA,GPIO_Pin_11)		   //Ƭѡ
//#define xpt2046_CS_1    GPIO_SetBits(GPIOA,GPIO_Pin_11)

#define xpt2046_isClick  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_1)		 //�жϴ������Ƿ��£�����ֵ����1ʱ����


struct TFT_Pointer			  //�������������Ľṹ�庯��  ��ҪӦ���� �Բɼ���ADֵ X Y  ���Ƿ��д������������ı�־λflag
{
 u16 x;
 u16 y;
 u8 flag;
};


struct T_i
{
 short xi;          //x ��������
 short yi;          //y ��������
 short a;		  //x ƫ����
 short b;	   	  //y ƫ����
};





void XPT2046_Init(void);             //xpt2046 IO�ڳ�ʼ��

struct TFT_Pointer TFT_Cm(void);     //TFT ����������

void T_correct(void);//����У׼����

u8 readflash_t(u8 sta);//��ȡУ׼����
#endif

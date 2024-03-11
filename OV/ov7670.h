	


#ifndef _OV7670_H
#define _OV7670_H
#include  "mcusys.h"
#include  "sccb.h"


#define OV7670_VSYNC  	PDin(0)			//ͬ���źż��IO
#define OV7670_WRST		  PDout(5)		//дָ�븴λ
#define OV7670_WREN		  PCout(13)		//д��FIFOʹ��
#define OV7670_RCK		  PDout(1)		//������ʱ��
#define OV7670_RRST		  PDout(6)  	//��ָ�븴λ
#define OV7670_CS		    PDout(7)  	//Ƭѡ�ź�(OE)


////����RCK  RRST	  
////ȥ��������λ��Ч��  ʹIO�ĵ����ٶȴﵽ���
//#define	OV7670_RCK_1    GPIOD->BSRRH=1<<1     //RCK�˿�  	 
//#define	OV7670_RCK_0    GPIOD->BSRRL=1<<1     //RCK�˿�  	 

//#define	OV7670_RRST_1    GPIOD->BSRRH=1<<6     //RRST�˿�  	 
//#define	OV7670_RRST_0    GPIOD->BSRRL=1<<6     //RRST�˿�  	     






															  					 
#define OV7670_DATA   GPIO_ReadInputData(GPIOA,0x00FF) 					//��������˿�
//GPIOC->IDR&0x00FF 
/////////////////////////////////////////
#define CHANGE_REG_NUM 							171			//��Ҫ���õļĴ�������		  
extern const u8 ov7670_init_reg_tbl[CHANGE_REG_NUM][2];		//�Ĵ����������ñ�
	    				 
u8 OV_Init(void);		 //ovģ���ʼ��
void OV_Gpio_Init(void); // OV��ӦIO��ʼ��
	  	   		 
void OV7670_Light_Mode(u8 mode);
void OV7670_Color_Saturation(u8 sat);
void OV7670_Brightness(u8 bright);
void OV7670_Contrast(u8 contrast);
void OV7670_Special_Effects(u8 eft);
void OV7670_Window_Set(u16 sx,u16 sy,u16 width,u16 height);
void EXTI0_Init(void);


u8 Ov_Photograph(void);	 //OV7670�򵥰���Ժ���

#endif






















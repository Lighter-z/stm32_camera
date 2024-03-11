
#include  "mcusys.h"
#include  "ov7670.h"
#include  "ov7670config.h"	  
#include  "delay.h"			 
#include  "sccb.h"
#include  "xpt2046.h"	
#include  "gui.h"
#include  "tft.h"
#include  "malloc.h"
#include  "led.h"
#include  "string.h"				//�ϲ�ָ�뺯��
#include  "OVbmp.h"
#include  "OVroot.h"

#define neg_film     30       //��Ƭ����

u8 ov_sta=0;		 //֡��־λ   �ж��е���

u8  pic=0;				 //��Ƭ�洢λ��

//�ⲿ�жϳ�ʼ��
void EXTI0_Init(void)
{   
  EXTI_InitTypeDef EXTI_InitStructure;			//�ж�����	  ���ĸ��ж���  EXTI_Line0-15
  //ģʽ EXTI_Mode_Interrupt�ж�  EXTI_Mode_Event �¼�
  //������ʽ  EXTI_Trigger_Falling �½��ش���
  //EXTI_Trigger_Rising	 �����ش���
  //EXTI_Trigger_Rising_Falling	  �����ƽ����

  NVIC_InitTypeDef NVIC_InitStructure;			//�жϲ��� �ж����ȼ�

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//ʹ��SYSCFGʱ��

  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, EXTI_PinSource0);//PD0 ���ӵ��ж���0

  /* ����EXTI_Line0 */ 
  EXTI_InitStructure.EXTI_Line=EXTI_Line0;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;			   //�ж�ģʽ
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;		   //�����½��ش���
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);	  	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���


  //�жϼ���Խ�� Խ����
  //���ж�  ��ռ=��Ӧ  ִ���ȷ���
  //�����ȼ�����ռ �ɶ� �����ȼ�
  //���ȼ����    ����Ӧ���ɶϵ���Ӧ
  NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;//�ⲿ�ж�
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;//��ռ���ȼ�0
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//�����ȼ�2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//ʹ���ⲿ�ж�ͨ��
  NVIC_Init(&NVIC_InitStructure);//����
 
}

 //�ⲿ�жϷ������
void EXTI0_IRQHandler(void)
{		 		
 
	if(EXTI_GetITStatus(EXTI_Line0)==SET)//��2�ߵ��ж�
	{     
		if(ov_sta<2)
		{
			if(ov_sta==0)
			{
				OV7670_WRST=0;	 	//��λдָ��		  		 
				OV7670_WRST=1;	
				OV7670_WREN=1;		//����д��FIFO
			}else 
			{
				OV7670_WREN=0;		//��ֹд��FIFO 
				OV7670_WRST=0;	 	//��λдָ��		  		 
				OV7670_WRST=1;	
			}
			ov_sta++;
		}
	}
 
	EXTI_ClearITPendingBit(EXTI_Line0);  //���EXTI2��·����λ						  
} 

void OV_Gpio_Init(void)   // OV��ӦIO��ʼ��
{
 	GPIO_InitTypeDef  GPIO_InitStructure;
 	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOD, ENABLE);	 //ʹ����ض˿�ʱ��

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_13; 	// ���� ����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
 	GPIO_Init(GPIOC, &GPIO_InitStructure);

	
 	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_1|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;	 // �˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
 	GPIO_Init(GPIOD, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin  = 0xff00; // ���� ����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��ͨ����ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
 	GPIO_Init(GPIOD, &GPIO_InitStructure);

 	SCCB_Init(); 
}
  			    
//��ʼ��OV7670
//����0:�ɹ�
//��������ֵ:�������
u8 OV_Init(void)
{
	u8 temp;
	u16 i=0;
	 
    //��ʼ��SCCB ��IO��	   	  
 	if(SCCB_WR_Reg(0x12,0x80)==0) return 1;	//��λSCCB	  
	delay_ms(50); 
	//��ȡ��Ʒ�ͺ�
 	temp=SCCB_RD_Reg(0x0b);   
	if(temp!=0x73) return 2; 
 	temp=SCCB_RD_Reg(0x0a);   
	if(temp!=0x76) return 2;
	//��ʼ������	  
	for(i=0;i<CHANGE_REG_NUM;i++)			  //д�Ĵ���ѭ��
	{
      SCCB_WR_Reg(ov7670_init_reg_tbl[i][0],ov7670_init_reg_tbl[i][1]);
	  delay_us(20);	
	}
    return 0;//ok
} 

////////////////////////////////////////////////////////////////////////////
//OV7670��������
//��ƽ������
//0:�Զ�
//1:̫��sunny
//2,����cloudy
//3,�칫��office
//4,����home
void OV7670_Light_Mode(u8 mode)
{
	u8 reg13val=0XE7;//Ĭ�Ͼ�������Ϊ�Զ���ƽ��
	u8 reg01val=0;
	u8 reg02val=0;
	switch(mode)
	{
		case 1://sunny
			reg13val=0XE5;
			reg01val=0X5A;
			reg02val=0X5C;
			break;	
		case 2://cloudy
			reg13val=0XE5;
			reg01val=0X58;
			reg02val=0X60;
			break;	
		case 3://office
			reg13val=0XE5;
			reg01val=0X84;
			reg02val=0X4c;
			break;	
		case 4://home
			reg13val=0XE5;
			reg01val=0X96;
			reg02val=0X40;
			break;	
	}
	SCCB_WR_Reg(0X13,reg13val);//COM8���� 
	SCCB_WR_Reg(0X01,reg01val);//AWB��ɫͨ������ 
	SCCB_WR_Reg(0X02,reg02val);//AWB��ɫͨ������ 
}				  
//ɫ������
//0:-2
//1:-1
//2,0
//3,1
//4,2
void OV7670_Color_Saturation(u8 sat)
{
	u8 reg4f5054val=0X80;//Ĭ�Ͼ���sat=2,��������ɫ�ȵ�����
 	u8 reg52val=0X22;
	u8 reg53val=0X5E;
 	switch(sat)
	{
		case 0://-2
			reg4f5054val=0X40;  	 
			reg52val=0X11;
			reg53val=0X2F;	 	 
			break;	
		case 1://-1
			reg4f5054val=0X66;	    
			reg52val=0X1B;
			reg53val=0X4B;	  
			break;	
		case 3://1
			reg4f5054val=0X99;	   
			reg52val=0X28;
			reg53val=0X71;	   
			break;	
		case 4://2
			reg4f5054val=0XC0;	   
			reg52val=0X33;
			reg53val=0X8D;	   
			break;	
	}
	SCCB_WR_Reg(0X4F,reg4f5054val);	//ɫ�ʾ���ϵ��1
	SCCB_WR_Reg(0X50,reg4f5054val);	//ɫ�ʾ���ϵ��2 
	SCCB_WR_Reg(0X51,0X00);			//ɫ�ʾ���ϵ��3  
	SCCB_WR_Reg(0X52,reg52val);		//ɫ�ʾ���ϵ��4 
	SCCB_WR_Reg(0X53,reg53val);		//ɫ�ʾ���ϵ��5 
	SCCB_WR_Reg(0X54,reg4f5054val);	//ɫ�ʾ���ϵ��6  
	SCCB_WR_Reg(0X58,0X9E);			//MTXS 
}
//��������
//0:-2
//1:-1
//2,0
//3,1
//4,2
void OV7670_Brightness(u8 bright)
{
	u8 reg55val=0X00;//Ĭ�Ͼ���bright=2
  	switch(bright)
	{
		case 0://-2
			reg55val=0XB0;	 	 
			break;	
		case 1://-1
			reg55val=0X98;	 	 
			break;	
		case 3://1
			reg55val=0X18;	 	 
			break;	
		case 4://2
			reg55val=0X30;	 	 
			break;	
	}
	SCCB_WR_Reg(0X55,reg55val);	//���ȵ��� 
}
//�Աȶ�����
//0:-2
//1:-1
//2,0
//3,1
//4,2
void OV7670_Contrast(u8 contrast)
{
	u8 reg56val=0X40;//Ĭ�Ͼ���contrast=2
  	switch(contrast)
	{
		case 0://-2
			reg56val=0X30;	 	 
			break;	
		case 1://-1
			reg56val=0X38;	 	 
			break;	
		case 3://1
			reg56val=0X50;	 	 
			break;	
		case 4://2
			reg56val=0X60;	 	 
			break;	
	}
	SCCB_WR_Reg(0X56,reg56val);	//�Աȶȵ��� 
}
//��Ч����
//0:��ͨģʽ    
//1,��Ƭ
//2,�ڰ�   
//3,ƫ��ɫ
//4,ƫ��ɫ
//5,ƫ��ɫ
//6,����	    
void OV7670_Special_Effects(u8 eft)
{
	u8 reg3aval=0X04;//Ĭ��Ϊ��ͨģʽ
	u8 reg67val=0XC0;
	u8 reg68val=0X80;
	switch(eft)
	{
		case 1://��Ƭ
			reg3aval=0X24;
			reg67val=0X80;
			reg68val=0X80;
			break;	
		case 2://�ڰ�
			reg3aval=0X14;
			reg67val=0X80;
			reg68val=0X80;
			break;	
		case 3://ƫ��ɫ
			reg3aval=0X14;
			reg67val=0Xc0;
			reg68val=0X80;
			break;	
		case 4://ƫ��ɫ
			reg3aval=0X14;
			reg67val=0X40;
			reg68val=0X40;
			break;	
		case 5://ƫ��ɫ
			reg3aval=0X14;
			reg67val=0X80;
			reg68val=0XC0;
			break;	
		case 6://����
			reg3aval=0X14;
			reg67val=0XA0;
			reg68val=0X40;
			break;	 
	}
	SCCB_WR_Reg(0X3A,reg3aval);//TSLB���� 
	SCCB_WR_Reg(0X68,reg67val);//MANU,�ֶ�Uֵ 
	SCCB_WR_Reg(0X67,reg68val);//MANV,�ֶ�Vֵ 
}	
//����ͼ���������
//��QVGA���á�
void OV7670_Window_Set(u16 sx,u16 sy,u16 width,u16 height)
{
	u16 endx;
	u16 endy;
	u8 temp; 
	endx=sx+width*2;	//V*2
 	endy=sy+height*2;
	if(endy>784)endy-=784;
	temp=SCCB_RD_Reg(0X03);				//��ȡVref֮ǰ��ֵ
	temp&=0XF0;
	temp|=((endx&0X03)<<2)|(sx&0X03);
	SCCB_WR_Reg(0X03,temp);				//����Vref��start��end�����2λ
	SCCB_WR_Reg(0X19,sx>>2);			//����Vref��start��8λ
	SCCB_WR_Reg(0X1A,endx>>2);			//����Vref��end�ĸ�8λ

	temp=SCCB_RD_Reg(0X32);				//��ȡHref֮ǰ��ֵ
	temp&=0XC0;
	temp|=((endy&0X07)<<3)|(sy&0X07);
	SCCB_WR_Reg(0X17,sy>>3);			//����Href��start��8λ
	SCCB_WR_Reg(0X18,endy>>3);			//����Href��end�ĸ�8λ
}




//�ԱȶȶԻ������ú���
void con_set(void)
{
 struct TFT_Pointer sp;  //���崥�������ṹ��

 Dialog_box(75,130,155,252,Blue,Black,window,"�Աȶ�����");

 GUI_arc(100,160,6,Black,0);
 GUI_arc(100,180,6,Black,0);
 GUI_arc(100,200,6,Black,0);
 GUI_arc(100,220,6,Black,0);
 GUI_arc(100,240,6,Black,0);
 GUI_sprintf_hzstr16x(122,152,"-2",Black,1);
 GUI_sprintf_hzstr16x(122,172,"-1",Black,1);
 GUI_sprintf_hzstr16x(130,192,"0",Black,1);
 GUI_sprintf_hzstr16x(130,212,"1",Black,1);
 GUI_sprintf_hzstr16x(130,232,"2",Black,1);
 while(1)
  {
   sp=TFT_Cm();
   if(sp.flag==1) 
    {
	 if(sp.x>75&&sp.x<155)
	  {
	    if(sp.y>152&&sp.y<169)
		 {
		  GUI_arc(100,160,5,Red,1);
		  OV7670_Contrast(0);
		  delay_ms(200);
		  break;
		 }
	    if(sp.y>172&&sp.y<189)
		 {
		  GUI_arc(100,180,5,Red,1);
		  OV7670_Contrast(1);
		  delay_ms(200);
		  break;
		 }
	    if(sp.y>192&&sp.y<209)
		 {
		  GUI_arc(100,200,5,Red,1);
		  OV7670_Contrast(2);
		  delay_ms(200);
		  break;
		 }
	    if(sp.y>212&&sp.y<229)
		 {
		  GUI_arc(100,220,5,Red,1);
		  OV7670_Contrast(3);
		  delay_ms(200);
		  break;
		 }
	    if(sp.y>232&&sp.y<259)
		 {
		  GUI_arc(100,240,5,Red,1);
		  OV7670_Contrast(4);
		  delay_ms(200);
		  break;
		 }	  
	  }
	  else break;
	
	}    
  }
}

//��ƽ������
void lig_set(void)
{
 struct TFT_Pointer sp;  //���崥�������ṹ��

 Dialog_box(75,150,155,272,Blue,Black,window,"��ƽ������");

 GUI_arc(90,180,6,Black,0);
 GUI_arc(90,200,6,Black,0);
 GUI_arc(90,220,6,Black,0);
 GUI_arc(90,240,6,Black,0);
 GUI_arc(90,260,6,Black,0);
 GUI_sprintf_hzstr16x(107,172,"�Զ�",Black,1);
 GUI_sprintf_hzstr16x(107,192,"̫��",Black,1);
 GUI_sprintf_hzstr16x(107,212,"����",Black,1);
 GUI_sprintf_hzstr16x(107,232,"�칫��",Black,1);
 GUI_sprintf_hzstr16x(107,252,"����",Black,1);
 while(1)
  {
   sp=TFT_Cm();
   if(sp.flag==1) 
    {
	 if(sp.x>75&&sp.x<155)
	  {
	    if(sp.y>172&&sp.y<189)
		 {
		  GUI_arc(90,180,5,Red,1);
		  OV7670_Light_Mode(0);
		  delay_ms(200);
		  break;
		 }
	    if(sp.y>192&&sp.y<209)
		 {
		  GUI_arc(90,200,5,Red,1);
		  OV7670_Light_Mode(1);
		  delay_ms(200);
		  break;
		 }
	    if(sp.y>212&&sp.y<229)
		 {
		  GUI_arc(90,220,5,Red,1);
		  OV7670_Light_Mode(2);
		  delay_ms(200);
		  break;
		 }
	    if(sp.y>232&&sp.y<249)
		 {
		  GUI_arc(90,240,5,Red,1);
		  OV7670_Light_Mode(3);
		  delay_ms(200);
		  break;
		 }
	    if(sp.y>252&&sp.y<279)
		 {
		  GUI_arc(90,260,5,Red,1);
		  OV7670_Light_Mode(4);
		  delay_ms(200);
		  break;
		 }	  
	  }
	  else break;
	
	}    
  }
}

//���ȶԻ������ú���
void bri_set(void)
{
 struct TFT_Pointer sp;  //���崥�������ṹ��

 Dialog_box(75,170,145,292,Blue,Black,window,"��������");

 GUI_arc(90,200,6,Black,0);
 GUI_arc(90,220,6,Black,0);
 GUI_arc(90,240,6,Black,0);
 GUI_arc(90,260,6,Black,0);
 GUI_arc(90,280,6,Black,0);
 GUI_sprintf_hzstr16x(112,192,"-2",Black,1);
 GUI_sprintf_hzstr16x(112,212,"-1",Black,1);
 GUI_sprintf_hzstr16x(120,232,"0",Black,1);
 GUI_sprintf_hzstr16x(120,252,"1",Black,1);
 GUI_sprintf_hzstr16x(120,272,"2",Black,1);
 while(1)
  {
   sp=TFT_Cm();
   if(sp.flag==1) 
    {
	 if(sp.x>75&&sp.x<155)
	  {
	    if(sp.y>192&&sp.y<209)
		 {
		  GUI_arc(90,200,5,Red,1);
		  OV7670_Brightness(0);
		  delay_ms(200);
		  break;
		 }
	    if(sp.y>212&&sp.y<229)
		 {
		  GUI_arc(90,220,5,Red,1);
		  OV7670_Brightness(1);
		  delay_ms(200);
		  break;
		 }
	    if(sp.y>232&&sp.y<249)
		 {
		  GUI_arc(90,240,5,Red,1);
		  OV7670_Brightness(2);
		  delay_ms(200);
		  break;
		 }
	    if(sp.y>252&&sp.y<269)
		 {
		  GUI_arc(90,260,5,Red,1);
		  OV7670_Brightness(3);
		  delay_ms(200);
		  break;
		 }
	    if(sp.y>272&&sp.y<289)
		 {
		  GUI_arc(90,280,5,Red,1);
		  OV7670_Brightness(4);
		  delay_ms(200);
		  break;
		 }	  
	  }
	  else break;
	
	}    
  }
}


//ɫ�ȶԻ������ú���
void col_set(void)
{
 struct TFT_Pointer sp;  //���崥�������ṹ��

 Dialog_box(75,190,145,312,Blue,Black,window,"ɫ������");

 GUI_arc(90,220,6,Black,0);
 GUI_arc(90,240,6,Black,0);
 GUI_arc(90,260,6,Black,0);
 GUI_arc(90,280,6,Black,0);
 GUI_arc(90,300,6,Black,0);
 GUI_sprintf_hzstr16x(112,212,"-2",Black,1);
 GUI_sprintf_hzstr16x(112,232,"-1",Black,1);
 GUI_sprintf_hzstr16x(120,252,"0",Black,1);
 GUI_sprintf_hzstr16x(120,272,"1",Black,1);
 GUI_sprintf_hzstr16x(120,292,"2",Black,1);
 while(1)
  {
   sp=TFT_Cm();
   if(sp.flag==1) 
    {
	 if(sp.x>75&&sp.x<155)
	  {
	    if(sp.y>212&&sp.y<229)
		 {
		  GUI_arc(90,220,5,Red,1);
		  OV7670_Color_Saturation(0);
		  delay_ms(200);
		  break;
		 }
	    if(sp.y>232&&sp.y<249)
		 {
		  GUI_arc(90,240,5,Red,1);
		  OV7670_Color_Saturation(1);
		  delay_ms(200);
		  break;
		 }
	    if(sp.y>252&&sp.y<269)
		 {
		  GUI_arc(90,260,5,Red,1);
		  OV7670_Color_Saturation(2);
		  delay_ms(200);
		  break;
		 }
	    if(sp.y>272&&sp.y<289)
		 {
		  GUI_arc(90,280,5,Red,1);
		  OV7670_Color_Saturation(3);
		  delay_ms(200);
		  break;
		 }
	    if(sp.y>292&&sp.y<309)
		 {
		  GUI_arc(90,300,5,Red,1);
		  OV7670_Color_Saturation(4);
		  delay_ms(200);
		  break;
		 }	  
	  }
	  else break;
	
	}    
  }
}



//��Ч����
void spe_set(void)
{
 struct TFT_Pointer sp;  //���崥�������ṹ��

 Dialog_box(75,70,155,232,Blue,Black,window,"��Ч����");

 GUI_arc(90,100,6,Black,0);
 GUI_arc(90,120,6,Black,0);
 GUI_arc(90,140,6,Black,0);
 GUI_arc(90,160,6,Black,0);
 GUI_arc(90,180,6,Black,0);
 GUI_arc(90,200,6,Black,0);
 GUI_arc(90,220,6,Black,0);
 GUI_sprintf_hzstr16x(107,92,"��ͨ",Black,1);
 GUI_sprintf_hzstr16x(107,112,"��Ƭ",Black,1);
 GUI_sprintf_hzstr16x(107,132,"�ڰ�",Black,1);
 GUI_sprintf_hzstr16x(107,152,"ƫ��ɫ",Black,1);
 GUI_sprintf_hzstr16x(107,172,"ƫ��ɫ",Black,1);
 GUI_sprintf_hzstr16x(107,192,"ƫ��ɫ",Black,1);
 GUI_sprintf_hzstr16x(107,212,"����",Black,1);
 while(1)
  {
   sp=TFT_Cm();
   if(sp.flag==1) 
    {
	 if(sp.x>75&&sp.x<155)
	  {
	    if(sp.y>92&&sp.y<109)
		 {
		  GUI_arc(90,100,5,Red,1);
		  OV7670_Special_Effects(0);
		  delay_ms(200);
		  break;
		  }
	    if(sp.y>112&&sp.y<129)
		 {
		  GUI_arc(90,120,5,Red,1);
		  OV7670_Special_Effects(1);
		  delay_ms(200);
		  break;
		 }
	    if(sp.y>132&&sp.y<149)
		 {
		  GUI_arc(90,140,5,Red,1);
		  OV7670_Special_Effects(2);
		  delay_ms(200);
		  break;
		 }
	    if(sp.y>152&&sp.y<169)
		 {
		  GUI_arc(90,160,5,Red,1);
		  OV7670_Special_Effects(3);
		  delay_ms(200);
		  break;
		 }
	    if(sp.y>172&&sp.y<189)
		 {
		  GUI_arc(90,180,5,Red,1);
		  OV7670_Special_Effects(4);
		  delay_ms(200);
		  break;
		 }
	    if(sp.y>192&&sp.y<209)
		 {
		  GUI_arc(90,200,5,Red,1);
		  OV7670_Special_Effects(5);
		  delay_ms(200);
		  break;
		 }
	    if(sp.y>212&&sp.y<229)
		 {
		  GUI_arc(90,220,5,Red,1);
		  OV7670_Special_Effects(6);
		  delay_ms(200);
		  break;
		 }	 		 	  
	  }
	  else break;
	
	}    
  }
}

//��Ƭ��������
//�����յ���Ƭ���Ѿ�����TF���ڵ�Ƭ����ʱ ����ʾ�����ڸ����µ���Ƭ��
//��ʱ���Կ��ǰ�TF�������Ƭ���Ƴ��� Ȼ��ָ���Ƭ����
//��Ƭ���ݣ���Ҫ�Ǵ���STC15��Ƭ��eeprom�е�0��ַ��һ���ֽ�
//          ������Ҫ������ �������ʱ ��Ƭ����ȡeeprom���� �ж�Ӧ�ø�����һ����Ƭ
//          ���� �Ѿ��ĺ���ǰ2��  ���ڵ�����ʱ  ��Ƭ�������ľ���2 ����������ϼ�1
//               �����ڵ�3����Ƭ�Ͽ�ʼ����  ��������Ժ� �ڰ����3 ����д�뵽eeprom��
//               �����Ƭ��30  ��30�Ŷ�����ʱ �Ǿ�Ҫ��neg_num�����eeprom��������30Ϊ0 
//               �������ܼ�����TF���ĵ�һ�ŵ�Ƭ��ʼ����
void neg_num(void)
{
  struct TFT_Pointer sp;  //���崥�������ṹ��
  Dialog_box(75,230,206,300,Red,Red,window,"��Ƭ����");
  GUI_sprintf_hzstr16x(76,252,"�ӵ�1�ŵ�Ƭ��ʼ��",Black,1);

  Touch_key(83,275,Blue2,Blue,0);     //ȷ����ť��
  Touch_key(160,275,Blue2,Blue,1);    //ȡ����ť��

  while(1)
  {
   sp=TFT_Cm();
   if(sp.flag==1)
    {
	  if(sp.y>275&&sp.y<300)
	  {
	    if(sp.x>83&&sp.x<123)
		 {
		  Touch_key(83,275,Gray,Black,0);     //ȷ����ť��
		  delay_ms(200);	
		  //IAP�޷�ʹ��
         // IapEraseSector(0);	    //����STC15оƬ�ϵ�eeprom�ռ�
         // IapProgramByte(0,0);	    //����Ƭ������0
		 pic=0;  //IAP��PIC����
		  GUI_sprintf_hzstr16x(75,302,"���ݸ�����ɣ�",White,1);
		  delay_ms(200);
		  break;
			  
		 }
	    if(sp.x>160&&sp.x<200)
		 {
		  Touch_key(160,275,Gray,Black,1);    //ȡ����ť��
		  delay_ms(300);
		  break;	  
		 }
	  }
	}
  
  }
}


//ov�������� 
//�ڵ����ĶԻ����п��Զ� OV�ĶԱȶ� ��ƽ�� ���� ɫ�� ��Ч��������
//�����Ը��µ�Ƭ���� eeprom
void ov_set(void)
{
 struct TFT_Pointer sp;  //���崥�������ṹ��

 Dialog_box(0,110,80,252,window,Blue,window,"�������");

 GUI_sprintf_hzstr16x(1,132,"�Աȶ�����",Black,1);
 GUI_sprintf_hzstr16x(1,152,"��ƽ������",Black,1);
 GUI_sprintf_hzstr16x(1,172,"��������",Black,1);
 GUI_sprintf_hzstr16x(1,192,"ɫ������",Black,1);
 GUI_sprintf_hzstr16x(1,212,"��Ч����",Black,1);
 GUI_sprintf_hzstr16x(1,232,"��Ƭ����",Black,1);

 while(1)
 {
   sp=TFT_Cm();
   if(sp.flag==1)   
    {
	  if(sp.x>1&&sp.x<110)
	  {
	    if(sp.y>130&&sp.y<149)
		 {
		   GUI_sprintf_hzstr16x(1,132,"�Աȶ�����",White,Red);
			 delay_ms(100);
		   con_set();
		   break;		 
		 } 
	    if(sp.y>152&&sp.y<169)
		 {
		   GUI_sprintf_hzstr16x(1,152,"��ƽ������",White,Red);
			 delay_ms(100);
		   lig_set();
		   break;
		 }
	    if(sp.y>172&&sp.y<189)
		 {
		   GUI_sprintf_hzstr16x(1,172,"��������",White,Red);
			 delay_ms(100);
		   bri_set();
		   break;
		 }
	    if(sp.y>192&&sp.y<209)
		 {
		   GUI_sprintf_hzstr16x(1,192,"ɫ������",White,Red);
			 delay_ms(100);
		   col_set();
		   break;
		 }
	    if(sp.y>212&&sp.y<229)
		 {
		   GUI_sprintf_hzstr16x(1,212,"��Ч����",White,Red);
			 delay_ms(100);
		   spe_set();
		   break;
		 }
	    if(sp.y>232&&sp.y<249)
		 {
		   GUI_sprintf_hzstr16x(1,232,"��Ƭ����",White,Red);
			 delay_ms(100);
		   neg_num();
		   break;
		 }	   
	  }
	  else break;	

	}	
 }

}


//���մ洢����
//�洢����ƬΪ240x240  16λɫBMP�ļ�
//�������ѡ��ʱ �ú���������Ļ�ϵ�ͼƬ�洢��TF��ָ��·����
//����������ǰ����:����ȷ��·���ϵ�BMP�ļ��Ǵ��ڵ� ���Ҵ�С������240x240
//˵����petit fatfs�ļ�ϵͳ���������µ��ļ� ֻ�������е��ļ��Ͻ��и���  
//      ������е��ļ��������֮Ϊ ����Ƭ��
u8 way[31][4]={"0","1","2","3","4","5","6","7","8","9","10",		//��Ƭ�洢·������
                    "11","12","13","14","15","16","17","18","19","20",
					"21","22","23","24","25","26","27","28","29","30",};
//u8  *ov;			  //ov�ĺõ���Ƭ �洢·��ָ��
void touch_pic(void)
{
//   u8  pic=0;				 //��Ƭ�洢λ��
	 u8 i;
   u8  ov[32];			   //ov�ĺõ���Ƭ �洢·��ָ��
	 for(i=0;i<32;i++)ov[i]=0;
	
   GUI_arc(200,280,14,Gray,1);		 //���հ�ť��̬��ʾЧ��
   delay_ms(200);
   GUI_arc(200,280,14,White,1);

   //IAP�޷�ʹ��
   //pic=IapReadByte(0);	  //��EEPROM��ַ0����ȡ �洢����Ƭ����
   pic+=1;                //׼������һ�Ŵ洢

   if(pic<=neg_film)	  //Ҫ�������ƬҪ�ڵ�Ƭ��Χ��
    {
   	 //ov=mymalloc(SRAMIN,32);	  //ov�洢·�������ڴ�
												//����Ҫ�洢��ͼƬ·��  ��"/ov76/m1.bmp"
												//��Ȼ��Щ·������TF������׼���õ�

     strcat((char*)ov,(const char*)"/ov76/m");									
     strcat((char*)ov,(const char*)way[pic]);   
     strcat((char*)ov,(const char*)".bmp");

	 led_d2=0;
	 GUI_sprintf_hzstr16x(0,241,"Begin save...Please wait!",Yellow,Black);
	 Write_Bmp((const char *)ov);								//��ʼ������Ƭ

	 led_d2=1;
	 GUI_sprintf_hzstr16x(0,241,"SAVE ok!                 ",Blue,Black);   //����ɹ�
	 delay_ms(300);

	 GUI_sprintf_hzstr16x(0,241,"Save to:/ov76/m  .bmp    ",Red,Black);	   //��ʾ�����·��
	 number(120,241,pic,Red,1);

	 delay_ms(800);    //������ʾ��ʱ
   GUI_sprintf_hzstr16x(0,241,"                         ",Red,Black);	   //����ʾ
	 //myfree(SRAMIN,ov);		//�ͷ�����Ĵ洢·��ָ��

	 //IAP�޷�ʹ��
     //IapEraseSector(0);	    //����STC15оƬ�ϵ�eeprom�ռ�
     //IapProgramByte(0,pic);	//����pic��ǰֵ
    }
   else if(pic>neg_film)
    {															 //����Ի���
	 Dialog_box(20,100,220,200,window,Red,window,"WARGING...");

	 GUI_sprintf_hzstr16x(25,120,"��û�е�Ƭ���Ը����ˣ���",Black,1);   
	 GUI_sprintf_hzstr16x(25,140,"�����TF����ĵ�Ƭ��",Black,1);
	 GUI_sprintf_hzstr16x(25,160,"�������������Ƭ���ݡ�",Black,1);  

	 delay_ms(2000);
	 delay_ms(2000);
	}
}


//ov������ʾ����
void ov_interface(void)
{
  GUI_Clear(Black);			    //��ɫ����

  GUI_arc(200,280,19,White,1);	//�����յ��ԲȦ
  GUI_arc(200,280,16,Black,1);
  GUI_arc(200,280,14,White,1);

  GUI_sprintf_hzstr16x(0,260,"�������",White,1);
  GUI_sprintf_hzstr16x(0,290,"���",White,1);

}  




//�������������
//֧������������� ���մ洢TF �򿪵�����Ṧ��
u8 Ov_Photograph(void)
{
	struct TFT_Pointer sp;  //���崥�������ṹ��
	u32  j;					 //��ʱ����
	// u8  sign=0;			 //��ʼ��־
	u16 color;
				     //�����ж�Ŀ��Ϊ  �ж�VSYNC Ϊ֡�����ж� ���д�����ʱ�� Ϊ��һ֡  
				                     //�����ʱ��ʼ�� ����оƬ AL422B����һ֡����
									 //�����ڶ�֡ʱ ˵�����ݹ������ ��ʱ��ȡ���� ������ʾ
	
	ov_interface();         //��ʾov����
				
	EXTI0_Init();					   //��ʼ���ж���
	OV7670_Window_Set(10,174,240,240);	//���ô���
	OV7670_CS=0;					   //ʹ������ͷƬѡ
	LCD_scan(5);   //����TFTɨ�跽�� Ϊ���µ��� ���ҵ���
	
	//TFT_RS_1;				   //����TFT д���ݶ� 
 
     	 
 //��ʼɨ�� ��ʾ����ͷ�ɼ�����
 while(1)
 {
   //OV7670_CS=1;								//�ص� ovʹ�ܶ�	 ����Ҫԭ�� ovʹ�ܶ� ��FLASH����һ��ʹ�ܿ� 
                                            //                 ��xpt2046 �� FLASH��ͬһSPI����  ���OVʹ�ܶ�ʹ��
											//                 ���൱�ڴ�FLASHƬѡ ������SPI�ϵ�XPT2046�Ĳɼ��������죩
   sp=TFT_Cm();	                            //ɨ�败����
   if(sp.flag==1)                           //�Ƿ��д����¼� ���� 
   { 
  	 if(sp.x>160&&sp.x<220&&sp.y>260&&sp.y<310) //������չ���
	 {
		 LCD_scan(1);   //����TFTɨ�跽�� Ϊ������ ���ϵ���
	   touch_pic();	  //���浱ǰ��Ƭ
		 LCD_scan(5);   //����TFTɨ�跽�� Ϊ���µ��� ���ҵ���
	 }
  	 if(sp.x>0&&sp.x<80)    
	 {
	   if(sp.y>280&&sp.y<310)	//������Ṧ��
	   {
				LCD_scan(1);   //����TFTɨ�跽�� Ϊ������ ���ϵ���
				GUI_sprintf_hzstr16x(0,290,"���",Gray,1); //�����Ч���仯
				delay_ms(150);
				tp_show("/ov76");
				ov_interface();      //��������ָ�OV����
				LCD_scan(5);   //����TFTɨ�跽�� Ϊ���µ��� ���ҵ���
	   }
	   if(sp.y>250&&sp.y<272)  //�������ѡ��
	   {
			  LCD_scan(1);   //����TFTɨ�跽�� Ϊ������ ���ϵ���
				GUI_sprintf_hzstr16x(0,260,"�������",Gray,1);
				delay_ms(150);
				GUI_sprintf_hzstr16x(0,260,"�������",White,1);
				ov_set();
				ov_interface();      //��������ָ�OV����
			  LCD_scan(5);   //����TFTɨ�跽�� Ϊ���µ��� ���ҵ���
	   }	   	 					
	  }	  
    }
//  // OV7670_CS=0;

		 
	if(ov_sta==2)				//�жϻ������Ƿ�����������
	{
	  Address_set(0,0,239,239,0,0);	  //������ʾ��Χ  ��ʾΪ240*240 
		TFT_RS=1;		                  //д����������		Ϊ���д���ٶ�
		 	 
		OV7670_RRST=0;				//��ʼ��λ��ָ�� 
		OV7670_RCK=0;
		OV7670_RCK=1;
		OV7670_RCK=0;		
		OV7670_RRST=1;				//��λ��ָ����� 
		OV7670_RCK=1;  
		
		//���÷� �ǽ� ����д���ݵĲ�����ȫ��  ��Ҫ��Ϊ�������ʾ�ٶ�  ������ע�Ͳ���Ϊ�ο����֣�
                     
		for(j=0;j<57600;j++)			   //�ֱ���Ϊ240x320   ÿ����ɫ��Ҫ�����ֽ� ���� 240x320x2=76800  ��
		{
			OV7670_RCK=0;				 //ÿһ��ʱ������ ��һ������
			color=GPIOD->IDR&0XFF00;	//������   ��ȡ��ɫ���ֽ�����
			OV7670_RCK=1; 

			OV7670_RCK=0;
			color|=((GPIOD->IDR&0XFF00)>>8);	//������   ��ȡ��ɫ���ֽ����� ��ת��Ϊ16λ 565 ��ɫ����
			OV7670_RCK=1; 
			
		  GPIOB->BSRRH = GPIO_Pin_All;      //��Ĵ�������
		  GPIOB->BSRRL = color;             //���ɼ�����ɫֵ��PB�ڼĴ���	    

		    TFT_WR=0;								      //��ʼд��
        TFT_WR=1;
//		  Lcd_Write_Data(color);
		}   							 
		EXTI_ClearITPendingBit(EXTI_Line0);  //���LINE�ϵ��жϱ�־λ
		ov_sta=0;					//��ʼ��һ�βɼ�
	} 
  }	
   	
}

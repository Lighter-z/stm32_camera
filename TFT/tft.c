#include "mcusys.h"
#include "tft.h"
#include "delay.h"


u16 lcd_id;

//#define DATAIN     GPIOB->IDR;   //��������

//	   �õ���GPIO  PBΪ���ݿ�	  CS PE.0    RS PC.4    WR PC.2     RD PC.0	  RES PE.4��λ
//								  BL PE.2
//

//tft  io�� ����
void Lcd_GPIO_Init(void)
{
   GPIO_InitTypeDef  GPIO_InitStructure;					 //����GPIO�ṹ��

 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOE, ENABLE);//ʹ��PC,PB,PEʱ��
// //PB 0-15��Ϊ���ݿ� ����Ҫ�ص� PB�ϵ�JTAG���� 	ֻ����SWJ����
// RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);	  //����AFIOʱ��
// GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); //����JTAG-DP Disabled and SW-DP Enabled 
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;			 //�˿�����
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
 
  GPIO_Init(GPIOB, &GPIO_InitStructure);				

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_2|GPIO_Pin_4;
  GPIO_Init(GPIOC, &GPIO_InitStructure);


  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_2|GPIO_Pin_4|GPIO_Pin_6; 
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  GPIO_ResetBits(GPIOE,GPIO_Pin_6);
}


//�⺯������
//�⺯���ڵ����� ��ԼĴ���Ҫ���ܶ� 

////���Ŷ���
//void TFT_RS(unsigned int d)	//����/����,0=����,1=����
//{
// if(d==1)
// GPIO_SetBits(GPIOC,GPIO_Pin_10);// GPIOC10;
// else if(d==0)
//  GPIO_ResetBits(GPIOC,GPIO_Pin_10); //GPIOC10;
//} 
//
//
//void TFT_WR(unsigned int d)		   //д����
//{
// if(d==1)
// GPIO_SetBits(GPIOC,GPIO_Pin_12);// GPIOC12;
// else if(d==0)
//  GPIO_ResetBits(GPIOC,GPIO_Pin_12);//GPIOC12;
//}                  
//
//void TFT_RD(unsigned int d)		   //������
//{									   
// if(d==1)
// GPIO_SetBits(GPIOC,GPIO_Pin_14);// GPIOC14;
// else if(d==0)
//  GPIO_ResetBits(GPIOC,GPIO_Pin_14);//GPIOC14;
//}     
//               
//void TFT_CS(unsigned int d)		   //Ƭѡ
//{
// if(d==1)
//  GPIO_SetBits(GPIOE,GPIO_Pin_0);//GPIOE0;
// else if(d==0)
//  GPIO_ResetBits(GPIOE,GPIO_Pin_0);//GPIOE0;
//}                 
//void TFT_RST(unsigned int d)	   //��λ
//{
// if(d==1)
// GPIO_SetBits(GPIOE,GPIO_Pin_4);// GPIOE4;
// else if(d==0)
// GPIO_ResetBits(GPIOE,GPIO_Pin_4);// GPIOE4;
//}                   
void TFT_DATA(unsigned int data)   //���ݽ�PD��
{
   GPIOB->BSRRH = GPIO_Pin_All;
   GPIOB->BSRRL = data;

//   GPIO_ResetBits(GPIOD,GPIO_Pin_All);//GPIOD=0xffff;
//   GPIO_SetBits(GPIOD,data);//GPIOB->BSRR=data;
}  

                       



//��ʾλ�üĴ�������
// 0x0050   //X����ʼλ��
// 0x0051   //X�����λ��
// 0x0052   //Y����ʼλ��
// 0x0053   //Y�����λ��
// 0x0020   //��ǰ����X��λ��
// 0x0021   //��ǰ����Y��λ��



//д����
void Lcd_WR_Reg(u16 data)
{
 TFT_RS=0;       //ѡ������
 TFT_DATA(data);  //��ֵ
 TFT_WR=0;
 TFT_WR=1;       //д�����ߵ�ƽ
}

//д����
void Lcd_Write_Data(u16 data)
{
 TFT_RS=1;       //ѡ������
 TFT_DATA(data);  //��ֵ
 TFT_WR=0;
 TFT_WR=1;       //д�����ߵ�ƽ
}

//��ĳһ����ַ���߼Ĵ�����д��һ��16λֵ
void Lcd_WriteReg(u16 com,u16 val)	
{
 Lcd_WR_Reg(com);
 Lcd_Write_Data(val);
}





//��ȡ lcd�ڲ�ָ���Ĵ�������
//���� LCD_Reg lcd�Ĵ�������
//��� ��ӦLCD�Ĵ��������� ����  
u16 LCD_ReadReg(u16 LCD_Reg)
{										   
	u16 t;

  GPIO_InitTypeDef  GPIO_InitStructure;					 //����GPIO�ṹ��

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);


  Lcd_WR_Reg(LCD_Reg);  //д��Ҫ���ļĴ�����  

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;			 //�˿�����
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_Init(GPIOB, &GPIO_InitStructure);


	TFT_RS=1;
	//��ȡ����(���Ĵ���ʱ,������Ҫ��2��)
	TFT_RD=0;
	delay_us(5);//FOR 8989,��ʱ5us					   
	TFT_RD=1;

	t=GPIO_ReadInputData(GPIOB);
 

 // GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;			 //�˿�����
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
 // GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_Init(GPIOB, &GPIO_InitStructure);

	return t;  
}




//��ILI93xx����������ΪBGR��ʽ��������д���ʱ��ΪRGB��ʽ��	 ��ʵ��֤���ֲ��ﲢû��˵����
//ͨ���ú���ת��
//c:BGR��ʽ����ɫֵ
//����ֵ��RGB��ʽ����ɫֵ
u16 LCD_bgr2rgb(u16 c)
{
	u16  r,g,b,rgb;   
	b=(c>>0)&0x1f;
	g=(c>>5)&0x3f;
	r=(c>>11)&0x1f;	 
	rgb=(b<<11)+(g<<5)+(r<<0);		 
	return(rgb);
} 



//��ȡ��ĳ�����ɫֵ	  ���ô˺��� Ҫ�ǵ�����ioΪ����
//x,y:����
//����ֵ:�˵����ɫ
u16 LCD_readpoint(u16 x,u16 y)
{
	u16 color,m0=0;
	u8 r=0,g=0,b=0;

  GPIO_InitTypeDef  GPIO_InitStructure;					 //����GPIO�ṹ��

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

  if(lcd_id==9325)
  {
  	Lcd_WR_Reg(0x0020);		  //���ö���ɫ����
	Lcd_Write_Data(x);	  
    Lcd_WR_Reg(0x0021);
	Lcd_Write_Data(y); 

	Lcd_WR_Reg(0x0022);		  //���Ͷ���ɫ����
	}

   else if(lcd_id==9341)
   {
	Lcd_WR_Reg(0X2A); 
	Lcd_Write_Data(x>>8); 
	Lcd_Write_Data(x&0XFF);	 
	Lcd_Write_Data(x>>8); 
	Lcd_Write_Data(x&0XFF);	

	Lcd_WR_Reg(0X2B); 
	Lcd_Write_Data(y>>8); 
    Lcd_Write_Data(y&0XFF);
	Lcd_Write_Data(y>>8); 
    Lcd_Write_Data(y&0XFF);

    Lcd_WR_Reg(0x2E);		   //���Ͷ���ɫ����
	}



  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All; //�˿�����Ϊ����״̬
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//100MHz
  GPIO_Init(GPIOB, &GPIO_InitStructure);


  TFT_RS=1;						  //1Ϊ������

  TFT_RD=0;					   	  //���ж�����
  TFT_RD=1;
 //dummy READ					  //��һ�ν���Ϊ��������  �ֲ�69ҳ �����
	delay_us(5);
  TFT_RD=0;					   
  TFT_RD=1;

	
  m0=GPIO_ReadInputData(GPIOB);

  if(lcd_id==9325) color=m0;//LCD_bgr2rgb(m0);


  else if(lcd_id==9341)
	{	//��ȡ����(��GRAMʱ,��Ҫ��2��)
		r=m0>>8;//P2;
		g=m0;//P0;
		delay_us(10);
	
		TFT_RD=0;					   
	 	TFT_RD=1;  
		m0=GPIO_ReadInputData(GPIOB);
		b=m0>>8;//P2;
			
		color=(((u16)r>>3)<<11)|(((u16)g>>2)<<5)|((u16)b>>3);
		
	}


//GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;//�˿��������ģʽ
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
//GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_Init(GPIOB, &GPIO_InitStructure);


  return color;		  //���ض�ȡ����ɫ		  
}




//�������귶Χ		
//x1 y1  ��ʼλ��
//x2 y2  ����λ��
//xq yq  ��ʼ������	 (9341����Ҫ���ֵ ��������0����)
void Address_set(u16 x1,u16 y1,u16 x2,u16 y2,u16 xq,u16 yq)
{
  if(lcd_id==9325)
  {
	Lcd_WR_Reg(0x0050);Lcd_Write_Data(x1);		//����Ҫ��ʾ������
	Lcd_WR_Reg(0x0051);Lcd_Write_Data(x2);
	Lcd_WR_Reg(0x0052);Lcd_Write_Data(y1);
	Lcd_WR_Reg(0x0053);Lcd_Write_Data(y2);  
    Lcd_WR_Reg(0x0020);Lcd_Write_Data(xq);	  	//������ʾ�ĳ�ʼλ��
    Lcd_WR_Reg(0x0021);Lcd_Write_Data(yq); 
    Lcd_WR_Reg(0x0022);							//��������
  } 
  else if(lcd_id==9341)
  {
		Lcd_WR_Reg(0X2A); 
		Lcd_Write_Data(x1>>8); 
		Lcd_Write_Data(x1&0XFF);
		Lcd_Write_Data(x2>>8); 
		Lcd_Write_Data(x2&0XFF);

		Lcd_WR_Reg(0X2B); 
		Lcd_Write_Data(y1>>8); 
		Lcd_Write_Data(y1&0XFF);
		Lcd_Write_Data(y2>>8); 
		Lcd_Write_Data(y2&0XFF);
	   Lcd_WR_Reg(0X2C);      //��ʼд��GRAM
   }
}




/*
9341ɨ���ص� ���ɨ�跽��ı�� ��ֱ�Ӹı�x y��ʼԭ��
�����ں�����ʾ��ʱ�� �Ͳ��ÿ��� x y�ı仯��Ľ�� ��Ϊԭ��Ҳ�����ı�
���ǿ���һ�ִ���ͨ��

*/

//tft������ɨ�跽ʽ����
//mode 1 -8  һ��8��ɨ�跽ʽ
//     1   ��>��  ��>�� 
//     2   ��>��  ��>��
//     3   ��>��  ��>��
//     4   ��>��  ��>�� 
//     5   ��>��  ��>��  
//     6   ��>��  ��>�� 
//     7   ��>��  ��>�� 
//     8   ��>��  ��>�� 
void LCD_scan(u8 mode)
{
 switch(mode)
 {
   case 1:
	Lcd_WR_Reg(0x36);    // Memory Access Control 	   saomiao
	Lcd_Write_Data(0x08); 	
    Lcd_WR_Reg(0X2C);      //��ʼд��GRAM			 
	break;
   case 2:
	Lcd_WR_Reg(0x36);    // Memory Access Control 	   saomiao
	Lcd_Write_Data(0x88); 
	Lcd_WR_Reg(0X2C);      //��ʼд��GRAM					 
	break;
   case 3:
	Lcd_WR_Reg(0x36);    // Memory Access Control 	   saomiao
	Lcd_Write_Data(0x48); 	
	Lcd_WR_Reg(0X2C);      //��ʼд��GRAM				
	break;
   case 4:
	Lcd_WR_Reg(0x36);    // Memory Access Control 	   saomiao
	Lcd_Write_Data(0xc8); 	
	Lcd_WR_Reg(0X2C);      //��ʼд��GRAM				 
	break;
   case 5:
	Lcd_WR_Reg(0x36);    // Memory Access Control 	   saomiao
	Lcd_Write_Data(0x28); 
	Lcd_WR_Reg(0X2C);      //��ʼд��GRAM					 
	break;
   case 6:
	Lcd_WR_Reg(0x36);    // Memory Access Control 	   saomiao
	Lcd_Write_Data(0x68); 	
	Lcd_WR_Reg(0X2C);      //��ʼд��GRAM				 
	break;
   case 7:
	Lcd_WR_Reg(0x36);    // Memory Access Control 	   saomiao
	Lcd_Write_Data(0xa8); 	
	Lcd_WR_Reg(0X2C);      //��ʼд��GRAM				 
	break;
   case 8:
	Lcd_WR_Reg(0x36);    // Memory Access Control 	   saomiao
	Lcd_Write_Data(0xe8); 
	Lcd_WR_Reg(0X2C);      //��ʼд��GRAM					 
	break;
 }
}














//TFT��ʼ��
void Lcd_Init(void)
{
 Lcd_GPIO_Init();	//LCD ����GPIO������

 TFT_RST=1;
 delay_ms(5);	
 TFT_RST=0;
 delay_ms(5) ;
 TFT_RST=1;

 TFT_CS=1;
 TFT_RD=1;
 TFT_WR=1;
 delay_ms(5) ;
 TFT_CS=0;


	delay_ms(50); // delay 50 ms 
	Lcd_WriteReg(0x0000,0x0001);
	delay_ms(50); // delay 50 ms 
	lcd_id = LCD_ReadReg(0x0000); 		 //��ȡ lcd id���� �ж�lcd�ͺ�

	if(lcd_id==0x9325||lcd_id==0x9328||lcd_id==0x933d||lcd_id==0x9320||lcd_id==0x9338)//���Ͼ�ΪILI9325����ϵ�� ����ͬһ����
	{	 
		lcd_id=9325;
		 
		Lcd_WriteReg(0x0001,0x0100);	//Driver Output Contral.	
		Lcd_WriteReg(0x0002,0x0700);	//LCD Driver Waveform Contral.
		Lcd_WriteReg(0x0003,0x1030);	//LCD��ʾ��������   �ֲ�56ҳ
		Lcd_WriteReg(0x0004,0x0000);	//Scalling Contral.			
		Lcd_WriteReg(0x0008,0x0202);	//Display Contral 2.(0x0207)		
		Lcd_WriteReg(0x0009,0x0000);	//Display Contral 3.(0x0000)
		Lcd_WriteReg(0x000A,0x0000);	//Frame Cycle Contal.(0x0000)
		Lcd_WriteReg(0x000C,0x0000);	
		Lcd_WriteReg(0x000D,0x0000);	//Frame Maker Position.
		Lcd_WriteReg(0x000F,0x0000);	//Extern Display Interface Contral 2.
	
		Lcd_WriteReg(0x0010,0x0000);	
		Lcd_WriteReg(0x0011,0x0007);								//Power Control 2.(0x0001)				    //Power Control 3.(0x0138)
		Lcd_WriteReg(0x0012,0x0000);	
		Lcd_WriteReg(0x0013,0x0000);								//Power Control 4.
		Lcd_WriteReg(0x0007,0x0001);								//Power Control 7.
		delay_ms(50);
	
	    Lcd_WriteReg(0x0010,0x1690);
		Lcd_WriteReg(0x0011,0x0227);
		delay_ms(50);
		Lcd_WriteReg(0x0012,0x009D);
		delay_ms(50);
		Lcd_WriteReg(0x0013,0x1900);	
		delay_ms(50);
		Lcd_WriteReg(0x0029,0x0025);	
		Lcd_WriteReg(0x002B,0x000D);
		delay_ms(50);
		Lcd_WriteReg(0x0020,0x0000);	//�е�ַ���� x
		Lcd_WriteReg(0x0021,0x0000);	//�е�ַ���� y
		delay_ms(50);
		Lcd_WriteReg(0x0030,0x0007);	
		Lcd_WriteReg(0x0031,0x0303);	
		Lcd_WriteReg(0x0032,0x0003);	
		Lcd_WriteReg(0x0035,0x0206);	
		Lcd_WriteReg(0x0036,0x0008);	
		Lcd_WriteReg(0x0037,0x0406);	
		Lcd_WriteReg(0x0038,0x0304);
		Lcd_WriteReg(0x0039,0x0007);	
		Lcd_WriteReg(0x003C,0x0601);	
		Lcd_WriteReg(0x003D,0x0008);			
						
		Lcd_WriteReg(0x0050,0x0000);	//����ʼ��ַ		  ��ʼ��������Ļ��ʾ��ΧΪ240x320
		Lcd_WriteReg(0x0051,0x00EF);	//�н�����ַ
		Lcd_WriteReg(0x0052,0x0000);	//����ʼ��ַ
		Lcd_WriteReg(0x0053,0x013F);	//�н�����ַ
		Lcd_WriteReg(0x0060,0xA700);	
		Lcd_WriteReg(0x0061,0x0001);	
		Lcd_WriteReg(0x006A,0x0000);	
		
		Lcd_WriteReg(0x0080,0x0000);	//Display Position? Partial Display 1.
		Lcd_WriteReg(0x0081,0x0000);	//RAM Address Start? Partial Display 1.
		Lcd_WriteReg(0x0082,0x0000);	//RAM Address End-Partial Display 1.
		Lcd_WriteReg(0x0083,0x0000);	//Displsy Position? Partial Display 2.
		Lcd_WriteReg(0x0084,0x0000);	//RAM Address Start? Partial Display 2.
		Lcd_WriteReg(0x0085,0x0000);	//RAM Address End? Partial Display 2.
	
		Lcd_WriteReg(0x0090,0x0010);	
		Lcd_WriteReg(0x0092,0x0600);	//Panel Interface Contral 2.(0x0000)
	
		Lcd_WriteReg(0x0007,0x0133);	//D1=D0=BASEE=1 ��ͼ����ʾ   GON=DTE=1   ������ʾ  �ø�����������ʾ������ �Դﵽ���͹���Ч��
	       
	    Lcd_WR_Reg(0x0022);

	}
	else   //9341����		9341��Ҫ�ص����� �ı�ɨ�跽��� ��ʼԭ��Ҳ��֮�ı� ����lcd������
	{
	    lcd_id=9341;

		Lcd_WR_Reg(0xCF);  
		Lcd_Write_Data(0x00); 
		Lcd_Write_Data(0xC1); 
		Lcd_Write_Data(0X30); 
		Lcd_WR_Reg(0xED);  
		Lcd_Write_Data(0x64); 
		Lcd_Write_Data(0x03); 
		Lcd_Write_Data(0X12); 
		Lcd_Write_Data(0X81); 
		Lcd_WR_Reg(0xE8);  
		Lcd_Write_Data(0x85); 
		Lcd_Write_Data(0x10); 
		Lcd_Write_Data(0x7A); 
		Lcd_WR_Reg(0xCB);  
		Lcd_Write_Data(0x39); 
		Lcd_Write_Data(0x2C); 
		Lcd_Write_Data(0x00); 
		Lcd_Write_Data(0x34); 
		Lcd_Write_Data(0x02); 
		Lcd_WR_Reg(0xF7);  
		Lcd_Write_Data(0x20); 
		Lcd_WR_Reg(0xEA);  
		Lcd_Write_Data(0x00); 
		Lcd_Write_Data(0x00); 
		Lcd_WR_Reg(0xC0);    //Power control 
		Lcd_Write_Data(0x1B);   //VRH[5:0] 
		Lcd_WR_Reg(0xC1);    //Power control 
		Lcd_Write_Data(0x01);   //SAP[2:0];BT[3:0] 
		Lcd_WR_Reg(0xC5);    //VCM control 
		Lcd_Write_Data(0x30); 	 //3F
		Lcd_Write_Data(0x30); 	 //3C
		Lcd_WR_Reg(0xC7);    //VCM control2 
		Lcd_Write_Data(0XB7); 
		Lcd_WR_Reg(0x36);    // Memory Access Control 	   saomiao
		Lcd_Write_Data(0x08); 
		Lcd_WR_Reg(0x3A);   
		Lcd_Write_Data(0x55); 
		Lcd_WR_Reg(0xB1);   
		Lcd_Write_Data(0x00);   
		Lcd_Write_Data(0x1A); 
		Lcd_WR_Reg(0xB6);    // Display Function Control 
		Lcd_Write_Data(0x0A); 
		Lcd_Write_Data(0xA2); 
		Lcd_WR_Reg(0xF2);    // 3Gamma Function Disable 
		Lcd_Write_Data(0x00); 
		Lcd_WR_Reg(0x26);    //Gamma curve selected 
		Lcd_Write_Data(0x01); 
		Lcd_WR_Reg(0xE0);    //Set Gamma 
		Lcd_Write_Data(0x0F); 
		Lcd_Write_Data(0x2A); 
		Lcd_Write_Data(0x28); 
		Lcd_Write_Data(0x08); 
		Lcd_Write_Data(0x0E); 
		Lcd_Write_Data(0x08); 
		Lcd_Write_Data(0x54); 
		Lcd_Write_Data(0XA9); 
		Lcd_Write_Data(0x43); 
		Lcd_Write_Data(0x0A); 
		Lcd_Write_Data(0x0F); 
		Lcd_Write_Data(0x00); 
		Lcd_Write_Data(0x00); 
		Lcd_Write_Data(0x00); 
		Lcd_Write_Data(0x00); 		 
		Lcd_WR_Reg(0XE1);    //Set Gamma 
		Lcd_Write_Data(0x00); 
		Lcd_Write_Data(0x15); 
		Lcd_Write_Data(0x17); 
		Lcd_Write_Data(0x07); 
		Lcd_Write_Data(0x11); 
		Lcd_Write_Data(0x06); 
		Lcd_Write_Data(0x2B); 
		Lcd_Write_Data(0x56); 
		Lcd_Write_Data(0x3C); 
		Lcd_Write_Data(0x05); 
		Lcd_Write_Data(0x10); 
		Lcd_Write_Data(0x0F); 
		Lcd_Write_Data(0x3F); 
		Lcd_Write_Data(0x3F); 
		Lcd_Write_Data(0x0F); 
		Lcd_WR_Reg(0x2B); 
		Lcd_Write_Data(0x00);
		Lcd_Write_Data(0x00);
		Lcd_Write_Data(0x01);
		Lcd_Write_Data(0x3f);
		Lcd_WR_Reg(0x2A); 
		Lcd_Write_Data(0x00);
		Lcd_Write_Data(0x00);
		Lcd_Write_Data(0x00);
		Lcd_Write_Data(0xef);	 
		Lcd_WR_Reg(0x11); //Exit Sleep
		delay_ms(120);
		Lcd_WR_Reg(0x29); //display on	

	 }

}






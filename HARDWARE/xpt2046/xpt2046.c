
 
#include "mcusys.h"
#include "math.h"
#include "xpt2046.h"
#include "delay.h"
#include "spi.h"
#include "gui.h"
#include "tft.h"
#include "flash.h"
#include "stdlib.h"
#include "math.h"
#include "led.h"
#include "key.h"

/*
  xpt2046
  AD_DCLK  PC5
  AD_CS    PC6
  AD_DIN   PC7
  AD_OUT   PC3			��ȡ������������
  AD_PEN   PC1
*/



u8 flash_tp[9];		//flash ����У׼���ݻ�����



//������У׼����
//���û��У׼��flash��  ʹ��Ĭ��ֵ
struct T_i T_flash=
{
 -657,		//�������� ��ֵҪ����10000������ʵֵ  ��ΪΪ�˷��㺯������ ����ʱ����10000 
 -901,		//�������� ��ֵҪ����10000������ʵֵ  ��ΪΪ�˷��㺯������ ����ʱ����10000 
  251,
  357,
};


//4KbytesΪһ��Sector
//16������Ϊ1��Block
//W25X16
//����Ϊ2M�ֽ�,����32��Block,512��Sector 

//��ȡFLASH�е�У׼�������� ����200 ��ַ0-7 �津������  8���־
//��ȡǰ  ����Ҫ�ж� ��ַ8  ��У׼��־λ �Ƿ�Ϊ0xf4
//sta ������ �Ƿ��flash�ж�ȡУ׼��������       1 ��ȡ   0 ����ȡ 
//����ֵ ���Ϊ0  ˵���Ѿ�У׼���������� Ȼ��ͨ��sta�ж��Ƿ���ȡflash�е�У׼���� ��Ϊ�����жϱ�׼
//����ֵ ���Ϊ1  ˵��û��У׼��   ֱ������У׼�����ṹ�� T_flash����ı������������ֵҲ��У׼��ֱ����д��������ģ�
u8 readflash_t(u8 sta)
{
SPI_Flash_Read(flash_tp,xpt_flash_sector*4096,9);


 if(sta==1&&flash_tp[8]==0xF4)    //��ȡУ׼���� 1 ��У׼�� ������У׼�� �ýṹ�屾����
 {
	T_flash.xi=((u16)flash_tp[1]<<8)+flash_tp[0];		  //����ȡ��У׼���ݸ��µ�������
	T_flash.yi=((u16)flash_tp[3]<<8)+flash_tp[2];
	T_flash.a=((u16)flash_tp[5]<<8)+flash_tp[4];
	T_flash.b=((u16)flash_tp[7]<<8)+flash_tp[6];
	return 0;              //��ȡ�ɹ�
 }
 else if(sta==0&&flash_tp[8]==0xF4) return 0;
 return 1;                 //û��У׼
}





//��ʼ�� ����оƬXPT2046 SPIƬѡIO ���ж�IO
void XPT2046_Init(void)
{	
  	GPIO_InitTypeDef GPIO_InitStructure;


	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOC, ENABLE);//ʹ��GPIOB,C,Fʱ��


    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1| GPIO_Pin_3;//PC1/PC3 ����Ϊ��������
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//����ģʽ
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
    GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��

	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_5|GPIO_Pin_7;//PC5 PC6 PC7����Ϊ�������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//���ģʽ
	GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��
		
		
}



//Ĭ��Ϊtouchtype=0������.
u8 CMD_RDX=0XD0;
u8 CMD_RDY=0X90;



//SPIд����
//������ICд��1byte����    
//num:Ҫд�������
void TP_Write_Byte(u8 num)    
{  
	u8 count=0;   
	for(count=0;count<8;count++)  
	{ 	  
		if(num&0x80)TP_DIN=1;  
		else TP_DIN=0;   
		num<<=1;    
		TP_CLK=0; 
		delay_us(1);
		TP_CLK=1;		//��������Ч	        
	}		 			    
} 


//SPI������ 
//�Ӵ�����IC��ȡadcֵ
//CMD:ָ��
//����ֵ:����������	   
u16 TP_Read_AD(u8 CMD)	  
{ 	 
	u8 count=0; 	  
	u16 Num=0; 
	TP_CLK=0;		//������ʱ�� 	 
	TP_DIN=0; 	//����������
	TP_CS=0; 		//ѡ�д�����IC
	TP_Write_Byte(CMD);//����������
	delay_us(6);//ADS7846��ת��ʱ���Ϊ6us
	TP_CLK=0; 	     	    
	delay_us(1);    	   
	TP_CLK=1;		//��1��ʱ�ӣ����BUSY
	delay_us(1);    
	TP_CLK=0; 	     	    
	for(count=0;count<16;count++)//����8λ���� 
	{ 				  
		Num<<=1; 	 
		TP_CLK=0;	//�½�����Ч  	    	   
		delay_us(1);    
 		TP_CLK=1;
 		if(TP_OUT)Num++; 		 
	}  	
	Num>>=4;   	//ֻ�и�12λ��Ч.
	TP_CS=1;		//�ͷ�Ƭѡ	 
	return(Num);   
}



//��ȡһ������ֵ(x����y)
//������ȡREAD_TIMES������,����Щ������������,
//Ȼ��ȥ����ͺ����LOST_VAL����,ȡƽ��ֵ 
//xy:ָ�CMD_RDX/CMD_RDY��
//����ֵ:����������
#define READ_TIMES 5 	//��ȡ����
#define LOST_VAL 1	  	//����ֵ
u16 TP_Read_XOY(u8 xy)
{
	u16 i, j;
	u16 buf[READ_TIMES];
	u16 sum=0;
	u16 temp;
	for(i=0;i<READ_TIMES;i++)buf[i]=TP_Read_AD(xy);		 		    
	for(i=0;i<READ_TIMES-1; i++)//����
	{
		for(j=i+1;j<READ_TIMES;j++)
		{
			if(buf[i]>buf[j])//��������
			{
				temp=buf[i];
				buf[i]=buf[j];
				buf[j]=temp;
			}
		}
	}	  
	sum=0;
	for(i=LOST_VAL;i<READ_TIMES-LOST_VAL;i++)sum+=buf[i];
	temp=sum/(READ_TIMES-2*LOST_VAL);
	return temp;   
} 




//��ȡx,y����
//��Сֵ��������100.
//x,y:��ȡ��������ֵ
//����ֵ:0,ʧ��;1,�ɹ���
u8 TP_Read_XY(u16 *x,u16 *y)
{
	u16 xtemp,ytemp;			 	 		  
	xtemp=TP_Read_XOY(CMD_RDX);
	ytemp=TP_Read_XOY(CMD_RDY);	  												   
	//if(xtemp<100||ytemp<100)return 0;//����ʧ��
	*x=xtemp;
	*y=ytemp;
	return 1;//�����ɹ�
}





//SPI���߷� ��ȡXPT2046����
//�ɼ�����ʱxpt������������� x y  
//�˺���û��ת����LCD��ʵ������ 
//ΪУ׼��������ȡ
//���� xpt2046ad.x  ����x����
//     xpt2046ad.y  ����y����
//     xpt2046ad.flag �ɼ��ɹ���־
struct TFT_Pointer xpt2046ad()		                                 
{
 struct TFT_Pointer result;
// u8   ax[8];
 u16 x1,y1;
 u16 x2,y2;

 result.x=0;
 result.y=0;
 result.flag=0;
 
#define ERR_RANGE 10 //��Χ 
  
 if(xpt2046_isClick==0)
 {	 
  delay_ms(1);
  if(xpt2046_isClick==0)
  {
    TP_CS=0; 		//��Ƭѡ
   
   /*�������16ʱ�����ڲɼ�  ��Ϊ �˴������ܲ��õ���SPI����
     ��SPI������ֻ��8λ����  XPT2046��AD�ֱ���Ϊ12λ  
	 ����8λ��SPI�����ǲ��е�
	 ���� ����XPT2046�ֲ��� 16ʱ������ ʱ��ͼ ���Կ���
	 ���Ͳɼ�����  ����һ��SPI���ݺ� �ڷ��Ϳչ��ܵ�SPI����  �ͻ��ʣ�µĲ��ֽ��յ�
	 �����Ƚ��յ� �ǵ��ֽ�����  �ڶ��ν��յ��Ǹ��ֽ�����  ��λ�� ����12λ��ADֵ   
   */

	TP_Read_XY(&x1,&y1);
	TP_Read_XY(&x2,&y2);


if(((x2<=x1&&x1<x2+ERR_RANGE)||(x1<=x2&&x2<x1+ERR_RANGE))//ǰ�����β�����+-ERR_RANGE��
    &&((y2<=y1&&y1<y2+ERR_RANGE)||(y1<=y2&&y2<y1+ERR_RANGE)))
   {
   	result.flag=1;			//�򿪱�־λ
	result.x=(x1+x2)/2;
	result.y=(y1+y2)/2;
   }
   else result.flag=0;

   TP_CS=1; 		//��Ƭѡ
   
  }
 } 

 return result;
}




//��������ת����ʵ�����꺯����
//���� result.x���� result.y���� 
struct TFT_Pointer TFT_Cm(void)		                                  
{
 u16 a,b;				//��ʱ����
 struct TFT_Pointer result;

 result=xpt2046ad();	 //��ȡADֵ

 if(result.flag)			 //�д�������Ϊǰ��
 {

//������X Y
   a=result.x;
   b=result.y;
/* ���������㹫ʽ
   lcdx=xa*tpx+xb;
   lcdy=ya*tpy+yb;
   lcdx,lcdyΪ������  tpx,tpyΪ������ֵ xa,yaΪ��������  xb,ybΪƫ����

   ���㷽�� 
   ����Ļ��ָ��lcdx,lcdyλ�û���ʮ��ͼ�� �ֱ�����Ļ�ϵ�4����λ��
   �ô����ʷֱ��� �õ����еĴ���ֵ
   ��������Ĺ�ʽ ����	xa,ya  xb,yb  ��������ʹ�ô��������ĻУ׼
  */
	 
//		 result.x=240-(0.065894*a-16);			//���õ���ADֵ���빫ʽ ����lcd����x y���� 
//		 result.y=320-(0.084031*b-14);		//���y�� ���µߵ� ��Ϊ	result.y=0.084031*b-14;


   		 result.x=((float)T_flash.xi/10000)*a+T_flash.a;			//���õ���ADֵ���빫ʽ ����lcd����x y���� 
		 result.y=((float)T_flash.yi/10000)*b+T_flash.b;


 }
 return result;								//��������ֵ
}














 //ʮ��У׼���
//c ʮ�ֽ�������
//color ʮ��У׼��ɫ
//color2 ʮ��У׼Բ��ɫ
void cross(u8 c,u16 color,u16 color2) 
{
switch(c)
{
case 1:
  GUI_box(18,0,20,39,color);	//����
  GUI_box(0,18,39,20,color);	//����
  GUI_arc(19,19,12,color2,0);     //��ͬ��Բ
  GUI_arc(19,19,10,color2,0);     //��ͬ��Բ
  break;
case 2:
  GUI_box(218,0,220,39,color);
  GUI_box(199,18,239,20,color);
  GUI_arc(220,19,12,color2,0);     //��ͬ��Բ
  GUI_arc(220,19,10,color2,0);     //��ͬ��Բ
  break;
case 3:
  GUI_box(18,279,20,319,color);
  GUI_box(0,298,39,300,color);
  GUI_arc(19,299,12,color2,0);     //��ͬ��Բ
  GUI_arc(19,299,10,color2,0);     //��ͬ��Բ
  break;
case 4:
  GUI_box(218,279,220,319,color);
  GUI_box(199,298,239,300,color);
  GUI_arc(220,299,12,color2,0);     //��ͬ��Բ
  GUI_arc(220,299,10,color2,0);     //��ͬ��Բ
  break;  
}

}



//����У׼����
//�˺��� ��Ҫ����У׼����T_i2c�������
//У׼�� ��ѡ���Ƿ���� 
//���� ֱ�ӽ�У׼�Ĳ��� ���� 24c02 10-17��ַΪ�� 18��У׼��־λ  ���º� ��ַ18�������д1
//������ ֱ�������ú���
void T_correct(void)
{
struct TFT_Pointer xpt;

u8 cnt=0,
   update=0;       //���´������ݱ�־

short pos_temp[4][2],//���껺��ֵ
      d1,d2;		   //��ʱ����

u32 tem1,tem2;	   //��ʱ����
  
short ox,oy;	   //�����������

float fac, 
      xfac,yfac;   //�����������

short xoff,yoff,   //�����������	ȫ������У׼ 4���������п����Ǹ�ֵ
      fx,fy;

GUI_Clear(White);	       //����
cross(1,Blue,Red3);			   //��ʮ�ֱ��


GUI_sprintf_hzstr16x(20,80, "���4��ʮ�ֱ���У׼ֵ",Red,White);
GUI_sprintf_hzstr16x(20,100, "У׼�ɹ��������Զ����£�",Blue,White);
while(1)
{
 xpt=xpt2046ad();		   //�ɼ�����ֵ ���ֵΪ ����AD����ʵֵ 
 if(xpt.flag==1)
 {
  delay_ms(500);
  pos_temp[cnt][0]=xpt.x;  //������x yֵ��������
  pos_temp[cnt][1]=xpt.y;
  cnt++;
  switch(cnt)
  {
   case 1:
       cross(1,White,White);	   //��һ�㴥�����
	   cross(2,Blue,Red3);	   //�����㴥�����
	   break;
   case 2:
       cross(2,White,White);	   //��2
	   cross(3,Blue,Red3);	   //��3
	   break;
   case 3:
       cross(3,White,White);	   //��3
	   cross(4,Blue,Red3);	   //��4
	   break;
   case 4:				   //4�㴥��ֵ���õ��� ��ʼУ׼
	    //�Ա����
		tem1=abs(pos_temp[0][0]-pos_temp[1][0]);//x1-x2
		tem2=abs(pos_temp[0][1]-pos_temp[1][1]);//y1-y2
		tem1*=tem1;
		tem2*=tem2;
		d1=sqrt(tem1+tem2);//�õ�1,2�ľ���
		
		tem1=abs(pos_temp[2][0]-pos_temp[3][0]);//x3-x4
		tem2=abs(pos_temp[2][1]-pos_temp[3][1]);//y3-y4
		tem1*=tem1;
		tem2*=tem2;
		d2=sqrt(tem1+tem2);//�õ�3,4�ľ���
		fac=(float)d1/d2;       
		if(fac<0.95f||fac>1.05f||d1==0||d2==0)//���ϸ�
		{
			cnt=0;
			led_d2=0;
	    	cross(4,White,White);	//�����4
			cross(1,Blue,Red3);	//����1 
			continue;
		}
		tem1=abs(pos_temp[0][0]-pos_temp[2][0]);//x1-x3
		tem2=abs(pos_temp[0][1]-pos_temp[2][1]);//y1-y3
		tem1*=tem1;
		tem2*=tem2;
		d1=sqrt(tem1+tem2);//�õ�1,3�ľ���
		
		tem1=abs(pos_temp[1][0]-pos_temp[3][0]);//x2-x4
		tem2=abs(pos_temp[1][1]-pos_temp[3][1]);//y2-y4
		tem1*=tem1;
		tem2*=tem2;
		d2=sqrt(tem1+tem2);//�õ�2,4�ľ���
		fac=(float)d1/d2;
		if(fac<0.95f||fac>1.05f)//���ϸ�
		{
			cnt=0;
			led_d2=0;
	    	cross(4,White,White);	//�����4
			cross(1,Blue,Red3);	//����1 
            continue;		//У׼���ϸ��1�㴥����� ����У׼
		}
		
		//��ȷ��
		GUI_Clear(White);	//����
			
		//��Ԫһ�η��̴��빫ʽ   LCDx=xfac*pos+xoff
		xfac=(float)(239-40)/(pos_temp[1][0]-pos_temp[0][0]);//�õ�xfac		 
		xoff=(239-xfac*(pos_temp[1][0]+pos_temp[0][0]))/2;//�õ�xoff
			  
		yfac=(float)(319-40)/(pos_temp[2][1]-pos_temp[0][1]);//�õ�yfac
		yoff=(319-yfac*(pos_temp[2][1]+pos_temp[0][1]))/2;//�õ�yoff 




		 GUI_sprintf_hzstr16x(20,20, "У׼�����",Blue,White);

	  	 GUI_sprintf_hzstr16x(20,40, "xi: 0.0",Red,White);
		 GUI_sprintf_hzstr16x(20,80, "yi: 0.0",Red,White);
		 GUI_sprintf_hzstr16x(20,60, "a:",Red,White);
		 GUI_sprintf_hzstr16x(20,100,"b:",Red,White);

         if(xfac<0)
		 {         
		   xfac=-xfac;
		   xfac=xfac*10000;
		   fx=-(short)xfac;	
		   GUI_sprintf_hzstr16x(44,40,"-",Red,White);	 
		 } 
		 else
		 {
		   xfac=xfac*10000;
		   fx=xfac;			 		 
		 }

         if(yfac<0)
		 {
		   yfac=-yfac;
		   yfac=yfac*10000;
		   fy=-(short)yfac;	
		   GUI_sprintf_hzstr16x(44,80,"-",Red,White);	 
		 } 
		 else
		 {
		   yfac=yfac*10000;
		   fy=yfac;			 		 
		 }


	    ox=xoff;				 //��ȡ
	    oy=yoff;

	    if(xoff<0)
	    {
	     ox=~ox+1;	 //ȡ����=����+1
	     GUI_sprintf_hzstr16x(36,60, "-",Red,White);
	    }


	    if(yoff<0)
	    {
	     oy=~oy+1;
	     GUI_sprintf_hzstr16x(36,100,"-",Red,White);
	    }


	    number(76,40,xfac,Red,White);
	    number(44,60,ox,Red,White);
	    number(76,80,yfac,Red,White);
	    number(44,100,oy,Red,White);

//	    Dialog_box(59,120,159,170,Black,Red,window,"���´�������");	  //������˵���ľ��ͶԻ���
//		Touch_key(65,145,Blue2,Blue,0);    //ȷ����ť��
//		Touch_key(117,145,Blue2,Blue,1);   //ȡ����ť��
		delay_ms(500);
        GUI_sprintf_hzstr16x(20,150, "У׼���ݿ�ʼ���¡�����",Black,White);
		delay_ms(500);
//		while(1)							//key14 ǿ�и��´������ݵ�24c��
//		{
//		 xpt=TFT_Cm();					   //�ɼ�������Ӧ����Ļ����
//		 if(xpt.flag==1||S3==0)                 //�Ƿ��д����¼�  
//         {
//		  if((xpt.y>145&&xpt.y<165)||S3==0)	   //ȷ��  ȡ��
//		   {
//		    if((xpt.x>65&&xpt.x<101)||S3==0)	   //ȷ�ϵ�ǰѡ��
//			{


   		flash_tp[0]=fx;				//��У׼ת����������
		flash_tp[1]=fx>>8;
		flash_tp[2]=fy;
		flash_tp[3]=fy>>8;
		flash_tp[4]=xoff;
		flash_tp[5]=xoff>>8;
		flash_tp[6]=yoff;
		flash_tp[7]=yoff>>8; 	   
		flash_tp[8]=0xF4;			//�ɹ�У׼�� ����־λ 0xF4

		SPI_Flash_Write(flash_tp,xpt_flash_sector*4096,9);	//��У׼ֵ����flashоƬ�� 200����  ��ַ��0-9 9���ֽ�

	 	GUI_sprintf_hzstr16x(20,180, "���ݸ�����ɣ�",Red3,White);
		delay_ms(500);
		GUI_sprintf_hzstr16x(20,200, "׼������������",Blue,White);
		update=1;
		delay_ms(1000);
	    break;
//			 }
//			}
//  	 	   }
//        } 
	 }
  }
 if(update!=0) break;	  //��������


}
}




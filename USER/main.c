#include  "mcusys.h"
#include  "delay.h"
#include  "led.h"
#include  "tft.h"
#include  "led.h"
#include  "ov7670.h"
#include  "gui.h"
#include  "spi.h"
#include  "xpt2046.h"
#include  "malloc.h"
#include  "flash.h"
#include  "sdio_sdcard.h" 
#include  "pff.h"				  //�ļ�ϵͳ����.h����
#include  "OVbmp.h"
#include  "OVroot.h"			


FATFS fatfs;	                  //�ļ�ϵͳ�ṹ�嶨��
u8 tbuf[512];


extern u8 ov_sta;	//֡���� ��λ��־λ


//��ģ����м�� ��ʼ������
void check_system(void)
{

	flash_binplay(0,0,22,16,305);	//�������ͼ��
	GUI_sprintf_hzstr16x(30,0,"STM32-OV Photograph",Black,White);
	delay_ms(260);	
	
	mem_init(SRAMIN);		//��ʼ���ڲ��ڴ�� 
	GUI_sprintf_hzstr16x(0,18,"SRAMin Init",Black,White);
	delay_ms(260);

	if(readflash_t(1)==0)GUI_sprintf_hzstr16x(0,36,"Touch correct ok",Black,White);	//��ȡflash�д���У׼���� �����У׼�������� 
	else  GUI_sprintf_hzstr16x(0,36,"Touch correct Error",Red,White);
	delay_ms(260);

	if(SPI_Flash_ReadID()==0xef14)	  //�ж��Ƿ�ΪW25X16
	GUI_sprintf_hzstr16x(0,54,"SPI Ex FLASH Init OK",Black,White);
	else  GUI_sprintf_hzstr16x(0,54,"SPI Ex FLASH Init Error",Black,White);
	delay_ms(260);

	if(TFsdio_Init()==0)GUI_sprintf_hzstr16x(0,72,"SDIO-TF Card Init ok",Black,White);
	else GUI_sprintf_hzstr16x(0,72,"SDIO-TF Card Init Error",Red,White);
	delay_ms(260);

	if(OV_Init()==0)GUI_sprintf_hzstr16x(0,90,"OV7670 Init ok",Black,White);	//ov��ʼ��
	else  GUI_sprintf_hzstr16x(0,90,"OV7670 Init Error",Red,White);
	delay_ms(200);
}


int main(void)
{
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);		//����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
  delay_init(168);	   //��ʱ������ʼ��	  
  LED_Init();		  	   //��ʼ����LED���ӵ�Ӳ���ӿ�
  SPI1_Init();			   //��ʼ��SPI1����
  SPI1_SetSpeed(SPI_BaudRatePrescaler_2);//����Ϊ18Mʱ��,����ģʽ			 
  Lcd_Init();			     //LCD  ��ʼ��
  SPI_Flash_Init();  	 //FLASH Ƭѡio CS ��ʼ��	
  XPT2046_Init();		 //����оƬXPT2046 io����    				  
  OV_Gpio_Init();
  GUI_Clear(White);		 //����

  pf_mount(&fatfs);	     //��ʼ��petit FATFS�ļ�ϵͳ  ����ȡtf����Ӧ����

  check_system();

  Ov_Photograph();			  //����ov�����

}


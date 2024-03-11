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
#include  "pff.h"				  //文件系统调用.h加载
#include  "OVbmp.h"
#include  "OVroot.h"			


FATFS fatfs;	                  //文件系统结构体定义
u8 tbuf[512];


extern u8 ov_sta;	//帧次数 置位标志位


//各模块进行检测 初始化函数
void check_system(void)
{

	flash_binplay(0,0,22,16,305);	//启光电子图标
	GUI_sprintf_hzstr16x(30,0,"STM32-OV Photograph",Black,White);
	delay_ms(260);	
	
	mem_init(SRAMIN);		//初始化内部内存池 
	GUI_sprintf_hzstr16x(0,18,"SRAMin Init",Black,White);
	delay_ms(260);

	if(readflash_t(1)==0)GUI_sprintf_hzstr16x(0,36,"Touch correct ok",Black,White);	//读取flash中触摸校准参数 如果有校准参数更新 
	else  GUI_sprintf_hzstr16x(0,36,"Touch correct Error",Red,White);
	delay_ms(260);

	if(SPI_Flash_ReadID()==0xef14)	  //判断是否为W25X16
	GUI_sprintf_hzstr16x(0,54,"SPI Ex FLASH Init OK",Black,White);
	else  GUI_sprintf_hzstr16x(0,54,"SPI Ex FLASH Init Error",Black,White);
	delay_ms(260);

	if(TFsdio_Init()==0)GUI_sprintf_hzstr16x(0,72,"SDIO-TF Card Init ok",Black,White);
	else GUI_sprintf_hzstr16x(0,72,"SDIO-TF Card Init Error",Red,White);
	delay_ms(260);

	if(OV_Init()==0)GUI_sprintf_hzstr16x(0,90,"OV7670 Init ok",Black,White);	//ov初始化
	else  GUI_sprintf_hzstr16x(0,90,"OV7670 Init Error",Red,White);
	delay_ms(200);
}


int main(void)
{
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);		//设置NVIC中断分组2:2位抢占优先级，2位响应优先级
  delay_init(168);	   //延时函数初始化	  
  LED_Init();		  	   //初始化与LED连接的硬件接口
  SPI1_Init();			   //初始化SPI1总线
  SPI1_SetSpeed(SPI_BaudRatePrescaler_2);//设置为18M时钟,高速模式			 
  Lcd_Init();			     //LCD  初始化
  SPI_Flash_Init();  	 //FLASH 片选io CS 初始化	
  XPT2046_Init();		 //触摸芯片XPT2046 io配置    				  
  OV_Gpio_Init();
  GUI_Clear(White);		 //清屏

  pf_mount(&fatfs);	     //初始化petit FATFS文件系统  并提取tf卡相应数据

  check_system();

  Ov_Photograph();			  //进入ov照相机

}


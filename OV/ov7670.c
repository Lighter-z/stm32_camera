
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
#include  "string.h"				//合并指针函数
#include  "OVbmp.h"
#include  "OVroot.h"

#define neg_film     30       //底片张数

u8 ov_sta=0;		 //帧标志位   中断中调用

u8  pic=0;				 //照片存储位置

//外部中断初始化
void EXTI0_Init(void)
{   
  EXTI_InitTypeDef EXTI_InitStructure;			//中断配置	  即哪个中断线  EXTI_Line0-15
  //模式 EXTI_Mode_Interrupt中断  EXTI_Mode_Event 事件
  //触发方式  EXTI_Trigger_Falling 下降沿触发
  //EXTI_Trigger_Rising	 上升沿触发
  //EXTI_Trigger_Rising_Falling	  任意电平触发

  NVIC_InitTypeDef NVIC_InitStructure;			//中断参数 中断优先级

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//使能SYSCFG时钟

  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, EXTI_PinSource0);//PD0 连接到中断线0

  /* 配置EXTI_Line0 */ 
  EXTI_InitStructure.EXTI_Line=EXTI_Line0;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;			   //中断模式
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;		   //配置下降沿触发
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);	  	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器


  //中断级别越低 越优先
  //两中断  抢占=响应  执行先发生
  //高优先级的抢占 可断 低优先级
  //优先级相等    高响应不可断低响应
  NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;//外部中断
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;//抢占优先级0
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//子优先级2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
  NVIC_Init(&NVIC_InitStructure);//配置
 
}

 //外部中断服务程序
void EXTI0_IRQHandler(void)
{		 		
 
	if(EXTI_GetITStatus(EXTI_Line0)==SET)//是2线的中断
	{     
		if(ov_sta<2)
		{
			if(ov_sta==0)
			{
				OV7670_WRST=0;	 	//复位写指针		  		 
				OV7670_WRST=1;	
				OV7670_WREN=1;		//允许写入FIFO
			}else 
			{
				OV7670_WREN=0;		//禁止写入FIFO 
				OV7670_WRST=0;	 	//复位写指针		  		 
				OV7670_WRST=1;	
			}
			ov_sta++;
		}
	}
 
	EXTI_ClearITPendingBit(EXTI_Line0);  //清除EXTI2线路挂起位						  
} 

void OV_Gpio_Init(void)   // OV相应IO初始化
{
 	GPIO_InitTypeDef  GPIO_InitStructure;
 	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOD, ENABLE);	 //使能相关端口时钟

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_13; 	// 输入 上拉
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
 	GPIO_Init(GPIOC, &GPIO_InitStructure);

	
 	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_1|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;	 // 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
 	GPIO_Init(GPIOD, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin  = 0xff00; // 输入 上拉
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输入模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
 	GPIO_Init(GPIOD, &GPIO_InitStructure);

 	SCCB_Init(); 
}
  			    
//初始化OV7670
//返回0:成功
//返回其他值:错误代码
u8 OV_Init(void)
{
	u8 temp;
	u16 i=0;
	 
    //初始化SCCB 的IO口	   	  
 	if(SCCB_WR_Reg(0x12,0x80)==0) return 1;	//复位SCCB	  
	delay_ms(50); 
	//读取产品型号
 	temp=SCCB_RD_Reg(0x0b);   
	if(temp!=0x73) return 2; 
 	temp=SCCB_RD_Reg(0x0a);   
	if(temp!=0x76) return 2;
	//初始化序列	  
	for(i=0;i<CHANGE_REG_NUM;i++)			  //写寄存器循环
	{
      SCCB_WR_Reg(ov7670_init_reg_tbl[i][0],ov7670_init_reg_tbl[i][1]);
	  delay_us(20);	
	}
    return 0;//ok
} 

////////////////////////////////////////////////////////////////////////////
//OV7670功能设置
//白平衡设置
//0:自动
//1:太阳sunny
//2,阴天cloudy
//3,办公室office
//4,家里home
void OV7670_Light_Mode(u8 mode)
{
	u8 reg13val=0XE7;//默认就是设置为自动白平衡
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
	SCCB_WR_Reg(0X13,reg13val);//COM8设置 
	SCCB_WR_Reg(0X01,reg01val);//AWB蓝色通道增益 
	SCCB_WR_Reg(0X02,reg02val);//AWB红色通道增益 
}				  
//色度设置
//0:-2
//1:-1
//2,0
//3,1
//4,2
void OV7670_Color_Saturation(u8 sat)
{
	u8 reg4f5054val=0X80;//默认就是sat=2,即不调节色度的设置
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
	SCCB_WR_Reg(0X4F,reg4f5054val);	//色彩矩阵系数1
	SCCB_WR_Reg(0X50,reg4f5054val);	//色彩矩阵系数2 
	SCCB_WR_Reg(0X51,0X00);			//色彩矩阵系数3  
	SCCB_WR_Reg(0X52,reg52val);		//色彩矩阵系数4 
	SCCB_WR_Reg(0X53,reg53val);		//色彩矩阵系数5 
	SCCB_WR_Reg(0X54,reg4f5054val);	//色彩矩阵系数6  
	SCCB_WR_Reg(0X58,0X9E);			//MTXS 
}
//亮度设置
//0:-2
//1:-1
//2,0
//3,1
//4,2
void OV7670_Brightness(u8 bright)
{
	u8 reg55val=0X00;//默认就是bright=2
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
	SCCB_WR_Reg(0X55,reg55val);	//亮度调节 
}
//对比度设置
//0:-2
//1:-1
//2,0
//3,1
//4,2
void OV7670_Contrast(u8 contrast)
{
	u8 reg56val=0X40;//默认就是contrast=2
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
	SCCB_WR_Reg(0X56,reg56val);	//对比度调节 
}
//特效设置
//0:普通模式    
//1,负片
//2,黑白   
//3,偏红色
//4,偏绿色
//5,偏蓝色
//6,复古	    
void OV7670_Special_Effects(u8 eft)
{
	u8 reg3aval=0X04;//默认为普通模式
	u8 reg67val=0XC0;
	u8 reg68val=0X80;
	switch(eft)
	{
		case 1://负片
			reg3aval=0X24;
			reg67val=0X80;
			reg68val=0X80;
			break;	
		case 2://黑白
			reg3aval=0X14;
			reg67val=0X80;
			reg68val=0X80;
			break;	
		case 3://偏红色
			reg3aval=0X14;
			reg67val=0Xc0;
			reg68val=0X80;
			break;	
		case 4://偏绿色
			reg3aval=0X14;
			reg67val=0X40;
			reg68val=0X40;
			break;	
		case 5://偏蓝色
			reg3aval=0X14;
			reg67val=0X80;
			reg68val=0XC0;
			break;	
		case 6://复古
			reg3aval=0X14;
			reg67val=0XA0;
			reg68val=0X40;
			break;	 
	}
	SCCB_WR_Reg(0X3A,reg3aval);//TSLB设置 
	SCCB_WR_Reg(0X68,reg67val);//MANU,手动U值 
	SCCB_WR_Reg(0X67,reg68val);//MANV,手动V值 
}	
//设置图像输出窗口
//对QVGA设置。
void OV7670_Window_Set(u16 sx,u16 sy,u16 width,u16 height)
{
	u16 endx;
	u16 endy;
	u8 temp; 
	endx=sx+width*2;	//V*2
 	endy=sy+height*2;
	if(endy>784)endy-=784;
	temp=SCCB_RD_Reg(0X03);				//读取Vref之前的值
	temp&=0XF0;
	temp|=((endx&0X03)<<2)|(sx&0X03);
	SCCB_WR_Reg(0X03,temp);				//设置Vref的start和end的最低2位
	SCCB_WR_Reg(0X19,sx>>2);			//设置Vref的start高8位
	SCCB_WR_Reg(0X1A,endx>>2);			//设置Vref的end的高8位

	temp=SCCB_RD_Reg(0X32);				//读取Href之前的值
	temp&=0XC0;
	temp|=((endy&0X07)<<3)|(sy&0X07);
	SCCB_WR_Reg(0X17,sy>>3);			//设置Href的start高8位
	SCCB_WR_Reg(0X18,endy>>3);			//设置Href的end的高8位
}




//对比度对话框设置函数
void con_set(void)
{
 struct TFT_Pointer sp;  //定义触摸变量结构体

 Dialog_box(75,130,155,252,Blue,Black,window,"对比度设置");

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

//白平衡设置
void lig_set(void)
{
 struct TFT_Pointer sp;  //定义触摸变量结构体

 Dialog_box(75,150,155,272,Blue,Black,window,"白平衡设置");

 GUI_arc(90,180,6,Black,0);
 GUI_arc(90,200,6,Black,0);
 GUI_arc(90,220,6,Black,0);
 GUI_arc(90,240,6,Black,0);
 GUI_arc(90,260,6,Black,0);
 GUI_sprintf_hzstr16x(107,172,"自动",Black,1);
 GUI_sprintf_hzstr16x(107,192,"太阳",Black,1);
 GUI_sprintf_hzstr16x(107,212,"阴天",Black,1);
 GUI_sprintf_hzstr16x(107,232,"办公室",Black,1);
 GUI_sprintf_hzstr16x(107,252,"家里",Black,1);
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

//亮度对话框设置函数
void bri_set(void)
{
 struct TFT_Pointer sp;  //定义触摸变量结构体

 Dialog_box(75,170,145,292,Blue,Black,window,"亮度设置");

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


//色度对话框设置函数
void col_set(void)
{
 struct TFT_Pointer sp;  //定义触摸变量结构体

 Dialog_box(75,190,145,312,Blue,Black,window,"色度设置");

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



//特效设置
void spe_set(void)
{
 struct TFT_Pointer sp;  //定义触摸变量结构体

 Dialog_box(75,70,155,232,Blue,Black,window,"特效设置");

 GUI_arc(90,100,6,Black,0);
 GUI_arc(90,120,6,Black,0);
 GUI_arc(90,140,6,Black,0);
 GUI_arc(90,160,6,Black,0);
 GUI_arc(90,180,6,Black,0);
 GUI_arc(90,200,6,Black,0);
 GUI_arc(90,220,6,Black,0);
 GUI_sprintf_hzstr16x(107,92,"普通",Black,1);
 GUI_sprintf_hzstr16x(107,112,"负片",Black,1);
 GUI_sprintf_hzstr16x(107,132,"黑白",Black,1);
 GUI_sprintf_hzstr16x(107,152,"偏红色",Black,1);
 GUI_sprintf_hzstr16x(107,172,"偏绿色",Black,1);
 GUI_sprintf_hzstr16x(107,192,"偏蓝色",Black,1);
 GUI_sprintf_hzstr16x(107,212,"复古",Black,1);
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

//底片数据设置
//当拍照的照片数已经到达TF卡内底片个数时 将提示不能在更新新的照片了
//这时可以考虑把TF卡里的照片复制出来 然后恢复底片数据
//底片数据：主要是存在STC15单片机eeprom中的0地址的一个字节
//          它的主要功能是 点击拍照时 单片机读取eeprom数据 判断应该更新哪一张照片
//          比如 已经拍好了前2张  当在点拍照时  单片机读到的就是2 在这个基础上加1
//               就是在第3张照片上开始更新  更新完成以后 在把这个3 重新写入到eeprom中
//               如果底片是30  当30张都拍完时 那就要到neg_num里清除eeprom中数据由30为0 
//               这样才能继续从TF卡的第一张底片开始更新
void neg_num(void)
{
  struct TFT_Pointer sp;  //定义触摸变量结构体
  Dialog_box(75,230,206,300,Red,Red,window,"底片数据");
  GUI_sprintf_hzstr16x(76,252,"从第1张底片开始？",Black,1);

  Touch_key(83,275,Blue2,Blue,0);     //确定按钮框
  Touch_key(160,275,Blue2,Blue,1);    //取消按钮框

  while(1)
  {
   sp=TFT_Cm();
   if(sp.flag==1)
    {
	  if(sp.y>275&&sp.y<300)
	  {
	    if(sp.x>83&&sp.x<123)
		 {
		  Touch_key(83,275,Gray,Black,0);     //确定按钮框
		  delay_ms(200);	
		  //IAP无法使用
         // IapEraseSector(0);	    //擦除STC15芯片上的eeprom空间
         // IapProgramByte(0,0);	    //将底片数据清0
		 pic=0;  //IAP清PIC数据
		  GUI_sprintf_hzstr16x(75,302,"数据更新完成！",White,1);
		  delay_ms(200);
		  break;
			  
		 }
	    if(sp.x>160&&sp.x<200)
		 {
		  Touch_key(160,275,Gray,Black,1);    //取消按钮框
		  delay_ms(300);
		  break;	  
		 }
	  }
	}
  
  }
}


//ov功能设置 
//在弹出的对话框中可以对 OV的对比度 白平衡 亮度 色度 特效进行设置
//还可以更新底片数据 eeprom
void ov_set(void)
{
 struct TFT_Pointer sp;  //定义触摸变量结构体

 Dialog_box(0,110,80,252,window,Blue,window,"相机设置");

 GUI_sprintf_hzstr16x(1,132,"对比度设置",Black,1);
 GUI_sprintf_hzstr16x(1,152,"白平衡设置",Black,1);
 GUI_sprintf_hzstr16x(1,172,"亮度设置",Black,1);
 GUI_sprintf_hzstr16x(1,192,"色度设置",Black,1);
 GUI_sprintf_hzstr16x(1,212,"特效设置",Black,1);
 GUI_sprintf_hzstr16x(1,232,"底片数据",Black,1);

 while(1)
 {
   sp=TFT_Cm();
   if(sp.flag==1)   
    {
	  if(sp.x>1&&sp.x<110)
	  {
	    if(sp.y>130&&sp.y<149)
		 {
		   GUI_sprintf_hzstr16x(1,132,"对比度设置",White,Red);
			 delay_ms(100);
		   con_set();
		   break;		 
		 } 
	    if(sp.y>152&&sp.y<169)
		 {
		   GUI_sprintf_hzstr16x(1,152,"白平衡设置",White,Red);
			 delay_ms(100);
		   lig_set();
		   break;
		 }
	    if(sp.y>172&&sp.y<189)
		 {
		   GUI_sprintf_hzstr16x(1,172,"亮度设置",White,Red);
			 delay_ms(100);
		   bri_set();
		   break;
		 }
	    if(sp.y>192&&sp.y<209)
		 {
		   GUI_sprintf_hzstr16x(1,192,"色度设置",White,Red);
			 delay_ms(100);
		   col_set();
		   break;
		 }
	    if(sp.y>212&&sp.y<229)
		 {
		   GUI_sprintf_hzstr16x(1,212,"特效设置",White,Red);
			 delay_ms(100);
		   spe_set();
		   break;
		 }
	    if(sp.y>232&&sp.y<249)
		 {
		   GUI_sprintf_hzstr16x(1,232,"底片数据",White,Red);
			 delay_ms(100);
		   neg_num();
		   break;
		 }	   
	  }
	  else break;	

	}	
 }

}


//拍照存储函数
//存储的照片为240x240  16位色BMP文件
//点击拍照选项时 该函数将把屏幕上的图片存储到TF卡指定路径下
//函数工作的前提是:必须确定路径上的BMP文件是存在的 而且大小必须是240x240
//说明：petit fatfs文件系统不能生成新的文件 只能在以有的文件上进行更新  
//      这个以有的文件在这里称之为 “底片”
u8 way[31][4]={"0","1","2","3","4","5","6","7","8","9","10",		//相片存储路径数组
                    "11","12","13","14","15","16","17","18","19","20",
					"21","22","23","24","25","26","27","28","29","30",};
//u8  *ov;			  //ov拍好的照片 存储路径指针
void touch_pic(void)
{
//   u8  pic=0;				 //照片存储位置
	 u8 i;
   u8  ov[32];			   //ov拍好的照片 存储路径指针
	 for(i=0;i<32;i++)ov[i]=0;
	
   GUI_arc(200,280,14,Gray,1);		 //拍照按钮动态显示效果
   delay_ms(200);
   GUI_arc(200,280,14,White,1);

   //IAP无法使用
   //pic=IapReadByte(0);	  //在EEPROM地址0中提取 存储的照片个数
   pic+=1;                //准备在下一张存储

   if(pic<=neg_film)	  //要保存的照片要在底片范围内
    {
   	 //ov=mymalloc(SRAMIN,32);	  //ov存储路径申请内存
												//制作要存储的图片路径  如"/ov76/m1.bmp"
												//当然这些路径都是TF卡里以准备好的

     strcat((char*)ov,(const char*)"/ov76/m");									
     strcat((char*)ov,(const char*)way[pic]);   
     strcat((char*)ov,(const char*)".bmp");

	 led_d2=0;
	 GUI_sprintf_hzstr16x(0,241,"Begin save...Please wait!",Yellow,Black);
	 Write_Bmp((const char *)ov);								//开始保存照片

	 led_d2=1;
	 GUI_sprintf_hzstr16x(0,241,"SAVE ok!                 ",Blue,Black);   //保存成功
	 delay_ms(300);

	 GUI_sprintf_hzstr16x(0,241,"Save to:/ov76/m  .bmp    ",Red,Black);	   //显示保存的路径
	 number(120,241,pic,Red,1);

	 delay_ms(800);    //保持显示延时
   GUI_sprintf_hzstr16x(0,241,"                         ",Red,Black);	   //清显示
	 //myfree(SRAMIN,ov);		//释放申请的存储路径指针

	 //IAP无法使用
     //IapEraseSector(0);	    //擦除STC15芯片上的eeprom空间
     //IapProgramByte(0,pic);	//更新pic当前值
    }
   else if(pic>neg_film)
    {															 //警告对话框
	 Dialog_box(20,100,220,200,window,Red,window,"WARGING...");

	 GUI_sprintf_hzstr16x(25,120,"已没有底片可以更新了！！",Black,1);   
	 GUI_sprintf_hzstr16x(25,140,"请更换TF卡里的底片。",Black,1);
	 GUI_sprintf_hzstr16x(25,160,"清空相机设置里底片数据。",Black,1);  

	 delay_ms(2000);
	 delay_ms(2000);
	}
}


//ov界面显示函数
void ov_interface(void)
{
  GUI_Clear(Black);			    //黑色清屏

  GUI_arc(200,280,19,White,1);	//画拍照点击圆圈
  GUI_arc(200,280,16,Black,1);
  GUI_arc(200,280,14,White,1);

  GUI_sprintf_hzstr16x(0,260,"相机设置",White,1);
  GUI_sprintf_hzstr16x(0,290,"相册",White,1);

}  




//照相机主函数体
//支持相机功能设置 拍照存储TF 打开电子相册功能
u8 Ov_Photograph(void)
{
	struct TFT_Pointer sp;  //定义触摸变量结构体
	u32  j;					 //临时变量
	// u8  sign=0;			 //初始标志
	u16 color;
				     //开启中断目的为  判断VSYNC 为帧数据判断 当有触发的时候 为来一帧  
				                     //这个是时候开始往 缓存芯片 AL422B灌入一帧数据
									 //当来第二帧时 说明数据灌入完成 此时提取数据 进行显示
	
	ov_interface();         //显示ov界面
				
	EXTI0_Init();					   //初始化中断线
	OV7670_Window_Set(10,174,240,240);	//设置窗口
	OV7670_CS=0;					   //使能摄像头片选
	LCD_scan(5);   //设置TFT扫描方向 为从下到上 从右到左
	
	//TFT_RS_1;				   //拉高TFT 写数据端 
 
     	 
 //开始扫描 显示摄像头采集数据
 while(1)
 {
   //OV7670_CS=1;								//关掉 ov使能端	 （主要原因 ov使能端 和FLASH公用一个使能口 
                                            //                 而xpt2046 和 FLASH又同一SPI总线  如果OV使能端使能
											//                 就相当于打开FLASH片选 这样对SPI上的XPT2046的采集产生音响）
   sp=TFT_Cm();	                            //扫描触摸屏
   if(sp.flag==1)                           //是否有触摸事件 发生 
   { 
  	 if(sp.x>160&&sp.x<220&&sp.y>260&&sp.y<310) //点击拍照功能
	 {
		 LCD_scan(1);   //设置TFT扫描方向 为从左到右 从上到下
	   touch_pic();	  //保存当前相片
		 LCD_scan(5);   //设置TFT扫描方向 为从下到上 从右到左
	 }
  	 if(sp.x>0&&sp.x<80)    
	 {
	   if(sp.y>280&&sp.y<310)	//进入相册功能
	   {
				LCD_scan(1);   //设置TFT扫描方向 为从左到右 从上到下
				GUI_sprintf_hzstr16x(0,290,"相册",Gray,1); //点击后效果变化
				delay_ms(150);
				tp_show("/ov76");
				ov_interface();      //跳出相册后恢复OV界面
				LCD_scan(5);   //设置TFT扫描方向 为从下到上 从右到左
	   }
	   if(sp.y>250&&sp.y<272)  //相机设置选项
	   {
			  LCD_scan(1);   //设置TFT扫描方向 为从左到右 从上到下
				GUI_sprintf_hzstr16x(0,260,"相机设置",Gray,1);
				delay_ms(150);
				GUI_sprintf_hzstr16x(0,260,"相机设置",White,1);
				ov_set();
				ov_interface();      //跳出相册后恢复OV界面
			  LCD_scan(5);   //设置TFT扫描方向 为从下到上 从右到左
	   }	   	 					
	  }	  
    }
//  // OV7670_CS=0;

		 
	if(ov_sta==2)				//判断缓存区是否存好摄像数据
	{
	  Address_set(0,0,239,239,0,0);	  //设置显示范围  显示为240*240 
		TFT_RS=1;		                  //写数据线拉高		为提高写入速度
		 	 
		OV7670_RRST=0;				//开始复位读指针 
		OV7670_RCK=0;
		OV7670_RCK=1;
		OV7670_RCK=0;		
		OV7670_RRST=1;				//复位读指针结束 
		OV7670_RCK=1;  
		
		//此用法 是将 下面写数据的步骤完全拆开  主要是为了提高显示速度  （下面注释部分为参考部分）
                     
		for(j=0;j<57600;j++)			   //分辨率为240x320   每个颜色点要两个字节 所以 240x320x2=76800  次
		{
			OV7670_RCK=0;				 //每一次时钟跳变 读一次数据
			color=GPIOD->IDR&0XFF00;	//读数据   读取颜色高字节数据
			OV7670_RCK=1; 

			OV7670_RCK=0;
			color|=((GPIOD->IDR&0XFF00)>>8);	//读数据   读取颜色低字节数据 并转化为16位 565 颜色数据
			OV7670_RCK=1; 
			
		  GPIOB->BSRRH = GPIO_Pin_All;      //清寄存器数据
		  GPIOB->BSRRL = color;             //将采集的颜色值给PB口寄存器	    

		    TFT_WR=0;								      //开始写入
        TFT_WR=1;
//		  Lcd_Write_Data(color);
		}   							 
		EXTI_ClearITPendingBit(EXTI_Line0);  //清除LINE上的中断标志位
		ov_sta=0;					//开始下一次采集
	} 
  }	
   	
}

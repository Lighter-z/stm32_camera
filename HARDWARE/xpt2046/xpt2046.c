
 
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
  AD_OUT   PC3			读取触摸返回数据
  AD_PEN   PC1
*/



u8 flash_tp[9];		//flash 触摸校准数据缓存区



//触摸屏校准参数
//如果没有校准到flash里  使用默认值
struct T_i T_flash=
{
 -657,		//比例因子 的值要除以10000才是真实值  因为为了方便函数处理 计算时乘以10000 
 -901,		//比例因子 的值要除以10000才是真实值  因为为了方便函数处理 计算时乘以10000 
  251,
  357,
};


//4Kbytes为一个Sector
//16个扇区为1个Block
//W25X16
//容量为2M字节,共有32个Block,512个Sector 

//读取FLASH中的校准参数变量 扇区200 地址0-7 存触摸数据  8存标志
//读取前  首先要判断 地址8  即校准标志位 是否为0xf4
//sta ：决定 是否从flash中读取校准过的数据       1 读取   0 不读取 
//返回值 如果为0  说明已经校准过触摸参数 然后通过sta判断是否提取flash中的校准参数 做为触摸判断标准
//返回值 如果为1  说明没有校准过   直接利用校准参数结构体 T_flash本身的变量（这个变量值也是校准过直接填写到函数里的）
u8 readflash_t(u8 sta)
{
SPI_Flash_Read(flash_tp,xpt_flash_sector*4096,9);


 if(sta==1&&flash_tp[8]==0xF4)    //读取校准参数 1 已校准过 其他无校准过 用结构体本身函数
 {
	T_flash.xi=((u16)flash_tp[1]<<8)+flash_tp[0];		  //将读取的校准数据更新到缓存中
	T_flash.yi=((u16)flash_tp[3]<<8)+flash_tp[2];
	T_flash.a=((u16)flash_tp[5]<<8)+flash_tp[4];
	T_flash.b=((u16)flash_tp[7]<<8)+flash_tp[6];
	return 0;              //读取成功
 }
 else if(sta==0&&flash_tp[8]==0xF4) return 0;
 return 1;                 //没有校准
}





//初始化 触摸芯片XPT2046 SPI片选IO 及判断IO
void XPT2046_Init(void)
{	
  	GPIO_InitTypeDef GPIO_InitStructure;


	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOC, ENABLE);//使能GPIOB,C,F时钟


    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1| GPIO_Pin_3;//PC1/PC3 设置为上拉输入
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//输入模式
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
    GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化

	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_5|GPIO_Pin_7;//PC5 PC6 PC7设置为推挽输出
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//输出模式
	GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化
		
		
}



//默认为touchtype=0的数据.
u8 CMD_RDX=0XD0;
u8 CMD_RDY=0X90;



//SPI写数据
//向触摸屏IC写入1byte数据    
//num:要写入的数据
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
		TP_CLK=1;		//上升沿有效	        
	}		 			    
} 


//SPI读数据 
//从触摸屏IC读取adc值
//CMD:指令
//返回值:读到的数据	   
u16 TP_Read_AD(u8 CMD)	  
{ 	 
	u8 count=0; 	  
	u16 Num=0; 
	TP_CLK=0;		//先拉低时钟 	 
	TP_DIN=0; 	//拉低数据线
	TP_CS=0; 		//选中触摸屏IC
	TP_Write_Byte(CMD);//发送命令字
	delay_us(6);//ADS7846的转换时间最长为6us
	TP_CLK=0; 	     	    
	delay_us(1);    	   
	TP_CLK=1;		//给1个时钟，清除BUSY
	delay_us(1);    
	TP_CLK=0; 	     	    
	for(count=0;count<16;count++)//读出8位数据 
	{ 				  
		Num<<=1; 	 
		TP_CLK=0;	//下降沿有效  	    	   
		delay_us(1);    
 		TP_CLK=1;
 		if(TP_OUT)Num++; 		 
	}  	
	Num>>=4;   	//只有高12位有效.
	TP_CS=1;		//释放片选	 
	return(Num);   
}



//读取一个坐标值(x或者y)
//连续读取READ_TIMES次数据,对这些数据升序排列,
//然后去掉最低和最高LOST_VAL个数,取平均值 
//xy:指令（CMD_RDX/CMD_RDY）
//返回值:读到的数据
#define READ_TIMES 5 	//读取次数
#define LOST_VAL 1	  	//丢弃值
u16 TP_Read_XOY(u8 xy)
{
	u16 i, j;
	u16 buf[READ_TIMES];
	u16 sum=0;
	u16 temp;
	for(i=0;i<READ_TIMES;i++)buf[i]=TP_Read_AD(xy);		 		    
	for(i=0;i<READ_TIMES-1; i++)//排序
	{
		for(j=i+1;j<READ_TIMES;j++)
		{
			if(buf[i]>buf[j])//升序排列
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




//读取x,y坐标
//最小值不能少于100.
//x,y:读取到的坐标值
//返回值:0,失败;1,成功。
u8 TP_Read_XY(u16 *x,u16 *y)
{
	u16 xtemp,ytemp;			 	 		  
	xtemp=TP_Read_XOY(CMD_RDX);
	ytemp=TP_Read_XOY(CMD_RDY);	  												   
	//if(xtemp<100||ytemp<100)return 0;//读数失败
	*x=xtemp;
	*y=ytemp;
	return 1;//读数成功
}





//SPI总线法 读取XPT2046数据
//采集触摸时xpt输出的物理坐标 x y  
//此函数没有转换成LCD的实际坐标 
//为校准函数做读取
//返回 xpt2046ad.x  物理x坐标
//     xpt2046ad.y  物理y坐标
//     xpt2046ad.flag 采集成功标志
struct TFT_Pointer xpt2046ad()		                                 
{
 struct TFT_Pointer result;
// u8   ax[8];
 u16 x1,y1;
 u16 x2,y2;

 result.x=0;
 result.y=0;
 result.flag=0;
 
#define ERR_RANGE 10 //误差范围 
  
 if(xpt2046_isClick==0)
 {	 
  delay_ms(1);
  if(xpt2046_isClick==0)
  {
    TP_CS=0; 		//开片选
   
   /*这里采用16时钟周期采集  因为 此触摸功能采用的是SPI总线
     而SPI功能是只能8位传输  XPT2046的AD分辨率为12位  
	 这样8位的SPI接收是不行的
	 所以 根据XPT2046手册中 16时钟周期 时序图 可以看出
	 发送采集数据  接收一次SPI数据后 在发送空功能的SPI数据  就会把剩下的部分接收到
	 这样先接收的 是低字节数据  第二次接收的是高字节数据  移位后 便是12位的AD值   
   */

	TP_Read_XY(&x1,&y1);
	TP_Read_XY(&x2,&y2);


if(((x2<=x1&&x1<x2+ERR_RANGE)||(x1<=x2&&x2<x1+ERR_RANGE))//前后两次采样在+-ERR_RANGE内
    &&((y2<=y1&&y1<y2+ERR_RANGE)||(y1<=y2&&y2<y1+ERR_RANGE)))
   {
   	result.flag=1;			//打开标志位
	result.x=(x1+x2)/2;
	result.y=(y1+y2)/2;
   }
   else result.flag=0;

   TP_CS=1; 		//关片选
   
  }
 } 

 return result;
}




//触摸数据转换屏实际坐标函数体
//返回 result.x坐标 result.y坐标 
struct TFT_Pointer TFT_Cm(void)		                                  
{
 u16 a,b;				//临时变量
 struct TFT_Pointer result;

 result=xpt2046ad();	 //读取AD值

 if(result.flag)			 //有触摸现象为前提
 {

//触摸板X Y
   a=result.x;
   b=result.y;
/* 触摸屏计算公式
   lcdx=xa*tpx+xb;
   lcdy=ya*tpy+yb;
   lcdx,lcdy为屏坐标  tpx,tpy为触屏板值 xa,ya为比例因子  xb,yb为偏移量

   计算方法 
   在屏幕上指定lcdx,lcdy位置画出十字图形 分别画在屏幕上的4个角位置
   用触摸笔分别点击 得到其中的触摸值
   根据上面的公式 计算	xa,ya  xb,yb  这样就能使得触摸板和屏幕校准
  */
	 
//		 result.x=240-(0.065894*a-16);			//将得到的AD值带入公式 计算lcd屏的x y坐标 
//		 result.y=320-(0.084031*b-14);		//如果y轴 上下颠倒 改为	result.y=0.084031*b-14;


   		 result.x=((float)T_flash.xi/10000)*a+T_flash.a;			//将得到的AD值带入公式 计算lcd屏的x y坐标 
		 result.y=((float)T_flash.yi/10000)*b+T_flash.b;


 }
 return result;								//返回坐标值
}














 //十字校准标记
//c 十字交点坐标
//color 十字校准颜色
//color2 十字校准圆颜色
void cross(u8 c,u16 color,u16 color2) 
{
switch(c)
{
case 1:
  GUI_box(18,0,20,39,color);	//画竖
  GUI_box(0,18,39,20,color);	//画横
  GUI_arc(19,19,12,color2,0);     //画同心圆
  GUI_arc(19,19,10,color2,0);     //画同心圆
  break;
case 2:
  GUI_box(218,0,220,39,color);
  GUI_box(199,18,239,20,color);
  GUI_arc(220,19,12,color2,0);     //画同心圆
  GUI_arc(220,19,10,color2,0);     //画同心圆
  break;
case 3:
  GUI_box(18,279,20,319,color);
  GUI_box(0,298,39,300,color);
  GUI_arc(19,299,12,color2,0);     //画同心圆
  GUI_arc(19,299,10,color2,0);     //画同心圆
  break;
case 4:
  GUI_box(218,279,220,319,color);
  GUI_box(199,298,239,300,color);
  GUI_arc(220,299,12,color2,0);     //画同心圆
  GUI_arc(220,299,10,color2,0);     //画同心圆
  break;  
}

}



//触摸校准函数
//此函数 主要更新校准参数T_i2c里的数据
//校准后 可选择是否更新 
//更新 直接将校准的参数 存入 24c02 10-17地址为里 18存校准标志位  更新后 地址18里的数会写1
//不更新 直接跳出该函数
void T_correct(void)
{
struct TFT_Pointer xpt;

u8 cnt=0,
   update=0;       //更新触摸数据标志

short pos_temp[4][2],//坐标缓存值
      d1,d2;		   //临时变量

u32 tem1,tem2;	   //临时变量
  
short ox,oy;	   //触摸计算变量

float fac, 
      xfac,yfac;   //触摸计算变量

short xoff,yoff,   //触摸计算变量	全触摸屏校准 4个变量都有可能是负值
      fx,fy;

GUI_Clear(White);	       //清屏
cross(1,Blue,Red3);			   //画十字标记


GUI_sprintf_hzstr16x(20,80, "请点4次十字标获得校准值",Red,White);
GUI_sprintf_hzstr16x(20,100, "校准成功后数据自动更新！",Blue,White);
while(1)
{
 xpt=xpt2046ad();		   //采集触摸值 这个值为 触摸AD的真实值 
 if(xpt.flag==1)
 {
  delay_ms(500);
  pos_temp[cnt][0]=xpt.x;  //将触摸x y值带入数组
  pos_temp[cnt][1]=xpt.y;
  cnt++;
  switch(cnt)
  {
   case 1:
       cross(1,White,White);	   //清一点触摸标记
	   cross(2,Blue,Red3);	   //画二点触摸标记
	   break;
   case 2:
       cross(2,White,White);	   //清2
	   cross(3,Blue,Red3);	   //画3
	   break;
   case 3:
       cross(3,White,White);	   //清3
	   cross(4,Blue,Red3);	   //画4
	   break;
   case 4:				   //4点触摸值都得到后 开始校准
	    //对边相等
		tem1=abs(pos_temp[0][0]-pos_temp[1][0]);//x1-x2
		tem2=abs(pos_temp[0][1]-pos_temp[1][1]);//y1-y2
		tem1*=tem1;
		tem2*=tem2;
		d1=sqrt(tem1+tem2);//得到1,2的距离
		
		tem1=abs(pos_temp[2][0]-pos_temp[3][0]);//x3-x4
		tem2=abs(pos_temp[2][1]-pos_temp[3][1]);//y3-y4
		tem1*=tem1;
		tem2*=tem2;
		d2=sqrt(tem1+tem2);//得到3,4的距离
		fac=(float)d1/d2;       
		if(fac<0.95f||fac>1.05f||d1==0||d2==0)//不合格
		{
			cnt=0;
			led_d2=0;
	    	cross(4,White,White);	//清除点4
			cross(1,Blue,Red3);	//画点1 
			continue;
		}
		tem1=abs(pos_temp[0][0]-pos_temp[2][0]);//x1-x3
		tem2=abs(pos_temp[0][1]-pos_temp[2][1]);//y1-y3
		tem1*=tem1;
		tem2*=tem2;
		d1=sqrt(tem1+tem2);//得到1,3的距离
		
		tem1=abs(pos_temp[1][0]-pos_temp[3][0]);//x2-x4
		tem2=abs(pos_temp[1][1]-pos_temp[3][1]);//y2-y4
		tem1*=tem1;
		tem2*=tem2;
		d2=sqrt(tem1+tem2);//得到2,4的距离
		fac=(float)d1/d2;
		if(fac<0.95f||fac>1.05f)//不合格
		{
			cnt=0;
			led_d2=0;
	    	cross(4,White,White);	//清除点4
			cross(1,Blue,Red3);	//画点1 
            continue;		//校准不合格后画1点触摸标记 继续校准
		}
		
		//正确了
		GUI_Clear(White);	//清屏
			
		//二元一次方程带入公式   LCDx=xfac*pos+xoff
		xfac=(float)(239-40)/(pos_temp[1][0]-pos_temp[0][0]);//得到xfac		 
		xoff=(239-xfac*(pos_temp[1][0]+pos_temp[0][0]))/2;//得到xoff
			  
		yfac=(float)(319-40)/(pos_temp[2][1]-pos_temp[0][1]);//得到yfac
		yoff=(319-yfac*(pos_temp[2][1]+pos_temp[0][1]))/2;//得到yoff 




		 GUI_sprintf_hzstr16x(20,20, "校准结果：",Blue,White);

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


	    ox=xoff;				 //提取
	    oy=yoff;

	    if(xoff<0)
	    {
	     ox=~ox+1;	 //取补码=反码+1
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

//	    Dialog_box(59,120,159,170,Black,Red,window,"更新触摸数据");	  //画带有说明的巨型对话框
//		Touch_key(65,145,Blue2,Blue,0);    //确定按钮框
//		Touch_key(117,145,Blue2,Blue,1);   //取消按钮框
		delay_ms(500);
        GUI_sprintf_hzstr16x(20,150, "校准数据开始更新。。。",Black,White);
		delay_ms(500);
//		while(1)							//key14 强行更新触摸数据到24c中
//		{
//		 xpt=TFT_Cm();					   //采集触摸对应的屏幕坐标
//		 if(xpt.flag==1||S3==0)                 //是否有触摸事件  
//         {
//		  if((xpt.y>145&&xpt.y<165)||S3==0)	   //确定  取消
//		   {
//		    if((xpt.x>65&&xpt.x<101)||S3==0)	   //确认当前选项
//			{


   		flash_tp[0]=fx;				//将校准转换到数组中
		flash_tp[1]=fx>>8;
		flash_tp[2]=fy;
		flash_tp[3]=fy>>8;
		flash_tp[4]=xoff;
		flash_tp[5]=xoff>>8;
		flash_tp[6]=yoff;
		flash_tp[7]=yoff>>8; 	   
		flash_tp[8]=0xF4;			//成功校准后 给标志位 0xF4

		SPI_Flash_Write(flash_tp,xpt_flash_sector*4096,9);	//将校准值存入flash芯片中 200扇区  地址是0-9 9个字节

	 	GUI_sprintf_hzstr16x(20,180, "数据更新完成！",Red3,White);
		delay_ms(500);
		GUI_sprintf_hzstr16x(20,200, "准备返回主界面",Blue,White);
		update=1;
		delay_ms(1000);
	    break;
//			 }
//			}
//  	 	   }
//        } 
	 }
  }
 if(update!=0) break;	  //跳出函数


}
}




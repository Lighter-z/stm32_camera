

#include  "mcusys.h"
#include  "delay.h"				  //延时函数
#include  "tft.h"			      //TFT IC底层驱动
#include  "gui.h"
#include  "xpt2046.h"
#include  "spi.h"
#include  "sdio_sdcard.h" 
#include  "pff.h"				  //文件系统调用.h加载
#include  "OVbmp.h"
#include  "malloc.h"


extern u8  tbuf[512];			   //512字节 SD卡数据缓存空间


//更新TF卡上以有240x240 BMP图片
//从屏幕（0,0）位置到(240,240)位置 截取240x240图片数据 并转换为BMP格式存入TF卡中
//说明：更新的图片必须是以存在TF卡指定路径的240x240 16位bmp图片
//      这里我们称之为底片  这个底片是用软件Image2Lcd软件制作的
//      底片我们已经制作好30张 存入在ov76文件夹下  可以直接调用
//函数思路：因为petit fatfs文件系统只能在原有的文件上更新，而BMP图片前54字节
//          是图片的信息 这一部分是不要更改 保存原有就行
//          所以程序上要先将前54字节取出 然后后在更新54字节以后的图片数据
//注意：	petit fatfs文件系统 在写数据时必须要从 扇区的开头写起 不弄成扇区的中间写数据
//          所以在函数提取了前54字节后 在和后面的颜色数据组成512字节 在重新写回扇区
//*path 保存路径
//返回0 保存成功  1保存失败
u8 Write_Bmp(const char *path)
{
FRESULT res;
u16 br,i,j,m=0,n=239,color;

	   
                                     //由于文件路径在外部SRAM中 所以这里要开启外部SRAM才能调用路径
res=pf_open(path);		             //打开指定路径文件	 这一步可以打开任何路径下的文件 注意它的功能就是打开文件，不是文件夹
                                     //读文件内数据的前提是必须要打开这个文件

if(res == FR_OK)
{
  pf_read(tbuf,54,&br);				 //提取BMP图片前54字节图片信息


  pf_open(path);					 //重新打开路径 将扇区指向图片首数据位置


     for(i=27;i<256;i++)				 //提取512个字节到tbuf中 即256个颜色点
     {
	     color=LCD_readpoint(m,n);		 //符合摄像头摄像效果 	  提取颜色
		   color=((color>>1)&0x7fe0)+(color&0x001f);	//将提取的565格式转换为555格式

       tbuf[i*2]=color;				 //存入时低字节在前
	     tbuf[i*2+1]=(color>>8);
	     m++;
     }	  
     pf_write(tbuf,512,&br);			 //向TF卡内写入512个字节

     for(j=0;j<254;j++)
	   {
	     for(i=0;i<256;i++)				 //提取512个字节到tbuf中 即256个颜色点
	      {
					color=LCD_readpoint(m,n);		 //符合摄像头摄像效果  提取颜色
					color=((color>>1)&0x7fe0)+(color&0x001f); //将提取的565格式转换为555格式

					tbuf[i*2]=color;				 //存入时低字节在前
					tbuf[i*2+1]=(color>>8);
	
		      m++;
		      if(m==240)
		      {
		         m=0;
			       n--;	   		//这里不用判断m 因为循环固定 直接会跳出 
		      }
	      }	  
	     pf_write(tbuf,512,&br);			 //向TF卡内写入512个字节	 
	 
	   }

//   SD_DisSelect();//取消TF卡片选  在写入函数里加取消片选 会有影响 所以在最后写入完成加取消片选
   return 0;  //写入成功

}
return 1;    //错误

}




//解码16位色、24位色BMP图片
//图片显示在 屏幕的正中位置
//进入bmp首扇区后 前4个字节为 为文件大小  18-21宽  22-25 高  28象素，其值为1、4、8、16、24、或32 
//*path  bmp图片文件名
//返回 0  显示完成  1 显示出现问题 
u8 Play_Bmp(const char *path)
{
FRESULT res; 

u16 br,zy,height,	  //width,height图片的初始左边
     y1,i1,tmp;		              //tmp 16位解码变量
u8 zx,width,x1,
   rgb=0, 
   Bmpcolor;

u32 num=0,num2;


res=pf_open(path);		 //打开指定路径文件

if(res == FR_OK){


    pf_read(tbuf, 54, &br);		 //取前54字节  前54字节含有bmp 文件大小 文件长宽尺度 像素值 等数据信息   
	if(br!=54) return 1;		 //提取出错

											//实际宽和高都是用4个字节表示的，但是犹豫图片的大小不能超过屏的尺寸
											//这里只用一个字节表示宽度,两个字节表示高度
	width= tbuf[18];				        //计算文件宽度	 
	height= ((u16)tbuf[23]<<8)+tbuf[22];	//计算文件高度

	Bmpcolor=tbuf[28]/8;					//获取颜色深度 一般是16位 24位 32位  

	//将小于屏幕尺寸的图片放到屏幕正中间显示
	if(width<239)   zx=(240-width)/2;         else zx=0;
	if(height<319)	zy=(320-height)/2;        else zy=0;
	
	num2=width*height;		   //计数此图片在屏幕显示的点（像素）的个数
	x1=zx; y1=zy;			   //赋值计算后的值


//	if(lcd_id==9325)
//	 {		//BMP图片解码的扫描方式为 从下到上 从左到右
//		    Lcd_WriteReg(0x0003,0x1010); //扫描方向设置
//		    Lcd_WR_Reg(0x0022);			 //发送设置�
//	  }
//	else if(lcd_id==9341)
	     LCD_scan(2);	  //BMP图片解码的扫描方式为 从左到右   从下到上 
	
    Address_set(x1,y1,x1+width-1,y1+height-1,x1,y1+height-1);         //设置显示范围 先扫横行 在扫纵行	 最后两位兼容9325  9341无效

	TFT_RS=1;    //写数据线拉高	 	  为提高写入速度 主循环前拉高
	while(1)                   //一直到最后一簇
	{
		  	  
	 pf_read(tbuf, 512, &br);		 //从54字节后位置读取512个字节到缓存区  
	     
		
	 for(i1=0;i1<512;i1++)
	 {
	  	if(Bmpcolor==2)				 //16位BMP
		{
         switch(rgb)				 //555转565格式
		 {
		  case 0:
		     tmp=((u16)tbuf[i1]&0x1f);			 //R
			 tmp+=(((u16)tbuf[i1])&0xe0)<<1;	 //G
			  break;
		  case 1:
             tmp+=(u16)tbuf[i1]<<9;				 //B
			 break;		     		 
		 }	                                                
		}
		else if(Bmpcolor==3)		//24位BMP解码 RGB分别占8个字节
		{
		  switch(rgb)
		  {
		    case 0:
			  tmp=tbuf[i1]>>3;					 //B
			  break;
			case 1:
			  tmp+=((u16)tbuf[i1]<<3)&0x07e0;	 //G
			  break;
			case 2:
			  tmp+=((u16)tbuf[i1]<<8)&0xf800;	 //R
			  break;	  
		  }
		
		}
		rgb++;

		if(rgb==Bmpcolor)
		{	  
			//为了提高显示速度 直接调用IO口本身
      GPIOB->BSRRH = GPIO_Pin_All;     //清寄存器数据
      GPIOB->BSRRL = tmp;            //将采集的颜色值给PB口寄存器 			
			


		    //写数据  因为高位在前面 所以前面的数要比后面的多1  void Lcd_Write_Data(u16 Data)函数的分解

			tmp=0;
			rgb=0;

		    TFT_WR=0;								      //开始写入
        TFT_WR=1;		  
			
			
			num++;//开始打点计数 	       
			if(num==num2)			 //当显示的像素点 和图片信息里计算的一样时 跳出
			{
	
//			  	if(lcd_id==9325)
//				{
//			        							 //恢复正常扫描方式
//				 Lcd_WriteReg(0x0003,0x1030);	 //LCD显示方向设置   手册56页
//				 Lcd_WR_Reg(0x0022);			 //发送设置
//				}
//				else if(lcd_id==9341) 
                LCD_scan(1);

				 return 0;    //显示完成	

			}

		 }

	  } 

	 }


	 
   }
 return 1;   //可能出错
}		    







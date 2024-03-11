
#include  "mcusys.h"
#include  "delay.h"				  //延时函数
#include  "tft.h"			  //TFT IC底层驱动
#include  "gui.h"				  //GUI显示控制函数
#include  "xpt2046.h"
#include  "spi.h"				  //STC12 硬体SPI底层函数
#include  "sdio_sdcard.h" 
#include  "led.h"
#include  "pff.h"
#include  "OVroot.h"
#include  "OVbmp.h"
#include  "string.h"				//合并指针函数
#include  "malloc.h"
//#include  "touch.h"
#include  "flash.h"
//#include  "eeprom.h"


extern FATFS fatfs;
extern u8 tbuf[512];






//文件系统根目录菜单 选项触摸值返回函数
//判断触摸后 符合要求的返回相应数据
//x y 触摸事件发生时 坐标值
//返回18 触摸值不在设置区域 不效应  返回0-17 在触摸设置范围
u8 Click_Root(u16 x,u16 y)	   
{
 u8 result=18;		          //13个触摸文件项  
							  
 if(x>=16&&x<=150&&y>=41&&y<=60)
  result=0;
 else if(x>=16&&x<=150&&y>=61&&y<=80)
  result=1;
 else if(x>=16&&x<=150&&y>=81&&y<=100)
  result=2;
 else if(x>=16&&x<=150&&y>=101&&y<=120)
  result=3;
 else if(x>=16&&x<=150&&y>=121&&y<=140)
  result=4;
 else if(x>=16&&x<=150&&y>=141&&y<=160)
  result=5;
 else if(x>=16&&x<=150&&y>=161&&y<=180)
  result=6;
 else if(x>=16&&x<=150&&y>=181&&y<=200)
  result=7;
 else if(x>=16&&x<=150&&y>=201&&y<=220)
  result=8;
 else if(x>=16&&x<=150&&y>=221&&y<=240)
  result=9;
 else if(x>=16&&x<=150&&y>=241&&y<=260)
  result=10;
 else if(x>=16&&x<=150&&y>=261&&y<=280)
  result=11;
 else if(x>=16&&x<=150&&y>=281&&y<=300)
  result=12;

 else if(x>=200&&x<=239&&y>=1&&y<=40)	 //返回
  result=17;
 return result;								 //返回结果
}


//根目录触摸函数  	  主要应用于文件系统FATFS
//支持划屏翻文件项和进度条箭头单点触摸移入文件项
//返回 对应文件项 位置值  为进一步读取 打开文件项做准备
u8  ROOT_Touch(void)
{

    u8 cp;
	u16 ya=0,yb=0;
	struct TFT_Pointer sp;

	sp=TFT_Cm();	   // 扫描触摸屏
	if(sp.flag==1)					//有触摸现象
	 {	
	   cp=Click_Root(sp.x,sp.y);	//判断触摸发生位置的对应值
	   ya=sp.y;		  				//第一次提取 y 坐标值  
		   
	   delay_ms(200);				//延时				 
	   while(xpt2046_isClick==0)        //判断触摸是否一直被按下  说明进入划屏翻页状态
		{
		  sp=TFT_Cm(); 				//提取触摸数据
		  if(sp.flag==1)			//有触摸现象
		  {
			yb=sp.y;				//第二次提取 y 坐标值	 将两次y值进行比较 判断划屏方向 
			if(yb>(ya+5)) return 15;		   //上移
			else if(yb<(ya-5)) return 16;	   //下移
			else return 18;					   //无现象 返回
		  }		  
		}
	   return cp; 		//单点触摸 直接返回采集数据
	   
	 }

  return 18;
}





//根目录触摸函数  	  主要应用于文件系统FATFS
//支持划屏翻文件项和进度条箭头单点触摸移入文件项
//返回 对应文件项 位置值  为进一步读取 打开文件项做准备
u8  ROOT_Touch2(void)
{
  u8 cp=18;
//	u16 xa=0,xb=0;
	struct TFT_Pointer sp;

	sp=TFT_Cm();	   // 扫描触摸屏
	if(sp.flag==1)					//有触摸现象
	 {	 
       if(sp.x>=200&&sp.x<=239&&sp.y>=300&&sp.y<=320)	 //返回
        cp=17;	 
			 
			 if(sp.x>0&&sp.x<=120&&sp.y>=40&&sp.y<=280) cp=19;
			 if(sp.x>121&&sp.x<=239&&sp.y>=40&&sp.y<=280) cp=20;
			 	 	 
//	   xa=sp.x;		  				//第一次提取 y 坐标值  
//		   
//	   delay_ms(200);				//延时				 
//	   while(xpt2046_isClick==0)        //判断触摸是否一直被按下  说明进入划屏翻页状态
//		{
//		  sp=TFT_Cm(); 				//提取触摸数据
//		  if(sp.flag==1)			//有触摸现象
//		  {
//			xb=sp.x;				//第二次提取 y 坐标值	 将两次y值进行比较 判断划屏方向 
//			if(xb>(xa+10)) return 19;		   //上移
//			else if(xb<(xa-10)) return 20;	   //下移
//			else return 18;					   //无现象 返回
//		  }		  
//		}
	      return cp; 		//单点触摸 直接返回采集数据	   
	 }

  return 18;
}




u8  *short_name[50];	  //短文件名 存储空间  主要为打开各种文件项 提供路径
//打开一个文件夹 并得到以下所有的文件名和文件内容
// *path  要打开的文件夹路径
//返回 扫描到的文件个数  如果返回0  失败
u8 scan_filename(const char *path)
{
 DIR dir;				 //源文件
 FILINFO finfo;		     //文件信息
 u8 list,i;//lf=0



pf_opendir(&dir,path);	   //读取指定文件名下的 DIR数据




 for(i=0;i<50;i++)		   //给每一个的指针数组 申请32字节内存空间  为存储文件名做准备 
 {
  short_name[i]=mymalloc(SRAMIN,32);
 }  





for(list=0;list<50;list++)			 //最大接收32个文件项
  {
    pf_readdir(&dir,&finfo);				  //读取打开文件夹下的文件信息 finfo

    if(finfo.fname[0]==0x00)				  //当文件夹名第一个字节为 0X00时 为无文件  跳出函数
	  {

	  list-=1;								  //list最后多加1 这里减去 取真实值
	  return list;
	  } 			            
	
	strcat((char*)short_name[list],(const char*)finfo.fname) ;	//存储所有短文件名 做读取时文件路径

  }



return 0;
		 
}




//设置显示区域
void scan_root()
{
GUI_Clear(White);	       //清屏

GUI_box(0,18,239,19,Black);
GUI_box(0,38,239,39,Black);

GUI_box(0,0,239,8,Black);
GUI_box(0,9,239,17,Black);
GUI_sprintf_hzstr16x(0,1,"OV相册",Yellow,1);

GUI_sprintf_hzstr16x(207,1,"返回",Yellow,1);
GUI_box(0,20,239,37,Green1);
GUI_sprintf_hzstr16x(0,21,"照片数量：",Red,1);

GUI_box(225,40,226,319,Black);	  //竖线1
GUI_box(238,40,239,319,Black);	  //竖线2

GUI_box(227,58,238,59,Black);	  //横线1
GUI_box(227,299,238,300,Black);	  //横线2

GUI_box(227,40,237,57,Blue);	  //调解区颜色填充
GUI_box(227,301,237,319,Blue);

}




//led指示灯
void led_forroot(void)
 {
  led_d2=0;delay_ms(100);led_d2=1;
 }

//文件系统 模块 扫描及显示主函数  
//此函数 包括 打开及显示整个tf 内容
//目前可以打开 多层次文件夹  TXT  BMP  BIN等文件
//此函数在打开多层文件夹时利用递归方式
//const char *path  要打开文件的文件名路径

u8 m[32];		           //提取内存文件名 备用数组  因显示 要关闭内存
		   
u8  *tp2;			     //存储预打开文件路径

u8 sign=0;			       //文件夹 进入层次  此函数为打开文件夹函数

void tp_show(const char *path)
{

u8 touch_root,		   //触摸值 提取临时存储变量
   i,j,			       //临时变量
   offset=0,st,        //文件 所在页面显示的偏移量  触摸调用
   st2;			       //判断当前文件夹下 文件项个数 是否小于14

u16 of=0; 			   //进度条计算变量
u8  yof=0;             
u8  to2,st3=0,		   //照片左右翻 变量
    page;			   //相片每14个一页 总相片所占的页数

st=scan_filename(path);   //提取指定路径下 所有文件名 及其信息 存入全局外部内存空间	  	 
						  //所有短文件名 存储空间*short_name	  
						  //st返回找到的文件个数

st+=1;			          //st是从0开始计算的 因此要多加1	 st为总文件项数
   
page=st/14;				  //每页14个文件项 通过总文件数 计算出需要的页数
if(st%14)
page++;

of=236/page;			  //进度条长度为236点， 计算每一页所要要用的进度条长度


sign=0;					  //跳出函数标志位置零

scan_root();                  //画显示区域
number(80,21,st,Red,1);	  //显示当前页文件项个数


tp2=mymalloc(SRAMIN,32);			  //申请 路径存储指针  tp2


							   
			        //显示 触摸区域
while(1)
{

	st2=14;						 //初始当页相片个数
								 //显示部分
	GUI_box(0,40,224,319,White); //清空文件项显示区域
    for(j=0;j<14;j++)		     //显示文件项  在屏幕上设置最多可以显示 13个文件项
    {	

	
	  for(i=0;i<32;i++)		     //将文件名存入临时数组 供显示
	  m[i]=short_name[offset+j][i];
	

	  if(m[0]!=0)					 //有文件则显示图标
	   flash_binplay(0,(j+2)*20,22,20,306);
	  else 	st2--;					 //如果没有照片 则当页相片个数减1

	  GUI_sprintf_hzstr16x(22,2+(j+2)*20,m,Black,White);	  //显示文件名	
    }



	//清进度条显示区域
	GUI_box(228,61,236,297,White);						  
	                                                    
	yof=((offset/14)+1)*of;							  //计算进度条 

	GUI_box(228,61,236,(61+yof),Blue);	              //显示当前进度


	while(1)	   //触摸判断区域
	{
		touch_root=ROOT_Touch();	//提取触摸值 给临时变量touch_root


		if(touch_root<st2)			//说明有文件被选中	   0-14 文件触摸数据返回值
		{
	     //在当前选择的文件项增加下划线提示
		 GUI_box(23,58+touch_root*20,224,59+touch_root*20,Blue);
		 delay_ms(500);
		

														//当前文件路径 + "/" + 要打开文件名=预打开文件路径 
		 strcat((char*)tp2,(const char*)path);							//提取当前文件路径
		 strcat((char*)tp2,(const char*)"/");							//加 /
		 strcat((char*)tp2,(const char*)short_name[offset+touch_root]); //生成预打开文件路径

			



			 GUI_Clear(Black);	       //清屏
			 Play_Bmp((const char *)tp2);

		 				     //显示打开图片的文件名字
			 for(i=0;i<32;i++)		     
			 m[i]=short_name[offset+touch_root][i];

			 GUI_sprintf_hzstr16x(0,0,m,White,1);

			 GUI_sprintf_hzstr16x(206,302,"返回",White,1);

			 st3=offset+touch_root;

			 while(1)				 //照片左右翻页功能
			  {
				to2=ROOT_Touch2();
				if(to2==20)
				 {
				     st3++;
					 
				     if(st3<st)
					 {
					   if(st3==st-1){st3-=1;GUI_sprintf_hzstr16x(143,0,"已到相册底部",White,1);}

					   for(i=0;i<32;i++)		   //清空路径指针
					   tp2[i]=0;					   
					   strcat((char*)tp2,(const char*)path);						//提取当前文件路径
					   strcat((char*)tp2,(const char*)"/");							//加 /
					   strcat((char*)tp2,(const char*)short_name[st3]);             //生成预打开文件路径


			 		   GUI_Clear(Black);	       //清屏
					   Play_Bmp((const char *)tp2);			   //显示照片

				 	   				   //显示照片名  
					   for(i=0;i<32;i++)		     
					   m[i]=short_name[st3][i];
				
					   GUI_sprintf_hzstr16x(0,0,m,White,1);
					   GUI_sprintf_hzstr16x(206,302,"返回",White,1);
					 }  
				 }
				if(to2==19)
				 {
				     st3--;
					 if(st3==255){st3+=1;GUI_sprintf_hzstr16x(143,0,"已到相册顶部",White,1);}
					 else if(st3>0&&st3<st)
					 {
			 		
					   for(i=0;i<32;i++)		   //清空路径指针
					   tp2[i]=0;					   
					   strcat((char*)tp2,(const char*)path);						//提取当前文件路径
					   strcat((char*)tp2,(const char*)"/");						    //加 /
					   strcat((char*)tp2,(const char*)short_name[st3]);             //生成预打开文件路径
					 

			 		   GUI_Clear(Black);	       //清屏
					   Play_Bmp((const char *)tp2);			   //显示照片

				 	    				   //显示照片名  
					   for(i=0;i<32;i++)		     
					   m[i]=short_name[st3][i];
				
					   GUI_sprintf_hzstr16x(0,0,m,White,1);
					   GUI_sprintf_hzstr16x(206,302,"返回",White,1);
					 }  
				 }
				if(to2==17) 
				 {
				   GUI_sprintf_hzstr16x(206,302,"返回",Red3,1);
				   GUI_sprintf_hzstr16x(207,303,"返回",White,1);
				   delay_ms(300);
				   break;
				 }
				 
			  }

		   	 scan_root();        //重新设置显示区域
			 number(80,21,st,White,1);	
			 														    
			
			 for(i=0;i<32;i++)		   //清空路径指针
			 tp2[i]=0;
		

			 led_forroot();		 //指示灯闪烁
			 break;              //跳出此级循环体 

		}




		
		if(touch_root==15)			//向上
		 {
		   offset-=14;
		   if(offset>100)offset=0;
		   break;					
		 }

		if(touch_root==16)			//向下
		 {
		   offset+=14;
		   if(offset>=st)offset-=14;	 //如果超过总数 那么就恢复offset原有的偏移值  
		   break;				 
		 }
		 	 		

		if(touch_root==17)		    //返回上一层
		{		  
		  GUI_sprintf_hzstr16x(207,1,"返回",Red,1);
		  led_forroot();		    //指示灯闪烁
		  delay_ms(300);
		  sign=1;				    

//		   for(i=0;i<32;i++)		      //释放短文件名数组  内存空间 
//		   	myfree(SRAMIN,short_name[i]);
//		  				 
//		   myfree(SRAMIN,tp2);					  //释放路径内存空间
		  mem_init(SRAMIN); 	  
		  if(sign==1)break;	    

		}

	 }


	 if(sign==1) break;			//跳出相册			
	 
}
}








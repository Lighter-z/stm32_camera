


#include "mcusys.h"
#include  "delay.h"
#include  "tft.h"
#include  "zifu8x16.h"
#include  "hz32x32.h"
#include  "zm16x32.h"
#include  "gui.h"
#include "flash.h"
#include "tft.h"

#include "malloc.h"

//u32  unicode_sector, 		   //UNI码首扇区
//       gbk12_sector;		   //GBK码首扇区

//提取 TF卡中  GBK点阵码  和 UIN码 首扇区  为直接以TF为字库读取即显示做准备  如果直接调用FLASH则可直接屏蔽此函数

////读取 两字库开始扇区
////hzk16_sector    GBK 点阵16X16字库 起始扇区
////unicode_sector  UNICODE码 起始扇区  主要应用于长文件名
////返回值 0  成功  其他失败
//u8 get_font_sector(void)
//{
//FRESULT res;
//
//res=pf_open("SYSTEM/UNI.TXT");  //找到 SYSTEM下的 UNI.txt  unicode码
//  if(res == FR_OK)				//寻找成功  读取文件的开始簇 并计算其扇区
//    unicode_sector=(fatfs.org_clust-fatfs.dirbase)*fatfs.csize+fatfs.database;
//	              //unicode_sector=（当前文件簇-目录所在簇）*每簇扇区数+ 根目录扇区数         
//  else return 1;
//res=pf_open("SYSTEM/GBK12.DZK");  //找到 SYSTEM下的 GBK12.DZK汉字库
//  if(res == FR_OK)				//寻找成功  读取文件的开始簇 并计算其扇区
//    gbk12_sector=(fatfs.org_clust-fatfs.dirbase)*fatfs.csize+fatfs.database;
//	              //理解同上
//				  //  gbk12_sector= fatfs.dsect;        
//  else return 2;
//
//return  0;
//}




//在指定位置显示 字库里的 GBK汉字   当字体背景颜色 只是0x0001 那么此时不显示背景颜色 背景颜色为默认颜色
//支持横向纵向显示选择功能 mode  但是只有纵向显示的时候才支持 背景颜色保持屏幕本身有的颜色
// x  y  显示的具体坐标
// Disp_char[2] 需要显示的GBK码
// fColor  	 bColor    字体 背景颜色 
// mode    0 纵向显示  1 横向显示  （主要要配合扫描方式）
//返回0 显示成功  返回1 在字库内 没有对应的GBK码
u8 PutGB1616(u16 x, u16  y, u8 Disp_char[2], u16 fColor,u16 bColor,u8 mode)
{

    u8 qh,ql;
	u8 i,j;
    u8  tmp_Char_Model[32];	//GBK12 点阵字库中  提取一个16X16点阵 即32个字节

    qh=Disp_char[0];
	ql=Disp_char[1];

	if(qh<0x81||ql<0x40||ql==0xff||qh==0xff)//非 常用汉字
	return 1; //结束访问 
	          
	if(ql<0x7f)ql-=0x40;//根据低字节在两个区域 将高低字节转移到 指定字节位置
	else ql-=0x41;
	qh-=0x81; 	 


    // 读字模 					   //  得到区值    	得到位数值			 一个字 32个字节
    //SD_read_Byte((gbk12_sector<<9) + ((u32)qh*190 + (u32)ql)*32,tmp_Char_Model,32);		//TF卡 读取模式	

	/*
	 TF卡模式
	 gbk12_sector 为字库所在扇区   gbk12_sector<<9相当于X512  转为字节数据 即字库所在字节开始位置
	 每个GBK 码由2 个字节组成，

	 FLASH 模式 
	 flash 中已经将TF卡里的 字库数据完整存入 flash中 只要确定地址即可 这个地址就是存入时的第一个地址 
	 这里是从 第322个扇区开始存入的  flash的一个扇区是4k的字节空间 即 4096个字节 所以首地址是 322*4096

	 第一个字节为0X81~0XFE，
	 第二个字节分为两部分， 一是0X40~0X7E，二是0X80~0XFE 。
	 第一个字节代表为区，那么GBK 里面总共有126 个区（0XFE-0X81+1 ）， 
     每个区内有 190 个汉字（0XFE-0X80+0X7E-0X40+2 ），总共就有 126*190=23940 个汉字
	 点阵库编码规则从 0X8140  开始   所以首先要判断低字节所在区域  然后得到具体字节位置
	 ((u32)qh*190 + (u32)ql)*32 每个区 190个字   一个字32个字节
	*/

	SPI_Flash_Read(tmp_Char_Model,(u32)322*4096+((u32)qh*190 + (u32)ql)*32,32);	   //FLASH读取模式


   if(bColor!=0x0001)							//支持背景颜色设置
    {														 //注意 不管设置哪一显示方向 前提是扫描要对应
	    if(mode==0)Address_set(x,y,  x+16-1, y+16-1,x,y);	 // 设置为纵向显示
		if(mode==1)Address_set(x,y,  x+16-1, y+16-1,x+15, y);// 设置为横向显示
	    //显示出来 
	    for(i=0;i<32;i++)
	    {
	       	        
	        for(j=0;j<8;j++) 
	        {
	            if(tmp_Char_Model[i] & 0x80) 
	                   {
	                	    Lcd_Write_Data(fColor);
	                	}
	            else 
	                    {
	                	    Lcd_Write_Data(bColor);
	                	}
	            tmp_Char_Model[i] <<= 1;
	        } 
	     }
	 }
   else										//不支持背景颜色设置  只显示字体颜色 背景颜色与刷屏时一致
	{
	    qh=x;								//复制x值给qh
		for(i=0;i<32;i++)
		{
	        for(j=0;j<8;j++) 
	        {
			    Address_set(x,y,x,y,x,y);	//显示坐标要一个点一个点显示
	            if(tmp_Char_Model[i] & 0x80) 
	                   {
	                	    Lcd_Write_Data(fColor);		//显示汉字的颜色
							x++;						//x自加
							if((x-qh)==16){x=qh;y++;}	//到一行底 清x值
	                	}
	            else 
	                    {								//没有汉字的颜色要显示 即背景 背景不显示使用屏幕自带
	                	    x++;						
							if((x-qh)==16){x=qh;y++;}	//到一行底 清x值
	                	}
	            tmp_Char_Model[i] <<= 1;				//下一个字节
	        } 		
		}
	
	}

return 0;//成功
   
}

//清屏
//color是背景颜色。
//说明：使用背景颜色清除TFT模块屏幕的全部显示内容。
void GUI_Clear(u16 color)
{
	u16 i;
	u8 j;
	Address_set(0,0,239,319,0,0);
    for(i=0;i<320;i++)
	 {
	  for (j=0;j<240;j++)
	   	{
        	 Lcd_Write_Data(color);
	    }

	  }
}



//画点
//(x，y)是点的坐标
//color 是点的颜色。
//说明：用指定的颜色在指定的坐标位置上画出一个点。
void GUI_Point(u8 x, u16 y, u16 color)
{  
	Address_set(x,y,x,y,x,y);
	Lcd_Write_Data(color);
}

   

//画直线（可以画任意方向直线，包括横线、竖线、斜线）。
//(x1,y1)起点, (x2,y2)终点, color颜色。
//说明：用指定的颜色在指定的两点间画出一条直线。
void GUI_line(u16 x1,u16 y1,u16 x2,u16 y2,u16 color)  
{  
    u16 t;  
    int xerr=0,yerr=0,delta_x,delta_y,distance;  
    int incx,incy;  
    u16 row,col;  
    delta_x = x2-x1;//计算坐标增量  
    delta_y = y2-y1;  
    col=x1;  
    row=y1;  
    if(delta_x>0) incx=1;//设置单步方向  
    else   
    {  
        if(delta_x==0) incx=0;//垂直线  
        else {incx=-1;delta_x=-delta_x;}  
    }  
    if(delta_y>0)incy=1;  
    else  
    {  
        if(delta_y==0) incy=0;//水平线  
        else {incy=-1;delta_y=-delta_y;}  
    }  
    if(delta_x>delta_y) distance=delta_x;//选取基本增量坐标轴  
    else distance=delta_y;  

    for(t=0;t<=distance+1;t++)  
	{                                     //画线输出  
	    GUI_Point(col, row, color);
        xerr+=delta_x;  
        yerr+=delta_y;  
      	if(xerr>distance)  
        {  
            xerr-=distance;  
            col+=incx;  
        }  
        if(yerr>distance)  
        {  
            yerr-=distance;  
            row+=incy;  
        }  
    }  
}




//画8点(Bresenham算法)		  
//(rx,ry,a,b):参数
//color:颜色
void gui_circle8(u16 rx,u16 ry,int a,int b,u16 color)
{
	GUI_Point(rx+a,ry-b,color);              
	GUI_Point(rx+b,ry-a,color);                       
	GUI_Point(rx+b,ry+a,color);                           
	GUI_Point(rx+a,ry+b,color);             
	GUI_Point(rx-a,ry+b,color);                  
	GUI_Point(rx-b,ry+a,color);               
	GUI_Point(rx-b,ry-a,color);                      
	GUI_Point(rx-a,ry-b,color);                  
}
//在指定位置画一个指定大小的圆
//(rx,ry):圆心
//r    :半径
//color:颜色
//mode :0,不填充;1,填充
void GUI_arc(u16 rx,u16 ry,u16 r,u16 color,u8 mode)
{
	int a,b,c;
	int di;	  
	a=0;b=r;	  
	di=3-(r<<1);	//判断下个点位置的标志
	while(a<=b)
	{
		if(mode)
		for(c=a;c<=b;c++)
		gui_circle8(rx,ry,a,c,color);//画实心圆
 		else gui_circle8(rx,ry,a,b,color);			//画空心圆
		a++;
		//使用Bresenham算法画圆     
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 							  
	}
}

//画实心矩形
//(sx,sy)左上角顶点坐标, 
//(ex,ey)右下角顶点坐标, color颜色
//返回: 无
//说明：在指定位置上画出实心矩形。
void GUI_box(u16 sx,u16 sy,u16 ex,u16 ey,u16 color)
{ 
	u16 temp,temp1,m,n;   
	Address_set(sx,sy,ex,ey,sx,sy); 
    n=ex-sx+1;
	m=ey-sy+1;

	for(temp=0;temp<m;temp++)
	{	 	
		for(temp1=0;temp1<n;temp1++)
	 	{	
			Lcd_Write_Data(color);
		}
	}
}



//画一个10x10的方块
//sx sy  方块坐标 
//color  方块颜色
void GUI_box10x10(u16 sx,u16 sy,u16 color)
{ 
	u16 temp,temp1,m,n,sx1,sy1,sx2,sy2;   

		sx1=239-(sy+9);
		sy1=sx;
		sx2=239-sy;
		sy2=sx+9;
		Address_set(sx1,sy1,sx2,sy2,sx1,sy1);
 	
     n=sx2-sx1+1;
	 m=sy2-sy1+1;

	for(temp=0;temp<m;temp++)
	{
	 	
		for(temp1=0;temp1<n;temp1++)
	 	{	
			Lcd_Write_Data(color);
		}
	}
}


////画颜色递变巨型  可以是横条 竖条等
////(sx,sy)左上角顶点坐标, 
////(ex,ey)右下角顶点坐标, color颜色
//// mode 模式  mode=1  纵向颜色变化 mode=2 横向颜色变化
//void GUI_boxchange(u8 sx,u16 sy,u8 ex,u16 ey,u16 color,u8 mode)
//{ 
//	u16 temp,temp1,m,n,color2;   
//	Address_set(sx,sy,ex,ey); 
//    n=ex-sx+1;
//	m=ey-sy+1;
//	if(mode==2)color2=color;
//	for(temp=0;temp<m;temp++)
//	{	 	
//		for(temp1=0;temp1<n;temp1++)
//	 	{	
//			Lcd_Write_Data(color);
//			if(mode==2)color+=15;
//		}
//		if(mode==1)color+=15;
//		if(mode==2)color=color2;
//	}
//}













//显示英文或数字字符
//函数支持 字符的纵向和横向显示 （要配合扫描方式）
//         支持忽略背景颜色显示 （横向没有开此功能）
//x  y  要显示字符的初始坐标
//value 显示字符数据
//dcolor 显示的字符颜色  bgcolor显示字符的背景颜色
//                       bgcolor 只给1 说明背景颜色使用 当前屏幕颜色（只适用于纵向显示）
//mode   0 字符纵向显示  1 字符横向显示
void GUI_sprintf_char(u16 x,u16 y,u8 value,u16 dcolor,u16 bgcolor,u8 mode)	
{  
	u8 i,j;
	u8 *temp=(u8 *)zifu816;    //temp是*temp的地址  这里temp就是zifu的首地址 
    u8 ma;  
	                    
	temp+=(value-32)*16;   //确定要显示的值
				           //用ascii表  前32个ascii没有存入zifu库里 所以要减32
	                       //每个字符用16个字节显示 所以在乘以16  就是对应的显示位的首地址

    if(bgcolor!=1)		   //用自己设定的背景色
	 {											   //注意 不管设置哪一显示方向 前提是扫描要对应
	  if(mode==0)Address_set(x,y,x+7,y+15,x,y);    //设置区域		   
	  if(mode==1)Address_set(x,y,x+15,y+7,x+15,y); //设置区域   
		for(j=0;j<16;j++)
		{
			for(i=0;i<8;i++)		    //先横扫
			{ 		     
			 	if((*temp&(1<<(7-i)))!=0)		   //将1 左移 然后对应位进行相与 				
				  Lcd_Write_Data(dcolor);		   //显示字符颜色
				 
				else				
				  Lcd_Write_Data(bgcolor);		   //显示背景颜色		
			}
			temp++;								   //下一字节
		 }
	  }
	else				  //用屏幕上自有的颜色做背景色 （只支持纵向显示）
	 {	
	    ma=x;			  //赋值x的初始值给ma 
		for(j=0;j<16;j++)
		{
			for(i=0;i<8;i++)		    //先横扫
			{ 
			    Address_set(x,y,x,y,x,y);
			 	if((*temp&(1<<(7-i)))!=0)	//将1 左移 然后对应位进行相与 
				{
				   Lcd_Write_Data(dcolor);	//显示字符颜色
				   x++;
				   if((x-ma)==8){x=ma;y++;}	//判断是否到一行
				} 
				else
				{
				   x++;						//没有要显示的数据 x自加跳过
				   if((x-ma)==8){x=ma;y++;}	//判断是否到一行
				} 
			}
			temp++;							//下一字节
		}		 			  
	 } 

}



//显示汉字及字符  纵向显示
//x1 y1 显示的初始位置
//*str 要显示的数据
//dcolor 显示汉字 字符的颜色
//bgcolor 显示汉字 字符的背景颜色  当bgcolor=1是 那么背景颜色为屏幕自有颜色
void GUI_sprintf_hzstr16x(u16 x1,u16 y1,u8 *str,u16 dcolor,u16 bgcolor)	  
{  
	 u8 l=0;
	while(*str)
	{	
	  if(*str<0x80)		  //小于128   ascii 都在数组内
	  {	  
		GUI_sprintf_char(x1+l*8,y1,*str,dcolor,bgcolor,0);
		l+=1;
		str++;
		}
	  else
	  {	  
	   PutGB1616(x1+l*8,y1,(u8*)str,dcolor, bgcolor,0);
			str+=2;l+=2;
	  }
	}	
}





//显示汉字及字符  横向显示
//x1 y1 显示的初始位置
//*str 要显示的数据
//dcolor 显示汉字 字符的颜色
//bgcolor 显示汉字 字符的背景颜色  不支持背景屏幕自有色
void GUI_sprintf_hzstr16h(u8 x1,u16 y1,u8 *str,u16 dcolor,u16 bgcolor)	  
{  
	 u8 l=0;
	while(*str)
	{	
	  if(*str<0x80)		  //小于128   ascii 都在数组内
	  {	  
		GUI_sprintf_char(x1,y1+l*8,*str,dcolor,bgcolor,1);
		l+=1;
		str++;
		}
	  else
	  {	  
	   PutGB1616(x1,y1+l*8,(u8*)str,dcolor, bgcolor,1);
			str+=2;l+=2;
	  }
	}	
}













//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////


//显示32x32单个汉字
//(x，y) 是显示内容的左上角坐标，c[2]表示汉字的两个字节
//dcolor 汉字的颜色   bgcolor 背景颜色
//说明：在指定位置上显示一个32x32的汉字
void GUI_sprintf_hz3232(u8 x, u16  y, u8 c[2], u16 dcolor,u16 bgcolor)
{
	u8 i,j,k;

	Address_set(x,y,x+32-1, y+32-1,x,y);     //设置显示范围

	for (k=0;k<40;k++) 	  //自建汉字库（k最大值和汉字库中汉字个数有关），循环查询内码
	{ 
	  if ((codeHZ_32[k].Index[0]==c[0])&&(codeHZ_32[k].Index[1]==c[1]))		//寻找要显示的汉字
	  { 
    	for(i=0;i<128;i++)													//1个汉字要128个字节点来显示
		{
		  u8 m=codeHZ_32[k].Msk[i];											//一个字节一个字节的显示
		  for(j=0;j<8;j++) 
		  {
			if((m&0x80)==0x80) 
			  Lcd_Write_Data(dcolor);	//显示汉字颜色
			else 
			  Lcd_Write_Data(bgcolor);	//显示背景颜色
			m<<=1;
		  }    
		}
	  }  
	}	
}



//显示16x32字母
//x，y是显示内容的左上角坐标，
//c 字母值
//dcolor 汉字的颜色   
//dcolor 显示的字符颜色  bgcolor显示字符的背景颜色
//                       bgcolor 只给1 说明背景颜色使用 当前屏幕颜色
//mode 字母字体 0 细体  1 粗体
//说明：在指定位置上显示英文字母
void GUI_sprintf_ZM3216(u8 x, u16  y, u8 c, u16 dcolor,u16 bgcolor,u8 mode)
{
	
//	struct  typFNT_ZM32 zm;

	u8 i,j,k,ma,m;
	u8 zm[64];
//	u8 *zm;
//  	zm=codeZM_32[];

	for (k=0;k<19;k++) 		  //自建16x32字母库（k最大值和字母库中汉字个数有关），循环查询内码
	{ 
	   
	  if(mode==1&&codeZM_32c[k].Index[0]==c)       //寻找要显示的粗体字母
	  { 

		for(i=0;i<64;i++)
		zm[i]=codeZM_32c[k].Msk[i];

		break;
	  }
	  else if(mode==0&&codeZM_32x[k].Index[0]==c)  //寻找要显示的细体字母
	  {
	    for(i=0;i<64;i++)
	  	zm[i]=codeZM_32x[k].Msk[i];
		break;
	  }
	}

	   if(bgcolor!=1)							   //用自己设定的背景色
		{
		    Address_set(x,y,x+16-1, y+32-1,x,y);       //设置显示范围
	    	for(i=0;i<64;i++) 						   //1个字母要64个字节点来显示
			{
//			  m=codeZM_32[k].Msk[i];				   //一个字节一个字节的显示
			  m=zm[i];
			  for(j=0;j<8;j++) 
			  {
				if(m&0x80) 			
				  Lcd_Write_Data(dcolor);	   //显示字母颜色			
				else 
				  Lcd_Write_Data(bgcolor);     //显示背景颜色
				m<<=1;
			  }    
			}

		}
	   else									   //用屏幕上自有的颜色做背景色 
		{
		    ma=x;							   //赋值x的初始值给ma
	    	for(i=0;i<64;i++) 				   //1个字母要64个字节点来显示
			{
//			  m=codeZM_32[k].Msk[i];		   //一个字节一个字节的显示
			  m=zm[i];
			  for(j=0;j<8;j++) 
			  {
			//	if(m&0x80) 
				if((m&(1<<(7-j)))!=0)
				 {
				   Address_set(x,y,x,y,x,y);	   //设置显示范围
				   Lcd_Write_Data(dcolor);	   //显示字母颜色
				   x++;
				   if((x-ma)==16){x=ma; y++;}
				 }
				else 
				 {
				  x++;						   //没有要显示的数据 x自加跳过
				  if((x-ma)==16){x=ma; y++;}		   //判断是否到一行
				 }
			//	 m<<=1;
			  }    
			}
		 }



}





//显示32x32汉字 和32x16 字母串
//x1，y1是显示内容的左上角坐标，
//dcolor 汉字的颜色   bgcolor 背景颜色
//*str 要显示的数据
//mode 字母字体 0 细体  1 粗体
void GUI_sprintf_ZMHZs(u8 x1,u16 y1,u8 *str,u16 dcolor,u16 bgcolor,u8 mode)	 
{  
   u8 l=0;	  
   while(*str)
   {
	 if(*str<0x80)		  //ascii数组内	        
	 {
	   GUI_sprintf_ZM3216(x1+l*8,y1,*str,dcolor, bgcolor,mode);
	   str+=1;l+=2;
	 }
	 else				  //显示汉字
	 {
	   GUI_sprintf_hz3232(x1+l*8,y1,(u8*)str,dcolor,bgcolor);
	   l+=4;
	   str+=2;
	 }
   }
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////






//显示最大5位的数据  如果非5位状态 其他位不显示 只显示有效值
//支持功能 如果背景颜色 为1 那么不显示背景颜色 背景颜色为写入前默认值
// x y 显示数值坐标  
//dat 数值
//dcolor  显示数值颜色  bgcolor背景颜色
void number(u8 x,u16 y,u32 dat,u16 dcolor,u16 bgcolor)				//坐标 要显示的数据	   跟踪程序数据用
{
 u8 take[5];
 u8 i,m;

 take[0]=dat/10000%10;
 take[1]=dat/1000%10;
 take[2]=dat/100%10;
 take[3]=dat/10%10;
 take[4]=dat%10;

							  //显示位数
 if(dat/10000)m=5;			  //万位
 else if(dat/1000)m=4;		  //千位
 else if(dat/100)m=3;		  //百位
 else if(dat/10)m=2;		  //十位
 else if(dat/1)m=1;			  //个位
 else if(dat==0)m=1;		  //如果读出来的值就是0 直接用一位表示

 for(i=0;i<m;i++)			  //显示
  GUI_sprintf_char(x+i*8,y,take[5-m+i]+'0',dcolor,bgcolor,0);			  //+'0'表示转换为ACCII码

}




//显示10位数据
void number10(u8 x,u16 y,u16 dat,u16 dcolor,u16 bgcolor)	//坐标 要显示的数据	 
{

   GUI_sprintf_char(x,y,dat/10%10+'0',dcolor,bgcolor,0);
   GUI_sprintf_char(x+8,y,dat%10+'0',dcolor,bgcolor,0); 
}


//显示100位数据
void number100(u8 x,u16 y,u16 dat,u16 dcolor,u16 bgcolor)	//坐标 要显示的数据	 
{
   GUI_sprintf_char(x,y,dat/100%10+'0',dcolor,bgcolor,0);
   GUI_sprintf_char(x+8,y,dat/10%10+'0',dcolor,bgcolor,0);
   GUI_sprintf_char(x+16,y,dat%10+'0',dcolor,bgcolor,0); 
}




//填充矩形
//x0,y0:矩形的左上角坐标
//width,height:矩形的尺寸
//color:颜色
void GUI_fill_box(u16 x0,u16 y0,u16 width,u16 height,u16 color)
{	  							   
	if(width==0||height==0)return;//非法.	 
	GUI_box(x0,y0,x0+width-1,y0+height-1,color);	   	   
}


//画圆角矩形/填充圆角矩形
//x,y,width,height:圆角矩形的位置和尺寸
//r:圆角的半径.
//upcolor:上半部分颜色
//downcolor:下半部分颜色
void GUI_arcrectangle(u16 x,u16 y,u16 width,u16 height,u8 r,u16 upcolor,u16 downcolor)
{
	u16 btnxh=0;
	if(height%2)btnxh=height+1;//基偶数处理
	else btnxh=height;

 		GUI_fill_box(x+r,y,width-2*r,btnxh/2,upcolor);			    //中上
  		GUI_fill_box(x+r,y+btnxh/2,width-2*r,btnxh/2,downcolor);	//中下
		GUI_fill_box(x,y+r,r,btnxh/2-r,upcolor);					//左上
		GUI_fill_box(x,y+btnxh/2,r,btnxh/2-r,downcolor);			//左下
		GUI_fill_box(x+width-r,y+r,r,btnxh/2-r,upcolor);			//右上
		GUI_fill_box(x+width-r,y+btnxh/2,r,btnxh/2-r,downcolor);	//右下

	GUI_arc(x+r,y+r,r,upcolor,1);//左上	
	GUI_arc(x+r,y+btnxh-r-1,r,downcolor,1);//左下	
	GUI_arc(x+width-r-1,y+r,r,upcolor,1);	 //右上
	GUI_arc(x+width-r-1,y+btnxh-r-1,r,downcolor,1);//右下
}







//画宽度为1的竖直线
//(sx,sy)顶点坐标, 
//(sx,ey)终点坐标, color颜色
//返回: 无
void GUI_ver(u8 sx,u16 sy,u16 ey,u16 color)
{ 
	u16 temp,m; 
	m=ey-sy+1;  
	Address_set(sx,sy,sx,ey,sx,sy); 
	
	for(temp=0;temp<m;temp++)		 	
	  Lcd_Write_Data(color);			
}


//画三角形 三角形为纵向
//x y  三角形起始坐标
//tsize 三角形的长度 
//color 颜色
//mode 1 纵向向右  0 纵向向左
void GUI_triangle(u8 x,u16 y,u8 tsize,u16 color,u8 mode)
{
 u8 xm,i=0;
 u16 ym;
 u8 time;
 xm=x;
 ym=y;
 time=tsize/2;
 if(mode==1)
  {
    for(i=0;i<time;i++)
	 {
	   GUI_ver(xm+i,ym+i,(ym+i+tsize)-i*2,color);
	 
	 } 

  }
 if(mode==0)
  {
    for(i=0;i<time;i++)
	 {
	   GUI_ver(xm-i,ym+i,(ym+i+tsize)-i*2,color);	 	 
	 } 
  }

}









//圆矩形触摸按键 
//显示确认 或 取消功能
//尺寸固定为 宽 36 高20 圆半径 3
//x y  初始坐标
//upc  downc  上 下部分颜色
//mode 0  确定   1   取消
void Touch_key(u16 x,u16 y,u16 upc,u16 downc,u8 mode)
{
GUI_arcrectangle(x,y,36,20,3,upc,downc);	
if(mode==0)
GUI_sprintf_hzstr16x(x+2,y+2,"确定",White,1);	//确定按钮框
else
GUI_sprintf_hzstr16x(x+2,y+2,"取消",White,1);	//取消按钮框
}






//巨型对话框
//外框颜色 内框说明背景色 设置区背景颜色
//功能：显示一个赋有说明的对话框 框架
//x y 外框初始坐标 x2 y2 外框结束坐标
//kc 外框颜色 n1 内框上说明字背景色 n2 内框下背景色
//*str  说明字
void Dialog_box(u16 x,u16 y,u16 x2,u16 y2 ,u16 kc,u16 n1,u16 n2,u8 *str)
{

GUI_box(x,y,x2,y2,kc);				//画 外边框
GUI_box(x+1,y+1,x2-1,y+16,n1);		//画 内框上说明字背景色
GUI_box(x+1,y+17,x2-1,y2-1,n2);		//画 内框下背景色
GUI_sprintf_hzstr16x(x+1,y+1,(u8*)str,White,1);	  //说明字

}











//截取指定区域的图片到SRAM中 同时可以恢复截图区的图片
//x1 y1  区域起始位置  x2 y2 区域结束位置
//mode  0：截图   1：将截图恢复
//说明：此函数涉及到申请内存  在截图的时候申请了内存
//      截图后一定要记得恢复截图  这样才能释放内存  否则将会导致内存泄漏 慎记！！ 
//函数涉及到内存的申请 截图的大小就决定内存申请的空间  外部内存空间大小有限 注意截图的尺寸
u16  *readcolor;		 //定义截图存储SRAM指针
void Read_AreaColor(u16 x1,u16 y1,u16 x2,u16 y2,u8 mode)
{
 u16 sx,sy,color,i,j,cr=0,end;

 sx=x2-x1+1;				     //计算截图区域的长宽
 sy=y2-y1+1;
 end=(u16)sx*sy*2;
 if(mode==0)					 //截图模式
 {
  readcolor=mymalloc(SRAMIN,end);    //申请内存空间 根据截图的面积 sx*sy为截图点数 一个颜色点2个字节 所以再*2	


  for(j=0;j<sy;j++)				 //完成整个截图区域数据的存储
  for(i=0;i<sx;i++)
  {
   color=LCD_readpoint(x1+i,y1+j);

   readcolor[cr]=color;			 //将读出的颜色值存入内存中
   cr++;						 //申请的内存指针地址移动

  }

 }
 if(mode==1)					 //恢复截图
 {
  Address_set(x1,y1,x2,y2,x1,y1);		 //设置恢复区域
  for(j=0;j<sx*sy;j++)			 //从SRAM中读取截图存入的颜色值 并显示
  {

  color=readcolor[j];			 //将SRAM中的颜色读出

  Lcd_Write_Data(color);
  }
  //mem_init();
  myfree(SRAMIN,readcolor);			 //释放截图时申请的内存空间	   防止内存泄漏
 }
}







//截取指定区域的图片到SRAM中 同时可以恢复截图区的图片
//二号截取 和上面的配合可以分别截取两个图片
//mode: 0 截取当前区域颜色数据到内存中
//      1 将截取到内存中的颜色数据显示到指定区域
u16   *readcolor2;		 //定义截图存储SRAM指针
void Read_AreaColor2(u16 x1,u16 y1,u16 x2,u16 y2,u8 mode)
{
 u16 sx,sy,color,i,j,cr=0,end;

 sx=x2-x1+1;				     //计算截图区域的长宽
 sy=y2-y1+1;
 end=(u16)sx*sy*2;
 if(mode==0)					 //截图模式
 {
  readcolor2=mymalloc(SRAMIN,end);    //申请内存空间 根据截图的面积 sx*sy为截图点数 一个颜色点2个字节 所以再*2	


  for(j=0;j<sy;j++)				 //完成整个截图区域数据的存储
  for(i=0;i<sx;i++)
  {
   color=LCD_readpoint(x1+i,y1+j);
   readcolor2[cr]=color;			 //将读出的颜色值存入内存中
   cr++;						 //申请的内存指针地址移动
  }

 }
 if(mode==1)					 //恢复截图
 {
  Address_set(x1,y1,x2,y2,x1,y1);		 //设置恢复区域
  for(j=0;j<sx*sy;j++)			 //从SRAM中读取截图存入的颜色值 并显示
  {
  color=readcolor2[j];			 //将SRAM中的颜色读出
  Lcd_Write_Data(color);
  }
  myfree(SRAMIN,readcolor2);			 //释放截图时申请的内存空间	   防止内存泄漏
 }
}












//进入函数时 上下条背景色
//m：0 不显示上条背景色  1 显示上条背景色
void tian(u8 m)						  //下填充 统一函数
{
   if(m!=2)
   {
   GUI_box(0,300,239,301,Black);	  //下填充色
   GUI_box(0,302,239,319,Red);
   GUI_sprintf_hzstr16x(207,303,"返回",White,1);
   }
   if(m==1)
   {
   	GUI_box(0,0,239,8,Blue2);		//画功能显示背景
    GUI_box(0,9,239,17,Blue);
   }
   if(m==2)
   {
    GUI_sprintf_hzstr16x(207,303,"返回",Black,1);
    delay_ms(200);
   	GUI_sprintf_hzstr16x(207,303,"返回",White,1);
   }
}



////返回字体动态效果
////主要应用于进入应用后 退出时点返回选项 返回的动态提示效果
////返回字体的颜色取决于 背景填充色
//void Return(void)
//{
//   GUI_box(207,302,239,319,Blue2);
//   GUI_sprintf_hzstr16x(208,304,"返回",White,Blue2);
//   delay_ms(50);
//}











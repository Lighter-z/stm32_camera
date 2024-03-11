#ifndef GUI_H
#define GUI_H
#include "mcusys.h"

/*GUI函数*/
void GUI_Clear(u16 color);	//清屏

void GUI_Point(u8 x, u16 y, u16 color);//画点

void GUI_line(u16 x1,u16 y1,u16 x2,u16 y2,u16 color)  ;//画直线

void GUI_sprintf_char(u16 x,u16 y,u8 value,u16 dcolor,u16 bgcolor,u8 mode); //显示英文或数字字符

void GUI_box(u16 sx,u16 sy,u16 ex,u16 ey,u16 color);//画实心矩形

void GUI_box10x10(u16 sx,u16 sy,u16 color);   //画一个10x10的方块

void GUI_sprintf_hzstr16x(u16 x1,u16 y1,u8 *str,u16 dcolor,u16 bgcolor);//显示汉字及字符  纵向显示

void GUI_sprintf_hzstr16h(u8 x1,u16 y1,u8 *str,u16 dcolor,u16 bgcolor);	//显示汉字及字符  横向显示


void number10(u8 x,u16 y,u16 dat,u16 dcolor,u16 bgcolor); //只显示10位数据
void number100(u8 x,u16 y,u16 dat,u16 dcolor,u16 bgcolor);//显示100位数据

void number(u8 x,u16 y,u32 dat,u16 dcolor,u16 bgcolor);   //显示最大5位的数据  如果非5位状态 其他位不显示 只显示有效值

void GUI_triangle(u8 x,u16 y,u8 tsize,u16 color,u8 mode);  //三角形

u8 PutGB1616(u16 x, u16  y, u8 Disp_char[2], u16 fColor,u16 bColor,u8 mode);//flash汉字显示


void GUI_arcrectangle(u16 x,u16 y,u16 width,u16 height,u8 r,u16 upcolor,u16 downcolor);  //画圆角巨型

void Touch_key(u16 x,u16 y,u16 upc,u16 downc,u8 mode);//圆矩形触摸按键 
void Dialog_box(u16 x,u16 y,u16 x2,u16 y2 ,u16 kc,u16 n1,u16 n2,u8 *str);//巨型对话框


void GUI_sprintf_ZMHZs(u8 x1,u16 y1,u8 *str,u16 dcolor,u16 bgcolor,u8 mode);//显示32x32 汉字及字母功能 

void Read_AreaColor(u16 x1,u16 y1,u16 x2,u16 y2,u8 mode);//截取指定区域的图片到SRAM中 同时可以恢复截图区的图片
void Read_AreaColor2(u16 x1,u16 y1,u16 x2,u16 y2,u8 mode);//截取工具 2号


void tian(u8 m)	;   //填充条
//void Return(void);	 //返回字体动态效果

void GUI_arc(u16 rx,u16 ry,u16 r,u16 color,u8 mode);//在指定位置画一个指定大小的圆



/*定义常用颜色码*/
#define Red      0xf800	//红
#define Yellow   0xffe0	//黄
#define Green    0x07e0	//绿
#define Cyan     0x07ff	//青
#define Blue     0x001f	//蓝
#define Purple   0xf81f	//紫
#define Black    0x0000	//黑
#define White    0xffff	//白
#define Gray     0x7bef	//灰
#define Blue1    0xa5ff //淡蓝
#define Blue2	 0x7cdf
#define Purple1  0x8a9e //淡紫
#define Green1   0x0410 //墨绿
#define Green2   0x2616
#define Blue3	 0x751E
#define Purple2  0xcd9e //淡紫
#define Red2	 0XF260	//淡红
#define Red3	 0X8000	//棕红
#define window   0XDED7
#define Jacinth  0XF9C0//橘红色
#endif


















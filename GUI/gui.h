#ifndef GUI_H
#define GUI_H
#include "mcusys.h"

/*GUI����*/
void GUI_Clear(u16 color);	//����

void GUI_Point(u8 x, u16 y, u16 color);//����

void GUI_line(u16 x1,u16 y1,u16 x2,u16 y2,u16 color)  ;//��ֱ��

void GUI_sprintf_char(u16 x,u16 y,u8 value,u16 dcolor,u16 bgcolor,u8 mode); //��ʾӢ�Ļ������ַ�

void GUI_box(u16 sx,u16 sy,u16 ex,u16 ey,u16 color);//��ʵ�ľ���

void GUI_box10x10(u16 sx,u16 sy,u16 color);   //��һ��10x10�ķ���

void GUI_sprintf_hzstr16x(u16 x1,u16 y1,u8 *str,u16 dcolor,u16 bgcolor);//��ʾ���ּ��ַ�  ������ʾ

void GUI_sprintf_hzstr16h(u8 x1,u16 y1,u8 *str,u16 dcolor,u16 bgcolor);	//��ʾ���ּ��ַ�  ������ʾ


void number10(u8 x,u16 y,u16 dat,u16 dcolor,u16 bgcolor); //ֻ��ʾ10λ����
void number100(u8 x,u16 y,u16 dat,u16 dcolor,u16 bgcolor);//��ʾ100λ����

void number(u8 x,u16 y,u32 dat,u16 dcolor,u16 bgcolor);   //��ʾ���5λ������  �����5λ״̬ ����λ����ʾ ֻ��ʾ��Чֵ

void GUI_triangle(u8 x,u16 y,u8 tsize,u16 color,u8 mode);  //������

u8 PutGB1616(u16 x, u16  y, u8 Disp_char[2], u16 fColor,u16 bColor,u8 mode);//flash������ʾ


void GUI_arcrectangle(u16 x,u16 y,u16 width,u16 height,u8 r,u16 upcolor,u16 downcolor);  //��Բ�Ǿ���

void Touch_key(u16 x,u16 y,u16 upc,u16 downc,u8 mode);//Բ���δ������� 
void Dialog_box(u16 x,u16 y,u16 x2,u16 y2 ,u16 kc,u16 n1,u16 n2,u8 *str);//���ͶԻ���


void GUI_sprintf_ZMHZs(u8 x1,u16 y1,u8 *str,u16 dcolor,u16 bgcolor,u8 mode);//��ʾ32x32 ���ּ���ĸ���� 

void Read_AreaColor(u16 x1,u16 y1,u16 x2,u16 y2,u8 mode);//��ȡָ�������ͼƬ��SRAM�� ͬʱ���Իָ���ͼ����ͼƬ
void Read_AreaColor2(u16 x1,u16 y1,u16 x2,u16 y2,u8 mode);//��ȡ���� 2��


void tian(u8 m)	;   //�����
//void Return(void);	 //�������嶯̬Ч��

void GUI_arc(u16 rx,u16 ry,u16 r,u16 color,u8 mode);//��ָ��λ�û�һ��ָ����С��Բ



/*���峣����ɫ��*/
#define Red      0xf800	//��
#define Yellow   0xffe0	//��
#define Green    0x07e0	//��
#define Cyan     0x07ff	//��
#define Blue     0x001f	//��
#define Purple   0xf81f	//��
#define Black    0x0000	//��
#define White    0xffff	//��
#define Gray     0x7bef	//��
#define Blue1    0xa5ff //����
#define Blue2	 0x7cdf
#define Purple1  0x8a9e //����
#define Green1   0x0410 //ī��
#define Green2   0x2616
#define Blue3	 0x751E
#define Purple2  0xcd9e //����
#define Red2	 0XF260	//����
#define Red3	 0X8000	//�غ�
#define window   0XDED7
#define Jacinth  0XF9C0//�ٺ�ɫ
#endif


















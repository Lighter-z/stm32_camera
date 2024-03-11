
#ifndef __TFT_H
#define __TFT_H


#define TFT_RS 	PCout(4)
#define TFT_WR  PCout(2)	
#define TFT_RD 	PCout(0)
#define TFT_CS 	PEout(0)		  
#define TFT_RST PEout(4)


//u16 lcd_id;
//void TFT_CS(unsigned int d);

void Lcd_Init(void);


void Lcd_Write_Data(u16 data);								 //д16λ����
void Address_set(u16 x1,u16 y1,u16 x2,u16 y2,u16 xq,u16 yq);    //������ʾ��Χ


void Lcd_WR_Reg(u16 Data);   //��������
void Lcd_WriteReg(u16 com,u16 val); //д�Ĵ���

u16 LCD_readpoint(u16 x,u16 y);     //��ȡ����ɫ   ���ô˺��� Ҫ�ǵ�����ioΪ����


void LCD_scan(u8 mode);//9341��Ļɨ��ģʽ����

//��ɫ����
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


//��Ļ�ߴ�
#define TFT_XMAX       239
#define TFT_YMAX       319




#endif










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

//u32  unicode_sector, 		   //UNI��������
//       gbk12_sector;		   //GBK��������

//��ȡ TF����  GBK������  �� UIN�� ������  Ϊֱ����TFΪ�ֿ��ȡ����ʾ��׼��  ���ֱ�ӵ���FLASH���ֱ�����δ˺���

////��ȡ ���ֿ⿪ʼ����
////hzk16_sector    GBK ����16X16�ֿ� ��ʼ����
////unicode_sector  UNICODE�� ��ʼ����  ��ҪӦ���ڳ��ļ���
////����ֵ 0  �ɹ�  ����ʧ��
//u8 get_font_sector(void)
//{
//FRESULT res;
//
//res=pf_open("SYSTEM/UNI.TXT");  //�ҵ� SYSTEM�µ� UNI.txt  unicode��
//  if(res == FR_OK)				//Ѱ�ҳɹ�  ��ȡ�ļ��Ŀ�ʼ�� ������������
//    unicode_sector=(fatfs.org_clust-fatfs.dirbase)*fatfs.csize+fatfs.database;
//	              //unicode_sector=����ǰ�ļ���-Ŀ¼���ڴأ�*ÿ��������+ ��Ŀ¼������         
//  else return 1;
//res=pf_open("SYSTEM/GBK12.DZK");  //�ҵ� SYSTEM�µ� GBK12.DZK���ֿ�
//  if(res == FR_OK)				//Ѱ�ҳɹ�  ��ȡ�ļ��Ŀ�ʼ�� ������������
//    gbk12_sector=(fatfs.org_clust-fatfs.dirbase)*fatfs.csize+fatfs.database;
//	              //����ͬ��
//				  //  gbk12_sector= fatfs.dsect;        
//  else return 2;
//
//return  0;
//}




//��ָ��λ����ʾ �ֿ���� GBK����   �����屳����ɫ ֻ��0x0001 ��ô��ʱ����ʾ������ɫ ������ɫΪĬ����ɫ
//֧�ֺ���������ʾѡ���� mode  ����ֻ��������ʾ��ʱ���֧�� ������ɫ������Ļ�����е���ɫ
// x  y  ��ʾ�ľ�������
// Disp_char[2] ��Ҫ��ʾ��GBK��
// fColor  	 bColor    ���� ������ɫ 
// mode    0 ������ʾ  1 ������ʾ  ����ҪҪ���ɨ�跽ʽ��
//����0 ��ʾ�ɹ�  ����1 ���ֿ��� û�ж�Ӧ��GBK��
u8 PutGB1616(u16 x, u16  y, u8 Disp_char[2], u16 fColor,u16 bColor,u8 mode)
{

    u8 qh,ql;
	u8 i,j;
    u8  tmp_Char_Model[32];	//GBK12 �����ֿ���  ��ȡһ��16X16���� ��32���ֽ�

    qh=Disp_char[0];
	ql=Disp_char[1];

	if(qh<0x81||ql<0x40||ql==0xff||qh==0xff)//�� ���ú���
	return 1; //�������� 
	          
	if(ql<0x7f)ql-=0x40;//���ݵ��ֽ����������� ���ߵ��ֽ�ת�Ƶ� ָ���ֽ�λ��
	else ql-=0x41;
	qh-=0x81; 	 


    // ����ģ 					   //  �õ���ֵ    	�õ�λ��ֵ			 һ���� 32���ֽ�
    //SD_read_Byte((gbk12_sector<<9) + ((u32)qh*190 + (u32)ql)*32,tmp_Char_Model,32);		//TF�� ��ȡģʽ	

	/*
	 TF��ģʽ
	 gbk12_sector Ϊ�ֿ���������   gbk12_sector<<9�൱��X512  תΪ�ֽ����� ���ֿ������ֽڿ�ʼλ��
	 ÿ��GBK ����2 ���ֽ���ɣ�

	 FLASH ģʽ 
	 flash ���Ѿ���TF����� �ֿ������������� flash�� ֻҪȷ����ַ���� �����ַ���Ǵ���ʱ�ĵ�һ����ַ 
	 �����Ǵ� ��322��������ʼ�����  flash��һ��������4k���ֽڿռ� �� 4096���ֽ� �����׵�ַ�� 322*4096

	 ��һ���ֽ�Ϊ0X81~0XFE��
	 �ڶ����ֽڷ�Ϊ�����֣� һ��0X40~0X7E������0X80~0XFE ��
	 ��һ���ֽڴ���Ϊ������ôGBK �����ܹ���126 ������0XFE-0X81+1 ���� 
     ÿ�������� 190 �����֣�0XFE-0X80+0X7E-0X40+2 �����ܹ����� 126*190=23940 ������
	 ������������ 0X8140  ��ʼ   ��������Ҫ�жϵ��ֽ���������  Ȼ��õ������ֽ�λ��
	 ((u32)qh*190 + (u32)ql)*32 ÿ���� 190����   һ����32���ֽ�
	*/

	SPI_Flash_Read(tmp_Char_Model,(u32)322*4096+((u32)qh*190 + (u32)ql)*32,32);	   //FLASH��ȡģʽ


   if(bColor!=0x0001)							//֧�ֱ�����ɫ����
    {														 //ע�� ����������һ��ʾ���� ǰ����ɨ��Ҫ��Ӧ
	    if(mode==0)Address_set(x,y,  x+16-1, y+16-1,x,y);	 // ����Ϊ������ʾ
		if(mode==1)Address_set(x,y,  x+16-1, y+16-1,x+15, y);// ����Ϊ������ʾ
	    //��ʾ���� 
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
   else										//��֧�ֱ�����ɫ����  ֻ��ʾ������ɫ ������ɫ��ˢ��ʱһ��
	{
	    qh=x;								//����xֵ��qh
		for(i=0;i<32;i++)
		{
	        for(j=0;j<8;j++) 
	        {
			    Address_set(x,y,x,y,x,y);	//��ʾ����Ҫһ����һ������ʾ
	            if(tmp_Char_Model[i] & 0x80) 
	                   {
	                	    Lcd_Write_Data(fColor);		//��ʾ���ֵ���ɫ
							x++;						//x�Լ�
							if((x-qh)==16){x=qh;y++;}	//��һ�е� ��xֵ
	                	}
	            else 
	                    {								//û�к��ֵ���ɫҪ��ʾ ������ ��������ʾʹ����Ļ�Դ�
	                	    x++;						
							if((x-qh)==16){x=qh;y++;}	//��һ�е� ��xֵ
	                	}
	            tmp_Char_Model[i] <<= 1;				//��һ���ֽ�
	        } 		
		}
	
	}

return 0;//�ɹ�
   
}

//����
//color�Ǳ�����ɫ��
//˵����ʹ�ñ�����ɫ���TFTģ����Ļ��ȫ����ʾ���ݡ�
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



//����
//(x��y)�ǵ������
//color �ǵ����ɫ��
//˵������ָ������ɫ��ָ��������λ���ϻ���һ���㡣
void GUI_Point(u8 x, u16 y, u16 color)
{  
	Address_set(x,y,x,y,x,y);
	Lcd_Write_Data(color);
}

   

//��ֱ�ߣ����Ի����ⷽ��ֱ�ߣ��������ߡ����ߡ�б�ߣ���
//(x1,y1)���, (x2,y2)�յ�, color��ɫ��
//˵������ָ������ɫ��ָ��������仭��һ��ֱ�ߡ�
void GUI_line(u16 x1,u16 y1,u16 x2,u16 y2,u16 color)  
{  
    u16 t;  
    int xerr=0,yerr=0,delta_x,delta_y,distance;  
    int incx,incy;  
    u16 row,col;  
    delta_x = x2-x1;//������������  
    delta_y = y2-y1;  
    col=x1;  
    row=y1;  
    if(delta_x>0) incx=1;//���õ�������  
    else   
    {  
        if(delta_x==0) incx=0;//��ֱ��  
        else {incx=-1;delta_x=-delta_x;}  
    }  
    if(delta_y>0)incy=1;  
    else  
    {  
        if(delta_y==0) incy=0;//ˮƽ��  
        else {incy=-1;delta_y=-delta_y;}  
    }  
    if(delta_x>delta_y) distance=delta_x;//ѡȡ��������������  
    else distance=delta_y;  

    for(t=0;t<=distance+1;t++)  
	{                                     //�������  
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




//��8��(Bresenham�㷨)		  
//(rx,ry,a,b):����
//color:��ɫ
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
//��ָ��λ�û�һ��ָ����С��Բ
//(rx,ry):Բ��
//r    :�뾶
//color:��ɫ
//mode :0,�����;1,���
void GUI_arc(u16 rx,u16 ry,u16 r,u16 color,u8 mode)
{
	int a,b,c;
	int di;	  
	a=0;b=r;	  
	di=3-(r<<1);	//�ж��¸���λ�õı�־
	while(a<=b)
	{
		if(mode)
		for(c=a;c<=b;c++)
		gui_circle8(rx,ry,a,c,color);//��ʵ��Բ
 		else gui_circle8(rx,ry,a,b,color);			//������Բ
		a++;
		//ʹ��Bresenham�㷨��Բ     
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 							  
	}
}

//��ʵ�ľ���
//(sx,sy)���ϽǶ�������, 
//(ex,ey)���½Ƕ�������, color��ɫ
//����: ��
//˵������ָ��λ���ϻ���ʵ�ľ��Ρ�
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



//��һ��10x10�ķ���
//sx sy  �������� 
//color  ������ɫ
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


////����ɫ�ݱ����  �����Ǻ��� ������
////(sx,sy)���ϽǶ�������, 
////(ex,ey)���½Ƕ�������, color��ɫ
//// mode ģʽ  mode=1  ������ɫ�仯 mode=2 ������ɫ�仯
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













//��ʾӢ�Ļ������ַ�
//����֧�� �ַ�������ͺ�����ʾ ��Ҫ���ɨ�跽ʽ��
//         ֧�ֺ��Ա�����ɫ��ʾ ������û�п��˹��ܣ�
//x  y  Ҫ��ʾ�ַ��ĳ�ʼ����
//value ��ʾ�ַ�����
//dcolor ��ʾ���ַ���ɫ  bgcolor��ʾ�ַ��ı�����ɫ
//                       bgcolor ֻ��1 ˵��������ɫʹ�� ��ǰ��Ļ��ɫ��ֻ������������ʾ��
//mode   0 �ַ�������ʾ  1 �ַ�������ʾ
void GUI_sprintf_char(u16 x,u16 y,u8 value,u16 dcolor,u16 bgcolor,u8 mode)	
{  
	u8 i,j;
	u8 *temp=(u8 *)zifu816;    //temp��*temp�ĵ�ַ  ����temp����zifu���׵�ַ 
    u8 ma;  
	                    
	temp+=(value-32)*16;   //ȷ��Ҫ��ʾ��ֵ
				           //��ascii��  ǰ32��asciiû�д���zifu���� ����Ҫ��32
	                       //ÿ���ַ���16���ֽ���ʾ �����ڳ���16  ���Ƕ�Ӧ����ʾλ���׵�ַ

    if(bgcolor!=1)		   //���Լ��趨�ı���ɫ
	 {											   //ע�� ����������һ��ʾ���� ǰ����ɨ��Ҫ��Ӧ
	  if(mode==0)Address_set(x,y,x+7,y+15,x,y);    //��������		   
	  if(mode==1)Address_set(x,y,x+15,y+7,x+15,y); //��������   
		for(j=0;j<16;j++)
		{
			for(i=0;i<8;i++)		    //�Ⱥ�ɨ
			{ 		     
			 	if((*temp&(1<<(7-i)))!=0)		   //��1 ���� Ȼ���Ӧλ�������� 				
				  Lcd_Write_Data(dcolor);		   //��ʾ�ַ���ɫ
				 
				else				
				  Lcd_Write_Data(bgcolor);		   //��ʾ������ɫ		
			}
			temp++;								   //��һ�ֽ�
		 }
	  }
	else				  //����Ļ�����е���ɫ������ɫ ��ֻ֧��������ʾ��
	 {	
	    ma=x;			  //��ֵx�ĳ�ʼֵ��ma 
		for(j=0;j<16;j++)
		{
			for(i=0;i<8;i++)		    //�Ⱥ�ɨ
			{ 
			    Address_set(x,y,x,y,x,y);
			 	if((*temp&(1<<(7-i)))!=0)	//��1 ���� Ȼ���Ӧλ�������� 
				{
				   Lcd_Write_Data(dcolor);	//��ʾ�ַ���ɫ
				   x++;
				   if((x-ma)==8){x=ma;y++;}	//�ж��Ƿ�һ��
				} 
				else
				{
				   x++;						//û��Ҫ��ʾ������ x�Լ�����
				   if((x-ma)==8){x=ma;y++;}	//�ж��Ƿ�һ��
				} 
			}
			temp++;							//��һ�ֽ�
		}		 			  
	 } 

}



//��ʾ���ּ��ַ�  ������ʾ
//x1 y1 ��ʾ�ĳ�ʼλ��
//*str Ҫ��ʾ������
//dcolor ��ʾ���� �ַ�����ɫ
//bgcolor ��ʾ���� �ַ��ı�����ɫ  ��bgcolor=1�� ��ô������ɫΪ��Ļ������ɫ
void GUI_sprintf_hzstr16x(u16 x1,u16 y1,u8 *str,u16 dcolor,u16 bgcolor)	  
{  
	 u8 l=0;
	while(*str)
	{	
	  if(*str<0x80)		  //С��128   ascii ����������
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





//��ʾ���ּ��ַ�  ������ʾ
//x1 y1 ��ʾ�ĳ�ʼλ��
//*str Ҫ��ʾ������
//dcolor ��ʾ���� �ַ�����ɫ
//bgcolor ��ʾ���� �ַ��ı�����ɫ  ��֧�ֱ�����Ļ����ɫ
void GUI_sprintf_hzstr16h(u8 x1,u16 y1,u8 *str,u16 dcolor,u16 bgcolor)	  
{  
	 u8 l=0;
	while(*str)
	{	
	  if(*str<0x80)		  //С��128   ascii ����������
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


//��ʾ32x32��������
//(x��y) ����ʾ���ݵ����Ͻ����꣬c[2]��ʾ���ֵ������ֽ�
//dcolor ���ֵ���ɫ   bgcolor ������ɫ
//˵������ָ��λ������ʾһ��32x32�ĺ���
void GUI_sprintf_hz3232(u8 x, u16  y, u8 c[2], u16 dcolor,u16 bgcolor)
{
	u8 i,j,k;

	Address_set(x,y,x+32-1, y+32-1,x,y);     //������ʾ��Χ

	for (k=0;k<40;k++) 	  //�Խ����ֿ⣨k���ֵ�ͺ��ֿ��к��ָ����йأ���ѭ����ѯ����
	{ 
	  if ((codeHZ_32[k].Index[0]==c[0])&&(codeHZ_32[k].Index[1]==c[1]))		//Ѱ��Ҫ��ʾ�ĺ���
	  { 
    	for(i=0;i<128;i++)													//1������Ҫ128���ֽڵ�����ʾ
		{
		  u8 m=codeHZ_32[k].Msk[i];											//һ���ֽ�һ���ֽڵ���ʾ
		  for(j=0;j<8;j++) 
		  {
			if((m&0x80)==0x80) 
			  Lcd_Write_Data(dcolor);	//��ʾ������ɫ
			else 
			  Lcd_Write_Data(bgcolor);	//��ʾ������ɫ
			m<<=1;
		  }    
		}
	  }  
	}	
}



//��ʾ16x32��ĸ
//x��y����ʾ���ݵ����Ͻ����꣬
//c ��ĸֵ
//dcolor ���ֵ���ɫ   
//dcolor ��ʾ���ַ���ɫ  bgcolor��ʾ�ַ��ı�����ɫ
//                       bgcolor ֻ��1 ˵��������ɫʹ�� ��ǰ��Ļ��ɫ
//mode ��ĸ���� 0 ϸ��  1 ����
//˵������ָ��λ������ʾӢ����ĸ
void GUI_sprintf_ZM3216(u8 x, u16  y, u8 c, u16 dcolor,u16 bgcolor,u8 mode)
{
	
//	struct  typFNT_ZM32 zm;

	u8 i,j,k,ma,m;
	u8 zm[64];
//	u8 *zm;
//  	zm=codeZM_32[];

	for (k=0;k<19;k++) 		  //�Խ�16x32��ĸ�⣨k���ֵ����ĸ���к��ָ����йأ���ѭ����ѯ����
	{ 
	   
	  if(mode==1&&codeZM_32c[k].Index[0]==c)       //Ѱ��Ҫ��ʾ�Ĵ�����ĸ
	  { 

		for(i=0;i<64;i++)
		zm[i]=codeZM_32c[k].Msk[i];

		break;
	  }
	  else if(mode==0&&codeZM_32x[k].Index[0]==c)  //Ѱ��Ҫ��ʾ��ϸ����ĸ
	  {
	    for(i=0;i<64;i++)
	  	zm[i]=codeZM_32x[k].Msk[i];
		break;
	  }
	}

	   if(bgcolor!=1)							   //���Լ��趨�ı���ɫ
		{
		    Address_set(x,y,x+16-1, y+32-1,x,y);       //������ʾ��Χ
	    	for(i=0;i<64;i++) 						   //1����ĸҪ64���ֽڵ�����ʾ
			{
//			  m=codeZM_32[k].Msk[i];				   //һ���ֽ�һ���ֽڵ���ʾ
			  m=zm[i];
			  for(j=0;j<8;j++) 
			  {
				if(m&0x80) 			
				  Lcd_Write_Data(dcolor);	   //��ʾ��ĸ��ɫ			
				else 
				  Lcd_Write_Data(bgcolor);     //��ʾ������ɫ
				m<<=1;
			  }    
			}

		}
	   else									   //����Ļ�����е���ɫ������ɫ 
		{
		    ma=x;							   //��ֵx�ĳ�ʼֵ��ma
	    	for(i=0;i<64;i++) 				   //1����ĸҪ64���ֽڵ�����ʾ
			{
//			  m=codeZM_32[k].Msk[i];		   //һ���ֽ�һ���ֽڵ���ʾ
			  m=zm[i];
			  for(j=0;j<8;j++) 
			  {
			//	if(m&0x80) 
				if((m&(1<<(7-j)))!=0)
				 {
				   Address_set(x,y,x,y,x,y);	   //������ʾ��Χ
				   Lcd_Write_Data(dcolor);	   //��ʾ��ĸ��ɫ
				   x++;
				   if((x-ma)==16){x=ma; y++;}
				 }
				else 
				 {
				  x++;						   //û��Ҫ��ʾ������ x�Լ�����
				  if((x-ma)==16){x=ma; y++;}		   //�ж��Ƿ�һ��
				 }
			//	 m<<=1;
			  }    
			}
		 }



}





//��ʾ32x32���� ��32x16 ��ĸ��
//x1��y1����ʾ���ݵ����Ͻ����꣬
//dcolor ���ֵ���ɫ   bgcolor ������ɫ
//*str Ҫ��ʾ������
//mode ��ĸ���� 0 ϸ��  1 ����
void GUI_sprintf_ZMHZs(u8 x1,u16 y1,u8 *str,u16 dcolor,u16 bgcolor,u8 mode)	 
{  
   u8 l=0;	  
   while(*str)
   {
	 if(*str<0x80)		  //ascii������	        
	 {
	   GUI_sprintf_ZM3216(x1+l*8,y1,*str,dcolor, bgcolor,mode);
	   str+=1;l+=2;
	 }
	 else				  //��ʾ����
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






//��ʾ���5λ������  �����5λ״̬ ����λ����ʾ ֻ��ʾ��Чֵ
//֧�ֹ��� ���������ɫ Ϊ1 ��ô����ʾ������ɫ ������ɫΪд��ǰĬ��ֵ
// x y ��ʾ��ֵ����  
//dat ��ֵ
//dcolor  ��ʾ��ֵ��ɫ  bgcolor������ɫ
void number(u8 x,u16 y,u32 dat,u16 dcolor,u16 bgcolor)				//���� Ҫ��ʾ������	   ���ٳ���������
{
 u8 take[5];
 u8 i,m;

 take[0]=dat/10000%10;
 take[1]=dat/1000%10;
 take[2]=dat/100%10;
 take[3]=dat/10%10;
 take[4]=dat%10;

							  //��ʾλ��
 if(dat/10000)m=5;			  //��λ
 else if(dat/1000)m=4;		  //ǧλ
 else if(dat/100)m=3;		  //��λ
 else if(dat/10)m=2;		  //ʮλ
 else if(dat/1)m=1;			  //��λ
 else if(dat==0)m=1;		  //�����������ֵ����0 ֱ����һλ��ʾ

 for(i=0;i<m;i++)			  //��ʾ
  GUI_sprintf_char(x+i*8,y,take[5-m+i]+'0',dcolor,bgcolor,0);			  //+'0'��ʾת��ΪACCII��

}




//��ʾ10λ����
void number10(u8 x,u16 y,u16 dat,u16 dcolor,u16 bgcolor)	//���� Ҫ��ʾ������	 
{

   GUI_sprintf_char(x,y,dat/10%10+'0',dcolor,bgcolor,0);
   GUI_sprintf_char(x+8,y,dat%10+'0',dcolor,bgcolor,0); 
}


//��ʾ100λ����
void number100(u8 x,u16 y,u16 dat,u16 dcolor,u16 bgcolor)	//���� Ҫ��ʾ������	 
{
   GUI_sprintf_char(x,y,dat/100%10+'0',dcolor,bgcolor,0);
   GUI_sprintf_char(x+8,y,dat/10%10+'0',dcolor,bgcolor,0);
   GUI_sprintf_char(x+16,y,dat%10+'0',dcolor,bgcolor,0); 
}




//������
//x0,y0:���ε����Ͻ�����
//width,height:���εĳߴ�
//color:��ɫ
void GUI_fill_box(u16 x0,u16 y0,u16 width,u16 height,u16 color)
{	  							   
	if(width==0||height==0)return;//�Ƿ�.	 
	GUI_box(x0,y0,x0+width-1,y0+height-1,color);	   	   
}


//��Բ�Ǿ���/���Բ�Ǿ���
//x,y,width,height:Բ�Ǿ��ε�λ�úͳߴ�
//r:Բ�ǵİ뾶.
//upcolor:�ϰ벿����ɫ
//downcolor:�°벿����ɫ
void GUI_arcrectangle(u16 x,u16 y,u16 width,u16 height,u8 r,u16 upcolor,u16 downcolor)
{
	u16 btnxh=0;
	if(height%2)btnxh=height+1;//��ż������
	else btnxh=height;

 		GUI_fill_box(x+r,y,width-2*r,btnxh/2,upcolor);			    //����
  		GUI_fill_box(x+r,y+btnxh/2,width-2*r,btnxh/2,downcolor);	//����
		GUI_fill_box(x,y+r,r,btnxh/2-r,upcolor);					//����
		GUI_fill_box(x,y+btnxh/2,r,btnxh/2-r,downcolor);			//����
		GUI_fill_box(x+width-r,y+r,r,btnxh/2-r,upcolor);			//����
		GUI_fill_box(x+width-r,y+btnxh/2,r,btnxh/2-r,downcolor);	//����

	GUI_arc(x+r,y+r,r,upcolor,1);//����	
	GUI_arc(x+r,y+btnxh-r-1,r,downcolor,1);//����	
	GUI_arc(x+width-r-1,y+r,r,upcolor,1);	 //����
	GUI_arc(x+width-r-1,y+btnxh-r-1,r,downcolor,1);//����
}







//������Ϊ1����ֱ��
//(sx,sy)��������, 
//(sx,ey)�յ�����, color��ɫ
//����: ��
void GUI_ver(u8 sx,u16 sy,u16 ey,u16 color)
{ 
	u16 temp,m; 
	m=ey-sy+1;  
	Address_set(sx,sy,sx,ey,sx,sy); 
	
	for(temp=0;temp<m;temp++)		 	
	  Lcd_Write_Data(color);			
}


//�������� ������Ϊ����
//x y  ��������ʼ����
//tsize �����εĳ��� 
//color ��ɫ
//mode 1 ��������  0 ��������
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









//Բ���δ������� 
//��ʾȷ�� �� ȡ������
//�ߴ�̶�Ϊ �� 36 ��20 Բ�뾶 3
//x y  ��ʼ����
//upc  downc  �� �²�����ɫ
//mode 0  ȷ��   1   ȡ��
void Touch_key(u16 x,u16 y,u16 upc,u16 downc,u8 mode)
{
GUI_arcrectangle(x,y,36,20,3,upc,downc);	
if(mode==0)
GUI_sprintf_hzstr16x(x+2,y+2,"ȷ��",White,1);	//ȷ����ť��
else
GUI_sprintf_hzstr16x(x+2,y+2,"ȡ��",White,1);	//ȡ����ť��
}






//���ͶԻ���
//�����ɫ �ڿ�˵������ɫ ������������ɫ
//���ܣ���ʾһ������˵���ĶԻ��� ���
//x y ����ʼ���� x2 y2 ����������
//kc �����ɫ n1 �ڿ���˵���ֱ���ɫ n2 �ڿ��±���ɫ
//*str  ˵����
void Dialog_box(u16 x,u16 y,u16 x2,u16 y2 ,u16 kc,u16 n1,u16 n2,u8 *str)
{

GUI_box(x,y,x2,y2,kc);				//�� ��߿�
GUI_box(x+1,y+1,x2-1,y+16,n1);		//�� �ڿ���˵���ֱ���ɫ
GUI_box(x+1,y+17,x2-1,y2-1,n2);		//�� �ڿ��±���ɫ
GUI_sprintf_hzstr16x(x+1,y+1,(u8*)str,White,1);	  //˵����

}











//��ȡָ�������ͼƬ��SRAM�� ͬʱ���Իָ���ͼ����ͼƬ
//x1 y1  ������ʼλ��  x2 y2 �������λ��
//mode  0����ͼ   1������ͼ�ָ�
//˵�����˺����漰�������ڴ�  �ڽ�ͼ��ʱ���������ڴ�
//      ��ͼ��һ��Ҫ�ǵûָ���ͼ  ���������ͷ��ڴ�  ���򽫻ᵼ���ڴ�й© ���ǣ��� 
//�����漰���ڴ������ ��ͼ�Ĵ�С�;����ڴ�����Ŀռ�  �ⲿ�ڴ�ռ��С���� ע���ͼ�ĳߴ�
u16  *readcolor;		 //�����ͼ�洢SRAMָ��
void Read_AreaColor(u16 x1,u16 y1,u16 x2,u16 y2,u8 mode)
{
 u16 sx,sy,color,i,j,cr=0,end;

 sx=x2-x1+1;				     //�����ͼ����ĳ���
 sy=y2-y1+1;
 end=(u16)sx*sy*2;
 if(mode==0)					 //��ͼģʽ
 {
  readcolor=mymalloc(SRAMIN,end);    //�����ڴ�ռ� ���ݽ�ͼ����� sx*syΪ��ͼ���� һ����ɫ��2���ֽ� ������*2	


  for(j=0;j<sy;j++)				 //���������ͼ�������ݵĴ洢
  for(i=0;i<sx;i++)
  {
   color=LCD_readpoint(x1+i,y1+j);

   readcolor[cr]=color;			 //����������ɫֵ�����ڴ���
   cr++;						 //������ڴ�ָ���ַ�ƶ�

  }

 }
 if(mode==1)					 //�ָ���ͼ
 {
  Address_set(x1,y1,x2,y2,x1,y1);		 //���ûָ�����
  for(j=0;j<sx*sy;j++)			 //��SRAM�ж�ȡ��ͼ�������ɫֵ ����ʾ
  {

  color=readcolor[j];			 //��SRAM�е���ɫ����

  Lcd_Write_Data(color);
  }
  //mem_init();
  myfree(SRAMIN,readcolor);			 //�ͷŽ�ͼʱ������ڴ�ռ�	   ��ֹ�ڴ�й©
 }
}







//��ȡָ�������ͼƬ��SRAM�� ͬʱ���Իָ���ͼ����ͼƬ
//���Ž�ȡ ���������Ͽ��Էֱ��ȡ����ͼƬ
//mode: 0 ��ȡ��ǰ������ɫ���ݵ��ڴ���
//      1 ����ȡ���ڴ��е���ɫ������ʾ��ָ������
u16   *readcolor2;		 //�����ͼ�洢SRAMָ��
void Read_AreaColor2(u16 x1,u16 y1,u16 x2,u16 y2,u8 mode)
{
 u16 sx,sy,color,i,j,cr=0,end;

 sx=x2-x1+1;				     //�����ͼ����ĳ���
 sy=y2-y1+1;
 end=(u16)sx*sy*2;
 if(mode==0)					 //��ͼģʽ
 {
  readcolor2=mymalloc(SRAMIN,end);    //�����ڴ�ռ� ���ݽ�ͼ����� sx*syΪ��ͼ���� һ����ɫ��2���ֽ� ������*2	


  for(j=0;j<sy;j++)				 //���������ͼ�������ݵĴ洢
  for(i=0;i<sx;i++)
  {
   color=LCD_readpoint(x1+i,y1+j);
   readcolor2[cr]=color;			 //����������ɫֵ�����ڴ���
   cr++;						 //������ڴ�ָ���ַ�ƶ�
  }

 }
 if(mode==1)					 //�ָ���ͼ
 {
  Address_set(x1,y1,x2,y2,x1,y1);		 //���ûָ�����
  for(j=0;j<sx*sy;j++)			 //��SRAM�ж�ȡ��ͼ�������ɫֵ ����ʾ
  {
  color=readcolor2[j];			 //��SRAM�е���ɫ����
  Lcd_Write_Data(color);
  }
  myfree(SRAMIN,readcolor2);			 //�ͷŽ�ͼʱ������ڴ�ռ�	   ��ֹ�ڴ�й©
 }
}












//���뺯��ʱ ����������ɫ
//m��0 ����ʾ��������ɫ  1 ��ʾ��������ɫ
void tian(u8 m)						  //����� ͳһ����
{
   if(m!=2)
   {
   GUI_box(0,300,239,301,Black);	  //�����ɫ
   GUI_box(0,302,239,319,Red);
   GUI_sprintf_hzstr16x(207,303,"����",White,1);
   }
   if(m==1)
   {
   	GUI_box(0,0,239,8,Blue2);		//��������ʾ����
    GUI_box(0,9,239,17,Blue);
   }
   if(m==2)
   {
    GUI_sprintf_hzstr16x(207,303,"����",Black,1);
    delay_ms(200);
   	GUI_sprintf_hzstr16x(207,303,"����",White,1);
   }
}



////�������嶯̬Ч��
////��ҪӦ���ڽ���Ӧ�ú� �˳�ʱ�㷵��ѡ�� ���صĶ�̬��ʾЧ��
////�����������ɫȡ���� �������ɫ
//void Return(void)
//{
//   GUI_box(207,302,239,319,Blue2);
//   GUI_sprintf_hzstr16x(208,304,"����",White,Blue2);
//   delay_ms(50);
//}










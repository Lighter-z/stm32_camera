
#include  "mcusys.h"
#include  "delay.h"				  //��ʱ����
#include  "tft.h"			  //TFT IC�ײ�����
#include  "gui.h"				  //GUI��ʾ���ƺ���
#include  "xpt2046.h"
#include  "spi.h"				  //STC12 Ӳ��SPI�ײ㺯��
#include  "sdio_sdcard.h" 
#include  "led.h"
#include  "pff.h"
#include  "OVroot.h"
#include  "OVbmp.h"
#include  "string.h"				//�ϲ�ָ�뺯��
#include  "malloc.h"
//#include  "touch.h"
#include  "flash.h"
//#include  "eeprom.h"


extern FATFS fatfs;
extern u8 tbuf[512];






//�ļ�ϵͳ��Ŀ¼�˵� ѡ���ֵ���غ���
//�жϴ����� ����Ҫ��ķ�����Ӧ����
//x y �����¼�����ʱ ����ֵ
//����18 ����ֵ������������ ��ЧӦ  ����0-17 �ڴ������÷�Χ
u8 Click_Root(u16 x,u16 y)	   
{
 u8 result=18;		          //13�������ļ���  
							  
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

 else if(x>=200&&x<=239&&y>=1&&y<=40)	 //����
  result=17;
 return result;								 //���ؽ��
}


//��Ŀ¼��������  	  ��ҪӦ�����ļ�ϵͳFATFS
//֧�ֻ������ļ���ͽ�������ͷ���㴥�������ļ���
//���� ��Ӧ�ļ��� λ��ֵ  Ϊ��һ����ȡ ���ļ�����׼��
u8  ROOT_Touch(void)
{

    u8 cp;
	u16 ya=0,yb=0;
	struct TFT_Pointer sp;

	sp=TFT_Cm();	   // ɨ�败����
	if(sp.flag==1)					//�д�������
	 {	
	   cp=Click_Root(sp.x,sp.y);	//�жϴ�������λ�õĶ�Ӧֵ
	   ya=sp.y;		  				//��һ����ȡ y ����ֵ  
		   
	   delay_ms(200);				//��ʱ				 
	   while(xpt2046_isClick==0)        //�жϴ����Ƿ�һֱ������  ˵�����뻮����ҳ״̬
		{
		  sp=TFT_Cm(); 				//��ȡ��������
		  if(sp.flag==1)			//�д�������
		  {
			yb=sp.y;				//�ڶ�����ȡ y ����ֵ	 ������yֵ���бȽ� �жϻ������� 
			if(yb>(ya+5)) return 15;		   //����
			else if(yb<(ya-5)) return 16;	   //����
			else return 18;					   //������ ����
		  }		  
		}
	   return cp; 		//���㴥�� ֱ�ӷ��زɼ�����
	   
	 }

  return 18;
}





//��Ŀ¼��������  	  ��ҪӦ�����ļ�ϵͳFATFS
//֧�ֻ������ļ���ͽ�������ͷ���㴥�������ļ���
//���� ��Ӧ�ļ��� λ��ֵ  Ϊ��һ����ȡ ���ļ�����׼��
u8  ROOT_Touch2(void)
{
  u8 cp=18;
//	u16 xa=0,xb=0;
	struct TFT_Pointer sp;

	sp=TFT_Cm();	   // ɨ�败����
	if(sp.flag==1)					//�д�������
	 {	 
       if(sp.x>=200&&sp.x<=239&&sp.y>=300&&sp.y<=320)	 //����
        cp=17;	 
			 
			 if(sp.x>0&&sp.x<=120&&sp.y>=40&&sp.y<=280) cp=19;
			 if(sp.x>121&&sp.x<=239&&sp.y>=40&&sp.y<=280) cp=20;
			 	 	 
//	   xa=sp.x;		  				//��һ����ȡ y ����ֵ  
//		   
//	   delay_ms(200);				//��ʱ				 
//	   while(xpt2046_isClick==0)        //�жϴ����Ƿ�һֱ������  ˵�����뻮����ҳ״̬
//		{
//		  sp=TFT_Cm(); 				//��ȡ��������
//		  if(sp.flag==1)			//�д�������
//		  {
//			xb=sp.x;				//�ڶ�����ȡ y ����ֵ	 ������yֵ���бȽ� �жϻ������� 
//			if(xb>(xa+10)) return 19;		   //����
//			else if(xb<(xa-10)) return 20;	   //����
//			else return 18;					   //������ ����
//		  }		  
//		}
	      return cp; 		//���㴥�� ֱ�ӷ��زɼ�����	   
	 }

  return 18;
}




u8  *short_name[50];	  //���ļ��� �洢�ռ�  ��ҪΪ�򿪸����ļ��� �ṩ·��
//��һ���ļ��� ���õ��������е��ļ������ļ�����
// *path  Ҫ�򿪵��ļ���·��
//���� ɨ�赽���ļ�����  �������0  ʧ��
u8 scan_filename(const char *path)
{
 DIR dir;				 //Դ�ļ�
 FILINFO finfo;		     //�ļ���Ϣ
 u8 list,i;//lf=0



pf_opendir(&dir,path);	   //��ȡָ���ļ����µ� DIR����




 for(i=0;i<50;i++)		   //��ÿһ����ָ������ ����32�ֽ��ڴ�ռ�  Ϊ�洢�ļ�����׼�� 
 {
  short_name[i]=mymalloc(SRAMIN,32);
 }  





for(list=0;list<50;list++)			 //������32���ļ���
  {
    pf_readdir(&dir,&finfo);				  //��ȡ���ļ����µ��ļ���Ϣ finfo

    if(finfo.fname[0]==0x00)				  //���ļ�������һ���ֽ�Ϊ 0X00ʱ Ϊ���ļ�  ��������
	  {

	  list-=1;								  //list�����1 �����ȥ ȡ��ʵֵ
	  return list;
	  } 			            
	
	strcat((char*)short_name[list],(const char*)finfo.fname) ;	//�洢���ж��ļ��� ����ȡʱ�ļ�·��

  }



return 0;
		 
}




//������ʾ����
void scan_root()
{
GUI_Clear(White);	       //����

GUI_box(0,18,239,19,Black);
GUI_box(0,38,239,39,Black);

GUI_box(0,0,239,8,Black);
GUI_box(0,9,239,17,Black);
GUI_sprintf_hzstr16x(0,1,"OV���",Yellow,1);

GUI_sprintf_hzstr16x(207,1,"����",Yellow,1);
GUI_box(0,20,239,37,Green1);
GUI_sprintf_hzstr16x(0,21,"��Ƭ������",Red,1);

GUI_box(225,40,226,319,Black);	  //����1
GUI_box(238,40,239,319,Black);	  //����2

GUI_box(227,58,238,59,Black);	  //����1
GUI_box(227,299,238,300,Black);	  //����2

GUI_box(227,40,237,57,Blue);	  //��������ɫ���
GUI_box(227,301,237,319,Blue);

}




//ledָʾ��
void led_forroot(void)
 {
  led_d2=0;delay_ms(100);led_d2=1;
 }

//�ļ�ϵͳ ģ�� ɨ�輰��ʾ������  
//�˺��� ���� �򿪼���ʾ����tf ����
//Ŀǰ���Դ� �����ļ���  TXT  BMP  BIN���ļ�
//�˺����ڴ򿪶���ļ���ʱ���õݹ鷽ʽ
//const char *path  Ҫ���ļ����ļ���·��

u8 m[32];		           //��ȡ�ڴ��ļ��� ��������  ����ʾ Ҫ�ر��ڴ�
		   
u8  *tp2;			     //�洢Ԥ���ļ�·��

u8 sign=0;			       //�ļ��� ������  �˺���Ϊ���ļ��к���

void tp_show(const char *path)
{

u8 touch_root,		   //����ֵ ��ȡ��ʱ�洢����
   i,j,			       //��ʱ����
   offset=0,st,        //�ļ� ����ҳ����ʾ��ƫ����  ��������
   st2;			       //�жϵ�ǰ�ļ����� �ļ������ �Ƿ�С��14

u16 of=0; 			   //�������������
u8  yof=0;             
u8  to2,st3=0,		   //��Ƭ���ҷ� ����
    page;			   //��Ƭÿ14��һҳ ����Ƭ��ռ��ҳ��

st=scan_filename(path);   //��ȡָ��·���� �����ļ��� ������Ϣ ����ȫ���ⲿ�ڴ�ռ�	  	 
						  //���ж��ļ��� �洢�ռ�*short_name	  
						  //st�����ҵ����ļ�����

st+=1;			          //st�Ǵ�0��ʼ����� ���Ҫ���1	 stΪ���ļ�����
   
page=st/14;				  //ÿҳ14���ļ��� ͨ�����ļ��� �������Ҫ��ҳ��
if(st%14)
page++;

of=236/page;			  //����������Ϊ236�㣬 ����ÿһҳ��ҪҪ�õĽ���������


sign=0;					  //����������־λ����

scan_root();                  //����ʾ����
number(80,21,st,Red,1);	  //��ʾ��ǰҳ�ļ������


tp2=mymalloc(SRAMIN,32);			  //���� ·���洢ָ��  tp2


							   
			        //��ʾ ��������
while(1)
{

	st2=14;						 //��ʼ��ҳ��Ƭ����
								 //��ʾ����
	GUI_box(0,40,224,319,White); //����ļ�����ʾ����
    for(j=0;j<14;j++)		     //��ʾ�ļ���  ����Ļ��������������ʾ 13���ļ���
    {	

	
	  for(i=0;i<32;i++)		     //���ļ���������ʱ���� ����ʾ
	  m[i]=short_name[offset+j][i];
	

	  if(m[0]!=0)					 //���ļ�����ʾͼ��
	   flash_binplay(0,(j+2)*20,22,20,306);
	  else 	st2--;					 //���û����Ƭ ��ҳ��Ƭ������1

	  GUI_sprintf_hzstr16x(22,2+(j+2)*20,m,Black,White);	  //��ʾ�ļ���	
    }



	//���������ʾ����
	GUI_box(228,61,236,297,White);						  
	                                                    
	yof=((offset/14)+1)*of;							  //��������� 

	GUI_box(228,61,236,(61+yof),Blue);	              //��ʾ��ǰ����


	while(1)	   //�����ж�����
	{
		touch_root=ROOT_Touch();	//��ȡ����ֵ ����ʱ����touch_root


		if(touch_root<st2)			//˵�����ļ���ѡ��	   0-14 �ļ��������ݷ���ֵ
		{
	     //�ڵ�ǰѡ����ļ��������»�����ʾ
		 GUI_box(23,58+touch_root*20,224,59+touch_root*20,Blue);
		 delay_ms(500);
		

														//��ǰ�ļ�·�� + "/" + Ҫ���ļ���=Ԥ���ļ�·�� 
		 strcat((char*)tp2,(const char*)path);							//��ȡ��ǰ�ļ�·��
		 strcat((char*)tp2,(const char*)"/");							//�� /
		 strcat((char*)tp2,(const char*)short_name[offset+touch_root]); //����Ԥ���ļ�·��

			



			 GUI_Clear(Black);	       //����
			 Play_Bmp((const char *)tp2);

		 				     //��ʾ��ͼƬ���ļ�����
			 for(i=0;i<32;i++)		     
			 m[i]=short_name[offset+touch_root][i];

			 GUI_sprintf_hzstr16x(0,0,m,White,1);

			 GUI_sprintf_hzstr16x(206,302,"����",White,1);

			 st3=offset+touch_root;

			 while(1)				 //��Ƭ���ҷ�ҳ����
			  {
				to2=ROOT_Touch2();
				if(to2==20)
				 {
				     st3++;
					 
				     if(st3<st)
					 {
					   if(st3==st-1){st3-=1;GUI_sprintf_hzstr16x(143,0,"�ѵ����ײ�",White,1);}

					   for(i=0;i<32;i++)		   //���·��ָ��
					   tp2[i]=0;					   
					   strcat((char*)tp2,(const char*)path);						//��ȡ��ǰ�ļ�·��
					   strcat((char*)tp2,(const char*)"/");							//�� /
					   strcat((char*)tp2,(const char*)short_name[st3]);             //����Ԥ���ļ�·��


			 		   GUI_Clear(Black);	       //����
					   Play_Bmp((const char *)tp2);			   //��ʾ��Ƭ

				 	   				   //��ʾ��Ƭ��  
					   for(i=0;i<32;i++)		     
					   m[i]=short_name[st3][i];
				
					   GUI_sprintf_hzstr16x(0,0,m,White,1);
					   GUI_sprintf_hzstr16x(206,302,"����",White,1);
					 }  
				 }
				if(to2==19)
				 {
				     st3--;
					 if(st3==255){st3+=1;GUI_sprintf_hzstr16x(143,0,"�ѵ���ᶥ��",White,1);}
					 else if(st3>0&&st3<st)
					 {
			 		
					   for(i=0;i<32;i++)		   //���·��ָ��
					   tp2[i]=0;					   
					   strcat((char*)tp2,(const char*)path);						//��ȡ��ǰ�ļ�·��
					   strcat((char*)tp2,(const char*)"/");						    //�� /
					   strcat((char*)tp2,(const char*)short_name[st3]);             //����Ԥ���ļ�·��
					 

			 		   GUI_Clear(Black);	       //����
					   Play_Bmp((const char *)tp2);			   //��ʾ��Ƭ

				 	    				   //��ʾ��Ƭ��  
					   for(i=0;i<32;i++)		     
					   m[i]=short_name[st3][i];
				
					   GUI_sprintf_hzstr16x(0,0,m,White,1);
					   GUI_sprintf_hzstr16x(206,302,"����",White,1);
					 }  
				 }
				if(to2==17) 
				 {
				   GUI_sprintf_hzstr16x(206,302,"����",Red3,1);
				   GUI_sprintf_hzstr16x(207,303,"����",White,1);
				   delay_ms(300);
				   break;
				 }
				 
			  }

		   	 scan_root();        //����������ʾ����
			 number(80,21,st,White,1);	
			 														    
			
			 for(i=0;i<32;i++)		   //���·��ָ��
			 tp2[i]=0;
		

			 led_forroot();		 //ָʾ����˸
			 break;              //�����˼�ѭ���� 

		}




		
		if(touch_root==15)			//����
		 {
		   offset-=14;
		   if(offset>100)offset=0;
		   break;					
		 }

		if(touch_root==16)			//����
		 {
		   offset+=14;
		   if(offset>=st)offset-=14;	 //����������� ��ô�ͻָ�offsetԭ�е�ƫ��ֵ  
		   break;				 
		 }
		 	 		

		if(touch_root==17)		    //������һ��
		{		  
		  GUI_sprintf_hzstr16x(207,1,"����",Red,1);
		  led_forroot();		    //ָʾ����˸
		  delay_ms(300);
		  sign=1;				    

//		   for(i=0;i<32;i++)		      //�ͷŶ��ļ�������  �ڴ�ռ� 
//		   	myfree(SRAMIN,short_name[i]);
//		  				 
//		   myfree(SRAMIN,tp2);					  //�ͷ�·���ڴ�ռ�
		  mem_init(SRAMIN); 	  
		  if(sign==1)break;	    

		}

	 }


	 if(sign==1) break;			//�������			
	 
}
}








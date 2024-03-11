

#include  "mcusys.h"
#include  "delay.h"				  //��ʱ����
#include  "tft.h"			      //TFT IC�ײ�����
#include  "gui.h"
#include  "xpt2046.h"
#include  "spi.h"
#include  "sdio_sdcard.h" 
#include  "pff.h"				  //�ļ�ϵͳ����.h����
#include  "OVbmp.h"
#include  "malloc.h"


extern u8  tbuf[512];			   //512�ֽ� SD�����ݻ���ռ�


//����TF��������240x240 BMPͼƬ
//����Ļ��0,0��λ�õ�(240,240)λ�� ��ȡ240x240ͼƬ���� ��ת��ΪBMP��ʽ����TF����
//˵�������µ�ͼƬ�������Դ���TF��ָ��·����240x240 16λbmpͼƬ
//      �������ǳ�֮Ϊ��Ƭ  �����Ƭ�������Image2Lcd���������
//      ��Ƭ�����Ѿ�������30�� ������ov76�ļ�����  ����ֱ�ӵ���
//����˼·����Ϊpetit fatfs�ļ�ϵͳֻ����ԭ�е��ļ��ϸ��£���BMPͼƬǰ54�ֽ�
//          ��ͼƬ����Ϣ ��һ�����ǲ�Ҫ���� ����ԭ�о���
//          ���Գ�����Ҫ�Ƚ�ǰ54�ֽ�ȡ�� Ȼ����ڸ���54�ֽ��Ժ��ͼƬ����
//ע�⣺	petit fatfs�ļ�ϵͳ ��д����ʱ����Ҫ�� �����Ŀ�ͷд�� ��Ū���������м�д����
//          �����ں�����ȡ��ǰ54�ֽں� �ںͺ������ɫ�������512�ֽ� ������д������
//*path ����·��
//����0 ����ɹ�  1����ʧ��
u8 Write_Bmp(const char *path)
{
FRESULT res;
u16 br,i,j,m=0,n=239,color;

	   
                                     //�����ļ�·�����ⲿSRAM�� ��������Ҫ�����ⲿSRAM���ܵ���·��
res=pf_open(path);		             //��ָ��·���ļ�	 ��һ�����Դ��κ�·���µ��ļ� ע�����Ĺ��ܾ��Ǵ��ļ��������ļ���
                                     //���ļ������ݵ�ǰ���Ǳ���Ҫ������ļ�

if(res == FR_OK)
{
  pf_read(tbuf,54,&br);				 //��ȡBMPͼƬǰ54�ֽ�ͼƬ��Ϣ


  pf_open(path);					 //���´�·�� ������ָ��ͼƬ������λ��


     for(i=27;i<256;i++)				 //��ȡ512���ֽڵ�tbuf�� ��256����ɫ��
     {
	     color=LCD_readpoint(m,n);		 //��������ͷ����Ч�� 	  ��ȡ��ɫ
		   color=((color>>1)&0x7fe0)+(color&0x001f);	//����ȡ��565��ʽת��Ϊ555��ʽ

       tbuf[i*2]=color;				 //����ʱ���ֽ���ǰ
	     tbuf[i*2+1]=(color>>8);
	     m++;
     }	  
     pf_write(tbuf,512,&br);			 //��TF����д��512���ֽ�

     for(j=0;j<254;j++)
	   {
	     for(i=0;i<256;i++)				 //��ȡ512���ֽڵ�tbuf�� ��256����ɫ��
	      {
					color=LCD_readpoint(m,n);		 //��������ͷ����Ч��  ��ȡ��ɫ
					color=((color>>1)&0x7fe0)+(color&0x001f); //����ȡ��565��ʽת��Ϊ555��ʽ

					tbuf[i*2]=color;				 //����ʱ���ֽ���ǰ
					tbuf[i*2+1]=(color>>8);
	
		      m++;
		      if(m==240)
		      {
		         m=0;
			       n--;	   		//���ﲻ���ж�m ��Ϊѭ���̶� ֱ�ӻ����� 
		      }
	      }	  
	     pf_write(tbuf,512,&br);			 //��TF����д��512���ֽ�	 
	 
	   }

//   SD_DisSelect();//ȡ��TF��Ƭѡ  ��д�뺯�����ȡ��Ƭѡ ����Ӱ�� ���������д����ɼ�ȡ��Ƭѡ
   return 0;  //д��ɹ�

}
return 1;    //����

}




//����16λɫ��24λɫBMPͼƬ
//ͼƬ��ʾ�� ��Ļ������λ��
//����bmp�������� ǰ4���ֽ�Ϊ Ϊ�ļ���С  18-21��  22-25 ��  28���أ���ֵΪ1��4��8��16��24����32 
//*path  bmpͼƬ�ļ���
//���� 0  ��ʾ���  1 ��ʾ�������� 
u8 Play_Bmp(const char *path)
{
FRESULT res; 

u16 br,zy,height,	  //width,heightͼƬ�ĳ�ʼ���
     y1,i1,tmp;		              //tmp 16λ�������
u8 zx,width,x1,
   rgb=0, 
   Bmpcolor;

u32 num=0,num2;


res=pf_open(path);		 //��ָ��·���ļ�

if(res == FR_OK){


    pf_read(tbuf, 54, &br);		 //ȡǰ54�ֽ�  ǰ54�ֽں���bmp �ļ���С �ļ�����߶� ����ֵ ��������Ϣ   
	if(br!=54) return 1;		 //��ȡ����

											//ʵ�ʿ�͸߶�����4���ֽڱ�ʾ�ģ�������ԥͼƬ�Ĵ�С���ܳ������ĳߴ�
											//����ֻ��һ���ֽڱ�ʾ���,�����ֽڱ�ʾ�߶�
	width= tbuf[18];				        //�����ļ����	 
	height= ((u16)tbuf[23]<<8)+tbuf[22];	//�����ļ��߶�

	Bmpcolor=tbuf[28]/8;					//��ȡ��ɫ��� һ����16λ 24λ 32λ  

	//��С����Ļ�ߴ��ͼƬ�ŵ���Ļ���м���ʾ
	if(width<239)   zx=(240-width)/2;         else zx=0;
	if(height<319)	zy=(320-height)/2;        else zy=0;
	
	num2=width*height;		   //������ͼƬ����Ļ��ʾ�ĵ㣨���أ��ĸ���
	x1=zx; y1=zy;			   //��ֵ������ֵ


//	if(lcd_id==9325)
//	 {		//BMPͼƬ�����ɨ�跽ʽΪ ���µ��� ������
//		    Lcd_WriteReg(0x0003,0x1010); //ɨ�跽������
//		    Lcd_WR_Reg(0x0022);			 //���������
//	  }
//	else if(lcd_id==9341)
	     LCD_scan(2);	  //BMPͼƬ�����ɨ�跽ʽΪ ������   ���µ��� 
	
    Address_set(x1,y1,x1+width-1,y1+height-1,x1,y1+height-1);         //������ʾ��Χ ��ɨ���� ��ɨ����	 �����λ����9325  9341��Ч

	TFT_RS=1;    //д����������	 	  Ϊ���д���ٶ� ��ѭ��ǰ����
	while(1)                   //һֱ�����һ��
	{
		  	  
	 pf_read(tbuf, 512, &br);		 //��54�ֽں�λ�ö�ȡ512���ֽڵ�������  
	     
		
	 for(i1=0;i1<512;i1++)
	 {
	  	if(Bmpcolor==2)				 //16λBMP
		{
         switch(rgb)				 //555ת565��ʽ
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
		else if(Bmpcolor==3)		//24λBMP���� RGB�ֱ�ռ8���ֽ�
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
			//Ϊ�������ʾ�ٶ� ֱ�ӵ���IO�ڱ���
      GPIOB->BSRRH = GPIO_Pin_All;     //��Ĵ�������
      GPIOB->BSRRL = tmp;            //���ɼ�����ɫֵ��PB�ڼĴ��� 			
			


		    //д����  ��Ϊ��λ��ǰ�� ����ǰ�����Ҫ�Ⱥ���Ķ�1  void Lcd_Write_Data(u16 Data)�����ķֽ�

			tmp=0;
			rgb=0;

		    TFT_WR=0;								      //��ʼд��
        TFT_WR=1;		  
			
			
			num++;//��ʼ������ 	       
			if(num==num2)			 //����ʾ�����ص� ��ͼƬ��Ϣ������һ��ʱ ����
			{
	
//			  	if(lcd_id==9325)
//				{
//			        							 //�ָ�����ɨ�跽ʽ
//				 Lcd_WriteReg(0x0003,0x1030);	 //LCD��ʾ��������   �ֲ�56ҳ
//				 Lcd_WR_Reg(0x0022);			 //��������
//				}
//				else if(lcd_id==9341) 
                LCD_scan(1);

				 return 0;    //��ʾ���	

			}

		 }

	  } 

	 }


	 
   }
 return 1;   //���ܳ���
}		    







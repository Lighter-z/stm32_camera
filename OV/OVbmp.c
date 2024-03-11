

#include  "mcusys.h"
#include  "delay.h"				  //ÑÓÊ±º¯Êý
#include  "tft.h"			      //TFT ICµ×²ãÇý¶¯
#include  "gui.h"
#include  "xpt2046.h"
#include  "spi.h"
#include  "sdio_sdcard.h" 
#include  "pff.h"				  //ÎÄ¼þÏµÍ³µ÷ÓÃ.h¼ÓÔØ
#include  "OVbmp.h"
#include  "malloc.h"


extern u8  tbuf[512];			   //512×Ö½Ú SD¿¨Êý¾Ý»º´æ¿Õ¼ä


//¸üÐÂTF¿¨ÉÏÒÔÓÐ240x240 BMPÍ¼Æ¬
//´ÓÆÁÄ»£¨0,0£©Î»ÖÃµ½(240,240)Î»ÖÃ ½ØÈ¡240x240Í¼Æ¬Êý¾Ý ²¢×ª»»ÎªBMP¸ñÊ½´æÈëTF¿¨ÖÐ
//ËµÃ÷£º¸üÐÂµÄÍ¼Æ¬±ØÐëÊÇÒÔ´æÔÚTF¿¨Ö¸¶¨Â·¾¶µÄ240x240 16Î»bmpÍ¼Æ¬
//      ÕâÀïÎÒÃÇ³ÆÖ®Îªµ×Æ¬  Õâ¸öµ×Æ¬ÊÇÓÃÈí¼þImage2LcdÈí¼þÖÆ×÷µÄ
//      µ×Æ¬ÎÒÃÇÒÑ¾­ÖÆ×÷ºÃ30ÕÅ ´æÈëÔÚov76ÎÄ¼þ¼ÐÏÂ  ¿ÉÒÔÖ±½Óµ÷ÓÃ
//º¯ÊýË¼Â·£ºÒòÎªpetit fatfsÎÄ¼þÏµÍ³Ö»ÄÜÔÚÔ­ÓÐµÄÎÄ¼þÉÏ¸üÐÂ£¬¶øBMPÍ¼Æ¬Ç°54×Ö½Ú
//          ÊÇÍ¼Æ¬µÄÐÅÏ¢ ÕâÒ»²¿·ÖÊÇ²»Òª¸ü¸Ä ±£´æÔ­ÓÐ¾ÍÐÐ
//          ËùÒÔ³ÌÐòÉÏÒªÏÈ½«Ç°54×Ö½ÚÈ¡³ö È»ºóºóÔÚ¸üÐÂ54×Ö½ÚÒÔºóµÄÍ¼Æ¬Êý¾Ý
//×¢Òâ£º	petit fatfsÎÄ¼þÏµÍ³ ÔÚÐ´Êý¾ÝÊ±±ØÐëÒª´Ó ÉÈÇøµÄ¿ªÍ·Ð´Æð ²»Åª³ÉÉÈÇøµÄÖÐ¼äÐ´Êý¾Ý
//          ËùÒÔÔÚº¯ÊýÌáÈ¡ÁËÇ°54×Ö½Úºó ÔÚºÍºóÃæµÄÑÕÉ«Êý¾Ý×é³É512×Ö½Ú ÔÚÖØÐÂÐ´»ØÉÈÇø
//*path ±£´æÂ·¾¶
//·µ»Ø0 ±£´æ³É¹¦  1±£´æÊ§°Ü
u8 Write_Bmp(const char *path)
{
FRESULT res;
u16 br,i,j,m=0,n=239,color;

	   
                                     //ÓÉÓÚÎÄ¼þÂ·¾¶ÔÚÍâ²¿SRAMÖÐ ËùÒÔÕâÀïÒª¿ªÆôÍâ²¿SRAM²ÅÄÜµ÷ÓÃÂ·¾¶
res=pf_open(path);		             //´ò¿ªÖ¸¶¨Â·¾¶ÎÄ¼þ	 ÕâÒ»²½¿ÉÒÔ´ò¿ªÈÎºÎÂ·¾¶ÏÂµÄÎÄ¼þ ×¢ÒâËüµÄ¹¦ÄÜ¾ÍÊÇ´ò¿ªÎÄ¼þ£¬²»ÊÇÎÄ¼þ¼Ð
                                     //¶ÁÎÄ¼þÄÚÊý¾ÝµÄÇ°ÌáÊÇ±ØÐëÒª´ò¿ªÕâ¸öÎÄ¼þ

if(res == FR_OK)
{
  pf_read(tbuf,54,&br);				 //ÌáÈ¡BMPÍ¼Æ¬Ç°54×Ö½ÚÍ¼Æ¬ÐÅÏ¢


  pf_open(path);					 //ÖØÐÂ´ò¿ªÂ·¾¶ ½«ÉÈÇøÖ¸ÏòÍ¼Æ¬Ê×Êý¾ÝÎ»ÖÃ


     for(i=27;i<256;i++)				 //ÌáÈ¡512¸ö×Ö½Úµ½tbufÖÐ ¼´256¸öÑÕÉ«µã
     {
	     color=LCD_readpoint(m,n);		 //·ûºÏÉãÏñÍ·ÉãÏñÐ§¹û 	  ÌáÈ¡ÑÕÉ«
		   color=((color>>1)&0x7fe0)+(color&0x001f);	//½«ÌáÈ¡µÄ565¸ñÊ½×ª»»Îª555¸ñÊ½

       tbuf[i*2]=color;				 //´æÈëÊ±µÍ×Ö½ÚÔÚÇ°
	     tbuf[i*2+1]=(color>>8);
	     m++;
     }	  
     pf_write(tbuf,512,&br);			 //ÏòTF¿¨ÄÚÐ´Èë512¸ö×Ö½Ú

     for(j=0;j<254;j++)
	   {
	     for(i=0;i<256;i++)				 //ÌáÈ¡512¸ö×Ö½Úµ½tbufÖÐ ¼´256¸öÑÕÉ«µã
	      {
					color=LCD_readpoint(m,n);		 //·ûºÏÉãÏñÍ·ÉãÏñÐ§¹û  ÌáÈ¡ÑÕÉ«
					color=((color>>1)&0x7fe0)+(color&0x001f); //½«ÌáÈ¡µÄ565¸ñÊ½×ª»»Îª555¸ñÊ½

					tbuf[i*2]=color;				 //´æÈëÊ±µÍ×Ö½ÚÔÚÇ°
					tbuf[i*2+1]=(color>>8);
	
		      m++;
		      if(m==240)
		      {
		         m=0;
			       n--;	   		//ÕâÀï²»ÓÃÅÐ¶Ïm ÒòÎªÑ­»·¹Ì¶¨ Ö±½Ó»áÌø³ö 
		      }
	      }	  
	     pf_write(tbuf,512,&br);			 //ÏòTF¿¨ÄÚÐ´Èë512¸ö×Ö½Ú	 
	 
	   }

//   SD_DisSelect();//È¡ÏûTF¿¨Æ¬Ñ¡  ÔÚÐ´Èëº¯ÊýÀï¼ÓÈ¡ÏûÆ¬Ñ¡ »áÓÐÓ°Ïì ËùÒÔÔÚ×îºóÐ´ÈëÍê³É¼ÓÈ¡ÏûÆ¬Ñ¡
   return 0;  //Ð´Èë³É¹¦

}
return 1;    //´íÎó

}




//½âÂë16Î»É«¡¢24Î»É«BMPÍ¼Æ¬
//Í¼Æ¬ÏÔÊ¾ÔÚ ÆÁÄ»µÄÕýÖÐÎ»ÖÃ
//½øÈëbmpÊ×ÉÈÇøºó Ç°4¸ö×Ö½ÚÎª ÎªÎÄ¼þ´óÐ¡  18-21¿í  22-25 ¸ß  28ÏóËØ£¬ÆäÖµÎª1¡¢4¡¢8¡¢16¡¢24¡¢»ò32 
//*path  bmpÍ¼Æ¬ÎÄ¼þÃû
//·µ»Ø 0  ÏÔÊ¾Íê³É  1 ÏÔÊ¾³öÏÖÎÊÌâ 
u8 Play_Bmp(const char *path)
{
FRESULT res; 

u16 br,zy,height,	  //width,heightÍ¼Æ¬µÄ³õÊ¼×ó±ß
     y1,i1,tmp;		              //tmp 16Î»½âÂë±äÁ¿
u8 zx,width,x1,
   rgb=0, 
   Bmpcolor;

u32 num=0,num2;


res=pf_open(path);		 //´ò¿ªÖ¸¶¨Â·¾¶ÎÄ¼þ

if(res == FR_OK){


    pf_read(tbuf, 54, &br);		 //È¡Ç°54×Ö½Ú  Ç°54×Ö½Úº¬ÓÐbmp ÎÄ¼þ´óÐ¡ ÎÄ¼þ³¤¿í³ß¶È ÏñËØÖµ µÈÊý¾ÝÐÅÏ¢   
	if(br!=54) return 1;		 //ÌáÈ¡³ö´í

											//Êµ¼Ê¿íºÍ¸ß¶¼ÊÇÓÃ4¸ö×Ö½Ú±íÊ¾µÄ£¬µ«ÊÇÓÌÔ¥Í¼Æ¬µÄ´óÐ¡²»ÄÜ³¬¹ýÆÁµÄ³ß´ç
											//ÕâÀïÖ»ÓÃÒ»¸ö×Ö½Ú±íÊ¾¿í¶È,Á½¸ö×Ö½Ú±íÊ¾¸ß¶È
	width= tbuf[18];				        //¼ÆËãÎÄ¼þ¿í¶È	 
	height= ((u16)tbuf[23]<<8)+tbuf[22];	//¼ÆËãÎÄ¼þ¸ß¶È

	Bmpcolor=tbuf[28]/8;					//»ñÈ¡ÑÕÉ«Éî¶È Ò»°ãÊÇ16Î» 24Î» 32Î»  

	//½«Ð¡ÓÚÆÁÄ»³ß´çµÄÍ¼Æ¬·Åµ½ÆÁÄ»ÕýÖÐ¼äÏÔÊ¾
	if(width<239)   zx=(240-width)/2;         else zx=0;
	if(height<319)	zy=(320-height)/2;        else zy=0;
	
	num2=width*height;		   //¼ÆÊý´ËÍ¼Æ¬ÔÚÆÁÄ»ÏÔÊ¾µÄµã£¨ÏñËØ£©µÄ¸öÊý
	x1=zx; y1=zy;			   //¸³Öµ¼ÆËãºóµÄÖµ


//	if(lcd_id==9325)
//	 {		//BMPÍ¼Æ¬½âÂëµÄÉ¨Ãè·½Ê½Îª ´ÓÏÂµ½ÉÏ ´Ó×óµ½ÓÒ
//		    Lcd_WriteReg(0x0003,0x1010); //É¨Ãè·½ÏòÉèÖÃ
//		    Lcd_WR_Reg(0x0022);			 //·¢ËÍÉèÖÃÃ
//	  }
//	else if(lcd_id==9341)
	     LCD_scan(2);	  //BMPÍ¼Æ¬½âÂëµÄÉ¨Ãè·½Ê½Îª ´Ó×óµ½ÓÒ   ´ÓÏÂµ½ÉÏ 
	
    Address_set(x1,y1,x1+width-1,y1+height-1,x1,y1+height-1);         //ÉèÖÃÏÔÊ¾·¶Î§ ÏÈÉ¨ºáÐÐ ÔÚÉ¨×ÝÐÐ	 ×îºóÁ½Î»¼æÈÝ9325  9341ÎÞÐ§

	TFT_RS=1;    //Ð´Êý¾ÝÏßÀ­¸ß	 	  ÎªÌá¸ßÐ´ÈëËÙ¶È Ö÷Ñ­»·Ç°À­¸ß
	while(1)                   //Ò»Ö±µ½×îºóÒ»´Ø
	{
		  	  
	 pf_read(tbuf, 512, &br);		 //´Ó54×Ö½ÚºóÎ»ÖÃ¶ÁÈ¡512¸ö×Ö½Úµ½»º´æÇø  
	     
		
	 for(i1=0;i1<512;i1++)
	 {
	  	if(Bmpcolor==2)				 //16Î»BMP
		{
         switch(rgb)				 //555×ª565¸ñÊ½
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
		else if(Bmpcolor==3)		//24Î»BMP½âÂë RGB·Ö±ðÕ¼8¸ö×Ö½Ú
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
			//ÎªÁËÌá¸ßÏÔÊ¾ËÙ¶È Ö±½Óµ÷ÓÃIO¿Ú±¾Éí
      GPIOB->BSRRH = GPIO_Pin_All;     //Çå¼Ä´æÆ÷Êý¾Ý
      GPIOB->BSRRL = tmp;            //½«²É¼¯µÄÑÕÉ«Öµ¸øPB¿Ú¼Ä´æÆ÷ 			
			


		    //Ð´Êý¾Ý  ÒòÎª¸ßÎ»ÔÚÇ°Ãæ ËùÒÔÇ°ÃæµÄÊýÒª±ÈºóÃæµÄ¶à1  void Lcd_Write_Data(u16 Data)º¯ÊýµÄ·Ö½â

			tmp=0;
			rgb=0;

		    TFT_WR=0;								      //¿ªÊ¼Ð´Èë
        TFT_WR=1;		  
			
			
			num++;//¿ªÊ¼´òµã¼ÆÊý 	       
			if(num==num2)			 //µ±ÏÔÊ¾µÄÏñËØµã ºÍÍ¼Æ¬ÐÅÏ¢Àï¼ÆËãµÄÒ»ÑùÊ± Ìø³ö
			{
	
//			  	if(lcd_id==9325)
//				{
//			        							 //»Ö¸´Õý³£É¨Ãè·½Ê½
//				 Lcd_WriteReg(0x0003,0x1030);	 //LCDÏÔÊ¾·½ÏòÉèÖÃ   ÊÖ²á56Ò³
//				 Lcd_WR_Reg(0x0022);			 //·¢ËÍÉèÖÃ
//				}
//				else if(lcd_id==9341) 
                LCD_scan(1);

				 return 0;    //ÏÔÊ¾Íê³É	

			}

		 }

	  } 

	 }


	 
   }
 return 1;   //¿ÉÄÜ³ö´í
}		    







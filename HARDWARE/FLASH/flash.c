


#include "mcusys.h"
#include "flash.h" 
#include "spi.h"
#include "delay.h" 
#include "led.h"  
#include  "pff.h"
#include  "gui.h"
#include  "tft.h"


extern u8 tbuf[512];

//4Kbytes为一个Sector
//16个扇区为1个Block
//W25X16
//容量为2M字节,共有32个Block,512个Sector 

//初始化SPI FLASH的IO口
void SPI_Flash_Init(void)
{	
  	GPIO_InitTypeDef GPIO_InitStructure;

		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);//PORTE时钟使能 


		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;//PE15
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//100MHz
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
		GPIO_Init(GPIOE, &GPIO_InitStructure);
		GPIO_SetBits(GPIOE,GPIO_Pin_15);

//	SPI1_Init();		   	//初始化SPI
//	SPI1_SetSpeed(SPI_BaudRatePrescaler_16);//设置为18M时钟,高速模式
}   



//读取SPI_FLASH的状态寄存器
//BIT7  6   5   4   3   2   1   0
//SPR   RV  TB BP2 BP1 BP0 WEL BUSY
//SPR:默认0,状态寄存器保护位,配合WP使用
//TB,BP2,BP1,BP0:FLASH区域写保护设置
//WEL:写使能锁定
//BUSY:忙标记位(1,忙;0,空闲)
//默认:0x00
u8 SPI_Flash_ReadSR(void)   
{  
	u8 byte=0;   
	//SPI_FLASH_CS=0;                            //使能器件
	SPI_FLASH_CS_0;   
	SPI1_ReadWriteByte(W25X_ReadStatusReg);    //发送读取状态寄存器命令    
	byte=SPI1_ReadWriteByte(0Xff);             //读取一个字节
	SPI_FLASH_CS_1;  
//	SPI_FLASH_CS=1;                            //取消片选     
	return byte;   
} 
//写SPI_FLASH状态寄存器
//只有SPR,TB,BP2,BP1,BP0(bit 7,5,4,3,2)可以写!!!
void SPI_FLASH_Write_SR(u8 sr)   
{   
	//SPI_FLASH_CS=0;                            //使能器件 
	SPI_FLASH_CS_0;   
	SPI1_ReadWriteByte(W25X_WriteStatusReg);   //发送写取状态寄存器命令    
	SPI1_ReadWriteByte(sr);               //写入一个字节 
	SPI_FLASH_CS_1;  
	//SPI_FLASH_CS=1;                            //取消片选     	      
}   
//SPI_FLASH写使能	
//将WEL置位   
void SPI_FLASH_Write_Enable(void)   
{
	SPI_FLASH_CS_0; //SPI_FLASH_CS=0;                            //使能器件
	   
    SPI1_ReadWriteByte(W25X_WriteEnable);      //发送写使能  
	SPI_FLASH_CS_1; //SPI_FLASH_CS=1;                            //取消片选     	      
} 
//SPI_FLASH写禁止	
//将WEL清零  
void SPI_FLASH_Write_Disable(void)   
{  
	SPI_FLASH_CS_0; //SPI_FLASH_CS=0;                            //使能器件   
    SPI1_ReadWriteByte(W25X_WriteDisable);     //发送写禁止指令    
	SPI_FLASH_CS_1; //SPI_FLASH_CS=1;                            //取消片选     	      
} 			    
//读取芯片ID W25X16的ID:0XEF14
u16 SPI_Flash_ReadID(void)
{
	u16 Temp = 0;	  
	SPI_FLASH_CS_0; //SPI_FLASH_CS=0;				    
	SPI1_ReadWriteByte(0x90);//发送读取ID命令	    
	SPI1_ReadWriteByte(0x00); 	    
	SPI1_ReadWriteByte(0x00); 	    
	SPI1_ReadWriteByte(0x00); 	 			   
	Temp|=SPI1_ReadWriteByte(0xFF)<<8;  
	Temp|=SPI1_ReadWriteByte(0xFF);	 
	SPI_FLASH_CS_1; //SPI_FLASH_CS=1;				    
	return Temp;
}   		    
//读取SPI FLASH  
//在指定地址开始读取指定长度的数据
//pBuffer:数据存储区
//ReadAddr:开始读取的地址(24bit)
//NumByteToRead:要读取的字节数(最大65535)
void SPI_Flash_Read(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead)   
{ 
 	u16 i;    												    
	SPI_FLASH_CS_0; //SPI_FLASH_CS=0;                            //使能器件   
    SPI1_ReadWriteByte(W25X_ReadData);         //发送读取命令   
    SPI1_ReadWriteByte((u8)((ReadAddr)>>16));  //发送24bit地址    
    SPI1_ReadWriteByte((u8)((ReadAddr)>>8));   
    SPI1_ReadWriteByte((u8)ReadAddr);   
    for(i=0;i<NumByteToRead;i++)
	{ 
        pBuffer[i]=SPI1_ReadWriteByte(0XFF);   //循环读数  
    }
	SPI_FLASH_CS_1; //SPI_FLASH_CS=1;                            //取消片选     	      
}  
//SPI在一页(0~65535)内写入少于256个字节的数据
//在指定地址开始写入最大256字节的数据
//pBuffer:数据存储区
//WriteAddr:开始写入的地址(24bit)
//NumByteToWrite:要写入的字节数(最大256),该数不应该超过该页的剩余字节数!!!	 
void SPI_Flash_Write_Page(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)
{
 	u16 i;  
    SPI_FLASH_Write_Enable();                  //SET WEL 
	SPI_FLASH_CS_0; //SPI_FLASH_CS=0;                            //使能器件   
    SPI1_ReadWriteByte(W25X_PageProgram);      //发送写页命令   
    SPI1_ReadWriteByte((u8)((WriteAddr)>>16)); //发送24bit地址    
    SPI1_ReadWriteByte((u8)((WriteAddr)>>8));   
    SPI1_ReadWriteByte((u8)WriteAddr);   
    for(i=0;i<NumByteToWrite;i++)SPI1_ReadWriteByte(pBuffer[i]);//循环写数  
	SPI_FLASH_CS_1; //SPI_FLASH_CS=1;                            //取消片选 
	SPI_Flash_Wait_Busy();					   //等待写入结束
} 
//无检验写SPI FLASH 
//必须确保所写的地址范围内的数据全部为0XFF,否则在非0XFF处写入的数据将失败!
//具有自动换页功能 
//在指定地址开始写入指定长度的数据,但是要确保地址不越界!
//pBuffer:数据存储区
//WriteAddr:开始写入的地址(24bit)
//NumByteToWrite:要写入的字节数(最大65535)
//CHECK OK
void SPI_Flash_Write_NoCheck(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)   
{ 			 		 
	u16 pageremain;	   
	pageremain=256-WriteAddr%256; //单页剩余的字节数		 	    
	if(NumByteToWrite<=pageremain)pageremain=NumByteToWrite;//不大于256个字节
	while(1)
	{	   
		SPI_Flash_Write_Page(pBuffer,WriteAddr,pageremain);
		if(NumByteToWrite==pageremain)break;//写入结束了
	 	else //NumByteToWrite>pageremain
		{
			pBuffer+=pageremain;
			WriteAddr+=pageremain;	

			NumByteToWrite-=pageremain;			  //减去已经写入了的字节数
			if(NumByteToWrite>256)pageremain=256; //一次可以写入256个字节
			else pageremain=NumByteToWrite; 	  //不够256个字节了
		}
	};	    
} 
//写SPI FLASH  
//在指定地址开始写入指定长度的数据
//该函数带擦除操作!
//pBuffer:数据存储区
//WriteAddr:开始写入的地址(24bit)
//NumByteToWrite:要写入的字节数(最大65535)  		   
u8 SPI_FLASH_BUF[4096];
void SPI_Flash_Write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)   
{ 
	u32 secpos;
	u16 secoff;
	u16 secremain;	   
 	u16 i;    
	 
	secpos=WriteAddr/4096;//扇区地址 0~511 for w25x16

	secoff=WriteAddr%4096;//在扇区内的偏移

	secremain=4096-secoff;//扇区剩余空间大小   
	
	if(NumByteToWrite<=secremain)secremain=NumByteToWrite;//不大于4096个字节
	while(1) 
	{
		SPI_Flash_Read(SPI_FLASH_BUF,secpos*4096,4096);//读出整个扇区的内容
		 
		for(i=0;i<secremain;i++)//校验数据
		{
			if(SPI_FLASH_BUF[secoff+i]!=0XFF)break;//需要擦除  	  
		}
		if(i<secremain)//需要擦除
		{
			SPI_Flash_Erase_Sector(secpos);//擦除这个扇区
			for(i=0;i<secremain;i++)	   //复制
			{
				SPI_FLASH_BUF[i+secoff]=pBuffer[i];	  
			}
			SPI_Flash_Write_NoCheck(SPI_FLASH_BUF,secpos*4096,4096);//写入整个扇区  

		}else SPI_Flash_Write_NoCheck(pBuffer,WriteAddr,secremain);//写已经擦除了的,直接写入扇区剩余区间. 				   
		if(NumByteToWrite==secremain){break;}//写入结束了
		else//写入未结束
		{	
			secpos++;//扇区地址增1
			secoff=0;//偏移位置为0 	 

		   	pBuffer+=secremain;  //指针偏移
			WriteAddr+=secremain;//写地址偏移	   
		   	NumByteToWrite-=secremain;				//字节数递减
			if(NumByteToWrite>4096)secremain=4096;	//下一个扇区还是写不完
			else secremain=NumByteToWrite;			//下一个扇区可以写完了
		}
			 
	};	 	 
}
//擦除整个芯片
//整片擦除时间:
//W25X16:25s 
//W25X32:40s 
//W25X64:40s 
//等待时间超长...
void SPI_Flash_Erase_Chip(void)   
{                                             
    SPI_FLASH_Write_Enable();                  //SET WEL 
    SPI_Flash_Wait_Busy();   
  	SPI_FLASH_CS_0; //SPI_FLASH_CS=0;                            //使能器件   
    SPI1_ReadWriteByte(W25X_ChipErase);        //发送片擦除命令  
	SPI_FLASH_CS_1; //SPI_FLASH_CS=1;                            //取消片选     	      
	SPI_Flash_Wait_Busy();   				   //等待芯片擦除结束
}   
//擦除一个扇区
//Dst_Addr:扇区地址 0~511 for w25x16
//擦除一个山区的最少时间:150ms
void SPI_Flash_Erase_Sector(u32 Dst_Addr)   
{   
	Dst_Addr*=4096;
    SPI_FLASH_Write_Enable();                  //SET WEL 	 
    SPI_Flash_Wait_Busy();   
  	SPI_FLASH_CS_0; //SPI_FLASH_CS=0;                            //使能器件   
    SPI1_ReadWriteByte(W25X_SectorErase);      //发送扇区擦除指令 
    SPI1_ReadWriteByte((u8)((Dst_Addr)>>16));  //发送24bit地址    
    SPI1_ReadWriteByte((u8)((Dst_Addr)>>8));   
    SPI1_ReadWriteByte((u8)Dst_Addr);  
	SPI_FLASH_CS_1; //SPI_FLASH_CS=1;                            //取消片选     	      
    SPI_Flash_Wait_Busy();   				   //等待擦除完成
}  
//等待空闲
void SPI_Flash_Wait_Busy(void)   
{   
	while ((SPI_Flash_ReadSR()&0x01)==0x01);   // 等待BUSY位清空
}  
//进入掉电模式
void SPI_Flash_PowerDown(void)   
{ 
  	SPI_FLASH_CS_0; //SPI_FLASH_CS=0;                            //使能器件   
    SPI1_ReadWriteByte(W25X_PowerDown);        //发送掉电命令  
	SPI_FLASH_CS_1; //SPI_FLASH_CS=1;                            //取消片选     	      
    delay_us(3);                               //等待TPD  
}   
//唤醒
void SPI_Flash_WAKEUP(void)   
{  
  	SPI_FLASH_CS_0; //SPI_FLASH_CS=0;                            //使能器件   
    SPI1_ReadWriteByte(W25X_ReleasePowerDown);   //  send W25X_PowerDown command 0xAB    
	SPI_FLASH_CS_1; //SPI_FLASH_CS=1;                            //取消片选     	      
    delay_us(3);                               //等待TRES1
}   







//将SD卡里的数据 复制到指定FLASH扇区位置  
//flash为2M 512个扇区 一个扇区4096个字节
//读取SD卡数据 建立在 FATFS文件系统上 
//存取方式 只能以扇区形式存入 flash中 
//写数据时不管当前位置有没有数据都会先清除一个扇区即4k字节 
//然后在进行写入 （写入时 一定要考虑当前路径是否为空，是否有用 切记！！）
// const char *path   sd卡文件路径
// sector  预存入flash的扇区位置
void copy_sdtoflash(const char *path,u16 sector) //要复制的文件名路径  要复制到flash的是从第几扇区开始
{
  FRESULT res;
  u16 br,num=0;
  u32 add;

  SPI_Flash_Erase_Sector(sector);	 //擦除开始扇区	 写入前先擦除相应扇区 然后进行写入

  add=(u32)sector*4096;              //计算出其开始地址

  res=pf_open(path);			    //打开指定文件	这个文件是TF卡中指定路径
//  GUI_sprintf_hzstr16x(0,0,"Copy TF to Flash update...",Blue,White);
//  GUI_sprintf_hzstr16x(0,16,"The sector:",Blue,White);
  if(res == FR_OK){
    while(1)
	 {
	  pf_read(tbuf, 512, &br);      //一次读取512个字节
	  num++;						//读取次数累加 一次读取512个字节 一个FLASH写入的扇区 是4k字节 
	                                //即要写入一个扇区的FLASH要读取8次512个字节

	  if(num>8)				        //判断 超过一个扇区时 擦除下一个扇区 为写入做准备
	   {
				sector++;				    //扇区自加
				SPI_Flash_Erase_Sector(sector);	   //擦除指定扇区 即下一个要更新的扇区
				num=1;								   //清计算变量
				//		 number(88,16,sector,Red,White);	   //显示要更新的扇区
				led_d2=!led_d2;  
	   }

	  SPI_Flash_Write_NoCheck(tbuf,add,br);	   //在指定FLASH地址内写入数据

	  add+=br;								   //地址增加


                                               //当br不是512时 说明从TF卡里读出的数据已经不足一个TF卡扇区了
											   //那就是已经到达文件底部了 更新完这一部分就完成了文件的复制	 
	  if(br!=512)   break;	
	  		   
	                                           
	 }
     //LED=!LED;
   }

}







extern u8 tbuf[512];			   //512字节 SD卡数据缓存空间

//显示flash里的bin文件 
//主要用于显示  图标  如果综合 各功能选择图标   SD卡 文件图标 等等
//bin文件 是通过  copy_sdtoflash 函数 读取SD卡指定地址数据 存入其中
// x1 y1  bin图片的开始坐标  
// width  height  bin文件的 宽度 和高度
// sector  bin文件 所在 flash中的指定扇区地址
//返回 0  成功   1 出现问题
u8 flash_binplay(u16 x1,u16 y1,u16 width,u16 height,u16 sector)
{

u16 i1,x,y;
u32 add=0;


add=(u32)sector*4096;			  //得到指定flash地址

 x=x1; y=y1;

Address_set(x,y,x+width-1,y+height-1,x,y);                     //设置显示范围	   要符号BIN 生成方式 否则产生乱码



while(1)                  				 //循环扫描显示
	{

	 SPI_Flash_Read(tbuf,add,512) ;		 //从初始位置开始一次读取512个字节到缓存区  
     
 	 	 
    	  for(i1=0;i1<256;i1++)                       //然后写到液晶屏，可以显示256个像素，每个像素16位即2个字节 也就是每两个字节能显示一个像素点 
	      { 		    

		   Lcd_Write_Data((tbuf[i1*2+1]<<8)+(tbuf[i1*2]));	

	        x++;								      //横向自加 加满一行 横向清零 纵向自加 直到扫完整个屏幕
	        if(x==width+x1)                           //检测是否到图片边缘
	        {	 
	        y++;
	        x=x1;
	        if(y==height+y1) return 0; 	          //都显示完 跳出函数  显示完成
	        }
          }

		 add+=512;									  //扇区自加
     }

// return 1;	  //出错

}






















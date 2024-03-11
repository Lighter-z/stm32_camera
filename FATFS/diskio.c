/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for Petit FatFs (C)ChaN, 2009   
   低端的磁盘I/O模块骨架Petit FatFs（C），2009   */
/*-----------------------------------------------------------------------*/

#include "diskio.h"
#include "mcusys.h"
#include "spi.h"

#include "sdio_sdcard.h"


u8 dest_tbuf[512];

/*-----------------------------------------------------------------------*/
/* Initialize Disk Drive      
   初始化磁盘驱动器    
   返回0 为成功  返回1 为失败                                       */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (void)
{
	DSTATUS stat;

	// Put your code here	   把你的代码写在这里
	 stat=TFsdio_Init();	  //初始化SD卡 并将返回值给stat


	return stat;
}



/*-----------------------------------------------------------------------*/
/* Read Partial Sector
   读扇区                                                   */
/*-----------------------------------------------------------------------*/

DRESULT disk_readp (
	BYTE* dest,			/* Pointer to the destination object 存放数据的地址*/
	DWORD sector,		/* Sector number (LBA)  要读的扇区数*/
	WORD sofs,			/* Offset in the sector 扇区偏移值*/
	WORD count			/* Byte count (bit15:destination) 读出的数据个数*/
)
{
	DRESULT res;

    // Put your code here	 把你的代码写在这里

	u16 i;
	u8 r1;
    res=RES_ERROR;

	
	r1=SD_ReadDisk(dest_tbuf,sector,1);//直接读取一个扇区所有数据

	if(r1==0)//指令发送成功
	 {
//			 if(sofs)  //如果有偏移量
//			 {		  
//			 for(i=0;i<sofs;i++)Moni_SPI_ReadWriteByte(0xff);  //将偏移值循环掉
			 			      
			 for(i=0;i<count;i++) *(dest++)=dest_tbuf[sofs++];  //接收要读出来的数据		   
//			 }
//			 else   //没有偏移值 即偏移值为0
//			 {
//			   //要读的数据个数复制给tmp
//			   for(i=0;i<count;i++) *(dest++)=Moni_SPI_ReadWriteByte(0xff);  //接收要读出来的数据
//			 }
//			 tmp=512-sofs-count;  //剩余为读数据   扇区数-偏移值-读取数据
//			 for(i=0;i<tmp;i++)Moni_SPI_ReadWriteByte(0xff);// 将剩余数据循环掉 以免对其他函数有干扰
//	

	 }
	else  return res;	 //读指令失败

	res=RES_OK;	   //数据读取成功
	return res;


//	u16 tmp,i;
//	u8 r1;
//    res=RES_ERROR;
//
//	r1=SD_SendCmd(CMD17,sector<<9,0X01);//读命令
//	
//	if(r1==0)//指令发送成功
//		{
//		    while(Moni_SPI_ReadWriteByte(0xff)!=0xfe);  //等待接收数据
//			 if(sofs)  //如果有偏移量
//			 {		  
//			 for(i=0;i<sofs;i++)Moni_SPI_ReadWriteByte(0xff);  //将偏移值循环掉
//			 			      
//			 for(i=0;i<count;i++) *(dest++)=Moni_SPI_ReadWriteByte(0xff);  //接收要读出来的数据		   
//			 }
//			 else   //没有偏移值 即偏移值为0
//			 {
//			   //要读的数据个数复制给tmp
//			   for(i=0;i<count;i++) *(dest++)=Moni_SPI_ReadWriteByte(0xff);  //接收要读出来的数据
//			 }
//			 tmp=512-sofs-count;  //剩余为读数据   扇区数-偏移值-读取数据
//			 for(i=0;i<tmp;i++)Moni_SPI_ReadWriteByte(0xff);// 将剩余数据循环掉 以免对其他函数有干扰
//
//
//			//下面是2个伪CRC（dummy CRC）
//			  Moni_SPI_ReadWriteByte(0xff); 
//              Moni_SPI_ReadWriteByte(0xff); 		   
//		}
//	  else  return res;	 //读指令失败
//
//	SD_DisSelect();//取消片选
//	res=RES_OK;	   //数据读取成功
//	return res;

//	u8 res1=0; 
//    if (!count)return RES_PARERR;//count不能等于0，否则返回参数错误		 	 
////	switch(pdrv)
////	{
////		case SD_CARD://SD卡
//			res1=SD_ReadDisk(dest,sector,count);	 
//			while(res1)//读出错
//			{
//				SD_Init();	//重新初始化SD卡
//				res1=SD_ReadDisk(dest,sector,count);	
//				//printf("sd rd error:%d\r\n",res);
//			}
////			break;
////		case EX_FLASH://外部flash
////			for(;count>0;count--)
////			{
////				W25QXX_Read(buff,sector*FLASH_SECTOR_SIZE,FLASH_SECTOR_SIZE);
////				sector++;
////				buff+=FLASH_SECTOR_SIZE;
////			}
////			res=0;
////			break;
////		default:
////			res=1; 
////	}
//   //处理返回值，将SPI_SD_driver.c的返回值转成ff.c的返回值
//    if(res1==0x00)return RES_OK;	 
//    else return RES_ERROR;	 


}





/*-----------------------------------------------------------------------*/
/* Write Partial Sector                                                  */
/*-----------------------------------------------------------------------*/

DRESULT disk_writep (
const	BYTE* buff,		/* Pointer to the data to be written, NULL:Initiate/Finalize write operation */
	DWORD sc		/* Sector number (LBA) or Number of bytes to send */
)
{	
	DRESULT res;
	u8 res1=0; 
	res = RES_ERROR;

	if (!buff) {
		if (sc) {

			// Initiate write process

			res=RES_OK;

		} else {

			// Finalize write process
			res=RES_OK;
		}
	} else {

		// Send data to the disk

			  res1=SD_WriteDisk((u8*)buff,sc,1);
//			while(res1)//写出错
//			{
//				TFsdio_Init();	//重新初始化SD卡
//				res1=SD_WriteDisk((u8*)buff,sc,1);	
//				//printf("sd wr error:%d\r\n",res);
//			}
		    if(res1 == 0x00)res=RES_OK;	 
		    else res= RES_ERROR;	
	}

	return res;
}



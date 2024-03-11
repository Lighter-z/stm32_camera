/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for Petit FatFs (C)ChaN, 2009   
   �Ͷ˵Ĵ���I/Oģ��Ǽ�Petit FatFs��C����2009   */
/*-----------------------------------------------------------------------*/

#include "diskio.h"
#include "mcusys.h"
#include "spi.h"

#include "sdio_sdcard.h"


u8 dest_tbuf[512];

/*-----------------------------------------------------------------------*/
/* Initialize Disk Drive      
   ��ʼ������������    
   ����0 Ϊ�ɹ�  ����1 Ϊʧ��                                       */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (void)
{
	DSTATUS stat;

	// Put your code here	   ����Ĵ���д������
	 stat=TFsdio_Init();	  //��ʼ��SD�� ��������ֵ��stat


	return stat;
}



/*-----------------------------------------------------------------------*/
/* Read Partial Sector
   ������                                                   */
/*-----------------------------------------------------------------------*/

DRESULT disk_readp (
	BYTE* dest,			/* Pointer to the destination object ������ݵĵ�ַ*/
	DWORD sector,		/* Sector number (LBA)  Ҫ����������*/
	WORD sofs,			/* Offset in the sector ����ƫ��ֵ*/
	WORD count			/* Byte count (bit15:destination) ���������ݸ���*/
)
{
	DRESULT res;

    // Put your code here	 ����Ĵ���д������

	u16 i;
	u8 r1;
    res=RES_ERROR;

	
	r1=SD_ReadDisk(dest_tbuf,sector,1);//ֱ�Ӷ�ȡһ��������������

	if(r1==0)//ָ��ͳɹ�
	 {
//			 if(sofs)  //�����ƫ����
//			 {		  
//			 for(i=0;i<sofs;i++)Moni_SPI_ReadWriteByte(0xff);  //��ƫ��ֵѭ����
			 			      
			 for(i=0;i<count;i++) *(dest++)=dest_tbuf[sofs++];  //����Ҫ������������		   
//			 }
//			 else   //û��ƫ��ֵ ��ƫ��ֵΪ0
//			 {
//			   //Ҫ�������ݸ������Ƹ�tmp
//			   for(i=0;i<count;i++) *(dest++)=Moni_SPI_ReadWriteByte(0xff);  //����Ҫ������������
//			 }
//			 tmp=512-sofs-count;  //ʣ��Ϊ������   ������-ƫ��ֵ-��ȡ����
//			 for(i=0;i<tmp;i++)Moni_SPI_ReadWriteByte(0xff);// ��ʣ������ѭ���� ��������������и���
//	

	 }
	else  return res;	 //��ָ��ʧ��

	res=RES_OK;	   //���ݶ�ȡ�ɹ�
	return res;


//	u16 tmp,i;
//	u8 r1;
//    res=RES_ERROR;
//
//	r1=SD_SendCmd(CMD17,sector<<9,0X01);//������
//	
//	if(r1==0)//ָ��ͳɹ�
//		{
//		    while(Moni_SPI_ReadWriteByte(0xff)!=0xfe);  //�ȴ���������
//			 if(sofs)  //�����ƫ����
//			 {		  
//			 for(i=0;i<sofs;i++)Moni_SPI_ReadWriteByte(0xff);  //��ƫ��ֵѭ����
//			 			      
//			 for(i=0;i<count;i++) *(dest++)=Moni_SPI_ReadWriteByte(0xff);  //����Ҫ������������		   
//			 }
//			 else   //û��ƫ��ֵ ��ƫ��ֵΪ0
//			 {
//			   //Ҫ�������ݸ������Ƹ�tmp
//			   for(i=0;i<count;i++) *(dest++)=Moni_SPI_ReadWriteByte(0xff);  //����Ҫ������������
//			 }
//			 tmp=512-sofs-count;  //ʣ��Ϊ������   ������-ƫ��ֵ-��ȡ����
//			 for(i=0;i<tmp;i++)Moni_SPI_ReadWriteByte(0xff);// ��ʣ������ѭ���� ��������������и���
//
//
//			//������2��αCRC��dummy CRC��
//			  Moni_SPI_ReadWriteByte(0xff); 
//              Moni_SPI_ReadWriteByte(0xff); 		   
//		}
//	  else  return res;	 //��ָ��ʧ��
//
//	SD_DisSelect();//ȡ��Ƭѡ
//	res=RES_OK;	   //���ݶ�ȡ�ɹ�
//	return res;

//	u8 res1=0; 
//    if (!count)return RES_PARERR;//count���ܵ���0�����򷵻ز�������		 	 
////	switch(pdrv)
////	{
////		case SD_CARD://SD��
//			res1=SD_ReadDisk(dest,sector,count);	 
//			while(res1)//������
//			{
//				SD_Init();	//���³�ʼ��SD��
//				res1=SD_ReadDisk(dest,sector,count);	
//				//printf("sd rd error:%d\r\n",res);
//			}
////			break;
////		case EX_FLASH://�ⲿflash
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
//   //������ֵ����SPI_SD_driver.c�ķ���ֵת��ff.c�ķ���ֵ
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
//			while(res1)//д����
//			{
//				TFsdio_Init();	//���³�ʼ��SD��
//				res1=SD_WriteDisk((u8*)buff,sc,1);	
//				//printf("sd wr error:%d\r\n",res);
//			}
		    if(res1 == 0x00)res=RES_OK;	 
		    else res= RES_ERROR;	
	}

	return res;
}



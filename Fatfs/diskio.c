
//需要修改，接口层文件

/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2016        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "diskio.h"		/* FatFs lower layer API */
#include "sd_card.h"

/* Definitions of physical drive number for each drive */
#define SD_CARD 0	//SD卡，卷标




/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
	return 0;
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
	u8 stat=0;
	//int result;
	
	switch (pdrv) {
	case SD_CARD :
		stat = SD_Init(SDIO_INTPRIO_CFG);
		break;
	default:
		stat=1;
		break;
	}
	if(stat) return STA_NOINIT;
	else return 0;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Start sector in LBA */
	UINT count		/* Number of sectors to read */
)
{
	u8 res;
	//int result;
	if (!count)return RES_PARERR;//count不能等于0，否则返回参数错误		 	 
	switch (pdrv) {
	case SD_CARD :
		res = SD_ReadDisk(buff, sector, count);
		while(res)
		{
			SD_Init(SDIO_INTPRIO_CFG);
			res=SD_ReadDisk(buff, sector, count);
		}
		break;
	default:
		res=1;
		break;
	}
	if(res==0x00) return RES_OK;
	else return RES_ERROR;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Start sector in LBA */
	UINT count			/* Number of sectors to write */
)
{
	u8 res;
	//int result;
	if (!count)return RES_PARERR;//count不能等于0，否则返回参数错误		 	 
	switch (pdrv) {
	case SD_CARD :
		res = SD_WriteDisk((u8*)buff, sector, count);
		while(res)
		{
			SD_Init(SDIO_INTPRIO_CFG);
			res=SD_WriteDisk((u8*)buff, sector, count);
		}
		break;
	default:
		res=1;
		break;
	}
	if(res==0x00) return RES_OK;
	else return RES_ERROR;
}



/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	DRESULT res;
	
	switch (pdrv) {
	case SD_CARD :
		switch(cmd)
		{
			case CTRL_SYNC:
				res=RES_OK;
				break;
			case GET_SECTOR_SIZE:
				*(DWORD*)buff=512;
				res=RES_OK;
				break;
			case GET_BLOCK_SIZE:
				*(DWORD*)buff=SDCardInfo.CardBlockSize;
				res=RES_OK;
				break;
			case GET_SECTOR_COUNT:
				*(DWORD*)buff=SDCardInfo.CardCapacity/512;
				res=RES_OK;
				break;
			default:
				res=RES_PARERR;
				break;
		}
		break;
	default:
		res=RES_PARERR;
		break;
	}
	return res;
}

DWORD get_fattime(void)
{
	return 0;
}

/*void *ff_memalloc(UINT size)
{
	
}

void ff_memfree(void *mf)
{
	
}*/



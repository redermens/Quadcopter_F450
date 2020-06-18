#include "fat_func.h"
#include "ff.h"
#include "stm32f4xx.h"

BYTE mkfs_work[_MAX_SS];

//检查格式化,0，无需格式化；1，格式化成功；2，格式化失败
unsigned char exf_ifmkfs(u8 in_res)
{
	u8 res;
	if(in_res==0x0D)
	{
		//CmdPrintln("SD Disk Formatting...");
		res=f_mkfs("0:",FM_ANY,1024,mkfs_work,sizeof mkfs_work);
		if(res==0)
		{
			f_setlabel((const TCHAR *)"0:CamSD");
			//CmdPrintln("SD Disk Format Finish");
		}
		else	//error
		{
			//CmdPrintln("SD Disk Format Error:%d",res);
		}
	}
	return 0;
}

/*
得到磁盘剩余容量
drv:磁盘编号("0:"/"1:")
total:总容量	 （单位KB）
free:剩余容量	 （单位KB）
返回值:0,正常.其他,错误代码
*/
unsigned char exf_getfree(TCHAR *drv,u32 *total,u32 *free)
{
	FATFS *fs1;
	u8 res;
    u32 fre_clust=0, fre_sect=0, tot_sect=0;
    //得到磁盘信息及空闲簇数量
    res =(u32)f_getfree((const TCHAR*)drv, (DWORD*)&fre_clust, &fs1);
    if(res==0)
	{
	    tot_sect=(fs1->n_fatent-2)*fs1->csize;	//得到总扇区数
	    fre_sect=fre_clust*fs1->csize;			//得到空闲扇区数	   
#if _MAX_SS!=512				  				//扇区大小不是512字节,则转换为512字节
		tot_sect*=fs1->ssize/512;
		fre_sect*=fs1->ssize/512;
#endif	  
		*total=tot_sect>>1;	//单位为KB
		*free=fre_sect>>1;	//单位为KB 
 	}
	return res;
}


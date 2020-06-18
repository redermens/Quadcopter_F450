#ifndef FAT_FUNC_H
#define FAT_FUNC_H

#include "ff.h"
#include "stm32f4xx.h"

unsigned char exf_getfree(TCHAR *drv,u32 *total,u32 *free);
unsigned char exf_ifmkfs(u8 in_res);

#endif

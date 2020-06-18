#ifndef __SRAM_H
#define __SRAM_H															    

void FSMC_SRAM_Init(void);
void FSMC_SRAM_WriteBuffer16(unsigned short *pBuffer,unsigned int WriteAddr16,unsigned int n);
void FSMC_SRAM_ReadBuffer16(unsigned short* pBuffer,unsigned int ReadAddr16,unsigned int n);
int FSMC_SRAM_check51216(void);

#endif


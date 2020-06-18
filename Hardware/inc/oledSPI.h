#ifndef __OLEDSPI_H
#define __OLEDSPI_H

#include "cfg_func_f407.h"	

#define OLED_PutString  LCD_P6x8Str
#define OLED_Clear      LCD_CLS


#define OLED_PIN_CK   GPIOE,GPIO_Pin_15
#define OLED_PIN_DI   GPIOE,GPIO_Pin_14
#define OLED_PIN_RES  GPIOE,GPIO_Pin_13
#define OLED_PIN_DC   GPIOE,GPIO_Pin_12
#define OLED_PIN_CS   GPIOE,GPIO_Pin_11

#define OLEDSPI_CK_H 	GPIO_SetBits(OLED_PIN_CK)
#define OLEDSPI_CK_L 	GPIO_ResetBits(OLED_PIN_CK)
#define OLEDSPI_DI_H 	GPIO_SetBits(OLED_PIN_DI)
#define OLEDSPI_DI_L 	GPIO_ResetBits(OLED_PIN_DI)
#define OLEDSPI_RST_H   GPIO_SetBits(OLED_PIN_RES)
#define OLEDSPI_RST_L	GPIO_ResetBits(OLED_PIN_RES)
#define OLEDSPI_DC_H 	GPIO_SetBits(OLED_PIN_DC)
#define OLEDSPI_DC_L 	GPIO_ResetBits(OLED_PIN_DC)
#define OLEDSPI_CS_H 	GPIO_SetBits(OLED_PIN_CS)
#define OLEDSPI_CS_L 	GPIO_ResetBits(OLED_PIN_CS)

#define OLED_INIT_CK  GPIO_Init_OutPP(OLED_PIN_CK)
#define OLED_INIT_DI  GPIO_Init_OutPP(OLED_PIN_DI)
#define OLED_INIT_RES GPIO_Init_OutPP(OLED_PIN_RES)
#define OLED_INIT_DC  GPIO_Init_OutPP(OLED_PIN_DC)
#define OLED_INIT_CS  GPIO_Init_OutPP(OLED_PIN_CS)

void OLED_GPIO_init(void);
void OLED_Init(void);
void LCD_CLS(void);
void LCD_P6x8Str(unsigned char x,unsigned char y,unsigned char ch[]);
void LCD_P8x16Str(unsigned char x,unsigned char y,unsigned char ch[]);
void LCD_P14x16Str(unsigned char x,unsigned char y,unsigned char ch[]);
void LCD_PrintValueI(unsigned char x, unsigned char y,int16_t data);
void LCD_PrintValueF(unsigned char x, unsigned char y, float data, unsigned char num);
void LCD_Print(unsigned char x, unsigned char y, unsigned char ch[]);
void LCD_PutPixel(unsigned char x,unsigned char y);
void Draw_BMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char bmp[]); 
void LCD_Fill(unsigned char dat);
void LCD_Set_Pos(unsigned char x, unsigned char y);


void LCD_P6x8char(unsigned char x,unsigned char y,unsigned char ch);



void OLED_DrawPoint(unsigned char x,unsigned char y,unsigned char t);
void OLED_Refresh_Gram(void);
void LED_PrintImage(unsigned char *pucTable, u16 usRowNum, u16 usColumnNum);

#endif


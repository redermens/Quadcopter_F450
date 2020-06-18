#ifndef LCD_H
#define LCD_H

#include "cfg_func_f407.h"

#define RED  	  0xf800
#define GREEN	  0x07e0
#define BLUE 	  0x001f
#define WHITE	  0xffff
#define BLACK  	0x0000
#define YELLOW  0xFFE0
#define GRAY0   0xEF7D
#define GRAY1   0x8410
#define GRAY2   0x4208

void LCD_Init(void);
void LCD_SetRegion(u16 x_start,u16 y_start,u16 x_end,u16 y_end);
void LCD_Clear(unsigned short Color);
void LCD_SetRam(unsigned short *Color,int x,int y,int width,int height);

#define LCD_PIN_CK   GPIOB,GPIO_Pin_10
#define LCD_PIN_CK_S GPIOB,GPIO_PinSource10

#define LCD_PIN_DI   GPIOB,GPIO_Pin_15
#define LCD_PIN_DI_S GPIOB,GPIO_PinSource15

#define LCD_PIN_RES  GPIOC,GPIO_Pin_5
#define LCD_PIN_DC   GPIOC,GPIO_Pin_4
#define LCD_PIN_CS   GPIOB,GPIO_Pin_12

#define LCD_CK_H 	GPIO_SetBits(LCD_PIN_CK)
#define LCD_CK_L 	GPIO_ResetBits(LCD_PIN_CK)
#define LCD_DI_H 	GPIO_SetBits(LCD_PIN_DI)
#define LCD_DI_L 	GPIO_ResetBits(LCD_PIN_DI)
#define LCD_RST_H GPIO_SetBits(LCD_PIN_RES)
#define LCD_RST_L	GPIO_ResetBits(LCD_PIN_RES)
#define LCD_DC_H 	GPIO_SetBits(LCD_PIN_DC)
#define LCD_DC_L 	GPIO_ResetBits(LCD_PIN_DC)
#define LCD_CS_H 	GPIO_SetBits(LCD_PIN_CS)
#define LCD_CS_L 	GPIO_ResetBits(LCD_PIN_CS)

#define LCD_INIT_CK  GPIO_Init_OutPP(LCD_PIN_CK)
#define LCD_INIT_DI  GPIO_Init_OutPP(LCD_PIN_DI)
#define LCD_INIT_RES GPIO_Init_OutPP(LCD_PIN_RES)
#define LCD_INIT_DC  GPIO_Init_OutPP(LCD_PIN_DC)
#define LCD_INIT_CS  GPIO_Init_OutPP(LCD_PIN_CS)

#endif

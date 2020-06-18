#include "lcd.h"
#include "cfg_func_f407.h"

void SPI2_WriteByte(unsigned char TxData)
{		 			 
  while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET){}//等待发送区空  
	SPI_I2S_SendData(SPI2, TxData);   
}

void SPI2_Init(void)
{	 
  SPI_InitTypeDef  SPI_InitStructure;
	
	//RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_CRC, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);//使能SPI2时钟
	
  GPIO_Init_AF(LCD_PIN_CK);
	GPIO_Init_AF(LCD_PIN_DI);
	GPIO_PinAFConfig(LCD_PIN_CK_S,GPIO_AF_SPI2);
	GPIO_PinAFConfig(LCD_PIN_DI_S,GPIO_AF_SPI2);
 
	SPI_I2S_DeInit(SPI2); 
	SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Tx;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		//串行同步时钟的空闲状态为高电平
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;	//串行同步时钟的第二个跳变沿（上升或下降）数据被采样
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;		//定义波特率预分频的值:波特率预分频值为256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRC值计算的多项式
	SPI_Init(SPI2, &SPI_InitStructure);  //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器
 
	SPI_Cmd(SPI2, ENABLE); //使能SPI外设
}



void Lcd_WriteIndex(unsigned char Data)
{
	unsigned int i=0;
	for(i=0;i<50;i++);
	LCD_DC_L;
	SPI2_WriteByte(Data);
}

void Lcd_WriteData(unsigned char Data)
{
	unsigned int i=0;
	for(i=0;i<50;i++);
	LCD_DC_H;
	SPI2_WriteByte(Data);
}

void LCD_WriteData_16Bit(unsigned short Data)
{
	unsigned char r8;
	
	for(r8=0;r8<8;r8++);
	LCD_DC_H;
	SPI2_WriteByte(Data>>8);
	SPI2_WriteByte(Data);
}

void Lcd_WriteReg(unsigned char Index,unsigned char Data)
{
	Lcd_WriteIndex(Index);
  Lcd_WriteData(Data);
}

void Lcd_Reset(void)
{
	unsigned long i;
	LCD_RST_L;
	for(i=0;i<1000000;i++); //100ms
	LCD_RST_H;
	for(i=0;i<1000000;i++); //50ms
}

void LCD_Init(void)
{
	unsigned long i;
	
	SPI2_Init();
	//LCD_INIT_CK;
	//LCD_INIT_DI;
	LCD_INIT_RES;
	LCD_INIT_DC;
	LCD_INIT_CS;
	LCD_CS_L;
	
	Lcd_Reset(); //Reset before LCD Init.

	//LCD Init For 1.44Inch LCD Panel with ST7735R.
	Lcd_WriteIndex(0x11);//Sleep exit 
	for(i=0;i<1000000;i++); //120ms
	Lcd_WriteIndex(0x21); 
  Lcd_WriteIndex(0x21); 

  Lcd_WriteIndex(0xB1); 
  Lcd_WriteData(0x05);
  Lcd_WriteData(0x3A);
  Lcd_WriteData(0x3A);

  Lcd_WriteIndex(0xB2);
  Lcd_WriteData(0x05);
  Lcd_WriteData(0x3A);
  Lcd_WriteData(0x3A);

  Lcd_WriteIndex(0xB3); 
  Lcd_WriteData(0x05);  
  Lcd_WriteData(0x3A);
  Lcd_WriteData(0x3A);
  Lcd_WriteData(0x05);
  Lcd_WriteData(0x3A);
  Lcd_WriteData(0x3A);

  Lcd_WriteIndex(0xB4);
  Lcd_WriteData(0x03);

  Lcd_WriteIndex(0xC0);
  Lcd_WriteData(0x62);
  Lcd_WriteData(0x02);
  Lcd_WriteData(0x04);

  Lcd_WriteIndex(0xC1);
  Lcd_WriteData(0xC0);

  Lcd_WriteIndex(0xC2);
  Lcd_WriteData(0x0D);
  Lcd_WriteData(0x00);

  Lcd_WriteIndex(0xC3);
  Lcd_WriteData(0x8D);
  Lcd_WriteData(0x6A);   

  Lcd_WriteIndex(0xC4);
  Lcd_WriteData(0x8D); 
  Lcd_WriteData(0xEE); 

  Lcd_WriteIndex(0xC5);  /*VCOM*/
  Lcd_WriteData(0x0E);    

  Lcd_WriteIndex(0xE0);
  Lcd_WriteData(0x10);
  Lcd_WriteData(0x0E);
  Lcd_WriteData(0x02);
  Lcd_WriteData(0x03);
  Lcd_WriteData(0x0E);
  Lcd_WriteData(0x07);
  Lcd_WriteData(0x02);
  Lcd_WriteData(0x07);
  Lcd_WriteData(0x0A);
  Lcd_WriteData(0x12);
  Lcd_WriteData(0x27);
  Lcd_WriteData(0x37);
  Lcd_WriteData(0x00);
  Lcd_WriteData(0x0D);
  Lcd_WriteData(0x0E);
  Lcd_WriteData(0x10);

  Lcd_WriteIndex(0xE1);
  Lcd_WriteData(0x10);
  Lcd_WriteData(0x0E);
  Lcd_WriteData(0x03);
  Lcd_WriteData(0x03);
  Lcd_WriteData(0x0F);
  Lcd_WriteData(0x06);
  Lcd_WriteData(0x02);
  Lcd_WriteData(0x08);
  Lcd_WriteData(0x0A);
  Lcd_WriteData(0x13);
  Lcd_WriteData(0x26);
  Lcd_WriteData(0x36);
  Lcd_WriteData(0x00);
  Lcd_WriteData(0x0D);
  Lcd_WriteData(0x0E);
  Lcd_WriteData(0x10);

  Lcd_WriteIndex(0x3A); 
  Lcd_WriteData(0x05);

  Lcd_WriteIndex(0x36);
  Lcd_WriteData(0xA8);//

  Lcd_WriteIndex(0x29);  
}

void LCD_SetRegion(u16 x_start,u16 y_start,u16 x_end,u16 y_end)
{		
	Lcd_WriteIndex(0x2a);
	Lcd_WriteData(0x00);
	Lcd_WriteData(x_start+1);
	Lcd_WriteData(0x00);
	Lcd_WriteData(x_end+1);

	Lcd_WriteIndex(0x2b);
	Lcd_WriteData(0x00);
	Lcd_WriteData(y_start+0x1A);
	Lcd_WriteData(0x00);
	Lcd_WriteData(y_end+0x1A);	
	Lcd_WriteIndex(0x2c);

}

void LCD_Clear(unsigned short Color)               
{
  unsigned int i,m;
  LCD_SetRegion(0,0,160-1,80-1);
  Lcd_WriteIndex(0x2C);
	for(i=0;i<80;i++) //h
    for(m=0;m<160;m++)  //l
    {	
	  	LCD_WriteData_16Bit(Color);
    }
}

void LCD_SetRam(unsigned short *Color,int x,int y,int width,int height)        
{
  unsigned int i,m;
  LCD_SetRegion(x,y,x+width-1,y+height-1);
  Lcd_WriteIndex(0x2C);
	for(i=0;i<height;i++) //h
    for(m=0;m<width;m++)  //l
    {	
	  	LCD_WriteData_16Bit(*Color);
			Color++;
    }
}





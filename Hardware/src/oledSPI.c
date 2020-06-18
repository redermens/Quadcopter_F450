
#include "cfg_func_f407.h"
#include "oledSPI.h"

#define XLevelL  0x00
#define XLevelH  0x10
#define XLevel  ((XLevelH&0x0F)*16+XLevelL)
#define Max_Column  128
#define Max_Row     64
#define	Brightness  0xCF 
#define X_WIDTH     128
#define Y_WIDTH     64

unsigned char OLED_GRAM[128][8];

const unsigned char F6X8[][6] =
{
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   // sp
    { 0x00, 0x00, 0x00, 0x2f, 0x00, 0x00 },   // !
    { 0x00, 0x00, 0x07, 0x00, 0x07, 0x00 },   // "
    { 0x00, 0x14, 0x7f, 0x14, 0x7f, 0x14 },   // #
    { 0x00, 0x24, 0x2a, 0x7f, 0x2a, 0x12 },   // $
    { 0x00, 0x62, 0x64, 0x08, 0x13, 0x23 },   // %
    { 0x00, 0x36, 0x49, 0x55, 0x22, 0x50 },   // &
    { 0x00, 0x00, 0x05, 0x03, 0x00, 0x00 },   // '
    { 0x00, 0x00, 0x1c, 0x22, 0x41, 0x00 },   // (
    { 0x00, 0x00, 0x41, 0x22, 0x1c, 0x00 },   // )
    { 0x00, 0x14, 0x08, 0x3E, 0x08, 0x14 },   // *
    { 0x00, 0x08, 0x08, 0x3E, 0x08, 0x08 },   // +
    { 0x00, 0x00, 0x00, 0xA0, 0x60, 0x00 },   // ,
    { 0x00, 0x08, 0x08, 0x08, 0x08, 0x08 },   // -
    { 0x00, 0x00, 0x60, 0x60, 0x00, 0x00 },   // .
    { 0x00, 0x20, 0x10, 0x08, 0x04, 0x02 },   // /
    { 0x00, 0x3E, 0x51, 0x49, 0x45, 0x3E },   // 0
    { 0x00, 0x00, 0x42, 0x7F, 0x40, 0x00 },   // 1
    { 0x00, 0x42, 0x61, 0x51, 0x49, 0x46 },   // 2
    { 0x00, 0x21, 0x41, 0x45, 0x4B, 0x31 },   // 3
    { 0x00, 0x18, 0x14, 0x12, 0x7F, 0x10 },   // 4
    { 0x00, 0x27, 0x45, 0x45, 0x45, 0x39 },   // 5
    { 0x00, 0x3C, 0x4A, 0x49, 0x49, 0x30 },   // 6
    { 0x00, 0x01, 0x71, 0x09, 0x05, 0x03 },   // 7
    { 0x00, 0x36, 0x49, 0x49, 0x49, 0x36 },   // 8
    { 0x00, 0x06, 0x49, 0x49, 0x29, 0x1E },   // 9
    { 0x00, 0x00, 0x36, 0x36, 0x00, 0x00 },   // :
    { 0x00, 0x00, 0x56, 0x36, 0x00, 0x00 },   // ;
    { 0x00, 0x08, 0x14, 0x22, 0x41, 0x00 },   // <
    { 0x00, 0x14, 0x14, 0x14, 0x14, 0x14 },   // =
    { 0x00, 0x00, 0x41, 0x22, 0x14, 0x08 },   // >
    { 0x00, 0x02, 0x01, 0x51, 0x09, 0x06 },   // ?
    { 0x00, 0x32, 0x49, 0x59, 0x51, 0x3E },   // @
    { 0x00, 0x7C, 0x12, 0x11, 0x12, 0x7C },   // A
    { 0x00, 0x7F, 0x49, 0x49, 0x49, 0x36 },   // B
    { 0x00, 0x3E, 0x41, 0x41, 0x41, 0x22 },   // C
    { 0x00, 0x7F, 0x41, 0x41, 0x22, 0x1C },   // D
    { 0x00, 0x7F, 0x49, 0x49, 0x49, 0x41 },   // E
    { 0x00, 0x7F, 0x09, 0x09, 0x09, 0x01 },   // F
    { 0x00, 0x3E, 0x41, 0x49, 0x49, 0x7A },   // G
    { 0x00, 0x7F, 0x08, 0x08, 0x08, 0x7F },   // H
    { 0x00, 0x00, 0x41, 0x7F, 0x41, 0x00 },   // I
    { 0x00, 0x20, 0x40, 0x41, 0x3F, 0x01 },   // J
    { 0x00, 0x7F, 0x08, 0x14, 0x22, 0x41 },   // K
    { 0x00, 0x7F, 0x40, 0x40, 0x40, 0x40 },   // L
    { 0x00, 0x7F, 0x02, 0x0C, 0x02, 0x7F },   // M
    { 0x00, 0x7F, 0x04, 0x08, 0x10, 0x7F },   // N
    { 0x00, 0x3E, 0x41, 0x41, 0x41, 0x3E },   // O
    { 0x00, 0x7F, 0x09, 0x09, 0x09, 0x06 },   // P
    { 0x00, 0x3E, 0x41, 0x51, 0x21, 0x5E },   // Q
    { 0x00, 0x7F, 0x09, 0x19, 0x29, 0x46 },   // R
    { 0x00, 0x46, 0x49, 0x49, 0x49, 0x31 },   // S
    { 0x00, 0x01, 0x01, 0x7F, 0x01, 0x01 },   // T
    { 0x00, 0x3F, 0x40, 0x40, 0x40, 0x3F },   // U
    { 0x00, 0x1F, 0x20, 0x40, 0x20, 0x1F },   // V
    { 0x00, 0x3F, 0x40, 0x38, 0x40, 0x3F },   // W
    { 0x00, 0x63, 0x14, 0x08, 0x14, 0x63 },   // X
    { 0x00, 0x07, 0x08, 0x70, 0x08, 0x07 },   // Y
    { 0x00, 0x61, 0x51, 0x49, 0x45, 0x43 },   // Z
    { 0x00, 0x00, 0x7F, 0x41, 0x41, 0x00 },   // [
    { 0x00, 0x55, 0x2A, 0x55, 0x2A, 0x55 },   // 55
    { 0x00, 0x00, 0x41, 0x41, 0x7F, 0x00 },   // ]
    { 0x00, 0x04, 0x02, 0x01, 0x02, 0x04 },   // ^
    { 0x00, 0x40, 0x40, 0x40, 0x40, 0x40 },   // _
    { 0x00, 0x00, 0x01, 0x02, 0x04, 0x00 },   // '
    { 0x00, 0x20, 0x54, 0x54, 0x54, 0x78 },   // a
    { 0x00, 0x7F, 0x48, 0x44, 0x44, 0x38 },   // b
    { 0x00, 0x38, 0x44, 0x44, 0x44, 0x20 },   // c
    { 0x00, 0x38, 0x44, 0x44, 0x48, 0x7F },   // d
    { 0x00, 0x38, 0x54, 0x54, 0x54, 0x18 },   // e
    { 0x00, 0x08, 0x7E, 0x09, 0x01, 0x02 },   // f
    { 0x00, 0x18, 0xA4, 0xA4, 0xA4, 0x7C },   // g
    { 0x00, 0x7F, 0x08, 0x04, 0x04, 0x78 },   // h
    { 0x00, 0x00, 0x44, 0x7D, 0x40, 0x00 },   // i
    { 0x00, 0x40, 0x80, 0x84, 0x7D, 0x00 },   // j
    { 0x00, 0x7F, 0x10, 0x28, 0x44, 0x00 },   // k
    { 0x00, 0x00, 0x41, 0x7F, 0x40, 0x00 },   // l
    { 0x00, 0x7C, 0x04, 0x18, 0x04, 0x78 },   // m
    { 0x00, 0x7C, 0x08, 0x04, 0x04, 0x78 },   // n
    { 0x00, 0x38, 0x44, 0x44, 0x44, 0x38 },   // o
    { 0x00, 0xFC, 0x24, 0x24, 0x24, 0x18 },   // p
    { 0x00, 0x18, 0x24, 0x24, 0x18, 0xFC },   // q
    { 0x00, 0x7C, 0x08, 0x04, 0x04, 0x08 },   // r
    { 0x00, 0x48, 0x54, 0x54, 0x54, 0x20 },   // s
    { 0x00, 0x04, 0x3F, 0x44, 0x40, 0x20 },   // t
    { 0x00, 0x3C, 0x40, 0x40, 0x20, 0x7C },   // u
    { 0x00, 0x1C, 0x20, 0x40, 0x20, 0x1C },   // v
    { 0x00, 0x3C, 0x40, 0x30, 0x40, 0x3C },   // w
    { 0x00, 0x44, 0x28, 0x10, 0x28, 0x44 },   // x
    { 0x00, 0x1C, 0xA0, 0xA0, 0xA0, 0x7C },   // y
    { 0x00, 0x44, 0x64, 0x54, 0x4C, 0x44 },   // z
    { 0x14, 0x14, 0x14, 0x14, 0x14, 0x14 }    // horiz lines
};

void OLED_GPIO_init()
{
	OLED_INIT_CK;
	OLED_INIT_DI;
	OLED_INIT_RES;
	OLED_INIT_DC;
	OLED_INIT_CS;
	OLEDSPI_CS_L;
}

void oled_Delay()
{
	unsigned int t;
	for(t=0;t<5;t++);
}

void LCD_WrDat(unsigned char data)
{
	unsigned char i=8;
	OLEDSPI_DC_H;
	oled_Delay(); 
	OLEDSPI_CK_L;	 
	oled_Delay(); 
	oled_Delay();
	oled_Delay();   
	while(i--)
	{
		if(data&0x80){
			OLEDSPI_DI_H;
		}
		else{
			OLEDSPI_DI_L;
		}
		oled_Delay();
		OLEDSPI_CK_H; 
		oled_Delay(); 
		oled_Delay();
		oled_Delay();          
		OLEDSPI_CK_L;
		oled_Delay();    
		data<<=1;
		oled_Delay();    
	}
}

void LCD_WrCmd(unsigned char cmd)
{
	unsigned char i=8;
	OLEDSPI_DC_L;
	oled_Delay();
	OLEDSPI_CK_L;
	oled_Delay();
	oled_Delay(); 
	oled_Delay();    
	while(i--)
	{
		if(cmd&0x80){
			OLEDSPI_DI_H;
		}
		else{
			OLEDSPI_DI_L;
		}
		oled_Delay();
		OLEDSPI_CK_H;
		oled_Delay();
		oled_Delay();
		oled_Delay();             
		OLEDSPI_CK_L;
		oled_Delay();    
		cmd<<=1;
		oled_Delay();  
	} 	
}

void OLED_Init(void)
{
	OLED_GPIO_init();
	OLEDSPI_CK_H;   	
	
	/*RST复位*/	
	OLEDSPI_RST_L;
	oled_Delay();
	oled_Delay();
	oled_Delay();
	oled_Delay();
	oled_Delay();
	OLEDSPI_RST_H;
	
	LCD_WrCmd(0xae);//--turn off oled panel
	LCD_WrCmd(0x00);//---set low column address
	LCD_WrCmd(0x10);//---set high column address
	LCD_WrCmd(0x40);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	LCD_WrCmd(0x81);//--set contrast control register
	LCD_WrCmd(0xcf);// Set SEG Output Current Brightness
	LCD_WrCmd(0xa1);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
	LCD_WrCmd(0xc8);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
	LCD_WrCmd(0xa6);//--set normal display
	LCD_WrCmd(0xa8);//--set multiplex ratio(1 to 64)
	LCD_WrCmd(0x3f);//--1/64 duty
	LCD_WrCmd(0xd3);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	LCD_WrCmd(0x00);//-not offset
	LCD_WrCmd(0xd5);//--set display clock divide ratio/oscillator frequency
	LCD_WrCmd(0x80);//--set divide ratio, Set Clock as 100 Frames/Sec
	LCD_WrCmd(0xd9);//--set pre-charge period
	LCD_WrCmd(0xf1);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	LCD_WrCmd(0xda);//--set com pins hardware configuration
	LCD_WrCmd(0x12);
	LCD_WrCmd(0xdb);//--set vcomh
	LCD_WrCmd(0x40);//Set VCOM Deselect Level
	LCD_WrCmd(0x20);//-Set Page Addressing Mode (0x00/0x01/0x02)
	LCD_WrCmd(0x02);//
	LCD_WrCmd(0x8d);//--set Charge Pump enable/disable
	LCD_WrCmd(0x14);//--set(0x10) disable
	LCD_WrCmd(0xa4);// Disable Entire Display On (0xa4/0xa5)
	LCD_WrCmd(0xa6);// Disable Inverse Display On (0xa6/a7) 
	LCD_WrCmd(0xaf);//--turn on oled panel
	LCD_Fill(0x00);  //初始清屏
	LCD_Set_Pos(0,0);  	
	LCD_CLS();
} 

void LCD_Set_Pos(unsigned char x, unsigned char y)
{ 
  LCD_WrCmd(0xb0+y);
  LCD_WrCmd(((x&0xf0)>>4)|0x10);
  LCD_WrCmd((x&0x0f)|0x01); 
} 

void LCD_Fill(unsigned char bmp_data)
{
	unsigned char y,x;
	for(y=0;y<8;y++)
	{
		LCD_WrCmd(0xb0+y);
		LCD_WrCmd(0x01);
		LCD_WrCmd(0x10);
		for(x=0;x<X_WIDTH;x++)
			LCD_WrDat(bmp_data);
	}
}

void LCD_CLS(void)
{
	unsigned char y,x;	
	for(y=0;y<8;y++)
	{
		LCD_WrCmd(0xb0+y);
		LCD_WrCmd(0x01);
		LCD_WrCmd(0x10); 
		for(x=0;x<X_WIDTH;x++)
			LCD_WrDat(0);
	}
}

void LCD_PutPixel(unsigned char x,unsigned char y)
{
	unsigned char data1;  //data1当前点的数据  
  LCD_Set_Pos(x,y); 
	data1 = 0x01<<(y%8); 	
	LCD_WrCmd(0xb0+(y>>3));
	LCD_WrCmd(((x&0xf0)>>4)|0x10);
	LCD_WrCmd((x&0x0f)|0x00);
	LCD_WrDat(data1); 	 	
}

void LCD_P6x8Str(unsigned char x,unsigned char y,unsigned char ch[])
{
  unsigned char c=0,i=0,j=0; 
  while (ch[j]!='\0')
  {    
    c =ch[j]-32;
    if(x>126){x=0;y++;}
    LCD_Set_Pos(x,y);    
  	for(i=0;i<6;i++)     
  		LCD_WrDat(F6X8[c][i]);  
  	x+=6;
  	j++;
  }
}

void LCD_P6x8char(unsigned char x,unsigned char y,unsigned char ch)
{
	unsigned char i,c;
  c =ch-32;
    if(x>126){x=0;y++;}
    LCD_Set_Pos(x,y);    
  	for(i=0;i<6;i++)     
  		LCD_WrDat(F6X8[c][i]);  
  	x+=6;
}

void OLED_DrawPoint(unsigned char x,unsigned char y,unsigned char t)
{
    u8 pos,bx,temp=0;
    if(x>127||y>63)return;//超出范围了.
    pos=7-y/8;
    bx=y%8;
    temp=1<<(7-bx);
    if(t)OLED_GRAM[x][pos]|=temp;
    else OLED_GRAM[x][pos]&=~temp;   
	//OLED_Refresh_Gram();  
}

void OLED_Refresh_Gram(void)
{
    u8 i,n;        
    for(i=0;i<8;i++) 
    {
        LCD_WrCmd(0xb0+i);    //设置页地址（0~7）
        LCD_WrCmd(0x00);      //设置显示位置—列低地址
        LCD_WrCmd(0x10);      //设置显示位置—列高地址  
        for(n=0;n<128;n++) LCD_WrDat(OLED_GRAM[n][i]);
    }  
}
/*
* @func 打印图像
* @para pucTable(uint8_t*) 图像地址
* @para usRowNum(uint8_t*) 行数
* @para usColumnNum(uint8_t*) 列数
*/
void LED_PrintImage(unsigned char *pucTable, u16 usRowNum, u16 usColumnNum)
{
	u8 ucData;
	u16 i, j, k, m, n;
	u16 usRowTmp;

	m = usRowNum >> 3;   //计算图片行数以8位为一组完整的组数
	n = usRowNum % 8;    //计算分完组后剩下的行数

	for (i = 0; i < m; i++) //完整组行扫描
	{
		LCD_Set_Pos(0, (u8)i);
		usRowTmp = i << 3;    //计算当前所在行的下标
		for (j = 0; j < usColumnNum; j++) //列扫描
		{
			ucData = 0;
			for (k = 0; k < 8; k++) //在i组中对这8行扫描
			{
				ucData = ucData >> 1;
				if ((pucTable + (usRowTmp + k) * usColumnNum)[j] == 1)
				{
					ucData = ucData | 0x80;
				}

			}
			LCD_WrDat(ucData);
		}
	}

	LCD_Set_Pos(0, (u8)i); //设置剩下的行显示的起始坐标
	usRowTmp = i << 3;       //计算当前所在行的下标
	for (j = 0; j < usColumnNum; j++) //列扫描
	{
		ucData = 0;
		for (k = 0; k < n; k++) //对剩下的行扫描
		{
			ucData = ucData >> 1;
			if ((pucTable + (usRowTmp + k) * usColumnNum)[j] == 1)
			{
				ucData = ucData | 0x80;
			}

		}
		ucData = ucData >> (8 - n);
		LCD_WrDat(ucData);
	}
	return;
}
//==============================================================
//函数名： void Draw_BMP(byte x,byte y)
//功能描述：显示BMP图片128×64
//参数：起始点坐标(x,y),x的范围0～127，y为页的范围0～7
//返回：无
//==============================================================
void Draw_BMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char bmp[])
{ 	
  unsigned int ii=0;
  unsigned char x,y;
  if(y1%8==0) y=y1/8;      
  else y=y1/8+1;
	for(y=y0;y<=y1;y++)
	{
		LCD_Set_Pos(x0,y);				
    for(x=x0;x<x1;x++)
	    {      
	    	LCD_WrDat(bmp[ii++]);	    	
	    }
	}
}


void LCD_PrintValueI(unsigned char x, unsigned char y,int16_t data)
{
	unsigned char i,j,k,l,m;
	if(data < 0)
	{
		LCD_P6x8char(x,y,'-');
		data = - data;
	}
	else
	{
		LCD_P6x8char(x,y,'+');
	}

	l  = data/10000;
	m= (data%10000)/1000;
	i = (data%1000)/100;
	j = (data%100)/10;
	k = data%10;
	LCD_P6x8char(x+6,y,l+48);
	LCD_P6x8char(x+12,y,m+48);
	LCD_P6x8char(x+18,y,i+48);
	LCD_P6x8char(x+24,y,j+48);
	LCD_P6x8char(x+30,y,k+48);
}

 void LED_PrintValueFP(uint8_t x, uint8_t y, unsigned int data, uint8_t num)
 {
 	uint8_t m,i,j,k;
 	LCD_P6x8char(x, y, '.');
	m= data/1000;
	i = (data%1000)/100;
	j = (data%100)/10;
	k = data%10;
	switch(num)
	{
		case 1:  	LCD_P6x8char(x+6,y,k+48);
				break;
		case 2:  	LCD_P6x8char(x+6,y,j+48);
				LCD_P6x8char(x+12,y,k+48);
				break;
		case 3:	LCD_P6x8char(x+6,y,i+48);
				LCD_P6x8char(x+12,y,j+48);
				LCD_P6x8char(x+18,y,k+48);
				break;
		case 4: 	LCD_P6x8char(x+6,y,m+48);
				LCD_P6x8char(x+12,y,i+48);
				LCD_P6x8char(x+18,y,j+48);
				LCD_P6x8char(x+24,y,k+48);
				break;
	}
 }


 void LCD_PrintValueF(uint8_t x, uint8_t y, float data, uint8_t num)
 {
 	uint8_t l,m,i,j,k;  //万千百十个
 	uint8_t databiti = 6; //整数位数
 	unsigned int tempdataui = 0;
  	int tempdataii = (int)data; //整数部分
 	long int tempdatalp = (long int)((data - (int)data)*10000); //取小数位后4位

 	//整数部分显示
 	if(data < 0.0)  LCD_P6x8char(x, y,'-');
 	else LCD_P6x8char(x, y,'+');
	if(tempdataii < 0)tempdataii = - tempdataii;  //去掉整数部分负号
	tempdataui = tempdataii;
 	l  = tempdataui/10000;
	m= (tempdataui%10000)/1000;
	i = (tempdataui%1000)/100;
	j = (tempdataui%100)/10;
	k = tempdataui%10;
 	if (l != 0)  //五位
 	{
 		LCD_P6x8char(x+6,y,l+48);
 		LCD_P6x8char(x+12,y,m+48);
		LCD_P6x8char(x+18,y,i+48);
		LCD_P6x8char(x+24,y,j+48);
		LCD_P6x8char(x+30,y,k+48);
 	}
 	else if(m != 0) //四位
 	{
 		databiti = 5;
 		LCD_P6x8char(x+6,y,m+48);
 		LCD_P6x8char(x+12,y,i+48);
		LCD_P6x8char(x+18,y,j+48);
		LCD_P6x8char(x+24,y,k+48);
 	}
  	else if(i != 0) //三位
  	{
  		databiti = 4;
  	 	LCD_P6x8char(x+6,y,i+48);
 		LCD_P6x8char(x+12,y,j+48);
		LCD_P6x8char(x+18,y,k+48);
  	}
  	else if(j != 0) //两位
  	{
    		databiti = 3;
  		LCD_P6x8char(x+6,y,j+48);
 		LCD_P6x8char(x+12,y,k+48);
  	}
	else
	{
		databiti = 2;
		LCD_P6x8char(x+6,y,k+48);
	}
 	if(tempdatalp < 0)tempdatalp = - tempdatalp;	//去掉小数部分负号
	switch(num)
	{
		case 0: break;
		case 1:  LED_PrintValueFP(x + databiti * 6, y, (unsigned int)(tempdatalp / 1000),num);break;
		case 2:  LED_PrintValueFP(x + databiti * 6, y, (unsigned int)(tempdatalp / 100),num);break;
		case 3:  LED_PrintValueFP(x + databiti * 6, y, (unsigned int)(tempdatalp / 10),num);break;
		case 4:  LED_PrintValueFP(x + databiti * 6, y, (unsigned int)(tempdatalp),num);break;
	}
 }
 
 
 
 
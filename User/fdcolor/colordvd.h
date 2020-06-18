/*
colordvd.h
用于颜色识别
*/

#ifndef COLORDVD_H
#define COLORDVD_H

//#define BRIGHT_TH	16	//亮度阈值
//#define COLORD_TH	20	//色差阈值

//#define COLOR_RED		250
//#define COLOR_GREEN		175
//#define COLOR_BLUE		130
//#define COLOR_YELLOW	210

#define FD_BLUE			98
#define FD_YELLOW		121
#define FD_GREEN		103
#define FD_RED			114

//在调用color_table_init之后，即可通过rgb值确定颜色
//得到的fd_color为FD_BLUE或FD_YELLOW或FD_GREEN或FD_RED
//color_recg_table[rgb565>>11][(rgb565>>5)&0x3f][rgb565&0x1f]
extern unsigned short color_recg_table[32][64][32];

//颜色表初始化函数，必须在使用之前调用，rad为颜色区分半径默认推荐为30
void color_table_init(unsigned char red,unsigned char green,unsigned char blue,unsigned char yellow,int rad,int minBright,int minColorDif);

//返回颜色逻辑值(0,255) rad为色相区域内半径
//unsigned char color2binary(unsigned char COLOR, unsigned char rad, unsigned char r, unsigned char g, unsigned char b);

int color_get(unsigned char r, unsigned char g, unsigned char b,int minBright,int minColorDif);

unsigned char ColorGetGray(unsigned short rgb565_in);

#endif // !COLORDVD_H

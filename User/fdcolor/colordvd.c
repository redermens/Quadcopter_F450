/*
colordvd.c
用于颜色识别
*/
#include "colordvd.h"
#include <math.h>

unsigned short color_recg_table[32][64][32] __attribute__((at(0X68012c00))); //128KB

int _abs(int in)
{
	return in>0 ? in : -1 * in;
}

//返回0~255颜色角度,-1为无效
int color_get(unsigned char r, unsigned char g, unsigned char b,int minBright,int minColorDif)
{
	int ave ,res;
	double negave,x,y,z,angle;
	ave	= ((int)r + (int)g + (int)b) / 3;
	if (ave < minBright) return -1;
	negave = -(r + g + b) / 3.0;
	x = r + negave;
	y = g + negave;
	z = b + negave;
	angle = acos((2 * x - y - z) / sqrt(6 * (x*x + y * y + z * z)));
	if ((y - z) > 0) angle = 2 * 3.14159265354 - angle;
	if (angle > 6.283) angle = 6.283;
	if (angle < 0) angle = 0;
	res = (int)(angle * 256 / 6.28);
	if (res > 255) res = 255;
	if (res < 0) res = 0;
	ave = (_abs(ave - r) + _abs(ave - g) + _abs(ave - b))/3;
	if (ave < minColorDif) return -1;
	return res;
}

//0识别否，255识别真
unsigned char color2binary(unsigned char COLOR, unsigned char rad, unsigned char r, unsigned char g, unsigned char b,int minBright,int minColorDif)
{
	int t,min;
	unsigned char aa,bb;
  t	= color_get(r,g,b,minBright,minColorDif);
	if (t < 0) return 0;
	min = 0;
	aa = _abs(COLOR - t);
	bb = _abs(COLOR + 255 - t);
	if (aa < bb) min = aa;
	else min = bb;
	if (min > rad) return 0;
	else return 255;
}

void color_table_init(unsigned char red,unsigned char green,unsigned char blue,unsigned char yellow,int rad,int minBright,int minColorDif)
{
	int i,j,k;
	for (i = 0; i<32; i++)
	{
		for (j = 0; j<64; j++)
		{
			for (k = 0; k<32; k++)
			{
				if (color2binary(red, rad, i<<3, j<<2, k<<3,minBright,minColorDif))
					color_recg_table[i][j][k] = FD_RED;
				else if (color2binary(green, rad, i<<3, j<<2, k<<3,minBright,minColorDif))
					color_recg_table[i][j][k] = FD_GREEN;
				else if (color2binary(blue, rad, i<<3, j<<2, k<<3,minBright,minColorDif))
					color_recg_table[i][j][k] = FD_BLUE;
				else if (color2binary(yellow, rad, i<<3, j<<2, k<<3,minBright,minColorDif))
					color_recg_table[i][j][k] = FD_YELLOW;
				else
					color_recg_table[i][j][k] = 0;
			}
		}
	}
}

unsigned char ColorGetGray(unsigned short rgb565_in)
{
	unsigned short tmp;
	unsigned char r,g,b;
	tmp=rgb565_in;
	b=(tmp&0x1f)<<3;
	tmp>>=3;
	g=tmp&0xfc;
	tmp>>=5;
  r=tmp&0xf8;
	return   (r*30 + g*59 + b*11 + 50) / 100;
}



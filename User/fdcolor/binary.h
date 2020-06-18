/*
binary.h
用于二值图像膨胀处理，采用菱形滤波
*/

#ifndef BINARY_H
#define BINARY_H

typedef struct POS {
	int x;
	int y;
}POS;


void binary_map_connected(unsigned char *map_in, const int height, const int width, 
	const int posh, const int posw, unsigned char *map_out);


void binary_edge(unsigned char *map_in, const int height, const int width, unsigned char *map_out);

//二值图膨胀处理，二值图每一个像素一个char，255表示敏感点，0表示暗点，对255处进行膨胀处理，level为膨胀大小等级
//此函数在这里仅支持120*160，若要更改，则需要修改binary.cpp
void binary_expand(unsigned char *map_in, const int height, const int width, const int level);

void binary_expand16(unsigned short *map_in, const int height, const int width, const int level);

void binary_smooth(unsigned char *map_in, const int heigh, const int width, const int level);

#endif

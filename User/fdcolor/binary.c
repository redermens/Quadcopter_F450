/*
binary.cpp
用于二值图像膨胀处理，采用菱形滤波
*/

#include "binary.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned int map_mht[120*160] __attribute__((at(0X68032c00))); //300KB

#define min2(a,b) a < b ? a : b

void binary_fill_edge_255(unsigned char *reco, int height, int width)
{
	int i;
	/* border limited */
	for (i = 0; i<width; i++)
	{
		reco[0 * width + i] = 255;
		reco[(height - 1)*width + i] = 255;
	}
	for (i = 0; i<height; i++)
	{
		reco[i*width + 0] = 255;
		reco[i*width + width - 1] = 255;
	}
}

void binary_fill_edge_255_16(unsigned short *reco, int height, int width)
{
	int i;
	/* border limited */
	for (i = 0; i<width; i++)
	{
		reco[0 * width + i] = 255;
		reco[(height - 1)*width + i] = 255;
	}
	for (i = 0; i<height; i++)
	{
		reco[i*width + 0] = 255;
		reco[i*width + width - 1] = 255;
	}
}


void binary_map_connected(unsigned char *map_in, const int height, const int width,
	const int posh, const int posw, unsigned char *map_out)
{
	unsigned int i, xx, yy;
	int top = -1;
	unsigned int *mStack;
	int xdir[4] = { 1,-1, 0, 0 };
	int ydir[4] = { 0, 0, 1,-1 };
	int hashtab;
	memset(map_out, 0, height*width);
	binary_fill_edge_255(map_out, height, width);
	mStack = (unsigned int*)malloc((height*width) * 2 * sizeof(int));
	/* initial position */
	map_out[posh*width + posw] = 255;
	for (i = 0; i < 4; i++)
	{
		hashtab = (posh + xdir[i])*width + posw + ydir[i];
		if ((map_in[hashtab] == 255) && (map_out[hashtab] == 0))
		{
			top++;
			mStack[top * 2 + 0] = posh + xdir[i];
			mStack[top * 2 + 1] = posw + ydir[i];
		}
	}
	/* width first search */
	while (top >= 0)
	{
		xx = mStack[top * 2 + 0];
		yy = mStack[top * 2 + 1];
		map_out[xx*width + yy] = 255;
		top--;
		for (i = 0; i < 4; i++)
		{
			hashtab = (xx + xdir[i])*width + yy + ydir[i];
			if ((map_in[hashtab] == 255) && (map_out[hashtab] == 0))
			{
				top++;
				mStack[top * 2 + 0] = xx + xdir[i];
				mStack[top * 2 + 1] = yy + ydir[i];
			}
		}
	}
	free(mStack);
}

void binary_edge(unsigned char *map_in, const int height, const int width, unsigned char *map_out)
{
#define EDGE_SENSI 255
	int i, j;
	int inner = EDGE_SENSI, outer = EDGE_SENSI;
	for (i = 0; i<height; i++)
		for (j = 0; j<width; j++)
			map_out[i*width + j] = 0;
	for (i = 1; i<height - 1; i++)
		for (j = 1; j<width - 1; j++)
		{
			inner = EDGE_SENSI;
			outer = EDGE_SENSI;
			if (map_in[(i - 1)*width + j] == 0 || map_in[(i + 1)*width + j] == 0 || map_in[i*width + j - 1] == 0 || map_in[i*width + j + 1] == 0)
				inner = 0;
			if (map_in[(i - 1)*width + j] == EDGE_SENSI || map_in[(i + 1)*width + j] == EDGE_SENSI || map_in[i*width + j - 1] == EDGE_SENSI || map_in[i*width + j + 1] == EDGE_SENSI)
				outer = 0;
			if (inner == 0 && outer == 0 && map_in[i*width + j] == EDGE_SENSI)
				map_out[i*width + j] = EDGE_SENSI;
		}
}

void binary_get_mht(unsigned char *map, unsigned int *map_mht, int heigh, int width)
{
	int i, j;
	for (i = 0; i<heigh; i++) {
		for (j = 0; j<width; j++) {
			if (map[i*width + j]) {
				map_mht[i*width + j] = 0;
			}
			else {
				map_mht[i*width + j] = 9999999;
				if (i>0) map_mht[i*width + j] = min2(map_mht[i*width + j], map_mht[(i - 1)*width + j] + 1);
				if (j>0) map_mht[i*width + j] = min2(map_mht[i*width + j], map_mht[i*width + j - 1] + 1);
			}
		}
	}
	for (i = heigh - 1; i >= 0; i--) {
		for (j = width - 1; j >= 0; j--) {
			if (i + 1<heigh) map_mht[i*width + j] = min2(map_mht[i*width + j], map_mht[(i + 1)*width + j] + 1);
			if (j + 1 < width) map_mht[i*width + j] = min2(map_mht[i*width + j], map_mht[i*width + j + 1] + 1);
		}
	}
}

void binary_get_mht_16(unsigned short *map, unsigned int *map_mht, int heigh, int width)
{
	int i, j;
	for (i = 0; i<heigh; i++) {
		for (j = 0; j<width; j++) {
			if (map[i*width + j]) {
				map_mht[i*width + j] = 0;
			}
			else {
				map_mht[i*width + j] = 99999999;
				if (i>0) map_mht[i*width + j] = min2(map_mht[i*width + j], map_mht[(i - 1)*width + j] + 1);
				if (j>0) map_mht[i*width + j] = min2(map_mht[i*width + j], map_mht[i*width + j - 1] + 1);
			}
		}
	}
	for (i = heigh - 1; i >= 0; i--) {
		for (j = width - 1; j >= 0; j--) {
			if (i + 1<heigh) map_mht[i*width + j] = min2(map_mht[i*width + j], map_mht[(i + 1)*width + j] + 1);
			if (j + 1 < width) map_mht[i*width + j] = min2(map_mht[i*width + j], map_mht[i*width + j + 1] + 1);
		}
	}
}

void binary_expand(unsigned char *map_in, const int height, const int width, const int level)
{
	int i, j;
	int hashtab = width * level;
	//unsigned int *map_mht = (unsigned int*)malloc(height*width * sizeof(int));
	binary_fill_edge_255(map_in, height, width);
	binary_get_mht(map_in, map_mht, height, width);
	for (i = level; i < height - level; i++)
	{
		for (j = level; j < width - level; j++)
		{
			map_in[hashtab + j] = map_mht[hashtab + j] < level ? 255 : 0;
		}
		hashtab += width;
	}
	//free(map_mht);
}

void binary_expand16(unsigned short *map_in, const int height, const int width, const int level)
{
	int i, j;
	int hashtab = width * level;
	//unsigned int *map_mht = (unsigned int*)malloc(height*width * sizeof(int));
	binary_fill_edge_255_16(map_in, height, width);
	binary_get_mht_16(map_in, map_mht, height, width);
	for (i = level; i < height - level; i++)
	{
		for (j = level; j < width - level; j++)
		{
			map_in[hashtab + j] = map_mht[hashtab + j] < level ? 255 : 0;
		}
		hashtab += width;
	}
	//free(map_mht);
}


void binary_smooth(unsigned char *map_in, const int height, const int width, const int level)
{
	int i;
	binary_expand(map_in, height, width, level);
	for (i = 0; i < height*width; i++)	map_in[i] = map_in[i] ? 0 : 255;
	binary_fill_edge_255(map_in, height, width);
	binary_expand(map_in, height, width, level);
	for (i = 0; i < height*width; i++)	map_in[i] = map_in[i] ? 0 : 255;
}

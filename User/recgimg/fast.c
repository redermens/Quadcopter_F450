//#include "stdafx.h"
#include "fast.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


int patternSize = 16;
static const int offsets[][2] =
	{
		{ 0,  3 },{ 1,  3 },{ 2,  2 },{ 3,  1 },{ 3, 0 },{ 3, -1 },{ 2, -2 },{ 1, -3 },
	{ 0, -3 },{ -1, -3 },{ -2, -2 },{ -3, -1 },{ -3, 0 },{ -3,  1 },{ -2,  2 },{ -1,  3 }
	};
	
void makeOffsets(int pixel[25], int rowStride, int patternSize)
{
	int k=0;
	for (; k < patternSize; k++)
		pixel[k] = offsets[k][0] + offsets[k][1] * rowStride;
	for (; k < 25; k++)
		pixel[k] = pixel[k - patternSize];
}


int FAST_t(unsigned char *img, int height, int width, int threshold, int *hOut, int *wOut, int maxnum)
{
	const int K = 16 / 2, N = 16 + 8 + 1;
	int i, j, k, pixel[25],vt,count,x,v,d;
	unsigned char threshold_tab[512];
	int numOfp = 0;
	unsigned char *_buf;
	unsigned char* buf[3];
	int* cpbuf[3];
	int ncorners,score;
	unsigned char* prev;
	unsigned char* pprev;
	unsigned char* ptr;
	unsigned char* curr;
	int* cornerpos;
	unsigned char* tab;
	
	makeOffsets(pixel, (int)width, patternSize);
	if (threshold > 255) threshold = 255;
	if (threshold < 0) threshold = 0;
	//keypoints.clear();
	
	for (i = -255; i <= 255; i++)
		threshold_tab[i + 255] = (unsigned char)(i < -threshold ? 1 : i > threshold ? 2 : 0);
	_buf = (unsigned char*)((width + 16) * 3 * (sizeof(int) + sizeof(unsigned char)) + 128);
	
	buf[0] = _buf; buf[1] = buf[0] + width; buf[2] = buf[1] + width;
	
	cpbuf[0] = (int*)(((size_t)(buf[2] + width) + (int) sizeof(int) - 1) & -(int)sizeof(int)) + 1;
	cpbuf[1] = cpbuf[0] + width + 1;
	cpbuf[2] = cpbuf[1] + width + 1;
	memset(buf[0], 0, width * 3);
	
	
	for (i = 3; i < height - 2; i++)
	{
		ptr = img + i * width + 3;  //img.ptr<uchar>(i) + 3;
		curr = buf[(i - 3) % 3];
		cornerpos = cpbuf[(i - 3) % 3];
		memset(curr, 0, width);
		ncorners = 0;
		if (i < height - 3)
		{
			j = 3;
			for (; j < width - 3; j++, ptr++)
			{
				v = ptr[0];
				tab = &threshold_tab[0] - v + 255;
				d = tab[ptr[pixel[0]]] | tab[ptr[pixel[8]]];
				if (d == 0)
					continue;
				d &= tab[ptr[pixel[2]]] | tab[ptr[pixel[10]]];
				d &= tab[ptr[pixel[4]]] | tab[ptr[pixel[12]]];
				d &= tab[ptr[pixel[6]]] | tab[ptr[pixel[14]]];
				if (d == 0)
					continue;
				d &= tab[ptr[pixel[1]]] | tab[ptr[pixel[9]]];
				d &= tab[ptr[pixel[3]]] | tab[ptr[pixel[11]]];
				d &= tab[ptr[pixel[5]]] | tab[ptr[pixel[13]]];
				d &= tab[ptr[pixel[7]]] | tab[ptr[pixel[15]]];
				if (d & 1)
				{
					vt = v - threshold;
					count = 0;

					for (k = 0; k < N; k++)
					{
						x = ptr[pixel[k]];
						if (x < vt)
						{
							if (++count > K)
							{
								cornerpos[ncorners++] = j;
								break;
							}
						}
						else
							count = 0;
					}
				}
				if (d & 2)
				{
					vt = v + threshold, count = 0;

					for (k = 0; k < N; k++)
					{
						x = ptr[pixel[k]];
						if (x > vt)
						{
							if (++count > K)
							{
								cornerpos[ncorners++] = j;
								break;
							}
						}
						else
							count = 0;
					}
				}
			}
		}
		cornerpos[-1] = ncorners;
		if (i == 3)
			continue;
		prev = buf[(i - 4 + 3) % 3];
		pprev = buf[(i - 5 + 3) % 3];
		cornerpos = cpbuf[(i - 4 + 3) % 3];
		ncorners = cornerpos[-1];

		for (k = 0; k < ncorners; k++)
		{
			j = cornerpos[k];
			score = prev[j];
			//keypoints.push_back(KeyPoint((float)j, (float)(i - 1), 7.f, -1, (float)score));
			if (numOfp < maxnum)
			{
				hOut[numOfp] = i;
				wOut[numOfp++] = j;
			}
		}
	}
	
	
	
	free(_buf);
	return numOfp;
}
//#include "stdafx.h"
#include "findCorner4.h"
#include "recgFSM.h"
#include <stdlib.h>

void _getBinaryImage(unsigned char *bmp, int height, int width)
{
	int i, j, max, i_max, i_max2, Th, t;
	int h_1 = height / 8;
	int h_2 = 2 * height / 8;
	int h_3 = 3 * height / 8;
	int h_4 = 4 * height / 8;
	int h_5 = 5 * height / 8;
	int h_6 = 6 * height / 8;
	int h_7 = 7 * height / 8;
	int w_1d2 = width / 2;
	int w_2d5 = 2 * width / 5;
	int *Histogram, *score;
	Histogram = (int*)malloc(sizeof(int) * 256);
	score = (int*)malloc(sizeof(int) * 256);
	for (i = 0; i<256; i++)
	{
		Histogram[i] = 0;
	}
	for (j = (w_1d2 - w_2d5); j<(w_1d2 + w_2d5); j++)
	{
		Histogram[bmp[h_1*width + j]]++;
		Histogram[bmp[h_2*width + j]]++;
		Histogram[bmp[h_3*width + j]]++;
		Histogram[bmp[h_4*width + j]]++;
		Histogram[bmp[h_5*width + j]]++;
		Histogram[bmp[h_6*width + j]]++;
		Histogram[bmp[h_7*width + j]]++;
	}
	max = -999999;
	for (i = 0; i<256; i++)
	{
		if (Histogram[i]>max)
		{
			max = Histogram[i];
			i_max = i;
		}
	}
	for (i = 0; i<256; i++)
	{
		score[i] = Histogram[i] * (i - i_max)*(i - i_max);
	}
	max = -999999;
	for (i = 0; i<256; i++)
	{
		if (score[i]>max)
		{
			max = score[i];
			i_max2 = i;
		}
	}
	Th = (i_max + i_max2) / 2;
	t = 0;
	for (i = 0; i<height; i++)
	{
		for (j = 0; j<width; j++)
		{
			if (*(bmp + t + j) >= Th) *(bmp + t + j) = 255;
			else *(bmp + t + j) = 0;

		}
		t += width;
	}
	free(Histogram);
	free(score);
}

//bmpΪ����Ҷ�ͼ,fastThΪFAST�㷨��ֵ
//L0Ϊ���Ԥ���հ״���С���ؿ�ȣ�L1ϸ��������ؿ�ȣ�L2�����е�������ؿ��
//�õ���������ֵ��Ϊ�������Ľ����꣬�ֱ�Ϊ���������ң��������� (ԭ��Ϊ�������)
int findCorner4(unsigned char *bmp, int h, int w, int fastTh, int L0, int L1, int L2, int resH[4], int resW[4])
{
	int i, _h[4], _w[4], isFind;

	//��ֵ��
	_getBinaryImage(bmp, h, w);
  //for(i=0;i<h*w;i++) {if(bmp[i]>135) bmp[i]=255; else bmp[i]=0;}
	
	//�ҽ��Ƶ�
	isFind = getSimilarCorner4((unsigned char*)bmp, h, w, L0, L1, L2, _h, _w);
	if (isFind < 0) return -1;
	for (i = 0; i < 4; i++)
	{
		resH[i] = _h[i];
		resW[i] = _w[i];
	}
	return 1;
}

//#include "stdafx.h"
#include "recgFSM.h"
#include <math.h>

int confirm_hcheck2(unsigned char *pos, int width, int level)
{
	//return 1;
	int i, j;
	for (i = 0; i < level; i++)
	{
		for (j = 0; j < level; j++)
		{
			if (pos[i*width + j] == 0) return 0;
		}
	}
	return 1;
}

//hsptr为每一行的首指针,width为图像宽度,
//L0为左边预留空白处最小像素宽度，L1细线最大像素宽度，L2方格中的最大像素宽度
int h_check2(unsigned char *hsptr, int width, int L0, int L1, int L2, int blankLv, int *pos3d)
{
	int xbase, state, pos, A, B, C, D, E, F, G, errStopFg, LE;
	for (xbase = 0; xbase < width / 2; xbase += 2)//左边区域逐点检查
	{
		state = A = B = C = D = E = F = G = 0;
		pos = xbase;
		errStopFg = 0;
		while ((pos < width) && (!errStopFg))
		{
			switch (state)
			{
			case 0:
				if (hsptr[pos] == 0)
				{
					if (A >= L0) state = 1;
					else errStopFg = 1;
				}
				else { A++; pos++; }
				break;
			case 1:
				if (hsptr[pos] == 0) { B++; pos++; }
				else
				{
					if ((B <= L1) && (A >= 4 * B)) state = 2;
					else errStopFg = 1;
				}
				break;
			case 2:
				if (hsptr[pos] == 0)
				{
					if ((C >= L2) && (C >= 10 * B)) state = 3;
					else errStopFg = 1;
				}
				else { C++; pos++; }
				break;
			case 3:
				if (hsptr[pos] == 0) { D++; pos++; }
				else
				{
					if (fabs(B - D) / (B + 0.1) <= 0.5) { LE = (B + D) / 2; state = 4; }
					else errStopFg = 1;
				}
				break;
			case 4:
				if (hsptr[pos] == 0)
				{
					if (fabs(E - C) / (C + 0.1) <= 0.5) state = 5;
					else errStopFg = 1;
				}
				else { E++; pos++; }
				break;
			case 5:
				if (hsptr[pos] == 0) { F++; pos++; }
				else
				{
					if (fabs(F - LE) / (LE + 0.1) <= 0.5) state = 6;
					else errStopFg = 1;
				}
				break;
			case 6:
				if (hsptr[pos] == 0) errStopFg = 1;
				else
				{
					G++; pos++;
					if (G > A)
					{
						if (confirm_hcheck2(hsptr + xbase, width, blankLv))  //进行xbase区空白度扫描
						{
							pos3d[0] = xbase + A + B / 2;
							pos3d[1] = xbase + A + B + C + D / 2;
							pos3d[2] = xbase + A + B + C + D + E + F / 2;
							return xbase;
						}
						else errStopFg = 1;
					}
				}
			}
		}
	}
	return -1;
}

int getSimilarCorner4(unsigned char *bmp, int height, int width, int L0, int L1, int L2, int *h_ar4, int *w_ar4)
{
	int i, k;
	int fg[3];
	for (i = 5; i < height / 2; i++)
	{
		k = h_check2(bmp + width * i, width, L0, L1, L2, 6, fg);
		if (k >= 0) break;
	}
	
	h_ar4[0] = i; w_ar4[0] = fg[1];
	h_ar4[1] = i; w_ar4[1] = fg[2];
	if (k < 0) return -1;
	for (i = height - 6; i > height / 2; i--)
	{
		k = h_check2((unsigned char*)bmp + width * i, width, L0, L1, L2, 6, fg);
		if (k >= 0) break;
	}
	
	h_ar4[2] = i; w_ar4[2] = fg[1];
	h_ar4[3] = i; w_ar4[3] = fg[2];
	if (k < 0) return -1;
	return 1;
}



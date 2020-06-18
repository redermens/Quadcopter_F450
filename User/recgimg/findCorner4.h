#ifndef FINDCORNER4_H
#define FINDCORNER4_H


//bmp为输入灰度图,fastTh为FAST算法阈值
//L0为左边预留空白处最小像素宽度，L1细线最大像素宽度，L2方格中的最大像素宽度
//得到坐标数组值，为正方形四角坐标，分别为：上左，上右，下左，下右 (原点为左上情况)
int findCorner4(unsigned char *bmp, int h, int w, int fastTh, int L0, int L1, int L2, int resH[4], int resW[4]);

#endif

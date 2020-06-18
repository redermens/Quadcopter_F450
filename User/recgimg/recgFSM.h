#ifndef RECGFSM_H
#define RECGFSM_H

//L0为左边预留空白处最小像素宽度，L1细线最大像素宽度，L2方格中的最大像素宽度
//得到int[4]数组值，为正方形四角坐标，分别为：上左，上右，下左，下右 (原点为左上情况)
int getSimilarCorner4(unsigned char *bmp, int height, int width, int L0, int L1, int L2, int *h_ar4, int *w_ar4);



#endif


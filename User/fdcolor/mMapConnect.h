/*
mMapconnect.h
寻找二值图像的连通域,并求得各个连通域质心坐标，支持最大连通分量254个
*/

#ifndef MMAPCONNECT_H
#define MMAPCONNECT_H

typedef struct stuPoint{
    union{
        struct{
            unsigned char x;
            unsigned char y;
            unsigned char next;
            unsigned char first;
            unsigned char tag;
        }type1;
        struct{
            unsigned int xpos;  //区域质心列坐标
            unsigned int ypos;  //区域质心行坐标
            unsigned int counts;    //区域面积
        }type2;
    }_un;
}stuPoint;

extern stuPoint ptAr[256]; //当调用完getMapFd后，此处为各点质心坐标
extern unsigned char ptArCt;  //外部调用为区域标记所在下标区域[1,ptArCt),总个数ptArCt-1

//void SetCombime(unsigned char small,unsigned char big);

/*
给二值图像中的0连通块划分区域
pMap值会改动，为保证原图像值不丢失，建议为图像副本
nCol:列数，nRow：行数
(二值化取值:0或255)
*/
int getMapConnect16(unsigned short *pMap,int nCol,int nRow);

#endif // MMAPCONNECT_H
/*
技术支持:
连通域标记算法http://www.blue1000.com/bkhtml/c17/2014-03/71515.htm?__=b492
邻接表资料http://developer.51cto.com/art/201404/435072.htm
*/

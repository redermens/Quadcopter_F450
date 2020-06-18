/*
mMapconnect.c
寻找二值图像的连通域,并求得各个连通域质心坐标，支持最大连通分量254个
*/

#include "mMapConnect.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mSQ_mlib.h"

stuPoint ptAr[256]; //当调用完getMapFd后，此处为各点质心坐标
unsigned char ptArCt;  //外部调用为区域标记所在下标区域[1,ptArCt),总个数ptArCt-1

void SetCombime(unsigned char small,unsigned char big)
{
    int i;
    for(i=1;i<ptArCt;i++)   //保证不重复
    {
        if((ptAr[i]._un.type1.x==small)&&(ptAr[i]._un.type1.y==big)) return;
    }
    ptAr[ptArCt]._un.type1.x=small;
    ptAr[ptArCt]._un.type1.y=big;
    ptArCt++;
    return;
}

/*
给二值图像中的0连通块划分区域
pMap值会改动，为保证原图像值不丢失，建议为图像副本
nCol:列数，nRow：行数
(二值化取值:0或255)
*/
int getMapConnect16(unsigned short *pMap,int nCol,int nRow)
{
    int i,j,k,p,nowPos,p0Start,pC,pStart,pEnd,fd_tag=1;
    unsigned char sta=0,mi,xfd_tag;
    ptArCt=1;   //编号从1开始,数据量为ptArCt-1,边数
    //================对map进行边界处理===================
    p=nCol*(nRow-1);
    for(i=0;i<nCol;i++)
    {//边界清255
        pMap[i]=255;
        pMap[p+i]=255;
    }
    p=0;
    for(i=0;i<nRow;i++)
    {//边界清255
        pMap[p]=255;
        pMap[p+nCol-1]=255;
        p+=nCol;
    }
    //====================逐行标记=======================
    p=nCol;
    for(i=1;i<nRow-1;i++)   //第二行开始
    {
        for(j=0;j<nCol;j++)   //从第二列开始
        {
            nowPos=p+j;
            if(pMap[nowPos]==0) //敏感点
            {
                switch(sta)
                {
                    case 0: //从不敏感点到敏感点
                        p0Start=nowPos;//定位
                        pC=1;   //点个数
                        sta=1;
                        break;
                    case 1: //从敏感点到敏感点
                        pC++;
                        break;
                }
            }
            else    //不敏感点
            {
                switch(sta)
                {
                    case 0: //从不敏感点到不敏感点
                        break;
                    case 1: //从敏感点到不敏感点,进行标号，更新集合关系表
                        pStart=p0Start-nCol-1;
                        pEnd=pStart+pC+1;
                        mi=255;
                        for(k=pStart;k<=pEnd;k++)
                        {
                            if(pMap[k]<mi) mi=pMap[k];
                        }
                        if(mi==255)
                        {
                            for(k=0;k<pC;k++)   //写入
                            {
                                pMap[p0Start+k]=fd_tag;
                            }
                            fd_tag++;
                        }
                        else
                        {
                            for(k=pStart;k<=pEnd;k++)   //更新集合关系表
                            {
                                if((pMap[k]!=255)&&(pMap[k]!=mi)) SetCombime(mi,pMap[k]);//保证小的在前面
                            }
                            for(k=0;k<pC;k++)   //写入
                            {
                                pMap[p0Start+k]=mi;
                            }
                        }
                        sta=0;
                        break;
                }
            }
        }
        p+=nCol;
    }   //[1,ptArCt)为边所在的下标域
    if(fd_tag>254) return -1;   //点数太多
    //==================生成邻接表数组====================
    for(i=0;i<256;i++)
    {
        ptAr[i]._un.type1.first=255;
        ptAr[i]._un.type1.tag=0;
    }
    for(i=1;i<ptArCt;i++)
    {
        ptAr[i]._un.type1.next=ptAr[ptAr[i]._un.type1.x]._un.type1.first;
        ptAr[ptAr[i]._un.type1.x]._un.type1.first=i;
    }
    //============宽度遍历邻接表找到连通分量================
    mQueue_Init(sizeof(unsigned char));
    xfd_tag=1;
    for(i=1;i<fd_tag;i++)   //从标号为1的点开始遍历
    {
        if(ptAr[i]._un.type1.tag==0)  //该点没有集合标号
        {
            if(ptAr[i]._un.type1.first==255)  //如果该点没有边，直接赋予集合标号
            {
                ptAr[i]._un.type1.tag=xfd_tag;
                xfd_tag++;
            }
            else    //该点存在边
            {
                mi=(unsigned char)i;    //mi暂存点
                mQueue_EnQueue(&mi);    //点入队
                while(!mQueue_IsEmpty())
                {
                    mQueue_DeQueue(&mi);    //取点
                    ptAr[mi]._un.type1.tag=xfd_tag;   //给点标号
                    sta=ptAr[mi]._un.type1.first; //sta为取的点的第一个边下标
                    while(sta!=255)
                    {
                        mi=ptAr[sta]._un.type1.y;    //边连接的点
                        if(ptAr[mi]._un.type1.tag==0)    //如果该点未被标记
                            mQueue_EnQueue(&mi); //则该点入队
                        sta=ptAr[sta]._un.type1.next; //继续提取下一个点
                    }
                }
                xfd_tag++;
            }
        }
    }
    p=nCol;
    for(i=1;i<nRow-1;i++)   //更新标记点
    {
        for(j=1;j<nCol-1;j++)
        {
            nowPos=p+j;
            if(pMap[nowPos]!=255)
            {
                pMap[nowPos]=ptAr[pMap[nowPos]]._un.type1.tag;
            }
        }
        p+=nCol;
    }
    //=====================得到质心=======================
    for(i=0;i<xfd_tag;i++)
    {
        ptAr[i]._un.type2.xpos=0;
        ptAr[i]._un.type2.ypos=0;
        ptAr[i]._un.type2.counts=0;
    }
    p=nCol;
    for(i=1;i<nRow-1;i++)
    {
        for(j=1;j<nCol-1;j++)
        {
            nowPos=p+j;
            if(pMap[nowPos]!=255)
            {
                ptAr[pMap[nowPos]]._un.type2.xpos+=j;
                ptAr[pMap[nowPos]]._un.type2.ypos+=i;
                ptAr[pMap[nowPos]]._un.type2.counts++;
            }
        }
        p+=nCol;
    }
    for(i=1;i<xfd_tag;i++)
    {
        ptAr[i]._un.type2.xpos=ptAr[i]._un.type2.xpos/ptAr[i]._un.type2.counts;
        ptAr[i]._un.type2.ypos=ptAr[i]._un.type2.ypos/ptAr[i]._un.type2.counts;
    }
    ptArCt=xfd_tag;
    return (int)xfd_tag;
}

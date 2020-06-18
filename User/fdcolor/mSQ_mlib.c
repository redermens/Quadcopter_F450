/*
mSQ_mlib.c
软栈队列
*/

#include "mSQ_mlib.h"
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

unsigned char mSQSpace[SQ_MAX_SIZE];    //栈队列内存空间

//栈相关
int mStackDataSize=0;
int mStackTop=0;    //向上增长
int mStackFastPushFg=0;

//队列相关
int mQueueDataSize=0;
int mQueueFront=0;
int mQueueRear=0;
int mQueueCount=0;
int mQueueFastFg=0;

//==========================栈程序=================================
void mStack_Init(int data_size) //初始化栈数据类型大小
{
    mStackDataSize=data_size;
    mStackTop=0;
    mStackFastPushFg=SQ_MAX_SIZE-mStackDataSize;
}

int mStack_Push(void *data)    //入栈
{
    if(mStackTop>mStackFastPushFg) return -1;
    memcpy(mSQSpace+mStackTop,data,mStackDataSize);
    mStackTop+=mStackDataSize;
    return 0;
}

int mStack_Pop(void *data_buf)    //出栈
{
    if(mStackTop<=0) return -1;
    memcpy(data_buf,mSQSpace+mStackTop-mStackDataSize,mStackDataSize);
    mStackTop-=mStackDataSize;
    return 0;
}

int mStack_Top(void *data_buf)    //查看栈顶
{
    if(mStackTop<=0) return -1;
    memcpy(data_buf,mSQSpace+mStackTop-mStackDataSize,mStackDataSize);
    return 0;
}

int mStack_IsEmpty(void)   //判空,1空
{
    return (mStackTop<=0);
}

void mStack_Empty(void)    //栈排空
{
    mStackTop=0;
}

int mStack_GetCount(void)  //查看数据元素个数
{
    return (mStackTop/mStackDataSize);
}

//=========================队列程序================================
void mQueue_Init(int data_size)
{
    mQueueDataSize=data_size;
    mQueueFront=0;
    mQueueRear=0;
    mQueueCount=0;
    mQueueFastFg=SQ_MAX_SIZE%data_size+data_size;
}

int mQueue_EnQueue(void *data)   //入队
{
    if((mQueueRear+mQueueFastFg)%(SQ_MAX_SIZE)==mQueueFront) return -1;   //队满
    memcpy(mSQSpace+mQueueRear,data,mQueueDataSize);  //拷贝数据
    mQueueRear=(mQueueRear+mQueueDataSize)%(SQ_MAX_SIZE);
    mQueueCount++;
    return 0;
}

int mQueue_DeQueue(void *data_buf)  //出队
{
    if(mQueueRear==mQueueFront) return -1;  //队空
    memcpy(data_buf,mSQSpace+mQueueFront,mQueueDataSize);
    mQueueFront=(mQueueFront+mQueueDataSize)%(SQ_MAX_SIZE);
    mQueueCount--;
    return 0;
}

int mQueue_IsEmpty(void)
{
    return (mQueueRear==mQueueFront);
}

int mQueue_GetCount(void)
{
    return mQueueCount;
}

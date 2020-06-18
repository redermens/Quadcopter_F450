/*
mSQ_mlib.h
软栈队列
*/

#ifndef MSQ_MLIB_H_INCLUDED
#define MSQ_MLIB_H_INCLUDED

#include<stdio.h>
#include<math.h>
#include<stdlib.h>

#define SQ_MAX_SIZE 2048     //静态空间最大值(byte)

//==========================栈程序=================================
void mStack_Init(int data_size); //初始化栈数据类型大小
int mStack_Push(void *data);    //入栈
int mStack_Pop(void *data_buf);    //出栈
int mStack_Top(void *data_buf);    //查看栈顶
int mStack_IsEmpty(void);   //判空,1空
void mStack_Empty(void);    //栈排空
int mStack_GetCount(void);  //查看数据元素个数
//=========================队列程序================================
void mQueue_Init(int data_size);
int mQueue_EnQueue(void *data);   //入队
int mQueue_DeQueue(void *data_buf);  //出队
int mQueue_IsEmpty(void);
int mQueue_GetCount(void);

#endif // MSTACK_MLIB_H_INCLUDED

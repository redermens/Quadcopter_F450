#ifndef FINDCORNER4_H
#define FINDCORNER4_H


//bmpΪ����Ҷ�ͼ,fastThΪFAST�㷨��ֵ
//L0Ϊ���Ԥ���հ״���С���ؿ�ȣ�L1ϸ��������ؿ�ȣ�L2�����е�������ؿ��
//�õ���������ֵ��Ϊ�������Ľ����꣬�ֱ�Ϊ���������ң��������� (ԭ��Ϊ�������)
int findCorner4(unsigned char *bmp, int h, int w, int fastTh, int L0, int L1, int L2, int resH[4], int resW[4]);

#endif

#ifndef PERSPECTIVE_H
#define PERSPECTIVE_H

typedef struct TRANSFORM_MAT_3X3 {
	float a11, a12, a13, a21, a22, a23, a31, a32, a33;
}TRANSFORM_MAT_3X3;

//��p������Ҫת�����ı��ε��Ķ�������
void quadrilateralToQuadrilateral(TRANSFORM_MAT_3X3 *mat,
	float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3,
	float x0p, float y0p, float x1p, float y1p, float x2p, float y2p, float x3p, float y3p);

//���ȵ���quadrilateralToQuadrilateral���ɱ任�������ܵ��ã���pointX��pointY�еĵ�ת��������
void transformPoints(TRANSFORM_MAT_3X3 mat, float *pointX, float *pointY, int point_size);

#endif

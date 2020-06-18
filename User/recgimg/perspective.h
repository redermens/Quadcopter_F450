#ifndef PERSPECTIVE_H
#define PERSPECTIVE_H

typedef struct TRANSFORM_MAT_3X3 {
	float a11, a12, a13, a21, a22, a23, a31, a32, a33;
}TRANSFORM_MAT_3X3;

//代p的是需要转换的四边形的四顶点坐标
void quadrilateralToQuadrilateral(TRANSFORM_MAT_3X3 *mat,
	float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3,
	float x0p, float y0p, float x1p, float y1p, float x2p, float y2p, float x3p, float y3p);

//当先调用quadrilateralToQuadrilateral生成变换矩阵后才能调用，将pointX与pointY中的点转换并更新
void transformPoints(TRANSFORM_MAT_3X3 mat, float *pointX, float *pointY, int point_size);

#endif

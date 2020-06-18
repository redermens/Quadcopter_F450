//#include "stdafx.h"
#include "perspective.h"

void set_transform_mat_3x3(TRANSFORM_MAT_3X3 *mat,
	float inA11, float inA21, float inA31,
	float inA12, float inA22, float inA32,
	float inA13, float inA23, float inA33)
{
	mat->a11 = inA11; mat->a12 = inA12; mat->a13 = inA13;
	mat->a21 = inA21; mat->a22 = inA22; mat->a23 = inA23;
	mat->a31 = inA31; mat->a32 = inA32; mat->a33 = inA33;
}

void quadrilateralToSquare(TRANSFORM_MAT_3X3 *mat,
	float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3)
{
	TRANSFORM_MAT_3X3 t;
	float dx3 = x0 - x1 + x2 - x3;
	float dy3 = y0 - y1 + y2 - y3;
	float dx1, dx2, dy1, dy2, denominator, a13, a23;
	if (dx3 == 0.0f && dy3 == 0.0f)
	{
		set_transform_mat_3x3(&t, x1 - x0, x2 - x1, x0, y1 - y0, y2 - y1, y0, 0.0f, 0.0f, 1.0f);
	}
	else
	{
		dx1 = x1 - x2;
		dx2 = x3 - x2;
		dy1 = y1 - y2;
		dy2 = y3 - y2;
		denominator = dx1 * dy2 - dx2 * dy1;
		a13 = (dx3 * dy2 - dx2 * dy3) / denominator;
		a23 = (dx1 * dy3 - dx3 * dy1) / denominator;
		set_transform_mat_3x3(&t, x1 - x0 + a13 * x1, x3 - x0 + a23 * x3, x0, y1 - y0
			+ a13 * y1, y3 - y0 + a23 * y3, y0, a13, a23, 1.0f);
	}
	set_transform_mat_3x3(mat, t.a22 * t.a33 - t.a23 * t.a32, t.a23 * t.a31 - t.a21 * t.a33, t.a21 * t.a32
		- t.a22 * t.a31, t.a13 * t.a32 - t.a12 * t.a33, t.a11 * t.a33 - t.a13 * t.a31,
		t.a12 * t.a31 - t.a11 * t.a32, t.a12 * t.a23 - t.a13 * t.a22,
		t.a13 * t.a21 - t.a11 * t.a23, t.a11 * t.a22 - t.a12 * t.a21);
}

void squareToQuadrilateral(TRANSFORM_MAT_3X3 *mat,
	float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3)
{
	float dx3 = x0 - x1 + x2 - x3;
	float dy3 = y0 - y1 + y2 - y3;
	float dx1, dx2, dy1, dy2, denominator, a13, a23;
	if (dx3 == 0.0f && dy3 == 0.0f)
	{
		set_transform_mat_3x3(mat, x1 - x0, x2 - x1, x0, y1 - y0, y2 - y1, y0, 0.0f, 0.0f, 1.0f);
	}
	else
	{
		dx1 = x1 - x2;
		dx2 = x3 - x2;
		dy1 = y1 - y2;
		dy2 = y3 - y2;
		denominator = dx1 * dy2 - dx2 * dy1;
		a13 = (dx3 * dy2 - dx2 * dy3) / denominator;
		a23 = (dx1 * dy3 - dx3 * dy1) / denominator;
		set_transform_mat_3x3(mat, x1 - x0 + a13 * x1, x3 - x0 + a23 * x3, x0, y1 - y0
			+ a13 * y1, y3 - y0 + a23 * y3, y0, a13, a23, 1.0f);
	}
}

void perspective_times(TRANSFORM_MAT_3X3 *res, TRANSFORM_MAT_3X3 a, TRANSFORM_MAT_3X3 other)
{
	set_transform_mat_3x3(res,a.a11 * other.a11 + a.a21 * other.a12 + a.a31 * other.a13,
		a.a11 * other.a21 + a.a21 * other.a22 + a.a31 * other.a23,
		a.a11 * other.a31 + a.a21 * other.a32 + a.a31 * other.a33,
		a.a12 * other.a11 + a.a22 * other.a12 + a.a32 * other.a13,
		a.a12 * other.a21 + a.a22 * other.a22 + a.a32 * other.a23,
		a.a12 * other.a31 + a.a22 * other.a32 + a.a32 * other.a33,
		a.a13* other.a11 + a.a23 * other.a12 + a.a33 * other.a13,
		a.a13 * other.a21 + a.a23 * other.a22 + a.a33 * other.a23,
		a.a13 * other.a31 + a.a23 * other.a32 + a.a33 * other.a33);
}

//代p的是需要转换的四边形的四顶点坐标
void quadrilateralToQuadrilateral(TRANSFORM_MAT_3X3 *mat,
	float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3,
	float x0p, float y0p, float x1p, float y1p, float x2p, float y2p, float x3p, float y3p)
{
	TRANSFORM_MAT_3X3 qToS, sToQ;
	quadrilateralToSquare(&qToS, x0, y0, x1, y1, x2, y2, x3, y3);
	squareToQuadrilateral(&sToQ, x0p, y0p, x1p, y1p, x2p, y2p, x3p, y3p);
	perspective_times(mat, sToQ, qToS);
}

void transformPoints(TRANSFORM_MAT_3X3 mat, float *pointX, float *pointY, int point_size)
{
	int i;
	float x, y, denominator;
	for (i = 0; i < point_size; i++)
	{
		x = pointX[i];
		y = pointY[i];
		denominator = mat.a13 * x + mat.a23 * y + mat.a33;
		pointX[i] = (mat.a11 * x + mat.a21 * y + mat.a31) / denominator;
		pointY[i] = (mat.a12 * x + mat.a22 * y + mat.a32) / denominator;
	}
}

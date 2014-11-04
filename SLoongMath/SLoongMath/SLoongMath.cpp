/************************************************************************/
/* 				Math.cpp --- Math                         */
/************************************************************************/
#include "stdafx.h"

#include "SloongMath.h"

using namespace SoaringLoong::SloongMath;

CSLMath::CSLMath()
{
	Build_Sin_Cos_Tables();
}


CSLMath::~CSLMath()
{
}


void CSLMath::Build_Sin_Cos_Tables(void)
{

	// create sin/cos lookup table
	// note the creation of one extra element; 360
	// this helps with logic in using the tables

	// generate the tables 0 - 360 inclusive
	for (int ang = 0; ang <= 360; ang++)
	{
		// convert ang to radians
		double theta = (double)ang*PI / (double)180;

		// insert next entry into table
		cos_look[ang] = cos(theta);
		sin_look[ang] = sin(theta);

	} // end for ang

} // end Build_Sin_Cos_Tables


/////////////////////////////////////////////////////////////////

int CSLMath::Mat_Mul1X2_3X2(CSLoongMatrix1x2* ma,
	CSLoongMatrix3x2* mb,
	CSLoongMatrix1x2* mprod)
{
	// this function multiplies a 1x2 matrix against a 
	// 3x2 matrix - ma*mb and stores the result
	// using a dummy element for the 3rd element of the 1x2 
	// to make the matrix multiply valid i.e. 1x3 X 3x2

	for (int col = 0; col < 2; col++)
	{
		// compute dot product from row of ma 
		// and column of mb

		double sum = 0; // used to hold result
		int index;
		for (index = 0; index < 2; index++)
		{
			// add in next product pair
			sum += (ma->M[index] * mb->M[index][col]);
		} // end for index

		// add in last element * 1 
		sum += mb->M[index][col];

		// insert resulting col element
		mprod->M[col] = sum;

	} // end for col

	return(1);

} // end Mat_Mul_1X2_3X2


///////////////////////////////////////////////////////////////

// these are the matrix versions, note they are more inefficient for
// single transforms, but their power comes into play when you concatenate
// multiple transformations, not to mention that all transforms are accomplished
// with the same code, just the matrix differs

int CSLMath::Translate_Polygon2D_Mat(CSLoongPolygon2D* poly, int dx, int dy)
{
	// this function translates the center of a polygon by using a matrix multiply
	// on the the center point, this is incredibly inefficient, but for educational purposes
	// if we had an object that wasn't in local coordinates then it would make more sense to
	// use a matrix, but since the origin of the object is at x0,y0 then 2 lines of code can
	// translate, but lets do it the hard way just to see :)

	// test for valid pointer
	if (!poly)
		return(0);

	CSLoongMatrix3x2 mt; // used to hold translation transform matrix

	// initialize the matrix with translation values dx dy
	Mat_Init_3X2(&mt, 1, 0, 0, 1, dx, dy);

	// create a 1x2 matrix to do the transform
	CSLoongMatrix1x2 p0 = { poly->x0, poly->y0 };
	CSLoongMatrix1x2 p1 = { 0, 0 }; // this will hold result

	// now translate via a matrix multiply
	Mat_Mul_1X2_3X2(&p0, &mt, &p1);

	// now copy the result back into polygon
	poly->x0 = p1.M[0];
	poly->y0 = p1.M[1];
	/*
	double lfTemp[] = { 1, 0, 0, 1, dx, dy };
	CMatrix oMatrix3x2(3, 2, lfTemp);

	// create a 1x2 matrix to do the transform
	double lf1x2[] = { poly->x0, poly->y0, 0 };
	CMatrix oMatix1x2(1, 3, lf1x2);
	// now translate via a matrix multiply
	CMatrix oRes = oMatix1x2*oMatrix3x2;
	// now copy the result back into polygon
	poly->x0 = (int)oRes.m_pMatrix[0][0];
	poly->y0 = (int)oRes.m_pMatrix[0][1];*/


	// return success
	return(1);

} // end Translate_Polygon2D_Mat

///////////////////////////////////////////////////////////////

int CSLMath::Rotate_Polygon2D_Mat(CSLoongPolygon2D* poly, int theta)
{
	// this function rotates the local coordinates of the polygon

	// test for valid pointer
	if (!poly)
		return(0);

	// test for negative rotation angle
	if (theta < 0)
		theta += 360;

	CSLoongMatrix3x2 mr; // used to hold rotation transform matrix

	// initialize the matrix with translation values dx dy
	Mat_Init_3X2(&mr, cos_look[theta], sin_look[theta],
		-sin_look[theta], cos_look[theta],
		0, 0);

	// loop and rotate each point, very crude, no lookup!!!
	for (int curr_vert = 0; curr_vert < poly->num_verts; curr_vert++)
	{
		// create a 1x2 matrix to do the transform
		CSLoongMatrix1x2 p0 = { poly->vlist[curr_vert].x, poly->vlist[curr_vert].y };
		CSLoongMatrix1x2 p1 = { 0, 0 }; // this will hold result

		// now rotate via a matrix multiply
		Mat_Mul_1X2_3X2(&p0, &mr, &p1);

		// now copy the result back into vertex
		poly->vlist[curr_vert].x = p1.M[0];
		poly->vlist[curr_vert].y = p1.M[1];

	} // end for curr_vert

	// this function rotates the local coordinates of the polygon

	// test for valid pointer
	// 	if (!poly)
	// 		return(0);
	// 
	// 	// test for negative rotation angle
	// 	if (theta < 0)
	// 		theta += 360;
	// 
	// 	try
	// 	{
	// 		// used to hold rotation transform matrix
	// 		/*	// initialize the matrix with translation values dx dy
	// 		double lfTemp[] = { cos_look[theta], sin_look[theta],
	// 		-sin_look[theta], cos_look[theta],
	// 		0, 0 };
	// 		CMatrix oMatrix3x2(3, 2, lfTemp);
	// 
	// 		// loop and rotate each point, very crude, no lookup!!!
	// 		for (int curr_vert = 0; curr_vert < poly->num_verts; curr_vert++)
	// 		{
	// 		// create a 1x2 matrix to do the transform
	// 		double lf1x2[] = { poly->vlist[curr_vert].x, poly->vlist[curr_vert].y, 0 };
	// 		CMatrix oMatix1x2(1, 3, lf1x2);
	// 
	// 		// now rotate via a matrix multiply
	// 		//Mat_Mul1X2_3X2(&p0, &mr, &p1);
	// 		CMatrix oRes = oMatix1x2*oMatrix3x2;
	// 
	// 		// now copy the result back into vertex
	// 		poly->vlist[curr_vert].x = oRes.m_pMatrix[0][0];
	// 		poly->vlist[curr_vert].y = oRes.m_pMatrix[0][1];
	// 
	// 		} // end for curr_vert*/
	// 	}
	// 	catch (LPCTSTR strErr)
	// 	{
	// 		//g_oLog.ResLog(ERR,1,strErr);
	// 	}
	// return success
	return(1);

} // end Rotate_Polygon2D_Mat

////////////////////////////////////////////////////////

int CSLMath::Scale_Polygon2D_Mat(CSLoongPolygon2D* poly, float sx, float sy)
{
	// this function scalesthe local coordinates of the polygon

	// test for valid pointer
	if (!poly)
		return(0);


	CSLoongMatrix3x2 ms; // used to hold scaling transform matrix

	// initialize the matrix with translation values dx dy
	Mat_Init_3X2(&ms, sx, 0,
		0, sy,
		0, 0);


	// loop and scale each point
	for (int curr_vert = 0; curr_vert < poly->num_verts; curr_vert++)
	{
		// scale and store result back

		// create a 1x2 matrix to do the transform
		CSLoongMatrix1x2 p0 = { poly->vlist[curr_vert].x, poly->vlist[curr_vert].y };
		CSLoongMatrix1x2 p1 = { 0, 0 }; // this will hold result

		// now scale via a matrix multiply
		Mat_Mul_1X2_3X2(&p0, &ms, &p1);

		// now copy the result back into vertex
		poly->vlist[curr_vert].x = p1.M[0];
		poly->vlist[curr_vert].y = p1.M[1];

	} // end for curr_vert
	/*	// used to hold scaling transform matrix
	// initialize the matrix with translation values dx dy
	double lfTemp[] = { sx, 0,
	0, sy,
	0, 0 };
	CMatrix oMatrix3x2(3, 2, lfTemp);

	// loop and scale each point
	for (int curr_vert = 0; curr_vert < poly->num_verts; curr_vert++)
	{
	// scale and store result back

	// create a 1x2 matrix to do the transform
	double lf1x2[] = { poly->vlist[curr_vert].x, poly->vlist[curr_vert].y, 0 };
	CMatrix oMatix1x2(1, 3, lf1x2);
	// now scale via a matrix multiply

	// now copy the result back into vertex
	CMatrix oRes = oMatix1x2*oMatrix3x2;

	// now copy the result back into vertex
	poly->vlist[curr_vert].x = oRes.m_pMatrix[0][0];
	poly->vlist[curr_vert].y = oRes.m_pMatrix[0][1];

	} // end for curr_vert*/
	// return success
	return(1);

} // end Scale_Polygon2D_Mat

///////////////////////////////////////////////////////////

int CSLMath::Mat_Mul_3X3(CSLoongMatrix3x3* ma,
	CSLoongMatrix3x3* mb,
	CSLoongMatrix3x3* mprod)
{
	// this function multiplies two matrices together and 
	// and stores the result

	for (int row = 0; row < 3; row++)
	{
		for (int col = 0; col < 3; col++)
		{
			// compute dot product from row of ma 
			// and column of mb

			float sum = 0; // used to hold result

			for (int index = 0; index < 3; index++)
			{
				// add in next product pair
				sum += (ma->M[row][index] * mb->M[index][col]);
			} // end for index

			// insert resulting row,col element
			mprod->M[row][col] = sum;

		} // end for col

	} // end for row

	return(1);

} // end Mat_Mul_3X3

////////////////////////////////////////////////////////////////

int CSLMath::Mat_Mul_1X3_3X3(CSLoongMatrix1x3* ma,
	CSLoongMatrix3x3* mb,
	CSLoongMatrix1x3* mprod)
{
	// this function multiplies a 1x3 matrix against a 
	// 3x3 matrix - ma*mb and stores the result

	for (int col = 0; col < 3; col++)
	{
		// compute dot product from row of ma 
		// and column of mb

		float sum = 0; // used to hold result

		for (int index = 0; index < 3; index++)
		{
			// add in next product pair
			sum += (ma->M[index] * mb->M[index][col]);
		} // end for index

		// insert resulting col element
		mprod->M[col] = sum;

	} // end for col

	return(1);

} // end Mat_Mul_1X3_3X3

////////////////////////////////////////////////////////////////

int CSLMath::Mat_Mul_1X2_3X2(CSLoongMatrix1x2* ma,
	CSLoongMatrix3x2* mb,
	CSLoongMatrix1x2* mprod)
{
	// this function multiplies a 1x2 matrix against a 
	// 3x2 matrix - ma*mb and stores the result
	// using a dummy element for the 3rd element of the 1x2 
	// to make the matrix multiply valid i.e. 1x3 X 3x2

	for (int col = 0; col < 2; col++)
	{
		// compute dot product from row of ma 
		// and column of mb

		float sum = 0; // used to hold result

		int index = 0;
		for (; index < 2; index++)
		{
			// add in next product pair
			sum += (ma->M[index] * mb->M[index][col]);
		} // end for index

		// add in last element * 1 
		sum += mb->M[index][col];

		// insert resulting col element
		mprod->M[col] = sum;

	} // end for col

	return(1);

} // end Mat_Mul_1X2_3X2

//////////////////////////////////////////////////////////////

inline int CSLMath::Mat_Init_3X2(CSLoongMatrix3x2* ma,
	float m00, float m01,
	float m10, float m11,
	float m20, float m21)
{
	// this function fills a 3x2 matrix with the sent data in row major form
	ma->M[0][0] = m00; ma->M[0][1] = m01;
	ma->M[1][0] = m10; ma->M[1][1] = m11;
	ma->M[2][0] = m20; ma->M[2][1] = m21;

	// return success
	return(1);

} // end Mat_Init_3X2


///////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////

int CSLMath::Scale_Polygon2D(CSLoongPolygon2D* poly, float sx, float sy)
{
	// this function scalesthe local coordinates of the polygon

	// test for valid pointer
	if (!poly)
		return(0);

	// loop and scale each point
	for (int curr_vert = 0; curr_vert < poly->num_verts; curr_vert++)
	{
		// scale and store result back
		poly->vlist[curr_vert].x *= sx;
		poly->vlist[curr_vert].y *= sy;

	} // end for curr_vert

	// return success
	return(1);

} // end Scale_Polygon2D

///////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////

// the following 3 functions are the standard transforms (no matrices)

int CSLMath::Translate_Polygon2D(CSLoongPolygon2D* poly, int dx, int dy)
{
	// this function translates the center of a polygon

	// test for valid pointer
	if (!poly)
		return(0);

	// translate
	poly->x0 += dx;
	poly->y0 += dy;

	// return success
	return(1);

} // end Translate_Polygon2D


int CSLMath::Rotate_Polygon2D(CSLoongPolygon2D* poly, int theta)
{
	// this function rotates the local coordinates of the polygon

	// test for valid pointer
	if (!poly)
		return(0);

	// test for negative rotation angle
	if (theta < 0)
		theta += 360;

	// loop and rotate each point, very crude, no lookup!!!
	for (int curr_vert = 0; curr_vert < poly->num_verts; curr_vert++)
	{

		// perform rotation
		double xr = (double)poly->vlist[curr_vert].x*cos_look[theta] -
			(double)poly->vlist[curr_vert].y*sin_look[theta];

		double yr = (double)poly->vlist[curr_vert].x*sin_look[theta] +
			(double)poly->vlist[curr_vert].y*cos_look[theta];

		// store result back
		poly->vlist[curr_vert].x = xr;
		poly->vlist[curr_vert].y = yr;

	} // end for curr_vert

	// return success
	return(1);

} // end Rotate_Polygon2D

////////////////////////////////////////////////////////////////////////////

int CSLMath::Fast_Distance_2D(int x, int y)
{
	// this function computes the distance from 0,0 to x,y with 3.5% error

	// first compute the absolute value of x,y
	x = abs(x);
	y = abs(y);

	// compute the minimum of x,y
	int mn = MIN(x, y);

	// return the distance
	return(x + y - (mn >> 1) - (mn >> 2) + (mn >> 4));

} // end Fast_Distance_2D

///////////////////////////////////////////////////////////////////////////////

float CSLMath::Fast_Distance_3D(float fx, float fy, float fz)
{
	// this function computes the distance from the origin to x,y,z

	int temp;  // used for swaping
	int x, y, z; // used for algorithm

	// make sure values are all positive
	x = fabs(fx) * 1024;
	y = fabs(fy) * 1024;
	z = fabs(fz) * 1024;

	// sort values
	if (y < x) SWAP(x, y, temp)

		if (z < y) SWAP(y, z, temp)

			if (y < x) SWAP(x, y, temp)

				int dist = (z + 11 * (y >> 5) + (x >> 2));

	// compute distance with 8% error
	return((float)(dist >> 10));

} // end Fast_Distance_3D

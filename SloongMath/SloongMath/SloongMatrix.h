#pragma once

#include "SloongMathBase.h"
namespace Sloong
{
	namespace Math
	{
		namespace Vector
		{
			class CVector3D;
			class CVector4D;
		}
		using namespace Vector;
		namespace Matrix
		{
			typedef class CMatrix1x2
			{
			public:
				union
				{
					float M[2]; // array indexed data storage

					// storage in row major form with explicit names
					struct
					{
						float M00, M01;

					}; // end explicit names
				}; // end union
			}MATRIX1X2, *LPMATRIX1X2, *MATRIX1X2_PTR;

			typedef class CMatrix1x3
			{
			public:
				union
				{
					float M[3]; // array indexed data storage

					// storage in row major form with explicit names
					struct
					{
						float M00, M01, M02;

					}; // end explicit names
				}; // end union
			}MATRIX1X3, *LPMATRIX1X3, *MATRIX1X3_PTR;

			typedef class CMatrix1x4
			{
			public:
				union
				{
					float M[4]; // array indexed data storage

					// storage in row major form with explicit names
					struct
					{
						float M00, M01, M02, M03;
					}; // end explicit names

				}; // end union
			} MATRIX1X4, *LPMATRIX1X4, *MATRIX1X4_PTR;

			typedef class CMatrix2x2
			{
			public:
				union
				{
					float M[2][2]; // array indexed data storage

					// storage in row major form with explicit names
					struct
					{
						float M00, M01;
						float M10, M11;
					}; // end explicit names

				}; // end union
			public:
				inline void MAT_COLUMN_SWAP_2X2(CMatrix2x2* m, int c, MATRIX1X2_PTR v)
				{
					m->M[0][c] = v->M[0]; m->M[1][c] = v->M[1];
				}
			public:
				void Mat_Init_2X2(CMatrix2x2* ma, float m00, float m01, float m10, float m11);
				float Mat_Det_2X2(CMatrix2x2* m);
				void Mat_Mul_2X2(CMatrix2x2* ma, CMatrix2x2* mb, CMatrix2x2* mprod);
				void Mat_Add_2X2(CMatrix2x2* ma, CMatrix2x2* mb, CMatrix2x2* msum);
				int Mat_Inverse_2X2(CMatrix2x2* m, CMatrix2x2* mi);
				int Solve_2X2_System(CMatrix2x2* A, MATRIX1X2_PTR X, MATRIX1X2_PTR B);
				void Copy(const CMatrix2x2& mSrc);
				void Copy(const CMatrix2x2* mSrc);
			} MATRIX2X2, *LPMATRIX2X2, *MATRIX2X2_PTR;

			typedef class CMatrix3x2
			{
			public:
				union
				{
					float M[3][2]; // array indexed data storage

					// storage in row major form with explicit names
					struct
					{
						float M00, M01;
						float M10, M11;
						float M20, M21;
					}; // end explicit names

				}; // end union
			public:
				int Mat_Init_3X2(CMatrix3x2* ma,float m00, float m01,float m10, float m11,float m20, float m21);

				int Mat_Mul_1X2_3X2(CMatrix1x2* ma,	CMatrix3x2* mb,	CMatrix1x2* mprod);
			} MATRIX3X2, *LPMATRIX3X2, *MATRIX3X2_PTR;

			typedef class CMatrix3x3
			{
			public:
				union
				{
					float M[3][3]; // array indexed data storage

					// storage in row major form with explicit names
					struct
					{
						float M00, M01, M02;
						float M10, M11, M12;
						float M20, M21, M22;
					}; // end explicit names

				}; // end union

			public:
				inline void MAT_TRANSPOSE_3X3(CMatrix3x3* m)
				{
					MATRIX3X3 mt;
					mt.M00 = m->M00; mt.M01 = m->M10; mt.M02 = m->M20;
					mt.M10 = m->M01; mt.M11 = m->M11; mt.M12 = m->M21;
					mt.M20 = m->M02; mt.M21 = m->M12; mt.M22 = m->M22;
					memcpy((void *)m, (void *)&mt, sizeof(MATRIX3X3));
				}

				inline void MAT_TRANSPOSE_3X3(CMatrix3x3* m, CMatrix3x3* mt)
				{
					mt->M00 = m->M00; mt->M01 = m->M10; mt->M02 = m->M20;
					mt->M10 = m->M01; mt->M11 = m->M11; mt->M12 = m->M21;
					mt->M20 = m->M02; mt->M21 = m->M12; mt->M22 = m->M22;
				}

				inline void MAT_COLUMN_SWAP_3X3(CMatrix3x3* m, int c, MATRIX1X3_PTR v)
				{
					m->M[0][c] = v->M[0]; m->M[1][c] = v->M[1]; m->M[2][c] = v->M[2];
				}


			public:
				void Mat_Add_3X3(CMatrix3x3* ma, CMatrix3x3* mb, CMatrix3x3* msum);
				void Mat_Mul_VECTOR3D_3X3( CVector3D*  va, CMatrix3x3* mb, CVector3D*  vprod);
				int Mat_Inverse_3X3(CMatrix3x3* m, CMatrix3x3* mi);
				void Mat_Init_3X3(CMatrix3x3* ma,
					float m00, float m01, float m02,
					float m10, float m11, float m12,
					float m20, float m21, float m22);
				float Mat_Det_3X3(CMatrix3x3* m);
				int Solve_3X3_System(CMatrix3x3* A, MATRIX1X3_PTR X, MATRIX1X3_PTR B);
				int Mat_Mul_3X3(CMatrix3x3* ma,	CMatrix3x3* mb,	CMatrix3x3* mprod);
				void Copy(const CMatrix3x3& mSrc);
				void Copy(const CMatrix3x3* mSrc);

				int Mat_Mul_1X3_3X3(CMatrix1x3* ma,	CMatrix3x3* mb,	CMatrix1x3* mprod);
			}MATRIX3X3, *LPMATRIX3X3, *MATRIX3X3_PTR;

			typedef class CMatrix4x3
			{
			public:
				union
				{
					float M[4][3]; // array indexed data storage

					// storage in row major form with explicit names
					struct
					{
						float M00, M01, M02;
						float M10, M11, M12;
						float M20, M21, M22;
						float M30, M31, M32;
					}; // end explicit names

				}; // end union

			public:
				inline void MAT_COLUMN_SWAP_4X3(CMatrix4x3* m, int c, MATRIX1X4_PTR v)
				{
					m->M[0][c] = v->M[0]; m->M[1][c] = v->M[1]; m->M[2][c] = v->M[2]; m->M[3][c] = v->M[3];
				}

			public:
				
				void Mat_Mul_VECTOR3D_4X3(CVector3D*  va, CMatrix4x3* mb, CVector3D*  vprod);
				
				void Mat_Mul_VECTOR4D_4X3(CVector4D*  va, CMatrix4x3* mb, CVector4D*  vprod);
				
			} MATRIX4X3, *LPMATRIX4X3, *MATRIX4X3_PTR;

			typedef class SLOONGMATH_API CMatrix4x4 : CMathBase
			{
			public:
				CMatrix4x4();
				CMatrix4x4(const CMatrix4x4 &m);
				CMatrix4x4(float r11, float r12, float r13, float r14,
					float r21, float r22, float r23, float r24,
					float r31, float r32, float r33, float r34,
					float r41, float r42, float r43, float r44);

				virtual ~CMatrix4x4();
			public:
				union
				{
					float M[4][4]; // array indexed data storage

					// storage in row major form with explicit names
					struct
					{
						float M00, M01, M02, M03;
						float M10, M11, M12, M13;
						float M20, M21, M22, M23;
						float M30, M31, M32, M33;
					}; // end explicit names

				}; // end union

			public:
				void operator=(CMatrix4x4 &m);
				CMatrix4x4 operator-(CMatrix4x4 &m);
				CMatrix4x4 operator+(CMatrix4x4 &m);
				CMatrix4x4 operator*(CMatrix4x4 &m);

				CMatrix4x4 operator*(float f);
				CMatrix4x4 operator/(float f);

				void operator +=(CMatrix4x4 &m);
				void operator -=(CMatrix4x4 &m);
				void operator *=(CMatrix4x4 &m);

				void operator *=(float f);
				void operator /=(float f);

			public:
				inline void MAT_TRANSPOSE_4X4(CMatrix4x4* m)
				{
					MATRIX4X4 mt;
					mt.M00 = m->M00; mt.M01 = m->M10; mt.M02 = m->M20; mt.M03 = m->M30;
					mt.M10 = m->M01; mt.M11 = m->M11; mt.M12 = m->M21; mt.M13 = m->M31;
					mt.M20 = m->M02; mt.M21 = m->M12; mt.M22 = m->M22; mt.M23 = m->M32;
					mt.M30 = m->M03; mt.M31 = m->M13; mt.M32 = m->M22; mt.M33 = m->M33;
					memcpy((void *)m, (void *)&mt, sizeof(MATRIX4X4));
				}

				inline void MAT_TRANSPOSE_4X4(CMatrix4x4* m, CMatrix4x4* mt)
				{
					mt->M00 = m->M00; mt->M01 = m->M10; mt->M02 = m->M20; mt->M03 = m->M30;
					mt->M10 = m->M01; mt->M11 = m->M11; mt->M12 = m->M21; mt->M13 = m->M31;
					mt->M20 = m->M02; mt->M21 = m->M12; mt->M22 = m->M22; mt->M23 = m->M32;
					mt->M30 = m->M03; mt->M31 = m->M13; mt->M32 = m->M22; mt->M33 = m->M33;
				}

				inline void MAT_COLUMN_SWAP_4X4(CMatrix4x4* m, int c, MATRIX1X4_PTR v)
				{
					m->M[0][c] = v->M[0]; m->M[1][c] = v->M[1]; m->M[2][c] = v->M[2]; m->M[3][c] = v->M[3];
				}

			public:
				void Identity();
				void Initialize(float m00, float m01, float m02, float m03,
					float m10, float m11, float m12, float m13,
					float m20, float m21, float m22, float m23,
					float m30, float m31, float m32, float m33);

				void Initialize(const CMatrix4x4& mSrc);

				void Copy(const CMatrix4x4& mSrc);

				void Zero();
				// ������������ˣ���������洢�ڱ���
				void Multiply(CMatrix4x4* ma, CMatrix4x4* mb);
				void Translate(CVector3D &Translate);
				void Translate(float x, float y, float z);
				void inverseTranslate();

				void Rotate(double angle, int x, int y, int z);

				CVector3D VectorMatrixMultiply(CVector3D &v);
				CVector3D VectorMatrixMultiply3x3(CVector3D &v);

				CVector3D Multiply( const CVector3D& va);
				CVector4D Multiply( const CVector4D& va) const;
				CVector4D Multiply( const CVector4D* va) const;
			public:

				void Mat_Add_4X4(CMatrix4x4* ma, CMatrix4x4* mb, CMatrix4x4* msum);
				
				void Mat_Mul_1X4_4X4(MATRIX1X4_PTR ma, CMatrix4x4* mb, MATRIX1X4_PTR mprod);
				
				int Mat_Inverse_4X4(CMatrix4x4* m, CMatrix4x4* mi);
				void BuildMoveMatrix(const CVector4D& vMove);
				void BuildRotateMatrix(double x, double y, double z);
			} MATRIX4X4, *LPMATRIX4X4, *MATRIX4X4_PTR;
		}
	}
}

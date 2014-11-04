#include "stdafx.h"

#include "SloongMath.h"
using namespace SoaringLoong::SloongMath;

CMatrix::CMatrix()
{
	m_nColumn = 0;
	m_nRow = 0;
	m_pMatrix = NULL;
}

CMatrix::CMatrix( UINT unRow, UINT unColumn, double* pValue )
{
	m_nColumn = 0;
	m_nRow = 0;
	m_pMatrix = NULL;
	if ( unColumn == 0 || unRow == 0 )
	{
		return;
	}
	else
	{
		m_nColumn = (int)unColumn;
		m_nRow = (int)unRow;
		m_pMatrix = new double*[m_nColumn];
		for (int i = 0; i < m_nColumn; i++)
		{
			m_pMatrix[i] = new double[m_nRow];
			for (int j = 0; j < m_nRow; j++)
			{
				m_pMatrix[i][j] = pValue[i*m_nRow+j];
			}
		}
	}
}

CMatrix::CMatrix( const CMatrix& oOrg )
{
	this->m_nColumn = oOrg.m_nColumn;
	this->m_nRow = oOrg.m_nRow;
	this->m_pMatrix = new double*[m_nColumn];
	for ( int i = 0; i < m_nColumn; i++ )
	{
		this->m_pMatrix[i] = new double[m_nRow];
		for ( int j = 0; j < m_nRow; j++ )
		{
			this->m_pMatrix[i][j] = oOrg.m_pMatrix[i][j];
		}
	}
}

CMatrix::~CMatrix()
{
	try
	{
		for (int i = 0; i < m_nColumn; i++)
		{
			SAFE_DELETE_ARR(m_pMatrix[i]);
		}
		SAFE_DELETE_ARR(m_pMatrix);
	}
	catch(...)
	{

	}
}

void CMatrix::Identity()
{
	for ( int i = 0; i < m_nColumn; i++ )
	{
		for (int j = 0; j < m_nRow; j++)
		{
			if ( i == j )
			{
				m_pMatrix[i][j] = 1.0;
			}
			else
			{
				m_pMatrix[i][j] = 0.0;
			}
		}
	}
}

void CMatrix::operator =(CMatrix& m)
{
	if ( m.m_nRow == m_nRow && m.m_nColumn == m_nColumn )
	{
		for (int i = 0; i < m_nColumn; i++)
		{
			for (int j = 0; j < m_nRow; j++)
			{
				this->m_nColumn = m.m_nColumn;
				this->m_nRow = m.m_nRow;
				this->m_pMatrix[i][j] = m.m_pMatrix[i][j];
			}
		}
	}
}

CMatrix CMatrix::operator +(CMatrix& m)
{
	double* pResult = new double[m_nColumn*m_nRow];
	for (int i = 0; i < m_nColumn; i++)
	{
		for (int j = 0; j < m_nRow; j++)
		{
			pResult[i*m_nRow+j] = m_pMatrix[i][j] + m.m_pMatrix[i][j];
		}
	}
	CMatrix pTemp( m_nColumn, m_nRow, pResult );
	delete[] pResult;
	return pTemp;
}

void CMatrix::operator +=(CMatrix& m)
{
	for (int i = 0; i < m_nColumn; i++)
	{
		for (int j = 0; j < m_nRow; j++)
		{
			m_pMatrix[i][j] = m_pMatrix[i][j] + m.m_pMatrix[i][j];
		}
	}
}

CMatrix CMatrix::operator -(CMatrix& m)
{
	if( this->m_nColumn != m.m_nColumn || this->m_nRow != m.m_nRow )
	{
		throw TEXT("The Matrix size is defferent.");
	}
	double* pResult = new double[m_nColumn*m_nRow];
	for (int i = 0; i < m_nColumn; i++)
	{
		for (int j = 0; j < m_nRow; j++)
		{
			pResult[i*m_nRow+j] = m_pMatrix[i][j] - m.m_pMatrix[i][j];
		}
	}
	CMatrix pTemp( m_nColumn, m_nRow, pResult );
	delete[] pResult;
	return pTemp;
}

void CMatrix::operator-=(CMatrix& m)
{
	for (int i = 0; i < m_nColumn; i++)
	{
		for (int j = 0; j < m_nRow; j++)
		{
			m_pMatrix[i][j] = m_pMatrix[i][j] - m.m_pMatrix[i][j];
		}
	}
}

CMatrix CMatrix::operator *(CMatrix& m)
{
	try
	{
		if ( this->m_nColumn < m.m_nRow )
		{
			int nRowTemp = m_nRow;
			this->m_nRow = m.m_nColumn;
			double** pDataTemp = this->m_pMatrix;
			this->m_pMatrix = new double*[this->m_nRow];
			for ( int i = 0; i < m.m_nRow; i++ )
			{
				m_pMatrix[i] = new double[this->m_nRow];
				for ( int j = 0; j < this->m_nColumn; j++ )
				{
					m_pMatrix[i][j] = 0.0;
				}
			}

			for ( int i = 0; i < nRowTemp; i++ )
			{
				for (int j = 0; j < this->m_nColumn; j++)
				{
					this->m_pMatrix[i][j] = pDataTemp[i][j];
				}
			}

			SAFE_DELETE_ARR(pDataTemp);
		}
		else if ( this->m_nRow > m.m_nColumn )
		{

		}

		double* pResult = new double[m_nRow*m.m_nColumn];
		for (int i = 0; i < m_nRow; i++)
		{
			for (int j = 0; j < m.m_nColumn; j++)
			{
				pResult[i*m.m_nColumn+j] = 0.0;
				for (int t = 0; t < m_nColumn; t++)
				{
					pResult[i*m.m_nColumn+j] += m_pMatrix[i][t] * m.m_pMatrix[t][j];
				}
			}
		}
		CMatrix pTemp( m.m_nRow, this->m_nColumn, pResult );
		delete[] pResult;
		return pTemp;
	}
	catch( ... )
	{
		throw;
	}
}

void CMatrix::operator*=(CMatrix& m)
{
	if ( m.m_nColumn == this->m_nRow )
	{
		// Compute the result
		double* pResult = new double[m_nColumn*m.m_nRow];
		for (int i = 0; i < m_nColumn; i++)
		{
			for (int j = 0; j < m.m_nRow; j++)
			{
				pResult[i*m.m_nRow+j] = 0.0;
				for (int t = 0; t < m_nRow; t++)
				{
					pResult[i*m.m_nRow+j] += m_pMatrix[i][t] * m.m_pMatrix[t][j];
				}
			}
		}

		// Reset the size
		m_nRow = m.m_nRow;
		for (int i = 0; i < m_nColumn; i++)
		{
			SAFE_DELETE_ARR(m_pMatrix[i]);
			m_pMatrix[i] = new double[m_nRow];
			for (int j = 0; j < m_nRow; j++)
			{
				m_pMatrix[i][j] = pResult[i*m_nRow+j];
			}
		}
	}
}

CMatrix CMatrix::operator*(double dValue)
{
	double* pTmp = new double[m_nColumn*m_nRow];
	for ( int i = 0; i < m_nColumn; i++ )
	{
		for (int j = 0; j < m_nRow; j++)
		{
			pTmp[i*m_nRow+j] = m_pMatrix[i][j] * dValue;
		}
	}
	CMatrix pTemp( m_nColumn, m_nRow, pTmp );
	SAFE_DELETE_ARR(pTmp);
	return pTemp;
}

void CMatrix::operator*=(double dValue)
{
	(*this) = (*this) * dValue;
}

CMatrix CMatrix::operator/(double dValue)
{
	if ( 0 == dValue )
	{
		dValue = 1;
	}
	dValue = 1/dValue;

	double* pTmp = new double[m_nColumn*m_nRow];
	for ( int i = 0; i < m_nColumn; i++ )
	{
		for (int j = 0; j < m_nRow; j++)
		{
			pTmp[i*m_nRow+j] = m_pMatrix[i][j] * dValue;
		}
	}
	CMatrix pTemp( m_nColumn, m_nRow, pTmp );
	SAFE_DELETE_ARR(pTmp);
	return pTemp;
}

void CMatrix::operator/=(double dVaule)
{
	(*this) = (*this) / dVaule;
}


CMatrix4x4::CMatrix4x4(const CMatrix4x4 &m)
{
	matrix[0][0]  = m.matrix[0][0];
	matrix[0][1]  = m.matrix[0][1];
	matrix[0][2]  = m.matrix[0][2];
	matrix[0][3]  = m.matrix[0][3];

	matrix[1][0]  = m.matrix[1][0];
	matrix[1][1]  = m.matrix[1][1];
	matrix[1][2]  = m.matrix[1][2];
	matrix[1][3]  = m.matrix[1][3];

	matrix[2][0]  = m.matrix[2][0];
	matrix[2][1]  = m.matrix[2][1];
	matrix[2][2]  = m.matrix[2][2];
	matrix[2][3]  = m.matrix[2][3];

	matrix[3][0]  = m.matrix[3][0];
	matrix[3][1]  = m.matrix[3][1];
	matrix[3][2]  = m.matrix[3][2];
	matrix[3][3]  = m.matrix[3][3];
}


CMatrix4x4::CMatrix4x4(float r11, float r12, float r13, float r14,
					   float r21, float r22, float r23, float r24,
					   float r31, float r32, float r33, float r34,
					   float r41, float r42, float r43, float r44)
{
	matrix[0][0]  = r11; matrix[0][1]  = r12; matrix[0][2]  = r13; matrix[0][3]  = r14;
	matrix[1][0]  = r21; matrix[1][1]  = r22; matrix[1][2]  = r23; matrix[1][3]  = r24;
	matrix[2][0]  = r31; matrix[2][1]  = r32; matrix[2][2]  = r33; matrix[2][3]  = r34;
	matrix[3][0]  = r41; matrix[3][1]  = r42; matrix[3][2]  = r43; matrix[3][3]  = r44;
}


void CMatrix4x4::Identity()
{
	matrix[0][0] = 1.0f; matrix[0][1] = 0.0f; matrix[0][2] = 0.0f; matrix[0][3] = 0.0f;
	matrix[1][0] = 0.0f; matrix[1][1] = 1.0f; matrix[1][2] = 0.0f; matrix[1][3] = 0.0f;
	matrix[2][0] = 0.0f; matrix[2][1] = 0.0f; matrix[2][2] = 1.0f; matrix[2][3] = 0.0f;
	matrix[3][0] = 0.0f; matrix[3][1] = 0.0f; matrix[3][2] = 0.0f; matrix[3][3] = 1.0f;
}


void CMatrix4x4::operator =(CMatrix4x4 &m)
{
	matrix[0][0]  = m.matrix[0][0];
	matrix[0][1]  = m.matrix[0][1];
	matrix[0][2]  = m.matrix[0][2];
	matrix[0][3]  = m.matrix[0][3];

	matrix[1][0]  = m.matrix[1][0];
	matrix[1][1]  = m.matrix[1][1];
	matrix[1][2]  = m.matrix[1][2];
	matrix[1][3]  = m.matrix[1][3];

	matrix[2][0]  = m.matrix[2][0];
	matrix[2][1]  = m.matrix[2][1];
	matrix[2][2]  = m.matrix[2][2];
	matrix[2][3]  = m.matrix[2][3];

	matrix[3][0]  = m.matrix[3][0];
	matrix[3][1]  = m.matrix[3][1];
	matrix[3][2]  = m.matrix[3][2];
	matrix[3][3]  = m.matrix[3][3];
}


CMatrix4x4 CMatrix4x4::operator -(CMatrix4x4 &m)
{
	return CMatrix4x4(	matrix[0][0]- m.matrix[0][0], matrix[0][1]- m.matrix[0][1], matrix[0][2]- m.matrix[0][2], matrix[0][3]- m.matrix[0][3], 
		matrix[1][0]- m.matrix[1][0], matrix[1][1]- m.matrix[1][1], matrix[1][2]- m.matrix[1][2], matrix[1][3]- m.matrix[1][3], 
		matrix[2][0]- m.matrix[2][0], matrix[2][1]- m.matrix[2][1], matrix[2][2]- m.matrix[2][2], matrix[2][1]- m.matrix[2][3],
		matrix[3][0]- m.matrix[3][0], matrix[3][1]- m.matrix[3][1], matrix[3][2]- m.matrix[3][2], matrix[3][3]- m.matrix[3][3]);
}


CMatrix4x4 CMatrix4x4::operator +(CMatrix4x4 &m)
{
	return CMatrix4x4(	matrix[0][0]+ m.matrix[0][0], matrix[0][1]+ m.matrix[0][1], matrix[0][2]+ m.matrix[0][2], matrix[0][3]+ m.matrix[0][3], 
		matrix[1][0]+ m.matrix[1][0], matrix[1][1]+ m.matrix[1][1], matrix[1][2]+ m.matrix[1][2], matrix[1][3]+ m.matrix[1][3], 
		matrix[2][0]+ m.matrix[2][0], matrix[2][1]+ m.matrix[2][1], matrix[2][2]+ m.matrix[2][2], matrix[2][3]+ m.matrix[2][3],
		matrix[3][0]+ m.matrix[3][0], matrix[3][1]+ m.matrix[3][1], matrix[3][2]+ m.matrix[3][2], matrix[3][3]+ m.matrix[3][3]);
}


CMatrix4x4 CMatrix4x4::operator *(CMatrix4x4 &m)
{
	float fLine1[4] = {0.0f};
	fLine1[0] = matrix[0][0]* m.matrix[0][0]+ matrix[1][0]* m.matrix[0][1]+ matrix[2][0]* m.matrix[0][2]+ matrix[3][0]* m.matrix[0][3];
	fLine1[1] = matrix[0][1]* m.matrix[0][0]+ matrix[1][1]* m.matrix[0][1]+ matrix[2][1]* m.matrix[0][2]+ matrix[3][1]* m.matrix[0][3];
	fLine1[2] = matrix[0][2]* m.matrix[0][0]+ matrix[1][2]* m.matrix[0][1]+ matrix[2][2]* m.matrix[0][2]+ matrix[3][2]* m.matrix[0][3];
	fLine1[3] = matrix[0][3]* m.matrix[0][0]+ matrix[1][3]* m.matrix[0][1]+ matrix[2][3]* m.matrix[0][2]+ matrix[3][3]* m.matrix[0][3];

	float fLine2[4] = {0.0f};
	fLine2[0] = matrix[0][0]* m.matrix[1][0]+ matrix[1][0]* m.matrix[1][1]+ matrix[2][0]* m.matrix[1][2]+ matrix[3][0]* m.matrix[1][3];
	fLine2[1] = matrix[0][1]* m.matrix[1][0]+ matrix[1][1]* m.matrix[1][1]+ matrix[2][1]* m.matrix[1][2]+ matrix[3][1]* m.matrix[1][3];
	fLine2[2] = matrix[0][2]* m.matrix[1][0]+ matrix[1][2]* m.matrix[1][1]+ matrix[2][2]* m.matrix[1][2]+ matrix[3][2]* m.matrix[1][3];
	fLine2[3] = matrix[0][3]* m.matrix[1][0]+ matrix[1][3]* m.matrix[1][1]+ matrix[2][3]* m.matrix[1][2]+ matrix[3][3]* m.matrix[1][3];

	float fLine3[4] = {0.0f};
	fLine3[0] = matrix[0][0]* m.matrix[2][0]+ matrix[1][0]* m.matrix[2][1]+ matrix[2][0]* m.matrix[2][2]+ matrix[3][0]* m.matrix[2][3];
	fLine3[1] = matrix[0][1]* m.matrix[2][0]+ matrix[1][1]* m.matrix[2][1]+ matrix[2][1]* m.matrix[2][2]+ matrix[3][1]* m.matrix[2][3];
	fLine3[2] = matrix[0][2]* m.matrix[2][0]+ matrix[1][2]* m.matrix[2][1]+ matrix[2][2]* m.matrix[2][2]+ matrix[3][2]* m.matrix[2][3];
	fLine3[3] = matrix[0][3]* m.matrix[2][0]+ matrix[1][3]* m.matrix[2][1]+ matrix[2][3]* m.matrix[2][2]+ matrix[3][3]* m.matrix[2][3];

	float fLine4[4] = {0.0f};
	fLine4[0] = matrix[0][0]* m.matrix[3][0]+ matrix[1][0]* m.matrix[3][1]+ matrix[2][0]* m.matrix[3][2]+ matrix[3][0]* m.matrix[3][3];
	fLine4[1] = matrix[0][1]* m.matrix[3][0]+ matrix[1][1]* m.matrix[3][1]+ matrix[2][1]* m.matrix[3][2]+ matrix[3][1]* m.matrix[3][3];
	fLine4[2] = matrix[0][2]* m.matrix[3][0]+ matrix[1][2]* m.matrix[3][1]+ matrix[2][2]* m.matrix[3][2]+ matrix[3][2]* m.matrix[3][3];
	fLine4[3] = matrix[0][3]* m.matrix[3][0]+ matrix[1][3]* m.matrix[3][1]+ matrix[2][3]* m.matrix[3][2]+ matrix[3][3]* m.matrix[3][3];

	return CMatrix4x4(	fLine1[0], fLine1[1], fLine1[2], fLine1[3],
		fLine2[0], fLine2[1], fLine2[2], fLine2[3],
		fLine3[0], fLine3[1], fLine3[2], fLine3[3],
		fLine4[0], fLine4[1], fLine4[2], fLine4[3]);
}


CMatrix4x4 CMatrix4x4::operator *(float f)
{
	return CMatrix4x4(	matrix[0][0] * f, matrix[0][1] * f, matrix[0][2] * f, matrix[0][3] * f,
		matrix[1][0] * f, matrix[1][1] * f, matrix[1][2] * f, matrix[1][3] * f,
		matrix[2][0] * f, matrix[2][1] * f, matrix[2][2] * f, matrix[2][3] * f,
		matrix[3][0] * f, matrix[3][1] * f, matrix[3][2] * f, matrix[3][3] * f);
}


CMatrix4x4 CMatrix4x4::operator /(float f)
{
	if(f == 0) f = 1;
	f = 1/f;

	return CMatrix4x4(	matrix[0][0] * f, matrix[0][1] * f, matrix[0][2] * f, matrix[0][3] * f,
		matrix[1][0] * f, matrix[1][1] * f, matrix[1][2] * f, matrix[1][3] * f,
		matrix[2][0] * f, matrix[2][1] * f, matrix[2][2] * f, matrix[2][3] * f,
		matrix[3][0] * f, matrix[3][1] * f, matrix[3][2] * f, matrix[3][3] * f);
}


void CMatrix4x4::operator +=(CMatrix4x4 &m)
{
	(*this) = (*this) + m;
}


void CMatrix4x4::operator -=(CMatrix4x4 &m)
{
	(*this) = (*this) - m;
}


void CMatrix4x4::operator *=(CMatrix4x4 &m)
{
	(*this) = (*this) * m;
}


void CMatrix4x4::operator *=(float f)
{
	(*this) = (*this) * f;
}


void CMatrix4x4::operator /=(float f)
{
	(*this) = (*this) / f;
}


void CMatrix4x4::Translate(CVector3 &Translate)
{
	matrix[3][0] = Translate.x;
	matrix[3][1] = Translate.y;
	matrix[3][2]= Translate.z;
	matrix[3][3]= 1.0f;
}


void CMatrix4x4::Translate(float x, float y, float z)
{
	matrix[3][0] = x;
	matrix[3][1] = y;
	matrix[3][2]= z;
	matrix[3][3]= 1.0f;
}


void CMatrix4x4::inverseTranslate()
{
	matrix[3][0] = -matrix[3][0];
	matrix[3][1] = -matrix[3][1];
	matrix[3][2] = -matrix[3][2];
}


void CMatrix4x4::Rotate(double angle, int x, int y, int z)
{
	angle = angle * PI / 180;
	float cosAngle = cosf((float)angle);
	float sineAngle = sinf((float)angle);

	if(z)
	{
		matrix[0][0]= cosAngle;
		matrix[0][1]= sineAngle;
		matrix[1][0]= -sineAngle;
		matrix[1][1]= cosAngle;
	}

	if(y)
	{
		matrix[0][0] = cosAngle;
		matrix[0][2] = -sineAngle;
		matrix[2][0] = sineAngle;
		matrix[2][2] = cosAngle;
	}

	if(x)
	{
		matrix[1][1] = cosAngle;
		matrix[1][2] = sineAngle;
		matrix[2][1] = -sineAngle;
		matrix[2][2] = cosAngle;
	}
}


CVector3 CMatrix4x4::VectorMatrixMultiply(CVector3 &v)
{
	CVector3 out;

	out.x = (v.x * matrix[0][0]) + (v.y * matrix[1][0]) + (v.z * matrix[2][0]) + matrix[3][0];
	out.y = (v.x * matrix[0][1]) + (v.y * matrix[1][1]) + (v.z * matrix[2][1]) + matrix[3][1];
	out.z = (v.x * matrix[0][2]) + (v.y * matrix[1][2]) + (v.z * matrix[2][2]) + matrix[3][2];

	return out;
}


CVector3 CMatrix4x4::VectorMatrixMultiply3x3(CVector3 &v)
{
	CVector3 out;

	out.x = (v.x * matrix[0][0]) + (v.y * matrix[1][0]) + (v.z * matrix[2][0]);
	out.y = (v.x * matrix[0][1]) + (v.y * matrix[1][1]) + (v.z * matrix[2][1]);
	out.z = (v.x * matrix[0][2]) + (v.y * matrix[1][2]) + (v.z * matrix[2][2]);

	return out;
}
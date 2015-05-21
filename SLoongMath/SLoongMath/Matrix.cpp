#include "stdafx.h"

#include "SloongMath.h"
using namespace Sloong::Math;

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
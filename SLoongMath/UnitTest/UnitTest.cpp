// UnitTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "SLMath.h"
#pragma comment(lib,"SLMath.lib")

using namespace SoaringLoong::SLMath;

int _tmain(int argc, _TCHAR* argv[])
{
	double pTemp[] = {1,2,3,4};
 	CMatrix Test( 2,2, pTemp );
	_tprintf(TEXT("1\n"));
	for ( int i = 0; i< Test.m_nRow; i++)
	{
		_tprintf(TEXT("|"));
		for (int j = 0; j < Test.m_nColumn; j++)
		{
			_tprintf(TEXT("%f,"),Test.m_pMatrix[i][j]);
		}
		_tprintf(TEXT("|\n"));
	}

	double temp2[] = {5,6,7,11,
						8,9,10, 12};
	CMatrix Test2( 2,4,temp2 );
	_tprintf(TEXT("2\n"));
	for ( int i = 0; i< Test2.m_nRow; i++)
	{
		_tprintf(TEXT("|"));
		for (int j = 0; j < Test2.m_nColumn; j++)
		{
			_tprintf(TEXT("%f,"),Test2.m_pMatrix[i][j]);
		}
		_tprintf(TEXT("|\n"));
	}

	try
	{
		double pBigData[] = {1,2,3,4,5,6,7,8,9};
		CMatrix oBigMatrix(3,3,pBigData);
		double pSmallData[] = {1,2,3,4};
		CMatrix oSmallMatrix(2,2,pSmallData);
		CMatrix oResMatrix = oSmallMatrix*oBigMatrix;
		_tprintf(TEXT("Res\n"));
		for ( int i = 0; i< oResMatrix.m_nRow; i++)
		{
			_tprintf(TEXT("|"));
			for (int j = 0; j < oResMatrix.m_nColumn; j++)
			{
				_tprintf(TEXT("%f,"),oResMatrix.m_pMatrix[i][j]);
			}
			_tprintf(TEXT("|\n"));
		}
		CMatrix testRes = Test*Test2;
		_tprintf(TEXT("Res\n"));
		for ( int i = 0; i< testRes.m_nRow; i++)
		{
			_tprintf(TEXT("|"));
			for (int j = 0; j < testRes.m_nColumn; j++)
			{
				_tprintf(TEXT("%f,"),testRes.m_pMatrix[i][j]);
			}
			_tprintf(TEXT("|\n"));
		}
	}
	catch( LPCTSTR strThrowText )
	{
		_tprintf(strThrowText);
	}
	
	_tsystem(TEXT("pause"));

	return 0;
}


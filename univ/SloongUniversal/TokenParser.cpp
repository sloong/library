// /************************************************************************/
// /* 				TokenParser.cpp --- Token Parser Implement              */
// /************************************************************************/
 #include "stdafx.h"
// #include "Universal/CUniversal.h"
// 
// // This is Token max size
// #define TOKEN_SIZE	256	
// 
// using namespace SoaringLoong;
// 
// //--- 0000014 --- 2013/10/25 --- WCB --- Add
// // Remarks:
// //		Construction function.
// CTokenParser::CTokenParser(void)
// 	: m_unTotalTokens(0)
// {
// }
// 
// //--- 0000014 --- 2013/10/25 --- WCB --- Add
// // Remarks:
// //		Destruct function.
// CTokenParser::~CTokenParser(void)
// {
// }
// 
// //--- 0000014 --- 2013/10/25 --- WCB --- Add
// // Remarks:
// //		Initialize function. Load file and set the member variable value.
// HRESULT CTokenParser::Initialize( LPCTSTR strFilePath )
// {
// 	FILE* pFile;
// 	errno_t nErr;
// 	nErr = _tfopen_s( &pFile, strFilePath, TEXT("rb,ccs=unicode"));
// 
// 	// Display errors if any.
// 	if( NULL == pFile || 0 != nErr )
// 	{
// 		return S_FALSE;
// 	}
// 
// 	// Set stream point position to file end.
// 	fseek(pFile, 0, SEEK_END);
// 	// Get the length of the file.
// 	int length = ftell(pFile);
// 	// Set stream point position to file head.
// 	fseek(pFile, 0, SEEK_SET);
// 
// 	// Create buffer 
// 	LPTSTR strFileData = new TCHAR[(length + 1) * sizeof(TCHAR)];
// 
// 	if( NULL == strFileData ) 
// 	{
// #define S_ERROR_CREATE_BUFFER	1
// 		// TODO: no just temp define. in last version. it will move to new library
// 		return S_ERROR_CREATE_BUFFER;
// 	}
// 
// 	// Read file stream to Buffer.
// 	fread( strFileData, sizeof(TCHAR), length , pFile);
// 	strFileData[length] = '\0';
// 
// 	// Close the file when we are done.
// 	fclose(pFile);
// 
// 	// Set our file to our lexer.
// 	SetTokenStream( strFileData );
// 
// 	// No longer need.
// 	if ( NULL != strFileData )
// 	{
// 		delete[] strFileData; 
// 		strFileData = NULL;
// 	}
// 	
// 	// Get total tokens
// 	TCHAR strTmpBuff[TOKEN_SIZE] = { 0 };
// 
// 	// Get all token amount
// 	while( S_OK == GetNextToken(strTmpBuff,TOKEN_SIZE) )
// 	{
// 		// is not a valid value
// 		if('\0' != strTmpBuff[0])
// 		{
// 			m_unTotalTokens++;
// 		}
// 	}
// 	
// 	// Reset
// 	Reset();
// 
// 	return S_OK;
// }
// 
// void CTokenParser::Shutdown()
// {
// 	if( NULL != m_strData)
// 	{
// 		delete[] m_strData;
// 	}
// 	m_strData = NULL;
// 
// 	m_unLength = m_unStartIndex = m_unEndIndex = 0;
// 	delete this;
// }
// 
// 
// // Remarks:
// // 		Just used in TokenStream Mode
// void CTokenParser::Reset()
// {
// 	m_unStartIndex = m_unEndIndex = 0;
// }
// 
// //--- 0000014 --- 2013/10/24 --- WCB --- Add
// // Remarks:
// //		Just used in TokenStream Mode
// HRESULT CTokenParser::SetTokenStream( LPTSTR strData )
// {
// 	if ( NULL == strData )
// 	{
// 		return S_FALSE;
// 	}
// 
// 	Shutdown();
// 
// 	//--- 0000017 --- 2013/10/25 --- WCB --- Modify
// 	m_unLength = (UINT)_tcslen( strData );
// 
// 	// Create buffer
// 	m_strData = new TCHAR[(m_unLength+1)*sizeof(TCHAR)];
// 	if ( NULL == m_strData )
// 	{
// 		return S_ERROR_CREATE_BUFFER;
// 	}
// 	_tcscpy_s( m_strData, m_unLength+1, strData );
// 	m_strData[m_unLength] = '\0';
// 
// 	return S_OK;
// }
// 
// //--- 0000014 --- 2013/10/24 --- WCB --- Add
// // Remarks:
// //		Just used in TokenStream Mode
// HRESULT CTokenParser::GetNextToken( LPTSTR strBuffer, UINT unBufSize )
// {
// 	// value init. inString is the "" mark, is can be three value, -1 is no in string , 0 is default value, 1 is in string.
// 	int inString = 0;
// 	// Set start index value to end index, so , it will find next.
// 	m_unStartIndex = m_unEndIndex;
// 	if ( NULL != strBuffer )
// 	{
// 		strBuffer[0] = '\0';
// 	}
// 
// 	// Find valid start index. if is out of file length or is space(" "), or line break("\t"), ignore it.
// 	// TODO: in here, i think the inString is no need, because it init value is false, and want change it, the data must is '"', but, when data is '"',
// 	// the while is false, so it can not change the inSting value.
// 	// now, i change it to if find '"', mark it.
// 	while ( m_unStartIndex < m_unLength && (( ' ' == m_strData[m_unStartIndex] || '\t' == m_strData[m_unStartIndex] ) || '"' == m_strData[m_unStartIndex] ))
// 	{
// 		// find '"',mark it, now the end mark is '"', so if file only have one '"', it will make process error.
// 		if ( '"' == m_strData[m_unStartIndex] )
// 		{
// 			inString = 1;
// 			// In here no need break operator, because it will delete the first '"'.
// 			//break;
// 		}
// 		m_unStartIndex++;
// 	}
// 
// 	m_unEndIndex = m_unStartIndex + 1;
// 
// 	if ( m_unStartIndex < m_unLength )
// 	{
// 		bool valid =  true;
// 
// 		// Check start value.
// 		if ( true == IsValidIdentifier(m_strData[m_unStartIndex]) )
// 		{
// 			// Find end mark, In string mode, end mark is '"'.
// 			if ( 1 == inString )
// 			{
// 				while ( 1 == inString )
// 				{
// 					if ( '"' == m_strData[m_unEndIndex] )
// 					{
// 						inString = -1;
// 					}
// 					m_unEndIndex++;
// 				}
// 			}
// 			// Normal mode end marks is no valid value. 
// 			else // !( 1 == inString )
// 			{
// 				while ( IsValidIdentifier(m_strData[m_unEndIndex]) || '.' == m_strData[m_unEndIndex] )
// 				{
// 					m_unEndIndex++;
// 				}
// 			}
// 			
// 		}
// 		else // !( true == IsValidIdentifier(m_strData[m_unStartIndex]) )
// 		{
// 			valid = false;
// 		}
// 
// 		if ( NULL != strBuffer )
// 		{
// 			if ( true == valid )
// 			{
// 				// Check buffer size
// 				UINT unStrLength = m_unEndIndex - m_unStartIndex;
// 				if ( unBufSize <= unStrLength )
// 				{
// #define S_ERROR_NO_ENOUGH_BUFFER	2
// 					// TODO: temp define. no enough buffer.
// 					return S_ERROR_NO_ENOUGH_BUFFER;
// 				}
// 				// In here, the three param is change the array index
// 				_tcsncpy_s( strBuffer, unBufSize, m_strData + m_unStartIndex, unStrLength );
// 				strBuffer[ unStrLength ] = '\0';
// 				if ( _tcscmp(strBuffer,TEXT("\n")) == 0 )
// 				{
// 					strBuffer[0] = '\0';
// 				}
// 			}
// 			else // !( true == valid )
// 			{
// 				strBuffer[0] = '\0';
// 			}
// 		}
// 		// Delete the end '"'.
// 		if ( -1 == inString )
// 		{
// 			strBuffer[m_unEndIndex-m_unStartIndex-1] = '\0';
// 		}
// 		return S_OK;
// 	}
// 	return S_FALSE;
// }
// 
// //--- 0000014 --- 2013/10/25 --- WCB --- Add
// // Remarks:
// //		Find a token with strTokenName, and set the Next Token to strNextTokenBuffer.
// //		if no find, return false.
// HRESULT CTokenParser::GetNextToken( LPTSTR strTokenName, LPTSTR strNextTokenBuffer, UINT nBufSize)
// {
// 	TCHAR tmpTokenName[TOKEN_SIZE];
// 
// 	// Find the Token
// 	while(GetNextToken(tmpTokenName,TOKEN_SIZE))
// 	{
// 		// Have find the Token
// 		if(_tcsicmp(tmpTokenName, strTokenName ) == 0)
// 		{
// 			// Return next token buffer.
// 			return GetNextToken( strNextTokenBuffer, nBufSize );
// 		}
// 	}
// 	return S_FALSE;
// }
// 
// bool CTokenParser::IsValidIdentifier( TCHAR c )
// {
// 	// It is valid if it falls within one of these ranges.
// 	if(	(c >= '0' && c <= '9') || 
// 		(c >= 'a' && c <= 'z') ||
// 		(c >= 'A' && c <= 'Z') || 
// 		c == '_' || 
// 		c == '"' || 
// 		c == '/' ||
// 		c == '(' ||
// 		c == ')' || 
// 		c == '-' || 
// 		c == '.')
// 		return true;
// 
// 	return false;
// }
// 
// bool CTokenParser::MoveToNextLine( LPTSTR buffer, int BufSize )
// {
// 	if(m_unStartIndex < m_unLength && m_unEndIndex < m_unLength)
// 	{
// 		m_unStartIndex = m_unEndIndex;
// 
// 		while(m_unEndIndex < m_unLength && (m_strData[m_unEndIndex] != '\n' &&
// 			m_strData[m_unEndIndex] != '\r' && m_strData[m_unEndIndex] != '\0')) m_unEndIndex++;
// 
// 		if(m_unEndIndex - m_unStartIndex >= 511) return false;
// 
// 			if( buffer != NULL)
// 			{
// 				_tcsncpy_s(buffer,BufSize, m_strData + m_unStartIndex, m_unEndIndex - m_unStartIndex);
// 				buffer[m_unEndIndex - m_unStartIndex] = '\0';
// 			}
// 	}
// 	else return false;
// 
// 	return true;
// }
// 
// int CTokenParser::GetTotalLine()
// {
// 	return m_unTotalTokens;
// }
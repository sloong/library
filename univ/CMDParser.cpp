// /************************************************************************/
// /*			 CMDParser.h --- Command Parsing Class Declaration			*/
// /************************************************************************/
 #include "stdafx.h"
// #include "error.h.h"
// #include "CUniversal.h"
// 
// using namespace Sloong;
// 
// //--- 0000011 --- 2013/10/12 --- WCB --- Add
// // Remarks:
// //		Init all numbers value.
// CCMDParser::CCMDParser()
// {
// 	m_nCurrentLine = 0;
// 	m_nCurrentLineChar = 0;
// 	m_nTotalScriptLines = 0;
// 	m_pStrScript = NULL;
// }
// 
// //--- 0000011 --- 2013/10/12 --- WCB --- Add
// // Remarks:
// //		Clear all resource.
// CCMDParser::~CCMDParser()
// {
// }
// 
// //--- 0000011 --- 2013/10/12 --- WCB --- Add
// // Remarks:
// //		Initialize CMD parser
// bool CCMDParser::Initialize( LPCTSTR strFilePath )
// {
// 	_tifstream input, input2;
// 	TCHAR tempLine[MAX_LINE_SIZE] = {0};
// 
// 	if( 0 != _taccess( strFilePath, 00 ))
// 	{
// 		return ERROR_FILE_NOT_FOUND;
// 	}
// 
// 	input.open( strFilePath );
// 
// 	if ( !input.is_open() )
// 	{
// 		return false;
// 	}
// 
// 
// 	// Open and get number of lines
// 	while ( !input.eof() )
// 	{
// 		input.getline( tempLine, MAX_LINE_SIZE , '\n' );
// 		m_nTotalScriptLines++;
// 	}
// 
// 	input.close();
// 
// 	// Re-open
// 	input2.open( strFilePath );
// 
// 	if ( !input2.is_open() )
// 	{
// 		return false;
// 	}
// 
// 	m_pStrScript = new TCHAR*[m_nTotalScriptLines];
// 
// 	// Read each line in.
// 	for ( int i=0; i<m_nTotalScriptLines; i++ )
// 	{
// 		m_pStrScript[i] = new TCHAR[MAX_LINE_SIZE+1];
// 		input2.getline( m_pStrScript[i], MAX_LINE_SIZE, '\n' );
// 	}
// 
// 	input2.close();
// 	return true;
// }
// 
// //--- 0000011 --- 2013/10/12 --- WCB --- Add
// // Remarks:
// //		Parse this line command and set the strCommand to first string.
// void CCMDParser::ParseCommand( LPTSTR strCommand )
// {
// 	int nCommandSize = 0;
// // Init string
//    strCommand[0] = '#';
//    strCommand[1] = '\0';
// 	if ( NULL == strCommand || m_nCurrentLine >= m_nTotalScriptLines || m_nCurrentLineChar >= (int)_tcslen( m_pStrScript[m_nCurrentLine] ) )
// 	{
// 		return;
// 	}
// 
// 	
// 
// 	if ( true == IsLineComment() )
// 	{
// 		strCommand[0] = '#';
// 		strCommand[1] = '\0';
// 		return;
// 	}
// 
// 	while ( m_nCurrentLineChar < (int)_tcslen( m_pStrScript[m_nCurrentLine] ) )
// 	{
// 		if ( ' ' == m_pStrScript[m_nCurrentLine][m_nCurrentLineChar] ||
// 			'\n' == m_pStrScript[m_nCurrentLine][m_nCurrentLineChar] ||
// 			'\t' == m_pStrScript[m_nCurrentLine][m_nCurrentLineChar] )
// 		{
// 			break;
// 		}
// 
// 		strCommand[nCommandSize] = m_pStrScript[m_nCurrentLine][m_nCurrentLineChar];
// 		nCommandSize++;
// 		m_nCurrentLineChar++;
// 	}
// 
// 	m_nCurrentLineChar++;
// 	strCommand[nCommandSize] = '\0';
// }
// 
// //--- 0000011 --- 2013/10/12 --- WCB --- Add
// // Remarks:
// //		Parse this command line with string mode
// void CCMDParser::ParseStringParam( LPTSTR strString )
// {
// 	int nParamSize = 0;
// 	bool bEndQuoteFound = false;
// 
// 	if ( NULL == strString || m_nCurrentLine >= m_nTotalScriptLines || m_nCurrentLineChar >= (int)_tcslen( m_pStrScript[m_nCurrentLine] ))
// 	{
// 		return;
// 	}
// 
// 	strString[0] = '#';
// 	strString[1] = '\0';
//  
// 	m_nCurrentLineChar++;
// 
// 	while ( m_nCurrentLineChar < (int)_tcslen( m_pStrScript[m_nCurrentLine]))
// 	{
// 		if ( '"' == m_pStrScript[m_nCurrentLine][m_nCurrentLineChar] )
// 		{
// 			bEndQuoteFound = true;
// 			break;
// 		}
// 
// 		if ( '\n' == m_pStrScript[m_nCurrentLine][m_nCurrentLineChar] || ' ' == m_pStrScript[m_nCurrentLine][m_nCurrentLineChar] )
// 		{
// 			break;
// 		}
// 
// 		strString[nParamSize] = m_pStrScript[m_nCurrentLine][m_nCurrentLineChar];
// 		nParamSize++;
// 		m_nCurrentLineChar++;
// 	}
// 
// 	if ( true == bEndQuoteFound )
// 	{
// 		m_nCurrentLineChar += 2;
// 	}
// 	else
// 	{
// 		m_nCurrentLineChar ++;
// 	}
// 
// 	strString[nParamSize] = '\0';
// }
// 
// //--- 0000011 --- 2013/10/12 --- WCB --- Add
// // Remarks:
// //		Parse this line with int model, it is call the ParseStringParam to parse this line, then , it will translate 
// //		it to int value.
// int CCMDParser::ParseIntParam()
// {
// 	// This funciton will take the next text and convert
// 	// it to a int that is returned.
// 
// 	TCHAR string[MAX_PARAM_SIZE];
// 	int paramSize = 0;
// 
// 	// If we run out of lines, or at the end
// 	// of the current line then we return.
// 	if(m_nCurrentLine >= m_nTotalScriptLines) return 0;
// 	if(m_nCurrentLineChar >= (int)_tcslen(m_pStrScript[m_nCurrentLine])) return 0;
// 
// 	// Loop through every character until you find a space or newline.
// 	// That means we are at the end of a variable.
// 	while(m_nCurrentLineChar < (int)_tcslen(m_pStrScript[m_nCurrentLine]))
// 	{
// 		if(m_pStrScript[m_nCurrentLine][m_nCurrentLineChar] == ' ' ||
// 			m_pStrScript[m_nCurrentLine][m_nCurrentLineChar] == '\n')
// 			break;
// 
// 		// Save the text in the array.
// 		string[paramSize] = m_pStrScript[m_nCurrentLine][m_nCurrentLineChar];
// 		paramSize++;
// 		m_nCurrentLineChar++;
// 	}
// 
// 	// Skip next space or newline.
// 	m_nCurrentLineChar++;
// 	string[paramSize] = '\0';
// 	return _ttoi(string);
// }
// 
// //--- 0000011 --- 2013/10/12 --- WCB --- Add
// // Remarks:
// //		Parse this line with bool model, it is call the ParseStringParam to parse this line, then , it will translate 
// //		it to bool value.
// bool CCMDParser::ParseBoolParam()
// {
// 	// This function will get the next text and will return
// 	// true if this text is "true" or false if it is "false".
// 
// 	TCHAR string[MAX_PARAM_SIZE];
// 	int paramSize = 0;
// 
// 	// If we run out of lines, or at the end
// 	// of the current line then we return.
// 	if(m_nCurrentLine >= m_nTotalScriptLines) return 0;
// 	if(m_nCurrentLineChar >= (int)_tcslen(m_pStrScript[m_nCurrentLine])) return 0;
// 
// 	// Loop through every character until you find a space or newline.
// 	// That means we are at the end of a variable.
// 	while(m_nCurrentLineChar < (int)_tcslen(m_pStrScript[m_nCurrentLine]))
// 	{
// 		if(m_pStrScript[m_nCurrentLine][m_nCurrentLineChar] == ' ' ||
// 			m_pStrScript[m_nCurrentLine][m_nCurrentLineChar] == '\n')
// 			break;
// 
// 		// Save the text in the array.
// 		string[paramSize] = m_pStrScript[m_nCurrentLine][m_nCurrentLineChar];
// 		paramSize++;
// 		m_nCurrentLineChar++;
// 	}
// 
// 	// Skip next space or newline.
// 	m_nCurrentLineChar++;
// 	string[paramSize] = '\0';
// 
// 	if(_tcsicmp(string, TEXT("true")) == 0) return true;
// 	return false;
// }
// 
// //--- 0000011 --- 2013/10/12 --- WCB --- Add
// // Remarks:
// //		Parse this line with float model, it is call the ParseStringParam to parse this line, then , it will translate 
// //		it to float value.
// float CCMDParser::ParseFloatParam()
// {
// 	// This funciton will take the next text and convert
// 	// it to a float that is returned.
// 
// 	TCHAR string[MAX_PARAM_SIZE];
// 	int paramSize = 0;
// 
// 	// If we run out of lines, or at the end
// 	// of the current line then we return.
// 	if( m_nCurrentLine >= m_nTotalScriptLines ) return 0;
// 	if(m_nCurrentLineChar >= (int)_tcslen(m_pStrScript[m_nCurrentLine])) return 0;
// 
// 	// Loop through every character until you find a space or newline.
// 	// That means we are at the end of a variable.
// 	while(m_nCurrentLineChar < (int)_tcslen(m_pStrScript[m_nCurrentLine]))
// 	{
// 		if(m_pStrScript[m_nCurrentLine][m_nCurrentLineChar] == ' ' ||
// 			m_pStrScript[m_nCurrentLine][m_nCurrentLineChar] == '\n')
// 			break;
// 
// 		// Save the text in the array.
// 		string[paramSize] = m_pStrScript[m_nCurrentLine][m_nCurrentLineChar];
// 		paramSize++;
// 		m_nCurrentLineChar++;
// 	}
// 
// 	// Skip next space or newline.
// 	m_nCurrentLineChar++;
// 	string[paramSize] = '\0';
// 	return (float)_ttof(string);
// }
// 
// //--- 0000011 --- 2013/10/12 --- WCB --- Add
// // Remarks:
// //		Get current line 
// int CCMDParser::GetCurrentLineNum()
// {
// 	return m_nCurrentLine;
// }
// 
// //--- 0000011 --- 2013/10/12 --- WCB --- Add
// // Remarks:
// //		Get total line numbers.
// int CCMDParser::GetTotalLines()
// {
// 	return m_nTotalScriptLines;
// }
// 
// //--- 0000011 --- 2013/10/12 --- WCB --- Add
// // Remarks:
// //		Check this line is not comment line.
// bool CCMDParser::IsLineComment()
// {
// 	if ( '#' == m_pStrScript[m_nCurrentLine][0] )
// 	{
// 		return true;
// 	}
// 	else
// 	{
// 		return false;
// 	}
// }
// 
// //--- 0000011 --- 2013/10/12 --- WCB --- Add
// // Remarks:
// //		Move current line to next line.
// void CCMDParser::MoveToNextLine()
// {
// 	m_nCurrentLine++;
// 	m_nCurrentLineChar = 0;
// }
// 
// //--- 0000011 --- 2013/10/12 --- WCB --- Add
// // Remarks:
// //		Move current line to start.
// void CCMDParser::MoveToStart()
// {
// 	m_nCurrentLine = 0;
// 	m_nCurrentLineChar = 0;
// }
// 
// //--- 0000011 --- 2013/10/12 --- WCB --- Add
// // Remarks:
// //		Clear all resource.
// void CCMDParser::Shutdown()
// {
// 	if ( NULL != m_pStrScript )
// 	{
// 		for ( int i=0; i<m_nTotalScriptLines; i++ )
// 		{
// 			if ( NULL != m_pStrScript[i] )
// 			{
// 				delete[] m_pStrScript[i];
// 				m_pStrScript[i] = NULL;
// 			}
// 		}
// 		delete m_pStrScript;
// 		m_pStrScript = NULL;
// 	}
// 
// 	m_nTotalScriptLines = 0;
// 	m_nCurrentLine = 0;
// 	m_nTotalScriptLines = 0;
// 
// 	delete this;
// }
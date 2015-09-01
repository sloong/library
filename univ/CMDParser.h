#ifndef CMDPARSER_H
#define CMDPARSER_H

class CCMDParser
{
public:
	CCMDParser();
	~CCMDParser();

	//--- Initialize Function annotation ---
	// Parameters:
	//		strFilePath:
	//			the command file path name.
	// Return value:
	//		If function succeeds, return S_OK. else return no zero value.
	// Remarks:
	//		Initialize CMD parser, Load the command file, and read the command to m_pStrScript.
	HRESULT Initialize(LPCTSTR strFilePath);

	//--- ParseCommand Function annotation ---
	// Parameters:
	//		strCommand:
	//			A char array buffer, it is the command line first string.
	//			Such as a command "AddText 10 20 'test text'", it will is "AddText"
	// Remarks:
	//		Parse the command type, it look like the command name.
	void	ParseCommand(LPTSTR strCommand);

	//--- ParseStringParam Function annotation ---
	// Parameters:
	//		strString:
	//			A char array buffer, it will the command line second string.
	//			Such as a command "PopupMsg 'test message'", it will is "test message"
	// Remarks:
	//		Parse the command string, it look like the attribute.
	void	ParseStringParam(LPTSTR strString);

	//--- ParseBoolParam Function annotation ---
	// Return value:
	//		This line bool type attribute
	// Remarks:
	//		Parse the command value, it is a bool type value.
	bool	ParseBoolParam();

	//--- ParseIntParam Function annotation ---
	// Return value:
	//		This line int type attribute value.
	// Remarks:
	//		Parse the command value, it is a int type value.
	int		ParseIntParam();

	//--- ParseFloatParam Function annotation ---
	// Return value:
	//		This line float type attribute value.
	// Remarks:
	//		Parse the command value, it is a float type value.
	float	ParseFloatParam();

	//--- Shutdown Function annotation ---
	// Remarks:
	//		Clear the resource memory.
	void	Shutdown();

	//--- MoveToStart Function annotation ---
	// Remarks:
	//		Move current line to file start point.
	void	MoveToStart();

	//--- MoveToNextLine Function annotation ---
	// Remarks:
	//		Move current line to next line.
	void	MoveToNextLine();

	//--- GetCurrentLineNum Function annotation ---
	// Return value:
	//		Now line number.
	// Remarks:
	//		Get now current line number.
	int		GetCurrentLineNum();

	//--- GetTotalLine Function annotation ---
	// Return value:
	//		This file total line number.
	// Remarks:
	//		Get this file total line number.
	int GetTotalLines();
	//--- IsLineComment Function annotation ---
	// Return value:
	//		If this line is comment line, return true.
	//		else return false.
	// Remarks:
	//		Check this line is not a comment line.
	bool	IsLineComment();

private:
	int		m_nTotalScriptLines;
	int		m_nCurrentLine;
	int		m_nCurrentLineChar;
	LPTSTR*	m_pStrScript;
};

#endif // !CMDPARSER_H


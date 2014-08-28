/************************************************************************/
/* 				SLUniversal.h --- SLUniversal Definition                */
/************************************************************************/
#pragma once
#ifdef SLUNIVERSAL_EXPORTS
#define SLUNIVERSAL_API __declspec(dllexport)
// Version
#define VERSION_NUMBER						1,0,0,1
#define VERSION_FILEVERSION					"1.0.0.1"
#define VERSION_BUILDTIME					"2014/4/25"
#ifdef _DEBUG
#define VERSION_PRODUCTVERSION 				"Ver.1.0 for Debug"
#define	VERSION_FILEDESCRIPTION				"SLoong Universal Debug Libaray"
#define	VERSION_PRODUCTNAME					"SLoong Universal Debug Libaray"
#else
#define VERSION_PRODUCTVERSION 				"Ver.1.0"
#define	VERSION_FILEDESCRIPTION				"SLoong Universal Libaray"
#define	VERSION_PRODUCTNAME					"SLoong Universal Libaray"
#endif // _DEBUG
#define VERSION_INTERNALNAME				"SLUniversal.dll"
#define VERSION_COMPANYNAME 				"SoaringLoong, Inc."
#define	VERSION_LEGALCOPYRIGHT				"Copyright (C) 2013 SoaringLoong, Inc."
#else
#define SLUNIVERSAL_API __declspec(dllimport)
#endif

#include <tchar.h>
#include <fstream>
#include <stdarg.h>

#pragma region SLUniversal Defines


#ifndef _ARGB
#define _ARGB(a,r,g,b) ((ULONG)((((a)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff)))
#endif	// _ARGB


#ifndef SAFE_DELETE
#define SAFE_DELETE(p)		{if(NULL != (p)){delete (p);(p)=NULL;}}
#endif	// SAFE_DELETE

#ifndef SAFE_DELETE_ARR
#define SAFE_DELETE_ARR(p)	{if(NULL != (p)){delete[] (p);(p)=NULL;}}
#endif	// SAFE_DELETE_ARR

#ifndef SAFE_SHUTDOWN
#define SAFE_SHUTDOWN(p)	{if(NULL != (p)){(p)->Shutdown();(p)=NULL;}}
#endif	// SAFE_SHUTDOWN

#ifndef SAFE_DESTROY
#define SAFE_DESTROY(p)		{if(NULL != (p)){(p)->Destroy();(p)=NULL;}}
#endif	// SAFE_DESTROY

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)		{if(NULL != (p)){(p)->Release();p = NULL;}}
#endif // !SAFE_RELEASE

#ifndef SAFE_DEL_ALL_DATA_FROM_LIST
#define SAFE_DEL_ALL_DATA_FROM_LIST(p,type,del) {if(NULL!=p){for(int i=0;i<p->m_nNum;i++){CLinkList* pNode=p->Locate(i);type pData=(type)pNode->GetData();del(pData);pNode->SetData(NULL,false);}}}
#endif // !SAFE_DEL_ALL_DATA_FROM_LIST

#ifdef _UNICODE
#include <string>
#include <fstream>
using std::wifstream;
using std::wstring;
using std::wofstream;
#define _tofstream		wofstream
#define _tifstream		wifstream
#define _tmemcpy		wmemcpy
#define _tmemcpy_s		wmemcpy_s
#define _tcscpy			wcscpy
#define _ftprintf		fwprintf
#define _tfopen			_wfopen
#define _tfopen_s		_wfopen_s
#define _stscanf		swscanf
#define _stscanf_s		swscanf_s
#define _sntprintf_s	_snwprintf_s
#define _sntprintf		_snwprintf
#define _tmemmove		wmemmove
#define _ttof			_wtof
#define _tcschr			wcschr
#define _fgetts			fgetws
#define _fputts			fputws
#define _tprintf		wprintf
#define _tsprintf		wsprintf
#define _tvsprintf		wvsprintf
#define _tvsprintf_s	vswprintf_s
#define _tcscat_s		wcscat_s
#define _ttoi			_wtoi
#define _tstring		wstring
#define UNSIGNED
#define __STR2WSTR(str) L##str
#define _STR2WSTR(str) __STR2WSTR(str)
#define __FUNCTIONW__ _STR2WSTR(__FUNCTION__)
#define __TFILE__		__FILEW__
#define __TFUNCTION__	__FUNCTIONW__

#else
#include <string>
#include <fstream>
using std::string;
using std::fstream;
using std::ofstream;
using std::ifstream;
#define _tofstream		ofstream
#define _tifstream		ifstream
#define _tmemcpy		memcpy
#define _tmemcpy_s		memcpy_s
#define _tcscpy			strcpy
#define _ftprintf		fprintf
#define _tfopen			fopen
#define _tfopen_s		fopen_s
#define _stscanf		sscanf
#define _stscanf_s		sscanf_s
#define _sntprintf		_snprintf
#define _sntprintf_s	_snprintf_s
#define _tmemmove		memmove
#define _ttof			atof
#define _tcschr			strchr
#define _fgetts			fgets
#define _fputts			fputs
#define _tprintf		printf
#define _tsprintf		wsprintf
#define _tvsprintf		vsprintf
#define _tvsprintf_s	vsprintf_s
#define _tcscat_s		strcat_s 
#define _ttoi			atoi
#ifndef	UNSIGNED
#define UNSIGNED		unsigned
#endif // !UNSIGNED
#define _tstring		string
#define __TFILE__		__FILE__
#define __TFUNCTION__	__FUNCTION__
#endif //_UNICODE
#pragma endregion

class TiXmlDocument;
class TiXmlElement;
class TiXmlAttribute;

namespace SLUniversal
{
	#define MAX_STRING							1024
	#define NUM_SWAP							5
	#define MAX_LINE_SIZE						3072
	#define MAX_COMMAND_SIZE					32
	#define MAX_PARAM_SIZE						2048
	#define CHAR_SWAP_SIZE						2048

	typedef enum _emLogLevel
	{
		FATAL,
		ERR,
		WARN,
		INF,
		All,
	}LOGLEVEL;

	enum PARSETYPE
	{
		PARSE_CMD,
		PARSE_XML,
		PARSE_TOKEN,
	};

	//--- Format Function annotation ---
	// Parameters:
	//		strString:
	//			The string with how to format.
	// Return:
	//		If function succeeded, return string, else return NULL.
	// Remarks:
	//		In default case, it have 5 group buffer to format string, once is empty always,
	//		so you can call this function 4 numbers in one time in one function .
	SLUNIVERSAL_API LPCTSTR Format( LPCTSTR strString, ... );

	SLUNIVERSAL_API void CopyStringToPoint( LPTSTR& lpTarget, LPCTSTR lpFrom );

	class SLUNIVERSAL_API CLinkList
	{
	public:
		CLinkList(void);

		//--- ~CLinkList Function annotation ---
		// Remarks:
		//		User cannot call it. and when list delete, this function don't call the destroy function. 
		~CLinkList(void);

		//--- Locate Function annotation ---
		// Parameters:
		//		nIndex:
		//			A unsigned int value, it is the node index in this list.
		//		szMarkName:
		//			This node mark name, if have more than one, it always return the first node.
		//			So, don't use same mark to Insert the node. 
		// Return value:
		//		If function succeeds, return the node pointer, else return NULL.
		// Remarks:
		// 		Find node with the mark for index or name. this function have two, one is find with
		//		Index number, other one is find with mark name.
		CLinkList* Locate( UINT nIndex );
		CLinkList* Locate( LPCTSTR szMarkName );

		//--- GetLast Function annotation ---
		// Return value:
		//		A Pointer, point to the Last node.
		// Remarks:
		//		Get last node.
		CLinkList* GetLast();

		//--- Insert function annotation ---
		// Parameters:
		//		pData:		
		//			a void type pointer, it must used the new operator create in the Heap.
		//		szMarkName: 
		//			This data mark in this list, if have other node have same mark,
		//			we not sure when find time, it return which one, only when used the index number.
		//			but, we still don't recommend used the same mark in one list.
		//			This param default value is "UsdIndex". if user use index, user can use this value.
		//		szDataType: 
		//			this data type mark in this list, it have the default value, so it just 
		//			used help you this data isn't you want data.
		// Return Value:
		//		This node's index in list.
		// Remarks: 
		//		Insert the data to this node back, the param is the marks of the data.
		//		and the data must is a pointer with new memory.
		int Insert( LPVOID pData, LPCTSTR szMarkName = TEXT("UseIndex"), LPCTSTR szDataType = TEXT("LPVOID") );

		//--- AddToList Function annotation ---
		// Parameters:
		//		pData:
		//			A void type pointer, it must used the new operator create in the Heap.
		//		szMarkName:
		//			This data mark in this list, if have other node have same mark,
		//			we not sure when find time, it return which one, only when used the index number.
		//			but, we still don't recommend used the same mark in one list.
		//			This param default value is "UsdIndex". if user use index, user can use this value.
		//		szDataType:
		//			This data type mark, default is "void*".
		//	Return value:
		//		This node's index in list.
		//	Remarks:
		//		Insert node at the list last.
		int AddToList( LPVOID pData, LPCTSTR szMarkName = TEXT("UseIndex"), LPCTSTR szDataType = TEXT("LPVOID") );

		//--- GetData Function annotation ---		
		// Return value:
		//		This node data, this is A void pointer, so you should translate it to your type, if you 
		//		don't know, see the member variable: m_szDataType.
		// Remarks:
		//		Get the data pointer, it return the void pointer, so you should change the type, if you don't know 
		//		it type, see the member variable: m_szDataType 
		LPVOID GetData();

		//--- SetData Function annotation ---
		// Parameters:
		//		pData:
		//			A void type pointer, it point to the new data as you want set.
		//		ulSize:
		//			Size of pData, LinkList will copy this data to list.
		//		bDelPrevData;
		//			If user was delete the data, the data pointer is invalid point. so 
		//			in this case, no delete the data.
		//	Remarks:
		//		Set node data.
		void SetData( LPVOID pData, bool bDelPrevData = true );

		//--- Delete Function annotation ---
		// Parameters:
		//		nIndex:
		//			Target node index number.
		// Remarks:
		//		Delete this node or other node in list, if no have param 1, it is delete this self.
		void Delete();
		void Delete( int nIndex );

		//--- Destroy Function annotation ---
		// Remarks:
		//		Delete this list.
		void Destroy();	


	private:
		// Set list last node, if the pLast is not a true node, maybe the list will chaos, so this 
		// function just can used in the self.
		void SetListLast(CLinkList* pLast);
		// Set the link list all node Numbers
		void SetListNum( UINT nNum );
		// Refresh the link list index, form head to last, refresh the index, 
		void RefreshIndex();


	public:
		// Member variable.
		// Pointer to this list head node.
		CLinkList* m_pListHead;
		// Pointer to this list last node.
		CLinkList* m_pListLast;	
		// Pointer to this node previous node.
		CLinkList* m_pPrevious;
		// Pointer to this node next node.
		CLinkList* m_pNext;
		// Name mark
		LPCTSTR m_szMarkName;
		// Pointer to the node data with the void,
		// if you want used is, please change the type
		void* m_pData;
		// the list all numbers.
		long m_nNum;
		// The index of this list.
		UINT m_nIndex;
		// The data type mark
		LPCTSTR m_szDataType;
	};

	class SLUNIVERSAL_API CSLoongLogSystem 
	{
	public:
		CSLoongLogSystem(void);
		~CSLoongLogSystem();

		//--- Create Function annotation ---
		// Parameters:
		//		pLog:
		//			The point and Pointer to NULL.
		// Remarks:
		//		Create the LogSystem object.
		static void Create( CSLoongLogSystem*& pLog );

		//--- Initialize Function annotation ---
		// Parameters:
		//		szFilePath:
		//			The path of save log file.
		//		emLevel:
		//			Log level.
		//		bFormatWindowsMsg:
		//			If true, when error happed, if will all window message.
		//		bWriteNoFailedInfo:
		//			If true, operation succeeded also write to log.
		// Remarks:
		//		Initialize the log system.
		LRESULT Initialize( LPCTSTR szFilePath, LOGLEVEL emLevel = LOGLEVEL::All, bool bFormatWindowsMsg = true, bool bWriteNoFailedInfo = false );

		//--- Shutdown Function annotation ---
		// Remarks:
		//		Shutdown log system.
		void Shutdown();

		//--- Clear Function annotation ---
		// Remarks:
		//		Clear log file.
		LRESULT Clear();

		//--- Write Function annotation ---
		// Parameters:
		//		szMessage
		//			The string for you add to log file.
		// Remarks:
		//		Write szMessage to log file.
		LRESULT Write( LPCTSTR szMessage );

		//--- Append Function annotation ---
		// Parameters:
		//		szMessage
		//			The string for you add to log file. it will add "time" begin string and "\n" in string last.
		// Remarks:
		//		Append szMessage to log file.
		LRESULT Append( LPCTSTR szMessage );

		//--- CheckResult Function annotation ---
		// Parameters:
		//		lpstrErrText
		//			The Error text, if g_lResult is not S_OK, the error text will append to log file.
		// Remarks:
		//		Check result function, if no error, function return ,
		//		if error, the error text will append to log file.
		void CheckResult( LOGLEVEL emLevel, DWORD dwCode, LPCTSTR strErrorText );

		//--- FormatWindowsErrorMessage Function annotation ---
		// Parameters:
		//		szErrText:
		//			A string buffer.
		//		dwSize:
		//			The size of buffer.
		//		dwErrCode:
		//			The Error code of windows.
		// Remarks:
		//		Format the Windows error string.
		LRESULT FormatWindowsErrorMessage( LPTSTR szErrText, DWORD dwSize, DWORD dwErrCode );

		//--- ProcessFatal Function annotation ---
		// Parameters:
		//		dwCode:
		//			The Fatal code. it will format to [Fatal %code%].
		//		strErrorText
		//			The Fatal text, if g_lResult is not S_OK, the text will append to log file.
		// Remarks:
		//		Format the Fatal string.
		LPCTSTR ProcessFatal( DWORD dwCode, LPCTSTR strErrorText );

		//--- ProcessError Function annotation ---
		// Parameters:
		//		dwCode:
		//			The Error code. it will format to [Error %code%].
		//		strErrorText
		//			The Error text, if g_lResult is not S_OK, the text will append to log file.
		// Remarks:
		//		Format the Fatal string.
		LPCTSTR ProcessError( DWORD dwCode, LPCTSTR strErrorText );

		//--- ProcessWarning Function annotation ---
		// Parameters:
		//		dwCode:
		//			The Warning code. it will format to [Warning %code%].
		//		strErrorText
		//			The Warning text, if g_lResult is not S_OK, the text will append to log file.
		// Remarks:
		//		Format the Fatal string.
		LPCTSTR ProcessWarning( DWORD dwCode, LPCTSTR strErrorText );

		//--- ProcessInformation Function annotation ---
		// Parameters:
		//		dwCode:
		//			The Information code. it will format to [Information %code%].
		//		strErrorText
		//			The Information text, the text will append to log file.
		// Remarks:
		//		Format the Information string.
		LPCTSTR ProcessInformation( DWORD dwCode, LPCTSTR strErrorText );

	private:
		LOGLEVEL m_emLevel;
		LPCTSTR m_szLogFilePath;
		bool	m_bFormatWindowsMsg;
		bool	m_bWriteNoFailedInfo;
		_tofstream* m_pLogFile;
	};

	class SLUNIVERSAL_API CSLException
	{
	public:
		CSLException();
		~CSLException();

		void SetExceptionText( LPCTSTR strMessage );
		LPCTSTR GetExceptionText();

		void SetExceptionCode( LRESULT lCode );
		LRESULT GetExceptionCode();

	private:
		LPTSTR m_strMessage;
		LRESULT m_lResult;
	};

	class SLUNIVERSAL_API CXMLParser
	{
	public:
		TiXmlDocument*	m_pDoc;
		TiXmlElement*	m_pRootNode;

	public:
		CXMLParser(void);
		~CXMLParser(void);

		LPCTSTR GetAttribute( LPCTSTR szNodeName, LPCTSTR szAttributeName, LRESULT& lResult, LPCTSTR szParentNode = NULL, bool bFindParent = false );
		LPCTSTR GetNodeText( LPCTSTR szNodeName , LRESULT& lResult, LPCTSTR szParentNode = NULL );
		int GetAttributeInt( LPCTSTR szNodeName, LPCTSTR szAttributeName ,LRESULT& lResult, LPCTSTR szParientNode = NULL, bool bFindParent = false );
		ULONG GetAttributeARGB( LPCTSTR szNodeName ,LRESULT& lResult, LPCTSTR szParientNode = NULL, bool bFindParent = false );
		LRESULT FindAllChildName( LPCTSTR szParentNodeName, SLUniversal::CLinkList* pChildList, LPCTSTR szRootNodeName = NULL );
		LRESULT FindAllChildText( LPCTSTR szParentNodeName, SLUniversal::CLinkList* pChildList, LPCTSTR szRootNodeName = NULL );
		LRESULT Initialize( LPCTSTR strPathe);
		LRESULT GetNodeByName(TiXmlElement* pRoot, LPCTSTR strNodeName, TiXmlElement*& pNode);
		LRESULT FindAllChildName( TiXmlElement* pNode, SLUniversal::CLinkList* pList );
		LRESULT FindAllChildText( TiXmlElement* pNode, SLUniversal::CLinkList* pList );
		void Shutdown();
		LPCTSTR GetAttribute( TiXmlAttribute* pNodeAttribute, LPCTSTR strAttributeName ,LRESULT& lResult);
		LPCTSTR GetAttribute( TiXmlElement* pNode, LPCTSTR strAttributeName, LRESULT& lResult );
		LPCTSTR GetNodeText( TiXmlElement* pNode, LRESULT& lResult );
		LRESULT GetParentNode( TiXmlElement* pChildNode, TiXmlElement*& pParentNode );
	};

	class SLUNIVERSAL_API CTokenParser
	{
	public:
		CTokenParser(void);
		~CTokenParser(void);
		LRESULT Initialize( LPCTSTR strFilePath );
		void Shutdown();
		//--- Reset Function annotation ---
		// Parameters:
		//		
		// Return value:
		//		
		// Remarks:
		//		It just used in TokenStream Mode.
		void Reset();

		//--- SetTokenStream Function annotation ---
		// Parameters:
		//		
		// Return value:
		//		
		// Remarks:
		//		It just used in TokenStream mode
		LRESULT SetTokenStream( LPTSTR strData );

		//--- GetNextToken Function annotation ---
		// Parameters:
		//		
		// Return value:
		//		
		// Remarks:
		//		It just used in TokenStream Mode.
		LRESULT GetNextToken( LPTSTR buffer, UINT BufSize );
		LRESULT GetNextToken( LPTSTR token, LPTSTR buffer, UINT nBufSize );

		//--- IsValidIdentifier Function annotation ---
		// Parameters:
		//		
		// Return value:
		//		
		// Remarks:
		//		Just used in TokenStream Mode.
		bool IsValidIdentifier( TCHAR c );

		//--- MoveToNextLine Function annotation ---
		// Parameters:
		//		
		// Return value:
		//		
		// Remarks:
		//		Just used in TokenStream
		bool MoveToNextLine( LPTSTR buffer, int BufSize );
		//--- 0000014 --- 2013/10/24 --- WCB --- Add
		// Just used in TokenStream Mode.
		//--- 0000017 --- 2013/10/25 --- WCB --- Modify

		int GetTotalLine();
	protected:
		UINT m_unTotalTokens;
		UINT m_unStartIndex, m_unEndIndex;
		UINT m_unLength;

		LPTSTR m_strData;
	};

	class SLUNIVERSAL_API CCMDParser
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
		LRESULT Initialize( LPCTSTR strFilePath );

		//--- ParseCommand Function annotation ---
		// Parameters:
		//		strCommand:
		//			A char array buffer, it is the command line first string.
		//			Such as a command "AddText 10 20 'test text'", it will is "AddText"
		// Remarks:
		//		Parse the command type, it look like the command name.
		void	ParseCommand( LPTSTR strCommand );

		//--- ParseStringParam Function annotation ---
		// Parameters:
		//		strString:
		//			A char array buffer, it will the command line second string.
		//			Such as a command "PopupMsg 'test message'", it will is "test message"
		// Remarks:
		//		Parse the command string, it look like the attribute.
		void	ParseStringParam( LPTSTR strString );

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

	LRESULT SLUNIVERSAL_API CreateScriptParsing( LPVOID& pParsing, PARSETYPE emType );

	SLUNIVERSAL_API extern LRESULT g_lResult;
}

#pragma once

#ifdef UNIVERSAL_EXPORTS
#define UNIVERSAL_API __declspec(dllexport)
// Version
#define VERSION_NUMBER						2,0,0,22
#define VERSION_FILEVERSION					"2.0.0.22"
#define VERSION_BUILDTIME					"2014/10/10"
#ifdef _DEBUG
#define VERSION_PRODUCTVERSION 				"Ver.2.0 for Debug"
#define	VERSION_FILEDESCRIPTION				"SLoong Universal Debug Libaray"
#define	VERSION_PRODUCTNAME					"SLoong Universal Debug Libaray"
#else
#define VERSION_PRODUCTVERSION 				"Ver.2.0"
#define	VERSION_FILEDESCRIPTION				"SLoong Universal Libaray"
#define	VERSION_PRODUCTNAME					"SLoong Universal Libaray"
#endif // _DEBUG
#define VERSION_INTERNALNAME				"SLUniversal.dll"
#define VERSION_COMPANYNAME 				"SoaringLoong, Inc."
#define	VERSION_LEGALCOPYRIGHT				"Copyright (C) 2014 SoaringLoong, Inc."
#else
#define UNIVERSAL_API __declspec(dllimport)
#endif

#include <unknwn.h>
#include <windows.h>
#include <tchar.h>
#include <assert.h>
#include <time.h>
#include <stdio.h>
#include <fstream>
#include <stdarg.h>
#include <comdef.h>

// {3D8C5798-69C0-B18C-7EE8-E6C99BEBC8C4}
static const GUID IID_IUniversal =
{ 0x3d8c5798, 0x69c0, 0xb18c, { 0x7e, 0xe8, 0xe6, 0xc9, 0x9b, 0xeb, 0xc8, 0xc4 } };

const LPCTSTR g_GUID = TEXT("{3D8C5798-69C0-B18C-7EE8-E6C99BEBC8C4}");
const LPCTSTR g_CLSID = TEXT("COMCTL.SLoongUniversal");

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
#define MAX_STRING							1024
#define NUM_SWAP							5
#define MAX_LINE_SIZE						3072
#define MAX_COMMAND_SIZE					32
#define MAX_PARAM_SIZE						2048
#define CHAR_SWAP_SIZE						2048


namespace SoaringLoong
{
	class CException;
	class CLinkList;
	class CLogSystem;
	class CTokenParser;
	class CXMLParser;
	class CSize;
	class CPoint;
	class CRect;


	UNIVERSAL_API void CopyStringToPoint(LPTSTR& lpTarget, LPCTSTR lpFrom);

	UNIVERSAL_API LPCTSTR Format(LPCTSTR strString, ...);


	class IUniversal :
		public IUnknown
	{
	public:
		virtual LPCTSTR _stdcall HelloWorld() = 0;
	};

	class CLinkList
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
		CLinkList* Locate(UINT nIndex);
		CLinkList* Locate(LPCTSTR szMarkName);

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
		int Insert(LPVOID pData, LPCTSTR szMarkName = TEXT("UseIndex"), LPCTSTR szDataType = TEXT("LPVOID"));

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
		int AddToList(LPVOID pData, LPCTSTR szMarkName = TEXT("UseIndex"), LPCTSTR szDataType = TEXT("LPVOID"));

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
		void SetData(LPVOID pData, bool bDelPrevData = true);

		//--- Delete Function annotation ---
		// Parameters:
		//		nIndex:
		//			Target node index number.
		// Remarks:
		//		Delete this node or other node in list, if no have param 1, it is delete this self.
		void Delete();
		void Delete(int nIndex);

		//--- Destroy Function annotation ---
		// Remarks:
		//		Delete this list.
		void Destroy();


	private:
		// Set list last node, if the pLast is not a true node, maybe the list will chaos, so this 
		// function just can used in the self.
		void SetListLast(CLinkList* pLast);
		// Set the link list all node Numbers
		void SetListNum(UINT nNum);
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

	class CLogSystem
	{
	public:
		typedef enum _emLogType
		{
			YEAR = 0,
			MONTH = 1,
			DAY = 2,
			ONEFILE = 3
		}LOGTYPE;

		typedef enum _emLogLevel
		{
			FATAL,
			ERR,
			WARN,
			INF,
			All,
		}LOGLEVEL;

		CLogSystem(LPCTSTR szPathName = TEXT("Log.log"), LOGLEVEL emLevel = LOGLEVEL::All, LOGTYPE emType = ONEFILE, bool bIsCoverPrev = false);

		~CLogSystem();

		//--- Create Function annotation ---
		// Parameters:
		//		pLog:
		//			The point and Pointer to NULL.
		// Remarks:
		//		Create the LogSystem object.
		static void Create(CLogSystem*& pLog);

		//--- Write Function annotation ---
		// Parameters:
		//		szMessage
		//			The string for you add to log file.
		// Remarks:
		//		Write szMessage to log file.
		DWORD Write(LPCTSTR szMessage);

		//--- WriteLog Function annotation ---
		// Parameters:
		//		szMessage
		//			The string for you add to log file. it will add "time" begin string and "\n" in string last.
		// Remarks:
		//		WriteLog szMessage to log file.
		void Log(LPCTSTR szLog);

		//--- ResLog Function annotation ---
		// Parameters:
		//		lpstrErrText
		//			The Error text, if g_hRes is not S_OK, the error text will append to log file.
		// Remarks:
		//		Check result function, if no error, function return ,
		//		if error, the error text will append to log file.
		void ResLog(LOGLEVEL emLevel, DWORD dwCode, LPCTSTR strErrorText, bool bFormatWinMsg = true, bool bJustFailedWrite = true);

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
		HRESULT FormatWindowsErrorMessage(LPTSTR szErrText, DWORD dwSize, DWORD dwErrCode);

		//--- SetConfiguration Function annotation ---
		// Parameters:
		//		szFileName:
		//			The new file name, the log system will create it and used it to write string after call log function.
		//			if no need change it, set to NULL.
		//		szFilePath:
		//			The new file path, it just used without LOG_TYPE::ONEFILE mode, the log system will create the directory.
		//			if no need change it, set to NULL.
		//		pType:
		//			The new log type.
		//		pLevel:
		//			The new level value.
		// Remarks:
		//		Set the log system configuration.
		void SetConfiguration(LPCTSTR szFileName, LPCTSTR szFilePath, LOGTYPE* pType, LOGLEVEL* pLevel);

		//--- IsOpen Function annotation ---
		// Remarks:
		//		Check the current log file is not opened.
		bool IsOpen();

		//--- Close Function annotation ---
		// Remarks:
		//		Close current log file.
		void Close();

		//--- GetFileName Function annotation ---
		// Remarks:
		//		Get current log file name with path.
		LPCTSTR GetFileName();

		//--- GetPath Function annotation ---
		// Remarks:
		//		Get current log file path, it used without LOG_TYPE::ONEFILE mode.
		LPCTSTR GetPath();

	protected:
		LPCTSTR FormatFatalMessage(DWORD dwCode, LPCTSTR strErrorText, bool bFormatWinMsg, bool bJustFailedWrite);
		LPCTSTR FormatErrorMessage(DWORD dwCode, LPCTSTR strErrorText, bool bFormatWinMsg, bool bJustFailedWrite);
		LPCTSTR FormatWarningMessage(DWORD dwCode, LPCTSTR strErrorText, bool bFormatWinMsg, bool bJustFailedWrite);
		LPCTSTR FormatInformationMessage(DWORD dwCode, LPCTSTR strErrorText, bool bFormatWinMsg, bool bJustFailedWrite);
		HRESULT OpenFile();
		void	Lock();
		void	Unlock();

	protected:
		LOGLEVEL	m_emLevel;
		HANDLE		m_hFileHandle;
		LPTSTR		m_szFilePath;
		LPTSTR		m_szFileName;
		CRITICAL_SECTION m_csLock;
		LPTSTR		m_szLastDate;
		int			m_emType;
		bool		m_bIsCoverPrev;
	};

	class CException
	{
	public:
		CException();
		~CException();

		void SetExceptionText(LPCTSTR strMessage);
		LPCTSTR GetExceptionText();

		void SetExceptionCode(HRESULT lCode);
		HRESULT GetExceptionCode();

	private:
		LPTSTR m_strMessage;
		HRESULT m_lResult;
	};

	class CXMLParser
	{
	public:
		TiXmlDocument*	m_pDoc;
		TiXmlElement*	m_pRootNode;

	public:
		CXMLParser(void);
		~CXMLParser(void);

		LPCTSTR GetAttribute(LPCTSTR szNodeName, LPCTSTR szAttributeName, HRESULT& hRes, LPCTSTR szParentNode = NULL, bool bFindParent = false);
		LPCTSTR GetNodeText(LPCTSTR szNodeName, HRESULT& hRes, LPCTSTR szParentNode = NULL);
		int GetAttributeInt(LPCTSTR szNodeName, LPCTSTR szAttributeName, HRESULT& hRes, LPCTSTR szParientNode = NULL, bool bFindParent = false);
		ULONG GetAttributeARGB(LPCTSTR szNodeName, HRESULT& hRes, LPCTSTR szParientNode = NULL, bool bFindParent = false);
		HRESULT FindAllChildName(LPCTSTR szParentNodeName, CLinkList* pChildList, LPCTSTR szRootNodeName = NULL);
		HRESULT FindAllChildText(LPCTSTR szParentNodeName, CLinkList* pChildList, LPCTSTR szRootNodeName = NULL);
		HRESULT Initialize(LPCTSTR strPathe);
		HRESULT GetNodeByName(TiXmlElement* pRoot, LPCTSTR strNodeName, TiXmlElement*& pNode);
		HRESULT FindAllChildName(TiXmlElement* pNode, CLinkList* pList);
		HRESULT FindAllChildText(TiXmlElement* pNode, CLinkList* pList);
		void Shutdown();
		LPCTSTR GetAttribute(TiXmlAttribute* pNodeAttribute, LPCTSTR strAttributeName, HRESULT& hRes);
		LPCTSTR GetAttribute(TiXmlElement* pNode, LPCTSTR strAttributeName, HRESULT& hRes);
		LPCTSTR GetNodeText(TiXmlElement* pNode, HRESULT& hRes);
		HRESULT GetParentNode(TiXmlElement* pChildNode, TiXmlElement*& pParentNode);
	};

	class XMLProc
	{
	public:

		IXMLDOMDocumentPtr createFile;
		IXMLDOMDocumentPtr readFile;
		IXMLDOMElementPtr createRoot;
		IXMLDOMElementPtr readRoot;
		IXMLDOMProcessingInstructionPtr pPI;
		HRESULT hr;

	public:
		XMLProc();
		~XMLProc(){}
		IXMLDOMDocumentPtr Create(LPCTSTR rootTag);
		BOOL NewElement(LPCTSTR tag, LPCTSTR text, IXMLDOMElement** newNode);
		BOOL NewElement(LPCTSTR tag, IXMLDOMElement** newNode);
		BOOL AddChild(IXMLDOMElement* childElem, IXMLDOMElement* parentElem);
		BOOL SaveCreateFile(LPCTSTR dfile);
		IXMLDOMDocumentPtr Read(LPCTSTR sfile);
		BOOL SelectElement(LPCTSTR tag, IXMLDOMElement** node);
		BOOL GetText(IXMLDOMElement* elem, LPTSTR text, int nBufferSize, int* tlen);
		BOOL SaveReadFile(LPCTSTR dfile);
		BOOL ChangeText(IXMLDOMElement* elem, LPCTSTR text);
		BOOL GetParent(IXMLDOMElement* childElem, IXMLDOMElement** parentElem);
		BOOL GetChild(IXMLDOMElement** childElem, IXMLDOMElement* parentElem);
		BOOL RemoveChild(IXMLDOMElement* childElem, IXMLDOMElement* parentElem);
		BOOL SetAttribute(IXMLDOMElement* node, LPCTSTR attr, LPCTSTR value);
		IXMLDOMDocumentPtr GetCreateFile();
		IXMLDOMDocumentPtr GetReadFile();
		IXMLDOMElement* GetCreateRoot();
		IXMLDOMElement* GetReadRoot();

		void Fail();

		void Exit();

	};

	class CTokenParser
	{
	public:
		CTokenParser(void);
		~CTokenParser(void);
		HRESULT Initialize(LPCTSTR strFilePath);
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
		HRESULT SetTokenStream(LPTSTR strData);

		//--- GetNextToken Function annotation ---
		// Parameters:
		//		
		// Return value:
		//		
		// Remarks:
		//		It just used in TokenStream Mode.
		HRESULT GetNextToken(LPTSTR buffer, UINT BufSize);
		HRESULT GetNextToken(LPTSTR token, LPTSTR buffer, UINT nBufSize);

		//--- IsValidIdentifier Function annotation ---
		// Parameters:
		//		
		// Return value:
		//		
		// Remarks:
		//		Just used in TokenStream Mode.
		bool IsValidIdentifier(TCHAR c);

		//--- MoveToNextLine Function annotation ---
		// Parameters:
		//		
		// Return value:
		//		
		// Remarks:
		//		Just used in TokenStream
		bool MoveToNextLine(LPTSTR buffer, int BufSize);
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



	/////////////////////////////////////////////////////////////////////////////
	// CSize - An extent, similar to Windows SIZE structure.
	class UNIVERSAL_API CSize : public tagSIZE
	{
	public:

		// Constructors
		// construct an uninitialized size
		CSize() throw();
		// create from two integers
		CSize(
			_In_ int initCX,
			_In_ int initCY) throw();
		// create from another size
		CSize(_In_ SIZE initSize) throw();
		// create from a point
		CSize(_In_ POINT initPt) throw();
		// create from a DWORD: cx = LOWORD(dw) cy = HIWORD(dw)
		CSize(_In_ DWORD dwSize) throw();

		// Operations
		BOOL operator==(_In_ SIZE size) const throw();
		BOOL operator!=(_In_ SIZE size) const throw();
		void operator+=(_In_ SIZE size) throw();
		void operator-=(_In_ SIZE size) throw();
		void SetSize(_In_ int CX, _In_ int CY) throw();

		// Operators returning CSize values
		CSize operator+(_In_ SIZE size) const throw();
		CSize operator-(_In_ SIZE size) const throw();
		CSize operator-() const throw();

		// Operators returning CPoint values
		CPoint operator+(_In_ POINT point) const throw();
		CPoint operator-(_In_ POINT point) const throw();

		// Operators returning CRect values
		CRect operator+(_In_ const RECT* lpRect) const throw();
		CRect operator-(_In_ const RECT* lpRect) const throw();
	};

	/////////////////////////////////////////////////////////////////////////////
	// CPoint - A 2-D point, similar to Windows POINT structure.
	class CPoint : public tagPOINT
	{
	public:
		// Constructors

		// create an uninitialized point
		CPoint() throw();
		// create from two integers
		CPoint(
			_In_ int initX,
			_In_ int initY) throw();
		// create from another point
		CPoint(_In_ POINT initPt) throw();
		// create from a size
		CPoint(_In_ SIZE initSize) throw();
		// create from an LPARAM: x = LOWORD(dw) y = HIWORD(dw)
		CPoint(_In_ LPARAM dwPoint) throw();


		// Operations

		// translate the point
		void Offset(
			_In_ int xOffset,
			_In_ int yOffset) throw();
		void Offset(_In_ POINT point) throw();
		void Offset(_In_ SIZE size) throw();
		void SetPoint(
			_In_ int X,
			_In_ int Y) throw();

		BOOL operator==(_In_ POINT point) const throw();
		BOOL operator!=(_In_ POINT point) const throw();
		void operator+=(_In_ SIZE size) throw();
		void operator-=(_In_ SIZE size) throw();
		void operator+=(_In_ POINT point) throw();
		void operator-=(_In_ POINT point) throw();

		// Operators returning CPoint values
		CPoint operator+(_In_ SIZE size) const throw();
		CPoint operator-(_In_ SIZE size) const throw();
		CPoint operator-() const throw();
		CPoint operator+(_In_ POINT point) const throw();

		// Operators returning CSize values
		CSize operator-(_In_ POINT point) const throw();

		// Operators returning CRect values
		CRect operator+(_In_ const RECT* lpRect) const throw();
		CRect operator-(_In_ const RECT* lpRect) const throw();
	};

	/////////////////////////////////////////////////////////////////////////////
	// CRect - A 2-D rectangle, similar to Windows RECT structure.
	class CRect : public tagRECT
	{
		// Constructors
	public:
		// uninitialized rectangle
		CRect() throw();
		// from left, top, right, and bottom
		CRect(
			_In_ int l,
			_In_ int t,
			_In_ int r,
			_In_ int b) throw();
		// copy constructor
		CRect(_In_ const RECT& srcRect) throw();

		// from a pointer to another rect
		CRect(_In_ LPCRECT lpSrcRect) throw();
		// from a point and size
		CRect(
			_In_ POINT point,
			_In_ SIZE size) throw();
		// from two points
		CRect(
			_In_ POINT topLeft,
			_In_ POINT bottomRight) throw();

		// Attributes (in addition to RECT members)

		// retrieves the width
		int Width() const throw();
		// returns the height
		int Height() const throw();
		// returns the size
		CSize Size() const throw();
		// reference to the top-left point
		CPoint& TopLeft() throw();
		// reference to the bottom-right point
		CPoint& BottomRight() throw();
		// const reference to the top-left point
		const CPoint& TopLeft() const throw();
		// const reference to the bottom-right point
		const CPoint& BottomRight() const throw();
		// the geometric center point of the rectangle
		CPoint CenterPoint() const throw();
		// swap the left and right
		void SwapLeftRight() throw();
		static void WINAPI SwapLeftRight(_Inout_ LPRECT lpRect) throw();

		// convert between CRect and LPRECT/LPCRECT (no need for &)
		operator LPRECT() throw();
		operator LPCRECT() const throw();

		// returns TRUE if rectangle has no area
		BOOL IsRectEmpty() const throw();
		// returns TRUE if rectangle is at (0,0) and has no area
		BOOL IsRectNull() const throw();
		// returns TRUE if point is within rectangle
		BOOL PtInRect(_In_ POINT point) const throw();

		// Operations

		// set rectangle from left, top, right, and bottom
		void SetRect(
			_In_ int x1,
			_In_ int y1,
			_In_ int x2,
			_In_ int y2) throw();
		void SetRect(
			_In_ POINT topLeft,
			_In_ POINT bottomRight) throw();
		// empty the rectangle
		void SetRectEmpty() throw();
		// copy from another rectangle
		void CopyRect(_In_ LPCRECT lpSrcRect) throw();
		// TRUE if exactly the same as another rectangle
		BOOL EqualRect(_In_ LPCRECT lpRect) const throw();

		// Inflate rectangle's width and height by
		// x units to the left and right ends of the rectangle
		// and y units to the top and bottom.
		void InflateRect(
			_In_ int x,
			_In_ int y) throw();
		// Inflate rectangle's width and height by
		// size.cx units to the left and right ends of the rectangle
		// and size.cy units to the top and bottom.
		void InflateRect(_In_ SIZE size) throw();
		// Inflate rectangle's width and height by moving individual sides.
		// Left side is moved to the left, right side is moved to the right,
		// top is moved up and bottom is moved down.
		void InflateRect(_In_ LPCRECT lpRect) throw();
		void InflateRect(
			_In_ int l,
			_In_ int t,
			_In_ int r,
			_In_ int b) throw();

		// deflate the rectangle's width and height without
		// moving its top or left
		void DeflateRect(
			_In_ int x,
			_In_ int y) throw();
		void DeflateRect(_In_ SIZE size) throw();
		void DeflateRect(_In_ LPCRECT lpRect) throw();
		void DeflateRect(
			_In_ int l,
			_In_ int t,
			_In_ int r,
			_In_ int b) throw();

		// translate the rectangle by moving its top and left
		void OffsetRect(
			_In_ int x,
			_In_ int y) throw();
		void OffsetRect(_In_ SIZE size) throw();
		void OffsetRect(_In_ POINT point) throw();
		void NormalizeRect() throw();

		// absolute position of rectangle
		void MoveToY(_In_ int y) throw();
		void MoveToX(_In_ int x) throw();
		void MoveToXY(
			_In_ int x,
			_In_ int y) throw();
		void MoveToXY(_In_ POINT point) throw();

		// set this rectangle to intersection of two others
		BOOL IntersectRect(
			_In_ LPCRECT lpRect1,
			_In_ LPCRECT lpRect2) throw();

		// set this rectangle to bounding union of two others
		BOOL UnionRect(
			_In_ LPCRECT lpRect1,
			_In_ LPCRECT lpRect2) throw();

		// set this rectangle to minimum of two others
		BOOL SubtractRect(
			_In_ LPCRECT lpRectSrc1,
			_In_ LPCRECT lpRectSrc2) throw();

		// Additional Operations
		void operator=(_In_ const RECT& srcRect) throw();
		BOOL operator==(_In_ const RECT& rect) const throw();
		BOOL operator!=(_In_ const RECT& rect) const throw();
		void operator+=(_In_ POINT point) throw();
		void operator+=(_In_ SIZE size) throw();
		void operator+=(_In_ LPCRECT lpRect) throw();
		void operator-=(_In_ POINT point) throw();
		void operator-=(_In_ SIZE size) throw();
		void operator-=(_In_ LPCRECT lpRect) throw();
		void operator&=(_In_ const RECT& rect) throw();
		void operator|=(_In_ const RECT& rect) throw();

		// Operators returning CRect values
		CRect operator+(_In_ POINT point) const throw();
		CRect operator-(_In_ POINT point) const throw();
		CRect operator+(_In_ LPCRECT lpRect) const throw();
		CRect operator+(_In_ SIZE size) const throw();
		CRect operator-(_In_ SIZE size) const throw();
		CRect operator-(_In_ LPCRECT lpRect) const throw();
		CRect operator&(_In_ const RECT& rect2) const throw();
		CRect operator|(_In_ const RECT& rect2) const throw();
		CRect MulDiv(
			_In_ int nMultiplier,
			_In_ int nDivisor) const throw();
	};

	typedef enum _emParseType
	{
		PARSE_CMD,
		PARSE_XML,
		PARSE_TOKEN,
	}PARSETYPE;

	HRESULT UNIVERSAL_API CreateScriptParsing(LPVOID& pParsing, PARSETYPE emType);

	UNIVERSAL_API extern HRESULT g_hRes;

	UNIVERSAL_API extern CLogSystem g_oLog;
}



UNIVERSAL_API HRESULT _stdcall CreateComper(LPVOID* pComper);

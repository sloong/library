#pragma once

#include "IUniversal.h"

using SoaringLoong::LOGLEVEL;
using SoaringLoong::LOGTYPE;
using SoaringLoong::ILogSystem;

class CLogSystem : public ILogSystem
{
public:
	CLogSystem(LPCTSTR szPathName = TEXT("Log.log"), LOGLEVEL emLevel = LOGLEVEL::All, LOGTYPE emType = LOGTYPE::ONEFILE, bool bIsCoverPrev = false);

	~CLogSystem();

	virtual HRESULT _stdcall QueryInterface(const IID& riid, void** ppvObject);
	virtual ULONG _stdcall AddRef();
	virtual ULONG _stdcall Release();

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
	DWORD _stdcall WriteLog(LPCTSTR szMessage);

	//--- WriteLog Function annotation ---
	// Parameters:
	//		szMessage
	//			The string for you add to log file. it will add "time" begin string and "\n" in string last.
	// Remarks:
	//		WriteLog szMessage to log file.
	void _stdcall Write(LPCTSTR szLog);

	//--- ResLog Function annotation ---
	// Parameters:
	//		lpstrErrText
	//			The Error text, if g_hRes is not S_OK, the error text will append to log file.
	// Remarks:
	//		Check result function, if no error, function return ,
	//		if error, the error text will append to log file.
	void _stdcall ResLog(LOGLEVEL emLevel, DWORD dwCode, LPCTSTR strErrorText, bool bFormatWinMsg = true, bool bJustFailedWrite = true);

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
	ULONG m_Ref;
	bool		m_bIsCoverPrev;
};
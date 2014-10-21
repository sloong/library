#pragma once

#include "IUniversal.h"

using SoaringLoong::LOGLEVEL;
using SoaringLoong::LOGTYPE;
using SoaringLoong::ILogSystem;
using SoaringLoong::IUniversal;

class CLogSystem : public ILogSystem
{
public:
	CLogSystem();
	~CLogSystem();

	static int Init();
	virtual HRESULT _stdcall QueryInterface(const IID& riid, void** ppvObject);
	virtual ULONG _stdcall AddRef();
	virtual ULONG _stdcall Release();

	virtual void _stdcall Initialize(IUniversal* pUniversal, LPCTSTR szPathName = TEXT("Log.log"), LOGLEVEL emLevel = LOGLEVEL::All, LOGTYPE emType = LOGTYPE::ONEFILE, bool bIsCoverPrev = false);

	DWORD _stdcall Write(LPCTSTR szMessage);
	void _stdcall WriteLine(LPCTSTR szLog);
	void _stdcall Log(LOGLEVEL emLevel, DWORD dwCode, LPCTSTR strErrorText, bool bFormatWinMsg = true, bool bJustFailedWrite = true);
	HRESULT FormatWindowsErrorMessage(LPTSTR szErrText, DWORD dwSize, DWORD dwErrCode);
	void SetConfiguration(LPCTSTR szFileName, LPCTSTR szFilePath, LOGTYPE* pType, LOGLEVEL* pLevel);
	bool IsOpen();
	void Close();
	LPCTSTR GetFileName();
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
	ULONG		m_Ref;
	bool		m_bIsCoverPrev;
	IUniversal*	m_pUniversal;
	static ULONG m_objNum;
	static CRITICAL_SECTION m_cs;
};
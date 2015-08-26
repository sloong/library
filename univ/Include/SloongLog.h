#pragma once

#include "SloongUniversal.h"
using namespace Sloong::Universal;


namespace Sloong
{
	namespace Universal
	{

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
		
		class UNIVERSAL_API CLog
		{
		public:
			CLog();
			~CLog();

			virtual void Initialize(wstring szPathName = L"Log.log", LOGLEVEL emLevel = LOGLEVEL::All, LOGTYPE emType = LOGTYPE::ONEFILE, bool bIsCoverPrev = false);

			virtual DWORD Write(wstring szMessage);
			virtual void WriteLine(wstring szLog);
			virtual void Log(LOGLEVEL emLevel, DWORD dwCode, wstring strErrorText, bool bFormatWinMsg = true, bool bJustFailedWrite = true);
			virtual void SetConfiguration(wstring szFileName, wstring szFilePath, LOGTYPE* pType, LOGLEVEL* pLevel);
			virtual bool IsOpen();
			virtual void Close();
			virtual wstring GetFileName();
			virtual wstring GetPath();

		protected:
			wstring FormatFatalMessage(DWORD dwCode, wstring strErrorText, bool bFormatWinMsg, bool bJustFailedWrite);
			wstring FormatErrorMessage(DWORD dwCode, wstring strErrorText, bool bFormatWinMsg, bool bJustFailedWrite);
			wstring FormatWarningMessage(DWORD dwCode, wstring strErrorText, bool bFormatWinMsg, bool bJustFailedWrite);
			wstring FormatInformationMessage(DWORD dwCode, wstring strErrorText, bool bFormatWinMsg, bool bJustFailedWrite);
			HRESULT OpenFile();

		protected:
			LOGLEVEL	m_emLevel;
			HANDLE		m_hFileHandle;
			wstring		m_szFilePath;
			wstring		m_szFileName;
			wstring		m_szLastDate;
			int			m_emType;
			bool		m_bIsCoverPrev;
			HRESULT g_hRes;
		};
	}
}
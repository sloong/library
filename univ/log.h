#ifndef LOG_H
#define LOG_H


#include "univ.h"
#include "string/string.h"
#pragma comment(lib,"string.lib")
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

			virtual void Initialize(CString szPathName = L"Log.log", LOGLEVEL emLevel = LOGLEVEL::All, LOGTYPE emType = LOGTYPE::ONEFILE, bool bIsCoverPrev = false);

			virtual void Write(CString szMessage);
			virtual void WriteLine(CString szLog);
			virtual void Log(LOGLEVEL emLevel, DWORD dwCode, CString strErrorText, bool bFormatWinMsg = true, bool bJustFailedWrite = true);
			virtual void SetConfiguration(CString szFileName, CString szFilePath, LOGTYPE* pType, LOGLEVEL* pLevel);
			virtual bool IsOpen();
			virtual void Close();
			virtual CString GetFileName();
			virtual CString GetPath();

		protected:
			CString FormatFatalMessage(DWORD dwCode, CString strErrorText, bool bFormatWinMsg, bool bJustFailedWrite);
			CString FormatErrorMessage(DWORD dwCode, CString strErrorText, bool bFormatWinMsg, bool bJustFailedWrite);
			CString FormatWarningMessage(DWORD dwCode, CString strErrorText, bool bFormatWinMsg, bool bJustFailedWrite);
			CString FormatInformationMessage(DWORD dwCode, CString strErrorText, bool bFormatWinMsg, bool bJustFailedWrite);
			bool OpenFile();
			static LPVOID LogSystemWorkLoop(LPVOID param);

		protected:
			LOGLEVEL	m_emLevel;
			ofstream	m_oFile;
			CString		m_szFilePath;
			CString		m_szFileName;
			CString		m_szLastDate;
			int		m_emType;
			bool		m_bIsCoverPrev;
			bool		g_hRes;
			bool		m_bInit;
            static bool     g_bDebug;
		};
	}
}

#endif // !LOG_H


#ifndef LOG_H
#define LOG_H


#include "univ.h"
#ifdef _WINDOWS
#endif
using namespace Sloong::Universal;

#include <boost/format.hpp>

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

			virtual void Initialize(std::string szPathName = "./Log.log", LOGLEVEL emLevel = LOGLEVEL::All, LOGTYPE emType = LOGTYPE::ONEFILE, bool bIsCoverPrev = false);

			virtual void Write(std::string szMessage);
			virtual void WriteLine(std::string szLog);
			virtual void Log(LOGLEVEL emLevel, DWORD dwCode, std::string strErrorText, bool bFormatWinMsg = true, bool bJustFailedWrite = true);
			virtual void SetConfiguration(std::string szFileName, std::string szFilePath, LOGTYPE* pType, LOGLEVEL* pLevel);
			virtual bool IsOpen();
			virtual void Close();
			virtual std::string GetFileName();
			virtual std::string GetPath();
			virtual bool IsInitialize();

		protected:
			std::string FormatFatalMessage(DWORD dwCode, std::string strErrorText, bool bFormatWinMsg, bool bJustFailedWrite);
			std::string FormatErrorMessage(DWORD dwCode, std::string strErrorText, bool bFormatWinMsg, bool bJustFailedWrite);
			std::string FormatWarningMessage(DWORD dwCode, std::string strErrorText, bool bFormatWinMsg, bool bJustFailedWrite);
			std::string FormatInformationMessage(DWORD dwCode, std::string strErrorText, bool bFormatWinMsg, bool bJustFailedWrite);
			bool OpenFile();
			static LPVOID LogSystemWorkLoop(LPVOID param);

		protected:
			LOGLEVEL	m_emLevel;
			ofstream	m_oFile;
			std::string		m_szFilePath;
			std::string		m_szFileName;
			std::string		m_szLastDate;
			int		m_emType;
			bool		m_bIsCoverPrev;
			bool		g_hRes;
			bool		m_bInit;
		public:
			static void showLog( LOGLEVEL level, std::string str );
			static void showLog( LOGLEVEL level, boost::format& ft );
            static bool     g_bDebug;
		};
	}
}

#endif // !LOG_H


#ifndef LOG_H
#define LOG_H


#include "univ.h"
#ifdef _WINDOWS
#else
#include <semaphore.h>
#endif
using namespace Sloong::Universal;

#include <boost/format.hpp>
#include <condition_variable>
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

            virtual void Initialize(std::string szPathName = "./Log.log", bool bDebug = true, LOGLEVEL emLevel = LOGLEVEL::All, LOGTYPE emType = LOGTYPE::ONEFILE, bool bIsCoverPrev = false);
			virtual void Start();
			virtual void End();
			/************************************************************************/
			/* SetWorkInterval function                                             */
			/* Set the log system work loop sleep time when the log list is empty   */
			/* in default, it is 100. you can change it by your need.               */
			/************************************************************************/
			virtual void SetWorkInterval(int nInterval = 100);
			virtual void Write(std::string szMessage);
			virtual void WriteLine(std::string szLog);
			virtual void Log(std::string strErrorText, LOGLEVEL emLevel = LOGLEVEL::INF, DWORD dwCode = 0 , bool bFormatSysMsg = true, bool bJustFailedWrite = true);
			virtual void Info(std::string strInfo, std::string strTitle="INFO");
            virtual void SetConfiguration(std::string szFileName, LOGTYPE* pType, LOGLEVEL* pLevel, bool bDeubg = true);
			virtual bool IsOpen();
			virtual void Close();
			virtual std::string GetFileName();
			virtual std::string GetPath();
			virtual bool IsInitialize();

		protected:
			std::string FormatFatalMessage(DWORD dwCode, std::string strErrorText);
			std::string FormatErrorMessage(DWORD dwCode, std::string strErrorText);
			std::string FormatWarningMessage(DWORD dwCode, std::string strErrorText);
			std::string FormatInformationMessage(DWORD dwCode, std::string strErrorText);
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
            bool        m_bDebug;
			int			m_nSleepInterval;
			condition_variable m_CV;
			mutex		m_Mutex;
			bool		m_bRunning;
		};
	}
}

#endif // !LOG_H


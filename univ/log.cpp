/************************************************************************/
/* 				StrandedError.cpp --- Error Message                     */
/************************************************************************/
#include "stdafx.h"
#include "univ.h"
#include "log.h"
#include "exception.h"
#include "threadpool.h"
using namespace Sloong;

const string g_strStart = "---------------------------------Start---------------------------------";
const string g_strEnd = "----------------------------------End----------------------------------";
const string g_strConnect = "-------Network log system connected-------";

#ifndef _WINDOWS
#include <errno.h>
#define PATH_SEPARATOR '/'
#else
#define PATH_SEPARATOR '\\'
#endif // !_WINDOWS

WCHAR g_szFormatBuffer[2048];

Sloong::Universal::CLog::CLog()
{
	m_bInit = false;
	m_nLastDate = 0;
	m_pFile = nullptr;
	m_pCustomFunction = nullptr;
}


Sloong::Universal::CLog::~CLog()
{
	End();
	m_bInit = false;
}


void Sloong::Universal::CLog::Log(std::string strErrorText, string strTitle, DWORD dwCode /* = 0 */, bool bFormatSysMsg /* = false */)
{
    WriteLine(CUniversal::Format("[%s]:[%s]", strTitle, strErrorText));
		
	if (bFormatSysMsg)
	{
		DWORD dwSysCode;
		string errMsg;
#ifdef _WINDOWS
		dwSysCode = GetLastError();
		errMsg = CUniversal::toansi(CUniversal::FormatWindowsErrorMessage(dwSysCode));
		errMsg = errMsg.substr(0, errMsg.length() - 2);
#else
		dwSysCode = errno;
		errMsg = strerror(dwSysCode);
#endif
		if (0 != dwSysCode)
		{
			std::string str = CUniversal::Format("[SYS CODE]:[%d];[SYSTEM MESSAGE]:[%s]", dwSysCode, errMsg.c_str());
			WriteLine(str);
		}
	}
	
}


void Sloong::Universal::CLog::Log(std::string strErrorText, LOGLEVEL level)
{
	switch (level)
	{
	case LOGLEVEL::Assert:
		Assert(strErrorText);
		break;
	case LOGLEVEL::Debug:
		Debug(strErrorText);
		break;
		case LOGLEVEL::Error:
			Error(strErrorText);
			break;
		case LOGLEVEL::Fatal:
			Fatal(strErrorText);
			break;
		case LOGLEVEL::Info:
			Info(strErrorText);
			break;
		case LOGLEVEL::Verbos:
			Verbos(strErrorText);
			break;
		case LOGLEVEL::Warn:
			Warn(strErrorText);
			break;
	default:
		break;
	}
}

void Sloong::Universal::CLog::WriteLine(std::string szLog)
{
	if (szLog.empty())
		return;

	time_t st;
    time(&st);
	struct tm* lt = localtime(&st);
	Write(CUniversal::Format("[%d/%d/%d-%.2d:%.2d:%.2d]:%s\n",(lt->tm_year + 1900) , lt->tm_mon , lt->tm_mday ,
		lt->tm_hour, lt->tm_min, lt->tm_sec,szLog));
}

void Sloong::Universal::CLog::Write(std::string szMessage)
{
	unique_lock <mutex> list_lock(m_oLogListMutex);
	m_waitWriteList.push(szMessage);
	m_CV.notify_all();
}


void CLog::ProcessLogList()
{
	unique_lock <mutex> list_lock(m_oWriteMutex);
	while (!m_logList.empty())
	{
		// get log message from queue.
		string str = m_logList.front();
		m_logList.pop();
		if( m_emOperation & LOGOPT::WriteToSTDOut)
			cout << str;
		if( (m_emOperation & LOGOPT::WriteToFile )&& m_pFile != nullptr )
			fputs(str.c_str(), m_pFile);
		if( m_emOperation & LOGOPT::ImmediatelyFlush)
			Flush();
		if( (m_emOperation & LOGOPT::WriteToCustomFunction) && m_pCustomFunction != nullptr )
			m_pCustomFunction(str);
	}
}

void CLog::LogSystemWorkLoop()
{
	unique_lock <mutex> lck(m_Mutex);
	while (m_emStatus != RUN_STATUS::Exit)
	{
		if (m_emStatus == RUN_STATUS::Created)
			continue;

		if( !IsOpen())
		{
			m_CV.wait_for(lck,chrono::milliseconds(500));
			continue;
		}

		if (m_waitWriteList.empty() && m_logList.empty())
		{
			m_CV.wait_for(lck,chrono::milliseconds(10));
			continue;
		}

		ProcessWaitList();	
		ProcessLogList();
	}
}


void CLog::ProcessWaitList()
{
	if (!m_waitWriteList.empty())
	{
		unique_lock <mutex> list_lock(m_oLogListMutex);
		while (!m_waitWriteList.empty())
		{
			m_logList.push(m_waitWriteList.front());
			m_waitWriteList.pop();
		}
		list_lock.unlock();
	}
}

bool Sloong::Universal::CLog::OpenFile()
{
	if(!(m_emOperation & LOGOPT::WriteToFile))
		return true;
	if (m_pFile != nullptr )
		return true;
	if (m_szFileName.empty())
		throw normal_except("Open log file failed.file name is empty.");

	cout << "Open log file. Path>>" << m_szFileName << endl;
	
	CUniversal::CheckFileDirectory(m_szFileName);
	auto flag = "a+";
	if ( m_emOperation & LOGOPT::AlwaysCreate )
		flag = "w+";

	int err_code;
#ifdef _WINDOWS
	err_code = fopen_s(&m_pFile, m_szFileName.c_str(), flag);
#else
	m_pFile = fopen(m_szFileName.c_str(),flag);
	err_code = errno;
#endif
	if ( m_pFile == nullptr)
	{
		cerr << "Open file error. error no " << err_code << endl;
	}
	return m_pFile != nullptr;
}

void Sloong::Universal::CLog::RegisterCustomFunction( pCustomLogFunction func )
{
	if( func != nullptr )
	{
		m_pCustomFunction = func;
		m_emOperation = (LOGOPT)(m_emOperation | LOGOPT::WriteToCustomFunction);
	}
}

std::string Sloong::Universal::CLog::GetFileName()
{
	return m_szFileName;
}

bool Sloong::Universal::CLog::IsOpen()
{
	if (!m_bInit)
		throw normal_except("No Initialize!");
	if(!(m_emOperation & LOGOPT::WriteToFile))
		return true;
	if ( m_emType != LOGTYPE::ONEFILE)
	{
		time_t now;
		struct tm* tmNow;
		time(&now);
		tmNow = localtime(&now);
		
		if ((m_nLastDate != tmNow->tm_mday) || m_nLastDate == 0 )
		{
			char szCurrentDate[10];
			static const char format[3][10] = { ("%Y"), ("%Y-%m"), ("%Y%m%d") };
			strftime(szCurrentDate, 9, format[m_emType], tmNow);
			m_szFileName = CUniversal::Format("%s%s%s.log", m_szFilePath, szCurrentDate, m_strExtendName);
			m_nLastDate = tmNow->tm_mday;
			Close();
		}
	}

	return OpenFile();
}

void Sloong::Universal::CLog::Close()
{
	if (m_pFile!= nullptr)
	{
		fclose(m_pFile);
		m_pFile = nullptr;
	}
}


void Sloong::Universal::CLog::End()
{
	if ( !m_bInit )	{
		return;
	}
	m_emStatus = RUN_STATUS::Exit;
	WriteLine(g_strEnd);
	if(IsOpen())
	{
		ProcessWaitList();
		ProcessLogList();
		Flush();
		Close();
	}
}


std::string Sloong::Universal::CLog::GetPath()
{
	return m_szFilePath;
}


/************************************************************************/
/* Flush function                                                       */
/* Add for issue #9 [https://git.sloong.com/public/library/issues/9]    */
/************************************************************************/
void Sloong::Universal::CLog::Flush()
{
	if( m_pFile != nullptr)
		fflush(m_pFile);
}

void CLog::SetConfiguration(std::string szFileName, LOGTYPE* pType, LOGLEVEL* pLevel, LOGOPT* pOpt , string* strExtendName )
{
	if (pType)
	{
		m_emType = *pType;
		WriteLine(CUniversal::Format("[Info]:[Set log file type to %d]",m_emType));
	}

	if (!szFileName.empty())
	{
		if ( m_emType != LOGTYPE::ONEFILE)
		{
			szFileName = CUniversal::replace(szFileName, "/", CUniversal::ntos(PATH_SEPARATOR));
			szFileName = CUniversal::replace(szFileName, "\\", CUniversal::ntos(PATH_SEPARATOR));
			char pLast = szFileName.c_str()[szFileName.length() - 1];
			if (pLast != PATH_SEPARATOR)
			{
				szFileName += PATH_SEPARATOR;
			}
			m_szFilePath = szFileName;
		}
		else
		{
			assert(szFileName.c_str());
			Close();
			m_szFileName = szFileName;
		}
	}

	if( strExtendName)
	{
		m_strExtendName = *strExtendName;
		WriteLine(CUniversal::Format("[Info]:[Set extend name to %d]",strExtendName));
	}
	

	if (pLevel)
	{
		m_emLevel = *pLevel;
		WriteLine(CUniversal::Format("[Info]:[Set log level to %d]",m_emLevel));
	}

	if( pOpt )
	{
		m_emOperation = *pOpt;
		WriteLine(CUniversal::Format("[Info]:[Set Operation to %d]",m_emOperation));
	}

}

void Sloong::Universal::CLog::Initialize()
{
	Initialize("./log.log");
}

void Sloong::Universal::CLog::Initialize(string szPathName, string strExtendName /*= ""*/, LOGOPT emOpt /*= WriteToFile */, LOGLEVEL emLevel /*= LOGLEVEL::All*/, LOGTYPE emType /*= LOGTYPE::ONEFILE*/)
{
	// All value init
	m_bInit = true;
	m_szFilePath.clear();
	m_szFileName.clear();
	
    SetConfiguration( szPathName, &emType, &emLevel, &emOpt, &strExtendName);

	Start();
}

bool Sloong::Universal::CLog::IsInitialize()
{
	return m_bInit;
}

void Sloong::Universal::CLog::Start()
{
	if (m_emStatus == RUN_STATUS::Running)
		return;

	m_emStatus = RUN_STATUS::Running;
	std::function<void(void)> workLoop = std::bind(&CLog::LogSystemWorkLoop, this);
	m_pThread = make_unique<thread>(workLoop);
	WriteLine(g_strStart);
}

void Sloong::Universal::CLog::Info(std::string strMsg)
{
	if (m_emLevel > LOGLEVEL::Info)
		return;
	Log(strMsg, "Info");
}

void Sloong::Universal::CLog::Warn(std::string strMsg)
{
	if (m_emLevel > LOGLEVEL::Warn)
		return;
	Log(strMsg, "Warn");
}

void Sloong::Universal::CLog::Error(std::string strMsg)
{
	if (m_emLevel > LOGLEVEL::Error)
		return;
	Log(strMsg, "Error");
}

void Sloong::Universal::CLog::Assert(std::string strMsg)
{
	if (m_emLevel > LOGLEVEL::Assert)
		return;
	Log(strMsg, "Assert");
}

void Sloong::Universal::CLog::Fatal(std::string strMsg)
{
	if (m_emLevel > LOGLEVEL::Fatal)
		return;
	Log(strMsg, "Fatal");
}

void Sloong::Universal::CLog::Verbos(std::string strMsg)
{
	if (m_emLevel > LOGLEVEL::Verbos)
		return;
	Log(strMsg, "Verbos");
}


void Sloong::Universal::CLog::Debug(std::string strMsg)
{
	if (m_emLevel > LOGLEVEL::Debug)
		return;
	Log(strMsg, "Debug");
}

/************************************************************************/
/* 				StrandedError.cpp --- Error Message                     */
/************************************************************************/
#include "stdafx.h"
#include "univ.h"
#include "log.h"
#include "exception.h"
#include "threadpool.h"
#include <assert.h>
#include <thread>
#include <queue>
#include <mutex>
#include <iostream>
#include <fstream>
#include <stdarg.h> // for va_list,va_start and va_end
#include <boost/format.hpp>
//#include <boost/algorithm/string.hpp>
using namespace Sloong;
mutex g_oLogListMutex;
queue<string> g_logList;
const string g_szStart = "---------------------------------Start---------------------------------";
const string g_szEnd = "----------------------------------End----------------------------------";
#ifdef _WINDOWS
const string g_szNewLine = "\r\n";

#else
const string g_szNewLine = "\n";
#include <errno.h>
#endif // !_WINDOWS

WCHAR g_szFormatBuffer[2048];

CLog::CLog()
{
	m_bInit = false;
    m_bDebug = true;
	m_nLastDate = 0;
	m_bOpenFileFirst = false;
}


CLog::~CLog()
{
	End();
	m_bInit = false;
}



void CLog::Log(std::string strErrorText, string strTitle, DWORD dwCode /* = 0 */, bool bFormatSysMsg /* = false */)
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

void CLog::WriteLine(std::string szLog)
{
	if (szLog.empty())
		return;

	time_t st;
    time(&st);
	struct tm* lt = localtime(&st);
	Write(CUniversal::Format("[%d/%d/%d - %.2d:%.2d:%.2d]:%s%s",(lt->tm_year + 1900) , lt->tm_mon , lt->tm_mday ,
		lt->tm_hour, lt->tm_min, lt->tm_sec,szLog,g_szNewLine));
}

void CLog::Write(std::string szMessage)
{
	//lock_guard<mutex> lck(g_oLogListMutex);
	unique_lock <mutex> lck(m_Mutex);
	g_logList.push(szMessage);
	m_CV.notify_all();
}


void* CLog::LogSystemWorkLoop(void* param)
{
	CLog* pThis = (CLog*)param;
	while (pThis->m_bRunning)
	{
		if( !g_logList.empty() )
        {
            unique_lock<mutex> lck(g_oLogListMutex);
            if ( g_logList.empty() )
            {
				lck.unlock();
                continue;
            }
			// get log message from queue.
			string str = g_logList.front();
			g_logList.pop();
            lck.unlock();

			pThis->IsOpen();

			// write log message to file
			pThis->m_oFile << str;
			// in debug mode, flush the message when write down. 
			// for issue #8 [https://git.sloong.com/public/library/issues/8]
			if (pThis->m_bDebug)
			{
				cout << str;
				pThis->m_oFile.flush();
			}
				
		}
        else
        {
			unique_lock <mutex> lck(pThis->m_Mutex);
			pThis->m_CV.wait(lck);
        }
	}
	return 0;
}

bool CLog::OpenFile()
{
	if (m_oFile.is_open())
		return true;
	if (m_szFileName.empty())
		throw normal_except("Open log file failed.file name is empty.");

	if (m_bOpenFileFirst)
	{
		m_bOpenFileFirst = true;
	}
	else
	{
		cout << "File no open , try open file. file path is :" << CUniversal::toansi(m_szFileName) << endl;
	}
	wstring folder = m_szFileName.substr(0,m_szFileName.find_last_of(L'/'));
	CUniversal::CheckFileDirectory(CUniversal::toansi(folder));
	auto flag = ios::out | ios::app;
	if (m_bIsCoverPrev == true)
		flag = ios::out;

	m_oFile.open(CUniversal::toansi(m_szFileName).c_str(),flag);

	return m_oFile.is_open();
}

std::wstring CLog::GetFileName()
{
	return m_szFileName;
}

std::string Sloong::Universal::CLog::GetFileNameA()
{
	return CUniversal::toansi(GetFileName());
}

bool CLog::IsOpen()
{
	if (!m_bInit)
		throw normal_except("No Initialize!");
	if (m_emType != LOGTYPE::ONEFILE)
	{
		
		time_t now;
		struct tm* tmNow;
		time(&now);
		tmNow = localtime(&now);
		
		if (m_nLastDate == 0 || (m_nLastDate != tmNow->tm_mday))
		{
			char szCurrentDate[10];
			static const char format[3][10] = { ("%Y"), ("%Y-%m"), ("%Y%m%d") };
			strftime(szCurrentDate, 9, format[m_emType], tmNow);
			m_szFileName = CUniversal::Format(L"%s%s.log", m_szFilePath, szCurrentDate);
			m_nLastDate = tmNow->tm_mday;
			Close();
		}
	}

	return OpenFile();
}

void CLog::Close()
{
	if (m_oFile.is_open())
	{
		m_oFile.close();
	}
}


void CLog::End()
{
	WriteLine(g_szEnd);
	Close();
	m_bRunning = false;
	m_CV.notify_all();
}


std::wstring CLog::GetPath()
{
	return m_szFilePath;
}

std::string Sloong::Universal::CLog::GetPathA()
{
	return CUniversal::toansi(GetPath());
}


/************************************************************************/
/* Flush function                                                       */
/* Add for issue #9 [https://git.sloong.com/public/library/issues/9]    */
/************************************************************************/
void Sloong::Universal::CLog::Flush()
{
	m_oFile.flush();
}

void CLog::SetConfiguration(std::wstring szFileName, LOGTYPE* pType, LOGLEVEL* pLevel, bool bDebug /* = true */)
{
	if (pType)
	{
		m_emType = *pType;
	}

	if (!szFileName.empty())
	{
		if ( m_emType != LOGTYPE::ONEFILE)
		{
			CUniversal::replace(szFileName, L"\\", L"/");
			wchar_t pLast = szFileName.c_str()[szFileName.length() - 1];
			if (pLast != L'/')
			{
				szFileName += L"/";
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


	if (pLevel)
	{
		m_emLevel = *pLevel;
		WriteLine(CUniversal::Format("[Info]:[Set Log Level To %d]",m_emLevel));
	}

    m_bDebug = bDebug;
}

void CLog::SetConfiguration(std::string szFileName, LOGTYPE* pType, LOGLEVEL* pLevel, bool bDebug /* = true */)
{
	SetConfiguration(CUniversal::toutf(szFileName), pType, pLevel, bDebug);
}


void Sloong::Universal::CLog::Initialize()
{
	Initialize(L"./log.log");
}

void CLog::Initialize(wstring szPathName, bool bDebug /*= true */, LOGLEVEL emLevel /*= LOGLEVEL::All*/, LOGTYPE emType /*= LOGTYPE::ONEFILE*/, bool bIsCoverPrev /*= false*/)
{
	
	// All value init
	m_bInit = true;
	m_szFilePath.clear();
	m_szFileName.clear();
	m_nLastDate = 0;
	m_bIsCoverPrev = false;

	// Set value
	m_bIsCoverPrev = bIsCoverPrev;
	
    SetConfiguration( szPathName, &emType, &emLevel, bDebug);

	Start();
}

void CLog::Initialize(string szPathName , bool bDebug /*= true */, LOGLEVEL emLevel /*= LOGLEVEL::All*/, LOGTYPE emType /*= LOGTYPE::ONEFILE*/, bool bIsCoverPrev /*= false*/)
{
	Initialize(CUniversal::toutf(szPathName), bDebug, emLevel, emType, bIsCoverPrev);
}

bool CLog::IsInitialize()
{
	return m_bInit;
}

void Sloong::Universal::CLog::SetWorkInterval(int nInterval /*= 100*/)
{
	m_nSleepInterval = nInterval;
}

void Sloong::Universal::CLog::Start()
{
	if (m_bRunning == true)
		return;

	m_bRunning = true;
	CThreadPool::AddWorkThread(CLog::LogSystemWorkLoop, this, 1);
	SetWorkInterval();
	WriteLine(g_szStart);
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


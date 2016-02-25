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
#include <stdarg.h> // for va_list,va_start and va_end
#include <boost/format.hpp>
//#include <boost/algorithm/string.hpp>
using namespace Sloong;
mutex g_oLogListMutex;
queue<string> g_logList;
const string g_szStart = "---------------------------------Start---------------------------------";
const string g_szEnd = "----------------------------------End----------------------------------";
#ifndef _WINDOWS
const string g_szNewLine = "\n";
#else
const string g_szNewLine = "\r\n";
#endif // !_WINDOWS

WCHAR g_szFormatBuffer[2048];

CLog::CLog()
{
	m_bInit = false;
    m_bDebug = true;
}


CLog::~CLog()
{
	WriteLine(g_szEnd);
	Close();
	m_bInit = false;
}

std::string CLog::FormatFatalMessage(DWORD dwCode, std::string strErrorText, bool bFormatWinMsg, bool bJustFailedWrite)
{
	if (FATAL <= m_emLevel)
	{
		if (true == g_hRes && false == bJustFailedWrite)
		{
			strErrorText = (boost::format("[SUCCESS];[FATAL %05d : %s];[RETURN %d]") % dwCode % strErrorText % g_hRes).str();
		}
		else
		{
			strErrorText = (boost::format("[FAILED];[FATAL %05d : %s];[RETURN %d]") % dwCode % strErrorText % g_hRes).str();
		}
		return strErrorText;
	}
	else
	{
		return "";
	}
}

std::string CLog::FormatErrorMessage(DWORD dwCode, std::string strErrorText, bool bFormatWinMsg, bool bJustFailedWrite)
{
	if (ERR <= m_emLevel)
	{
		if (true == g_hRes && false == bJustFailedWrite)
		{
			strErrorText = (boost::format("[SUCCESS];[ERROR %05d : %s];[RETURN %d]") % dwCode % strErrorText % g_hRes).str();
		}
		else
		{
			strErrorText = (boost::format("[FAILED];[ERROR %05d : %s];[RETURN %d]") % dwCode % strErrorText % g_hRes).str();
		}
		return strErrorText;
	}
	else
	{
		return "";
	}
}

std::string CLog::FormatWarningMessage(DWORD dwCode, std::string strErrorText, bool bFormatWinMsg, bool bJustFailedWrite)
{
	if (WARN <= m_emLevel)
	{
		if (true == g_hRes && false == bJustFailedWrite)
		{
			strErrorText = (boost::format("[SUCCESS];[WARN %05d : %s];[RETURN %d]") % dwCode % strErrorText % g_hRes).str();
		}
		else
		{
			strErrorText = (boost::format("[FAILED];[WARN %05d : %s];[RETURN %d]") % dwCode % strErrorText % g_hRes).str();
		}
		return strErrorText;
	}
	else
	{
		return "";
	}
}

std::string CLog::FormatInformationMessage(DWORD dwCode, std::string strErrorText, bool bFormatWinMsg, bool bJustFailedWrite)
{
	if (INF <= m_emLevel)
	{
        strErrorText = (boost::format("[INF] : [%s]") % strErrorText).str();
		return strErrorText;
	}
	else
	{
		return "";
	}
}


void CLog::Log(std::string strErrorText, LOGLEVEL emLevel /* = INF */, DWORD dwCode /* = 0 */, bool bFormatWinMsg /* = false */, bool bJustFailedWrite /* = true */)
{
	std::string strLogText;

    if (true != g_hRes || dwCode != 0 || false == bJustFailedWrite || INF == emLevel)
	{
		switch (emLevel)
		{
		case LOGLEVEL::FATAL:
			strLogText = this->FormatFatalMessage(dwCode, strErrorText, bFormatWinMsg, bJustFailedWrite);
			//PostQuitMessage(dwCode);
			break;
		case LOGLEVEL::ERR:
			strLogText = this->FormatErrorMessage(dwCode, strErrorText, bFormatWinMsg, bJustFailedWrite);
			break;
		case LOGLEVEL::WARN:
			strLogText = this->FormatWarningMessage(dwCode, strErrorText, bFormatWinMsg, bJustFailedWrite);
			break;
		case LOGLEVEL::INF:
		case LOGLEVEL::All:
		default:
			strLogText = this->FormatInformationMessage(dwCode, strErrorText, bFormatWinMsg, bJustFailedWrite);
			break;
		}
	}

    WriteLine(strLogText);

	#ifdef _WINDOWS
	if (true != g_hRes && true == bFormatWinMsg)
	{
		DWORD dwWinErrCode = GetLastError();
		if (S_OK != dwWinErrCode)
		{
			wstring szWinErrText = CUniversal::FormatWindowsErrorMessage(dwWinErrCode);
			szWinErrText = szWinErrText.substr(0, szWinErrText.length() - 2);
			// Add WINDOWS MSG in head.
			std::string str = (boost::format("[WINDOWS MESSAGE] : [%s]") % (CUniversal::toansi(szWinErrText))).str();
			WriteLine(str);
		}
	}
	#endif
}

void CLog::WriteLine(std::string szLog)
{
	if (szLog.empty())
		return;

	time_t st;
    time(&st);
	struct tm* lt = localtime(&st);
	std::string strTime = (boost::format("[%d/%d/%d - %.2d:%.2d:%.2d] : ") %(lt->tm_year + 1900) % lt->tm_mon % lt->tm_mday %
		lt->tm_hour% lt->tm_min% lt->tm_sec).str();
	Write(strTime+szLog+g_szNewLine);
}

void CLog::Write(std::string szMessage)
{
	lock_guard<mutex> lck(g_oLogListMutex);
	g_logList.push(szMessage);
#ifdef _WINDOWS
#else
	sem_post(&m_stSem);
#endif // _WINDOWS

}


void* CLog::LogSystemWorkLoop(void* param)
{
	CLog* pThis = (CLog*)param;
	while(true)
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
            if ( pThis->m_bDebug )
				cout << str;
		}
        else
        {
#ifdef _WINDOWS
			SLEEP(pThis->m_nSleepInterval);
#else
			sem_wait(&pThis->m_stSem);
#endif // _WINDOWS

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

	cout<<"File no open , try open file. file path is :"<<m_szFileName<<endl;
	
	auto flag = ios::out | ios::app;
	if (m_bIsCoverPrev == true)
		flag = ios::out;

	m_oFile.open(m_szFileName.c_str(),flag);

	return m_oFile.is_open();
}

std::string CLog::GetFileName()
{
	return m_szFileName;
}

bool CLog::IsOpen()
{
	if (!m_bInit)
		throw normal_except("No Initialize!");
	if (m_emType != LOGTYPE::ONEFILE)
	{
		char szCurrentDate[10];
		static const char format[3][10] = { ("%Y"), ("%Y-%m"), ("%Y%m%d") };

		time_t now;
		struct tm* tmNow;
		time(&now);
		tmNow = localtime(&now);
		strftime(szCurrentDate, 9, format[m_emType], tmNow);
	
		if (m_szLastDate.empty() || (m_szLastDate!=szCurrentDate))
		{
			std::string szTemp = (boost::format("%s\\%s.log")%m_szFilePath% szCurrentDate).str();
			SetConfiguration(szTemp, "", NULL, NULL);
			m_szLastDate = szCurrentDate;
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


std::string CLog::GetPath()
{
	return m_szFilePath;
}

void CLog::SetConfiguration(std::string szFileName, std::string szFilePath, LOGTYPE* pType, LOGLEVEL* pLevel, bool bDebug /* = true */)
{
	if (!szFileName.empty())
	{
		assert(szFileName.c_str());
		Close();
		m_szFileName = szFileName;
	}

	#ifdef _WINDOWS
	if (!szFilePath.empty())
	{
		assert(szFilePath.c_str());
		WIN32_FIND_DATA wfd;
		TCHAR temp[MAX_PATH + 1] = { 0 };

		if (FindFirstFile(CUniversal::toutf(szFilePath).c_str(), &wfd) == INVALID_HANDLE_VALUE && CreateDirectory(CUniversal::toutf(szFilePath).c_str(), NULL) == 0)
		{
			assert(false);
			exit(1);
		}
		else
		{
			GetFullPathName(CUniversal::toutf(szFilePath).c_str(), MAX_PATH, temp, NULL);
			m_szFilePath = CUniversal::toansi(temp);
		}
	}
	#endif
	if (pType)
	{
		m_emType = *pType;
	}

	if (pLevel)
	{
		m_emLevel = *pLevel;
	}

    m_bDebug = bDebug;
}


void CLog::Initialize(std::string szPathName /*= TEXT("Log.log")*/, bool bDebug /*= true */, LOGLEVEL emLevel /*= LOGLEVEL::All*/, LOGTYPE emType /*= LOGTYPE::ONEFILE*/, bool bIsCoverPrev /*= false*/)
{
	// All value init
	g_hRes = true;
	m_bInit = true;
	m_emLevel = emLevel;
	m_szFilePath.clear();
	m_szFileName.clear();
	m_szLastDate.clear();
	m_bIsCoverPrev = false;

	// Set value
	m_emType = emType;
	m_bIsCoverPrev = bIsCoverPrev;
	if (emType != LOGTYPE::ONEFILE)
	{
        SetConfiguration("", szPathName, NULL, NULL, bDebug);
	}
	else
	{
        SetConfiguration(szPathName, "", NULL, NULL, bDebug);
	}
#ifdef _WINDOWS
#else
    sem_init(&m_stSem, 0, 0);
#endif // _WINDOWS
    CThreadPool::AddWorkThread(CLog::LogSystemWorkLoop,this,1);
	SetWorkInterval();
	WriteLine(g_szStart);
}

bool CLog::IsInitialize()
{
	return m_bInit;
}

void Sloong::Universal::CLog::SetWorkInterval(int nInterval /*= 100*/)
{
	m_nSleepInterval = nInterval;
}

// namespace YaoUtil {
// 
// 	// max size of ini file, in bytes
// 	const int MAX_INI_FILE_SIZE = 1024*32;
// 
// 	// default log buffer size, in bytes
// 	const size_t BUF_SIZE = 1024*4;
// 
// 	// max size of the queue, for post
// 	const size_t MAX_QUEUE_SIZE = 1000;
// 
// 	const int THREAD_CYCLE_SPAN_MILLISECOND = 2000;
// 	const int THREAD_EXIT_TIMEOUT_MILLISECOND = 300;
// 	char *g_pData = NULL;
// 	char *g_pRealData = NULL;
// 	bool g_bUseNewBuffer = false;
// 	std::queue<std::pair<std::std::string, std::std::string> > g_textData;
// 	std::queue<std::pair<std::std::string, std::std::string> > g_binData;
// 	std::map<std::std::string, P_FILE> g_files;
// 
// 	void ResetBuffer()
// 	{
// 		memset(g_pData, 0, BUF_SIZE);
// 		if (g_bUseNewBuffer)
// 		{
// 			delete [] g_pRealData;
// 			g_bUseNewBuffer = false;
// 		}
// 		g_pRealData = NULL;
// 	}
// 
// 	LOGOUT_FLAG ToFlag(const std::std::string& outFlag)
// 	{
// 		LOGOUT_FLAG flag_;
// 		if (StrUtil::CompareNoCase(outFlag, "file")) flag_ = LOGOUT_FLAG_FILE;
// 		else if (StrUtil::CompareNoCase(outFlag, "stdout")) flag_ = LOGOUT_FLAG_STDOUT;
// 		else if (StrUtil::CompareNoCase(outFlag, "remote")) flag_ = LOGOUT_FLAG_REMOTE;
// 		else flag_ = LOGOUT_FLAG_OUTPUTDEBUGSTRING;
// 		return flag_;
// 	}
// 
// 	void Queue2Vector(std::queue<std::pair<std::std::string, std::std::string> >& data_,
// 		std::vector<std::pair<std::std::string, std::vector<std::std::string> > >& new_data)
// 	{
// 		std::std::string last_key;
// 		std::vector<std::std::string> new_value;
// 		bool firstElement = true;
// 		while (!data_.empty())
// 		{
// 			std::std::string& key_ = data_.front().first;
// 			std::std::string& value_ = data_.front().second;
// 			if (firstElement)
// 			{
// 				last_key = key_;
// 				new_value.push_back(value_);
// 				firstElement = false;
// 			}
// 			else
// 			{
// 				if (key_ != last_key)
// 				{
// 					new_data.push_back(make_pair(last_key, new_value));
// 					last_key = key_;
// 					new_value.clear();
// 				}
// 				new_value.push_back(value_);
// 			}
// 			data_.pop();
// 		}
// 		if (!last_key.empty())
// 		{
// 			assert(new_value.size() > 0);
// 			new_data.push_back(make_pair(last_key, new_value));
// 		}
// 	}
// 
// 	TinyMutex::TinyMutex()
// 	{
// #ifdef _YAO_LOG_WIN32_
// 		InitializeCriticalSection(&m_handle);
// #else
// 		pthread_mutex_init(&m_handle, NULL);
// #endif
// 	}
// 
// 	TinyMutex::~TinyMutex()
// 	{
// #ifdef _YAO_LOG_WIN32_
// 		DeleteCriticalSection(&m_handle);
// #else
// 		pthread_mutex_destroy(&m_handle);
// #endif
// 	}
// 
// 	void TinyMutex::Lock()
// 	{
// #ifdef _YAO_LOG_WIN32_
// 		EnterCriticalSection(&m_handle);
// #else
// 		pthread_mutex_lock(&m_handle);
// #endif
// 	}
// 
// 	void TinyMutex::Unlock()
// 	{
// #ifdef _YAO_LOG_WIN32_
// 		LeaveCriticalSection(&m_handle);
// #else
// 		pthread_mutex_unlock(&m_handle);
// #endif
// 	}
// 
// 	void StrUtil::vFormat(yao_tstring& s, const TCHAR *fmt, va_list ap)
// 	{
// #ifdef _YAO_LOG_UNICODE_
// 		vFormatW(s, fmt, ap);
// #else
// 		vFormatA(s, fmt, ap);
// #endif
// 	}
// 
// 	void StrUtil::vFormatA(std::std::string& s, const char *fmt, va_list ap)
// 	{
// 		// Allocate a buffer on the stack that's big enough for us almost
// 		// all the time.  Be prepared to allocate dynamically if it doesn't fit.
// 		size_t size = 1024;
// 		char stackbuf[1024] = { 0 };
// 		std::vector<char> dynamicbuf;
// 		char *buf = &stackbuf[0];
// 
// 		s.clear();
// 
// 		while (1) {
// 			// Try to vsnprintf into our buffer.
// 			int needed = vsnprintf(buf, size, fmt, ap);
// 			// NB. C99 (which modern Linux and OS X follow) says vsnprintf
// 			// failure returns the length it would have needed.  But older
// 			// glibc and current Windows return -1 for failure, i.e., not
// 			// telling us how much was needed.
// 
// 			if (needed <= (int)size && needed >= 0) {
// 				// It fit fine so we're done.
// 				s = std::std::string(buf, (size_t)needed);
// 				return;
// 			}
// 
// 			// vsnprintf reported that it wanted to write more characters
// 			// than we allotted.  So try again using a dynamic buffer.  This
// 			// doesn't happen very often if we chose our initial size well.
// 			size = (needed > 0) ? (needed+1) : (size*2);
// 			dynamicbuf.resize(size);
// 			buf = &dynamicbuf[0];
// 		}
// 	}
// 
// #ifdef _YAO_LOG_WIN32_
// 	void StrUtil::vFormatW(std::wstring& s, const wchar_t *fmt, va_list ap)
// 	{
// 		// Allocate a buffer on the stack that's big enough for us almost
// 		// all the time.  Be prepared to allocate dynamically if it doesn't fit.
// 		size_t size = 1024;
// 		wchar_t stackbuf[1024] = { 0 };
// 		std::vector<wchar_t> dynamicbuf;
// 		wchar_t *buf = &stackbuf[0];
// 
// 		s.clear();
// 
// 		while (1) {
// 			// Try to vsnprintf into our buffer.
// 			int needed = _vsnwprintf(buf, size, fmt, ap);
// 			// NB. C99 (which modern Linux and OS X follow) says vsnprintf
// 			// failure returns the length it would have needed.  But older
// 			// glibc and current Windows return -1 for failure, i.e., not
// 			// telling us how much was needed.
// 
// 			if (needed <= (int)size && needed >= 0) {
// 				// It fit fine so we're done.
// 				s = std::wstring(buf, (size_t)needed);
// 				return;
// 			}
// 
// 			// vsnprintf reported that it wanted to write more characters
// 			// than we allotted.  So try again using a dynamic buffer.  This
// 			// doesn't happen very often if we chose our initial size well.
// 			size = (needed > 0) ? (needed+1) : (size*2);
// 			dynamicbuf.resize(size);
// 			buf = &dynamicbuf[0];
// 		}
// 	}
// #endif
// 
// #ifdef _YAO_LOG_WIN32_
// 	std::std::string StrUtil::WStrToStr(const std::wstring& strIn)
// 	{
// 		int nBufSize = ::WideCharToMultiByte(GetACP(), 0, strIn.c_str(), -1, NULL, 0, 0, FALSE);
// 
// 		char* szBuf = new char[nBufSize];
// 		if (!szBuf) return "";
// 
// 		::WideCharToMultiByte(GetACP(), 0, strIn.c_str(), -1, szBuf, nBufSize, 0, FALSE);
// 
// 		std::std::string strRet(szBuf);
// 
// 		delete [] szBuf;
// 		szBuf = NULL;
// 
// 		return strRet;
// 	}
// #endif
// 
// #ifdef _YAO_LOG_WIN32_
// 	std::wstring StrUtil::StrToWStr(const std::std::string& strIn)
// 	{
// 		int nBufSize = ::MultiByteToWideChar(GetACP(), 0, strIn.c_str(), -1, NULL, 0);
// 
// 		wchar_t* wsBuf = new wchar_t[nBufSize];
// 		if (!wsBuf) return L"";
// 
// 		::MultiByteToWideChar(GetACP(), 0, strIn.c_str(), -1, wsBuf, nBufSize);
// 
// 		std::wstring wstrRet(wsBuf);
// 
// 		delete [] wsBuf;
// 		wsBuf = NULL;
// 
// 		return wstrRet;
// 	}
// #endif
// 
// 	bool StrUtil::CompareNoCase(const std::std::string& strIn1, const std::std::string& strIn2)
// 	{
// 		std::std::string s1(strIn1);
// 		std::std::string s2(strIn2);
// 		transform(s1.begin(), s1.end(), s1.begin(), ::tolower);
// 		transform(s2.begin(), s2.end(), s2.begin(), ::tolower);
// 
// 		return s1 == s2;
// 	}
// 
// 	std::std::string StrUtil::GetSysTimeStr(bool withMillisecond)
// 	{
// 		timeb timebuffer;
// 		ftime(&timebuffer);
// 		tm *local;
// 		local = localtime(&timebuffer.time);
// 
// 		char buf[30] = { 0 };
// 
// 		if (withMillisecond)
// 		{
// 			sprintf(buf, "%04u-%02u-%02u %02u:%02u:%02u.%03u",
// 				local->tm_year+1900, local->tm_mon+1, local->tm_mday,
// 				local->tm_hour, local->tm_min, local->tm_sec, timebuffer.millitm);
// 		}
// 		else
// 		{
// 			sprintf(buf, "%04u-%02u-%02u %02u:%02u:%02u",
// 				local->tm_year+1900, local->tm_mon+1, local->tm_mday,
// 				local->tm_hour, local->tm_min, local->tm_sec);
// 		}
// 
// 		return std::std::string(buf);
// 	}
// 
// 	std::std::string StrUtil::GetSysTimeStrForFileName(bool isBinary)
// 	{
// 		timeb timebuffer;
// 		ftime(&timebuffer);
// 		tm *local;
// 		local = localtime(&timebuffer.time);
// 
// 		char buf[30] = { 0 };
// 
// 		sprintf(buf, "_%04u%02u%02u_%02u%02u%02u",
// 			local->tm_year+1900, local->tm_mon+1, local->tm_mday,
// 			local->tm_hour, local->tm_min, local->tm_sec);
// 
// 		std::std::string str(buf);
// 		if (isBinary) return str + ".bl";
// 		else return str + ".log";
// 	}
// 
// 	int StrUtil::SplitString(const std::std::string& strIn,
// 		const std::std::string& strDelimiter,
// 		std::vector<std::std::string>& ret,
// 		bool retWithEmpty)
// 	{
// 		ret.clear();
// 
// 		size_t iPos = 0;
// 		size_t newPos = std::std::string::npos;
// 		size_t delimiterLength = strDelimiter.size();
// 		size_t strInLength = strIn.size();
// 
// 		if (delimiterLength == 0 || strInLength == 0)
// 			return 0;
// 
// 		std::vector<size_t> positions;
// 
// 		newPos = strIn.find(strDelimiter, 0);
// 
// 		if (newPos == std::std::string::npos)
// 		{
// 			ret.push_back(strIn);
// 			return 1;
// 		}
// 
// 		int numFound = 0;
// 
// 		while (newPos != std::std::string::npos && newPos >= iPos)
// 		{
// 			numFound++;
// 			positions.push_back(newPos);
// 			iPos = newPos;
// 			newPos = strIn.find(strDelimiter, iPos + delimiterLength);
// 		}
// 
// 		for (size_t i = 0; i <= positions.size(); ++i)
// 		{
// 			std::std::string s;
// 			if (i == 0) 
// 			{ 
// 				s = strIn.substr(i, positions[i]);
// 			}
// 			else
// 			{
// 				size_t offset = positions[i-1] + delimiterLength;
// 				if (offset < strInLength)
// 				{
// 					if (i == positions.size())
// 					{
// 						s = strIn.substr(offset);
// 					}
// 					else
// 					{
// 						s = strIn.substr(offset, positions[i] - positions[i-1] - delimiterLength);
// 					}
// 				}
// 			}
// 
// 			if (retWithEmpty)
// 			{
// 				ret.push_back(s);
// 			}
// 			else if (s.size() > 0)
// 			{
// 				ret.push_back(s);
// 			}
// 		}
// 
// 		return numFound;
// 	}
// 
// 	size_t StrUtil::IndexOfFirst(const std::std::string& strIn, const std::std::string& strMatch)
// 	{
// 		return strIn.find(strMatch, 0);
// 	}
// 	size_t StrUtil::IndexOfLast(const std::std::string& strIn, const std::std::string& strMatch)
// 	{
// 		return strIn.rfind(strMatch, strIn.size() - 1);
// 	}
// 
// 	bool StrUtil::ParseUrl(const std::std::string& strUrl,
// 		std::std::string& strServer,
// 		std::std::string& strPath,
// 		int& nPort)
// 	{
// 		nPort = 80;
// 
// 		if (strUrl.size() < 4)
// 		{
// 			return false;
// 		}
// 
// 		std::std::string url = strUrl;
// 
// 		// remove protocol
// 		size_t n = IndexOfFirst(url, "://");
// 		if (n != std::std::string::npos)
// 		{
// 			if ((int)url.size() > n + 3)
// 			{
// 				url = url.substr(n + 3);
// 			}
// 			else
// 			{
// 				return false;
// 			}
// 		}
// 
// 		// parse server and path
// 		size_t n2 = IndexOfFirst(url, "/");
// 		if (n2 == std::std::string::npos)
// 		{
// 			strServer = url;
// 			strPath = "";
// 		}
// 		else if (n2 == 0)
// 		{
// 			return false;
// 		}
// 		else
// 		{
// 			if ((int)url.size() > n2 + 1)
// 			{
// 				strServer = url.substr(0, n2);
// 				strPath = url.substr(n2);
// 			}
// 			else
// 			{
// 				// url.size() == n2 + 1
// 				strServer = url.substr(0, n2);
// 				strPath = "";
// 			}
// 		}
// 
// 		// parse port
// 		size_t nColon = IndexOfLast(strServer, ":");
// 		if (nColon != std::std::string::npos)
// 		{
// 			if ((int)strServer.size() > nColon + 1)
// 			{
// 				nPort = atoi(strServer.substr(nColon + 1).c_str());
// 				if (nPort == 0) return false;
// 				strServer = strServer.substr(0, nColon);
// 			}
// 			else
// 			{
// 				return false;
// 			}
// 		}
// 
// 		return true;
// 	}
// 
// 	std::std::string PathUtil::ModuleFileDirectory_()
// 	{
// #ifdef _YAO_LOG_WIN32_
// 		char buf[MAX_PATH] = { 0 };
// 		::GetModuleFileNameA(NULL, buf, MAX_PATH);
// 		::PathRemoveFileSpecA(buf);
// 		return std::std::string(buf);
// #else
// 		char buf[260] = { 0 };
// 		readlink("/proc/self/exe", buf, 260);
// 		return GetDirectory_(buf);
// #endif
// 	}
// 
// 	std::std::string PathUtil::GetDirectory_(const std::std::string& fileFullPath)
// 	{
// #ifdef _YAO_LOG_WIN32_
// 		return std::std::string(fileFullPath, 0, fileFullPath.rfind("\\"));
// #else
// 		assert(fileFullPath.size() > 1);
// 		if (fileFullPath.find('/', 1) == std::std::string::npos)
// 			return std::std::string("/");
// 		else
// 			return std::std::string(fileFullPath, 0, fileFullPath.rfind("/"));
// #endif
// 	}
// 
// 	std::std::string PathUtil::GetFile_(const std::std::string& fileFullPath)
// 	{
// #ifdef _YAO_LOG_WIN32_
// 		return std::std::string(fileFullPath, fileFullPath.rfind("\\")+1);
// #else
// 		return std::std::string(fileFullPath, fileFullPath.rfind("/")+1);
// #endif
// 	}
// 
// 	std::std::string PathUtil::PathCombine_(const std::std::string& path1, const std::std::string& path2)
// 	{
// #ifdef _YAO_LOG_WIN32_
// 		char buf[MAX_PATH] = { 0 };
// 		::PathCombineA(buf, path1.c_str(), path2.c_str());
// 		return std::std::string(buf);
// #else
// 		assert(path2[0] != '/');
// 		std::std::string path_(path1);
// 		if (path_[path_.size() - 1] != '/')
// 			path_ += std::std::string("/");
// 		return path_ + path2;
// #endif
// 	}
// 
// 	bool PathUtil::PathFileExists_(const std::std::string& path_)
// 	{
// #ifdef _YAO_LOG_WIN32_
// 		return ::PathFileExistsA(path_.c_str()) ? true : false;
// #else
// 		struct stat sts;
// 		if ((stat(path_.c_str(), &sts)) == -1 && errno == ENOENT)
// 			return false;
// 		return true;
// #endif
// 	}
// 
// 	bool PathUtil::IsStrJustFileName_(const std::std::string& str)
// 	{
// 		return !(
// 			(str.find(':', 0) != std::std::string::npos)  ||
// 			(str.find('\\', 0) != std::std::string::npos) ||
// 			(str.find('/', 0) != std::std::string::npos));
// 	}
// 
// 	void PathUtil::CreateFolder_(const std::std::string& directory_)
// 	{
// 		if (!PathUtil::PathFileExists_(directory_))
// 		{
// #ifdef _YAO_LOG_WIN32_
// 			::SHCreateDirectoryExA(NULL, directory_.c_str(), NULL);
// #else
// 			std::std::string s("mkdir -p " + directory_);
// 			system(s.c_str());
// #endif
// 		}
// 	}
// 
// 	int IniReader::LoadIniFile(const char *file, char *buf, int *file_size)
// 	{
// 		FILE *in = NULL;
// 		int i = 0;
// 		*file_size = 0;
// 
// 		assert(file != NULL);
// 		assert(buf != NULL);
// 
// 		in = fopen(file,"r");
// 		if (NULL == in) {
// 			return 0;
// 		}
// 
// 		buf[i] = fgetc(in);
// 
// 		//load initialization file
// 		while (buf[i] != (char)EOF) {
// 			i++;
// 			assert(i < MAX_INI_FILE_SIZE); //redefine MAX_INI_FILE_SIZE to fit the big file
// 			buf[i] = fgetc(in);
// 		}
// 
// 		buf[i] = '\0';
// 		*file_size = i;
// 
// 		fclose(in);
// 		return 1;
// 	}
// 
// 	int IniReader::NewLine(char c)
// 	{
// 		return ('\n' == c || '\r' == c) ? 1 : 0;
// 	}
// 
// 	int IniReader::EndOfStr(char c)
// 	{
// 		return '\0' == c ? 1 : 0;
// 	}
// 
// 	int IniReader::LeftBrace(char c)
// 	{
// 		return '[' == c ? 1 : 0;
// 	}
// 
// 	int IniReader::RightBrace(char c)
// 	{
// 		return ']' == c ? 1 : 0;
// 	}
// 
// 	int IniReader::ParseFile(const char *section, const char *key_,
// 		const char *buf, int *sec_s, int *sec_e,
// 		int *key_s, int *key_e, int *value_s, int *value_e)
// 	{
// 		const char *p = buf;
// 		int i = 0;
// 
// 		assert(buf != NULL);
// 		assert(section != NULL && strlen(section));
// 		assert(key_ != NULL && strlen(key_));
// 
// 		*sec_e = *sec_s = *key_e = *key_s = *value_s = *value_e = -1;
// 
// 		while (!EndOfStr(p[i])) {
// 			//find the section
// 			if (( 0==i || NewLine(p[i-1])) && LeftBrace(p[i]))
// 			{
// 				int section_start = i + 1;
// 
// 				//find the ']'
// 				do {
// 					i++;
// 				} while (!RightBrace(p[i]) && !EndOfStr(p[i]));
// 
// 
// 				if (i - section_start == (int)strlen(section) && 0 == strncmp(p + section_start, section, strlen(section))) {
// 					int newline_start = 0;
// 
// 					i++;
// 
// 					//Skip over space char after ']'
// 					while (isspace(p[i])) {
// 						i++;
// 					}
// 
// 					//find the section
// 					*sec_s = section_start;
// 					*sec_e = i;
// 
// 					while (!(NewLine(p[i-1]) && LeftBrace(p[i])) && !EndOfStr(p[i])) {
// 						int j = 0;
// 						//get a new line
// 						newline_start = i;
// 
// 						while (!NewLine(p[i]) && !EndOfStr(p[i])) {
// 							i++;
// 						}
// 
// 						//now i is equal to end of the line
// 						j = newline_start;
// 						int valid = j;
// 
// 						if (';' != p[j]) //skip over comment
// 						{
// 							while(j < i && p[j] != '=') {
// 								j++;
// 
// 								if (' ' != p[j] && '\t' != p[j] && '=' != p[j])
// 									valid = j;
// 								if ('=' == p[j]) {
// 									if (valid - newline_start + 1 == (int)strlen(key_) && strncmp(key_, p + newline_start, strlen(key_)) == 0)
// 									{
// 										//find the key ok
// 										*key_s = newline_start;
// 										*key_e = j - 1;
// 
// 										valid = j + 1;
// 										while (' ' == p[valid] || '\t' == p[valid])
// 											valid++;
// 										*value_s = valid;
// 										*value_e = i;
// 
// 										return 1;
// 									}
// 								}
// 							}
// 						}
// 
// 						i++;
// 					}
// 				}
// 			}
// 			else
// 			{
// 				i++;
// 			}
// 		}
// 		return 0;
// 	}
// 
// 	/**
// 	*@brief read std::string in initialization file
// 	* retrieves a std::string from the specified section in an initialization file
// 	*@param section [in] name of the section containing the key name
// 	*@param key_ [in] name of the key pairs to value
// 	*@param value_ [in&out] pointer to the buffer that receives the retrieved std::string
// 	*@param size_ [in] size of result's buffer
// 	*@param defaultValue [in] default value of result
// 	*@param file [in] path of the initialization file
// 	*@return 1 : read success; return 0 : read fail
// 	*/
// 	int IniReader::ReadStr_(const char *section, const char *key_, char *value_,
// 		int size_, const char *defaultValue, const char *file)
// 	{
// 		char *buf = new char[MAX_INI_FILE_SIZE];
// 		if (!buf) return 0;
// 
// 		memset(buf, 0, MAX_INI_FILE_SIZE);
// 
// 		int file_size;
// 		int sec_s, sec_e, key_s, key_e, value_s, value_e;
// 
// 		//check parameters
// 		assert(section != NULL && strlen(section));
// 		assert(key_ != NULL && strlen(key_));
// 		assert(value_ != NULL);
// 		assert(size_ > 0);
// 		assert(file != NULL && strlen(key_));
// 
// 		if (!LoadIniFile(file, buf, &file_size))
// 		{
// 			if (defaultValue != NULL)
// 			{
// 				strncpy(value_, defaultValue, size_);
// 			}
// 			delete []buf;
// 			return 0;
// 		}
// 
// 		if (!ParseFile(section, key_, buf, &sec_s, &sec_e, &key_s, &key_e, &value_s, &value_e))
// 		{
// 			if (defaultValue != NULL)
// 			{
// 				strncpy(value_, defaultValue, size_);
// 			}
// 			delete []buf;
// 			return 0; //not find the key
// 		}
// 		else
// 		{
// 			int cpcount = value_e - value_s;
// 
// 			if (size_ - 1 < cpcount)
// 			{
// 				cpcount =  size_ - 1;
// 			}
// 
// 			memset(value_, 0, size_);
// 			memcpy(value_, buf + value_s, cpcount);
// 			value_[cpcount] = '\0';
// 
// 			delete []buf;
// 			return 1;
// 		}
// 	}
// 
// 	bool IniReader::ReadStrA(const std::std::string& section, const std::std::string& key_,
// 		std::std::string& value_, const std::std::string& defaultValue,
// 		const std::std::string& filePath)
// 	{
// 		char buf[4096] = { 0 };
// 		int success = ReadStr_(section.c_str(), key_.c_str(), buf, sizeof(buf),
// 			defaultValue.c_str(), filePath.c_str());
// 
// 		if (success) value_ = std::std::string(buf);
// 		else value_ = defaultValue;
// 
// 		return success == 1 ? true : false;
// 	}
// 
// 	int IniReader::ReadIntA(const std::std::string& section, const std::std::string& key_,
// 		int defaultValue, std::std::string& filePath)
// 	{
// 		std::std::string value_;
// 		std::std::string strDefaultValue;
// 		if (ReadStrA(section, key_, value_, strDefaultValue, filePath))
// 		{
// 			return atoi(value_.c_str());
// 		}
// 		else
// 		{
// 			return defaultValue;
// 		}
// 	}
// 
// 	std::std::string MachineID::GetMachineID()
// 	{
// 		std::std::string s;
// 		unsigned char result[6] = { 0 };
// #ifdef _YAO_LOG_WIN32_
// 		if (GetMACAddressMSW(result) == 0)
// #else
// 		if (GetMACAddressLinux(result) == 0)
// #endif
// 		{
// 			char buf[18] = { 0 };
// 			sprintf(buf, "%02X:%02X:%02X:%02X:%02X:%02X",
// 				(unsigned int)result[0], (unsigned int)result[1], (unsigned int)result[2],
// 				(unsigned int)result[3], (unsigned int)result[4], (unsigned int)result[5]);
// 			s = std::std::string(buf);
// 		}
// 		return s;
// 	}
// 
// #ifdef _YAO_LOG_WIN32_
// 	long MachineID::GetMACAddressMSW(unsigned char * result)
// 	{
// 		UUID uuid;
// 		if(UuidCreateSequential(&uuid) == RPC_S_UUID_NO_ADDRESS) return -1;
// 		memcpy(result, (char*)(uuid.Data4+2), 6);
// 		return 0;
// 	}
// #else
// 	long MachineID::GetMACAddressLinux(unsigned char * result)
// 	{
// 		struct ifreq ifr;
// 		struct ifreq *IFR;
// 		struct ifconf ifc;
// 		char buf[1024];
// 		int s, i;
// 		int ok = 0;
// 
// 		s = socket(AF_INET, SOCK_DGRAM, 0);
// 		if (s == -1)
// 		{
// 			return -1;
// 		}
// 
// 		ifc.ifc_len = sizeof(buf);
// 		ifc.ifc_buf = buf;
// 		ioctl(s, SIOCGIFCONF, &ifc);
// 
// 		IFR = ifc.ifc_req;
// 		for (i = ifc.ifc_len / sizeof(struct ifreq); --i >= 0; IFR++)
// 		{
// 			strcpy(ifr.ifr_name, IFR->ifr_name);
// 			if (ioctl(s, SIOCGIFFLAGS, &ifr) == 0)
// 			{
// 				if (! (ifr.ifr_flags & IFF_LOOPBACK))
// 				{
// 					if (ioctl(s, SIOCGIFHWADDR, &ifr) == 0)
// 					{
// 						ok = 1;
// 						break;
// 					}
// 				}
// 			}
// 		}
// 
// 		shutdown(s, SHUT_RDWR);
// 		if (ok)
// 		{
// 			bcopy(ifr.ifr_hwaddr.sa_data, result, 6);
// 		}
// 		else
// 		{
// 			return -1;
// 		}
// 		return 0;
// 	}
// #endif
// 
// 	P_FILE FileUtil::Open(const std::std::string& path_, bool overwrite_)
// 	{
// 		P_FILE pFile = NULL;
// 
// #ifdef _YAO_LOG_WIN32_
// 		pFile = CreateFileA(
// 			path_.c_str(),     
// 			GENERIC_READ | GENERIC_WRITE,   
// 			FILE_SHARE_READ,
// 			NULL,   
// 			overwrite_ ? CREATE_ALWAYS : OPEN_ALWAYS,
// 			FILE_ATTRIBUTE_NORMAL,
// 			NULL);
// 		SetFilePointer(pFile, 0, NULL, FILE_END);
// #else
// 		pFile = fopen(path_.c_str(), overwrite_ ? "wb" : "ab");
// #endif
// 
// 		return pFile;
// 	}
// 
// 	void FileUtil::Write(P_FILE pFile, const char *pBuf, unsigned int bufSize)
// 	{
// #ifdef _YAO_LOG_WIN32_
// 		DWORD dwBytesWritten = 0;
// 		WriteFile(pFile, pBuf, bufSize, &dwBytesWritten, NULL);
// 		FlushFileBuffers(pFile);
// #else
// 		fwrite(pBuf, bufSize, 1, pFile);
// 		fflush(pFile);
// #endif
// 	}
// 
// 	void FileUtil::Close(P_FILE pFile)
// 	{
// #ifdef _YAO_LOG_WIN32_
// 		CloseHandle(pFile);
// #else
// 		fclose(pFile);
// #endif
// 	}
// 
// 	bool FileUtil::Valid(P_FILE pFile)
// 	{
// #ifdef _YAO_LOG_WIN32_
// 		return (pFile != NULL) && (pFile != INVALID_HANDLE_VALUE);
// #else
// 		return (pFile != NULL);
// #endif
// 	}
// 
// 	const std::std::string Encoding::base64_chars = 
// 		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
// 		"abcdefghijklmnopqrstuvwxyz"
// 		"0123456789+/";
// 
// 	bool Encoding::IsBase64(unsigned char c) 
// 	{
// 		return (isalnum(c) || (c == '+') || (c == '/'));
// 	}
// 
// 	std::std::string Encoding::Base64Encode(const unsigned char *bytes_to_encode, unsigned int in_len)
// 	{
// 		std::std::string ret;
// 		int i = 0;
// 		int j = 0;
// 		unsigned char char_array_3[3];
// 		unsigned char char_array_4[4];
// 
// 		while (in_len--)
// 		{
// 			char_array_3[i++] = *(bytes_to_encode++);
// 			if (i == 3)
// 			{
// 				char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
// 				char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
// 				char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
// 				char_array_4[3] = char_array_3[2] & 0x3f;
// 
// 				for (i = 0; (i < 4) ; i++)
// 					ret += base64_chars[char_array_4[i]];
// 				i = 0;
// 			}
// 		}
// 
// 		if (i)
// 		{
// 			for (j = i; j < 3; j++)
// 				char_array_3[j] = '\0';
// 
// 			char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
// 			char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
// 			char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
// 			char_array_4[3] = char_array_3[2] & 0x3f;
// 
// 			for (j = 0; (j < i + 1); j++)
// 				ret += base64_chars[char_array_4[j]];
// 
// 			while ((i++ < 3))
// 				ret += '=';
// 
// 		}
// 
// 		return ret;
// 
// 	}
// 
// 	std::std::string Encoding::Base64Decode(std::std::string const& encoded_string)
// 	{
// 		int in_len = encoded_string.size();
// 		int i = 0;
// 		int j = 0;
// 		int in_ = 0;
// 		unsigned char char_array_4[4], char_array_3[3];
// 		std::std::string ret;
// 
// 		while (in_len-- && ( encoded_string[in_] != '=') && IsBase64(encoded_string[in_]))
// 		{
// 			char_array_4[i++] = encoded_string[in_]; in_++;
// 			if (i == 4)
// 			{
// 				for (i = 0; i < 4; i++)
// 					char_array_4[i] = base64_chars.find(char_array_4[i]);
// 
// 				char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
// 				char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
// 				char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];
// 
// 				for (i = 0; (i < 3); i++)
// 					ret += char_array_3[i];
// 				i = 0;
// 			}
// 		}
// 
// 		if (i)
// 		{
// 			for (j = i; j < 4; j++)
// 				char_array_4[j] = 0;
// 
// 			for (j = 0; j < 4; j++)
// 				char_array_4[j] = base64_chars.find(char_array_4[j]);
// 
// 			char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
// 			char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
// 			char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];
// 
// 			for (j = 0; (j < i - 1); j++) ret += char_array_3[j];
// 		}
// 
// 		return ret;
// 	}
// 
// 	unsigned char Encoding::ToHex(unsigned char x) 
// 	{ 
// 		return  x > 9 ? x + 55 : x + 48; 
// 	}
// 
// 	unsigned char Encoding::FromHex(unsigned char x) 
// 	{ 
// 		unsigned char y;
// 		if (x >= 'A' && x <= 'Z') y = x - 'A' + 10;
// 		else if (x >= 'a' && x <= 'z') y = x - 'a' + 10;
// 		else if (x >= '0' && x <= '9') y = x - '0';
// 		else assert(0);
// 		return y;
// 	}
// 
// 	std::std::string Encoding::UrlEncode(const std::std::string& str)
// 	{
// 		std::std::string strTemp = "";
// 		size_t length = str.length();
// 		for (size_t i = 0; i < length; i++)
// 		{
// 			if (isalnum((unsigned char)str[i]) || 
// 				(str[i] == '-') ||
// 				(str[i] == '_') || 
// 				(str[i] == '.') || 
// 				(str[i] == '~'))
// 				strTemp += str[i];
// 			else if (str[i] == ' ')
// 				strTemp += "+";
// 			else
// 			{
// 				strTemp += '%';
// 				strTemp += ToHex((unsigned char)str[i] >> 4);
// 				strTemp += ToHex((unsigned char)str[i] % 16);
// 			}
// 		}
// 		return strTemp;
// 	}
// 
// 	std::std::string Encoding::UrlDecode(const std::std::string& str)
// 	{
// 		std::std::string strTemp = "";
// 		size_t length = str.length();
// 		for (size_t i = 0; i < length; i++)
// 		{
// 			if (str[i] == '+') strTemp += ' ';
// 			else if (str[i] == '%')
// 			{
// 				assert(i + 2 < length);
// 				unsigned char hign = FromHex((unsigned char)str[++i]);
// 				unsigned char low = FromHex((unsigned char)str[++i]);
// 				strTemp += hign*16 + low;
// 			}
// 			else strTemp += str[i];
// 		}
// 		return strTemp;
// 	}
// 
// 	HttpConn::HttpConn(const std::std::string& host_, int port_) :
// 		m_host(host_),
// 		m_port(port_),
// 		m_sock(-1)
// 	{
// 	}
// 
// 	HttpConn::~HttpConn()
// 	{
// 		this->close();
// 	}
// 
// 	bool HttpConn::connect()
// 	{
// 		in_addr addr = host2addr(m_host.c_str());
// 		if (addr.s_addr == INADDR_NONE) return false;
// 
// 		sockaddr_in address;
// 		memset((char*)&address, 0, sizeof(address));
// 		address.sin_family = AF_INET;
// 		address.sin_port = htons(m_port);
// 		address.sin_addr.s_addr = addr.s_addr;
// 
// 		m_sock = socket(AF_INET, SOCK_STREAM, 0);
// 		if (m_sock < 0) return false;
// 
// 		if (::connect(m_sock, (const sockaddr*)&address, sizeof(address)) < 0)
// 			return false;
// 
// 		return true;
// 	}
// 
// 	void HttpConn::close()
// 	{
// #ifdef WIN32
// 		if (m_sock >= 0)
// 			::closesocket(m_sock);
// #else
// 		if (m_sock >= 0)
// 			::close(m_sock);
// #endif
// 		m_sock = -1;
// 	}
// 
// 	void HttpConn::putrequest(const char *method_, const char *url_)
// 	{
// 		char req[512] = { 0 };
// 		sprintf(req, "%s %s HTTP/1.1", method_, url_);
// 		m_buffer.push_back(req);
// 
// 		putheader("Host", m_host.c_str());    // required for HTTP1.1
// 		putheader("Accept-Encoding", "identity");
// 	}
// 
// 	void HttpConn::putheader(const std::std::string& header_, const std::std::string& value_)
// 	{
// 		m_buffer.push_back(header_ + ": " + value_);
// 	}
// 
// 	void HttpConn::putheader(const std::std::string& header_, int numericvalue)
// 	{
// 		char buf[32] = { 0 };
// 		sprintf(buf, "%d", numericvalue);
// 		putheader(header_, buf);
// 	}
// 
// 	void HttpConn::endheaders()
// 	{
// 		m_buffer.push_back("");
// 
// 		std::std::string msg;
// 		std::vector<std::std::string>::const_iterator it;
// 		for (it = m_buffer.begin(); it != m_buffer.end(); ++it)
// 			msg += (*it) + "\r\n";
// 
// 		m_buffer.clear();
// 
// 		this->send((const unsigned char*)msg.c_str(), msg.size());
// 	}
// 
// 	void HttpConn::send(const unsigned char *buf, int numbytes)
// 	{
// 		if (m_sock < 0)
// 		{
// 			if (!this->connect()) return;
// 		}
// 
// 		while (numbytes > 0)
// 		{
// #ifdef WIN32
// 			int n = ::send(m_sock, (const char*)buf, numbytes, 0);
// #else
// 			int n = ::send(m_sock, buf, numbytes, 0);
// #endif
// 			numbytes -= n;
// 			buf += n;
// 		}
// 	}
// 
// 	void HttpConn::rev()
// 	{
// 		if (m_sock < 0) return;
// 
// 		// maybe you need to improve it if the response buffer is larger than 2048...
// 		char buf[2048] = { 0 };
// 		int iResult = ::recv(m_sock, (char*)buf, sizeof(buf), 0);
// 		//printf("iResult=%d\n", iResult);
// 	}
// 
// 	struct in_addr HttpConn::host2addr(const char *host_name)
// 	{
// 		struct hostent *host;
// 		struct in_addr addr;
// 
// 		// First try nnn.nnn.nnn.nnn form
// 		addr.s_addr = inet_addr(host_name);
// 		if (addr.s_addr != INADDR_NONE)
// 			return addr;
// 
// 		host = gethostbyname(host_name);
// 		if (host)
// 			addr = *((in_addr*)*host->h_addr_list);
// 
// 		return addr;
// 	}
// 
// 	TinyThread::TinyThread()
// 	{
// #ifdef _YAO_LOG_WIN32_
// 		m_event = CreateEvent(NULL, FALSE, FALSE, NULL);
// #else
// 		pthread_mutex_init(&m_mutex, NULL);
// 		pthread_cond_init(&m_cond, NULL);
// #endif
// 	}
// 
// 	// notify thread to exit
// 	void TinyThread::SetSig(int timeout_millisecond)
// 	{
// #ifdef _YAO_LOG_WIN32_
// 		SetEvent(m_event);
// #else
// 		int timeout_ = timeout_millisecond >= 50 ? timeout_millisecond : 50;
// 		for (int i = 0; i < timeout_/50; i++)
// 		{
// 			if (pthread_mutex_trylock(&m_mutex) != 0)
// 			{
// 				// wait 50 milliseconds and try again...
// 				// printf("trylock failed...%d/%d time(s)\n", i+1, timeout_/50);
// 				struct timeval tv;
// 				tv.tv_sec = 0;
// 				tv.tv_usec= 1000*50;
// 				select(0, NULL, NULL, NULL ,&tv);
// 			}
// 			else
// 			{
// 				pthread_cond_signal(&m_cond);
// 				pthread_mutex_unlock(&m_mutex);
// 				break;
// 			}
// 		}
// #endif
// 	}
// 
// 	void TinyThread::Join(int timeout_millisecond)
// 	{
// #ifdef _YAO_LOG_WIN32_
// 		// for windows, wait until timeout
// 		DWORD ret = ::WaitForSingleObject(m_handle, timeout_millisecond);
// 		CloseHandle(m_handle);
// 		if (ret == WAIT_OBJECT_0)
// 		{
// 			//printf("the work thread has exited...\n");
// 		}
// 		else
// 		{
// 			//printf("the work thread timed out...\n");
// 		}
// #else
// 		// for linux, query status(we are already wait it in SetSig)
// 		// just for debug and do nothing actually...
// 		int ret = pthread_kill(m_handle, 0);
// 		if (ret == ESRCH)
// 		{
// 			//printf("the work thread has exited...\n");
// 		}
// 		else
// 		{
// 			// m_handle is alive, but...
// 			// because we're already detach the thread, so we cannot
// 			// know whether m_handle is the original thread or not
// 		}
// #endif
// 	}
// 
// 	bool TinyThread::Start(
// 		void (*pFn)(void *),
// 		void *pArg,
// 		int cycleSpanMilliseconds,
// 		bool *pThreadEnd)
// 	{
// 		ThreadStartInfo *pSI = new ThreadStartInfo;
// 		if (!pSI) return false;
// 
// 		pSI->pFn = pFn;
// 		pSI->pArg = pArg;
// 		pSI->cycleSpanMilliseconds = cycleSpanMilliseconds;
// 		pSI->pThreadEnd = pThreadEnd;
// 		pSI->pThread = this;
// 
// #ifdef _YAO_LOG_WIN32_
// 		m_handle = (HANDLE)_beginthreadex(0, 0, Run, (void*)pSI, 0, &m_threadID);
// 		if (m_handle == NULL) return false;
// 		return true;
// #else
// 		if (0 != pthread_create(&m_handle, NULL, Run, (void*)pSI)) return false;
// 		return true;
// #endif
// 	}
// 
// #ifdef _YAO_LOG_WIN32_
// 	unsigned WINAPI TinyThread::Run(void *pArg)
// 	{
// 		ThreadStartInfo *pSI = (ThreadStartInfo*)pArg;
// 		TinyThread *pThread = (TinyThread*)pSI->pThread;
// 		bool *pThreadEnd = pSI->pThreadEnd;
// 
// 		do 
// 		{
// 			pSI->pFn(pSI->pArg);
// 			::WaitForSingleObject(pThread->m_event, pSI->cycleSpanMilliseconds);
// 		}
// 		while (!(*pThreadEnd));
// 
// 		delete pSI;
// 
// 		return 0;
// 	}
// #else
// 	void *TinyThread::Run(void *pArg)
// 	{
// 		ThreadStartInfo *pSI = (ThreadStartInfo*)pArg;
// 		TinyThread *pThread = (TinyThread*)pSI->pThread;
// 		bool *pThreadEnd = pSI->pThreadEnd;
// 
// 		pthread_detach(pthread_self());
// 		struct timeval now_;
// 		struct timespec future_;
// 		pthread_mutex_lock(&pThread->m_mutex);
// 
// 		do 
// 		{
// 			pSI->pFn(pSI->pArg);
// 
// 			// usec -> microsecond
// 			// nsec -> nanosecond
// 			int sec = pSI->cycleSpanMilliseconds / 1000;
// 			int usec = (pSI->cycleSpanMilliseconds % 1000) * 1000;
// 			gettimeofday(&now_, NULL);
// 			int sec_ = now_.tv_sec + sec;
// 			int usec_ = now_.tv_usec + usec;
// 			if (usec_ > 1000000)
// 			{
// 				sec_++;
// 				usec_ -= 1000000;
// 			}
// 			future_.tv_sec = sec_;
// 			future_.tv_nsec = usec_ * 1000;
// 			pthread_cond_timedwait(&pThread->m_cond, &pThread->m_mutex, &future_);
// 		}
// 		while (!(*pThreadEnd));
// 
// 		pthread_mutex_unlock(&pThread->m_mutex);
// 
// 		delete pSI;
// 
// 		return 0;
// 	}
// #endif
// 
// 
// 
// 	///////////////////////////////////////////////////////////////
// 	// LogFactory
// 	///////////////////////////////////////////////////////////////
// 	TinyMutex LogFactory::m_tm;
// 	std::map<std::std::string, BaseLog*> LogFactory::m_allLoggers;
// 	bool LogFactory::m_threadEnd;
// 	TinyThread *LogFactory::m_pTT;
// 	bool LogFactory::m_inited;
// 	bool LogFactory::m_totallyDisableAllLog = false;
// 
// 	void LogFactory::BkWorker(void *pArg)
// 	{
// 		m_tm.Lock();
// 
// 		// update config from ini...
// 		std::map<std::std::string, BaseLog*>::iterator it;
// 		for (it = m_allLoggers.begin(); it != m_allLoggers.end(); it++)
// 		{
// 			BaseLog* pLog = it->second;
// 			if (pLog->m_iniFullPath.empty()) continue;
// 			LogParams lp;
// 			if (pLog->ParseConfigFile(lp))
// 			{
// 				if (lp == pLog->m_params)
// 				{
// 					continue;
// 				}
// 				else
// 				{
// 					pLog->m_params = lp;
// 					pLog->SetLogFilePath();
// 				}
// 			}
// 		}
// 
// 		// coyp log data
// 		std::vector<std::pair<std::std::string, std::vector<std::std::string> > > textData;
// 		std::vector<std::pair<std::std::string, std::vector<std::std::string> > > binData;
// 		Queue2Vector(g_textData, textData);
// 		Queue2Vector(g_binData, binData);
// 
// 		m_tm.Unlock();
// 
// 
// 		// post log data
// 		// may be slow, but never block the other threads
// 		for (size_t i = 0; i < textData.size(); i++)
// 		{
// 			PostData(textData[i].first, textData[i].second);
// 		}
// 		for (size_t i = 0; i < binData.size(); i++)
// 		{
// 			PostData(binData[i].first, binData[i].second);
// 		}
// 	}
// 
// 	void LogFactory::PostData(const std::std::string& logID,
// 		const std::vector<std::std::string>& logData)
// 	{
// 		BaseLog *p = Get(logID);
// 		if (!p) return;
// 
// 		std::std::string isText = p->m_bBinary ? "0" : "1";
// 
// 		std::std::string server_, path_;
// 		int port_;
// 		if (!StrUtil::ParseUrl(p->m_params.destUrl, server_, path_, port_))
// 			return;
// 
// 		static std::std::string machineID;
// 		if (machineID.empty())
// 			machineID = MachineID::GetMachineID();
// 
// 		HttpConn conn(server_, port_);
// 		for (size_t i = 0; i < logData.size(); i++)
// 		{
// 			//printf("%s\n", logData[i].data());
// 			std::std::string sData = Encoding::Base64Encode(
// 				(const unsigned char*)logData[i].data(),
// 				logData[i].size());
// 			sData = Encoding::UrlEncode(sData);
// 			sData = (
// 				"logID="+logID+
// 				"&isText="+isText+
// 				"&machineID="+machineID+
// 				"&logData="+sData);
// 
// 			conn.putrequest("POST", path_.c_str());
// 			conn.putheader("Connection", "keep-alive");
// 			conn.putheader("Content-Length", sData.size());
// 			conn.putheader("Content-type", "application/x-www-form-urlencoded");
// 			conn.putheader("Accept", "text/plain");
// 			conn.endheaders();
// 			conn.send((const unsigned char*)sData.data(), sData.size());
// 			conn.rev();
// 		}
// 	}
// 
// 	void LogFactory::Init()
// 	{
// 		m_tm.Lock();
// 
// 		if (m_inited) goto INIT_SUCCESS;
// 
// #ifdef _YAO_LOG_WIN32_
// 		WSAData wsaData;
// 		int code = WSAStartup(MAKEWORD(1, 1), &wsaData);
// 		if (code != 0) goto INIT_FAILED;
// #endif
// 
// 		g_pData = new char[BUF_SIZE];
// 		if (!g_pData) goto INIT_FAILED;
// 		else memset(g_pData, 0, BUF_SIZE);
// 
// 		m_threadEnd = false;
// 		m_pTT = new TinyThread(); 
// 		if (!m_pTT) goto INIT_FAILED;
// 
// 		if (!m_pTT->Start(BkWorker, 0, THREAD_CYCLE_SPAN_MILLISECOND, &m_threadEnd))
// 			goto INIT_FAILED;
// 
// INIT_SUCCESS:
// 		m_inited = true;
// 		m_tm.Unlock();
// 		return;
// 
// INIT_FAILED:
// 		m_inited = false;
// 		m_tm.Unlock();
// 	}
// 
// 	void LogFactory::Exit()
// 	{
// 		if (!m_inited) return;
// 
// 		m_tm.Lock();
// 
// 		if (g_pData)
// 		{
// 			delete [] g_pData;
// 			g_pData = NULL;
// 		}
// 
// 		// delete all loggers
// 		std::map<std::std::string, BaseLog*>::iterator it;
// 		for (it = m_allLoggers.begin(); it != m_allLoggers.end(); it++)
// 		{
// 			delete it->second;
// 		}
// 		m_allLoggers.clear();
// 
// 		// close all log files
// 		std::map<std::std::string, P_FILE>::iterator it1;
// 		for (it1 = g_files.begin(); it1 != g_files.end(); it1++)
// 		{
// 			if (FileUtil::Valid(it1->second))
// 				FileUtil::Close(it1->second);
// 		}
// 		g_files.clear();
// 
// 		m_tm.Unlock();
// 
// 		// notify the worker thread to exit...
// 		if (!m_threadEnd)
// 		{
// 			m_threadEnd = true;
// 			m_pTT->SetSig(THREAD_EXIT_TIMEOUT_MILLISECOND);
// 			m_pTT->Join(THREAD_EXIT_TIMEOUT_MILLISECOND);
// 			delete m_pTT;
// 			m_pTT = NULL;
// 		}
// 
// #ifdef _YAO_LOG_WIN32_
// 		WSACleanup();
// #endif
// 	}
// 
// 	bool LogFactory::Create(const std::std::string& logID, bool bEnable, bool bTextLog)
// 	{
// 		m_tm.Lock();
// 
// 		BaseLog *p = NULL;
// 
// 		if (!m_inited) goto CREATE_FAILED;
// 		if (logID.size() == 0) goto CREATE_FAILED;
// 		if (Exists(logID)) goto CREATE_FAILED;
// 
// 		if (bTextLog)
// 			p = new YaoLog(logID, bEnable);
// 		else
// 			p = new YaoBinLog(logID, bEnable);
// 
// 		if (p == NULL)
// 		{
// 			goto CREATE_FAILED;
// 		}
// 		else
// 		{
// 			m_allLoggers.insert(make_pair(logID, p));
// 		}
// 
// 		m_tm.Unlock();
// 		return true;
// 
// CREATE_FAILED:
// 		m_tm.Unlock();
// 		return false;
// 	}
// 
// 	bool LogFactory::Exists(const std::std::string& logID)
// 	{
// 		return m_allLoggers.find(logID) != m_allLoggers.end();
// 	}
// 
// 	BaseLog *LogFactory::Get(const std::std::string& logID)
// 	{
// 		std::map<std::std::string, BaseLog*>::const_iterator it = m_allLoggers.find(logID);
// 		if (it != m_allLoggers.end()) return it->second;
// 		return NULL;
// 	}
// 
// 	void LogFactory::SetAttr(
// 		const std::std::string& logID,
// 		bool bEnable,
// 		int nOutFlag,
// 		bool bWithTime,
// 		bool bWithMillisecond,
// 		bool bWithSrcFile,
// 		bool bWithFunction,
// 		bool bOverwrite,
// 		const std::std::string& logFileDir,
// 		const std::std::string& logFileName,
// 		const std::std::string& destUrl)
// 	{
// 		m_tm.Lock();
// 
// 		BaseLog *p = Get(logID);
// 		if (p == NULL)
// 		{
// 			m_tm.Unlock();
// 			return;
// 		}
// 
// 		p->SetAttr(bEnable, nOutFlag, bWithTime, bWithMillisecond,
// 			bWithSrcFile, bWithFunction,  bOverwrite,
// 			logFileDir, logFileName, destUrl);
// 
// 		m_tm.Unlock();
// 	}
// 
// 	void LogFactory::SetAttrFromConfigFile(const std::std::string& logID,
// 		const std::std::string& ini_)
// 	{
// 		m_tm.Lock();
// 
// 		BaseLog *p = Get(logID);
// 		if (p == NULL)
// 		{
// 			m_tm.Unlock();
// 			return;
// 		}
// 
// 		p->SetAttrFromConfigFile(ini_);
// 
// 		m_tm.Unlock();
// 	}
// 
// 
// 
// 	BaseLog::BaseLog(const std::std::string& logID, bool bEnable)
// 	{
// 		assert(logID.size() != 0);
// 
// 		m_logID = logID;
// 		m_params.bEnable = bEnable;
// 		m_bBinary = false;
// 	}
// 
// 	BaseLog::~BaseLog()
// 	{
// 	}
// 
// 	void BaseLog::SetAttr(
// 		bool bEnable,
// 		int nOutFlag,
// 		bool bWithTime,
// 		bool bWithMillisecond,
// 		bool bWithSrcFile,
// 		bool bWithFunction,
// 		bool bOverwrite,
// 		const std::std::string& logFileDir,
// 		const std::std::string& logFileName,
// 		const std::std::string& destUrl)
// 	{
// 		// doesn't use ini...
// 		m_iniFullPath.clear();
// 
// 		m_params.bEnable = bEnable;
// 		m_params.nOutFlag = nOutFlag;
// 		m_params.bWithTime = bWithTime;
// 		m_params.bWithMillisecond = bWithMillisecond;
// 		m_params.bWithSrcFile = bWithSrcFile;
// 		m_params.bWithFunction = bWithFunction;
// 		m_params.bOverwrite = bOverwrite;
// 		m_params.logFileDir = logFileDir;
// 		m_params.logFileName = logFileName;
// 		m_params.destUrl = destUrl;
// 
// 		SetLogFilePath();
// 	}
// 
// 	void BaseLog::SetAttrFromConfigFile(const std::std::string& ini_)
// 	{
// 		assert(ini_.size() != 0);
// 
// 		m_iniFullPath = ini_;
// 
// 		// get ini file full path
// 		if (PathUtil::IsStrJustFileName_(m_iniFullPath))
// 		{
// 			std::std::string moduleFileDirectory = PathUtil::ModuleFileDirectory_();
// 			m_iniFullPath = PathUtil::PathCombine_(moduleFileDirectory, m_iniFullPath);
// 		}
// 
// 		if (ParseConfigFile(m_params))
// 		{
// 			SetLogFilePath();
// 		}
// 		else
// 		{
// 			// ini file doesn't exist...
// 			m_iniFullPath.clear();
// 			m_params.Reset();
// 		}
// 	}
// 
// 	bool BaseLog::ParseConfigFile(LogParams& lp)
// 	{
// 		if (PathUtil::PathFileExists_(m_iniFullPath))
// 		{
// 			// enable log?
// 			lp.bEnable = (
// 				IniReader::ReadIntA(m_logID, "Enable", 0, m_iniFullPath)
// 				!= 0);
// 
// 			// out flag
// 			std::std::string outFlag;
// 			IniReader::ReadStrA(
// 				m_logID, "OutFlag", outFlag, "stdout", m_iniFullPath);
// 			std::vector<std::std::string> flags;
// 			StrUtil::SplitString(outFlag, "|", flags, false);
// 			if (flags.empty())
// 			{
// 				lp.nOutFlag = LOGOUT_FLAG_STDOUT;
// 			}
// 			else
// 			{
// 				lp.nOutFlag = ToFlag(flags[0]);
// 				for (size_t i = 1; i < flags.size(); i++)
// 				{
// 					lp.nOutFlag = (lp.nOutFlag|ToFlag(flags[i]));
// 				}
// 			}
// 
// 			// show log time?
// 			lp.bWithTime = (
// 				IniReader::ReadIntA(m_logID, "WithTime", 1, m_iniFullPath)
// 				!= 0);
// 
// 			// show millisecond?
// 			lp.bWithMillisecond = (
// 				IniReader::ReadIntA(
// 				m_logID, "WithMillisecond", 0, m_iniFullPath)
// 				!= 0);
// 
// 			// show source file?
// 			lp.bWithSrcFile = (
// 				IniReader::ReadIntA(m_logID, "WithSrcFile", 0, m_iniFullPath)
// 				!= 0);
// 
// 			// show function?
// 			lp.bWithFunction = (
// 				IniReader::ReadIntA(
// 				m_logID, "WithFunction", 0, m_iniFullPath)
// 				!= 0);
// 
// 			// overwrite or append?
// 			lp.bOverwrite = (
// 				IniReader::ReadIntA(m_logID, "Overwrite", 0, m_iniFullPath)
// 				!= 0);
// 
// 			// log file dir
// 			IniReader::ReadStrA(
// 				m_logID, "LogFileDir", lp.logFileDir, "", m_iniFullPath);
// 
// 			// log file name
// 			IniReader::ReadStrA(
// 				m_logID, "LogFileName", lp.logFileName, "", m_iniFullPath);
// 
// 			// dest url
// 			IniReader::ReadStrA(
// 				m_logID, "DestUrl", lp.destUrl, "", m_iniFullPath);
// 
// 			return true;
// 		}
// 		else
// 		{
// 			return false;
// 		}
// 	}
// 
// 	void BaseLog::SetLogFilePath()
// 	{
// 		// set log file directory
// 		if (m_params.logFileDir.size() == 0)
// 		{
// 			m_logFullPath =
// 				PathUtil::PathCombine_(
// 				PathUtil::ModuleFileDirectory_(), "log");
// 		}
// 		else
// 		{
// 			m_logFullPath = m_params.logFileDir;
// 		}
// 
// 		// set log file name
// 		if (m_params.logFileName.size() == 0)
// 		{
// 			std::std::string fileName =
// 				m_logID + StrUtil::GetSysTimeStrForFileName(m_bBinary);
// 
// 			m_logFullPath =
// 				PathUtil::PathCombine_(m_logFullPath, fileName);
// 		}
// 		else
// 		{
// 			m_logFullPath =
// 				PathUtil::PathCombine_(m_logFullPath, m_params.logFileName);
// 		}
// 	}
// 
// 	P_FILE BaseLog::GetFilePointer()
// 	{
// 		P_FILE pFile = NULL;
// 
// 		if (g_files.find(m_logFullPath) == g_files.end())
// 		{
// 			g_files.insert(make_pair(m_logFullPath, pFile));
// 		}
// 
// 		std::map<std::std::string, P_FILE>::iterator it = g_files.find(m_logFullPath);
// 		pFile = it->second;
// 		if (!FileUtil::Valid(pFile))
// 		{
// 			std::std::string sPath = PathUtil::GetDirectory_(m_logFullPath);
// 			PathUtil::CreateFolder_(sPath);
// 
// 			//open the log file...
// 			pFile = FileUtil::Open(m_logFullPath, m_params.bOverwrite);
// 			it->second = pFile;
// 		}
// 
// 		return pFile;
// 	}
// 
// 
// 	///////////////////////////////////////////////////////////////////////
// 	// YaoLog
// 	///////////////////////////////////////////////////////////////////////
// 	YaoLog::YaoLog(const std::std::string& logID, bool bEnable) : BaseLog(logID, bEnable)
// 	{
// 		m_bBinary = false;
// 	}
// 
// 	YaoLog::~YaoLog()
// 	{
// 	}
// 
// 	int YaoLog::MakeLogText(const std::std::string& str,
// 		const std::std::string& srcFile,
// 		const std::std::string& function_,
// 		int nLine)
// 	{
// 		size_t size_ = 100 + m_logID.size() + srcFile.size() +
// 			function_.size() + str.size();
// 
// 		// in most cases we use g_pData...
// 		if (size_ > BUF_SIZE)
// 		{
// 			g_pRealData = new char[size_];
// 			if (!g_pRealData) return 0;
// 			memset(g_pRealData, 0, size_);
// 			g_bUseNewBuffer = true;
// 		}
// 		else
// 		{
// 			g_pRealData = g_pData;
// 		}
// 
// 		std::std::string lineEnd("\n");
// #ifdef _YAO_LOG_WIN32_
// 		lineEnd = "\r\n";
// #endif
// 
// 		bool withSrcFile = (
// 			m_params.bWithSrcFile  &&
// 			srcFile.size() > 0 &&
// 			nLine > 0);
// 
// 		bool withFunction = (m_params.bWithFunction && function_.size() > 0);
// 
// 		int written = 0;
// 		if (m_params.bWithTime)
// 		{
// 			std::std::string now = StrUtil::GetSysTimeStr(m_params.bWithMillisecond);
// 
// 			if (withSrcFile)
// 			{
// 				if (withFunction)
// 				{
// 					written = sprintf(g_pRealData, "%s %s(%s,%d) [%s] -> %s%s",
// 						now.c_str(), PathUtil::GetFile_(srcFile).c_str(),
// 						function_.c_str(), nLine, m_logID.c_str(),
// 						str.c_str(), lineEnd.c_str());
// 				} 
// 				else
// 				{
// 					written = sprintf(g_pRealData, "%s %s(%d) [%s] -> %s%s",
// 						now.c_str(), PathUtil::GetFile_(srcFile).c_str(),
// 						nLine, m_logID.c_str(), str.c_str(), lineEnd.c_str());
// 				}
// 			} 
// 			else
// 			{
// 				written = sprintf(g_pRealData, "%s [%s] -> %s%s",
// 					now.c_str(), m_logID.c_str(), str.c_str(), lineEnd.c_str());
// 			}
// 		}
// 		else
// 		{
// 			if (withSrcFile)
// 			{
// 				if (withFunction)
// 				{
// 					written = sprintf(g_pRealData, "%s(%s,%d) [%s] -> %s%s",
// 						PathUtil::GetFile_(srcFile).c_str(),
// 						function_.c_str(), nLine, m_logID.c_str(),
// 						str.c_str(), lineEnd.c_str());
// 				} 
// 				else
// 				{
// 					written = sprintf(g_pRealData, "%s(%d) [%s] -> %s%s",
// 						PathUtil::GetFile_(srcFile).c_str(),
// 						nLine, m_logID.c_str(), str.c_str(), lineEnd.c_str());
// 				}
// 			} 
// 			else
// 			{
// 				written = sprintf(g_pRealData, "[%s] -> %s%s",
// 					m_logID.c_str(), str.c_str(), lineEnd.c_str());
// 			}
// 		}
// 
// 		return written;
// 	}
// 
// 	void YaoLog::DoLog(const std::std::string& str,
// 		const std::std::string& srcFile,
// 		const std::std::string& function_,
// 		int nLine)
// 	{
// 		int size_ = MakeLogText(str, srcFile, function_, nLine);
// 		if (size_ == 0) return;
// 
// 		//do the actual logging...
// 		if (m_params.nOutFlag & LOGOUT_FLAG_FILE)
// 		{
// 			P_FILE pFile = GetFilePointer();
// 			if (FileUtil::Valid(pFile))
// 			{
// 				FileUtil::Write(pFile, g_pRealData, size_);
// 			}
// 		}
// 		if (m_params.nOutFlag & LOGOUT_FLAG_STDOUT)
// 		{
// #ifdef _YAO_LOG_WIN32_
// 			AllocConsole();
// 			freopen("CONOUT$", "w+t", stdout);
// 			//freopen("CONIN$", "r+t", stdin);
// #else
// 			// TODO...
// #endif
// 			printf("%s", g_pRealData);
// 		}
// 		if (m_params.nOutFlag & LOGOUT_FLAG_REMOTE)
// 		{
// 			if (g_textData.size() >= MAX_QUEUE_SIZE)
// 				g_textData.pop();
// 
// 			g_textData.push(make_pair(m_logID, std::std::string(g_pRealData)));
// 		}
// 		if (m_params.nOutFlag & LOGOUT_FLAG_OUTPUTDEBUGSTRING)
// 		{
// #ifdef _YAO_LOG_WIN32_
// 			::OutputDebugStringA(g_pRealData);
// #endif
// 		}
// 
// 		ResetBuffer();
// 	}
// 
// 	void YaoLog::Log(const char *szSrcFile, const char *szFunction,
// 		int nLine, const TCHAR *szFormat, ...)
// 	{
// 		if (!m_params.bEnable) return;
// 
// 		yao_tstring str;
// 		va_list args;
// 		va_start(args, szFormat);
// 		StrUtil::vFormat(str, szFormat, args);
// 		va_end(args);
// 
// #ifdef _YAO_LOG_UNICODE_
// 		std::std::string s1 = StrUtil::WStrToStr(str);
// 		DoLog(s1, szSrcFile, szFunction, nLine);
// #else
// 		DoLog(str, szSrcFile, szFunction, nLine);
// #endif
// 	}
// 
// #ifdef _YAO_LOG_WIN32_
// 	void YaoLog::LogA(char *szSrcFile, char *szFunction,
// 		int nLine, const char *szFormat, ...)
// 	{
// 		if (!m_params.bEnable) return;
// 
// 		std::std::string str;
// 		va_list args;
// 		va_start(args, szFormat);
// 		StrUtil::vFormatA(str, szFormat, args);
// 		va_end(args);
// 
// 		DoLog(str, szSrcFile, szFunction, nLine);
// 	}
// #endif
// 
// #ifdef _YAO_LOG_WIN32_
// 	void YaoLog::LogW(char *szSrcFile, char *szFunction,
// 		int nLine, const wchar_t *szFormat, ...)
// 	{
// 		if (!m_params.bEnable) return;
// 
// 		std::wstring str;
// 		va_list args;
// 		va_start(args, szFormat);
// 		StrUtil::vFormatW(str, szFormat, args);
// 		va_end(args);
// 
// 		std::std::string s1 = StrUtil::WStrToStr(str);
// 		DoLog(s1, szSrcFile, szFunction, nLine);
// 	}
// #endif
// 
// 
// 
// 	YaoBinLog::YaoBinLog(const std::std::string& logID, bool bEnable) : BaseLog(logID, bEnable)
// 	{
// 		m_bBinary = true;
// 	}
// 
// 	void YaoBinLog::LogBin(void *pData, int nSize)
// 	{
// 		if (!m_params.bEnable) return;
// 
// 		//do the actual logging...
// 		if (m_params.nOutFlag & LOGOUT_FLAG_FILE)
// 		{
// 			P_FILE pFile = GetFilePointer();
// 			if (FileUtil::Valid(pFile))
// 			{
// 				FileUtil::Write(pFile, (char*)pData, nSize);
// 			}
// 		}
// 		if (m_params.nOutFlag & LOGOUT_FLAG_STDOUT)
// 		{
// 			// do nothing here...
// 		}
// 		if (m_params.nOutFlag & LOGOUT_FLAG_REMOTE)
// 		{
// 			if (g_binData.size() >= MAX_QUEUE_SIZE)
// 				g_binData.pop();
// 
// 			g_binData.push(make_pair(m_logID, std::std::string((char*)pData, nSize)));
// 		}
// 		if (m_params.nOutFlag & LOGOUT_FLAG_OUTPUTDEBUGSTRING)
// 		{
// 			// do nothing here...
// 		}
// 	}
// 
// 
// } // namespace YaoUtil

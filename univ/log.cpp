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
#define INVALID_SOCKET -1
#define closesocket close
#define PATH_SEPARATOR '/'
#else
#define PATH_SEPARATOR '\\'
#endif // !_WINDOWS

WCHAR g_szFormatBuffer[2048];

Sloong::Universal::CLog::CLog()
{
	m_bInit = false;
    m_bDebug = true;
	m_nLastDate = 0;
	m_bOpenFileFirst = false;
	m_pFile = nullptr;
	m_nNetLogListenSocket = INVALID_SOCKET;
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
		// write log message to file
		fputs(str.c_str(), m_pFile);
		if (m_nNetLogListenSocket != INVALID_SOCKET )
		{
			char pBufLen[8] = { 0 };
			auto len = str.length() + 1;
			CUniversal::LongToBytes(len, pBufLen);
			// send log message to socket
			BOOST_FOREACH(SOCKET sock, m_vLogSocketList)
			{
				CUniversal::SendEx(sock, pBufLen, 8);
				CUniversal::SendEx(sock, str.c_str(), len);
			}
		}
		
		// in debug mode, flush the message when write down. 
		// for issue #8 [https://git.sloong.com/public/library/issues/8]
		if (m_bDebug)
		{
			cout << str;
			fflush(m_pFile);
		}
	}
}

void CLog::LogSystemWorkLoop()
{
	unique_lock <mutex> lck(m_Mutex);
	while (m_emStatus != RUN_STATUS::Exit)
	{
		if (m_emStatus == RUN_STATUS::Created)
		{
			continue;
		}

		IsOpen();

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

/************************************************************************
			               Enable Network Log 
		Create by wcb in 2017/09/27											
	Params:
		port : the log system will license in this port,
	Return: 
		S_OK if listen succeed. 
		else return error code.
	Remarks: 
		 when connect down,	the log system will send the log data to this port.
*************************************************************************/
int Sloong::Universal::CLog::EnableNetworkLog(int port)
{
	m_nNetLogListenSocket = socket(AF_INET, SOCK_STREAM, 0);

#ifdef _WINDOWS
    if (m_nNetLogListenSocket == INVALID_SOCKET)
    {
        return GetLastError();
    }
    struct sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
	sin.sin_addr.S_un.S_addr = INADDR_ANY; 
	if(SOCKET_ERROR == ::bind(m_nNetLogListenSocket, (LPSOCKADDR)&sin, sizeof(sin)))
	{
		return GetLastError();
	}

    if (listen(m_nNetLogListenSocket, 10) == SOCKET_ERROR)
	{
		return GetLastError();
	}
#else
    struct sockaddr_in address;
    memset(&address,0,sizeof(address));
    address.sin_addr.s_addr=htonl(INADDR_ANY);
    address.sin_port=htons(port);

    // 缁戝畾绔彛
    errno = bind(m_nNetLogListenSocket,(struct sockaddr*)&address,sizeof(address));
    errno = listen(m_nNetLogListenSocket,10);
#endif
	CThreadPool::AddWorkThread(AcceptNetlogLoop, this, 1);
    return 0;
}



bool Sloong::Universal::CLog::OpenFile()
{
	if (m_pFile != nullptr)
		return true;
	if (m_szFileName.empty())
		throw normal_except("Open log file failed.file name is empty.");

	if (m_bOpenFileFirst){
		m_bOpenFileFirst = true;
	}else{
		cout << "File no open , try open file. file path is :" << m_szFileName << endl;
	}
	CUniversal::CheckFileDirectory(m_szFileName);
	auto flag = "a+";
	if (!m_bIsCoverPrev)
		flag = "w+";

#ifdef _WINDOWS
	auto errno = fopen_s(&m_pFile, m_szFileName.c_str(), flag);
#else
	m_pFile = fopen(m_szFileName.c_str(),flag);
#endif
	if ( m_pFile == nullptr)
	{
		cerr << "Open file error. error no " << errno << endl;
	}

	return m_pFile != nullptr;
}

LPVOID Sloong::Universal::CLog::AcceptNetlogLoop(LPVOID param)
{
	CLog* pThis = (CLog*)param;
	SOCKET sClient;
    const char* pData = g_strConnect.c_str();
    int nLen = g_strConnect.length()+1;
	while (pThis->m_emStatus != RUN_STATUS::Exit)
	{
        sClient = accept(pThis->m_nNetLogListenSocket, NULL,NULL);
		if (sClient == INVALID_SOCKET)
		{
			continue;
		}
        char pLen[8] = {0};
        CUniversal::Int64ToBytes(nLen,pLen);
        CUniversal::SendEx(sClient,pLen,8);
        CUniversal::SendEx(sClient,pData,nLen);
		pThis->m_vLogSocketList.push_back(sClient);
	}
	return NULL;
}

std::string Sloong::Universal::CLog::GetFileName()
{
	return m_szFileName;
}

bool Sloong::Universal::CLog::IsOpen()
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
	IsOpen();
	ProcessWaitList();
	ProcessLogList();
	Flush();
	Close();
	BOOST_FOREACH(SOCKET sock, m_vLogSocketList)
	{
		closesocket(sock);
	}
	m_vLogSocketList.clear();
	closesocket(m_nNetLogListenSocket);
	m_nNetLogListenSocket = INVALID_SOCKET;
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
	fflush(m_pFile);
}

void CLog::SetConfiguration(std::string szFileName, LOGTYPE* pType, LOGLEVEL* pLevel, bool bDebug , string strExtendName )
{
	if (pType)
	{
		m_emType = *pType;
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
	m_strExtendName = strExtendName;

	if (pLevel)
	{
		m_emLevel = *pLevel;
		WriteLine(CUniversal::Format("[Info]:[Set Log Level To %d]",m_emLevel));
	}

    m_bDebug = bDebug;
}

void Sloong::Universal::CLog::Initialize()
{
	Initialize("./log.log");
}

void Sloong::Universal::CLog::Initialize(string szPathName, string strExtendName /*= ""*/, bool bDebug /*= true */, LOGLEVEL emLevel /*= LOGLEVEL::All*/, LOGTYPE emType /*= LOGTYPE::ONEFILE*/, bool bIsCoverPrev /*= false*/)
{
	// All value init
	m_bInit = true;
	m_szFilePath.clear();
	m_szFileName.clear();

	// Set value
	m_bIsCoverPrev = bIsCoverPrev;
	
    SetConfiguration( szPathName, &emType, &emLevel, bDebug, strExtendName);

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

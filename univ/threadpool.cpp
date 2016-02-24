#include "stdafx.h"
#include "univ.h"
#include "threadpool.h"
#include <boost/foreach.hpp>

#ifndef _WINDOWS
#include <unistd.h>
#endif

using namespace Sloong::Universal;

queue<ThreadParam*> Sloong::Universal::CThreadPool::m_pJobList;
vector<thread*> Sloong::Universal::CThreadPool::m_pThreadList;
bool Sloong::Universal::CThreadPool::m_bExit = false;
bool Sloong::Universal::CThreadPool::m_bStart = false;
int Sloong::Universal::CThreadPool::m_nSleepInterval = 100;

mutex Sloong::Universal::CThreadPool::g_oJobListMutex;


void Sloong::Universal::CThreadPool::Initialize(int nThreadNum, int nSleepInterval /* = 100 */)
{
	m_nSleepInterval = nSleepInterval;
	for (int i = 0; i < nThreadNum; i++)
	{
		thread* pThread = new thread(ThreadWorkLoop);
		m_pThreadList.push_back(pThread);
	}
}

void Sloong::Universal::CThreadPool::Start()
{
	m_bStart = true;
}

void Sloong::Universal::CThreadPool::ThreadWorkLoop()
{
	while (true)
	{
		try
		{
			if ( m_bExit )
			{
				break;
			}
			if (m_bStart == false)
			{
				SLEEP(m_nSleepInterval);
				continue;
			}
			if (m_pJobList.empty())
			{
				SLEEP(m_nSleepInterval);
				continue;
			}
			
			// Run the job list first. 
			if (!m_pJobList.empty())
			{
				std::unique_lock<mutex> lck(g_oJobListMutex);
				if (m_pJobList.empty())
				{
					lck.unlock();
					continue;
				}
				auto pJob = m_pJobList.front();
				m_pJobList.pop();
				lck.unlock();
				(*pJob->pJob)(pJob->pParam);
				// Just delete the ThreadParam, the pParmam function should be delete in the job. 
				// because don't know the pararm type. 
				SAFE_DELETE(pJob);
			}
		}
		catch (...)
		{
			printf( "Unknown error happened in threadpool work loop.");
		}
	}
}

void Sloong::Universal::CThreadPool::End()
{
	m_bExit = true;
	m_bStart = false;
	// clear the job list.
	while (!m_pJobList.empty())
	{
		auto pJob = m_pJobList.front();
		m_pJobList.pop();
		SAFE_DELETE(pJob);
	}
}

int Sloong::Universal::CThreadPool::EnqueTask(LPCALLBACKFUNC pJob, LPVOID pParam )
{
	ThreadParam* pItem = new ThreadParam();
	pItem->pJob = pJob;
	pItem->pParam = pParam;
	std::lock_guard<mutex> lck(g_oJobListMutex);
	m_pJobList.push(pItem);
	return m_pJobList.size() - 1;
}

int Sloong::Universal::CThreadPool::AddWorkThread(LPCALLBACKFUNC pJob, LPVOID pParam, int nNum /* = 1*/ )
{
    int nIndex = CThreadPool::m_pThreadList.size();
    for( int i = 0; i < nNum; i++ )
    {
        thread* pThread = new thread(pJob, pParam);
        CThreadPool::m_pThreadList.push_back(pThread);
    }

    return nIndex;
}


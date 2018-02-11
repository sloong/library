#include "stdafx.h"
#include "univ.h"
#include "threadpool.h"

#include <chrono>

#ifndef _WINDOWS
#include <unistd.h>
#endif

using namespace Sloong;
using namespace Sloong::Universal;

queue<ThreadParam*> Sloong::Universal::CThreadPool::m_pJobList;
vector<thread*> Sloong::Universal::CThreadPool::m_pThreadList;
RUN_STATUS Sloong::Universal::CThreadPool::m_emStatus = Created;
condition_variable Sloong::Universal::CThreadPool::g_oRunJobCV;
mutex Sloong::Universal::CThreadPool::g_oJobListMutex;
mutex Sloong::Universal::CThreadPool::g_oRunJobMutex;

void Sloong::Universal::CThreadPool::Initialize(int nThreadNum)
{
	for (int i = 0; i < nThreadNum; i++)
	{
		thread* pThread = new thread(ThreadWorkLoop);
		m_pThreadList.push_back(pThread);
	}
}

void Sloong::Universal::CThreadPool::Run()
{
	m_emStatus = Running;
}

void Sloong::Universal::CThreadPool::ThreadWorkLoop()
{
	unique_lock<mutex> lck(g_oRunJobMutex);
	while (m_emStatus != RUN_STATUS::Exit)
	{
		try
		{
			if (m_emStatus == RUN_STATUS::Created)
			{
				SLEEP(1000);
				continue;
			}
			// each 1 second check the list once.
			if (m_pJobList.empty())
			{
				g_oRunJobCV.wait(lck);
				continue;
			}
			
			// Run the job list first. 
			if (!m_pJobList.empty())
			{
				unique_lock <mutex> list_lock(g_oJobListMutex);
				if (m_pJobList.empty())
				{
					list_lock.unlock();
					continue;
				}
				auto pJob = m_pJobList.front();
				m_pJobList.pop();
				list_lock.unlock();
				switch (pJob->nJobType)
				{
				case 1:
					(*(LPCALLBACKFUNC)pJob->pJob)(pJob->pParam);
					break;
				case 2:
					(*(LPCALLBACK2FUNC)pJob->pJob)(pJob->pParam,pJob->pParam2);
					break;
				case 3:
					(*(LPCALLBACK3FUNC)pJob->pJob)(pJob->pParam,pJob->pParam2,pJob->pParam3);
					break;
				}
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

void Sloong::Universal::CThreadPool::Exit()
{
	m_emStatus = RUN_STATUS::Exit;
	// clear the job list.
	while (!m_pJobList.empty())
	{
		auto pJob = m_pJobList.front();
		m_pJobList.pop();
		SAFE_DELETE(pJob);
	}
}

int Sloong::Universal::CThreadPool::EnqueTask(LPCALLBACKFUNC pJob, LPVOID pParam)
{
	ThreadParam* pItem = new ThreadParam();
	pItem->nJobType = 1;
	pItem->pJob = pJob;
	pItem->pParam = pParam;
	std::lock_guard<mutex> lck(g_oJobListMutex);
	m_pJobList.push(pItem);
	g_oRunJobCV.notify_one();
	return m_pJobList.size() - 1;
}

int Sloong::Universal::CThreadPool::EnqueTask2(LPCALLBACK2FUNC pJob, LPVOID pParam, LPVOID pParam2)
{
	ThreadParam* pItem = new ThreadParam();
	pItem->nJobType = 2;
	pItem->pJob = pJob;
	pItem->pParam = pParam;
	pItem->pParam2 = pParam2;
	std::lock_guard<mutex> lck(g_oJobListMutex);
	m_pJobList.push(pItem);
	g_oRunJobCV.notify_one();
	return m_pJobList.size() - 1;
}

int Sloong::Universal::CThreadPool::EnqueTask3(LPCALLBACK3FUNC pJob, LPVOID pParam, LPVOID pParam2, LPVOID pParam3)
{
	ThreadParam* pItem = new ThreadParam();
	pItem->nJobType = 3;
	pItem->pJob = pJob;
	pItem->pParam = pParam;
	pItem->pParam2 = pParam2;
	pItem->pParam3 = pParam3;
	std::lock_guard<mutex> lck(g_oJobListMutex);
	m_pJobList.push(pItem);
	g_oRunJobCV.notify_one();
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


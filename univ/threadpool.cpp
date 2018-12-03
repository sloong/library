#include "stdafx.h"
#include "univ.h"
#include "threadpool.h"

#include <chrono>

#ifndef _WINDOWS
#include <unistd.h>
#endif

using namespace Sloong;
using namespace Sloong::Universal;

queue<shared_ptr<ThreadParam>> Sloong::Universal::CThreadPool::m_pJobList;
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
				auto pJobInfo = m_pJobList.front();
				m_pJobList.pop();
				list_lock.unlock();

				LPVOID pRes = nullptr;
				if ( pJobInfo->pJob != nullptr )
					pRes = (*pJobInfo->pJob)(pJobInfo->pParam);
				if ( pJobInfo->pCallBack != nullptr )
					(*pJobInfo->pCallBack)(pRes);
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
	}
}


int Sloong::Universal::CThreadPool::EnqueTask(LPCALLBACKFUNC pJob, LPCALLBACKFUNC pCallBack, LPVOID pParam)
{
	auto pItem = make_shared<ThreadParam>();
	pItem->pJob = pJob;
	pItem->pCallBack = pCallBack;
	pItem->pParam = pParam;

	std::lock_guard<mutex> lck(g_oJobListMutex);
	m_pJobList.push(pItem);
	g_oRunJobCV.notify_one();
	return m_pJobList.size();
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


#include "stdafx.h"
#include "univ.h"
#include "threadpool.h"

#include <chrono>

#ifndef _WINDOWS
#include <unistd.h>
#endif

using namespace Sloong;
using namespace Sloong::Universal;

map<ULONG, shared_ptr<TaskParam>> Sloong::Universal::CThreadPool::m_oJobList;
queue<ULONG> Sloong::Universal::CThreadPool::m_oWaitList;
vector<thread*> Sloong::Universal::CThreadPool::m_pThreadList;
RUN_STATUS Sloong::Universal::CThreadPool::m_emStatus = Created;
condition_variable Sloong::Universal::CThreadPool::g_oRunJobCV;
mutex Sloong::Universal::CThreadPool::g_oJobListMutex;
mutex Sloong::Universal::CThreadPool::g_oRunJobMutex;
ULONG Sloong::Universal::CThreadPool::m_nIDCursor;

void Sloong::Universal::CThreadPool::Initialize(int nThreadNum)
{
	for (int i = 0; i < nThreadNum; i++)
	{
		thread* pThread = new thread(ThreadWorkLoop);
		m_pThreadList.push_back(pThread);
	}
	m_nIDCursor = 0;
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
			if (m_oWaitList.empty())
			{
				g_oRunJobCV.wait(lck);
				continue;
			}
			
			// Run the job list first. 
			if (!m_oWaitList.empty())
			{
				unique_lock <mutex> list_lock(g_oJobListMutex);
				if (m_oWaitList.empty())
				{
					list_lock.unlock();
					continue;
				}
				int nJobID = m_oWaitList.front();
				m_oWaitList.pop();
				auto pJobInfo = m_oJobList[nJobID];
				m_oJobList.erase(nJobID);
				list_lock.unlock();

				switch (pJobInfo->emTaskType)
				{
				case TaskType::SmartFunc: {
					SMARTER pRes = nullptr;
					if (pJobInfo->pSmartFuncJob != nullptr)
						pRes = pJobInfo->pSmartFuncJob(pJobInfo->pSmartParam);
					if (pJobInfo->pSmartFuncCallback != nullptr)
						pJobInfo->pSmartFuncCallback(pJobInfo->nTaskID, pRes);
				}
					break;
				case TaskType::SmartParam:
				{
					SMARTER pRes = nullptr;
					if (pJobInfo->pSmartJob != nullptr)
						pRes = (*pJobInfo->pSmartJob)(pJobInfo->pSmartParam);
					if (pJobInfo->pSmartCallBack != nullptr)
						(*pJobInfo->pSmartCallBack)(pJobInfo->nTaskID, pRes);
				}
					break;
				case TaskType::Normal:
				default:
				{
					LPVOID pRes = nullptr;
					if (pJobInfo->pJob != nullptr)
						pRes = (*pJobInfo->pJob)(pJobInfo->pParam);
					if (pJobInfo->pCallBack != nullptr)
						(*pJobInfo->pCallBack)(pJobInfo->nTaskID, pRes);
				}
					break;
				}
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
	m_oJobList.clear();
	m_nIDCursor = 0;
}


ULONG Sloong::Universal::CThreadPool::EnqueTask(LPTASKFUNC pJob, LPVOID pParam, LPTASKCALLBACK pCallBack)
{
	auto pItem = make_shared<TaskParam>();
	pItem->pJob = pJob;
	pItem->pCallBack = pCallBack;
	pItem->pParam = pParam;
	pItem->emTaskType = TaskType::Normal;
	std::lock_guard<mutex> lck(g_oJobListMutex);
	m_nIDCursor++;
	pItem->nTaskID = m_nIDCursor;
	m_oJobList[m_nIDCursor] = pItem;
	m_oWaitList.push(m_nIDCursor);
	g_oRunJobCV.notify_one();
	return m_nIDCursor;
}

ULONG Sloong::Universal::CThreadPool::EnqueTask(LPSMARTFUNC pJob, SMARTER pParam, LPSMARTCALLBACK pCallBack)
{
	auto pItem = make_shared<TaskParam>();
	pItem->pSmartJob = pJob;
	pItem->pSmartCallBack = pCallBack;
	pItem->pSmartParam = pParam;
	pItem->emTaskType = TaskType::SmartParam;
	std::lock_guard<mutex> lck(g_oJobListMutex);
	m_nIDCursor++;
	pItem->nTaskID = m_nIDCursor;
	m_oJobList[m_nIDCursor] = pItem;
	m_oWaitList.push(m_nIDCursor);
	g_oRunJobCV.notify_one();
	return m_nIDCursor;
}


ULONG Sloong::Universal::CThreadPool::EnqueTask(SmartFunction pJob, SMARTER pParam /* = nullptr */, SmartCallbackFunction pCallBack /* = nullptr */)
{
	auto pItem = make_shared<TaskParam>();
	pItem->pSmartFuncJob = pJob;
	pItem->pSmartFuncCallback = pCallBack;
	pItem->pSmartParam = pParam;
	pItem->emTaskType = TaskType::SmartFunc;
	std::lock_guard<mutex> lck(g_oJobListMutex);
	m_nIDCursor++;
	pItem->nTaskID = m_nIDCursor;
	m_oJobList[m_nIDCursor] = pItem;
	m_oWaitList.push(m_nIDCursor);
	g_oRunJobCV.notify_one();
	return m_nIDCursor;
}


int Sloong::Universal::CThreadPool::AddWorkThread(LPTASKFUNC pJob, LPVOID pParam, int nNum /* = 1*/ )
{
    int nIndex = CThreadPool::m_pThreadList.size();
    for( int i = 0; i < nNum; i++ )
    {
        thread* pThread = new thread(pJob, pParam);
        CThreadPool::m_pThreadList.push_back(pThread);
    }

    return nIndex;
}


int Sloong::Universal::CThreadPool::AddWorkThread(std::function<void(SMARTER)> pJob, SMARTER pParam, int nNum/* = 1*/)
{
	int nIndex = CThreadPool::m_pThreadList.size();
	for (int i = 0; i < nNum; i++)
	{
		thread* pThread = new thread(pJob, pParam);
		CThreadPool::m_pThreadList.push_back(pThread);
	}

	return nIndex;
}



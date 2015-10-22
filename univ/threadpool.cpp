#include "stdafx.h"
#include "univ.h"
#include "threadpool.h"


#ifndef _WINDOWS
#include <unistd.h>
#define Sleep sleep
#endif

using namespace Sloong::Universal;

queue<ThreadParam*> Sloong::Universal::CThreadPool::m_pJobList;
vector<thread*> Sloong::Universal::CThreadPool::m_pThreadList;

mutex Sloong::Universal::CThreadPool::g_oMutex;

CThreadPool g_oThreadPool;

Sloong::Universal::CThreadPool::CThreadPool()
{
	//m_pThreadList = new vector<thread*>;
	//m_pJobList = new queue<ThreadParam*>;
}

Sloong::Universal::CThreadPool::~CThreadPool()
{
	//SAFE_DELETE(m_pThreadList);
	//SAFE_DELETE(m_pJobList);
}

void Sloong::Universal::CThreadPool::Initialize(int nThreadNum)
{
	for (int i = 0; i < nThreadNum; i++)
	{
		thread* pThread = new thread(ThreadWorkLoop);
		m_pThreadList.push_back(pThread);
	}
}

void Sloong::Universal::CThreadPool::Start()
{
	//g_oMutex = //CreateMutex(NULL, FALSE, _T("SloongThreadPoolMutex"));
// 	for each (auto& item in *m_pThreadList)
// 	{
// 		item->join();
// 	}
}

void Sloong::Universal::CThreadPool::ThreadWorkLoop()
{
	while (true)
	{
		try
		{
			if (m_pJobList.empty() || 0 == m_pJobList.size())
			{
				Sleep(1);
				continue;
			}
			std::lock_guard<mutex> lck(g_oMutex);
			if (m_pJobList.empty() || 0 == m_pJobList.size())
			{
				continue;
			}
			auto pItem = m_pJobList.front();
			m_pJobList.pop();
			(*pItem->pJob)(pItem->pParam);
			SAFE_DELETE(pItem);
		}
		catch (...)
		{
			printf( "Error happened in threadpool work loop.");
		}
	}
}

void Sloong::Universal::CThreadPool::End()
{

}

int Sloong::Universal::CThreadPool::AddTask(LPCALLBACKFUNC pJob, LPVOID pParam)
{
	ThreadParam* pItem = new ThreadParam();
	pItem->pJob = pJob;
	pItem->pParam = pParam;
	std::lock_guard<mutex> lck(g_oMutex);
	m_pJobList.push(pItem);
	return (int)m_pJobList.size();
}

void Sloong::Universal::CThreadPool::RemoveTask(int index)
{

}

int Sloong::Universal::CThreadPool::GetTaskTotal()
{
	return (int)m_pJobList.size();
}

thread* Sloong::Universal::CThreadPool::AddWorkThread(LPCALLBACKFUNC pJob, LPVOID pParam)
{
	thread* pThread = new thread(pJob, pParam);
	g_oThreadPool.m_pThreadList.push_back(pThread);
	return pThread;
}


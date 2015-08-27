#include "stdafx.h"
#include "univ.h"
#include "threadpool.h"
using namespace Sloong::Universal;

list<ThreadParam*>* Sloong::Universal::CThreadPool::m_pJobList;

HANDLE Sloong::Universal::CThreadPool::g_pMutex;
CRITICAL_SECTION g_pData;
Sloong::Universal::CThreadPool::CThreadPool()
{
	m_pThreadList = new vector<HANDLE>;
	m_pJobList = new list<ThreadParam*>;
}

Sloong::Universal::CThreadPool::~CThreadPool()
{
	SAFE_DELETE(m_pThreadList);
	SAFE_DELETE(m_pJobList);
}

void Sloong::Universal::CThreadPool::Initialize(int nThreadNum)
{
	for (int i = 0; i < nThreadNum; i++)
	{
		auto pThread = CreateThread(NULL, NULL, ThreadWorkLoop, NULL, NULL, NULL);
		m_pThreadList->push_back(pThread);
	}
}

void Sloong::Universal::CThreadPool::Start()
{
	g_pMutex = CreateMutex(NULL, FALSE, _T("SloongThreadPoolMutex"));
	// 	for each (auto& item in *m_pThreadList)
	// 	{
	// 	}
}

DWORD Sloong::Universal::CThreadPool::ThreadWorkLoop(LPVOID lpParam)
{
	while (true)
	{
		if (m_pJobList->empty() || 0 == m_pJobList->size())
		{
			Sleep(1);
			continue;
		}
		WaitForSingleObject(g_pMutex, INFINITE);
		if (m_pJobList->empty() || 0 == m_pJobList->size())
		{
			ReleaseMutex(g_pMutex);
			continue;
		}
		auto pItem = m_pJobList->front();
		m_pJobList->pop_front();
		ReleaseMutex(g_pMutex);
		(*pItem->pJob)(pItem->pParam);
		SAFE_DELETE(pItem);
	}
	return 0;
}

void Sloong::Universal::CThreadPool::End()
{

}

int Sloong::Universal::CThreadPool::AddTask(LPCALLBACKFUNC pJob, LPVOID pParam)
{
	ThreadParam* pItem = new ThreadParam();
	pItem->pJob = pJob;
	pItem->pParam = pParam;
	WaitForSingleObject(g_pMutex, INFINITE);
	m_pJobList->push_back(pItem);
	ReleaseMutex(g_pMutex);
	return (int)m_pJobList->size();
}

void Sloong::Universal::CThreadPool::RemoveTask(int index)
{

}

int Sloong::Universal::CThreadPool::GetTaskTotal()
{
	return (int)m_pJobList->size();
}

HANDLE Sloong::Universal::CThreadPool::GetJobListMutex()
{
	return g_pMutex;
}

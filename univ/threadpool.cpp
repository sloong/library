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
vector<ThreadParam*> Sloong::Universal::CThreadPool::m_pStaticJob;
bool Sloong::Universal::CThreadPool::m_bExit;
bool Sloong::Universal::CThreadPool::m_bStart;

mutex Sloong::Universal::CThreadPool::g_oMutex;


Sloong::Universal::CThreadPool::CThreadPool()
{
	m_bExit = false;
	m_bStart = false;
}

Sloong::Universal::CThreadPool::~CThreadPool()
{
	m_bExit = true;
	m_bStart = false;
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
                SLEEP(100);
				continue;
			}
			if ((m_pJobList.empty() || 0 == m_pJobList.size()) && m_pStaticJob.size() == 0)
			{
                SLEEP(100);
				continue;
			}
			std::lock_guard<mutex> lck(g_oMutex);
			// Run the job list first. 
			if (!m_pJobList.empty() && 0 < m_pJobList.size())
			{
				auto pJob = m_pJobList.front();
				m_pJobList.pop();
				(*pJob->pJob)(pJob->pParam);
				// Just delete the ThreadParam, the pParmam function should be delete in the job.
				SAFE_DELETE(pJob);
			}
			
			// Then foreach the static job 
			if ( m_pStaticJob.size() > 0)
			{
				BOOST_FOREACH (auto pItem , m_pStaticJob)
				{
					if (pItem)
					{
						(*pItem->pJob)(pItem->pParam);
					}
				}
			}
		}
		catch (...)
		{
			printf( "Error happened in threadpool work loop.");
		}
	}
}

void Sloong::Universal::CThreadPool::End()
{
	// clear the job list.
	while (!m_pJobList.empty())
	{
		auto pJob = m_pJobList.front();
		m_pJobList.pop();
		SAFE_DELETE(pJob);
	}
	while (!m_pStaticJob.empty())
	{
		auto pJob = m_pStaticJob.back();
		m_pStaticJob.pop_back();
		SAFE_DELETE(pJob);
	}
	m_pStaticJob.clear();
	m_bExit = true;
	m_bStart = false;
}

int Sloong::Universal::CThreadPool::AddTask(LPCALLBACKFUNC pJob, LPVOID pParam, bool bStatic /* = false */ )
{
	ThreadParam* pItem = new ThreadParam();
	pItem->pJob = pJob;
	pItem->pParam = pParam;
	std::lock_guard<mutex> lck(g_oMutex);
	if (bStatic)
	{
		m_pStaticJob.push_back(pItem);
	}
	else
	{
		m_pJobList.push(pItem);
	}
	return GetTaskTotal(bStatic)-1;
}

void Sloong::Universal::CThreadPool::RemoveTask(int index)
{
	// Just delete the param. no remove it from vector object. 
	// if remove it, the all is returned index will be error.
	auto pItem = m_pStaticJob[index];
	SAFE_DELETE(pItem);
	m_pStaticJob[index] = NULL;
}

int Sloong::Universal::CThreadPool::GetTaskTotal( bool bStatic /* = false */ )
{
	if ( bStatic )
		return (int)m_pStaticJob.size();
	else
		return (int)m_pJobList.size();
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


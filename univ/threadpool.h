#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <thread>
#include <mutex>
#include <queue>
#include <vector>
#include <map>
#include <condition_variable>
using std::queue;
using std::thread;
using std::mutex;
namespace Sloong
{
	namespace Universal
	{
		typedef LPVOID(*pTaskJobFunc)(LPVOID);
		typedef void(*pTaskCallBack)(long long, LPVOID);
		typedef pTaskJobFunc LPTASKFUNC;
		typedef pTaskCallBack LPTASKCALLBACK;
		struct TaskParam
		{
			LPTASKFUNC		pJob = nullptr;
			LPTASKCALLBACK	pCallBack= nullptr;
			LPVOID			pParam= nullptr;
			ULONG			nTaskID = 0;
		};
		
		class UNIVERSAL_API CThreadPool
		{
		public:
			static void Initialize(int nThreadNum);

			static void Run();
			static void Exit();
			// Add a task to job list.
			// the pJob is the job function pointer.
			// the pParam is the job function param when call the function.
			// the bStatic is the job is not doing once. if ture, the job will always run it in the threadpool. 
			// and the function return the job index in job list. for once job, it can not do anything, for static job
			// it can used in RemoveTask function.
			static ULONG EnqueTask(LPTASKFUNC pJob, LPTASKCALLBACK pCallBack, LPVOID pParam);

            // Add a work thread to the threadlist.
            // return the thread index in threadlist. if the nNum param is not 1, the other
            // thread index is base on return value.
            static int AddWorkThread(LPTASKFUNC pJob, LPVOID pParam, int nNum = 1);

		protected:
			static map<ULONG, shared_ptr<TaskParam>>	m_oJobList;
			static vector<thread*> 		m_pThreadList;
			static queue<ULONG>			m_oWaitList;
			static void ThreadWorkLoop(void);
			static mutex g_oJobListMutex;
			static mutex g_oRunJobMutex;
			static condition_variable g_oRunJobCV;
			static RUN_STATUS m_emStatus;
			static ULONG m_nIDCursor;
		};
	}
}

#endif // !THREADPOOL_H


#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <thread>
#include <mutex>
#include <queue>
using std::queue;
using std::thread;
using std::mutex;
namespace Sloong
{
	namespace Universal
	{

//		typedef LPTHREAD_START_ROUTINE LPCALLBACKFUNC;
		typedef LPVOID(*pCallBack)(LPVOID);
		typedef pCallBack LPCALLBACKFUNC;
		struct ThreadParam
		{
			LPCALLBACKFUNC	pJob;
			LPVOID			pParam;
			mutex			oMutex;
			bool		    bIsRunning;
		};

		class UNIVERSAL_API CThreadPool
		{
		public:
			CThreadPool();
			virtual ~CThreadPool();

			virtual void Initialize(int nThreadNum);

			virtual void Start();
			virtual void End();
			// Add a task to job list.
			// the pJob is the job function pointer.
			// the pParam is the job function param when call the function.
			// the bStatic is the job is not doing once. if ture, the job will always run it in the threadpool. 
			// and the function return the job index in job list. for once job, it can not do anything, for static job
			// it can used in RemoveTask function.
			virtual int AddTask(LPCALLBACKFUNC pJob, LPVOID pParam, bool bStatic = false);

			// remove a task from job list.
			virtual void RemoveTask(int index);
			virtual int GetTaskTotal( bool bStatic = false );

		public:
            // Add a work thread to the threadlist.
            // return the thread index in threadlist. if the nNum param is not 1, the other
            // thread index is base on return value.
            static int AddWorkThread(LPCALLBACKFUNC pJob, LPVOID pParam, int nNum = 1);

		protected:
			static vector<thread*> m_pThreadList;
			static queue<ThreadParam*>	m_pJobList;
			static vector<ThreadParam*>	m_pStaticJob;
			static void ThreadWorkLoop(void);
			static mutex g_oJobListMutex;
			static bool m_bExit;
			static bool m_bStart;
			
		};
	}
}

#endif // !THREADPOOL_H


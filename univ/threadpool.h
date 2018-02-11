#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <thread>
#include <mutex>
#include <queue>
#include <condition_variable>
using std::queue;
using std::thread;
using std::mutex;
namespace Sloong
{
	namespace Universal
	{
		typedef LPVOID(*pCallBack)(LPVOID);
		typedef LPVOID(*pCallBack2)(LPVOID,LPVOID);
		typedef LPVOID(*pCallBack3)(LPVOID,LPVOID,LPVOID);
		typedef pCallBack LPCALLBACKFUNC;
		typedef pCallBack2 LPCALLBACK2FUNC;
		typedef pCallBack3 LPCALLBACK3FUNC;
		struct ThreadParam
		{
			int				nJobType;
			LPVOID			pJob;
			LPVOID			pParam;
			LPVOID			pParam2;
			LPVOID			pParam3;
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
			static int EnqueTask(LPCALLBACKFUNC pJob, LPVOID pParam);
			static int EnqueTask2(LPCALLBACK2FUNC pJob, LPVOID pParam, LPVOID pParam2 );
			static int EnqueTask3(LPCALLBACK3FUNC pJob, LPVOID pParam, LPVOID pParam2, LPVOID pParam3);

            // Add a work thread to the threadlist.
            // return the thread index in threadlist. if the nNum param is not 1, the other
            // thread index is base on return value.
            static int AddWorkThread(LPCALLBACKFUNC pJob, LPVOID pParam, int nNum = 1);

		protected:

			static vector<thread*> m_pThreadList;
			static queue<ThreadParam*>	m_pJobList;
			static void ThreadWorkLoop(void);
			static mutex g_oJobListMutex;
			static mutex g_oRunJobMutex;
			static condition_variable g_oRunJobCV;
			static RUN_STATUS m_emStatus;
		};
	}
}

#endif // !THREADPOOL_H


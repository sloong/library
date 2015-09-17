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
			~ThreadParam()
			{
				SAFE_DELETE(pParam);
			}
		};

		class UNIVERSAL_API CThreadPool
		{
		public:
			CThreadPool();
			virtual ~CThreadPool();

			virtual void Initialize(int nThreadNum);

			virtual void Start();
			virtual void End();
			// Add a task to job list, and return the task index.
			virtual int AddTask(LPCALLBACKFUNC pJob, LPVOID pParam);
			// Add work thread 
			virtual thread* AddWorkThread(LPCALLBACKFUNC pJob, LPVOID pParam);

			// remove a task from job list.
			virtual void RemoveTask(int index);
			virtual int GetTaskTotal();
		protected:
			static vector<thread*> m_pThreadList;
			static queue<ThreadParam*>	m_pJobList;
			static void ThreadWorkLoop(void);
			static mutex g_oMutex;
		};
	}
}

#endif // !THREADPOOL_H


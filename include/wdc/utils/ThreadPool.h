/**
* Copyright 2016 IBM Corp. All Rights Reserved.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*      http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*
*/

#ifndef WDC_THREAD_POOL_H
#define WDC_THREAD_POOL_H

#include <list>

#include "Delegate.h"
#include "tinythread++/tinythread.h"
#include "WDCLib.h"

//! This class manages a pool of threads, and allows those threads to invoke functions back on the main thread.
class WDC_API ThreadPool 
{
public:
	//! Singleton instance
	static ThreadPool * Instance();

	//! Construction
	ThreadPool( int a_Threads = 20 );
	~ThreadPool();

	//! Invoke this function to queue the provided delegate to be invoked by one of the 
	//! background threads from the thread pool.
	template<typename ARG>
	void InvokeOnThread( Delegate<ARG> a_Callback, ARG a_Arg )
	{
		InvokeOnThread( new Callback<ARG>( a_Callback, a_Arg ) );
	}

	void InvokeOnThread( VoidDelegate a_Callback )
	{
		InvokeOnThread(new VoidCallback(a_Callback));
	}

	//! This function can be invoked from any thread to invoke a function on the main thread.
	template<typename ARG>
	void InvokeOnMain( Delegate<ARG> a_Callback, ARG a_Arg )
	{
		InvokeOnMain( new Callback<ARG>( a_Callback, a_Arg ) );
	}

	void InvokeOnMain( VoidDelegate a_Callback )
	{
		InvokeOnMain( new VoidCallback( a_Callback ) );
	}

	//! This function should be called by the main loop of the application to process any
	//! main thread invokes.
	void ProcessMainThread();
	//! This function runs the main thread until StopMainThread() is invoked.
	int RunMainThread();
	//! This is called to make ProcessMainThread() exit.
	void StopMainThread(int a_ExitCode = 0);

private:
	//! Types
	class ICallback
	{
	public:
		virtual ~ICallback()
		{}
		virtual void Invoke() = 0;
		virtual void Destroy() = 0;
		virtual const char * GetFile() const = 0;
		virtual int GetLine() const = 0;
	};

	class VoidCallback : public ICallback
	{
	public:
		VoidCallback()
		{}
		VoidCallback( VoidDelegate a_Delegate ) : m_Delegate( a_Delegate )
		{}

		virtual void Invoke()
		{
			m_Delegate();
		}
		virtual void Destroy()
		{
			delete this;
		}

		virtual const char * GetFile() const
		{
#if ENABLE_DELEGATE_DEBUG
			return m_Delegate.GetFile();
#else
			return "";
#endif
		}

		virtual int GetLine() const
		{
#if ENABLE_DELEGATE_DEBUG
			return m_Delegate.GetLine();
#else
			return 0;
#endif
		}

		VoidDelegate	m_Delegate;
	};

	template<typename ARG>
	class Callback : public ICallback
	{
	public:
		Callback()
		{}
		Callback( Delegate<ARG> a_Delegate, ARG a_Arg ) : m_Delegate( a_Delegate ), m_Arg( a_Arg )
		{}

		virtual void Invoke()
		{
			m_Delegate( m_Arg );
		}
		virtual void Destroy()
		{
			delete this;
		}

		virtual const char * GetFile() const
		{
#if ENABLE_DELEGATE_DEBUG
			return m_Delegate.GetFile();
#else
			return "";
#endif
		}

		virtual int GetLine() const
		{
#if ENABLE_DELEGATE_DEBUG
			return m_Delegate.GetLine();
#else
			return 0;
#endif
		}

		Delegate<ARG>	m_Delegate;
		ARG m_Arg;
	};
	typedef std::list<ICallback *>			DelegateList;
	typedef std::list<tthread::thread *>	ThreadList;
	typedef std::list<ThreadPool *>			PoolList;

	//! Functions
	void InvokeOnThread( ICallback * a_pCallback );
	void InvokeOnMain( ICallback * a_pCallback );

	//! Data
	volatile bool		m_StopMain;
	int					m_ThreadCount;
	ThreadList			m_Threads;
	int 				m_ExitCode;

	tthread::recursive_mutex		
						m_ThreadQueueLock;
	DelegateList		m_ThreadQueue;
	tthread::recursive_mutex
						m_MainQueueLock;
	DelegateList		m_MainQueue;

	tthread::condition_variable
						m_WakeThread;
	tthread::condition_variable
						m_WakeMain;
	volatile int		m_ActiveThreads;
	volatile int		m_BusyThreads;
	volatile bool		m_Shutdown;

	static void ThreadMain( void * arg );
	static ThreadPool *	sm_pInstance;
};

inline ThreadPool * ThreadPool::Instance()
{
	return sm_pInstance;
}

#endif


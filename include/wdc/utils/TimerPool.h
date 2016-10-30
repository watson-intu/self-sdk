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

#ifndef WDC_TIMERPOOL_H
#define WDC_TIMERPOOL_H

#include <set>

#include "boost/enable_shared_from_this.hpp"
#include "boost/shared_ptr.hpp"
#include "boost/thread.hpp"

#include "Time.h"
#include "ThreadPool.h"
#include "WDCLib.h"

//! This class manages one or more timers, invoking functions using the provided ThreadPool object.
class WDC_API TimerPool
{
public:
	//! Singleton instance
	static TimerPool * Instance();

	//! Types
	struct ITimer : public boost::enable_shared_from_this<ITimer>
	{
		//! Types
		typedef boost::shared_ptr<ITimer>		SP;
		typedef boost::weak_ptr<ITimer>			WP;

		//! Construction
		ITimer( double a_Interval, bool a_invokeOnMain, bool a_Recurring ) :
			m_Interval( a_Interval ),
			m_InvokeOnMain( a_invokeOnMain ),
			m_Recurring( a_Recurring ),
			m_NextSignal( Time().GetEpochTime() + a_Interval )
		{}
		virtual ~ITimer()
		{}

		virtual void Invoke() = 0;

		TimerPool *		m_pPool;
		double			m_Interval;
		bool			m_InvokeOnMain;
		bool			m_Recurring;
		Time			m_NextSignal;
	};

	//! Construction
	TimerPool();
	~TimerPool();

	//! Invoke this function to queue the provided delegate to be invoked by one of the 
	//! background threads from the thread pool.
	//! a_Callback .. The function to invoke when the timer fires.
	//! a_Arg .. The argument to pass to the function.
	//! a_Interval .. When to call the function, in seconds.
	//! a_InvokeOnMain .. If false, then the function will be invoked on a background thread.
	//! a_Recurring .. If true, the timer will keep invoking.
	template<typename ARG>
	ITimer::SP StartTimer( Delegate<ARG> a_Callback, ARG a_Arg, double a_Interval, bool a_InvokeOnMain, bool a_Recurring )
	{
		ITimer::SP spNewTimer( new Timer<ARG>( a_Callback, a_Arg, a_Interval, a_InvokeOnMain, a_Recurring ) );
		InsertTimer( spNewTimer, true );

		return spNewTimer;
	}

	ITimer::SP StartTimer( VoidDelegate a_Callback, double a_Interval, bool a_InvokeOnMain, bool a_Recurring )
	{
		ITimer::SP spNewTimer( new VoidTimer( a_Callback, a_Interval, a_InvokeOnMain, a_Recurring ) );
		InsertTimer( spNewTimer, true );

		return spNewTimer;
	}

	//! Stop the given timer.
	bool StopTimer( ITimer::SP a_pTimer );
	//! Stop all Timers
	void StopAllTimers();


private:
	//! Types
	// Timer struct to enable the sorting of weak pointers within multiset
	struct TimerMultiSetStruct
	{
		TimerMultiSetStruct( ITimer::SP a_pTimer )
		{
			m_NextSignalEpochTime = a_pTimer->m_NextSignal.GetEpochTime();
			m_pTimer = a_pTimer;
		}
		ITimer::WP 		m_pTimer;
		double 			m_NextSignalEpochTime;
	};

	struct TimerCompare {
		bool operator() (const TimerMultiSetStruct & a_Timer1, const TimerMultiSetStruct & a_Timer2) const {
			return a_Timer1.m_NextSignalEpochTime < a_Timer2.m_NextSignalEpochTime;
		}
	};

	typedef std::multiset<TimerMultiSetStruct, TimerCompare, std::allocator<TimerMultiSetStruct> >				TimerMultiSet;

	template<typename ARG>
	struct Timer : public ITimer
	{
		Timer( Delegate<ARG> a_Delegate, ARG a_Arg, double a_Interval, bool a_invokeOnMain, bool a_Recurring ) :
			ITimer( a_Interval, a_invokeOnMain, a_Recurring ), 
			m_Delegate( a_Delegate ), 
			m_Arg( a_Arg )
		{}

		virtual void Invoke()
		{
			if (m_Delegate.IsValid())
				m_Delegate(m_Arg);
		}

		Delegate<ARG>	m_Delegate;
		ARG				m_Arg;
	};

	struct VoidTimer : public ITimer
	{
		VoidTimer( VoidDelegate a_Delegate, double a_Interval, bool a_invokeOnMain, bool a_Recurring ) :
			ITimer(a_Interval, a_invokeOnMain, a_Recurring),
			m_Delegate( a_Delegate )
		{}

		virtual void Invoke()
		{
			if (m_Delegate.IsValid())
				m_Delegate();
		}

		VoidDelegate m_Delegate;
	};

	void InsertTimer( ITimer::SP a_pTimer, bool newTimer );
	void InvokeTimer(ITimer::WP a_wpTimer);

	static void TimerThread( void * arg );

	//! Data
	volatile bool		m_bShutdown;
	boost::thread *		m_pTimerThread;
	boost::mutex		m_TimerQueueLock;
	TimerMultiSet		m_TimerQueue;
	boost::condition_variable
						m_WakeTimer;
	static TimerPool *	sm_pInstance;

};

inline TimerPool * TimerPool::Instance()
{
	return sm_pInstance;
}

#endif


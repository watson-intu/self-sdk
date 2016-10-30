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

#ifndef UNIT_TEST_H
#define UNIT_TEST_H

#include <list>
#include <vector>
#include <string>
#include <stdio.h>

#include <boost/thread.hpp>

#include "utils/WatsonException.h"
#include "utils/Time.h"
#include "utils/ThreadPool.h"

#include "WDCLib.h"		// include last always

//! Unit test base class for writing tests of the SELF library.
class WDC_API UnitTest 
{
public:
	//! Construction
	UnitTest( const char * a_pTestName ) :
		m_TestName( a_pTestName )
	{
		GetTestList().push_back( this );
	}
	virtual ~UnitTest()
	{
		GetTestList().remove( this );
	}

	const std::string & GetName() const
	{
		return m_TestName;
	}

	static void Test( bool condition )
	{
		if (! condition )
			throw WatsonException( "Test condition failed");
	}

	void Spin( int & a_Count, int a_Amount, double a_Timeout = 30.0 )
	{
		Time start;
		while( (Time().GetEpochTime() - start.GetEpochTime()) < a_Timeout && a_Count < a_Amount )
		{
			ThreadPool::Instance()->ProcessMainThread();
			boost::this_thread::sleep( boost::posix_time::milliseconds(1) );
		}
	}

	void Spin( bool & a_TestCondition, double a_Timeout = 30.0 )
	{
		Time start;
		while( (Time().GetEpochTime() - start.GetEpochTime()) < a_Timeout && !a_TestCondition )
		{
			ThreadPool::Instance()->ProcessMainThread();
			boost::this_thread::sleep( boost::posix_time::milliseconds(1) );
		}
	}

	virtual void RunTest() = 0;

	//! Run one or more tests, optionally provide a name to filter tests. Returns 0 on success, otherwise it returns the number of tests that failed.
	static int RunTests( const std::vector<std::string> & a_Tests );

private:
	//! Types
	typedef std::list<UnitTest *> TestList;

	//! Data
	const std::string 	m_TestName;

	static TestList & GetTestList()
	{
		// declare a static pointer, we don't want this to get destroyed
		// during global cleanup otherwise we can crash on exit based on 
		// an unknown order in which static data is destroyed. By using the heap,
		// we ensure our list doesn't get destroyed.
		static TestList * LIST = new TestList();
		return *LIST;
	}
};

#endif

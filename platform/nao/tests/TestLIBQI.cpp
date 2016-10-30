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

#include "tests/UnitTest.h"
#include "utils/Log.h"

#include "qi/session.hpp"
#include "qi/applicationsession.hpp"

#ifndef _WIN32

class TestLIBQI : UnitTest
{
public:
	//! Construction
	TestLIBQI() : UnitTest( "TestLIBQI" )
	{}

	virtual void RunTest()
	{
		try {

			boost::shared_ptr<qi::Session> spSession( new qi::Session() );
			//spSession->connect( "tcp://192.168.1.184" );
			spSession->connect( "tcp://127.0.0.1:51033" );
			//spSession->connect( "tcp://127.0.0.1:9559" );

			qi::FutureSync< std::vector<qi::ServiceInfo> > services = spSession->services();
			for( size_t i=0;i<services.value().size();++i)
			{
				Log::Debug( "TestLIBQI", "ServiceInfo: name: %s", services.value()[i].name().c_str() );
			}

			qi::AnyObject tts = spSession->service( "ALTextToSpeech" );

			qi::AnyObject motion = spSession->service("ALMotion");
			qi::Future<void> sayOp = tts.async<void>( "say", "Hello world" );
			qi::Future<void> moveOp = motion.async<void>("moveTo",1,0,0 );

			sayOp.wait();
			moveOp.wait();

			spSession->close();
		}
		catch( std::exception ex )
		{
			Log::Error( "TestLIBQI", "Caught Exception: %s", ex.what() );
		}
		catch( ... )
		{
			Log::Error( "TestLIBQI", "Caught unhanded exception." );
		}

	}


};

TestLIBQI TEST_LIBQI;

#endif



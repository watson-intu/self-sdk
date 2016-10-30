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

#include "NaoGaze.h"
#include "SelfInstance.h"
#include "utils/Log.h"
#include "utils/TimerPool.h"
#include "utils/SelfException.h"

REG_SERIALIZABLE(NaoGaze);
RTTI_IMPL(NaoGaze, ISensor);


void NaoGaze::Serialize(Json::Value & json)
{
    ISensor::Serialize(json);

    json["m_Tolerance"] = m_Tolerance;
}

void NaoGaze::Deserialize(const Json::Value & json)
{
    ISensor::Deserialize(json);

    if (json.isMember("m_Tolerance"))
        m_Tolerance = json["m_Tolerance"].asFloat();
}

bool NaoGaze::OnStart()
{
    Log::Status("NaoGaze", "NaoGaze started");

    ThreadPool::Instance()->InvokeOnThread<void *>(DELEGATE(NaoGaze, ReceiveData, void *, this), NULL);
    return true;
}

bool NaoGaze::OnStop()
{
    Log::Status("NaoGaze", "NaoGaze stopped");
    m_Memory.reset();
#ifndef _WIN32
    m_pGaze->unsubscribe("myApplication");
    delete m_pGaze;
    m_pGaze = NULL;
#endif
    return true;
}

void NaoGaze::ReceiveData(void * arg)
{
    try {
        DoReceiveData(arg);
    }
    catch( const std::exception & ex )
    {
        Log::Error( "NaoGaze", "Caught Exception: %s", ex.what() );
    }
}

void NaoGaze::DoReceiveData(void *)
{
#ifndef _WIN32
    m_Memory = NaoPlatform::Instance()->GetSession()->service("ALMemory");
    std::string robotIp("127.0.0.1");
    if ( SelfInstance::GetInstance() != NULL )
        robotIp = SelfInstance::GetInstance()->GetRobotIp();

    m_pGaze = new  AL::ALGazeAnalysisProxy(robotIp);
    m_pGaze->subscribe("myApplication");
    Log::Debug("NaoGaze", "Tolerance currently set to %f", m_pGaze->getTolerance());
    m_pGaze->setTolerance(m_Tolerance);

    Log::Debug("NaoGaze", "Tolerance currently set to %f", m_pGaze->getTolerance());
    m_PersonStartsLooking = m_Memory.call<qi::AnyObject>( "subscriber", "GazeAnalysis/PersonStartsLookingAtRobot" );
    m_PersonStopsLooking = m_Memory.call<qi::AnyObject>( "subscriber", "GazeAnalysis/PersonStopsLookingAtRobot" );
//    m_PeopleStartLooking = m_Memory.call<qi::AnyObject>( "subscriber", "GazeAnalysis/PeopleLookingAtRobot" );
    m_PersonStartsLooking.connect("signal", qi::AnyFunction::fromDynamicFunction( boost::bind( &NaoGaze::OnPersonStartsLooking, this, _1 ) ) );
    m_PersonStopsLooking.connect("signal", qi::AnyFunction::fromDynamicFunction( boost::bind( &NaoGaze::OnPersonStopsLooking, this, _1 ) ) );
//    m_PeopleStartLooking.connect("signal", qi::AnyFunction::fromDynamicFunction( boost::bind( &NaoGaze::OnPersonStartsLooking, this, _1 ) ) );
#endif
}

qi::AnyReference NaoGaze::OnPersonStartsLooking(const std::vector <qi::AnyReference> &recognizeInfo)
{
    try
    {
        return DoOnRecognized(true);
    }
    catch( const std::exception & ex )
    {
        Log::Error( "NaoGaze", "Caught Exception: %s", ex.what() );
    }

    return qi::AnyReference();
}

qi::AnyReference NaoGaze::OnPersonStopsLooking(const std::vector <qi::AnyReference> &recognizeInfo)
{
    try
    {
        return DoOnRecognized(false);
    }
    catch( const std::exception & ex )
    {
        Log::Error( "NaoGaze", "Caught Exception: %s", ex.what() );
    }

    return qi::AnyReference();
}

qi::AnyReference NaoGaze::DoOnRecognized(bool a_IsPersonLooking)
{
    ThreadPool::Instance()->InvokeOnMain<GazeData *>(DELEGATE(NaoGaze, SendingData, GazeData * , this),
                                                      new GazeData( a_IsPersonLooking ));

    return qi::AnyReference();
}

void NaoGaze::SendingData(GazeData * a_pData)
{
    SendData(a_pData);
}

void NaoGaze::OnPause()
{}

void NaoGaze::OnResume()
{}

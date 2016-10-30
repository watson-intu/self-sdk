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

#include "NaoGestureSensor.h"
#include "NaoPlatform.h"
#include "SelfInstance.h"
#include "utils/Log.h"
#include "utils/TimerPool.h"
#include "utils/SelfException.h"
#include "utils/AlHelpers.h"
#include "utils/QiHelpers.h"
#include <string.h>
#include <sstream>

REG_SERIALIZABLE(NaoGestureSensor);
REG_OVERRIDE_SERIALIZABLE(GestureSensor, NaoGestureSensor);
RTTI_IMPL(NaoGestureSensor, GestureSensor);

void NaoGestureSensor::Serialize(Json::Value & json)
{
    json["m_MaxWaveDetectionRange"] = m_MaxWaveDetectionRange;
    json["m_WaveThreshold"] = m_WaveThreshold;
}

void NaoGestureSensor::Deserialize(const Json::Value & json)
{
    if (json.isMember("m_MaxWaveDetectionRange"))
        m_MaxWaveDetectionRange = json["m_MaxWaveDetectionRange"].asFloat();
    if (json.isMember("m_WaveThreshold"))
        m_WaveThreshold = json["m_WaveThreshold"].asFloat();
}

bool NaoGestureSensor::OnStart()
{
    Log::Status("NaoGestureSensor", "NaoGestureSensor started");

    ThreadPool::Instance()->InvokeOnThread<void *>(DELEGATE(NaoGestureSensor, ReceiveData, void *, this), NULL);
    return true;
}

bool NaoGestureSensor::OnStop()
{
    Log::Debug("NaoGestureSensor", "NaoGestureSensor stopped");
    m_Memory.reset();
#ifndef _WIN32
    m_pWave->unsubscribe("wave");
    m_pPeoplePerception->unsubscribe("myApplication2");
    m_pWave = NULL;
    m_pPeoplePerception = NULL;
#endif
    return true;
}

void NaoGestureSensor::ReceiveData(void * arg)
{
    try {
        DoReceiveData(arg);
    }
    catch( const std::exception & ex )
    {
        Log::Error( "NaoGestureSensor", "Caught Exception: %s", ex.what() );
    }
}

void NaoGestureSensor::DoReceiveData(void *)
{
    Log::Debug( "NaoGestureSensor", "Inside DoReceiveData");

    m_Memory = NaoPlatform::Instance()->GetSession()->service("ALMemory");
    std::string robotIp("127.0.0.1");
    if ( SelfInstance::GetInstance() != NULL )
        robotIp = SelfInstance::GetInstance()->GetRobotIp();

#ifndef _WIN32
    m_pPeoplePerception = new  AL::ALPeoplePerceptionProxy(robotIp);
    m_pPeoplePerception->subscribe("myApplication1");
    m_pPeoplePerception->setFastModeEnabled(true);
#endif

    m_PeopleDetected = m_Memory.call<qi::AnyObject>( "subscriber", "PeoplePerception/PeopleDetected");
    m_PeopleDetected.connect("signal", qi::AnyFunction::fromDynamicFunction( boost::bind( &NaoGestureSensor::OnPeopleDetected, this, _1 ) ) );

#ifndef _WIN32
	m_pWave = new  AL::ALWavingDetectionProxy(robotIp);
    m_pWave->subscribe("myApplication2");
    m_pWave->setMaxDistance(m_MaxWaveDetectionRange);
    m_pWave->setMinSize(0.05);
    Log::Debug("NaoGestureSensor", "Max Wave Detection Range set to: %.2f", m_pWave->getMaxDistance());
#endif

    m_PersonWaving = m_Memory.call<qi::AnyObject>( "subscriber", "WavingDetection/Waving" );
    m_PersonWaving.connect("signal", qi::AnyFunction::fromDynamicFunction( boost::bind( &NaoGestureSensor::OnHandWaveDetected, this, _1 ) ) );
}

qi::AnyReference NaoGestureSensor::OnPeopleDetected(const std::vector <qi::AnyReference> &args)
{
    //Log::Debug("NaoGestureSensor", "Received event from People Detected");
	return qi::AnyReference();
}

qi::AnyReference NaoGestureSensor::OnHandWaveDetected(const std::vector <qi::AnyReference> &args)
{
    Log::Debug( "NaoGestureSensor", "OnHandWaveDetected");
    try
    {
        return DoOnHandWaveDetected(args);
    }
    catch( const std::exception & ex )
    {
        Log::Error( "NaoGestureSensor", "OnHandWaveDetected - Caught Exception: %s", ex.what() );
    }
	return qi::AnyReference();
}

qi::AnyReference NaoGestureSensor::DoOnHandWaveDetected(const std::vector <qi::AnyReference> &args)
{
    if(m_isWaving)
    {
        m_isWaving = false;
        ThreadPool::Instance()->InvokeOnMain<GestureData *>(
                DELEGATE(NaoGestureSensor, SendingData, GestureData * , this),
                new GestureData("wave_anim"));
        Log::Debug("NaoGestureSensor", "Gesture Data Set - Invoking on Main");
        m_spTimer = TimerPool::Instance()->StartTimer( VOID_DELEGATE(NaoGestureSensor, OnTimer, this), 5.0f, false, false );
    }
    return qi::AnyReference();
}

void NaoGestureSensor::SendingData(GestureData * a_pData)
{
    Log::Debug("NaoGestureSensor", "Sending Wave data to GestureExtractor!");
    SendData(a_pData);
}

void NaoGestureSensor::OnTimer()
{
    m_isWaving = true;
}

void NaoGestureSensor::OnPause()
{}

void NaoGestureSensor::OnResume()
{}


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

#include "NaoSonar.h"
#include "SelfInstance.h"
#include "utils/Log.h"
#include "utils/TimerPool.h"
#include "utils/SelfException.h"

#ifndef _WIN32
#include <alproxies/alsonarproxy.h>
#endif

#include <stdio.h>

#ifndef _WIN32
REG_SERIALIZABLE(NaoSonar);
REG_OVERRIDE_SERIALIZABLE( Sonar, NaoSonar);
#endif
RTTI_IMPL(NaoSonar, Sonar);

void NaoSonar::Serialize(Json::Value & json)
{
    Sonar::Serialize(json);
    SerializeVector("m_SonarSensors", m_SonarSensors, json);
    json["m_Interval"] = m_Interval;
}

void NaoSonar::Deserialize(const Json::Value & json)
{
    Sonar::Deserialize(json);
    DeserializeVector("m_SonarSensors", json, m_SonarSensors);
    if ( json.isMember("m_Interval") )
        m_Interval = json["m_Interval"].asFloat();  
}

bool NaoSonar::OnStart()
{
    Log::Status("NaoSonar", "NaoSonar started: Sampling from %d sonars every %.2f seconds", m_SonarSensors.size(), m_Interval);
    m_spTimer = TimerPool::Instance()->StartTimer( VOID_DELEGATE(NaoSonar, OnGetData, this), m_Interval, false, true );
    return true;
}

bool NaoSonar::OnStop()
{
    Log::Status("NaoSonar", "NaoSonar stopped");
    m_Memory.reset();
    return true;
}

void NaoSonar::OnPause()
{
    m_bPaused = true;
}

void NaoSonar::OnResume()
{
    m_bPaused = false;
}

void NaoSonar::OnGetData()
{
    try {
        DoGetData();
    }
    catch( const std::exception & ex )
    {
        Log::Error( "NaoSonar", "Caught Exception: %s", ex.what() );
    }
}

void NaoSonar::DoGetData()
{
    m_Memory = NaoPlatform::Instance()->GetSession()->service("ALMemory");
    for (size_t i = 0; i < m_SonarSensors.size(); i++)
    {
        qi::AnyValue val = m_Memory.call<qi::AnyValue>("getData", m_SonarSensors[i]);
        if (! m_bPaused )
        {
            ThreadPool::Instance()->InvokeOnMain( 
                DELEGATE(NaoSonar, SendSonarData, SonarData *, this),
                new SonarData( val.as<float>() ) );        
        }
    }
}

void NaoSonar::SendSonarData( SonarData * a_pData )
{
    ISensor::SendData( a_pData );
}

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

#include "NaoLaser.h"
#include "SelfInstance.h"
#include "utils/Log.h"
#include "utils/TimerPool.h"
#include "utils/SelfException.h"

#ifndef _WIN32
    #include <alproxies/allaserproxy.h>
#endif

#include <stdio.h>

#ifndef _WIN32
REG_SERIALIZABLE( NaoLaser );
REG_OVERRIDE_SERIALIZABLE( Laser, NaoLaser );
#endif

RTTI_IMPL( NaoLaser, Laser );

void NaoLaser::Serialize(Json::Value & json)
{
    Laser::Serialize(json);
    SerializeVector("m_LaserSensors", m_LaserSensors, json);
    json["m_Interval"] = m_Interval;
    json["m_DistanceThreshold"] = m_DistanceThreshold;
}

void NaoLaser::Deserialize(const Json::Value & json)
{
    Laser::Deserialize(json);
    DeserializeVector("m_LaserSensors", json, m_LaserSensors);
    if ( json.isMember("m_Interval") )
        m_Interval = json["m_Interval"].asFloat();
    if ( json.isMember("m_DistanceThreshold") )
        m_DistanceThreshold = json["m_DistanceThreshold"].asFloat();
}

bool NaoLaser::OnStart()
{
    try
    {
        ConfigureLasers();
        Log::Status("NaoLaser", "NaoLaser started: Sampling from %d Lasers every %.2f seconds", m_LaserSensors.size(), m_Interval);
    }
    catch( const std::exception & ex )
    {
        Log::Error( "NaoLaser", "Caught Exception on configure: %s", ex.what() );
    }
    
    m_Memory = NaoPlatform::Instance()->GetSession()->service("ALMemory");

    m_spTimer = TimerPool::Instance()->StartTimer( VOID_DELEGATE(NaoLaser, OnGetData, this), m_Interval, false, true );
    return true;
}

bool NaoLaser::OnStop()
{
    Log::Status("NaoLaser", "NaoLaser stopped");
    m_Memory.reset();
    m_spTimer.reset();
    return true;
}

void NaoLaser::OnPause()
{
    m_bPaused = true;
}

void NaoLaser::OnResume()
{
    m_bPaused = false;
}

void NaoLaser::OnGetData()
{
    try
    {
        DoGetData();
    }
    catch( const std::exception & ex )
    {
        Log::Error( "NaoLaser", "Caught Exception: %s", ex.what() );
    }
}

void NaoLaser::DoGetData()
{
    m_Memory = NaoPlatform::Instance()->GetSession()->service("ALMemory");
    for (size_t i = 0; i < m_LaserSensors.size(); i++)
    {
        qi::AnyValue val = m_Memory.call<qi::AnyValue>("getData", m_LaserSensors[i]);

        if (! m_bPaused && val.as<float>() < m_DistanceThreshold)
        {
            Log::Debug("NaoLaser", "Key: %s, val: %.3f", m_LaserSensors[i].c_str(), val.as<float>() );
            ThreadPool::Instance()->InvokeOnMain( 
                DELEGATE(NaoLaser, SendLaserData, LaserData *, this),
                    new LaserData( val.as<float>(), 0, 0 ) );
            break;
        }
    }
}

void NaoLaser::SendLaserData( LaserData * a_pData )
{
    ISensor::SendData( a_pData );
}

void NaoLaser::ConfigureLasers()
{
    // Turn on Lasers
    std::string robotIp("127.0.0.1");
	SelfInstance * pInstance = SelfInstance::GetInstance();
	if ( pInstance != NULL )
		robotIp = pInstance->GetRobotIp();
#ifndef _WIN32
    AL::ALLaserProxy laser(robotIp, 9559);
    laser.laserON();
    laser.setDetectingLength(20, 1500);
#endif
}
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

#ifndef SELF_NAOLASER_H
#define SELF_NAOLASER_H

#include "qi/anyobject.hpp"
#include "NaoPlatform.h"
#include "sensors/Laser.h"

class NaoLaser : public Laser
{
public:
    RTTI_DECL();

    //! Construction
    NaoLaser() : Laser(),
        m_Interval(2.0),
        m_bPaused(false)
    {}

    //! ISerializable interface
	virtual void Serialize(Json::Value & json);
	virtual void Deserialize(const Json::Value & json);

    //! ISensor interface
    virtual const char * GetSensorName() 
	{
        return "Laser";
    }

    virtual bool OnStart();
    virtual bool OnStop();
    virtual void OnPause();
    virtual void OnResume();

private:

    //! Members
    bool                        m_bPaused;
    std::vector<std::string>    m_LaserSensors;
    float                       m_Interval;
    float                       m_DistanceThreshold;
    TimerPool::ITimer::SP       m_spTimer;
    qi::AnyObject               m_Memory;
    
    void OnGetData();
    void DoGetData();
    void SendLaserData( LaserData * a_pData );
    void ConfigureLasers();

};

#endif //SELF_NAOLASER_H

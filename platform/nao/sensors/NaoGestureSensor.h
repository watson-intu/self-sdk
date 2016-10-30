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

#ifndef SELF_NAOGESTURESENSOR_H
#define SELF_NAOGESTURESENSOR_H

#include "NaoPlatform.h"
#include "sensors/ISensor.h"
#include "sensors/GestureData.h"
#include "sensors/GestureSensor.h"

#ifndef _WIN32
#include <alproxies/alwavingdetectionproxy.h>
#include <alproxies/alpeopleperceptionproxy.h>
#endif

class NaoGestureSensor : public GestureSensor
{
public:
    RTTI_DECL();

    NaoGestureSensor() : m_MaxWaveDetectionRange(0.5f), m_WaveThreshold(0.5), m_isWaving(true)
    {}

    //! ISensor interface
    virtual const char * GetSensorName()
    {
        return "NaoGestureSensor";
    }


    virtual bool OnStart();
    virtual bool OnStop();
    virtual void OnPause();
    virtual void OnResume();

    //! ISerializable interface
    virtual void Serialize(Json::Value & json);
    virtual void Deserialize(const Json::Value & json);


    qi::AnyReference OnHandWaveDetected(const std::vector <qi::AnyReference> &recognizeInfo);
    qi::AnyReference DoOnHandWaveDetected(const std::vector <qi::AnyReference> &recognizeInfo);
    qi::AnyReference OnPeopleDetected(const std::vector <qi::AnyReference> &args);


private:

    qi::AnyObject       m_Memory;
    qi::AnyObject       m_PersonWaving;
    qi::AnyObject       m_PeopleDetected;

    TimerPool::ITimer::SP       m_spTimer;

#ifndef _WIN32
    AL::ALPeoplePerceptionProxy * m_pPeoplePerception;
    AL::ALWavingDetectionProxy * m_pWave;
#endif

    float               m_Tolerance;
    float               m_MaxWaveDetectionRange;
    double              m_WaveThreshold;
    bool                m_isWaving;

    void                ReceiveData(void *);
    void                DoReceiveData( void * );
    void                SendingData(GestureData *a_pData);
    void                OnTimer();
    qi::AnyReference    DoOnRecognized(bool a_IsPersonLooking);
};

#endif //SELF_NAOGAZE_H

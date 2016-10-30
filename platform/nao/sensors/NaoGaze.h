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

#ifndef SELF_NAOGAZE_H
#define SELF_NAOGAZE_H

#include "qi/anyobject.hpp"
#include "NaoPlatform.h"
#include "sensors/ISensor.h"
#include "sensors/GazeData.h"

#ifndef _WIN32
#include <alproxies/algazeanalysisproxy.h>
#endif

class NaoGaze : public ISensor
{
public:
    RTTI_DECL();

    NaoGaze() 
#ifndef _WIN32
		: m_pGaze(NULL), m_Tolerance (0.8f)
#endif
	{}

    //! ISensor interface
    virtual const char * GetSensorName()
    {
        return "Gaze";
    }

    virtual const char * GetDataType()
    {
        return GazeData::GetStaticRTTI().GetName().c_str();
    }

    virtual bool OnStart();
    virtual bool OnStop();
    virtual void OnPause();
    virtual void OnResume();

    //! ISerializable interface
    virtual void Serialize(Json::Value & json);
    virtual void Deserialize(const Json::Value & json);


    qi::AnyReference OnPersonStartsLooking(const std::vector <qi::AnyReference> &recognizeInfo);
    qi::AnyReference OnPersonStopsLooking(const std::vector <qi::AnyReference> &recognizeInfo);

private:

    qi::AnyObject       m_Memory;
    qi::AnyObject       m_PersonStartsLooking;
    qi::AnyObject       m_PersonStopsLooking;
    qi::AnyObject       m_PeopleStartLooking;
#ifndef _WIN32
    AL::ALGazeAnalysisProxy * m_pGaze;
#endif

    float               m_Tolerance;

    void                ReceiveData(void *);
    void                DoReceiveData( void * );
    void                SendingData(GazeData *a_pData);
    qi::AnyReference    DoOnRecognized(bool a_IsPersonLooking);
};

#endif //SELF_NAOGAZE_H

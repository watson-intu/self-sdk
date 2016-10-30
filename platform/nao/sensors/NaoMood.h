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

#ifndef SELF_NAOMOOD_H
#define SELF_NAOMOOD_H

#include "qi/anyobject.hpp"
#include "NaoPlatform.h"
#include "sensors/ISensor.h"
#include "sensors/MoodData.h"

class NaoMood : public ISensor 
{
public:
    RTTI_DECL();

    //! Construction
    NaoMood()
    {}

    //! ISensor interface
    virtual const char *GetSensorName() 
	{
        return "Mood";
    }

    virtual const char * GetDataType()
    {
        return MoodData::GetStaticRTTI().GetName().c_str();
    }

    virtual bool OnStart();
    virtual bool OnStop();
    virtual void OnPause();
    virtual void OnResume();

    qi::AnyReference OnPersonSmiling(const std::vector <qi::AnyReference> &recognizeInfo);


private:


    qi::AnyObject       m_Memory;
    qi::AnyObject       m_PersonSmiling;

    void                ReceiveData(void *);
    void                DoReceiveData( void * );
    void                SendingData(MoodData *a_pData);
    qi::AnyReference    DoOnRecognized(const std::vector <qi::AnyReference> & args);
};

#endif //SELF_NAOMOOD_H

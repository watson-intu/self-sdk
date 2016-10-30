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


#ifndef SELF_TONEANALYZER_H
#define SELF_TONEANALYZER_H

#include "utils/Delegate.h"
#include "utils/DataCache.h"
#include "services/IService.h"
#include "DataModels.h"
#include "WDCLib.h"

class WDC_API ToneAnalyzer : public IService
{
public:
    RTTI_DECL();

    //! Types
    typedef Delegate<DocumentTones *>	OnMessage;

    //! Construction
    ToneAnalyzer();

    //! ISerializable
    virtual void Serialize(Json::Value & json);
    virtual void Deserialize(const Json::Value & json);

    //! IService interface
    virtual bool Start();

    void GetTone( const std::string & a_Text, OnMessage a_Callback );

private:

    std::string m_Version;

};


#endif //SELF_TONEANALYZER_H

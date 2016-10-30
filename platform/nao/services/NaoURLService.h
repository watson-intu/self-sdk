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

#ifndef SELF_NAOURLSERVICE_H
#define SELF_NAOURLSERVICE_H

#include "services/IService.h"
#include "blackboard/URL.h"
#include "SelfLib.h"
#include "services/URLService.h"
#include "SelfInstance.h"
#include "qi/session.hpp"
#include "NaoPlatform.h"

class NaoURLService : public URLService
{
public:
    RTTI_DECL();

    //! Construction
    NaoURLService();

    //! IService
    virtual bool Start();

    //! ISerializable
    virtual void Serialize(Json::Value & json);
    virtual void Deserialize(const Json::Value & json);

    //! Upload the specified dialog
    virtual void SendURL( const Url::SP & a_spUrlAgent, UrlCallback a_Callback );

    qi::AnyReference OnTouchData(const std::vector <qi::AnyReference> & args);    
    
private:
    void ConfigureTablet();
    void OnCheckConnection();
    void OnDisplayLogo();

    void ReceiveData(void * arg);
    void DoReceiveData(void * arg);

    //! Members
    qi::AnyObject           m_Tablet;
    qi::AnyObject           m_Memory;
    qi::AnyObject           m_Signal;
    qi::AnyObject           m_Object;
    bool                    m_bTabletConnected;
    bool                    m_bDisplayLogo;
    float                   m_TabletCheckInterval;
    float                   m_TabletDisplayTime;
    float                   m_fBrightness;
    std::string             m_Security;
    std::string             m_SSID;
    std::string             m_Password;
    TimerPool::ITimer::SP   m_spTabletCheckTimer;
    TimerPool::ITimer::SP   m_spTabletDisplayTimer;
};

#endif

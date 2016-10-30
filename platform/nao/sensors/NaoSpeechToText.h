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

#ifndef NAO_SPEECH_TO_TEXT
#define NAO_SPEECH_TO_TEXT

#include "qi/anyobject.hpp"
#include "NaoPlatform.h"
#include "sensors/LocalSpeechToText.h"

#ifndef _WIN32
#include <alproxies/alspeechrecognitionproxy.h>
#endif

//! This ISensor gets audio data from the Nao internal speech to text input.
class NaoSpeechToText : public LocalSpeechToText
{
public:
    RTTI_DECL();

    //! Construction
    NaoSpeechToText() : m_Paused( 0 )
    {}

    //! ISensor interface
    virtual const char * GetSensorName()
    {
        return "SpeechToText";
    }

    virtual bool OnStart();
    virtual bool OnStop();
    virtual void OnPause();
    virtual void OnResume();

    qi::AnyReference    OnRecognized( const std::vector<qi::AnyReference> & recognizeInfo );

private:
    //! Data
    volatile int 		m_Paused;

    qi::AnyObject       m_Memory;
    qi::AnyObject       m_Recognized;

#ifndef _WIN32
    AL::ALSpeechRecognitionProxy * m_pAsr;
#endif

    void                ReceiveData( void * );
    void                DoReceiveData( void * );
    void				SendingData( TextData * a_pData );
    qi::AnyReference    DoOnRecognized(const std::vector<qi::AnyReference> & recognizeInfo );
};

#endif

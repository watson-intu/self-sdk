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

#include "NaoSpeechToText.h"
#include "SelfInstance.h"
#include "utils/Log.h"
#include "utils/TimerPool.h"
#include "utils/SelfException.h"

#include <stdio.h>

#ifndef _WIN32
REG_SERIALIZABLE(NaoSpeechToText);
REG_OVERRIDE_SERIALIZABLE( LocalSpeechToText, NaoSpeechToText);
#endif
RTTI_IMPL(NaoSpeechToText, LocalSpeechToText);

bool NaoSpeechToText::OnStart()
{
    Log::Status("NaoSpeechToText", "NaoSpeechToText started");
    m_Paused = 0;

    ThreadPool::Instance()->InvokeOnThread<void *>(DELEGATE(NaoSpeechToText, ReceiveData, void *, this), NULL);
    return true;
}

bool NaoSpeechToText::OnStop()
{
	Log::Status("NaoSpeechToText", "NaoSpeechToText stopped");
	m_Memory.reset();
#ifndef _WIN32
	m_pAsr->unsubscribe("WordRecognized");
	delete m_pAsr;
	m_pAsr = NULL;
#endif
    return true;
}

void NaoSpeechToText::ReceiveData(void * arg)
{
    try {
        DoReceiveData(arg);
    } 
	catch( const std::exception & ex )
    {
        Log::Error( "NaoSpeechToText", "Caught Exception: %s", ex.what() );
    }
}

void NaoSpeechToText::DoReceiveData(void *)
{
#ifndef _WIN32
	m_Memory = NaoPlatform::Instance()->GetSession()->service("ALMemory");

	std::string robotIp("127.0.0.1");
	SelfInstance * pInstance = SelfInstance::GetInstance();
	if ( pInstance != NULL )
		robotIp = pInstance->GetRobotIp();

	while(!NaoPlatform::Instance()->IsAutonomousLifeStarted())
		tthread::this_thread::sleep_for( tthread::chrono::milliseconds( 1000 ) );

    m_pAsr = new AL::ALSpeechRecognitionProxy(robotIp.c_str());

	m_pAsr->pause( true );
	m_pAsr->setWordListAsVocabulary(m_VocabularyList);
	m_pAsr->setLanguage("English");
	m_pAsr->pause( false );
	m_pAsr->setAudioExpression(false);
	m_pAsr->setVisualExpression(false);
	m_pAsr->subscribe("WordRecognized");

	m_Recognized = m_Memory.call<qi::AnyObject>("subscriber", "WordRecognized" );
	m_Recognized.connect("signal", qi::AnyFunction::fromDynamicFunction( boost::bind( &NaoSpeechToText::OnRecognized, this, _1 ) ) );
#endif
}

qi::AnyReference NaoSpeechToText::OnRecognized( const std::vector<qi::AnyReference> & args )
{
	try
	{
		return DoOnRecognized(args);
	}
	catch( const std::exception & ex )
	{
		Log::Error( "NaoSpeechToText", "Caught Exception: %s", ex.what() );
	}

	return qi::AnyReference();
}

qi::AnyReference NaoSpeechToText::DoOnRecognized(const std::vector <qi::AnyReference> & args)
{
	qi::AnyReference arg = args[0].content();

	std::string word = arg[0].content().asString();
	float confidence = arg[1].content().asFloat();

	Log::Debug("NaoSpeechToText", "word: %s, Confidence: %f", word.c_str(), confidence);
	if(m_Paused <= 0)
		ThreadPool::Instance()->InvokeOnMain<TextData *>(DELEGATE(NaoSpeechToText, SendingData, TextData * , this),
													 new TextData(word, confidence ));

	return qi::AnyReference();
}

void NaoSpeechToText::SendingData(TextData * a_pData)
{
    SendData(a_pData);
}

void NaoSpeechToText::OnPause()
{
    m_Paused++;
}

void NaoSpeechToText::OnResume()
{
    m_Paused--;
}
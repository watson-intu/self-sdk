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

#include "NaoSoundGesture.h"
#include "NaoPlatform.h"
#include "utils/StringUtil.h"

#include <stdio.h>

REG_OVERRIDE_SERIALIZABLE(SoundGesture, NaoSoundGesture);
RTTI_IMPL( NaoSoundGesture, SoundGesture );

bool NaoSoundGesture::Execute( GestureDelegate a_Callback, const ParamsMap & a_Params )
{
	if ( PushRequest( a_Callback, a_Params ) )
		Play();
	return true;
}

bool NaoSoundGesture::Abort()
{
	return true;
}

void NaoSoundGesture::Play()
{
	Request * pReq = ActiveRequest();
	if ( pReq != NULL )
	{
		m_bPlaying = true;

		if ( pReq->m_Params.GetData().isMember("sound") )
			m_Sound = pReq->m_Params["sound"].asString();
		if ( pReq->m_Params.GetData().isMember("volume") )
			m_fVolume = pReq->m_Params["volume"].asFloat();
		if ( pReq->m_Params.GetData().isMember("pan") )
			m_fPan = pReq->m_Params["pan"].asFloat();

		ThreadPool::Instance()->InvokeOnThread( VOID_DELEGATE( NaoSoundGesture, PlayThread, this ) );
	}
}

void NaoSoundGesture::PlayThread()
{
	bool bWebRequest = StringUtil::StartsWith( m_Sound, "http://", true ) ||
		StringUtil::StartsWith( m_Sound, "https://", true);

	if ( bWebRequest )
	{
		m_bStreaming = true;

		IWebClient * pClient = IWebClient::Create();
		pClient->SetURL(m_Sound);
		pClient->SetStateReceiver(DELEGATE(NaoSoundGesture, OnStreamState, IWebClient *, this));
		pClient->SetDataReceiver(DELEGATE(NaoSoundGesture, OnStreamData, IWebClient::RequestData *, this));

		if (pClient->Send())
		{
			while (m_bStreaming)
				boost::this_thread::sleep(boost::posix_time::milliseconds(50));

			if ( m_Player != NULL )
			{
				fclose( m_Player );
				m_Player = NULL;
			}
		}
		else
		{
			Log::Error("DSSoundGesture", "Unable to connect to url: %s", m_Sound.c_str());
			m_bStreaming = false;
		}
	}
	else
	{
		std::string cmd( StringUtil::Format( "aplay %s", m_Sound.c_str() ) );
		Log::Debug( "NaoSoundGesture", "Running Process: %s", cmd.c_str() );
		system( cmd.c_str() );
	}

	ThreadPool::Instance()->InvokeOnThread( VOID_DELEGATE( NaoSoundGesture, PlayDone, this ) );
}

void NaoSoundGesture::PlayDone()
{
	m_bPlaying = false;
	if ( PopRequest() )
		Play();
}


void NaoSoundGesture::OnStreamState( IWebClient * a_pClient )
{
	if ( a_pClient->GetState() == IWebClient::DISCONNECTED 
		|| a_pClient->GetState() == IWebClient::CLOSED )
	{
		Log::Debug( "DSSOundGesture", "Sound stream closed." );
		m_bStreaming = false;
		delete a_pClient;
	}
}

static bool ParseContentType( const std::string & a_ContentType, int & a_nChannels, int & a_nRate, std::string & a_Format, std::string & a_Type )
{
	// parse the content type, excpecting something like audio/L16;rate=16000
	std::vector<std::string> parts;
	StringUtil::Split( a_ContentType, ";", parts );
	if ( parts.size() < 1 )
		return false;

	if ( StringUtil::Compare( parts[0], "audio/L16", true ) == 0 )
	{
		a_nChannels = 1;
		a_nRate = 16000;
		a_Format = "S16_LE";
		a_Type = "raw";

		for(size_t i=1;i<parts.size();++i)
		{
			std::vector<std::string> kv;
			StringUtil::Split( parts[i], "=", kv );
			if ( kv.size() != 2 )
				continue;

			if ( StringUtil::Compare( kv[0], "rate", true ) == 0 )
				a_nRate = atoi( kv[1].c_str() );
			else if ( StringUtil::Compare( kv[0], "channels", true) == 0 )
				a_nChannels = atoi( kv[1].c_str() );
		}

		return true;
	}
	else if ( StringUtil::Compare( parts[0], "audio/wav", true ) == 0
		|| StringUtil::Compare( parts[0], "audio/x-wav", true ) == 0 )
	{
		a_Type = "wav";
		return true;
	}

	return false;
}

void NaoSoundGesture::OnStreamData( IWebClient::RequestData * a_pData )
{
	if ( m_Player == NULL )
	{
		const std::string & contentType = a_pData->m_Headers["Content-Type"];

		std::string format, type;
		int nChannels, nRate;
		if ( ParseContentType( contentType, nChannels, nRate, format, type ) )
		{
			std::string cmd;
			if ( type == "raw" )
				cmd = StringUtil::Format( "aplay -t raw -f %s -r %d -c %d", format.c_str(), nRate, nChannels );
			else
				cmd = StringUtil::Format( "aplay -t %s", type.c_str() );
			
			Log::Debug( "NaoSoundGesture", "Opening Process: %s", cmd.c_str() );
#ifndef _WIN32
			m_Player = popen(cmd.c_str(), "w");
#endif
		}
		else
		{
			Log::Error( "NaoSoundGesture", "Unsupported content type %s", contentType.c_str() );
		}
	}

	if ( m_Player != NULL )
		fwrite( a_pData->m_Content.c_str(), 1, a_pData->m_Content.size(), m_Player );
}


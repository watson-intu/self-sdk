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

#include "blackboard/Status.h"
#include "NaoPlatform.h"
#include "NaoStatusGesture.h"
#include "SelfInstance.h"

// override the serialization of SpeechGesture
REG_OVERRIDE_SERIALIZABLE( StatusGesture, NaoStatusGesture );
REG_SERIALIZABLE( NaoStatusGesture );
RTTI_IMPL( NaoStatusGesture, StatusGesture );

bool NaoStatusGesture::Execute( GestureDelegate a_Callback, const ParamsMap & a_Params )
{
	bool bError = false;
	try {
		return DoExecute(a_Callback, a_Params);
	}
	catch( const std::exception & ex )
	{
		Log::Error( "NaoStatusGesture", "Caught Exception: %s", ex.what() );
	}

	if ( a_Callback.IsValid() )
		a_Callback( IGesture::Result( this, bError ) );

	return false;
}

bool NaoStatusGesture::DoExecute( GestureDelegate a_Callback, const ParamsMap & a_Params )
{
	std::string colorString;

	std::map<std::string, std::string>::const_iterator it = m_StateMap.begin(), end = m_StateMap.end();
	for(; it != end; ++it)
	{
		if(a_Params.ValidPath("status") && a_Params["status"].asString().compare(it->first) == 0)
			colorString = it->second;
	}

	if ( colorString.length() > 0 )
	{
		unsigned int color = strtoul( colorString.c_str(), NULL, 16 );

		m_Blue = (color & 0xff) / 255.0f;
		m_Green = ((color & 0xff00) >> 8) / 255.0f;
		m_Red = ((color & 0xff0000) >> 16) / 255.0f;
	}

	qi::AnyObject leds = NaoPlatform::Instance()->GetSession()->service("ALLeds");
	leds.async<void>("fadeRGB", "FaceLeds",  (int)(m_Red * 255), (int)(m_Green * 255), (int)(m_Blue * 255), 0.0f );

	return true;
}

bool NaoStatusGesture::Abort()
{
	return false;
}


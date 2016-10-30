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

#ifndef NAO_TOUCH_H
#define NAO_TOUCH_H

#include "NaoPlatform.h"
#include "utils/Time.h"
#include "sensors/ISensor.h"
#include "sensors/TouchData.h"

#include "qi/anyobject.hpp"

class NaoTouch : public ISensor
{
public:
	RTTI_DECL();

	NaoTouch() : m_bPaused( false )
	{}

	//! ISensor interface
	virtual const char * GetSensorName()
	{
		return "Touch";
	}
	virtual const char * GetDataType()
	{
		return TouchData::GetStaticRTTI().GetName().c_str();
	}

	virtual bool OnStart();
	virtual bool OnStop();
	virtual void OnPause();
	virtual void OnResume();

	void StartTouch();
	bool StopTouch();

	qi::AnyReference OnTouch( const std::vector<qi::AnyReference> & touchInfo );
	void SendData( TouchData * a_pData );

private:
	//! Data
	qi::AnyObject m_Memory;
	qi::AnyObject m_TouchSub;
	bool m_bPaused;

	qi::AnyReference DoOnTouch(const std::vector<qi::AnyReference> & touchInfo );
	void DoStartTouch();
	void DoStopTouch();
};

#endif

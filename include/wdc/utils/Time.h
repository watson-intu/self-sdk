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

#ifndef WDC_TIME_H
#define WDC_TIME_H

#include <sys/timeb.h>
#include <stdlib.h>
#include <string>

#include "WDCLib.h"

class WDC_API Time
{
public:
	//! Construction
	Time()
	{
		ftime( &m_tb );
	}
	Time( time_t a_Time )
	{
		m_tb.time = a_Time;
		m_tb.millitm = 0;
	}
	Time( const Time & a_Copy )
	{
		m_tb = a_Copy.m_tb;
	}
	Time( double a_Time )
	{
		ftime( &m_tb );			// set the current time to get the dstflags and other variables set correctly.

		m_tb.time = (time_t)a_Time;
		m_tb.millitm = (unsigned short)((a_Time - m_tb.time) * 1000);
	}
	Time( const std::string & a_File )
	{
		m_tb.time = GetFileModifyTime( a_File );
		m_tb.millitm = 0;
	}

	time_t GetTime() const
	{
		return m_tb.time;
	}
	unsigned short GetMilliseconds() const
	{
		return m_tb.millitm;
	}

	//! Returns the epoch time in seconds, the fractional part is calculated from the milliseconds.
	double GetEpochTime() const
	{
		double epochTime = (double)m_tb.time;
		epochTime += ((double)m_tb.millitm) / 1000.0;

		return epochTime;
	}

	//! Get a string containing a formatted time, see strftime()
	std::string GetFormattedTime( const char * a_pFormat );
	//! Converts time from string to seconds - "1:15:00"
	static time_t ParseTime(const std::string & a_Time);
	//! Set this time based on the file's last modify time, returns the epoch time in seconds.
	static time_t GetFileModifyTime( const std::string & a_File );

private:
	//! Data
	timeb		m_tb;

};

#endif


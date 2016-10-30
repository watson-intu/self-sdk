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

#ifndef WDC_LOG_H
#define WDC_LOG_H

#undef ERROR

#include <string>
#include <list>
#include <stdarg.h>

#include <boost/thread.hpp>
#include "WDCLib.h"		// include last

enum LogLevel
{
	LL_DEBUG_LOW,
	LL_DEBUG_MED,
	LL_DEBUG_HIGH,
	LL_DEBUG = LL_DEBUG_HIGH,
	LL_STATUS,
	LL_WARNING,
	LL_ERROR,
	LL_CRITICAL
};

struct LogRecord
{
	LogLevel		m_Level;
	std::string		m_Time;
	std::string		m_SubSystem;
	std::string		m_Message;
	time_t          m_TimeEpoch;
};

//! Abstract interface for any object that wants to hook into the LogSystem.
class ILogReactor
{
public:
	virtual void Process(const LogRecord & a_Record) = 0;
};

class WDC_API ConsoleReactor : public ILogReactor
{
public:
	ConsoleReactor(LogLevel a_MinLevel = LL_STATUS) : m_MinLevel(a_MinLevel)
	{}

	virtual void Process(const LogRecord & a_Record);

private:
	LogLevel			m_MinLevel;
};

class WDC_API FileReactor : public ILogReactor
{
public:
	FileReactor(const char * a_pLogFile, LogLevel a_MinLevel = LL_STATUS);

	virtual void Process(const LogRecord & a_Record);

private:
	std::string			m_LogFile;
	LogLevel			m_MinLevel;
};

class WDC_API Log
{
public:
	static void RegisterReactor(ILogReactor * a_pReactor);
	static void RemoveReactor(ILogReactor * a_pReactor);

	static void DoLog(LogLevel a_Level, const char * a_pSub, const char * a_pFormat, va_list args );
	static void ProcessRecord(const LogRecord & rec);

	static void DebugLow(const char * a_pSub, const char * a_pFormat, ...);
	static void DebugMed(const char * a_pSub, const char * a_pFormat, ...);
	static void DebugHigh(const char * a_pSub, const char * a_pFormat, ...);
	static void Debug(const char * a_pSub, const char * a_pFormat, ...);
	static void Status( const char * a_pSub, const char * a_pFormat, ... );
	static void Warning( const char * a_pSub, const char * a_pFormat, ... );
	static void Error( const char * a_pSub, const char * a_pFormat, ... );
	static void Critical( const char * a_pSub, const char * a_pFormat, ... );

	static const char * LevelText( LogLevel a_Level );

private:
	//! Types
	typedef std::list<ILogReactor *>		ReactorList;
	//! Data
	static ReactorList & GetReactorList();
	static boost::recursive_mutex & GetReactorLock();
};

#endif


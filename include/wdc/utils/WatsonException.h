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

#ifndef WATSON_EXCEPTION_H
#define WATSON_EXCEPTION_H

#include <exception>

//! Class for exceptions throw by the WDC 
class WatsonException : public std::exception
{
public:
	WatsonException( const char * a_pMessage ) : m_Message( a_pMessage )
	{}

	~WatsonException() throw()
	{}

	const char * Message() const		// DEPRECATED, use what() instead..
	{
		return m_Message.c_str();
	}
	virtual const char * what() const throw()
	{
		return m_Message.c_str();
	}

private:
	//! Data
	std::string m_Message;
};

#endif

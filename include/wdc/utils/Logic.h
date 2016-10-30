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

#ifndef WDC_LOGIC_H
#define WDC_LOGIC_H

#include <string>
#include <vector>

#include "jsoncpp/json/json.h"
#include "WDCLib.h"

//! Logical and equality operators
class WDC_API Logic
{
public:
	enum EqualityOp
	{
		EQ,		// ==
		NE,		// !=
		GR,		// >
		GE,		// >=
		LS,		// <
		LE,		// <=

		LAST_EO
	};

	enum LogicalOp
	{
		AND,		// all conditions must be true
		OR,			// any condition may be true
		XOR,		// exclusive OR
		NOT			// all conditions must be false
	};	

	//! EqualityOp Helpers
	static const char * EqualityOpText(EqualityOp a_Op);
	static EqualityOp	GetEqualityOp(const std::string & a_Op);
	static bool			TestEqualityOp( EqualityOp a_Op, 
		const Json::Value & a_LHS, 
		const Json::Value & a_RHS );

	//! LogicalOp Helpers
	static const char *	LogicalOpText(LogicalOp a_Op);
	static LogicalOp	GetLogicalOp(const std::string & a_Op);
	static bool			TestLogicalOp(LogicalOp a_Op,
		const std::vector<bool> & a_Values);
};

#endif


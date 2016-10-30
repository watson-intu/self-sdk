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

#ifndef WDC_STRINGUTIL_H
#define WDC_STRINGUTIL_H

#include <string>
#include <vector>

#include "../WDCLib.h"		// include last always

//! string utility functions.
class WDC_API StringUtil
{
public:
	static void ToLower( std::string & a_String );
	static void ToUpper( std::string & a_String );
	//! Compares two strings, returns 0 if they are equal.
	static int Compare( const std::string & a_Subject, 
		const std::string & a_Target,
		bool a_bInsenstive = false );
	//! Check if a string ends with the target string
	static bool EndsWith( const std::string & a_Subject,
		const std::string & a_Ending,
		bool a_bInsenstive = false );
	//! Check if a string begins with the target string.
	static bool StartsWith( const std::string & a_Subject, 
		const std::string & a_Start, 
		bool a_bInsenstive = false );
	//! Find a string in a the subject string, returns the offset of the found string
	static size_t Find( const std::string & a_Subject, 
		const std::string & a_Search, 
		size_t a_Offset = 0, 
		bool a_bInsenstive = false );
	//! Replace all occurrences of "search" with "replace" in the subject string, returns the number replaced.
	static int Replace(std::string & a_Subject, 
		const std::string & a_Search, 
		const std::string & a_Replace,
		bool a_bInsenstive = false );
	static void ConvertToTime( const std::string & a_String,
		std::string & a_Time);
	//! Format a string using sprintf into a std::string buffer.
	static std::string Format( const char * a_pFormat, ... );
	//! Returns true if any special characters are all escaped.
	static bool IsEscaped( const std::string & a_Input );
	//! Escape special characters in the given string for passing as a parameter in a URL.
	static std::string UrlEscape( const std::string & a_Input, bool a_bEscapeWithPlus = true );
	//! Helper for appending parameters onto a URL string, will separate parameters with &
	static void AppendParameter( const std::string & a_Param, 
		std::string & a_Output );
	//! Split a string by the specified separators
	static void Split( const std::string & a_String,		//! the subject to split
		const std::string & a_Seperators,					//! a string containing any number of separators
		std::vector< std::string > & a_Split );				//! the split strings
	//! Trim a string of characters
	static std::string RightTrim( const std::string & a_String, 
		const std::string & a_Trim = " " );
	static std::string LeftTrim( const std::string & a_String,
		const std::string & a_Trim = " " );
	static std::string Trim( const std::string & a_String, 
		const std::string & a_Trim = " " );
	//! base64 encode/decode functions
	static std::string EncodeBase64( const std::string & a_Input );
	static std::string DecodeBase64( const std::string & a_Input );
    //! Returns the substring after the last slash if found, returns whole string is no slash found
    static std::string GetFilename( const std::string & a_Path );
	//! Match a wildcard string (e.g. match*.???) to a given string.
	static bool WildMatch(const std::string & a_Pattern, 
		const std::string & a_Search);
	static bool WildMatch(const char * pat, 
		const char * str );

	// case-independent (ci) compare_less binary function
	template<typename charT>
	struct nocase_equal
	{
		bool operator()( charT ch1, charT ch2 )
		{
			return tolower(ch1) == tolower(ch2);
		}
	};

	struct nocase_compare : public std::binary_function<unsigned char, unsigned char, bool>
	{
		bool operator() (const unsigned char& c1, const unsigned char& c2) const {
			return tolower(c1) < tolower(c2);
		}
	};

	struct ci_less : std::binary_function<std::string, std::string, bool>
	{
		bool operator() (const std::string & s1, const std::string & s2) const
		{
			return std::lexicographical_compare
				(s1.begin(), s1.end(),		// source range
					s2.begin(), s2.end(),   // dest range
					nocase_compare());		// comparison
		}
	};

};

#endif


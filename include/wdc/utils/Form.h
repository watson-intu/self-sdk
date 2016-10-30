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

//! Author rsinha

#ifndef WDC_FORM_H
#define WDC_FORM_H

#include <string>
#include <vector>

#include "WDCLib.h"

//! This class is used to generate multi-part form data to be sent through REST
//! requests
class WDC_API Form
{
public:
	//! Construction
	Form();

	//! Methods
	void AddFormField(const std::string & a_Id, const std::string & a_Value,
		const std::string & a_CharSet = "UTF-8" );
	bool AddFilePartFromPath(const std::string & fieldName, 
		const std::string & a_FullFilePath, 
		const std::string & a_ContentType = "application/octet-stream" );
	bool AddFilePart(const std::string & fieldName, 
		const std::string & a_FileName, 
		const std::string & a_FileData,
		const std::string & a_ContentType = "application/octet-stream" );

	//! We are done, generates the body and any headers
	void Finish();

	const std::string & GetContentType() const { return m_ContentType; }
	const std::string & GetBody() const { return m_Body; }
    const std::string & GetBoundary() const { return m_boundary; }

private:
	//! Data
	std::string	m_boundary;
	std::vector<std::string>
				m_Parts;
	std::string	m_ContentType;
	std::string m_Body;

	//! Static members
	static std::string	m_lineFeed;
};

#endif

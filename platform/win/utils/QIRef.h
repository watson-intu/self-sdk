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

#ifndef QIREF_H
#define QIREF_H

//! Helper template class for holding a IUnknown pointer, this will
//! call Release() automatically on destruction, and will call AddRef()
//! when copies of a pointer are made.
template<typename T>
class QIRef
{
public:
	QIRef() : m_pReference( NULL )
	{}
	QIRef( const QIRef & a_Copy ) : m_pReference( a_Copy.m_pReference )
	{
		if ( m_pReference != NULL )
			m_pReference->AddRef();
	}
	~QIRef()
	{
		Release();
	}
	bool IsValid()
	{
		return m_pReference != NULL;
	}
	void Release()
	{
		if ( m_pReference != NULL )
		{
			m_pReference->Release();
			m_pReference = NULL;
		}
	}

	T operator->()
	{
		return m_pReference;
	}
	T * operator&()
	{
		return &m_pReference;
	}
	QIRef & operator=( const QIRef & a_Copy )
	{
		Release();
		m_pReference = a_Copy.m_pReference;
		if ( m_pReference != NULL )
			m_pReference->AddRef();
		return *this;
	}

private:
	T m_pReference;
};

#endif

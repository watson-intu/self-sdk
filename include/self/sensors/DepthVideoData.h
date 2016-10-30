/* ***************************************************************** */
/*                                                                   */
/* IBM Confidential                                                  */
/* OCO Source Materials                                              */
/*                                                                   */
/* (C) Copyright IBM Corp. 2001, 2014                                */
/*                                                                   */
/* The source code for this program is not published or otherwise    */
/* divested of its trade secrets, irrespective of what has been      */
/* deposited with the U.S. Copyright Office.                         */
/*                                                                   */
/* ***************************************************************** */

#ifndef SELF_DEPTH_VIDEODATA_H
#define SELF_DEPTH_VIDEODATA_H

#include <vector>
#include <cstring>
#include "IData.h"
#include "SelfLib.h"

class SELF_API DepthVideoData : public IData
{
public:
	RTTI_DECL();

	DepthVideoData()
	{}
	DepthVideoData(std::vector<unsigned char> a_BinaryData) :
		m_BinaryData((const char *)&a_BinaryData[0], a_BinaryData.size())
	{}

	DepthVideoData(const unsigned char * a_pBinaryData, int a_BinaryLength) :
		m_BinaryData((const char *)a_pBinaryData, a_BinaryLength)
	{}

	//! ISerializable interface
	virtual void Serialize(Json::Value & json)
	{
		json["m_BinaryData"] = StringUtil::EncodeBase64( m_BinaryData );
	}
	virtual void Deserialize(const Json::Value & json)
	{
		m_BinaryData = StringUtil::DecodeBase64( json["m_BinaryData"].asString() );
	}

	//! IData interface
	virtual bool ToBinary(std::string & a_Output)
	{
		a_Output = m_BinaryData;
		return true;
	}
	virtual bool FromBinary( const std::string & a_Type, const std::string & a_Input)
	{
		m_BinaryData = a_Input;
		return true;
	}

	//!Accessors
	const std::string & GetBinaryData() const
	{
		return m_BinaryData;
	}

private:
	//!Data
	std::string			m_BinaryData;
};

#endif //SELF_DEPTH_VIDEODATA_H

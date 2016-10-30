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

#ifndef PACKAGE_STORE_DATAMODELS_H
#define PACKAGE_STORE_DATAMODELS_H

#include "utils/ISerializable.h"
#include "SelfLib.h"

struct ConfigData : public ISerializable
{
	RTTI_DECL();

	std::string		m_DevVersion;
	std::string		m_RecVersion;
	std::string		m_ReqVersion;

	virtual void Serialize(Json::Value & json)
	{
		json["m_DevVersion"] = m_DevVersion;
		json["m_RecVersion"] = m_RecVersion;
		json["m_ReqVersion"] = m_ReqVersion;
	}

	virtual void Deserialize(const Json::Value & json)
	{
		m_DevVersion = json["m_DevVersion"].asString();
		m_RecVersion = json["m_RecVersion"].asString();
		m_ReqVersion = json["m_ReqVersion"].asString();
	}
};

#endif //PACKAGE_STORE_DATAMODELS_H
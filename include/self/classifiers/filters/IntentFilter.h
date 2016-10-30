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

#ifndef SELF_INTENT_FILTER_H
#define SELF_INTENT_FILTER_H

#include "classifiers/proxies/ITextClassifierProxy.h"
#include "SelfLib.h"

//! This filter will filter a response if it matches one of the provided intents
class SELF_API IntentFilter : public ITextClassifierProxy::IClassFilter
{
public:
	RTTI_DECL();

	IntentFilter();

	//! ISerialiable interface
	virtual void Serialize(Json::Value & json);
	virtual void Deserialize(const Json::Value & json);
	//! IClassFilter interface
	virtual bool ApplyFilter(Json::Value & a_Intent);

private:
	//! Data
	std::vector<std::string> m_Intents;
};

#endif // SELF_INTENT_FILTER_H

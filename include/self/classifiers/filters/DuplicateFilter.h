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

#ifndef SELF_DUPLICATE_FILTER_H
#define SELF_DUPLICATE_FILTER_H

#include "classifiers/proxies/ITextClassifierProxy.h"
#include "SelfLib.h"

//! This filter looks for the same intent within a time window and will ignore those
//! intents.
class SELF_API DuplicateFilter : public ITextClassifierProxy::IClassFilter
{
public:
	RTTI_DECL();

	DuplicateFilter();

	//! ISerialiable interface
	virtual void Serialize(Json::Value & json);
	virtual void Deserialize(const Json::Value & json);
	//! IClassFilter interface
	virtual bool ApplyFilter(Json::Value & a_Intent);

private:
	//! Data
	double			m_MinIntentWindow;				// min time between the same intent, this is a window to prevent duplicate from multiple inputs
	double			m_LastIntentTime;
	std::string		m_LastIntent;
};

#endif // SELF_DUPLICATE_FILTER_H

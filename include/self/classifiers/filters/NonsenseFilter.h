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

#ifndef SELF_NONSENSE_FILTER_H
#define SELF_NONSENSE_FILTER_H

#include "classifiers/proxies/ITextClassifierProxy.h"
#include "SelfLib.h"

//! This filter looks for the nonsense intent and will ignore text if it makes no sense
class SELF_API NonsenseFilter : public ITextClassifierProxy::IClassFilter
{
public:
	RTTI_DECL();

	NonsenseFilter();

	//! ISerialiable interface
	virtual void Serialize(Json::Value & json);
	virtual void Deserialize(const Json::Value & json);
	//! IClassFilter interface
	virtual bool ApplyFilter(Json::Value & a_Intent);

	//! Mutators
	void SetActiveState(bool a_State);
	bool GetActiveState();

private:
	//! Methods
	void CalculateChatterConf();
	void IgnoreChatter();

	//! Data
	std::string		m_NonsenseClass;				// any text classified as this is ignored
	float			m_MinIgnoreNonsense;
	float 			m_DynamicNonsenseInterval;
	float 			m_DynamicNonsenseIntervalIncrement;
	bool			m_bActive;
};

#endif // SELF_NONSENSE_FILTER_H

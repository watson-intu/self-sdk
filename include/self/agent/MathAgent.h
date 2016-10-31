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

#ifndef SELF_NAMEAGENT_H
#define SELF_NAMEAGENT_H

#include "IAgent.h"
#include "blackboard/Calculate.h"

#include "SelfLib.h"

class SELF_API MathAgent : public IAgent
{
public:
	RTTI_DECL();

	MathAgent()
	{}

	enum MathOp
	{
		SUM,		// +
		SUBTRACT,	// -
		DIVIDE,		// /
		MULTIPLY,	// *

		LAST_EO
	};

	//! ISerializable interface
	virtual void Serialize(Json::Value & json);
	virtual void Deserialize(const Json::Value & json);

	//! IAgent interface
	virtual bool OnStart();
	virtual bool OnStop();

private:
	const char * MathOpText(MathOp a_Op);
	MathAgent::MathOp GetMathOp(const std::string & a_Op);
	void PopulateVectorFromData(const std::string & a_Data, const std::string & a_Key, std::vector<double> & a_Vector);
	double PerformArithmetic(const std::string & a_Arithmetic, std::vector<double> & a_Vector);

	//! Callbacks
	void OnCalculate(const ThingEvent & a_ThingEvent);

};

#endif // SELF_MATHAGENT_H

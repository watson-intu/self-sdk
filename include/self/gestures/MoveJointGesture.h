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

#ifndef MOVEJOINTGESTURE_H
#define MOVEJOINTGESTURE_H

#include "IGesture.h"
#include "SelfLib.h"

//! This is the base class for any gesture that moves a single joint on a robot.
class SELF_API MoveJointGesture : public IGesture
{
public:
	RTTI_DECL();

	//! Construction
	MoveJointGesture() : m_fStiffness( 0.0f ), m_bAbsolute( false )
	{}
	MoveJointGesture( const std::string & a_GestureId ) : IGesture( a_GestureId ),
		m_fStiffness( 0.0f ), m_bAbsolute( false )
	{}

	//! ISerializable interface
	virtual void Serialize(Json::Value & json);
	virtual void Deserialize(const Json::Value & json);

	//! IGesture interface
	virtual bool Execute( GestureDelegate a_Callback, const ParamsMap & a_Params );

	//! Mutators
	void SetStiffness(float a_fStiffness)
	{
		m_fStiffness = a_fStiffness;
	}
	void SetAsbsolute( bool a_bAbsolute )
	{
		m_bAbsolute = a_bAbsolute;
	}

protected:
	//! Data
	std::vector<std::string> m_JointNames;
	std::vector<float> m_fAngles;
	std::vector<float> m_fSpeeds;
	float m_fStiffness;
	bool m_bAbsolute;
};


#endif //IGESTURE_H

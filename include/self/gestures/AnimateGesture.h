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

#ifndef ANIMATE_GESTURE_H
#define ANIMATE_GESTURE_H

#include "IGesture.h"
#include "SelfLib.h"

//! This gesture animates a robot using a built in animation ID
class SELF_API AnimateGesture : public IGesture
{
public:
	RTTI_DECL();

	//! Construction
	AnimateGesture()
	{}

	//! ISerializable interface
	virtual void Serialize(Json::Value & json);
	virtual void Deserialize(const Json::Value & json);

	//! IGesture interface
	virtual bool Execute( GestureDelegate a_Callback, const ParamsMap & a_Params );
	virtual bool Abort();

protected:
	struct AnimationEntry
	{
		AnimationEntry( const std::string & a_AnimId, const std::string & a_RequiredPosture ) :
			m_AnimId( a_AnimId ), m_RequiredPosture( a_RequiredPosture )
		{}

		std::string m_AnimId;
		std::string m_RequiredPosture;
	};
	std::vector<AnimationEntry> m_Animations;
};


#endif //ANIMATE_GESTURE_H

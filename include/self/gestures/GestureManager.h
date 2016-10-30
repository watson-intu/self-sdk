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

#ifndef GESTURE_MANAGER_H
#define GESTURE_MANAGER_H

#include <map>

#include "utils/ParamsMap.h"
#include "utils/Factory.h"
#include "utils/Delegate.h"
#include "utils/RTTI.h"
#include "SelfLib.h"				// include last

//! Forward declare
class IGesture;
class ProxyGesture;

//! This manager manages all gestures available to the local self instance. Normally a SkillGesture
//! will invoke into this manager to execute various gestures to perform actions.
class SELF_API GestureManager : public ISerializable
{
public:
	RTTI_DECL();

	//! Types
	typedef std::list<std::string>	GestureFiles;
	typedef boost::shared_ptr<IGesture>	IGestureSP;
	typedef boost::shared_ptr<ProxyGesture> ProxyGestureSP;
	typedef std::vector< IGestureSP >	GestureList;
	typedef std::multimap< std::string, IGestureSP >	GestureMap;
	typedef std::map< std::string, ProxyGestureSP >		ProxyMap;
	typedef Factory< IGesture >		GestureFactory;
	typedef Delegate<IGesture *>	GestureDelegate;

	//! Construction
	GestureManager();
	~GestureManager();

	//! ISerializable interface
	virtual void Serialize(Json::Value & json);
	virtual void Deserialize(const Json::Value & json);

	//! Accessors
	const GestureMap & GetGestureMap() const
	{
		return m_GestureMap;
	}

	//! This finds all gestures who have the given ID, returns false if none are found.
	bool FindGestures( const std::string & a_GestureId, 
		GestureList & a_Gestures ) const;

	//! Initialize and start this sensor manager.
	bool Start(const GestureFiles & a_Files );
	//! Stop this sensor manager.
	bool Stop();

	bool AddGesture( const IGestureSP & a_spGesture );
	bool RemoveGesture( const IGestureSP & a_spGesture );

private:
	//! Data
	bool					m_bActive;
	GestureMap				m_GestureMap;	
	ProxyMap				m_ProxyMap;

	void OnSubscriber( const ITopics::SubInfo & a_Info );
	void OnGestureEvent( const ITopics::Payload & a_Payload );
};

#endif

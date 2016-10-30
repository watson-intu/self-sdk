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

#ifndef SELF_LASERDATA_H
#define SELF_LASERDATA_H

#include "IData.h"
#include "SelfLib.h"

class SELF_API LaserData : public IData
{
public:
	RTTI_DECL();

	LaserData() : 
		m_fDistance( 0.0f ), 
		m_fAzimuthalAngle( 0.0f ), 
		m_fElevationAngle( 0.0f )
	{}
	LaserData(float a_Distance, float a_AzimuthalAngle, float a_ElevationAngle) : 
		m_fDistance(a_Distance),
		m_fAzimuthalAngle(a_AzimuthalAngle),
		m_fElevationAngle(a_ElevationAngle)
	{}

	~LaserData()
	{}

	//! ISerializable interface
	virtual void Serialize(Json::Value & json)
	{
		json["m_fDistance"] = m_fDistance;
		json["m_fAzimuthalAngle"] = m_fAzimuthalAngle;
		json["m_fElevationAngle"] = m_fElevationAngle;
	}
	virtual void Deserialize(const Json::Value & json)
	{
		m_fDistance = json["m_fDistance"].asFloat();
		m_fAzimuthalAngle = json["m_fAzimuthalAngle"].asFloat();
		m_fElevationAngle = json["m_fElevationAngle"].asFloat();
	}

	//!Accessors
	float GetDistance() const
	{
		return m_fDistance;
	}
	float GetAzimuthalAngle() const
	{
		return m_fAzimuthalAngle;
	}
	float GetElevationAngle() const
	{
		return m_fElevationAngle;
	}
private:
	//!Data
	float		m_fDistance;
	float		m_fAzimuthalAngle;	
	float		m_fElevationAngle;
};

#endif //SELF_LASERATA_H

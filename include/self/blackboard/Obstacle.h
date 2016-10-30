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

#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "IThing.h"
#include "utils/Vector3.h"

//! This thing represents an obstruction/obstacle in the environment to be avoided.
class SELF_API Obstacle : public IThing
{
public:
	RTTI_DECL();

	Obstacle() : IThing( TT_PERCEPTION )
	{}

	const Vector3 & GetMin() const
	{
		return m_Min;
	}
	const Vector3 & GetMax() const
	{
		return m_Max;
	}

	void SetBounds( const Vector3 & a_Min, const Vector3 & a_Max )
	{
		m_Min = a_Min;
		m_Max = a_Max;
	}

private:
	//! Data
	Vector3		m_Min, m_Max;
};

#endif

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

#ifndef VECTOR3_H
#define VECTOR3_H

#include <string>
#include <stdio.h>

//! This class is used to contain a 3D position in space.
class Vector3
{
public:
	//! Construction
	Vector3() : m_X( 0.0f ), m_Y( 0.0f ), m_Z( 0.0f )
	{}
	Vector3( const Vector3 & a_Copy ) : 
		m_X( a_Copy.m_X ),
		m_Y( a_Copy.m_Y ),
		m_Z( a_Copy.m_Z )
	{}
	Vector3( float x, float y, float z )
	{
	}
	Vector3( const std::string & a_Parse )
	{
		sscanf( a_Parse.c_str(), "%g,%g,%g", &m_X, &m_Y, &m_Z );
	}

	std::string ToString() const
	{
		char buffer[ 256 ];
		snprintf( buffer, sizeof(buffer), "%g,%g,%g", m_X, m_Y, m_Z );
	}

	float GetX() const
	{
		return m_X;
	}
	float GetY() const
	{
		return m_Y;
	}
	float GetZ() const
	{
		return m_Z;
	}

private:
	//! Data
	float	m_X;
	float	m_Y;
	float	m_Z;
};

#endif


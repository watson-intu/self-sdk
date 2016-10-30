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

#ifndef PERSON_H
#define PERSON_H

#include "IThing.h"

//! A person as recognized by the visual recognition system.
class SELF_API Person : public IThing
{
public:
	RTTI_DECL();

	//! Types
	typedef boost::shared_ptr<Person>	SP;
	typedef boost::weak_ptr<Person>		WP;

	Person() : IThing( TT_PERCEPTION )
	{}

	const std::string & GetName() const
	{
		return m_Name;
	}
	void SetName( const std::string & a_Name )
	{
		m_Name = a_Name;
	}

	const double & GetConfidence() const
	{
		return m_Confidence;
	}
	void SetConfidence( const double a_Confidence )
	{
		m_Confidence = a_Confidence;
	}

	const std::string & GetGender() const
	{
		return m_Gender;
	}

	void SetGender( const std::string & a_Gender )
	{
		m_Gender = a_Gender;
	}

	const std::string  & GetAgeRange() const
	{
		return m_AgeRange;
	}

	void SetAgeRange( const std::string & a_Age)
	{
		m_AgeRange = a_Age;
	}

	const std::string & GetPosX() const
	{
		return m_PosX;
	}
	void SetPosX( const std::string & a_PosX )
	{
		m_PosX = a_PosX;
	}

	const std::string & GetPosY() const
	{
		return m_PosY;
	}
	void SetPosY( const std::string & a_PosY )
	{
		m_PosY = a_PosY;
	}

	const std::string & GetWidth() const
	{
		return m_Width;
	}
	void SetWidth( const std::string & a_Width )
	{
		m_Width = a_Width;
	}

	const std::string & GetHeight() const
	{
		return m_Height;
	}
	void SetHeight( const std::string & a_Height)
	{
		m_Height = a_Height;
	}


	virtual void Serialize(Json::Value & json);
	virtual void Deserialize(const Json::Value & json);

private:
	//! Data
	std::string m_Name;
	std::string m_Gender;
	std::string m_AgeRange;
	std::string m_PosX;
	std::string m_PosY;
	std::string	m_Width;
	std::string m_Height;
	double 		m_Confidence;

};

#endif

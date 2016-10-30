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

#ifndef OBJECT_H
#define OBJECT_H

#include <list>

#include "IThing.h"
#include "SelfLib.h"		// include last always

class SELF_API Object : public IThing
{
public:
    RTTI_DECL();

	Object() : IThing( TT_PERCEPTION )
	{}

    const std::list<std::string> & GetObjectTypes() const
    {
        return m_ObjectTypeList;
    }
    void AddObjectType( const std::string & a_ObjectType )
    {
        m_ObjectTypeList.push_back(a_ObjectType);
    }

    //! ISerializable interface
    virtual void Serialize(Json::Value & json);
    virtual void Deserialize(const Json::Value & json);

private:
    //! Data
    std::list<std::string> m_ObjectTypeList;
};

#endif

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

#ifndef SELF_WEX_H
#define SELF_WEX_H

#include "services/IService.h"

class WEX : public IService
{
public:
    RTTI_DECL();

    //! Construction
    WEX();

    //! ISerializable
    virtual void Serialize(Json::Value & json);
    virtual void Deserialize(const Json::Value & json);

    //! IService interface
    virtual bool Start();

    void Search(const std::string & a_Query, const std::string & a_Collection, Delegate<const std::string &> a_Callback );
    void Search(const std::string & a_Query, Delegate<const std::string &> a_Callback );

private:
    //!Data
    std::string m_Collection;

};

#endif

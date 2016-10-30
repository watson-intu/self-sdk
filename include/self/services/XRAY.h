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


#ifndef SELF_XRAYSERVICE_H
#define SELF_XRAYSERVICE_H

#include "services/IService.h"
#include "SelfLib.h"

class SELF_API XRAY : public IService
{
public:
    RTTI_DECL();

    //! Construction
    XRAY();

    //! ISerializable interface
    virtual void Serialize(Json::Value & json);
    virtual void Deserialize(const Json::Value & json);

    //! IService interface
    virtual bool Start();


	//! Ask a question of all pipelines, returns the best answer.
	void AskQuestion( const std::string & a_Input,
		Delegate<const Json::Value &> a_Callback );
	//! ask a question of a specific pipeline
	void AskQuestion( const std::string & a_Pipeline,
		const std::string & a_Input,
		Delegate<const Json::Value &> a_Callback );
	//! Request the configuration data 
	Request * RequestConfig( Delegate<const std::string &> a_Callback );
	Request * RequestLogin(Delegate<const std::string &> a_Callback);

private:
	//! Callbacks
    void			OnConfigured(const std::string & a_Response);
	void 			OnLogin(const std::string & a_Response);

	//! Data
	std::list<std::string>	
					m_Pipelines;

	class AskPipelines
	{
	public:
		AskPipelines( XRAY * a_pService, const std::string & a_Input, Delegate<const Json::Value &> a_Callback );

	private:
		//! Callbacks
		void			OnResult( const Json::Value & a_Result );

		//! Data
		XRAY *			m_pService;
		std::string		m_Input;
		Delegate<const Json::Value &>
						m_Callback;

		int				m_PendingRequests;
		std::list<Json::Value>
						m_Results;
	};

};

#endif //SELF_XRAYSERVICE_H

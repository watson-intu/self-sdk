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

#ifndef TOPIC_MANAGER_H
#define TOPIC_MANAGER_H

#include <map>

#include "ITopics.h"
#include "agent/IAgent.h"
#include "utils/IWebClient.h"
#include "utils/IWebServer.h"
#include "utils/TimerPool.h"

//! A agent that implements the ITopics interface. 
class SELF_API TopicManager : public ITopics, public ILogReactor, public ISerializable
{
public:
	RTTI_DECL();

	//! Construction
	TopicManager();
	~TopicManager();

	//! ISerializable interface
	virtual void Serialize(Json::Value & json);
	virtual void Deserialize(const Json::Value & json);

	//! ILogReactor interface
	virtual void Process(const LogRecord & a_Record);

	//! Start this manager, returns false on failure.
	bool Start();
	bool Stop();

	//! ITopics interface
	virtual void GetTopics( TopicVector & a_Topics );
	virtual void RegisterTopic(
		const std::string & a_Topic,
		const std::string & a_Type,
		SubCallback a_SubscriberCallback = SubCallback() );
	virtual void UnregisterTopic( const std::string & a_TopicId);

	virtual bool IsSubscribed(const std::string & a_TopicId);
	virtual size_t GetSubscriberCount(const std::string & a_TopicId);
	virtual bool Publish( 
		const std::string & a_TopicId, 
		const std::string & a_Data, 
		bool a_bPersisted = false,
		bool a_bBinary = false );
	virtual bool Send(
		const std::string & a_Targets,
		const std::string & a_TopicId,
		const std::string & a_Data,
		bool a_bBinary = false );
	virtual bool PublishAt(
		const std::string & a_Path,
		const std::string & a_Data,
		bool a_bPersisted = false,
		bool a_bBinary = false );

	virtual void Query(
		const std::string & a_Path,				//! the path to the parent topic, we will return all topics underneath the provided topic.
		QueryCallback a_Callback );
	virtual bool Subscribe(
		const std::string & a_Path,		//! The topic to subscribe, ".." moves up to a parent self, "+" is a wild-card.
		PayloadCallback a_Callback);
	virtual bool Unsubscribe( 
		const std::string & a_Path,
		void * a_pObject = NULL );

	//! Accessors
	const std::string & GetParentHost() const { return m_ParentHost; }

	//! Mutators
	void SetCertFile( const std::string & a_CertFile )
	{
		m_CertFile = a_CertFile;
	}
	void SetKeyFile( const std::string & a_KeyFile )
	{
		m_KeyFile = a_KeyFile;
	}
	void SetVerifyFile( const std::string & a_VerifyFile )
	{
		m_VerifyFile = a_VerifyFile;
	}
	void SetParentHost( const std::string & a_ParentHost )
	{
		if ( m_ParentHost != a_ParentHost )
		{
			m_ParentHost = a_ParentHost;
			if ( m_spParentConnection )
			{
				if ( m_ParentHost.size() > 0 )
					m_spParentConnection->GetSocket()->Close();
				else
					RemoveConnection( m_spParentConnection );
			}
		}
	}
	void SetInterface( const std::string & a_Interface )
	{
		m_Interface = a_Interface;
	}
	void SetPort( int a_nPort )
	{
		m_nPort = a_nPort;
	}
	void SetThreads( int a_nThreads )
	{
		m_nThreads = a_nThreads;
	}
	void SetRequestTimeout( float a_fReqTimeout )
	{
		m_fRequestTimeout = a_fReqTimeout;
	}
	void SetReconnectInterval( float a_fReconnectInterval )
	{
		m_fReconnectInterval = a_fReconnectInterval;
	}
	void SetConfigFile( const std::string & a_ConfigFile )
	{
		m_ConfigFile = a_ConfigFile;
	}
	void SetSelfId( const std::string & a_SelfId )
	{
		m_SelfId = a_SelfId;
	}
	void SetBearerToken( const std::string & a_BearerToken )
	{
		m_BearerToken = a_BearerToken;
	}

private:
	//! Types
	typedef Delegate<const Json::Value &>			MessageHandler;
	typedef std::map<std::string, MessageHandler>	MessageHandlerMap;

	//! This handles a topic subscriber through the REST interface
	class Subscriber : public boost::enable_shared_from_this<Subscriber>
	{
	public:
		//! Types
		typedef boost::shared_ptr<Subscriber>			SP;

		//! Construction
		Subscriber();
		~Subscriber();

		unsigned int GetSubscriberId() const { return m_nSubscriberId; }
		IWebServer::RequestSP GetRequest() const { return m_spRequest; }

		bool Start(TopicManager * a_pManager, const IWebServer::RequestSP & a_spRequest);
		void Stop();

	private:
		//! Data
		TopicManager *	m_pManager;
		std::string		m_TopicId;
		std::string		m_Boundry;
		unsigned int	m_nSubscriberId;
		IWebServer::RequestSP
						m_spRequest;
		bool			m_bHeaderSent;
		size_t			m_ContentLen;
		std::string		m_Publish;

		//! Callbacks
		void OnPublish( std::string * a_Data );
		void OnPayload( const ITopics::Payload & a_Payload );
		void OnError( IWebSocket * );
	};
	typedef std::map< unsigned int, Subscriber::SP>	SubscriberMap;

	//! This handles a single connection to our parent or a client.
	class Connection : public boost::enable_shared_from_this<Connection>
	{
	public:
		//! Types 
		typedef boost::shared_ptr<Connection>			SP;

		//! Construction
		Connection();
		~Connection();

		unsigned int GetConnectionId() const { return m_ConnectionId; }
		const std::string & GetSelfId() const { return m_SelfId; }
		const std::string & GetBearerToken() const { return m_BearerToken; } 
		bool IsAuthenticated() const { return m_bAuthenticated; }
		IWebSocket::SP GetSocket() const { return m_spSocket; }

		void Start( TopicManager * a_pAgent, IWebSocket::SP a_spSocket );
		void Authenticate( const std::string & a_SelfId, const std::string & a_Token );

	private:
		//! Types
		typedef std::list<IWebSocket::FrameSP>	FramesList;

		//! Data
		TopicManager *	m_pAgent;
		unsigned int	m_ConnectionId;
		std::string		m_SelfId;
		std::string		m_BearerToken;
		bool			m_bAuthenticated;
		IWebSocket::SP	m_spSocket;
		FramesList		m_PendingFrames;
		TimerPool::ITimer::SP
						m_spKeepAliveTimer;

		//! Callbacks
		void OnAuthenticate( const Json::Value & a_Response );
		void OnAuthenticated();
		void OnFrame( IWebSocket::FrameSP a_spFrame );
		void OnError( IWebSocket * );
		void OnKeepAlive();
	};
	typedef std::map< unsigned int, Connection::SP >		ConnectionMap;
	typedef std::map< std::string, unsigned int >			ConnectionLookup;

	typedef std::list< std::string >					StringList;			// list of subscribers by their path
	struct TopicData
	{
		TopicData()
		{}
		TopicData( const std::string & a_Topic, const std::string & a_Type, SubCallback a_SubscriberCallback ) : 
			m_Topic( a_Topic ), m_Type( a_Type ), m_SubscriberCallback( a_SubscriberCallback )
		{}

		std::string			m_Topic;				// topic ID
		std::string			m_Type;					// type of topic
		SubCallback			m_SubscriberCallback;	// callback to invoke when a subscriber is added/removed
		StringList			m_Subscribers;			// who is subscribed to this topic
	};
	typedef std::map< std::string, TopicData >					TopicMap;
	typedef std::map< std::string, std::string>					TopicDataMap;
	typedef std::pair< std::string, TopicData>					TopicPair;
	typedef std::map< std::string, StringList >					TargetMap;

	//! This is created for each subscription we have created.
	struct Subscription
	{
		Subscription()
		{}
		Subscription( const std::string & a_Path, PayloadCallback a_Callback ) : 
			m_Path( a_Path ), m_Callback( a_Callback )
		{}

		std::string m_Path;
		PayloadCallback m_Callback;
	};
	typedef std::list<Subscription>						SubscriptionList;
	typedef std::map< std::string, SubscriptionList >	SubscriptionMap;

	struct QueryRequest
	{
		QueryRequest()
		{}
		QueryRequest(const std::string & a_Path, QueryCallback a_Callback) :
			m_Path(a_Path), m_Callback(a_Callback)
		{}

		std::string m_Path;
		QueryCallback m_Callback;
	};
	typedef std::map< unsigned int, QueryRequest >		QueryRequestMap;

	//! Data
	bool			m_bActive;
	std::string		m_CertFile;
	std::string		m_KeyFile;
	std::string		m_VerifyFile;
	std::string		m_ParentHost;					// parent host-name
	std::string		m_Interface;
	int				m_nPort;
	int				m_nThreads;
	float			m_fRequestTimeout;
	float			m_fReconnectInterval;
	std::string		m_RestUser;
	std::string		m_RestPassword;
	TopicDataMap	m_TopicDataMap;					// persisted topic data 

	std::string		m_ConfigFile;
	std::string		m_SelfId;						// our self ID
	std::string		m_BearerToken;					// our authentication token

	Connection::SP	m_spParentConnection;
	TimerPool::ITimer::SP
					m_spReconnectTimer;
	IWebClient::SP	m_spWebClient;					// connection to our parent
	IWebServer::SP	m_spWebServer;					// our web server object for handling incoming connections
	ConnectionMap	m_ConnectionMap;
	ConnectionLookup
					m_ConnectionLookup;
	unsigned int	m_NextConnectionId;	
	SubscriberMap	m_SubscriberMap;
	unsigned int	m_NextSubscriberId;

	TopicMap		m_TopicMap;
	SubscriptionMap m_SubscriptionMap;
	MessageHandlerMap
					m_MessageHandlerMap;
	QueryRequestMap	m_QueryRequestMap;
	unsigned int	m_NextRequestId;
	bool			m_bProcessingLog;

	bool Authenticate(IWebServer::RequestSP a_spRequest);
	void OnStreamRequest(IWebServer::RequestSP a_spRequest);
	void OnInfoRequest(IWebServer::RequestSP a_spRequest);
	void OnTopicRequest(IWebServer::RequestSP a_spRequest);

	void ConnectToParent();
	void OnClientState( IWebClient * a_pClient );
	Subscriber::SP AddSubscriber(IWebServer::RequestSP a_spRequest);
	void RemoveSubscriber(const Subscriber::SP & a_spSubscriber);
	Connection::SP AddConnection( IWebSocket::SP a_spSocket );
	void RemoveConnection(const Connection::SP & a_spConnection);

	Json::Value GetTargets( const std::string & a_TopicId );
	void RouteMessage( const Json::Value & a_Message, Connection * a_pOrigin = NULL );
	void ProcessMessage( const Json::Value & a_Message );
	void HandleSubscribe(const Json::Value & a_Message);
	void HandleSubscribeFailed(const Json::Value & a_Message);
	void HandlePublish(const Json::Value & a_Message);
	void HandlePublishAt(const Json::Value & a_Message);
	void HandleUnsubscribe(const Json::Value & a_Message);
	void HandleNoRoute(const Json::Value & a_Message);
	void HandleQuery(const Json::Value & a_Message);
	void HandleQueryResponse(const Json::Value & a_Message);

	Connection::SP ResolveConnection( const std::string & a_Target,
		std::string & a_Origin );
	Connection::SP FindConnection( IWebSocket::SP a_spSocket );
	Connection::SP FindConnection( const std::string & a_SelfId );

	void OnTopicManagerEvent( const ITopics::Payload & a_Payload );
};

#endif

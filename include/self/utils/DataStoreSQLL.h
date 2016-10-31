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

#ifndef SELF_DATA_STORE_SQLL_H
#define SELF_DATA_STORE_SQLL_H

#include "IDataStore.h"

struct sqlite3;

class DataStoreSQLL : public IDataStore
{
public:
	RTTI_DECL();

	//! COnstruction
	DataStoreSQLL();
	~DataStoreSQLL();

	//! IDataStore interface
	virtual bool Start(const std::string & a_DB,
		const Json::Value & a_DataDefinition);
	virtual bool Stop();

	virtual bool Save(const std::string & a_ID,
		const Json::Value & a_Data,
		Delegate<bool> a_Callback = Delegate<bool>());
	virtual bool Load(const std::string & a_ID,
		Delegate<Json::Value *> a_Callback);
	virtual bool Delete(const std::string & a_ID,
		Delegate<bool> a_Callback = Delegate<bool>());
	virtual bool Find(const Conditions & a_Conditions,
		Delegate<QueryResults *> a_Callback);

private:
	//! Types
	struct ICommand
	{
		ICommand( const std::string & a_SQL ) : m_SQL( a_SQL )
		{}
		virtual ~ICommand()
		{}
		std::string		m_SQL;
		virtual void Execute( sqlite3 * a_pDB ) = 0;
	};
	typedef std::list<ICommand *>		CommandQueue;

	struct ExecCommand : public ICommand
	{
		ExecCommand( const std::string & a_SQL, Delegate<bool> a_Callback ) : ICommand( a_SQL ), m_Callback( a_Callback )
		{}

		Delegate<bool>	m_Callback;

		virtual void Execute( sqlite3 * a_pDB );
	};
	struct LoadCommand : public ICommand
	{
		LoadCommand( const std::string & a_SQL, Delegate<Json::Value *> a_Callback, const std::string & a_ID ) 
			: ICommand( a_SQL ), m_Callback( a_Callback ), m_ID( a_ID )
		{}

		Delegate<Json::Value *> m_Callback;
		std::string m_ID;

		virtual void Execute( sqlite3 * a_pDB );
	};
	struct QueryCommand : public ICommand
	{
		QueryCommand( const std::string & a_SQL, Delegate<QueryResults *> a_Callback ) :
			ICommand( a_SQL ), m_Callback( a_Callback )
		{}

		Delegate<QueryResults *> m_Callback;

		virtual void Execute( sqlite3 * a_pDB );
	};

	//! Data
	boost::mutex	m_DBLock;
	boost::condition_variable
					m_CommandQueued;
	CommandQueue	m_CommandQueue;
	volatile bool	m_bStop;
	volatile bool	m_bThreadStopped;

	sqlite3 *		m_pDB;
	std::string		m_DBFile;
	std::string		m_Table;
	Json::Value		m_Definition;

	static void ApplyDefinition(Json::Value & json, const Json::Value & def);
	static std::string GetWhereOp(Logic::EqualityOp a_Op);
	static std::string GetWhereClause(const Conditions & a_Conditions, Logic::LogicalOp a_LogOp = Logic::AND );

	void PushCommand( ICommand * a_pCommand );
	void ProcessCommandQueue();
};

#endif

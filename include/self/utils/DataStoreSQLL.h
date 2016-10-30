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

	//! IDataStore interface
	virtual bool Start(const std::string & a_DB,
		const Json::Value & a_DataDefinition);
	virtual bool Stop();

	virtual bool Save(const std::string & a_ID,
		const Json::Value & a_Data,
		Delegate<bool> a_Callback = Delegate<bool>());
	virtual bool Load(const std::string & a_ID,
		Delegate<const Json::Value &> a_Callback);
	virtual bool Delete(const std::string & a_ID,
		Delegate<bool> a_Callback = Delegate<bool>());
	virtual bool Find(const Conditions & a_Conditions,
		Delegate<const QueryResults &> a_Callback);

private:
	//! Data
	sqlite3 *		m_pDB;
	std::string		m_DBFile;
	std::string		m_Table;
	Json::Value		m_Definition;

	static void ApplyDefinition(Json::Value & json, const Json::Value & def);
	static std::string GetWhereOp(Logic::EqualityOp a_Op);
	static std::string GetWhereClause(const Conditions & a_Conditions, Logic::LogicalOp a_LogOp = Logic::AND );
};

#endif

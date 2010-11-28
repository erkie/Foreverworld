/*
 *  usermanager.cpp
 *  Spel
 *
 *  Created by Erik Andersson on 2010-11-24.
 *  Copyright 2010 Åva gymnasium. All rights reserved.
 *
 */

#include <mysql++.h>
#include "Common/messages.h"

#include "types.h"
#include "usermanager.h"

UserManager::UserManager(mysqlpp::Connection *conn): _db_conn(conn)
{
	
}

unsigned int UserManager::count()
{
	mysqlpp::Query query = _db_conn->query();
	query << "SELECT COUNT(*) FROM players";
	
	mysqlpp::StoreQueryResult res = query.store();
	return res[0][0];
}

unsigned int UserManager::count(const std::string &username)
{
	mysqlpp::Query query = _db_conn->query();
	query << "SELECT COUNT(*) FROM players"
		<< " WHERE username = " << mysqlpp::quote << username
		<< " LIMIT 1";
	
	return query.store()[0][0];
}

inet::id_type UserManager::registerPlayer(const std::string &username, const std::string &password, const std::string &email)
{
	if ( count(username) > 0 )
	{
		std::cout << "The user " << username << " was already registered\n";
		return 0;
	}
	
	mysqlpp::Query query = _db_conn->query();
	query << "INSERT INTO players (username, password, email, created)"
		<< " VALUES("
		<< mysqlpp::quote << username << ", "
		<< mysqlpp::quote << password << ", "
		<< mysqlpp::quote << email << ", "
		<< "NOW()"
		<< ")";
	
	mysqlpp::SimpleResult res = query.execute();
	return res.insert_id();
}

inet::id_type UserManager::login(const std::string &username, const std::string &password)
{
	mysqlpp::Query query = _db_conn->query();
	query << "SELECT id FROM players"
		<< " WHERE username = " << mysqlpp::quote << username
		<< " AND password = " << mysqlpp::quote << password
		<< " LIMIT 1";
	
	mysqlpp::StoreQueryResult res = query.store();
	if ( res.size() == 0 ) return 0;
	return res[0][0];
}

void UserManager::setCharacter(inet::id_type u_id, int32_t c_id)
{
	mysqlpp::Query query = _db_conn->query();
	query << "UPDATE players SET character_id = "
		<< mysqlpp::quote << c_id
		<< " WHERE id = "
		<< mysqlpp::quote << u_id << " LIMIT 1";
	
	query.execute();
}

inet::LoggedInMemberData UserManager::loadMember(inet::id_type id)
{
	mysqlpp::Query query = _db_conn->query();
	query << "SELECT * FROM players"
		<< " WHERE id = " << mysqlpp::quote << id
		<< " LIMIT 1";
	
	inet::LoggedInMemberData data;
	sql::player player = query.use().fetch_row();
	
	data.id = player.id;
	data.character_id = player.character_id;
	STR2USERNAME(data.username, player.username);
	
	return data;
}

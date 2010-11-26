/*
 *  charactermanager.cpp
 *  Spel
 *
 *  Created by Erik Andersson on 2010-11-26.
 *  Copyright 2010 Åva gymnasium. All rights reserved.
 *
 */

#include <vector>
#include <mysql++.h>

#define MYSQLPP_SSQLS_NO_STATICS
#include "types.h"
#include "charactermanager.h"

CharacterManager::CharacterManager(mysqlpp::Connection *conn): _db_conn(conn)
{
	
}

std::vector<sql::character> CharacterManager::fetchAll()
{
	mysqlpp::Query query = _db_conn->query();
	query << "SELECT * FROM characters ORDER BY id";
	
	std::vector<sql::character> ret;
	query.storein(ret);
	
	return ret;
}

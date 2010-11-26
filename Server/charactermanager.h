/*
 *  charactermanager.h
 *  Spel
 *
 *  Created by Erik Andersson on 2010-11-26.
 *  Copyright 2010 Åva gymnasium. All rights reserved.
 *
 */

#ifndef __CHARACTERMANAGER_H__
#define __CHARACTERMANAGER_H__

#include <vector>

namespace sql
{
	class character;
}

class CharacterManager
{
	mysqlpp::Connection *_db_conn;
public:
	CharacterManager(mysqlpp::Connection *);
	
	std::vector<sql::character> fetchAll();
};

#endif

/*
 *  usermanager.h
 *  Spel
 *
 *  Created by Erik Andersson on 2010-11-24.
 *  Copyright 2010 Åva gymnasium. All rights reserved.
 *
 */

#ifndef __USERMANAGER_H__
#define __USERMANAGER_H__

#include <mysql++.h>
#include "Common/messages.h"

namespace sql
{
	class player;
}

class UserManager
{
	mysqlpp::Connection *_db_conn;
public:
	UserManager(mysqlpp::Connection *conn);
	
	unsigned int count();
	unsigned int count(const std::string &username);
	
	inet::id_type registerPlayer(const std::string &username, const std::string &password, const std::string &email);
	inet::id_type login(const std::string &username, const std::string &password);
	
	inet::LoggedInMemberData loadMember(inet::id_type id);
};

#endif

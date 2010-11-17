/*
 *  user.cpp
 *  Spel
 *
 *  Created by Erik Andersson on 2010-11-16.
 *  Copyright 2010 Åva gymnasium. All rights reserved.
 *
 */

#include "Gaem/user.h"

namespace Gaem
{
	User::User(): _id(0)
	{
		
	}
	
	void User::setId(inet::id_type id)
	{
		_id = id;
	}
	
	void User::setUsername(const std::string &name)
	{
		_username = name;
	}
	
	inet::id_type User::getId()
	{
		return _id;
	}
	
	std::string User::getUsername()
	{
		return _username;
	}
}
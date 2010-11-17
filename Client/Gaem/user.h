/*
 *  user.h
 *  Spel
 *
 *  Created by Erik Andersson on 2010-11-16.
 *  Copyright 2010 Åva gymnasium. All rights reserved.
 *
 */

#ifndef __GAEM_USER_H__
#define __GAEM_USER_H__

#include <string>
#include "Common/messages.h"

namespace Gaem
{
	class User
	{
		inet::id_type _id;
		std::string _username;
	public:
		User();
		
		void setId(inet::id_type);
		inet::id_type getId();
		
		void setUsername(const std::string &username);
		std::string getUsername();
	};
}

#endif

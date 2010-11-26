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
	class ValidationException {};
	
	class User
	{
		inet::id_type _id;
		std::string _username;
		std::string _password;
		std::string _email;
		
		bool _is_logged_in;
	public:
		User();
		
		void setId(inet::id_type);
		inet::id_type getId();
		
		void setUsername(const std::string &username);
		std::string getUsername();
		void getNetworkUsername(char o[21]);
		
		void setPassword(const std::string &password);
		std::string getPassword();
		void getNetworkPassword(char o[41]);
		
		void setEmail(const std::string &email);
		std::string getEmail();
		void getNetworkEmail(char o[101]);
		
		void login(const std::string &username, const std::string &password);
		void signup(const std::string &username, const std::string &password, const std::string &email);
		
		void setIsLoggedIn(bool);
		bool isLoggedIn();
	};
}

#endif

/*
 *  user.cpp
 *  Spel
 *
 *  Created by Erik Andersson on 2010-11-16.
 *  Copyright 2010 Åva gymnasium. All rights reserved.
 *
 */

#include <iostream>

#include "My/sha1.h"

#include "Gaem/gaem.h"
#include "Gaem/networkmanager.h"
#include "Gaem/user.h"

namespace Gaem
{
	User::User(): _id(0), _username(""), _password(""), _is_logged_in(false)
	{
		
	}
	
	void User::setId(inet::id_type id)
	{
		_id = id;
	}
	
	inet::id_type User::getId()
	{
		return _id;
	}
		
	void User::setUsername(const std::string &name)
	{
		if ( name.size() > 20 )
			throw new ValidationException;
		_username = name;
	}
	
	std::string User::getUsername()
	{
		return _username;
	}
	
	void User::getNetworkUsername(char o[21])
	{
		memset(o, 0, 21);
		for ( unsigned int i = 0; i < _username.size() && i < 20; i++ )
			o[i] = _username[i];
		o[20] = '\0';
	}
	
	void User::setPassword(const std::string &pass)
	{
		if ( pass.size() == 0 )
			throw new ValidationException;
		
		SHA1 crypt;
		crypt.addBytes(pass.c_str(), pass.size());
		unsigned char* digest = crypt.getDigest();
		_password = SHA1::toString(digest, 20);
		free(digest);
	}
	
	std::string User::getPassword()
	{
		return _password;
	}
	
	void User::getNetworkPassword(char o[41])
	{
		memset(o, 0, 41);
		for ( unsigned int i = 0; i < _password.size() && i < 40; i++ )
			o[i] = _password[i];
		o[40] = '\0';
	}
	
	void User::setEmail(const std::string &mail)
	{
		if ( mail.size() > 100 )
			throw new ValidationException;
		_email = mail;
	}
	
	std::string User::getEmail()
	{
		return _email;
	}
	
	void User::getNetworkEmail(char o[101])
	{
		memset(o, 0, 101);
		for ( unsigned int i = 0; i < _email.size() && i < 101; i++ )
			o[i] = _email[i];
		o[100] = '\0';
	}

	void User::setMember(inet::LoggedInMemberData data)
	{
		_member = data;
	}
	
	inet::LoggedInMemberData User::getMember()
	{
		return _member;
	}

	void User::login(const std::string &username, const std::string &password)
	{
		setUsername(username);
		setPassword(password);
		
		Gaem::Gaem::getInstance()->getNetworkManager()->login(this);
	}
	
	void User::signup(const std::string &username, const std::string &password, const std::string &email)
	{
		setUsername(username);
		setPassword(password);
		setEmail(email);
		
		Gaem::Gaem::getInstance()->getNetworkManager()->signup(this);
	}
	
	void User::saveCharacter(int32_t id)
	{
		Gaem::Gaem::getInstance()->getNetworkManager()->saveCharacter(this, id);
	}
	
	void User::setIsLoggedIn(bool is)
	{
		_is_logged_in = is;
	}
	
	bool User::isLoggedIn()
	{
		return _is_logged_in;
	}
}
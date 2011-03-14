/*
 *  messages.cpp
 *  Spel
 *
 *  Created by Erik Andersson on 2010-11-26.
 *  Copyright 2010 Åva gymnasium. All rights reserved.
 *
 */

#include <string>
#include <sstream>
#include <iostream>

#include "Common/messages.h"

namespace inet
{
	inet::Version getVersion()
	{
		inet::Version version = {0, 8, 'a'};
		return version;
	}
	
	std::string getVersionString(const inet::Version &v)
	{
		std::stringstream ss;
		ss << v.major << '.' << v.minor << v.type;
		return ss.str();
	}
	
	unsigned char getPacketIdentifier(RakNet::Packet *p)
	{
		if ((unsigned char)p->data[0] == ID_TIMESTAMP)
			return (unsigned char) p->data[sizeof(unsigned char) + sizeof(RakNet::Time)];
		else
			return (unsigned char) p->data[0];
	}
}

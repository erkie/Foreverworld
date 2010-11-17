/*
 *  server.h
 *  Spel
 *
 *  Created by Erik Andersson on 2010-11-05.
 *  Copyright 2010 Åva gymnasium. All rights reserved.
 *
 */

#ifndef __SERVER_H__
#define __SERVER_H__

#include <map>
#include <queue>

#include "RakPeerInterface.h"
#include "MessageIdentifiers.h"
#include "Common/messages.h"

#include "player.h"

class Server
{
	unsigned short _port;
	unsigned int _max_players;
	
	unsigned long _current_id;
	
	typedef std::map<unsigned long, Player*> player_map;
	
	player_map _players;
	std::map<RakNet::RakNetGUID, unsigned long> _player_id_table;
	
	std::queue<Player*> _new_players;
	std::queue<RakNet::RakNetGUID> _removed_players;
	
	RakNet::RakPeerInterface *_peer;
public:
	Server(unsigned short port, unsigned int max_players);
	~Server();
	
	void run();
	void handlePacket();
	
	Player *addPlayer(const std::string &name, RakNet::RakNetGUID guid);
	void removePlayer(RakNet::RakNetGUID guid);
};

#endif

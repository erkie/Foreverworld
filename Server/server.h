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
#include <vector>
#include <mysql++.h>

#include "RakPeerInterface.h"
#include "MessageIdentifiers.h"
#include "Common/messages.h"

#include "player.h"
#include "usermanager.h"
#include "charactermanager.h"

namespace sql
{
	class character;
}

class Server
{
	unsigned short _port;
	unsigned int _max_players;
	
	unsigned long _current_id;
	
	typedef std::map<unsigned long, Player*> player_map;
	
	player_map _players;
	std::map<RakNet::RakNetGUID, unsigned long> _player_id_table;

	std::queue<Player*> _new_players;
	
	std::vector<sql::character> _characters;
	
	// GUID:s are used because the user may have already disconnected when it is time to update
	std::queue<RakNet::RakNetGUID> _removed_players;
	std::queue<RakNet::RakNetGUID> _updated_players;
	std::queue<RakNet::RakNetGUID> _character_updates;
	
	RakNet::RakPeerInterface *_peer;
	
	mysqlpp::Connection  *_db_conn;
	UserManager *_user_manager;
	CharacterManager *_character_manager;
public:
	Server(unsigned short port, unsigned int max_players);
	~Server();
	
	void run();
	void handlePacket();
	
	Player *addPlayer(inet::id_type, RakNet::RakNetGUID guid);
	void removePlayer(RakNet::RakNetGUID guid);
	
	Player *getPlayerByGUID(RakNet::RakNetGUID guid);
};

#endif

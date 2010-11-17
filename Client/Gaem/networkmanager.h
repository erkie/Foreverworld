/*
 *  networkmanager.h
 *  Spel
 *
 *  Created by Erik Andersson on 2010-11-12.
 *  Copyright 2010 Åva gymnasium. All rights reserved.
 *
 */

#ifndef __GAEM_NETWORKMANAGER_H__
#define __GAEM_NETWORKMANAGER_H__

#include "RakNetTypes.h"
#include "RakPeerInterface.h"

#include "Common/messages.h"

#include "Entities/player.h"

namespace Gaem
{
	class NetworkManager
	{
		RakNet::RakPeerInterface *_peer;
		RakNet::SystemAddress _hostaddr;
		
		bool _is_connected;
		inet::id_type _id;
		
		void sendPacket(char *data, std::size_t size);
	public:
		NetworkManager();
		~NetworkManager();
		
		// Connect to server and start receiving updates
		void connect();
		
		// Add my character to the playing field and start playing
		void joinGame(const std::string &player);
		void addPlayer(const inet::PlayerAdded *);
		void removePlayer(const inet::PlayerRemoved *player);
		
		// An event, running, jumping, hitting, defending, anything involving the keyboard
		void event(Entities::Player *player);
		
		void sendChatMessage();
		void registerNewMember();
		void login();
		
		// Get all updates sent to me since the latest frame
		void getUpdates();
		
		void sendUpdate();
		
		bool isConnected();
		int getPing();
		inet::id_type getId();
	};
}

#endif

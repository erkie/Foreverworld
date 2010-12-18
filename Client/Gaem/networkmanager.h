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

namespace Entities
{
	class Player;
}

namespace Gaem
{
	class User;
	
	class NetworkManager
	{
		RakNet::RakPeerInterface *_peer;
		RakNet::SystemAddress _hostaddr;
		
		bool _is_connected, _has_connected;
		inet::id_type _id;
		
		std::string _latest_news_text;
		std::string _latest_news_posted;
		
		void sendPacket(char *data, std::size_t size);
	public:
		NetworkManager();
		~NetworkManager();
		
		// Connect to server and start receiving updates
		void connect();
		
		// Add my character to the playing field and start playing
		void login(User *);
		
		void addPlayer(const inet::PlayerAdded *);
		void removePlayer(const inet::PlayerRemoved *player);
		void characterUpdated(const inet::LoggedInMemberData);
		
		// An event, running, jumping, hitting, defending, anything involving the keyboard
		void event(Entities::Player *player);
		void handleEvent(inet::EventUpdate *);
		
		void sendChatMessage(const std::string &);
		void handleChatMessage(inet::ChatMessage *);
		
		void signup(User *);
		void saveCharacter(User *, int32_t id);
		
		// Get all updates sent to me since the latest frame
		void getUpdates();
		
		
		
		void setNews(std::string posted, std::string text);
		std::string getNewsText();
		std::string getNewsPosted();
		
		void sendUpdate();
		
		bool isConnected();
		int getPing();
		inet::id_type getId();
	};
}

#endif

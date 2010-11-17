/*
 *  networkmanager.cpp
 *  Spel
 *
 *  Created by Erik Andersson on 2010-11-12.
 *  Copyright 2010 Åva gymnasium. All rights reserved.
 *
 */

#include <cstdlib>
#include <iostream>

#include <RakNetTypes.h>
#include <RakPeerInterface.h>
#include <MessageIdentifiers.h>
#include <BitStream.h>

#include "Common/messages.h"

#include "Gaem/gaem.h"
#include "Gaem/config.h"
#include "Gaem/user.h"
#include "Gaem/entitymanager.h"

#include "Entities/player.h"

#include "Gaem/networkmanager.h"

namespace Gaem
{
	NetworkManager::NetworkManager(): _is_connected(false), _id(0)
	{
		RakNet::SocketDescriptor socket;
		
		_peer = RakNet::RakPeerInterface::GetInstance();
		_peer->Startup(1, &socket, 1);
		_peer->SetMaximumIncomingConnections(1);
	}
	
	NetworkManager::~NetworkManager()
	{
		_peer->Shutdown(100, 0, IMMEDIATE_PRIORITY);
	}
	
	void NetworkManager::connect()
	{
		const std::string host = Gaem::Gaem::getInstance()->getConfig()->get("server_host");
		unsigned short port = Gaem::Gaem::getInstance()->getConfig()->getInt("port", 6010);
		
		_peer->Connect(host.c_str(), port, 0, 0);
		
		_hostaddr = RakNet::SystemAddress(host.c_str(), port);
		_peer->Ping(_hostaddr);
	}
	
	void NetworkManager::joinGame(const std::string &character)
	{
		if ( _id ) return;
		
		inet::StartGame data;
		data.type = inet::MESS_START_GAME;
		memcpy(data.character, character.c_str(), 50);
		
		_peer->Send((char*)&data, sizeof(data), HIGH_PRIORITY, RELIABLE_ORDERED, 0, _hostaddr, false);
	}
	
	void NetworkManager::event(Entities::Player *player)
	{
		// They be trollin'?
		if ( ! player ) return;
		
		inet::EventBasic ev;
		ev.type = inet::MESS_EVENT;
		ev.state = player->getPlayerStruct();
		
		_peer->Send((char*)&ev, sizeof(ev), HIGH_PRIORITY, UNRELIABLE, 0, _hostaddr, false);
	}
	
	void NetworkManager::handleEvent(inet::EventUpdate *event)
	{
		if ( event->id == _id )
		{
			std::cout << "This is me\n";
		}
		else
		{
			Gaem::Gaem::getInstance()->getEntityManager()->updatePlayer(event->id, event->state);
		}
	}
	
	void NetworkManager::addPlayer(const inet::PlayerAdded *player)
	{
		Gaem::Gaem::getInstance()->getEntityManager()->addPlayer(player->id, player->player);
	}
	
	void NetworkManager::removePlayer(const inet::PlayerRemoved *player)
	{
		Gaem::Gaem::getInstance()->getEntityManager()->removePlayer(player->id);
	}
	
	void NetworkManager::getUpdates()
	{
		RakNet::Packet *packet;
		
		for ( packet = _peer->Receive(); packet; _peer->DeallocatePacket(packet), packet = _peer->Receive() )
		{
			switch (packet->data[0])
			{
				case ID_CONNECTION_REQUEST_ACCEPTED:
					std::cout << "We are offically connected to " << packet->systemAddress.ToString() << "\n";
					_is_connected = true;
					joinGame("player_1");
					break;
				
				// Response after I sent a START_GAME message
				case inet::MESS_SUCCESSFULLY_ADDED:
					// Got my ID back, and they have it stored too
					// I should now receive a player-added notification
					_id = ((inet::SuccessfullyAdded*)packet->data)->id;
					Gaem::Gaem::getInstance()->getUser()->setId(_id);
					std::cout << "My ID is " << _id << "\n";
					break;
				
				// A new player has been added
				case inet::MESS_NEW_PLAYER:
					std::cout << "New player!\n";
					addPlayer((inet::PlayerAdded*)packet->data);
					break;
				
				case inet::MESS_REMOVED_PLAYER:
					std::cout << "A player has left\n";
					removePlayer((inet::PlayerRemoved*)packet->data);
				break;
				
				case inet::MESS_EVENT:
					handleEvent((inet::EventUpdate*)packet->data);
					break;
				
				// Failures
				case ID_CONNECTION_ATTEMPT_FAILED:
					std::cout << "Failed to connect to server\n";
					break;
					
				case ID_DISCONNECTION_NOTIFICATION:
					_is_connected = false;
					std::cout << "We have been disconnected. Reconnect?\n";
					break;
					
				case ID_CONNECTION_LOST:
					_is_connected = false;
					std::cout << "We have lost our connection. Reconnect?\n";
					connect();
					break;

				default:
					std::cout << "Got unspecified message " << (int)packet->data[0] << std::endl;
					break;
			}
		}
	}
	
	bool NetworkManager::isConnected()
	{
		return _is_connected;
	}
	
	int NetworkManager::getPing()
	{
		return _peer->GetAveragePing(_hostaddr);
	}
	
	inet::id_type NetworkManager::getId()
	{
		return _id;
	}
}

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
#include "Gaem/menumanager.h"

#include "Entities/player.h"
#include "Entities/chat.h"

#include "Gaem/networkmanager.h"

namespace Gaem
{
	NetworkManager::NetworkManager(): _is_connected(false), _id(0)
	{
		RakNet::SocketDescriptor socket;
		
		_peer = RakNet::RakPeerInterface::GetInstance();
		_peer->Startup(1, &socket, 1);
		_peer->SetMaximumIncomingConnections(1);
		_peer->SetOccasionalPing(true);
	}
	
	NetworkManager::~NetworkManager()
	{
		_peer->Shutdown(100, 0, IMMEDIATE_PRIORITY);
	}
	
	void NetworkManager::connect()
	{
		const std::string host = Gaem::Gaem::getInstance()->getConfig()->get("server_host");
		unsigned short port = Gaem::Gaem::getInstance()->getConfig()->getInt("server_port", 6010);
		
		_peer->Connect(host.c_str(), port, 0, 0);
		
		_hostaddr = RakNet::SystemAddress(host.c_str(), port);
	}
	
	void NetworkManager::login(User *user)
	{
		if ( _id ) return;
		
		inet::LoginPlayer data;
		data.type = inet::MESS_LOGIN_PLAYER;
		user->getNetworkUsername(data.login.username);
		user->getNetworkPassword(data.login.password);
		
		_peer->Send((char*)&data, sizeof(data), HIGH_PRIORITY, RELIABLE_ORDERED, 0, _hostaddr, false);
	}
	
	void NetworkManager::signup(User *user)
	{
		if ( _id ) return;
		
		inet::RegisterPlayer data;
		data.type = inet::MESS_REGISTER_PLAYER;
		
		user->getNetworkUsername(data.member.username);
		user->getNetworkPassword(data.member.password);
		user->getNetworkEmail(data.member.email);
		
		_peer->Send((char*)&data, sizeof(data), HIGH_PRIORITY, RELIABLE_ORDERED, 0, _hostaddr, false);
	}
	
	void NetworkManager::saveCharacter(User *user, int32_t id)
	{
		inet::CharacterUpdate mess;
		mess.type = inet::MESS_CHARACTER_UPDATE;
		mess.member.id = user->getId();
		mess.member.username[0] = '\0';
		mess.member.character_id = id;
		
		_peer->Send((char*)&mess, sizeof(mess), HIGH_PRIORITY, RELIABLE_ORDERED, 0, _hostaddr, false);
	}
	
	void NetworkManager::sendChatMessage(const std::string &message)
	{
		inet::ChatMessage mess;
		mess.type = inet::MESS_CHAT_MESSAGE;
		strcpy(mess.message, message.c_str());
		mess.message[sizeof(mess.message)-1] = '\0';
		mess.user_id = Gaem::Gaem::getInstance()->getUser()->getId();
		
		_peer->Send((char*)&mess, sizeof(mess), MEDIUM_PRIORITY, RELIABLE_ORDERED, 0, _hostaddr, false);
	}
	
	void NetworkManager::handleChatMessage(inet::ChatMessage *mess)
	{
		std::string username = Gaem::getInstance()->getEntityManager()->getUsernameById(mess->user_id);
		Gaem::getInstance()->getEntityManager()->getChat()->addMessage(username, mess->message);
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
		Gaem::Gaem::getInstance()->getEntityManager()->addPlayer(player->id, player->player, player->member);
	}
	
	void NetworkManager::removePlayer(const inet::PlayerRemoved *player)
	{
		Gaem::Gaem::getInstance()->getEntityManager()->removePlayer(player->id);
	}
	
	void NetworkManager::characterUpdated(const inet::LoggedInMemberData data)
	{
		Gaem::Gaem::getInstance()->getEntityManager()->updateCharacter(data.id, data.character_id);
	}
	
	void NetworkManager::getUpdates()
	{
		RakNet::Packet *packet;
		
		for ( packet = _peer->Receive(); packet; _peer->DeallocatePacket(packet), packet = _peer->Receive() )
		{
			switch (packet->data[0])
			{
				case ID_CONNECTION_REQUEST_ACCEPTED: {
					std::cout << "We are connected to " << packet->systemAddress.ToString() << "\n";
					_is_connected = true;
					
					// Send version test
					inet::VersionCheck version;
					version.type = inet::MESS_VERSION_CHECK;
					version.version = inet::getVersion();
					
					_peer->Send((char*)&version, sizeof(version), HIGH_PRIORITY, RELIABLE_ORDERED, 0, _hostaddr, false);
					} break;
				
				// Got the version the server is running back
				// They have to match
				case inet::MESS_VERSION_CHECK: {
					inet::VersionCheck *response = (inet::VersionCheck*)packet->data;
					inet::Version current = inet::getVersion();
					
					if ( response->version.major != current.major || response->version.minor != current.minor )
					{
						throw GAEM_NONFATAL_EXCEPTION("The latest version is " + inet::getVersionString(response->version) + " and you are running " + inet::getVersionString(current) + ". Please visit the website and upgrade your client.");
					}
				} break;
				
				// Response after I sent a login message
				case inet::MESS_LOGIN_STATUS: {
					// Got my ID back, and they have it stored too
					// I should now receive a player-added notification
					inet::LoginStatus* data = (inet::LoginStatus*)packet->data;
					
					Gaem::Gaem::getInstance()->getMenuManager()->hideLoading();
					
					// Successfully logged in
					if ( data->id )
					{
						Gaem::Gaem::getInstance()->getMenuManager()->hideAll();
						_id = data->id;
						
						Gaem::Gaem::getInstance()->getUser()->setId(_id);
						Gaem::Gaem::getInstance()->getUser()->setIsLoggedIn(true);
					}
					// Failed login attempt
					else
					{
						Gaem::Gaem::getInstance()->getMenuManager()->alert("Failed login. Please try again.");
					}
					} break;
				
				// Response after a register attempt
				case inet::MESS_REGISTER_STATUS: {
					// Just a bool back wether I did it or not
					inet::SuccessfullyRegistered *data = (inet::SuccessfullyRegistered*)packet->data;
					
					Gaem::Gaem::getInstance()->getMenuManager()->hideLoading();
					
					if ( data->succeeded )
					{
						Gaem::Gaem::getInstance()->getMenuManager()->alert("Welcome! We love you. Let's log on");
						login(Gaem::Gaem::getInstance()->getUser());
					}
					else
					{
						Gaem::Gaem::getInstance()->getMenuManager()->alert("The username requested was already taken");
					}
				} break;
				
				// A new player has been added
				case inet::MESS_NEW_PLAYER:
					addPlayer((inet::PlayerAdded*)packet->data);
					break;
				
				// A player has disconnected/left
				case inet::MESS_REMOVED_PLAYER:
					removePlayer((inet::PlayerRemoved*)packet->data);
					break;
				
				// Information about a character
				case inet::MESS_CHARACTER_DATA: {
					inet::CharacterData *data = (inet::CharacterData*)packet->data;
					Gaem::Gaem::getInstance()->getEntityManager()->addCharacter(data->character);
					} break;
				
				case inet::MESS_EVENT:
					handleEvent((inet::EventUpdate*)packet->data);
					break;
				
				// Someone update his/her character
				case inet::MESS_CHARACTER_UPDATE:
					characterUpdated(((inet::CharacterUpdate*)packet->data)->member);
					break;
				
				// Latest news update
				case inet::MESS_NEWS: {
					inet::News *news = (inet::News*)packet->data;
					setNews(news->posted, news->text);
					} break;
				
				// A chat message appeared!
				case inet::MESS_CHAT_MESSAGE: {
					inet::ChatMessage *mess = (inet::ChatMessage*)packet->data;
					handleChatMessage(mess);
					} break;
				
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
	
	void NetworkManager::setNews(std::string posted, std::string text)
	{
		_latest_news_text = text;
		_latest_news_posted = posted;
	}
	
	std::string NetworkManager::getNewsText()
	{
		return _latest_news_text;
	}
	
	std::string NetworkManager::getNewsPosted()
	{
		return _latest_news_posted;
	}
	
	bool NetworkManager::isConnected()
	{
		return _is_connected;
	}
	
	int NetworkManager::getPing()
	{
		return _peer->GetLastPing(_hostaddr);
	}
	
	inet::id_type NetworkManager::getId()
	{
		return _id;
	}
}

/*
 *  server.cpp
 *  Spel
 *
 *  Created by Erik Andersson on 2010-11-05.
 *  Copyright 2010 Åva gymnasium. All rights reserved.
 *
 */

#include <iostream>
#include <vector>
#include <SFML/System.hpp>
#include <mysql++.h>

#include "RakString.h"
#include "BitStream.h"

#include "server.h"
#include "player.h"

Server::Server(unsigned short port, unsigned int max_players): _port(port), _max_players(max_players), _current_id(1)
{
	RakNet::SocketDescriptor socket(port, 0);
	
	std::cout << "... Creating network interface\n";
	
	// Bind to port
	_peer = RakNet::RakPeerInterface::GetInstance();
	_peer->Startup(max_players, &socket, 1);
	_peer->SetMaximumIncomingConnections(max_players);
	
	std::cout << "... Connecting to database\n";
	
	_db_conn = new mysqlpp::Connection("foreverworld", "localhost", "root", "");
	mysqlpp::Query query = _db_conn->query();
	query << "SELECT * FROM players";
	
	mysqlpp::StoreQueryResult res = query.store();
	std::cout << "Number of players: " << res.num_rows() << std::endl;
	
	std::cout << "... Loading characters\n";
	
	_character_manager = new CharacterManager(_db_conn);
	_characters = _character_manager->fetchAll();
	
	_user_manager = new UserManager(_db_conn);
	Player::user_manager = _user_manager;
	
	// Set the latest news' id to 0 to say it is the first run 
	_latest_news.id = 0;
}

Server::~Server()
{
	RakNet::RakPeerInterface::DestroyInstance(_peer);
	delete _db_conn;
	delete _user_manager;
}

void Server::run()
{
	std::cout << "... Listening for clients\n";
	
	RakNet::Packet *packet;
	while (true)
	{
		// Do we need to ping the database?
		// Otherwise MySQL will drop the connection every couple of hours
		if ( _last_db_ping.GetElapsedTime() > 60 * 60 )
		{
			std::cout << "... Pinging database\n";
			_last_db_ping.Reset();
			_db_conn->ping();
		}
		
		// Fetch the latest news every 15 minutes (or if it is the first run)
		if ( _last_news_fetch.GetElapsedTime() > 60 * 15 || _latest_news.id == 0 )
		{
			std::cout << "... Fetching news\n";
			_last_news_fetch.Reset();
			mysqlpp::Query query = _db_conn->query("SELECT * FROM news ORDER BY id DESC LIMIT 1");
			std::vector<sql::news> res;
			query.storein(res);
			
			_latest_news = res[0];
		}
	
		// Get new messages and handle them
		
		for ( packet = _peer->Receive(); packet; _peer->DeallocatePacket(packet), packet = _peer->Receive())
		{
			switch (packet->data[0]) {
				case ID_REMOTE_DISCONNECTION_NOTIFICATION:
					// disconnection
					std::cout << "Another client has connected\n";
					break;
				
				case ID_REMOTE_CONNECTION_LOST:
					// lost client
					std::cout << "Another client has lost its connection\n";
					break;
				
				case ID_REMOTE_NEW_INCOMING_CONNECTION:
					// new incomming connection
					std::cout << "Another client has connected\n";
					break;
				
				case ID_CONNECTION_REQUEST_ACCEPTED:
					// requested a connection
					std::cout << "Our connection request has been accepted\n";
					break;
				
				case ID_NEW_INCOMING_CONNECTION: {
					// an incoming connection
					std::cout << "A connection is incoming\n";
					
					// Send all characters to the client
					for ( std::vector<sql::character>::iterator iter = _characters.begin(); iter != _characters.end(); ++iter )
					{
						const sql::character &c = (*iter);
						
						inet::CharacterData mess;
						mess.type = inet::MESS_CHARACTER_DATA;
						mess.character.id = c.id;
						mess.character.scale = c.scale;
						mess.character.can_jump = c.can_jump;
						mess.character.speed = c.speed;
						mess.character.up_speed = c.up_speed;
						memset(mess.character.name, 0, 21);
						memcpy(mess.character.name, c.name.c_str(), 20);
						memset(mess.character.slug, 0, 21);
						memcpy(mess.character.slug, c.slug.c_str(), 20);
						
						_peer->Send((char *)&mess, sizeof(mess), HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);
					}
					
					// Send a list of all players to the newly added player
					for ( player_map::iterator iter = _players.begin(); iter != _players.end(); ++iter )
					{
						Player *p = (*iter).second;
						
						inet::PlayerAdded mess;
						mess.type = inet::MESS_NEW_PLAYER;
						mess.id = p->getId();
						mess.player = p->getState();
						mess.member = p->getMember();
						
						_peer->Send((char *)&mess, sizeof(mess), MEDIUM_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);
					}
					
					// Send the latest news to the user
					inet::News mess;
					mess.type = inet::MESS_NEWS;
					memcpy(mess.posted, std::string(_latest_news.posted).c_str(), 19);
					memcpy(mess.text, _latest_news.text.c_str(), 1000);
					
					mess.text[999] = mess.posted[19] = '\0';
					
					_peer->Send((char *)&mess, sizeof(mess), LOW_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);
					} break;
				
				case ID_NO_FREE_INCOMING_CONNECTIONS:
					// full as hell
					std::cout << "The server is full, hoorah!\n";
					break;
				
				case ID_DISCONNECTION_NOTIFICATION:
					// a client has disconnected
					std::cout << "A client has disconnected\n";
					_removed_players.push(packet->guid);
					break;
					
				case ID_CONNECTION_LOST:
					// a client has lost connection, remove the sorry bastard
					std::cout << "A client lost connection\n";
					_removed_players.push(packet->guid);
					break;
				
				case inet::MESS_VERSION_CHECK: {
					inet::VersionCheck response;
					response.type = inet::MESS_VERSION_CHECK;
					response.version = inet::getVersion();
					
					_peer->Send((char *)&response, sizeof(response), HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);
					} break;
				
				case inet::MESS_LOGIN_PLAYER: {
					inet::LoginPlayer *mess = (inet::LoginPlayer*)packet->data;
					
					inet::id_type id = _user_manager->login(mess->login.username, mess->login.password);
					
					// Send a message to the user how it went
					inet::LoginStatus response;
					response.type = inet::MESS_LOGIN_STATUS;
					response.id = id;
					
					_peer->Send((char *)&response, sizeof(response), HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);
					
					// If it was successful, send a PlayerAdded-message to everyone
					if ( id )
					{
						Player *player = addPlayer(id, packet->guid);
						_new_players.push(player);
					}
				} break;
				
				case inet::MESS_REGISTER_PLAYER: {
					inet::RegisterPlayer *mess = (inet::RegisterPlayer*)packet->data;
					
					std::cout << mess->member.username << " wants to register\n";
					
					// Add player to database (if everything is OK)
					inet::id_type id = _user_manager->registerPlayer(mess->member.username, mess->member.password, mess->member.email);
					
					std::cout << "Registered him and got the id " << id << " back\n";
					
					inet::SuccessfullyRegistered response;
					response.type = inet::MESS_REGISTER_STATUS;
					response.succeeded = id > 0;
					
					_peer->Send((char *)&response, sizeof(response), HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);
				} break;
				
				case inet::MESS_EVENT: {
					inet::EventBasic *event = (inet::EventBasic*)packet->data;

					inet::id_type id = _player_id_table[packet->guid];
					if ( ! id )
					{
						// That user is not connected. Connect him?
						std::cout << "A user who is not connected is sending updates to me. Help, I'm scared\n";
						break;
					}
					
					Player *player = _players[id];
					
					// Update my definition of the player
					player->setDir(event->state.dir[0], event->state.dir[1]);
					player->setLeft(event->state.left);
					player->setElevation(event->state.elevation);
					player->setDepth(event->state.depth);
					player->setState(event->state.state);
					player->setVelocity(event->state.velocity[0], event->state.velocity[1]);
					
					// Send out my definition of the players stats to other players
					_updated_players.push(packet->guid);
				} break;
				
				case inet::MESS_CHARACTER_UPDATE: {
					inet::CharacterUpdate *mess = (inet::CharacterUpdate*)packet->data;
					
					inet::id_type id = _player_id_table[packet->guid];
					if ( ! id )
					{
						std::cout << "A user who is not connected wants to update his character. " << packet->guid.ToString() << '\n';
						break;
					}
					
					_user_manager->setCharacter(id, mess->member.character_id);
					_players[id]->setCharacter(mess->member.character_id);
					
					_character_updates.push(packet->guid);
				} break;
				
				case inet::MESS_CHAT_MESSAGE: {
					inet::ChatMessage *mess = (inet::ChatMessage*)packet->data;
					
					inet::id_type id = _player_id_table[packet->guid];
					if ( ! id )
					{
						std::cout << "A user who is not connected whats to send a chat message. " << packet->guid.ToString() << "\n";
						break;
					}
					
					// Log chat message
					std::cout << _players[id]->getUsername() << ": " << mess->message << '\n';
					
					mess->user_id = id;
					_chat_messages.push(*mess);
				} break;
				
				default:
					std::cout << "Wat? " << (char)packet->data[0] << "\n";
				break;
			}
		}
		
		// Send out packets that need to be sent
		// --
		
		// New players
		while ( ! _new_players.empty() )
		{
			Player *player = _new_players.front();
			
			inet::PlayerAdded response;
			response.type = inet::MESS_NEW_PLAYER;
			response.id = player->getId();
			response.player = player->getState();
			response.member = player->getMember();
			
			_peer->Send((char *)&response, sizeof(response), HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
			
			_new_players.pop();
		}
		
		// Removed players who have disconnected/dropped out
		while ( ! _removed_players.empty() )
		{
			RakNet::RakNetGUID guid = _removed_players.front();
			_removed_players.pop();
			
			if ( ! _player_id_table[guid] )
				continue;
			
			unsigned long id = _player_id_table[guid];
			
			inet::PlayerRemoved response;
			response.type = inet::MESS_REMOVED_PLAYER;
			response.id = id;
			
			delete _players[id];
			_players.erase(id);
			_player_id_table.erase(guid);
			
			_peer->Send((char *)&response, sizeof(response), HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
		}
		
		// Send updates about players who claim to have updated themselves
		while ( ! _updated_players.empty() )
		{
			RakNet::RakNetGUID guid = _updated_players.front();
			Player *player = getPlayerByGUID(guid);
			_updated_players.pop();
			
			if ( ! player )
				continue;
			
			inet::EventUpdate ev;
			ev.type = inet::MESS_EVENT;
			ev.state = player->getState();
			ev.id = player->getId();
			
			_peer->Send((char *)&ev, sizeof(ev), MEDIUM_PRIORITY, UNRELIABLE, 0, guid, true);
		}
		
		// Send updates about updated characters
		while ( ! _character_updates.empty() )
		{
			RakNet::RakNetGUID guid = _character_updates.front();
			Player *player = getPlayerByGUID(guid);
			_character_updates.pop();
			
			if ( ! player )
				continue;
			
			inet::CharacterUpdate mess;
			mess.type = inet::MESS_CHARACTER_UPDATE;
			mess.member = player->getMember();
			
			_peer->Send((char *)&mess, sizeof(mess), MEDIUM_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
		}
		
		// Send new chat messages, just echo them out to everybody
		while ( ! _chat_messages.empty() )
		{
			_peer->Send((char *)&(_chat_messages.front()), sizeof(inet::ChatMessage), MEDIUM_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
			_chat_messages.pop();
		}
		
		sf::Sleep(0.001f);
	}
}

Player *Server::addPlayer(inet::id_type id, RakNet::RakNetGUID guid)
{
	Player *player = new Player(id);
	
	_players[id] = player;
	_player_id_table[guid] = id;
	
	return player;
}

Player *Server::getPlayerByGUID(RakNet::RakNetGUID guid)
{
	if ( ! _player_id_table[guid] )
		return NULL;
	
	unsigned long id = _player_id_table[guid];	
	return _players[id];
}

/*
 *  server.cpp
 *  Spel
 *
 *  Created by Erik Andersson on 2010-11-05.
 *  Copyright 2010 Åva gymnasium. All rights reserved.
 *
 */

#include <iostream>
#include <SFML/System.hpp>

#include "RakString.h"
#include "BitStream.h"

#include "server.h"
#include "player.h"

Server::Server(unsigned short port, unsigned int max_players): _port(port), _max_players(max_players), _current_id(1)
{
	RakNet::SocketDescriptor socket(port, 0);
	
	// Bind to port
	_peer = RakNet::RakPeerInterface::GetInstance();
	_peer->Startup(max_players, &socket, 1);
	_peer->SetMaximumIncomingConnections(max_players);
}

Server::~Server()
{
	RakNet::RakPeerInterface::DestroyInstance(_peer);
}

void Server::run()
{
	std::cout << "Listening for clients...\n";
	
	RakNet::Packet *packet;
	while (true)
	{
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
				
				case ID_NEW_INCOMING_CONNECTION:
					// an incoming connection
					std::cout << "A connection is incoming\n";
					break;
				
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
				
				case inet::MESS_START_GAME: {
					inet::StartGame *mess = (inet::StartGame*)packet->data;
					
					// Add player and...
					// Send message back to user that she was successfully added, together with her id
					// And then the players already on the playing field
					Player *player = addPlayer(mess->character, packet->guid);
					
					inet::SuccessfullyAdded response;
					response.type = inet::MESS_SUCCESSFULLY_ADDED;
					response.id = player->getId();
					response.player = player->getPlayer();
					
					_new_players.push(player);
					_peer->Send((char *)&response, sizeof(response), HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);
					
					// Send a list of all players to the newly added player
					for ( player_map::iterator iter = _players.begin(); iter != _players.end(); iter++ )
					{
						Player *p = (*iter).second;
						if ( player == p ) continue;
						
						inet::PlayerAdded mess;
						mess.type = inet::MESS_NEW_PLAYER;
						mess.id = p->getId();
						mess.player = p->getPlayer();
						
						_peer->Send((char *)&mess, sizeof(response), HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);
					}
					
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
					player->setDir(event->state.dir);
					player->setLeft(event->state.left);
					player->setElevation(event->state.elevation);
					player->setDepth(event->state.depth);
					player->setState(event->state.state);
					
					// Send out my definition of the players stats to other players
					_updated_players.push(packet->guid);
				} break;
				
				default:
					std::cout << "Wat? " << packet->data[0] << "\n";
				break;
			}
		}
		
		// Send out packets that need to be sent
		while ( ! _new_players.empty() )
		{
			Player *player = _new_players.front();
			
			inet::PlayerAdded response;
			response.type = inet::MESS_NEW_PLAYER;
			response.id = player->getId();
			response.player = player->getPlayer();
			
			_peer->Send((char *)&response, sizeof(response), HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
			
			_new_players.pop();
		}
		
		// Removed players who have disconnected/dropped out
		while ( ! _removed_players.empty() )
		{
			std::cout << "Removing player ";
			
			RakNet::RakNetGUID guid = _removed_players.front();
			_removed_players.pop();
			
			if ( ! _player_id_table[guid] )
				continue;
			
			unsigned long id = _player_id_table[guid];
			
			std::cout << " at " << id << '\n';
			
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
			Player *player = getPlayerByGUID(_updated_players.front());
			_updated_players.pop();
			
			if ( ! player )
				continue;
			
			inet::EventUpdate ev;
			ev.type = inet::MESS_EVENT;
			ev.state = player->getState();
			ev.id = player->getId();
			
			_peer->Send((char *)&ev, sizeof(ev), MEDIUM_PRIORITY, UNRELIABLE, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
		}
		
		sf::Sleep(0.001f);
	}
}

Player *Server::addPlayer(const std::string &name, RakNet::RakNetGUID guid)
{
	Player *player = new Player("ErikPerik");
	player->setCharacter(name);
	player->setId(_current_id++);
	
	_players[player->getId()] = player;
	_player_id_table[guid] = player->getId();
	
	return player;
}

Player *Server::getPlayerByGUID(RakNet::RakNetGUID guid)
{
	if ( ! _player_id_table[guid] )
		return NULL;
	
	unsigned long id = _player_id_table[guid];	
	return _players[id];
}

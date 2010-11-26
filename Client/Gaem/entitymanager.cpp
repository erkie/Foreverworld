/*
 *  entitymanager.cpp
 *  Spel
 *
 *  Created by Erik Andersson on 2010-10-12.
 *  Copyright 2010 Åva gymnasium. All rights reserved.
 *
 */

#include <iostream>
#include <algorithm>
#include <SFML/Graphics.hpp>

#include "Gaem/gaem.h"
#include "Gaem/user.h"
#include "Gaem/networkmanager.h"
#include "Gaem/entitymanager.h"
#include "Gaem/entity.h"
#include "Gaem/exception.h"

#include "Entities/world.h"
#include "Entities/player.h"
#include "Entities/connectionstatus.h"

namespace Gaem
{
	EntityManager::EntityManager(): _current_player(NULL)
	{
		
	}
	
	void EntityManager::init()
	{
		// Add ever-present game entities
		Entities::World *world = new Entities::World();
		add(world);
		setWorld(world);
		
		add(new Entities::ConnectionStatus);
	}
	
	void EntityManager::add(Entity *entity)
	{
		_entities.push_back(entity);
	}
	
	void EntityManager::remove(Entity *entity)
	{
		_entities.remove(entity);
		delete entity;
		
		if ( _current_player == entity )
			setCurrentPlayer(NULL);
	}
	
	void EntityManager::logic()
	{
		if ( _remove_list.size() )
		{
			std::cout << "Before " << _remove_list.size() << " total " << _entities.size();
			for ( entity_list::iterator iter = _remove_list.begin(); iter != _remove_list.end(); iter++ )
			{
				remove(*iter);
			}
			_remove_list.erase(_remove_list.begin(), _remove_list.end());
			
			std::cout << " after " << _remove_list.size() << " total " << _entities.size() << " Removing done\n";
		}
	
		for ( entity_list::iterator iter = _entities.begin(); iter != _entities.end(); iter++ )
		{
			(*iter)->logic();
		}
	}
	
	void EntityManager::draw(sf::RenderWindow &window)
	{
		for ( entity_list::iterator iter = _entities.begin(); iter != _entities.end(); iter++ )
		{
			(*iter)->draw(window);
		}
	}
	
	void EntityManager::handleEvent(const sf::Event &event)
	{	
		for ( entity_list::iterator iter = _entities.begin(); iter != _entities.end(); iter++ )
		{
			(*iter)->handleEvent(event);
		}
	}
	
	void EntityManager::addPlayer(inet::id_type id, const inet::PlayerState player, const inet::LoggedInMemberData member)
	{
		// Create player
		Entities::Player *p = new Entities::Player(getCharacter(member.character_id));
		
		// Set player stats
		p->setDir(player.dir[0], player.dir[1]);
		p->setLeft(player.left);
		p->setDepth(player.depth);
		p->setElevation(player.elevation);
		
		// Add to playing field
		add(p);
		_players[id] = p;
		
		// This is me (the player)
		if ( Gaem::Gaem::getInstance()->getUser()->getId() == id )
		{
			setCurrentPlayer(p);
			Gaem::Gaem::getInstance()->getUser()->setUsername(member.username);
			p->setUser(Gaem::Gaem::getInstance()->getUser());
		}
		else
		{
			User *user = new User;
			user->setUsername(std::string(member.username));
			p->setUser(user);
		}
	}
	
	void EntityManager::removePlayer(inet::id_type id)
	{
		Entities::Player *player = _players[id];
		_players.erase(id);
		_remove_list.push_back(player);
	}
	
	void EntityManager::updatePlayer(inet::id_type id, const inet::PlayerState state)
	{
		Entities::Player *player = _players[id];
		if ( ! player )
		{
			std::cout << "This player did not exist (EntityManager::updatePlayer)\n";
			return;
		}
		
		player->setDir(state.dir[0], state.dir[1]);
		player->setLeft(state.left);
		player->setElevation(state.elevation);
		player->setDepth(state.depth);
		player->setVelocity(state.velocity[0], state.velocity[1]);
		player->setState(state.state);
	}
	
	void EntityManager::addCharacter(inet::Character character)
	{
		_characters[character.id] = character;
	}
	
	inet::Character EntityManager::getCharacter(int32_t id)
	{
		return _characters[id];
	}
	
	void EntityManager::sendUpdates()
	{
		NetworkManager *network = Gaem::Gaem::getInstance()->getNetworkManager();
		network->event(_current_player);
	}
	
	void EntityManager::setCurrentPlayer(Entities::Player *player)
	{
		_current_player = player;
	}
	
	Entities::Player *EntityManager::getCurrentPlayer()
	{
		return _current_player;
	}
	
	void EntityManager::setWorld(Entities::World *world)
	{
		_world = world;
	}
	
	Entities::World *EntityManager::getWorld()
	{
		return _world;
	}
}

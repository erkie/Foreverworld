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
		// sendEvent(KEYDOWN, K_UP, time());
		//NetworkManager *network = Gaem::Gaem::getInstance()->getNetworkManager();
		
		bool is_interesting = false;
		if ( is_interesting )
		{
			//network->sendUpdate();
			//network->;
		}
		
		for ( entity_list::iterator iter = _entities.begin(); iter != _entities.end(); iter++ )
		{
			(*iter)->handleEvent(event);
		}
	}
	
	void EntityManager::addPlayer(inet::id_type id, const inet::Player player)
	{
		// Create player
		Entities::Player *p = new Entities::Player("resources/players/" + std::string(player.character) + ".txt");
		
		// Set player stats
		p->setDir(player.dir);
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
		}
	}
	
	void EntityManager::removePlayer(inet::id_type id)
	{
		Entities::Player *player = _players[id];
		_players.erase(id);
		_remove_list.push_back(player);
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

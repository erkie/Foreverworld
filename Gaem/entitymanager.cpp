/*
 *  entitymanager.cpp
 *  Spel
 *
 *  Created by Erik Andersson on 2010-10-12.
 *  Copyright 2010 Åva gymnasium. All rights reserved.
 *
 */

#include <algorithm>
#include <SFML/Graphics.hpp>

#include "Gaem/entitymanager.h"
#include "Gaem/entity.h"
#include "Gaem/exception.h"

#include "Entities/world.h"
#include "Entities/player.h"

namespace Gaem
{
	EntityManager::EntityManager()
	{
		
	}
	
	void EntityManager::add(Entity *entity)
	{
		_entities.push_back(entity);
	}
	
	void EntityManager::remove(Entity *entity)
	{
		_entities.erase(std::find(_entities.begin(), _entities.end(), entity));
		delete entity;
	}
	
	void EntityManager::logic()
	{
		for ( entity_list::iterator iter = _entities.begin(); iter != _entities.end(); iter++ )
		{
			(*iter)->logic();
		}
		
		this->you->runRight();
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

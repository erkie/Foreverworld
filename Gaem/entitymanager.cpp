/*
 *  entitymanager.cpp
 *  Spel
 *
 *  Created by Erik Andersson on 2010-10-12.
 *  Copyright 2010 Åva gymnasium. All rights reserved.
 *
 */

#include <SFML/Graphics.hpp>

#include "Gaem/entitymanager.h"
#include "Gaem/entity.h"
#include "Gaem/exception.h"

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
		throw GAEM_EXCEPTION("EntityManager::remove Stub");
	}
	
	void EntityManager::logic()
	{
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
}

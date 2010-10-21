/*
 *  entitymanager.h
 *  Spel
 *
 *  Created by Erik Andersson on 2010-10-12.
 *  Copyright 2010 Åva gymnasium. All rights reserved.
 *
 */

#ifndef __GAEM_ENTITYMANAGER_H__
#define __GAEM_ENTITYMANAGER_H__

#include <list>
#include <SFML/Graphics.hpp>

namespace Entities
{
	class Player;
	class World;
}

namespace Gaem
{
	class Entity;
	
	class EntityManager
	{
	protected:
		typedef std::list<Entity*> entity_list;
		entity_list _entities;
		entity_list _remove_list;
		
		Entities::Player *_current_player;
		Entities::World *_world;
	public:
		EntityManager();
		
		Entities::Player *you;
		
		void add(Entity *);
		void remove(Entity *);
		
		void logic();
		void draw(sf::RenderWindow &);
		void handleEvent(const sf::Event &);
		
		void setCurrentPlayer(Entities::Player *);
		Entities::Player *getCurrentPlayer();
		
		void setWorld(Entities::World *);
		Entities::World *getWorld();
	};
}

#endif

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
#include <map>
#include <SFML/Graphics.hpp>

#include "Common/messages.h"

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
		
		typedef std::map<inet::id_type, Entities::Player*> player_map;
		typedef std::map<int32_t, inet::Character> character_map;
		
		player_map _players;
		character_map _characters;
		
		Entities::Player *_current_player;
		Entities::World *_world;
	public:
		EntityManager();
		void init();
		
		void add(Entity *);
		void remove(Entity *);
		
		void logic();
		void draw(sf::RenderWindow &);
		void handleEvent(const sf::Event &);
		
		void addPlayer(inet::id_type id, const inet::PlayerState player, const inet::LoggedInMemberData member);
		void removePlayer(inet::id_type id);
		void updatePlayer(inet::id_type, const inet::PlayerState state);
		
		void addCharacter(inet::Character);
		inet::Character getCharacter(int32_t id);
		
		void sendUpdates();
		
		void setCurrentPlayer(Entities::Player *);
		Entities::Player *getCurrentPlayer();
		
		void setWorld(Entities::World *);
		Entities::World *getWorld();
	};
}

#endif

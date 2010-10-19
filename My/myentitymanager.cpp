/*
 *  myentitymanager.cpp
 *  Spel
 *
 *  Created by Erik Andersson on 2010-10-18.
 *  Copyright 2010 Åva gymnasium. All rights reserved.
 *
 */

#include "My/myentitymanager.h"

#include "Entities/world.h"
#include "Entities/player.h"

namespace My
{
	MyEntityManager::MyEntityManager(): Gaem::EntityManager()
	{
		
	}
	
	void MyEntityManager::setCurrentPlayer(Entities::Player *player)
	{
		_current_player = player;
	}
	
	Entities::Player *MyEntityManager::getCurrentPlayer()
	{
		return _current_player;
	}
	
	void MyEntityManager::setWorld(Entities::World *world)
	{
		_world = world;
	}
	
	Entities::World *MyEntityManager::getWorld()
	{
		return _world;
	}
}

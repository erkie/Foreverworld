/*
 *  myentitymanager.h
 *  Spel
 *
 *  Created by Erik Andersson on 2010-10-18.
 *  Copyright 2010 Åva gymnasium. All rights reserved.
 *
 */

#ifndef __MY_ENTITYMANAGER_H__
#define __MY_ENTITYMANAGER_H__

#include "Gaem/entitymanager.h"

namespace Entities
{
	class Player;
	class World;
}

namespace My
{
	class MyEntityManager: public Gaem::EntityManager
	{
	protected:
		Entities::Player *_current_player;
		Entities::World *_world;
		
	public:
		MyEntityManager();
		
		void setCurrentPlayer(Entities::Player *);
		Entities::Player *getCurrentPlayer();
		
		void setWorld(Entities::World *);
		Entities::World *getWorld();
	};
}

#endif

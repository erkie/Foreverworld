/*
 *  entity.cpp
 *  Spel
 *
 *  Created by Erik Andersson on 2010-10-13.
 *  Copyright 2010 Åva gymnasium. All rights reserved.
 *
 */

#include "Gaem/entity.h"

namespace Gaem
{
	Entity::Entity()
	{
		
	}
	
	int Entity::getZIndex()
	{
		return 0;
	}
	
	bool sortEntities(Entity *one, Entity *two)
	{
		return one->getZIndex() < two->getZIndex();
	}
}

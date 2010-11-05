/*
 *  entity.h
 *  Spel
 *
 *  Created by Erik Andersson on 2010-10-13.
 *  Copyright 2010 Åva gymnasium. All rights reserved.
 *
 */

#ifndef __GAEM_ENTITY_H__
#define __GAEM_ENTITY_H__

#include <SFML/Graphics.hpp>

namespace Gaem
{
	class Entity
	{
	public:
		Entity();
		
		virtual void handleEvent(const sf::Event &) = 0;
		virtual void logic() = 0;
		virtual void draw(sf::RenderWindow &) = 0;
	};
}

#endif

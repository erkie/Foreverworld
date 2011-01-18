/*
 *  connectionstatus.h
 *  Spel
 *
 *  Created by Erik Andersson on 2010-11-15.
 *  Copyright 2010 Åva gymnasium. All rights reserved.
 *
 */

#ifndef __ENTITIES_CONNECTIONSTATUS_H__
#define __ENTITIES_CONNECTIONSTATUS_H__

#include <SFML/Graphics.hpp>

#include "Gaem/entity.h"

namespace Entities
{
	class ConnectionStatus: public Gaem::Entity
	{
	public:
		void handleEvent(const sf::Event &);
		void logic();
		void draw(sf::RenderWindow &);
		
		virtual int getZIndex();
	};
}

#endif

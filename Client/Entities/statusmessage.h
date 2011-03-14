/*
 *  statusmessage.h
 *  Spel
 *
 *  Created by Erik Andersson on 2011-03-14.
 *  Copyright 2011 Åva gymnasium. All rights reserved.
 *
 */

#ifndef __ENTITIES_STATUSMESSAGE_H__
#define __ENTITIES_STATUSMESSAGE_H__

#include <SFML/Graphics.hpp>
#include "Gaem/entity.h"

namespace Entities
{
	class StatusMessage: public Gaem::Entity
	{
		sf::Font *_font;
		std::string _message;
	public:
		StatusMessage();
		
		void draw(sf::RenderWindow &);
		void logic();
		void handleEvent(const sf::Event &);
		
		void setMessage(std::string str);
		
		int getZIndex();
	};
}

#endif

/*
 *  damaged.h
 *  Spel
 *
 *  Created by Erik Andersson on 2011-02-08.
 *  Copyright 2011 Åva gymnasium. All rights reserved.
 *
 */

#ifndef __ENTITIES_DAMAGED_H__
#define __ENTITIES_DAMAGED_H__

#include <SFML/Graphics.hpp>
#include "Gaem/entity.h"

namespace Entities
{
	class Damaged: public Gaem::Entity
	{
		sf::Sprite *_left, *_right;
		float _last_w, _last_h;
	public:
		Damaged();
		~Damaged();
		
		void draw(sf::RenderWindow &);
		void logic();
		void handleEvent(const sf::Event &);
		
		virtual int getZIndex();
	};
}

#endif

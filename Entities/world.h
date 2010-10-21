/*
 *  world.h
 *  Spel
 *
 *  Created by Erik Andersson on 2010-10-13.
 *  Copyright 2010 Åva gymnasium. All rights reserved.
 *
 */

#ifndef __ENTITIES_WORLD_H__
#define __ENTITIES_WORLD_H__

#include <string>
#include <list>
#include <SFML/Graphics.hpp>

#include "Gaem/entity.h"
#include "Gaem/sprite.h"

namespace Entities
{
	class World: public Gaem::Entity
	{
		struct Layer {
			float factor;
			Gaem::Sprite *sprite;
		};
		
		typedef std::list<Layer*> layer_list;
		layer_list _layers;
		
		int _scroll_left;
		int _field_height;
	public:
		World();
		~World();
		
		void loadLayers(const std::string &path);
		
		// Returns the width of the playing field
		int getWidth();
		
		// Returns the height of the playing field
		int getHeight();
		int getScrollLeft();
		int getReverseScrollLeft();
		
		// Reverse
		void reverseScrollLeft(bool left);
		
		void handleEvent(const sf::Event &);
		void logic();
		void draw(sf::RenderWindow &);
	};
}

#endif

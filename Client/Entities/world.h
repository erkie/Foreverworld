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
#include "Gaem/lazysprite.h"

namespace Entities
{
	class World: public Gaem::Entity
	{
		struct Layer {
			float factor;
			int size;
			int width;
			int height;
			int height_offset;
			int last_width;
			Gaem::LazySprite *sprites;
		};
		
		typedef std::list<Layer*> layer_list;
		layer_list _layers;
		
		int _scroll_left;
		int _field_height;
		int _field_width;
		
		int _last_scroll_left;
	public:
		World();
		~World();
		
		void loadLayers(const std::string &path);
		void readPieces(const std::string &path, Layer *);
		
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
		
		virtual int getZIndex();
	};
}

#endif

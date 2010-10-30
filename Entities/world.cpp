/*
 *  world.cpp
 *  Spel
 *
 *  Created by Erik Andersson on 2010-10-13.
 *  Copyright 2010 Åva gymnasium. All rights reserved.
 *
 */

#include <fstream>
#include <iostream>

#include <SFML/Graphics.hpp>

#include "My/myutilities.h"

#include "Gaem/gaem.h"
#include "Gaem/exception.h"

#include "Entities/world.h"
#include "Entities/player.h"

namespace Entities
{
	World::World(): Gaem::Entity(), _scroll_left(0), _field_height(70)
	{
		loadLayers("resources/worlds/info.txt");
	}
	
	World::~World()
	{
		// delete layers
		for ( layer_list::iterator iter = _layers.begin(); iter != _layers.end(); iter++ )
		{
			delete (*iter)->sprite;
			delete (*iter);
		}
	}
	
	void World::loadLayers(const std::string &path)
	{
		// Read paths in info.txt
		// Read images and their specs
		std::fstream info(path.c_str(), std::ios::in);
		
		if (info.good())
		{
			// Read directory from file
			std::string directory;
			info >> directory;
			
			// Read [factor] [filename] until EOF
			float factor;
			std::string filename;
			
			while (info.good())
			{
				info >> factor >> filename;
				
				// Add layers to draw
				Layer *l = new Layer;
				l->factor = factor;
				l->sprite = new Gaem::Sprite(directory + filename);
				
				_layers.push_back(l);
			}	
		}
		else
		{
			throw GAEM_NONFATAL_EXCEPTION("Could not load world layer: " + path);
		}
	}
	
	int World::getWidth()
	{
		// Get last layer's width
		return (*(--_layers.rend()))->sprite->getSprite()->GetSize().x;
	}
	
	int World::getHeight()
	{
		return _field_height;
	}
	
	int World::getScrollLeft()
	{
		return _scroll_left;
	}
	
	int World::getReverseScrollLeft()
	{
		return _scroll_left - getWidth();
	}
	
	void World::reverseScrollLeft(bool left)
	{
		_scroll_left = _scroll_left + ((left ? 1 : -1) * getWidth());
	}
	
	void World::handleEvent(const sf::Event &event)
	{
		// Pass
	}
	
	void World::logic()
	{
		int viewport_w = Gaem::Gaem::getInstance()->getWidth();
		int viewport_h = Gaem::Gaem::getInstance()->getHeight();
		
		// Get position of player
		Player *player = Gaem::Gaem::getInstance()->getEntityManager()->getCurrentPlayer();
		
		// Goes outside the screen to the right
		if ( player->getRight() - _scroll_left > viewport_w - 100 )
		{
			//_scroll_left = player->getLeft() - player->getLeft() - viewport_w - 100;
			_scroll_left += player->getRight() - _scroll_left - (viewport_w - 100);

		}
		// Goes outside the screen to the left
		else if ( player->getLeft() - _scroll_left < 100 )
		{
			_scroll_left += player->getLeft() - _scroll_left - 100;
		}
	
		// Update scroll pos to player pos
		for ( layer_list::iterator iter = _layers.begin(); iter != _layers.end(); iter++ )
		{
			Layer *layer = *iter;
			int x = _scroll_left * layer->factor;
			
			layer->sprite->setX(-x);
			layer->sprite->setY(viewport_h - layer->sprite->getSprite()->GetSize().y);
		}
	}
	
	void World::draw(sf::RenderWindow &window)
	{
		int viewport_w = Gaem::Gaem::getInstance()->getWidth();
		
		for ( layer_list::iterator iter = _layers.begin(); iter != _layers.end(); iter++ )
		{
			Gaem::Sprite *sprite = (*iter)->sprite;
			window.Draw(*(sprite->getSprite()));
			
			// If we are at the end (left side)
			int old_x = sprite->getX();
			while ( sprite->getX() > 0 )
			{
				old_x = sprite->getX();
				sprite->setX(old_x - sprite->getSprite()->GetSize().x);
				window.Draw(*(sprite->getSprite()));
			}
			sprite->setX(old_x);
			
			// If we are at the end (right side)
			old_x = sprite->getX();
			while ( sprite->getX() < -(sprite->getSprite()->GetSize().x - viewport_w) )
			{
				old_x = sprite->getX();
				sprite->setX(old_x + sprite->getSprite()->GetSize().x);
				window.Draw(*(sprite->getSprite()));
			}
			sprite->setX(old_x);
		}
		
		// Draw last layer repeatedly upwards so the sky never ends
		Gaem::Sprite *last = (*(--_layers.rend()))->sprite;
		int old_y = last->getY();
		while ( last->getY() > 0 )
		{
			old_y = last->getY();
			last->setY(old_y - last->getHeight());
			window.Draw(*last->getSprite());
		}
		last->setY(old_y);
	}
}

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
#include <vector>

#include <SFML/Graphics.hpp>

#include "My/myutilities.h"

#include "Gaem/gaem.h"
#include "Gaem/exception.h"
#include "Gaem/entitymanager.h"
#include "Gaem/resourcemanager.h"

#include "Entities/world.h"
#include "Entities/player.h"

#define PIECE_SIZE 100
#define PIECE_OFFSET 20

namespace Entities
{
	World::World(): Gaem::Entity(), _scroll_left(0), _field_height(250), _field_width(0),_last_scroll_left(-1)
	{
		loadLayers("resources/world/");
	}
	
	World::~World()
	{
		// delete layers
		for ( layer_list::iterator iter = _layers.begin(); iter != _layers.end(); iter++ )
		{
			delete [] (*iter)->sprites;
			delete (*iter);
		}
	}
	
	void World::loadLayers(const std::string &path)
	{
		// Read paths in info.txt
		// Read images and their specs
		std::fstream info((path + "info.txt").c_str(), std::ios::in);
		
		int last_width = 0;
		
		if (info.good())
		{
			// Read [factor] [width]x[height] [height offset] [filename] until EOF
			float factor;
			int width, height, height_offset;
			std::string filename;
			
			while (info.good())
			{
				char x;
				info >> factor >> width >> x >> height >> height_offset >> filename;
				
				// Add layers to draw
				// Read files until finished
				Layer *l = new Layer;
				l->factor = factor;
				
				readPieces(path + filename, l);
				l->width = width;
				l->height = height;
				l->height_offset = height_offset;
				last_width = width;
				
				_layers.push_back(l);
			}
			
			_field_width = last_width;
		}
		else
		{
			throw GAEM_NONFATAL_EXCEPTION("Could not load world layer: " + path);
		}
	}
	
	void World::readPieces(const std::string &path, Layer *layer)
	{
		file_list files;
		list_files(path, files);
		
		// Count how many pieces there is
		int num_files = 0;
		for ( file_list::iterator iter = files.begin(); iter != files.end(); iter++ )
		{
			if ( (*iter).find("-") != std::string::npos )
				num_files++;
		}
		
		Gaem::LazySprite *sprites = new Gaem::LazySprite[num_files];
		
		// Create the array of filenames
		int i = 0;
		for ( file_list::iterator iter = files.begin(); iter != files.end(); iter++ )
		{
			if ( (*iter).find("-") != std::string::npos )
			{
				sprites[i] = Gaem::LazySprite();
				sprites[i].setPath(path + (*iter));
				
				i++;
				
				// This is the last image, we know that all other images are PIECE_SIZE 
				if ( i == num_files )
				{
					layer->last_width = Gaem::Gaem::getInstance()->getResourceManager()->getImage(path + (*iter))->GetWidth();
				}
			}
		}
		
		layer->size = num_files;
		layer->sprites = sprites;
	}
	
	int World::getWidth()
	{
		return _field_width;
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
		
		if ( player )
		{
			// Goes outside the screen to the right
			if ( player->getRight() - _scroll_left > viewport_w - 100 )
			{
				_scroll_left += player->getRight() - _scroll_left - (viewport_w - 100);

			}
			// Goes outside the screen to the left
			else if ( player->getLeft() - _scroll_left < 100 )
			{
				_scroll_left += player->getLeft() - _scroll_left - 100;
			}
		}
		
		// Update scroll pos to player pos
		// Only if the scroll pos has changed
		if ( _last_scroll_left != _scroll_left )
		{
			for ( layer_list::iterator iter = _layers.begin(); iter != _layers.end(); iter++ )
			{
				Layer *layer = *iter;
				int x = _scroll_left * layer->factor;
				
				// Position the pieces
				for ( int i = 0; i < (int)layer->size; i++ )
				{
					layer->sprites[i].setX(-x + i * PIECE_SIZE);
					layer->sprites[i].setY(viewport_h - layer->height - layer->height_offset);
				}
				
				// Activate the correct layers
				
				// Get current piece
				int current = ((float)x / layer->width) * layer->size;
				if ( current < 0 )
					current = layer->size + current;
				
				int i;
				// Just keep all activated if there is less then 10 pieces
				if ( layer->size >= 10 )
				{
					std::vector<int> done(layer->size, 0);
					
					// Load five to the right
					for ( i = current, x = 0; x < PIECE_OFFSET; x++, i++ )
					{
						if ( i == layer->size )
							i = 0;
						layer->sprites[i].load();
						done[i] = 1;
					}
					
					// Load five to the left
					for ( i = current, x = 0; x < PIECE_OFFSET; x++, i-- )
					{
						if ( i < 0 )
							i = layer->size + i;
						layer->sprites[i].load();
						done[i] = 1;
					}
					
					// Unload those that have not been loaded
					i = 0;
					for ( std::vector<int>::iterator iter = done.begin(); iter != done.end(); ++iter, ++i )
						if ( (*iter) == 0 )
							layer->sprites[i].unload();
				}
				else
				{
					for ( int i = 0; i < layer->size; i++ )
						layer->sprites[i].load();
				}
			}
		}
		
		_last_scroll_left = _scroll_left;
	}
	
	void World::draw(sf::RenderWindow &window)
	{
		int viewport_w = Gaem::Gaem::getInstance()->getWidth();
		
		window.Clear(sf::Color(0x73, 0x98, 0xC5));
		
		for ( layer_list::iterator iter = _layers.begin(); iter != _layers.end(); iter++ )
		{
			Layer *layer = (*iter);
			for ( int i = 0; i < layer->size; i++ )
			{
				Gaem::Sprite *sprite = &(layer->sprites[i]);
				window.Draw(*sprite->getSprite());
				
				// If we are at the end (left side)
				int old_x = sprite->getX();
				if ( sprite->getX() > 0 )
				{
					old_x = sprite->getX();
					sprite->setX(old_x - layer->width);
					window.Draw(*(sprite->getSprite()));
				}
				sprite->setX(old_x);
				
				// If we are at the end (right side)
				old_x = sprite->getX();
				if ( sprite->getX() < -(sprite->getSprite()->GetSize().x - viewport_w) )
				{
					old_x = sprite->getX();
					sprite->setX(old_x + layer->width);
					window.Draw(*(sprite->getSprite()));
				}
				sprite->setX(old_x);
			}
		}
	}
	
	int World::getZIndex()
	{
		return 0;
	}
}

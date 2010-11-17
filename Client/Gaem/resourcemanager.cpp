/*
 *  resourcemanager.cpp
 *  Spel
 *
 *  Created by Erik Andersson on 2010-10-13.
 *  Copyright 2010 Åva gymnasium. All rights reserved.
 *
 */

#include <map>
#include <SFML/Graphics.hpp>

#include "Gaem/exception.h"
#include "Gaem/resourcemanager.h"

namespace Gaem
{
	ResourceManager::ResourceManager()
	{
		
	}
	
	ResourceManager::~ResourceManager()
	{
		// Clean up
	}
	
	sf::Font *ResourceManager::getFont(const std::string &filename, unsigned int charsize)
	{
		if ( ! _fonts[filename][charsize] )
		{
			sf::Font *font = new sf::Font();
			if ( ! font->LoadFromFile(filename, charsize) )
			{
				throw GAEM_NONFATAL_EXCEPTION("ResourceManager could not load font: " + filename);
			}
			_fonts[filename][charsize] = font;
		}
		return _fonts[filename][charsize];
	}
	
	void ResourceManager::removeFont(const std::string &filename, unsigned int charsize)
	{
		if ( _fonts[filename][charsize] )
		{
			delete _fonts[filename][charsize];
			_fonts[filename].erase(charsize);
		}
	}
	
	sf::Image *ResourceManager::getImage(const std::string &filename)
	{
		if ( ! _images[filename] )
		{
			sf::Image *img = new sf::Image;
			img->SetSmooth(false);
			if ( ! img->LoadFromFile(filename) )
			{
				throw GAEM_NONFATAL_EXCEPTION("ResourceManager could not load image resource: " + filename);
			}
			_images[filename] = img;
		}
		return _images[filename];
	}
	
	void ResourceManager::removeImage(const std::string &filename)
	{
		if ( _images[filename] )
		{
			delete _images[filename];
			_images.erase(filename);
		}
	}
}

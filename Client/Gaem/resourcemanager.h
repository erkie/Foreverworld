/*
 *  resourcemanager.h
 *  Spel
 *
 *  Created by Erik Andersson on 2010-10-13.
 *  Copyright 2010 Åva gymnasium. All rights reserved.
 *
 */

#ifndef __GAEM_RESOURCEMANAGER_H__
#define __GAEM_RESOURCEMANAGER_H__

#include <map>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

namespace Gaem
{
	class ResourceManager
	{
		std::map<const std::string, sf::Image*> _images;
		std::map<const std::string, std::map<unsigned int, sf::Font*> > _fonts;
	public:
		ResourceManager();
		~ResourceManager();
		
		sf::Font *getFont(const std::string &, unsigned int);
		void removeFont(const std::string &filename, unsigned int charsize);
		
		sf::Image *getImage(const std::string &);
		void removeImage(const std::string &);
	};
}

#endif

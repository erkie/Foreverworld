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

namespace Gaem
{
	class ResourceManager
	{
		std::map<const std::string, sf::Image*> _images;
	public:
		ResourceManager();
		~ResourceManager();
		
		sf::Image *getImage(const std::string &);
		void removeImage(const std::string &);
	};
}

#endif

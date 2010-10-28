/*
 *  gaem.h
 *  Spel
 *
 *  Created by Erik Andersson on 2010-09-28.
 *  Copyright 2010 Åva gymnasium. All rights reserved.
 *
 */

#ifndef __GAEM_GAEM_H__
#define __GAEM_GAEM_H__

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include "Gaem/exception.h"

#include "Gaem/config.h"
#include "Gaem/resourcemanager.h"
#include "Gaem/menumanager.h"
#include "Gaem/entitymanager.h"

namespace Gaem
{
	class Gaem
	{
		static Gaem *_instance;
		
		sf::RenderWindow _app;
		
		Config *_config;
		MenuManager *_menu_manager;
		EntityManager *_entity_manager;
		ResourceManager *_resource_manager;
		
		int _w, _h;
	public:
		Gaem();
		~Gaem();
		
		void init();
		void main();
		void errorMain(const std::string &msg);
		
		void initWindow(int w, int h);
		int getWidth();
		int getHeight();
		
		float getTDelta();
		
		sf::RenderWindow *getWindow();
		Config *getConfig();
		MenuManager *getMenuManager();
		EntityManager *getEntityManager();
		ResourceManager *getResourceManager();
		
		static Gaem *getInstance();
	};
}

#endif

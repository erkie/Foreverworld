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

namespace Gaem
{
	class Config;
	class User;
	class ResourceManager;
	class MenuManager;
	class EntityManager;
	class NetworkManager;
	class SoundManager;
	
	class Gaem
	{
		static Gaem *_instance;

		sf::RenderWindow _app;

		Config *_config;
		User *_user;
		MenuManager *_menu_manager;
		EntityManager *_entity_manager;
		ResourceManager *_resource_manager;
		NetworkManager *_network_manager;
		SoundManager *_sound_manager;
		
		NonFatalException _exception;

		float _tdelta;
		bool _error_run;

		int _w, _h;
	public:
		Gaem();
		~Gaem();

		void init();
		void quit();
		void quitError();
		void main();
		void errorMain(const std::string &msg, NonFatalException &);

		void initWindow(int w, int h);
		void resize(int w, int h);
		int getWidth();
		int getHeight();

		float getTDelta();
		std::string getErrorPost();

		sf::RenderWindow *getWindow();
		Config *getConfig();
		User *getUser();
		MenuManager *getMenuManager();
		EntityManager *getEntityManager();
		ResourceManager *getResourceManager();
		NetworkManager *getNetworkManager();
		SoundManager *getSoundManager();

		static Gaem *getInstance();
	};
}

#endif

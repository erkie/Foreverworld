/*
 *  gaem.cpp
 *  Spel
 *
 *  Created by Erik Andersson on 2010-09-28.
 *  Copyright 2010 Åva gymnasium. All rights reserved.
 *
 */

#include <iostream>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "Gaem/gaem.h"
#include "Gaem/config.h"
#include "Gaem/exception.h"

#include "Menus/main.h"
#include "Menus/alert.h"
#include "Menus/loading.h"

#include "My/myentitymanager.h"
#include "Entities/world.h"
#include "Entities/player.h"

namespace Gaem
{
	Gaem* Gaem::_instance = NULL;
	
	Gaem::Gaem()
	{
		Gaem::_instance = this;
		
		_config = new Config("master");
		
		// Create window
		// Fullscreen or windowed
		
		// Set window mode
		float width = _config->getFloat("window_width", 640);
		float height = _config->getFloat("window_height", 480);
		
		initWindow(width, height);
		
		// Frame limiting
		_app.UseVerticalSync(_config->isTrue("use_vsync"));
		_app.SetFramerateLimit(_config->getInt("max_framerate", 60));
		
		// Resources
		_resource_manager = new ResourceManager();
		
		// Menus
		_menu_manager = new MenuManager(_app);
		
		_menu_manager->add("main", new ::Menus::Main());
		_menu_manager->add("loading", new ::Menus::Loading());
		
		// World entity manager
		_entity_manager = new My::MyEntityManager();
	}
	
	Gaem::~Gaem()
	{
		delete _config;
		delete _menu_manager;
	}
	
	void Gaem::initWindow(int w, int h)
	{
		_w = w;
		_h = h;
		
		bool use_fullscreen = _config->isTrue("window_fullscreen");
		unsigned long window_style = use_fullscreen ? sf::Style::Fullscreen : (sf::Style::Resize | sf::Style::Close);
		sf::VideoMode mode = use_fullscreen ? sf::VideoMode::GetDesktopMode() : sf::VideoMode(w, h); 
		
		// Initialize the window
		_app.Create(mode, "Foreverworld", window_style);
	}
	
	void Gaem::init()
	{
		Entities::World *world = new Entities::World();
		_entity_manager->add(world);
		_entity_manager->setWorld(world);
		
		Entities::Player *player = new Entities::Player();
		_entity_manager->add(player);
		_entity_manager->setCurrentPlayer(player);
		
		_menu_manager->show("main");
	}
	
	void Gaem::main()
	{
		while (_app.IsOpened())
		{
			sf::Event event;
			while (_app.GetEvent(event))
			{
				// Global events
				if ( event.Type == sf::Event::Closed )
					_app.Close();
				
				if ( event.Type == sf::Event::KeyPressed && event.Key.Code == sf::Key::Escape )
					_app.Close();
				
				if ( event.Type == sf::Event::Resized )
				{
					// resize window?
				}
				
				// Only entity events if no menu is active
				// Otherwise pipe the events to the menu
				if ( _menu_manager->hasMenus() )
					_menu_manager->handleEvent(event);
				else
				// Handle entity events
					_entity_manager->handleEvent(event);
			}
			
			_app.Clear(sf::Color::White);
			
			// Logic for menus
			// Draw menu
			_entity_manager->logic();
			_entity_manager->draw(_app);
			
			// Logic and draw sprites
			try {
				_menu_manager->logic();
				_menu_manager->draw();
			}
			catch ( gcn::Exception e )
			{
				std::cerr << e.getMessage() << '\n';
				return;
			}
			
			_app.Display();
		}
	}
	
	void Gaem::errorMain(const std::string &error)
	{
		_menu_manager->alert(error);
		
		while (_app.IsOpened())
		{
			sf::Event event;
			while (_app.GetEvent(event))
			{
				// Global events
				if ( event.Type == sf::Event::Closed )
					_app.Close();
				
				// Handle menu events
				_menu_manager->handleEvent(event);
			}
			
			_app.Clear(sf::Color::White);
			
			// Logic and draw sprites
			try {
				_menu_manager->logic();
				_menu_manager->draw();
			}
			catch ( gcn::Exception e )
			{
				std::cerr << e.getMessage() << '\n';
				return;
			}
			
			_app.Display();
		}
	}
	
	Gaem *Gaem::getInstance()
	{
		return Gaem::_instance;
	}
	
	int Gaem::getWidth()
	{
		return _app.GetWidth();
	}
	
	int Gaem::getHeight()
	{
		return _app.GetHeight();
	}
	
	float Gaem::getTDelta()
	{
		return _app.GetFrameTime();
	}
	
	sf::RenderWindow *Gaem::getWindow()
	{
		return &_app;
	}
	
	Config *Gaem::getConfig()
	{
		return _config;
	}
	
	MenuManager *Gaem::getMenuManager()
	{
		return _menu_manager;
	}
	
	My::MyEntityManager *Gaem::getEntityManager()
	{
		return _entity_manager;
	}
	
	ResourceManager *Gaem::getResourceManager()
	{
		return _resource_manager;
	}
}

/*
 *  gaem.cpp
 *  Spel
 *
 *  Created by Erik Andersson on 2010-09-28.
 *  Copyright 2010 Åva gymnasium. All rights reserved.
 *
 */

#include <iostream>
#include <map>
#include <sstream>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "Gaem/gaem.h"
#include "Gaem/config.h"
#include "Gaem/exception.h"

#include "Gaem/config.h"
#include "Gaem/user.h"
#include "Gaem/resourcemanager.h"
#include "Gaem/menumanager.h"
#include "Gaem/entitymanager.h"
#include "Gaem/networkmanager.h"
#include "Gaem/soundmanager.h"

#include "My/myutilities.h"

#include "Menus/main.h"
#include "Menus/dashboard.h"
#include "Menus/developer.h"
#include "Menus/alert.h"
#include "Menus/loading.h"
#include "Menus/chat.h"
#include "Menus/error.h"

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

		// World entity manager
		_entity_manager = new EntityManager();
		
		// Network manager, connect to server, send/receive updates
		_network_manager = new NetworkManager();
		
		// Sound manager, play music and sounds
		_sound_manager = new SoundManager();
		
		// Information about me
		_user = new User();
	}

	Gaem::~Gaem()
	{
		delete _config;
		delete _menu_manager;
		delete _entity_manager;
		delete _resource_manager;
		delete _user;
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

	void Gaem::resize(int w, int h)
	{
		sf::View &view = _app.GetDefaultView();
		view.SetFromRect(sf::FloatRect(0, 0, w, h));
		_app.SetView(view);
		_menu_manager->resize();
	}

	void Gaem::init()
	{
		_entity_manager->init();
		
		// Load menus
		_menu_manager->add("main", new Menus::Main());
		_menu_manager->add("dashboard", new Menus::Dashboard());
		_menu_manager->add("developer", new Menus::Developer());
		_menu_manager->add("loading", new Menus::Loading());
		_menu_manager->add("chat", new Menus::Chat());
		_menu_manager->add("error", new Menus::Error());

		// Show main menu
		_menu_manager->show("main");
		
		_network_manager->connect();
	}

	void Gaem::main()
	{
		sf::Event event;
		
		const sf::Input &input = getWindow()->GetInput();
		
		while (_app.IsOpened())
		{
			_tdelta = _app.GetFrameTime();
			
			// Get updates for network code
			_network_manager->getUpdates();
			
			// Handle user events
			while (_app.GetEvent(event))
			{
				// Global events
				// Quit
				if ( event.Type == sf::Event::Closed )
					return quit();
				
				// ... If I press Q
				if ( event.Type == sf::Event::KeyPressed && event.Key.Code == sf::Key::Q && ! _menu_manager->hasMenus() )
					return quit();
				
				// ... If I press Escape on home screen
				if ( event.Type == sf::Event::KeyPressed && event.Key.Code == sf::Key::Escape && _menu_manager->getCurrentMenu() == "main" )
					return quit();
				
				// Go into fullscreen mode
				if ( ! _menu_manager->hasMenus() && event.Type == sf::Event::KeyPressed && event.Key.Code == sf::Key::F && false )
				{
					_config->set("window_fullscreen", "yes");
					initWindow(-1, -1);
					resize(_app.GetWidth(), _app.GetHeight());
				}

				// Open main menu on [esc]
				if ( event.Type == sf::Event::KeyPressed && event.Key.Code == sf::Key::Escape )
				{
					std::string menu_to_show = _user->isLoggedIn() ? "dashboard" : "main";
					if ( ! _menu_manager->hasMenus() )
						_menu_manager->show(menu_to_show);
					else
						_menu_manager->hide();
				}
				
				// Open developer menu on [ctrl+Y]
				if ( event.Type == sf::Event::KeyPressed && event.Key.Code == sf::Key::Y && input.IsKeyDown(sf::Key::LControl)  )
				{
					_menu_manager->show("developer");
				}
				
				// Resize window
				if ( event.Type == sf::Event::Resized )
				{
					resize(event.Size.Width, event.Size.Height);
				}

				// Only entity events if no menu is active
				// Otherwise pipe the events to the menu
				if ( _menu_manager->hasMenus() )
					_menu_manager->handleEvent(event);
				// Handle entity events
				else
					_entity_manager->handleEvent(event);
			}

			// Logic for menus
			// Draw sprites
			_entity_manager->logic();
			_entity_manager->draw(_app);

			// Logic and draw menus
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

	void Gaem::errorMain(const std::string &error, NonFatalException &ex)
	{
		// Clear menu queue so the user can't hide the alert and continue everything
		_menu_manager->hideAll();
		
		_menu_manager->show("error");
		_error_run = true;

		_exception = ex;

		while (_app.IsOpened() && _error_run)
		{
			sf::Event event;
			while (_app.GetEvent(event))
			{
				// Global events
				if ( event.Type == sf::Event::Closed )
					return quit();

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
	
	std::string Gaem::getErrorPost()
	{
		std::stringstream ss, s2, s3;
		s2 << _exception.getLine();
		
		std::map<std::string, std::string> params;
		params["message"] = _exception.getMessage();
		params["file"] = _exception.getFilename();
		params["line"] = s2.str();
		params["user"] = _user->getUsername();
		
		for ( std::map<std::string, std::string>::iterator iter = params.begin(); iter != params.end(); ++iter )
		{
			ss << urlencode((*iter).first);
			ss << "=";
			ss << urlencode((*iter).second);
			ss << "&";
		}
		return ss.str();
	}
	
	void Gaem::quit()
	{
		delete _network_manager;
	}
	
	void Gaem::quitError()
	{
		_error_run = false;
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
		return _tdelta;
	}

	sf::RenderWindow *Gaem::getWindow()
	{
		return &_app;
	}

	Config *Gaem::getConfig()
	{
		return _config;
	}
	
	User *Gaem::getUser()
	{
		return _user;
	}

	MenuManager *Gaem::getMenuManager()
	{
		return _menu_manager;
	}

	EntityManager *Gaem::getEntityManager()
	{
		return _entity_manager;
	}

	ResourceManager *Gaem::getResourceManager()
	{
		return _resource_manager;
	}
	
	NetworkManager *Gaem::getNetworkManager()
	{
		return _network_manager;
	}
}

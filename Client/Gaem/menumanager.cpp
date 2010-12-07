/*
 *  menumanager.cpp
 *  Spel
 *
 *  Created by Erik Andersson on 2010-10-03.
 *  Copyright 2010 Åva gymnasium. All rights reserved.
 *
 */

#include <algorithm>

#include <SFML/Graphics.hpp>

#include <guichan/guichan.hpp>
#include <guichan/sfml.hpp>

#include "Gaem/gaem.h"
#include "Gaem/exception.h"
#include "Gaem/menumanager.h"

#include "Menus/alert.h"
#include "Menus/loading.h"

#include "Widgets/passwordfield.h"

namespace Gaem
{
	//
	// Gaem::Menu
	//

	Menu::Menu(): _manager(NULL), _root(NULL), _inited(false), _name("") {}

	Menu::~Menu()
	{
		for ( widget_list::iterator iter = _widgets.begin(); iter != _widgets.end(); iter++ )
			delete *iter;

		for ( listener_list::iterator iter = _listeners.begin(); iter != _listeners.end(); iter++ )
			delete *iter;

		_table.clear();
		_widgets.clear();
		_listeners.clear();
	}

	void Menu::show()
	{
		// pass
	}

	gcn::Widget *Menu::get(const std::string &name)
	{
		return _table[name];
	}

	void Menu::setActive()
	{
		setRoot(_root);
		show();
	}

	void Menu::setManager(MenuManager *manager)
	{
		_manager = manager;
		if ( ! _inited )
		{
			init();
			_inited = true;
		}
	}

	void Menu::setRoot(gcn::Container *root)
	{
		_root = root;
		_manager->getGui()->setTop(root);
	}
	
	void Menu::setName(const std::string &name)
	{
		_name = name;
	}
	
	std::string Menu::getName()
	{
		return _name;
	}

	gcn::Widget *Menu::getRoot()
	{
		return _root;
	}

	void Menu::centerRoot()
	{
		_root->setX(Gaem::Gaem::getInstance()->getWidth()/2 - _root->getWidth()/2);
		_root->setY(Gaem::Gaem::getInstance()->getHeight()/2 - _root->getHeight()/2);
	}

	//
	// Gaem::MenuManager
	//

	MenuManager::MenuManager(sf::RenderWindow &app)
	{
		_gui = new gcn::Gui();
		_gui_graphics = new gcn::SFMLGraphics(app);
		_gui_input = new gcn::SFMLInput();
		_gui_imageloader = new gcn::SFMLImageLoader();

		// _sf_input is used by the SFML/guichan event handler
		_sf_input = &app.GetInput();

		// Load and apply fonts
		if ( ! _font.LoadFromFile("resources/main_font.ttf", 15) )
		{
			throw GAEM_EXCEPTION("Could not load resources/main_font.ttf");
		}

		_gui_font = new gcn::SFMLFont(_font);
		gcn::Widget::setGlobalFont(_gui_font);

		// Set some global variables
		_gui->setGraphics(_gui_graphics);
		_gui->setInput(_gui_input);
	}

	void MenuManager::resize()
	{
		delete _gui_graphics;
		_gui_graphics = new gcn::SFMLGraphics(*Gaem::Gaem::getInstance()->getWindow());
		_gui->setGraphics(_gui_graphics);
	}

	void MenuManager::add(const std::string &id, Menu *menu)
	{
		menu->setManager(this);
		menu->setName(id);
		_menus[id] = menu;
	}

	void MenuManager::add(Menu *menu)
	{
		menu->setManager(this);
	}

	void MenuManager::show(const std::string &id)
	{
		Menu* menu = _menus[id];
		menu->setActive();
		_menu_queue.push(menu);
	}

	void MenuManager::show(Menu *menu)
	{
		menu->setActive();
		_menu_queue.push(menu);
	}

	void MenuManager::showLoading(const std::string &message)
	{
		static_cast<Menus::Loading*>(_menus["loading"])->setMessage(message);
		show("loading");
	}

	void MenuManager::hideLoading()
	{
		hide();
	}

	void MenuManager::alert(const std::string &message)
	{
		Menus::Alert *alert = new Menus::Alert(message);
		_dynamic_menus.push_back(alert);

		add(alert);
		show(alert);
	}

	void MenuManager::hide()
	{
		Menu *menu = _menu_queue.top();
		_menu_queue.pop();

		// If this is a dynamically added menu we have to remove it
		// The garbage collection is taken care of in ::logic
		if ( std::count(_dynamic_menus.begin(), _dynamic_menus.end(), menu) > 0 )
		{
			_remove_list.push_back(menu);
			_dynamic_menus.remove(menu);
		}

		// Show the next menu in cue (if any)
		if ( _menu_queue.size() )
		{
			Menu *next = _menu_queue.top();
			next->setActive();
		}
	}
	
	void MenuManager::hideAll()
	{
		while ( hasMenus() )
			hide();
	}

	void MenuManager::handleEvent(sf::Event &event)
	{
		if ( _menu_queue.size() == 0 ) return;
		_gui_input->pushEvent(event, *_sf_input);
	}

	void MenuManager::logic()
	{
		if ( _menu_queue.size() == 0 ) return;

		for ( menu_list::iterator iter = _remove_list.begin(); iter != _remove_list.end(); iter++ )
			delete *iter;
		_remove_list.clear();

		_gui->logic();
	}

	void MenuManager::draw()
	{
		if ( _menu_queue.size() == 0 ) return;
		_gui->draw();
	}

	bool MenuManager::hasMenus()
	{
		return _menu_queue.size() > 0;
	}
	
	std::string MenuManager::getCurrentMenu()
	{
		return hasMenus() ? _menu_queue.top()->getName() : "";
	}

	gcn::Gui *MenuManager::getGui()
	{
		return _gui;
	}
}

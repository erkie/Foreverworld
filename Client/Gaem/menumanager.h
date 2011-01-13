/*
 *  menumanager.h
 *  Spel
 *
 *  Created by Erik Andersson on 2010-10-03.
 *  Copyright 2010 Åva gymnasium. All rights reserved.
 *
 */

#ifndef __GAEM_MENUMANAGER_H__
#define __GAEM_MENUMANAGER_H__

#include <string>
#include <list>
#include <map>
#include <stack>

#include <SFML/Window.hpp>

#include <guichan/guichan.hpp>
#include <guichan/sfml.hpp>

namespace Gaem
{
	class MenuManager;

	// Base class for event listeners so we can create and delete them in a
	// general way
	class Listener {};

	//
	// Menu
	//

	class Menu
	{
	protected:
		typedef std::list<gcn::Widget*> widget_list;
		typedef std::list<Listener*> listener_list;
		std::map<std::string, gcn::Widget*> _table;
		
		widget_list _widgets;
		listener_list _listeners;
	
		MenuManager *_manager;
		gcn::Container *_root;
		
		bool _inited;
		std::string _name;
	public:
		Menu();
		~Menu();
		
		virtual void init() = 0;
		virtual void show();
		
		void setManager(MenuManager *);
		
		void setActive();
		void setRoot(gcn::Container *);
		gcn::Widget *getRoot();
		gcn::Widget *get(const std::string& key);
		
		template<typename T>
		T *get(const std::string &key)
		{
			return static_cast<T*>(_table[key]);
		}
		
		void setName(const std::string &);
		std::string getName();
		
		void centerRoot();
		
		template<typename T>
		T* newWidget()
		{
			T *w = new T;
			_widgets.push_back(w);
			return w;
		}
		
		template<typename T>
		T* newWidget(const std::string &a)
		{
			T *w = new T(a);
			_widgets.push_back(w);
			return w;
		}
		
		template<typename T>
		T* newNamedWidget(const std::string &name)
		{
			T *w = new T;
			_widgets.push_back(w);
			_table[name] = w;
			return w;
		}
		
		template<typename T>
		T* newNamedWidget(const std::string &name, const std::string &a)
		{
			T *w = new T(a);
			_widgets.push_back(w);
			_table[name] = w;
			return w;
		}
		
		template<typename T>
		T* newListener()
		{
			T *l = new T;
			_listeners.push_back(l);
			return l;
		}
	};
	
	//
	// MenuManager
	//
	
	class MenuManager
	{
		typedef std::list<Menu*> menu_list;
		
		std::map<const std::string, Menu*> _menus;
		std::stack<Menu*> _menu_queue;
		menu_list _dynamic_menus;
		menu_list _remove_list;
		
		sf::Font _font;
		
		gcn::SFMLGraphics *_gui_graphics;
		gcn::SFMLInput *_gui_input;
		gcn::SFMLImageLoader *_gui_imageloader;
		gcn::SFMLFont *_gui_font;
		
		const sf::Input *_sf_input;
		
		gcn::Gui *_gui;
	public:
		MenuManager(sf::RenderWindow &);
		void resize();
		
		void add(Menu *);
		void add(const std::string &, Menu *);
		
		void show(const std::string&);
		void show(Menu *);
		void hide();
		void hideAll();
		
		void handleEvent(sf::Event &);
		void logic();
		void draw();
		
		void showLoading(const std::string &message);
		void hideLoading();
		void alert(const std::string &message);
		
		bool hasMenus();
		std::string getCurrentMenu();
		gcn::Gui *getGui();
	};
}

#endif

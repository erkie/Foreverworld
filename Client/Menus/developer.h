/*
 *  developer.h
 *  Spel
 *
 *  Created by Erik Andersson on 2010-10-27.
 *  Copyright 2010 Åva gymnasium. All rights reserved.
 *
 */

#ifndef __MENUS_DEVELOPER_H__
#define __MENUS_DEVELOPER_H__

#include <guichan/guichan.hpp>
#include "Gaem/menumanager.h"

#include "Widgets/animationdemo.h"

namespace Menus
{
	class Developer: public Gaem::Menu
	{
		gcn::TabbedArea *_tabs;
		gcn::ListModel *_model;
		std::string _current_player;
	public:
		static Developer *instance;
		
		~Developer();
		
		void init();
		void show();
		
		void initHome();
		void initAnimations();
		void initPlayers();
		
		void setCurrentPlayer(const std::string &);
		std::string getCurrentPlayer();
		
		gcn::Container *makeContainer();
	};
}

#endif

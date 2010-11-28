/*
 *  dashboard.h
 *  Spel
 *
 *  Created by Erik Andersson on 2010-11-25.
 *  Copyright 2010 Åva gymnasium. All rights reserved.
 *
 */

#ifndef __MENUS_DASHBOARD_H__
#define __MENUS_DASHBOARD_H__

#include <vector>
#include <guichan/guichan.hpp>

#include "Gaem/menumanager.h"

namespace Menus
{
	class Dashboard: public Gaem::Menu
	{
		gcn::TabbedArea *_tabs;
	public:
		static Dashboard *instance;
		
		void init();
		void show();
		
		void initHome();
		void initStats();
		void initSettings();
		void initAbout();
		
		gcn::Container *makeContainer();
	};
}

#endif

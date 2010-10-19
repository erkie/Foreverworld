/*
 *  main.h
 *  Spel
 *
 *  Created by Erik Andersson on 2010-10-03.
 *  Copyright 2010 Åva gymnasium. All rights reserved.
 *
 */

#ifndef __MENUS_MAIN_H__
#define __MENUS_MAIN_H__

#include <iostream>

#include <guichan/guichan.hpp>
#include "Gaem/menumanager.h"

namespace Menus
{	
	class Main: public ::Gaem::Menu
	{
		gcn::TabbedArea *_tabs;
		
		void initLogin();
		void initSignup();
		void initAbout();
		
		gcn::Container *makeContainer();
	public:
		void init();
		void deinit();
	};
}

#endif

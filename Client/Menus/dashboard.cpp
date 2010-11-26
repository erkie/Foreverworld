/*
 *  dashboard.cpp
 *  Spel
 *
 *  Created by Erik Andersson on 2010-11-25.
 *  Copyright 2010 Åva gymnasium. All rights reserved.
 *
 */

#include "Gaem/gaem.h"
#include "Gaem/user.h"
#include "Gaem/menumanager.h"

#include "Menus/dashboard.h"

namespace Menus
{	
	Dashboard *Dashboard::instance = NULL;
	
	void Dashboard::init()
	{
		Dashboard::instance = this;
		
		// The window
		gcn::Window *base = newWidget<gcn::Window>();
		base->setTitleBarHeight(25);
		base->setSize(400, 400);
		
		// The tabs
		_tabs = newWidget<gcn::TabbedArea>();
		_tabs->setSize(base->getChildrenArea().width, base->getChildrenArea().height);
		
		initHome();
		initStats();
		initSettings();
		initAbout();
		
		setRoot(base);
		centerRoot();
	}
	
	void Dashboard::show()
	{
		centerRoot();
		
		static_cast<gcn::Window*>(_root)->setCaption("Welcome, " + Gaem::Gaem::getInstance()->getUser()->getUsername());
	}
	
	void Dashboard::initHome()
	{
		
	}
	
	void Dashboard::initStats()
	{
		
	}
	
	void Dashboard::initSettings()
	{
		
	}
	
	void Dashboard::initAbout()
	{
		
	}
}

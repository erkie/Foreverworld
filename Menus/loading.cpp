/*
 *  loading.cpp
 *  Spel
 *
 *  Created by Erik Andersson on 2010-10-03.
 *  Copyright 2010 Åva gymnasium. All rights reserved.
 *
 */

#include <guichan/guichan.hpp>

#include "loading.h"

namespace Menus
{
	void Loading::init()
	{
		gcn::Window *r = newWidget<gcn::Window>("Loading");
		r->setTitleBarHeight(25);
		r->setSize(300, 100);
		setRoot(r);
		
		gcn::Label *indicator = newWidget<gcn::Label>("Loading a lot of things...");
		indicator->adjustSize();
		indicator->setX(_root->getChildrenArea().width/2 - indicator->getWidth()/2);
		indicator->setY(_root->getChildrenArea().height/2 - indicator->getHeight()/2);
		
		_root->add(indicator);
		
		centerRoot();
	}
}

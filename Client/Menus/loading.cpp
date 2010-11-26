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
		
		_indicator = newWidget<gcn::Label>();
		setMessage("Loading a lot of things");
		
		_root->add(_indicator);
		
		centerRoot();
	}
	
	void Loading::setMessage(const std::string &message)
	{
		_message = message;
		
		_indicator->setCaption(message);
		_indicator->adjustSize();
		_root->setWidth(_indicator->getWidth() + 20);
		_indicator->setX(_root->getChildrenArea().width/2 - _indicator->getWidth()/2);
		_indicator->setY(_root->getChildrenArea().height/2 - _indicator->getHeight()/2);
		
		centerRoot();
	}
}

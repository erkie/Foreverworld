/*
 *  alert.cpp
 *  Spel
 *
 *  Created by Erik Andersson on 2010-10-03.
 *  Copyright 2010 Åva gymnasium. All rights reserved.
 *
 */

#include <guichan/guichan.hpp>
#include "Widgets/fixedlabel.h"

#include "Gaem/gaem.h"

#include "alert.h"

namespace Menus
{
	class AlertOkay: public gcn::ActionListener, public Gaem::Listener
	{
		void action(const gcn::ActionEvent &event)
		{
			Gaem::Gaem::getInstance()->getMenuManager()->hide();
		}
	};

	Alert::Alert(): _message("Failed to connect to server. Status code received: 418") {}

	Alert::Alert(const std::string &message): _message(message)
	{}

	void Alert::init()
	{
		_my_root = newWidget<gcn::Window>();
		_my_root->setTitleBarHeight(25);
		_my_root->setDimension(gcn::Rectangle(0, 0, 500, 100));
		_my_root->setCaption("Attention, please");
		_root = _my_root;
		
		_label = newWidget<Widgets::FixedLabel>();
		_label->setPosition(10, 10);
		_label->setWidth(_root->getChildrenArea().width - 10*2);
		
		_okay = newWidget<gcn::Button>("Okay");
		_okay->adjustSize();
		_okay->setX(_root->getChildrenArea().width/2 - _okay->getWidth()/2);
		
		setMessage(_message);//"Failed to connect to server. Status code received: 418");
		
		_okay->addActionListener(newListener<AlertOkay>());
		
		_root->add(_label);
		_root->add(_okay);
		
		setRoot(_root);
	}
	
	void Alert::setMessage(const std::string &message)
	{
		_message = _message;
		
		_label->setCaption(_message);
		_label->adjustHeight();

		_okay->setY(_label->getY() + _label->getHeight() + 10);
		
		_root->setHeight(_okay->getY() + _okay->getHeight() + 10 + _my_root->getTitleBarHeight());
		centerRoot();
	}
}

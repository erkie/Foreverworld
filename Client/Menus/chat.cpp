/*
 *  chat.cpp
 *  Spel
 *
 *  Created by Erik Andersson on 2010-12-07.
 *  Copyright 2010 Åva gymnasium. All rights reserved.
 *
 */

#include <guichan/guichan.hpp>

#include "Gaem/gaem.h"
#include "Gaem/menumanager.h"
#include "Gaem/networkmanager.h"

#include "Menus/chat.h"

namespace Menus
{
	class ChatTyped: public gcn::KeyListener, public gcn::ActionListener, public Gaem::Listener
	{
	public:
		void keyPressed(gcn::KeyEvent &event)
		{
			if ( event.getKey() == gcn::Key::ENTER )
				send();
		}
		
		void action(const gcn::ActionEvent &event) { send(); }
		
		void send()
		{
			gcn::TextField *field = static_cast<gcn::TextField*>(Chat::instance->get("text_input"));
			std::string message = field->getText();
			
			if ( message != "" )
			{
				field->setText("");
				Gaem::Gaem::getInstance()->getNetworkManager()->sendChatMessage(message);
			}
			Gaem::Gaem::getInstance()->getMenuManager()->hide();
		}
	};

	Chat *Chat::instance = NULL;

	void Chat::init()
	{
		instance = this;
		
		_root = newWidget<gcn::Container>();
		_root->setPosition(10, 10);
		_root->setOpaque(false);
		setRoot(_root);
		
		gcn::TextField *input = newNamedWidget<gcn::TextField>("text_input");
		input->setWidth(500);
		input->setHeight(20);
		input->setPosition(0, 0);
		input->setFocusable(true);
		
		gcn::Button *send = newWidget<gcn::Button>("Send");
		send->setWidth(100);
		send->setHeight(20);
		send->setX(input->getRight() + 10);
		send->setY(0);
		
		ChatTyped *listener = newListener<ChatTyped>();
		input->addKeyListener(listener);
		send->addActionListener(listener);
		
		_root->setWidth(send->getRight());
		_root->setHeight(input->getHeight());
		
		_root->add(send);
		_root->add(input);
	}
	
	void Chat::show()
	{
		get("text_input")->requestFocus();
	}
}

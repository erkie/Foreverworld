/*
 *  chat.cpp
 *  Spel
 *
 *  Created by Erik Andersson on 2010-12-07.
 *  Copyright 2010 Åva gymnasium. All rights reserved.
 *
 */

#include <iostream>
#include <algorithm>
#include <SFML/Graphics.hpp>

#include "Gaem/gaem.h"
#include "Gaem/menumanager.h"
#include "Gaem/resourcemanager.h"

#include "Entities/chat.h"

#define MESSAGE_TIME 7.0

namespace Entities
{
	bool Chat::removeOld(message_type *mess)
	{
		return mess->received.GetElapsedTime() > MESSAGE_TIME;
	}

	Chat::Chat()
	{
		
	}
	
	void Chat::addMessage(const std::string &username, const std::string &message)
	{
		message_type *mess = new message_type;
		mess->username = username;
		mess->message = message;
		mess->received = sf::Clock();
		
		_messages.push_back(mess);
	}
	
	void Chat::logic()
	{
		// Remove old messages
		_messages.erase(std::remove_if(_messages.begin(), _messages.end(), removeOld), _messages.end());
	}
	
	void Chat::handleEvent(const sf::Event &event)
	{
		if ( event.Type == sf::Event::KeyPressed && event.Key.Code == sf::Key::Return )
		{
			// Show prompt
			Gaem::Gaem::getInstance()->getMenuManager()->show("chat");
		}
	}
	
	void Chat::draw(sf::RenderWindow &window)
	{
		int font_size = 13;
		sf::Font *font = Gaem::Gaem::getInstance()->getResourceManager()->getFont("resources/rounded_font.ttf", font_size);
		
		int y = 50;
		for ( message_list::iterator iter = _messages.begin(); iter != _messages.end(); ++iter )
		{
			message_type *mess = *iter;
			
			sf::String username(mess->username + ": ", *font, font_size);
			username.SetX(10);
			username.SetY(y);
			username.SetColor(sf::Color(0x00, 0x9F, 0x6B));
			
			sf::String message(mess->message, *font, font_size);
			message.SetFont(*font);
			message.SetX(username.GetPosition().x + username.GetRect().GetWidth());
			message.SetY(y);
			
			y += username.GetRect().GetHeight() + 5;
			
			window.Draw(username);
			window.Draw(message);
		}
	}
}

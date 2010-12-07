/*
 *  chat.h
 *  Spel
 *
 *  Created by Erik Andersson on 2010-12-07.
 *  Copyright 2010 Åva gymnasium. All rights reserved.
 *
 */

#ifndef __ENTITIES_CHAT_H__
#define __ENTITIES_CHAT_H__

#include <SFML/Graphics.hpp>
#include <string>
#include <list>

#include "Gaem/entity.h"

namespace Entities
{
	class Chat: public Gaem::Entity
	{
		struct message_type {
			std::string message;
			std::string username;
			sf::Clock received;
		};
		
		typedef std::list<message_type*> message_list;
		
		message_list _messages;
	public:
		static bool removeOld(message_type *);
	
		Chat();
		
		void addMessage(const std::string &username, const std::string &message);
		
		void draw(sf::RenderWindow &window);
		void handleEvent(const sf::Event &event);
		void logic();
	};
}

#endif

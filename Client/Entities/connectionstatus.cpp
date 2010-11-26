/*
 *  connectionstatus.cpp
 *  Spel
 *
 *  Created by Erik Andersson on 2010-11-15.
 *  Copyright 2010 Åva gymnasium. All rights reserved.
 *
 */

#include <SFML/Graphics.hpp>
#include <sstream>

#include "Gaem/gaem.h"
#include "Gaem/resourcemanager.h"
#include "Gaem/networkmanager.h"

#include "Entities/connectionstatus.h"

namespace Entities
{
	void ConnectionStatus::handleEvent(const sf::Event &e)
	{}
	
	void ConnectionStatus::logic()
	{}
	
	void ConnectionStatus::draw(sf::RenderWindow &window)
	{
		bool is_connected = Gaem::Gaem::getInstance()->getNetworkManager()->isConnected();
		
		if ( ! is_connected )
		{
			sf::Shape rectangle1 = sf::Shape::Rectangle(0, 0, 15, 50, sf::Color::White);
			sf::Shape rectangle2 = sf::Shape::Rectangle(0, 0, 15, 50, sf::Color::White);
			
			rectangle1.SetPosition(10, 10);
			rectangle2.SetPosition(rectangle1.GetPosition().x + 15 + 10, 10);
			
			window.Draw(rectangle1);
			window.Draw(rectangle2);
		}
		else
		{
			return;
			std::stringstream ss;
			ss << "Ping: " << Gaem::Gaem::getInstance()->getNetworkManager()->getPing();
			
			std::string str = ss.str();
			sf::String ping(str, *Gaem::Gaem::getInstance()->getResourceManager()->getFont("resources/main_font.ttf", 15), 15);
			ping.SetPosition(10, 10);
			window.Draw(ping);
		}
	}
}

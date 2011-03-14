/*
 *  statusmessage.cpp
 *  Spel
 *
 *  Created by Erik Andersson on 2011-03-14.
 *  Copyright 2011 Åva gymnasium. All rights reserved.
 *
 */

#include "Gaem/gaem.h"
#include "Gaem/resourcemanager.h"

#include "Entities/statusmessage.h"

namespace Entities
{
	StatusMessage::StatusMessage(): Gaem::Entity(), _message("")
	{
		_font = Gaem::Gaem::getInstance()->getResourceManager()->getFont("resources/market_deco.ttf", 30);
	}
	
	void StatusMessage::draw(sf::RenderWindow &window)
	{
		if ( _message == "" ) return;
		
		sf::String mess(_message, *_font);
		
		int w = mess.GetRect().Right - mess.GetRect().Left;
		int h = mess.GetRect().Bottom - mess.GetRect().Top;
		
		int centerX = Gaem::Gaem::getInstance()->getWidth()/2 - w/2;
		int centerY = Gaem::Gaem::getInstance()->getHeight()/2 - h/2;
		
		mess.SetX(centerX);
		mess.SetY(centerY);
		
		window.Draw(mess);
	}
	
	void StatusMessage::setMessage(std::string mess)
	{
		_message = mess;
	}
	
	void StatusMessage::logic()
	{
		
	}
	
	void StatusMessage::handleEvent(const sf::Event &)
	{
		
	}
	
	int StatusMessage::getZIndex()
	{
		return 1001;
	}
}

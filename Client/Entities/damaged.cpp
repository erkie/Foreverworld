/*
 *  damaged.cpp
 *  Spel
 *
 *  Created by Erik Andersson on 2011-02-08.
 *  Copyright 2011 Åva gymnasium. All rights reserved.
 *
 */

#include <iostream>
#include <sstream>
#include <SFML/Graphics.hpp>

#include "Gaem/gaem.h"
#include "Gaem/resourcemanager.h"
#include "Gaem/entitymanager.h"

#include "Entities/player.h"
#include "Entities/damaged.h"

namespace Entities
{
	Damaged::Damaged(): _last_w(0), _last_h(0)
	{
		_left = new sf::Sprite(*Gaem::Gaem::getInstance()->getResourceManager()->getImage("resources/damaged-left.png"));
		_right = new sf::Sprite(*Gaem::Gaem::getInstance()->getResourceManager()->getImage("resources/damaged-right.png"));
		
	}
	
	Damaged::~Damaged()
	{
		delete _left;
		delete _right;
	}
	
	void Damaged::logic()
	{
		Entities::Player *player = Gaem::Gaem::getInstance()->getEntityManager()->getCurrentPlayer();
		if ( ! player )
			return;
		
		int opacity = (1-player->getHP()) * 255;
		
		float w, h;
		w = Gaem::Gaem::getInstance()->getWidth();
		h = Gaem::Gaem::getInstance()->getHeight();
		
		_left->SetColor(sf::Color(255, 255, 255, opacity));
		_right->SetColor(sf::Color(255, 255, 255, opacity));
		
		_left->SetX(50);
		_left->SetY(50);
		
		_right->SetX(w - _right->GetSize().x - 50);
		_right->SetY(50);
		
		if ( _last_w != w || _last_h != h )
		{
			_left->SetScale(1, 1);
			_right->SetScale(1, 1);
			
			_left->SetScale(1, (h - 100)/_left->GetSize().y);
			_right->SetScale(1, (h - 100)/_right->GetSize().y);
		}
		
		_last_w = w;
		_last_h = h;
	}
	
	void Damaged::handleEvent(const sf::Event &event)
	{}
	
	void Damaged::draw(sf::RenderWindow &window)
	{
		Entities::Player *player = Gaem::Gaem::getInstance()->getEntityManager()->getCurrentPlayer();
		if ( ! player )
			return;
		
		window.Draw(*_left);
		window.Draw(*_right);
		
		/*std::stringstream ss;
		ss << player->getHP();
		
		sf::String str(ss.str());
		window.Draw(str);*/
	}
	
	int Damaged::getZIndex()
	{
		return 500;
	}
}
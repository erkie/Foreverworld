/*
 *  WarbirdAttack.cpp
 *  Spel
 *
 *  Created by Erik Andersson on 2011-03-13.
 *  Copyright 2011 Åva gymnasium. All rights reserved.
 *
 */

#include "Gaem/entitymanager.h"
#include "WarbirdAttack.h"

namespace Attacks
{
	Warbird::Warbird(const std::string &cfg, Entities::Player *me): Gaem::Attack(cfg, me)
	{
		Gaem::Config config(cfg);
		
		_tornado = new Gaem::AnimatedSprite();
		_tornado->loadAnimation("tornado", config.get("tornado animation"));
		_tornado->setAnimation("tornado");
	}
	
	Warbird::~Warbird()
	{
		delete _tornado;
	}
	
	Gaem::player_hitlist Warbird::isHit(int frame)
	{
		if ( _did_hit )
			return Gaem::player_hitlist();
		
		// Get all players
		Gaem::player_map players = Gaem::Gaem::getInstance()->getEntityManager()->getPlayers();
		
		// Return people near me
		Gaem::player_hitlist res;
		
		for ( Gaem::player_map::iterator iter = players.begin(); iter != players.end(); iter++ )
		{
			Entities::Player *player = (*iter).second;
			
			if ( player == _me ) continue;
			
			int left = player->getLeft();
			
			int distanceX = abs(_me->getLeft() - left);
			float distanceY = fabs(_me->getDepth() - player->getDepth());
			
			if ( distanceX < 50 && distanceY < 20 )
			{
				res.push_back(player);
			}
		}
		
		return res;
	}
	
	void Warbird::drawAttackAtAfter(sf::FloatRect pos, sf::RenderWindow &window)
	{
		// Draw tornado behind warbird
		_tornado->setX(pos.Left + (pos.Right-pos.Left)/2 - _tornado->getWidth()/2);
		_tornado->setY(pos.Bottom - _tornado->getHeight() + _me->getElevation());
		
		window.Draw(*_tornado->getSprite());
	}
	
	void Warbird::step()
	{
		_tornado->step();
	}
	
	void Warbird::end()
	{
		Gaem::Attack::end();
		
		_tornado->getAnimation()->reallyReset();
	}
	
	bool Warbird::hasOwnAnimation()
	{
		return true;
	}
	
	bool Warbird::isDone()
	{
		return _tornado->getAnimation()->isDone();
	}
	
	bool Warbird::abortByJump()
	{
		return false;
	}
	
	std::string Warbird::getAnimation()
	{
		return "-none-";
	}
}

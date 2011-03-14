/*
 *  MaarnAttack.cpp
 *  Spel
 *
 *  Created by Erik Andersson on 2011-03-13.
 *  Copyright 2011 Åva gymnasium. All rights reserved.
 *
 */

#include <iostream>

#include "Gaem/config.h"
#include "Gaem/animatedsprite.h"
#include "Gaem/entitymanager.h"
#include "Gaem/resourcemanager.h"

#include "Entities/world.h"

#include "MaarnAttack.h"

namespace Attacks
{
	Maarn::Maarn(const std::string &cfg, Entities::Player *me): Gaem::Attack(cfg, me)
	{
		Gaem::Config config(cfg);
		
		_tiger = Gaem::Gaem::getInstance()->getResourceManager()->getImage(config.get("tiger"));
		_x = 0;
		_dir = me->getDirX();
	}
	
	Maarn::~Maarn()
	{
		//delete _tiger;
	}
	
	Gaem::player_hitlist Maarn::isHit(int frame)
	{
		if ( _did_hit )
			return Gaem::player_hitlist();
		
		// Get all players
		Gaem::player_map players = Gaem::Gaem::getInstance()->getEntityManager()->getPlayers();
		
		int world_width = Gaem::Gaem::getInstance()->getEntityManager()->getWorld()->getWidth();
		
		// Return people near me
		Gaem::player_hitlist res;
		
		int my_left = _me->getLeft() + _x;
		if ( my_left < 0 )
			my_left = world_width + my_left;
		if ( my_left > world_width )
			my_left = my_left - world_width;
		
		for ( Gaem::player_map::iterator iter = players.begin(); iter != players.end(); iter++ )
		{
			Entities::Player *player = (*iter).second;
			
			if ( player == _me || player->getDead() ) continue;
			
			int left = player->getLeft();
			
			int distanceX = abs(my_left - left);
			float distanceY = fabs(_me->getDepth() - player->getDepth());
			
			if ( distanceX < 50 && distanceY < 20 )
			{
				res.push_back(player);
			}
		}
		
		return res;	
	}
	
	void Maarn::start()
	{
		_x = 0;
		_dir = _me->getFacingDir() ? -1 : 1;
	}
	
	void Maarn::step()
	{
		_x += Gaem::Gaem::getInstance()->getTDelta() * 500 * _dir;
	}
	
	void Maarn::drawAttackAtAfter(sf::FloatRect pos, sf::RenderWindow &window)
	{
		sf::Sprite tiger;
		tiger.SetImage(*_tiger);
		tiger.FlipX(_dir == -1);
		
		// Draw shards behind Maarn
		tiger.SetX(pos.Left + (pos.Right-pos.Left)/2 - _tiger->GetWidth()/2 + _x);
		tiger.SetY(pos.Bottom - _tiger->GetHeight());
		
		window.Draw(tiger);
	}
}

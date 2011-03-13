/*
 *  KooriAttack.cpp
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

#include "KooriAttack.h"

namespace Attacks
{
	Koori::Koori(const std::string &cfg, Entities::Player *me): Gaem::Attack(cfg, me)
	{
		Gaem::Config config(cfg);
		
		_shards = new Gaem::AnimatedSprite();
		_shards->loadAnimation("shards", config.get("shards animation"));
		_shards->setAnimation("shards");
	}
	
	Koori::~Koori()
	{
		delete _shards;
	}
	
	Gaem::player_hitlist Koori::isHit(int frame)
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
			float distanceY = fabs(_me->getElevation() - player->getElevation());
			
			if ( distanceX < 50 && distanceY < 20 )
			{
				res.push_back(player);
			}
		}
		
		return res;	
	}
	
	void Koori::step()
	{
		_shards->step();
	}
	
	void Koori::end()
	{
		Gaem::Attack::end();
		_shards->setAnimation("shards", true);
	}
	
	void Koori::drawAttackAtAfter(sf::FloatRect pos, sf::RenderWindow &window)
	{
		// Draw shards behind Koori
		_shards->setX(pos.Left + (pos.Right-pos.Left)/2 - _shards->getWidth()/2);
		_shards->setY(pos.Bottom - _shards->getHeight());
		
		window.Draw(*_shards->getSprite());
	}
}

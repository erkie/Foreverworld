/*
 *  EmanonAttack.cpp
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

#include "Entities/player.h"
#include "Entities/world.h"

#include "EmanonAttack.h"

namespace Attacks
{
	Emanon::Emanon(const std::string &cfg, Entities::Player *me): Gaem::Attack(cfg, me)
	{
		Gaem::Config config(cfg);
		
		me->getSprite()->loadAnimation("attack2_appear", config.get("appear animation"));
		_current_kill = NULL;
	}
	
	void Emanon::start()
	{
		//std::cout << "Let's begin!\n";
		_is_done = false;
		_in_killing = false;
		_current_kill = NULL;
	}
	
	void Emanon::step()
	{
		if ( _me->getSprite()->getAnimation()->isDone() && _to_kill.size() == 0 && ! _in_killing )
		{
			// First animation is done!
			_in_killing = false;
			_current_kill = NULL;
			
			// Pick out five near players to hit
			Gaem::player_map players = Gaem::Gaem::getInstance()->getEntityManager()->getPlayers();
			for ( Gaem::player_map::iterator iter = players.begin(); iter != players.end(); iter++ )
			{
				if ( (*iter).second == _me || (*iter).second->getDead() ) continue;
				
				if ( isPlayerNearMe((*iter).second) )
				{
					_to_kill.push((*iter).second);
					
					// Only allow five to be killed
					if ( _to_kill.size() == 5 )
						break;
				}
			}
			
			if ( _to_kill.size() == 0 )
				_is_done = true;
		}
		
		// If we have anyone to kill, move to them, play hit roll, then play roll1, and do the same
		bool didJustChange = false;
		if ( _to_kill.size() && _me->getSprite()->getAnimation()->isDone() )
		{
			//std::cout << "Move somewhere " << _to_kill.size() << "\n";
			
			_in_killing = true;
			_current_kill = _to_kill.front();
			std::cout << _current_kill << "\n";
			
			// Move me to the current player and show appear animation
			_me->setLeft(_to_kill.front()->getLeft());
			_me->setDepth(_to_kill.front()->getDepth());
			
			_me->getSprite()->setAnimation("attack2_appear", true);
			
			_to_kill.pop();
			
			didJustChange = true;
		}
		
		// If we have played the last appear animation and there is nothing more to kill
		if ( _to_kill.size() == 0 && _me->getSprite()->getAnimation()->isDone() && ! didJustChange )
		{
			//std::cout << "Done!\n";
			
			_is_done = true;
			_in_killing = false;
			_current_kill = NULL;
		}
	}
	
	Gaem::player_hitlist Emanon::isHit(int frame)
	{
		Gaem::player_hitlist res;
		
		// Frame 4 is the hit frame
		if ( _in_killing && frame == 4 && _current_kill )
		{
			//std::cout << "Current kill is not null! And we are on frame number 4.\n";
			res.push_back(_current_kill);
			_current_kill = NULL;
		}
		
		return res;
	}
	
	bool Emanon::isDone()
	{
		return _is_done;
	}

	bool Emanon::isPlayerNearMe(Entities::Player *p)
	{
		int myLeft = _me->getLeft();
		int playerLeft = p->getLeft();
		
		int worldWidth = Gaem::Gaem::getInstance()->getEntityManager()->getWorld()->getWidth();
		
		int diff = abs(myLeft - playerLeft);
		if ( diff > worldWidth )
			diff = diff - worldWidth;
		
		return diff < 500;
	}
}

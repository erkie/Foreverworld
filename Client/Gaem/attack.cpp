/*
 *  attack.cpp
 *  Spel
 *
 *  Created by Erik Andersson on 2011-01-16.
 *  Copyright 2011 Åva gymnasium. All rights reserved.
 *
 */

#include <sstream>
#include <iostream>

#include "Gaem/gaem.h"
#include "Gaem/config.h"
#include "Gaem/attack.h"
#include "Gaem/entitymanager.h"

#include "Entities/player.h"

namespace Gaem
{
	Attack::Attack(const std::string &file, Entities::Player *me): _id("undefined"), _cooldown(0), _damage(0), _in_attack(false), _did_hit(false), _me(me)
	{
		Config config(file);
		
		_id = config.get("id");
		_cooldown = config.getFloat("cooldown", 0);
		_damage = config.getFloat("damage", 0);
		
		// Get keyboard combo
		std::stringstream ss(config.get("combo"));
		char c;
		while ( ss >> c )
		{
			_combo.push_back(c);
		}
		
		// Get hitpoints
		cfg_strings hitpoints = config.getVector("hitpoint");
		
		int i = 0;
		for ( cfg_strings::iterator iter = hitpoints.begin(); iter != hitpoints.end(); iter++ )
		{
			if ( *iter == "" ) continue;
			
			std::stringstream sp(*iter);
			int x, y;
			char _x_;
			sp >> x >> _x_ >> y;
			
			_hitpoints[i++] = sf::Vector2i(x, y);
		}
		
		// Reset timer
		_current_timer.Reset();
	}
	
	Entities::Player* Attack::isHit(int frame)
	{
		if ( _did_hit )
			return false;
		
		// Get hitpoint
		sf::Vector2i hitpoint = _hitpoints[frame];
		hitpoint.x += _me->getLeft();
		
		// Get all players
		player_map players = Gaem::Gaem::getInstance()->getEntityManager()->getPlayers();
		
		// Return whether it hit anybody
		for ( player_map::iterator iter = players.begin(); iter != players.end(); iter++ )
		{
			Entities::Player *player = (*iter).second;
			
			if ( player == _me ) continue;
			
			if ( player->isOn(hitpoint, _me->getDepth()) )
			{
				_did_hit = true;
				return player;
			}
		}
		
		return NULL;
	}
	
	void Attack::end()
	{
		_current_timer.Reset();
		_in_attack = false;
		_did_hit = false;
		_started_time = 0;
		_current_combo.erase(_current_combo.begin(), _current_combo.end());
	}
	
	bool Attack::handleAttack(sf::Key::Code code)
	{
		if ( _in_attack )
			return false;
	
		bool is_first = _current_combo.size() == 0;
		
		if ( _combo[_current_combo.size()] == (char)code )
		{
			if ( is_first || _current_timer.GetElapsedTime() < 0.1 )
			{
				_current_combo.push_back((char)code);
				_current_timer.Reset();
			}
		}
		else
		{
			_current_combo.erase(_current_combo.begin(), _current_combo.end());
		}
		
		if ( _current_combo.size() == _combo.size() )
		{
			_in_attack = true;
			_started_time = RakNet::GetTime();
			return true;
		}
		return false;
	}
	
	void Attack::setAnimation(const std::string &a)
	{
		_animation = a;
	}
	
	void Attack::setElapsedTime(RakNet::Time t)
	{
		
	}
	
	std::string Attack::getID()
	{
		return _id;
	}
	
	std::string Attack::getAnimation()
	{
		return _animation;
	}
	
	float Attack::getCooldown()
	{
		return _cooldown;
	}
	
	float Attack::getDamage()
	{
		return _damage;
	}
	
	RakNet::Time Attack::getStartTime()
	{
		return _started_time;
	}
}

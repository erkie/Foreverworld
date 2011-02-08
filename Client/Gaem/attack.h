/*
 *  attack.h
 *  Spel
 *
 *  Created by Erik Andersson on 2011-01-16.
 *  Copyright 2011 Åva gymnasium. All rights reserved.
 *
 */

#ifndef __GAEM_ATTACK_H__
#define __GAEM_ATTACK_H__

#include <map>
#include <vector>
#include <list>

#include <SFML/Graphics.hpp>
#include "GetTime.h"

namespace Entities
{
	class Player;
}

namespace Gaem
{
	class Attack
	{
		typedef std::map<int, sf::Vector2<int> > hitpoint_t;
		
		std::string _id;
		float _cooldown;
		float _damage;
		std::vector<char> _combo;
		hitpoint_t _hitpoints;
		
		std::string _animation;
		
		bool _in_attack;
		bool _did_hit;
		std::list<char> _current_combo;
		sf::Clock _current_timer;
		RakNet::Time _started_time;
		
		Entities::Player *_me;
	public:
		Attack(const std::string &, Entities::Player *me);
		
		Entities::Player *isHit(int frame);
		void end();
		bool handleAttack(sf::Key::Code);
		
		void setAnimation(const std::string &);
		void setElapsedTime(RakNet::Time);
		
		std::string getID();
		std::string getAnimation();
		float getCooldown();
		float getDamage();
		RakNet::Time getStartTime();
	};
}

#endif

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
	typedef std::list<Entities::Player*> player_hitlist;
	
	class Attack
	{
	protected:
		typedef std::map<int, sf::Vector2<int> > hitpoint_t;
		
		std::string _id;
		float _cooldown;
		float _damage;
		std::vector<char> _combo;
		hitpoint_t _hitpoints;
		float _required_mana;
		
		std::string _animation;
		
		bool _in_attack;
		bool _did_hit;
		std::list<char> _current_combo;
		sf::Clock _current_timer;
		RakNet::Time _started_time;
		
		Entities::Player *_me;
	public:
		Attack(const std::string &, Entities::Player *me);
		
		// Check whether attack has hit anyone
		virtual player_hitlist isHit(int frame);
		
		// Called when the attack is commenced
		virtual void start();
		
		// Reset everything after we are done
		virtual void end();
		
		// Check whether enough keys have been hit in the combo
		bool handleAttack(sf::Key::Code, float mana);
		
		void setAnimation(const std::string &);
		void setElapsedTime(RakNet::Time);
		
		std::string getID();
		virtual std::string getAnimation();
		float getCooldown();
		float getDamage();
		float getRequiredMana();
		RakNet::Time getStartTime();
		
		virtual void drawAttackAtBefore(sf::FloatRect pos, sf::RenderWindow &window);
		virtual void drawAttackAtAfter(sf::FloatRect pos, sf::RenderWindow &window);
		virtual bool isDone();
		virtual bool hasOwnAnimation();
		virtual bool abortByJump();
		virtual void step();
	};
}

#endif

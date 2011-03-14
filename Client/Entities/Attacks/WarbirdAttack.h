/*
 *  WarbirdAttack.h
 *  Spel
 *
 *  Created by Erik Andersson on 2011-03-13.
 *  Copyright 2011 Åva gymnasium. All rights reserved.
 *
 */

#ifndef __WARBIRD_ATTACK_H__
#define __WARBIRD_ATTACK_H__

#include "Gaem/gaem.h"
#include "Gaem/attack.h"
#include "Gaem/animatedsprite.h"
#include "Entities/player.h"

namespace Attacks
{
	class Warbird: public Gaem::Attack
	{
		Gaem::AnimatedSprite *_tornado;
		sf::Clock _last_check;
		
	public:
		Warbird(const std::string &cfg, Entities::Player *me);
		~Warbird();
		
		void drawAttackAtAfter(sf::FloatRect pos, sf::RenderWindow &window);
		void step();
		void end();

		Gaem::player_hitlist isHit(int frame);
		
		std::string getAnimation();
		bool hasOwnAnimation();
		bool isDone();
		bool abortByJump();
	};
}

#endif

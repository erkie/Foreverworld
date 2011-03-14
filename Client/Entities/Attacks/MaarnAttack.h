/*
 *  MaarnAttack.h
 *  Spel
 *
 *  Created by Erik Andersson on 2011-03-13.
 *  Copyright 2011 Åva gymnasium. All rights reserved.
 *
 */


#ifndef __MAARN_ATTACK_H__
#define __MAARN_ATTACK_H__

#include "Gaem/gaem.h"
#include "Gaem/attack.h"
#include "Gaem/animatedsprite.h"
#include "Entities/player.h"

namespace Attacks
{
	class Maarn: public Gaem::Attack
	{
		sf::Image *_tiger;
		int _x, _dir;
	public:
		Maarn(const std::string &cfg, Entities::Player *me);
		~Maarn();
		
		Gaem::player_hitlist isHit(int frame);
		
		void start();
		void step();
		
		void drawAttackAtAfter(sf::FloatRect pos, sf::RenderWindow &window);
	};
}

#endif

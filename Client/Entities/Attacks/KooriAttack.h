/*
 *  KooriAttack.h
 *  Spel
 *
 *  Created by Erik Andersson on 2011-03-13.
 *  Copyright 2011 Åva gymnasium. All rights reserved.
 *
 */

#ifndef __KOORI_ATTACK_H__
#define __KOORI_ATTACK_H__

#include "Gaem/gaem.h"
#include "Gaem/attack.h"
#include "Gaem/animatedsprite.h"
#include "Entities/player.h"

namespace Attacks
{
	class Koori: public Gaem::Attack
	{
		Gaem::AnimatedSprite *_shards;
	public:
		Koori(const std::string &cfg, Entities::Player *me);
		~Koori();
		
		Gaem::player_hitlist isHit(int frame);
		
		void step();
		void end();
		
		void drawAttackAtAfter(sf::FloatRect pos, sf::RenderWindow &window);
	};
}

#endif

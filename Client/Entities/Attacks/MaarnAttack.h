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
		Gaem::AnimatedSprite *_shards;
	public:
		Maarn(const std::string &cfg, Entities::Player *me): Gaem::Attack(cfg, me) {}
	};
}

#endif

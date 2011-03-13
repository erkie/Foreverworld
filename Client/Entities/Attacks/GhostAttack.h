/*
 *  GhostAttack.h
 *  Spel
 *
 *  Created by Erik Andersson on 2011-03-13.
 *  Copyright 2011 Åva gymnasium. All rights reserved.
 *
 */


#ifndef __GHOST_ATTACK_H__
#define __GHOST_ATTACK_H__

#include "Gaem/attack.h"

namespace Attacks
{
	class Ghost: public Gaem::Attack
	{
	public:
		Ghost(const std::string &cfg, Entities::Player *me): Gaem::Attack(cfg, me) {}
	};
}

#endif

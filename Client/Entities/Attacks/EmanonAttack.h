/*
 *  EmanonAttack.h
 *  Spel
 *
 *  Created by Erik Andersson on 2011-03-13.
 *  Copyright 2011 Åva gymnasium. All rights reserved.
 *
 */


#ifndef __EMANON_ATTACK_H__
#define __EMANON_ATTACK_H__

#include "Gaem/gaem.h"
#include "Gaem/attack.h"

namespace Attacks
{
	class Emanon: public Gaem::Attack
	{
	public:
		Emanon(const std::string &cfg, Entities::Player *me): Gaem::Attack(cfg, me) {}
	};
}

#endif

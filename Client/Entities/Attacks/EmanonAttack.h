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

#include <queue>

#include "Gaem/gaem.h"
#include "Gaem/attack.h"

#include "Entities/player.h"

namespace Attacks
{
	typedef std::queue<Entities::Player*> player_queue;
	
	class Emanon: public Gaem::Attack
	{
		player_queue _to_kill;
		bool _is_done;
		bool _in_killing;
		
		Entities::Player *_current_kill;
	public:
		Emanon(const std::string &cfg, Entities::Player *me);
		
		Gaem::player_hitlist isHit(int frame);
		
		void start();
		void step();
		bool isDone();
		
		bool isPlayerNearMe(Entities::Player*);
	};
}

#endif

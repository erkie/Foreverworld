/*
 *  GhostAttack.cpp
 *  Spel
 *
 *  Created by Erik Andersson on 2011-03-13.
 *  Copyright 2011 Åva gymnasium. All rights reserved.
 *
 */

#include "Gaem/gaem.h"
#include "Gaem/animatedsprite.h"
#include "Gaem/entitymanager.h"
#include "Gaem/resourcemanager.h"

#include "Entities/world.h"
#include "Entities/player.h"

#include "GhostAttack.h"

namespace Attacks
{
	Gaem::player_hitlist Ghost::isHit(int frame)
	{
		if ( _did_hit )
			return Gaem::player_hitlist();
		
		// Get all players
		Gaem::player_map players = Gaem::Gaem::getInstance()->getEntityManager()->getPlayers();
		
		// Return person in my line of site
		Gaem::player_hitlist res;
		
		for ( Gaem::player_map::iterator iter = players.begin(); iter != players.end(); iter++ )
		{
			Entities::Player *player = (*iter).second;
			
			if ( player == _me || player->getDead() ) continue;
			
			int left = player->getLeft();
			int facingDir = _me->getFacingDir() ? -1 : 1;
			
			int absDistanceX = abs(_me->getLeft() - left);
			int distanceX = _me->getLeft() - left;
			float distanceY = fabs(_me->getDepth() - player->getDepth());
			
			// Not in my line of site
			if ( facingDir == 1 && distanceX > 0 )
				continue;
			else if ( facingDir == -1 && distanceX < 0 )
				continue;
			
			// If I'm within a distance of 1000 pixels
			if ( absDistanceX < 1000 && distanceY < 20 )
			{
				_did_hit = true;
				res.push_back(player);
			}
		}
		
		return res;	
	}
}

/*
 *  player.h
 *  Spel
 *
 *  Created by Erik Andersson on 2010-11-15.
 *  Copyright 2010 Åva gymnasium. All rights reserved.
 *
 */

#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <string>
#include "Common/messages.h"

class Player
{
	std::string _username;
	
	inet::id_type _id;
	
	int _dir;
	int _pos_left;
	float _pos_depth;
	float _elevation;
	
	// Specs
	std::string _character;
public:
	Player(const std::string &);
	
	void loadSpecs();
	void setCharacter(const std::string &);
	void setId(inet::id_type);
	
	inet::id_type getId();
	int getDir();
	int getLeft();
	float getDepth();
	float getElevation();
	std::string getCharacter();
	
	inet::Player getPlayer();
};

#endif

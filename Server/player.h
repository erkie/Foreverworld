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
	inet::PlayerActionState _state;
	
	// Specs
	std::string _character;
public:
	Player(const std::string &);
	
	// Update player stats every second
	void tick();
	
	void loadSpecs();
	void setCharacter(const std::string &);
	void setId(inet::id_type);
	
	void setDir(int);
	void setLeft(int);
	void setDepth(float);
	void setElevation(float);
	void setState(inet::PlayerActionState);
	
	inet::id_type getId();
	int getDir();
	int getLeft();
	float getDepth();
	float getElevation();
	std::string getCharacter();
	
	inet::Player getPlayer();
	inet::PlayerState getState();
};

#endif

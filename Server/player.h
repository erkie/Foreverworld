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
	inet::id_type _id;
	
	inet::PlayerState _state;
	
	// Specs
	std::string _character;
	std::string _username;
public:
	Player(const std::string &);
	
	// Update player stats every second
	void tick();
	
	void loadSpecs();
	void setCharacter(const std::string &);
	void setUsername(const std::string &);
	void setId(inet::id_type);
	
	void setDir(int, int);
	void setLeft(int);
	void setDepth(float);
	void setElevation(float);
	void setVelocity(float, float);
	void setState(inet::PlayerActionState);
	
	inet::id_type getId();
	int *getDir();
	int getLeft();
	float getDepth();
	float getElevation();
	float *getVelocity();
	std::string getCharacter();
	std::string getUsername();
	
	inet::Player getPlayer();
	inet::PlayerState getState();
};

#endif

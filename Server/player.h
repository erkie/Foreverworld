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
#include "usermanager.h"

class Player
{
	inet::id_type _id;
	
	inet::PlayerState _state;
	
	// Specs
	inet::LoggedInMemberData _member;
public:
	Player(inet::id_type id);
	
	static UserManager *user_manager;
	
	// Update player stats every second
	void tick();
	
	void loadSpecs();
	
	void setDir(int, int);
	void setFlyingDir(int);
	void setLeft(int);
	void setDepth(float);
	void setElevation(float);
	void setVelocity(float, float);
	void setState(inet::PlayerActionState);
	void setCharacter(int32_t);
	void setAttackId(std::string);
	void setHP(float);
	
	inet::id_type getId();
	int *getDir();
	int getLeft();
	float getDepth();
	float getElevation();
	float *getVelocity();
	float getHP();
	std::string getUsername();
	
	inet::PlayerState getState();
	inet::LoggedInMemberData getMember();
};

#endif

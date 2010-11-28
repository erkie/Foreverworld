/*
 *  player.cpp
 *  Spel
 *
 *  Created by Erik Andersson on 2010-11-15.
 *  Copyright 2010 Åva gymnasium. All rights reserved.
 *
 */

#include <cstdlib>
#include <cstring>

#include "player.h"
#include "usermanager.h"

UserManager *Player::user_manager = NULL;

Player::Player(inet::id_type id)
{
	_id = id;

	// Load user credentials into
	 _member = Player::user_manager->loadMember(id);
	
	_state.left = 100;
	_state.depth = 50;
	_state.elevation = 0;
	_state.dir[0] = 1;
	_state.dir[1] = 0;
	_state.velocity[0] = 0;
	_state.velocity[1] = 0;
	
	// Get my character's specs
}

void Player::setDir(int dir_x, int dir_y)
{
	_state.dir[0] = dir_x;
	_state.dir[1] = dir_y;
}

void Player::setLeft(int left)
{
	_state.left = left;
}

void Player::setElevation(float el)
{
	_state.elevation = el;
}

void Player::setDepth(float depth)
{
	_state.depth = depth;
}

void Player::setVelocity(float x, float y)
{
	_state.velocity[0] = x;
	_state.velocity[1] = y;
}

void Player::setState(inet::PlayerActionState state)
{
	_state.state = state;
}

void Player::setCharacter(int32_t id)
{
	_member.character_id = id;
}

inet::id_type Player::getId()
{
	return _id;
}

int* Player::getDir()
{
	return _state.dir;
}

int Player::getLeft()
{
	return _state.left;
}

float Player::getDepth()
{
	return _state.depth;
}

float Player::getElevation()
{
	return _state.elevation;
}

float *Player::getVelocity()
{
	return _state.velocity;
}

std::string Player::getUsername()
{
	return "USERNAMEFIXME";
}

inet::PlayerState Player::getState()
{
	inet::PlayerState p;
	p.dir[0] = *(getDir());
	p.dir[1] = *(getDir()+1);
	p.left = getLeft();
	p.depth = getDepth();
	p.elevation = getElevation();
	p.velocity[0] = *(getVelocity());
	p.velocity[1] = *(getVelocity()+1);
	p.state = _state.state;
	return p;
}

inet::LoggedInMemberData Player::getMember()
{
	return _member;
}

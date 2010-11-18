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

Player::Player(const std::string &name)
{
	_username = name;
	
	_state.left = 100;
	_state.depth = 50;
	_state.elevation = 0;
	_state.dir[0] = 1;
	_state.dir[1] = 0;
	_state.velocity[0] = 0;
	_state.velocity[1] = 0;
}

void Player::setCharacter(const std::string &name)
{
	// Load specs
	_character = name;
}

void Player::setUsername(const std::string &name)
{
	_username = name;
}

void Player::setId(inet::id_type id)
{
	_id = id;
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
	return _username;
}

std::string Player::getCharacter()
{
	return _character;
}

inet::Player Player::getPlayer()
{
	inet::Player p;
	p.dir[0] = *(getDir());
	p.dir[1] = *(getDir()+1);
	p.left = getLeft();
	p.depth = getDepth();
	p.elevation = getElevation();
	memcpy(p.character, getCharacter().c_str(), 50);
	memcpy(p.username, getUsername().c_str(), 50);
	return p;
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

/*
 *  player.cpp
 *  Spel
 *
 *  Created by Erik Andersson on 2010-11-15.
 *  Copyright 2010 Åva gymnasium. All rights reserved.
 *
 */

#include <cstdlib>
#include "player.h"

Player::Player(const std::string &name)
{
	_username = name;
	
	_pos_left = 100;
	_pos_depth = 50;
	_elevation = 0;
	_dir = 1;
}

void Player::setCharacter(const std::string &name)
{
	// Load specs
	_character = name;
}

void Player::setId(inet::id_type id)
{
	_id = id;
}

void Player::setDir(int dir)
{
	_dir = dir;
}

void Player::setLeft(int left)
{
	_pos_left = left;
}

void Player::setElevation(float el)
{
	_elevation = el;
}

void Player::setDepth(float depth)
{
	_pos_depth = depth;
}

void Player::setState(inet::PlayerActionState state)
{
	_state = state;
}

inet::id_type Player::getId()
{
	return _id;
}

int Player::getDir()
{
	return _dir;
}

int Player::getLeft()
{
	return _pos_left;
}

float Player::getDepth()
{
	return _pos_depth;
}

float Player::getElevation()
{
	return _elevation;
}

std::string Player::getCharacter()
{
	return _character;
}

inet::Player Player::getPlayer()
{
	char character[50];
	memcpy(character, getCharacter().c_str(), 50);
	
	inet::Player p;
	p.dir = getDir();
	p.left = getLeft();
	p.depth = getDepth();
	p.elevation = getElevation();
	memcpy(p.character, character, 50);
	return p;
}

inet::PlayerState Player::getState()
{
	inet::PlayerState p;
	p.dir = getDir();
	p.left = getLeft();
	p.depth = getDepth();
	p.elevation = getElevation();
	p.state = _state;
	return p;
}

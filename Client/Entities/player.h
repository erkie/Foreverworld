/*
 *  player.h
 *  Spel
 *
 *  Created by Erik Andersson on 2010-10-16.
 *  Copyright 2010 Åva gymnasium. All rights reserved.
 *
 */

#ifndef __ENTITIES_GAEM_H__
#define __ENTITIES_GAEM_H__

#include <SFML/Graphics.hpp>

#include "Gaem/entity.h"
#include "Gaem/animatedsprite.h"

#include "Common/messages.h"

namespace Gaem
{
	class User;
}

namespace Entities
{
	class Player: public Gaem::Entity
	{
	protected:
		sf::Vector2f _position;
		
		Gaem::AnimatedSprite *_sprite;
		
		// Player specifc
		float _scale;
		float _speed;
		float _speed_up;
		bool _can_jump;
		std::string _name;
		
		Gaem::User *_user;
		
		int _dir[2];
		int _pos_left;
		float _pos_depth;
		float _elevation;
		sf::Vector2f _velocity;
		inet::PlayerActionState _state;
	public:
		Player(const std::string &);
		~Player();
		
		void setUser(Gaem::User *);
		
		void setDepth(float);
		float getDepth();
		
		int getLeft();
		int getRight();
		float getElevation();
		
		void setDir(int, int);
		void setLeft(int);
		void setElevation(float);
		void setVelocity(float, float);
		void setState(inet::PlayerActionState);
		
		bool isActivePlayer();
		
		void jump();
		void runLeft();
		void runRight();
		void moveUp();
		void moveDown();
		
		void clampPos();
		
		void handleEvent(const sf::Event &);
		void logic();
		void draw(sf::RenderWindow &);
		
		inet::PlayerState getPlayerStruct();
	};
}

#endif

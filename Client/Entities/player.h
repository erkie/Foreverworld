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

#include <map>
#include <SFML/Graphics.hpp>

#include "Gaem/entity.h"
#include "Gaem/animatedsprite.h"
#include "Gaem/attack.h"

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
		typedef std::map<std::string, Gaem::Attack*> attack_map;
		sf::Vector2f _position;
		
		Gaem::AnimatedSprite *_sprite;
		
		// Character-specifc
		float _scale;
		float _speed;
		float _speed_up;
		bool _can_jump;
		std::string _name;
		
		// Player user
		Gaem::User *_user;
		
		// Position and movement
		int _dir[2];
		int _pos_left;
		float _pos_depth;
		float _elevation;
		sf::Vector2f _velocity;
		inet::PlayerActionState _state;
		
		// Attack related
		attack_map _attacks;
		Gaem::Attack *_current_attack;
		float _defence;
		float _hp;
	public:
		void init();
	
		Player(const std::string &);
		Player(const inet::Character &);
		~Player();
		
		void initAttacks();
		
		void setDepth(float);
		float getDepth();
		
		int getLeft();
		
		// Not really right
		int getRight();
		float getElevation();
		Gaem::User *getUser();
		
		void setUser(Gaem::User *);
		void setDir(int, int);
		void setLeft(int);
		void setElevation(float);
		void setVelocity(float, float);
		void setState(inet::PlayerActionState);
		void setCharacter(const inet::Character &);
		void setCurrentAttack(const std::string &id);
		
		bool isActivePlayer();
		
		void jump();
		void runLeft();
		void runRight();
		void moveUp();
		void moveDown();
		
		void clampPos();
		
		bool isOn(sf::Vector2i pos, float depth);
		
		void handleEvent(const sf::Event &);
		void logic();
		void draw(sf::RenderWindow &);
		
		inet::PlayerState getPlayerStruct();
		virtual int getZIndex();
	};
}

#endif

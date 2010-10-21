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

namespace Entities
{
	class Player: public Gaem::Entity
	{
		sf::Vector2f _position;
		sf::Vector2f _velocity;
		
		Gaem::AnimatedSprite *_sprite;
		
		// Player specifc
		float _scale;
		std::string _name;
		
		int _dir;
		int _pos_left;
		float _pos_depth;
		float _elevation;
	public:
		Player(const std::string &);
		~Player();
		
		void setDepth(float);
		int getLeft();
		int getRight();
		float getDepth();
		
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
	};
}

#endif

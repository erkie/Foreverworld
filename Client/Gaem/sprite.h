/*
 *  sprite.h
 *  Spel
 *
 *  Created by Erik Andersson on 2010-10-12.
 *  Copyright 2010 Åva gymnasium. All rights reserved.
 *
 */

#ifndef __GAEM_SPRITE_H__
#define __GAEM_SPRITE_H__

#include <string>

#include <SFML/Graphics.hpp>

namespace Gaem
{
	class Sprite
	{
	protected:
		sf::Sprite _sprite;
		sf::Vector2f _position;
		bool _is_flipped;
	public:
		Sprite();
		Sprite(const std::string &);
		
		void setPosition(sf::Vector2f);
		void setPosition(float x, float y);
		void setX(float);
		void setY(float);
		sf::Vector2f getPosition();
		sf::FloatRect getRect();
		
		sf::Sprite *getSprite();
		float getX();
		float getY();
		int getWidth();
		int getHeight();
		
		bool isFlipped();
		void flip(bool);
	};
}

#endif

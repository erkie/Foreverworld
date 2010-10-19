/*
 *  sprite.cpp
 *  Spel
 *
 *  Created by Erik Andersson on 2010-10-12.
 *  Copyright 2010 Åva gymnasium. All rights reserved.
 *
 */

#include <SFML/Graphics.hpp>

#include "Gaem/gaem.h"
#include "Gaem/resourcemanager.h"

#include "Gaem/sprite.h"

namespace Gaem
{
	Sprite::Sprite()
	{
		
	}
	
	Sprite::Sprite(const std::string &file)
	{
		_sprite.SetImage(*Gaem::Gaem::getInstance()->getResourceManager()->getImage(file));
	}
	
	void Sprite::setPosition(sf::Vector2f pos)
	{
		_position = pos;
		_sprite.SetPosition(pos);
	}
	
	void Sprite::setPosition(float x, float y)
	{
		_position.x = x;
		_position.y = y;
		
		_sprite.SetPosition(x, y);
	}
	
	void Sprite::setX(float x)
	{
		_position.x = x;
		_sprite.SetX(x);
	}
	
	void Sprite::setY(float y)
	{
		_position.y = y;
		_sprite.SetY(y);
	}
	
	sf::Vector2f Sprite::getPosition()
	{
		return _position;
	}
	
	sf::Sprite *Sprite::getSprite()
	{
		return &_sprite;
	}
	
	float Sprite::getX()
	{
		return _position.x;
	}
	
	float Sprite::getY()
	{
		return _position.y;
	}
	
	int Sprite::getWidth()
	{
		return _sprite.GetSize().x;
	}
	
	int Sprite::getHeight()
	{
		return _sprite.GetSize().y;
	}
}

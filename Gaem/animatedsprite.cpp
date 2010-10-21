/*
 *  animatedsprite.cpp
 *  Spel
 *
 *  Created by Erik Andersson on 2010-10-17.
 *  Copyright 2010 Åva gymnasium. All rights reserved.
 *
 */
 
#include <iostream>

#include "Gaem/animatedsprite.h"

namespace Gaem
{
	AnimatedSprite::AnimatedSprite(): Sprite()
	{
		
	}
	
	AnimatedSprite::~AnimatedSprite()
	{
		for ( animation_map::iterator iter = _animations.begin(); iter != _animations.end(); iter++ )
			delete (*iter).second;
	}
	
	void AnimatedSprite::loadAnimation(const std::string &key, const std::string &path)
	{
		_animations[key] = new Gaem::Animation(path);
	}
	
	void AnimatedSprite::setAnimation(const std::string &key)
	{
		// We don't have to reset everything if it is already active
		if ( _current_animation == _animations[key] ) return;
		
		_current_animation = _animations[key];
		_current_animation->reset();
		_sprite.SetImage(*_current_animation->getImage());
		_sprite.SetSubRect(_current_animation->getRect());
		
		_sprite.SetX(_position.x);
		_sprite.SetY(_position.y);
	}
	
	void AnimatedSprite::step()
	{
		_current_animation->step();
		_sprite.SetSubRect(_current_animation->getRect());
	}
	
	int AnimatedSprite::getWidth()
	{
		return _current_animation->getWidth() * _sprite.GetScale().x;
	}
}

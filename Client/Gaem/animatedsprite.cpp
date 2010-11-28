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
	AnimatedSprite::AnimatedSprite(): Sprite(), _current_animation(NULL)
	{
		
	}
	
	AnimatedSprite::~AnimatedSprite()
	{
		for ( animation_map::iterator iter = _animations.begin(); iter != _animations.end(); iter++ )
			delete (*iter).second;
	}
	
	void AnimatedSprite::loadAnimation(const std::string &key, const std::string &path)
	{
		bool was_new = true;
		if ( _animations[key] )
		{
			delete _animations[key];
			_animations[key] = NULL;
			was_new = false;
		}
		_animations[key] = new Gaem::Animation(path);
		
		// If we updated the current animation, update its pointer
		if ( ! was_new && _current == key )
		{
			setAnimation(key, true);
		}
	}
	
	void AnimatedSprite::setAnimation(const std::string &key, bool force)
	{
		// We don't have to reset everything if it is already active
		if ( _current == key && ! force ) return;
		
		_current_animation = _animations[key];
		_current = key;
		_current_animation->reset();
		_sprite.SetImage(*_current_animation->getImage());
		_sprite.SetSubRect(_current_animation->getRect());
		
		_sprite.SetX(_position.x);
		_sprite.SetY(_position.y);
	}
	
	Animation *AnimatedSprite::getAnimation()
	{
		return _current_animation;
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

/*
 *  animatedsprite.h
 *  Spel
 *
 *  Created by Erik Andersson on 2010-10-17.
 *  Copyright 2010 Åva gymnasium. All rights reserved.
 *
 */

#ifndef __GAEM_ANIMATEDSPRITE_H__
#define __GAEM_ANIMATEDSPRITE_H__

#include <string>
#include <map>

#include "Gaem/sprite.h"
#include "Gaem/animation.h"

namespace Gaem
{
	class AnimatedSprite: public Sprite
	{
	protected:
		typedef std::map<const std::string, Animation*> animation_map;
		
		animation_map _animations;
		Animation *_current_animation;
	public:
		AnimatedSprite();
		~AnimatedSprite();
		
		void loadAnimation(const std::string &key, const std::string &path);
		void setAnimation(const std::string &key);
		
		Animation *getAnimation();
		
		void step();
		
		int getWidth();
	};
}

#endif

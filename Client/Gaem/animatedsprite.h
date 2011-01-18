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
		std::string _current;
	public:
		AnimatedSprite();
		~AnimatedSprite();
		
		void loadAnimation(const std::string &key, const std::string &path, bool throwError = true);
		void setAnimation(const std::string &key, bool force = false);
		Animation *getAnimation();
		Animation *getAnimtion(const std::string &key);
		
		void step();
		
		int getWidth();
	};
}

#endif

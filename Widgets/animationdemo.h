/*
 *  animationdemo.h
 *  Spel
 *
 *  Created by Erik Andersson on 2010-10-25.
 *  Copyright 2010 Åva gymnasium. All rights reserved.
 *
 */

#ifndef __WIDGETS_ANIMATIONDEMO_H__
#define __WIDGETS_ANIMATIONDEMO_H__

#include <guichan/guichan.hpp>

#include "Gaem/animatedsprite.h"

namespace Widgets
{
	class AnimationDemo: public gcn::Widget
	{
		Gaem::AnimatedSprite *_sprite;
	public:
		AnimationDemo();
		~AnimationDemo();
		
		void setAnimation(const std::string &name);
		
		virtual void draw(gcn::Graphics *graphics);
		virtual void logic();
	};
}

#endif

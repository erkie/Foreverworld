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
	class AnimationDemo: public gcn::Widget, public gcn::MouseListener
	{
		Gaem::AnimatedSprite *_sprite;
		float _scale;
		
		int _offset_x, _offset_y;
		int _last_x, _last_y;
	public:
		AnimationDemo();
		~AnimationDemo();
		
		void setAnimation(const std::string &name);
		
		void setScale(float);
		float getScale();
		
		void flip();
		void reset();
		void start();
		void stop();
		void step();
		void rewind();
		
		virtual void mouseDragged(gcn::MouseEvent &mouseEvent);
		virtual void mouseMoved(gcn::MouseEvent &mouseEvent);
		
		virtual void draw(gcn::Graphics *graphics);
		virtual void logic();
	};
}

#endif

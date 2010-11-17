/*
 *  animation.h
 *  Spel
 *
 *  Created by Erik Andersson on 2010-10-17.
 *  Copyright 2010 Åva gymnasium. All rights reserved.
 *
 */

#ifndef __GAEM_ANIMATION_H__
#define __GAEM_ANIMATION_H__

#include <string>
#include <SFML/Graphics.hpp>

namespace Gaem
{
	class Animation
	{
		int _width;
		int _height;
		
		std::string _path;
		
		sf::Image *_image;
		sf::IntRect _rect;
		float _frame_time;
		bool _run_once;
		bool _playing;
		
		float _last_time;
	public:
		Animation(const std::string &path);
		void load();
		
		void step();
		void nextFrame();
		void prevFrame();
		void reset();
		
		sf::Image *getImage();
		sf::IntRect getRect();
		
		void setPlaying(bool);
		bool getPlaying();
		
		int getWidth();
		int getHeight();
		
		int getFrameNum();
	};
}

#endif
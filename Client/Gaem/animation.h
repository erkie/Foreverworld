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
#include <vector>
#include <SFML/Graphics.hpp>

#include "Gaem/config.h"
#include "GetTime.h"

namespace Gaem
{
	typedef std::vector<sf::Image*> image_vector;
	
	class Animation
	{
		int _width;
		int _height;
		int _total_width;
		
		std::string _path;
		
		int _current_image;
		image_vector _images;
		sf::IntRect _rect;
		float _frame_time;
		bool _run_once;
		bool _playing;
		float _duration;
		float _played_time;
		
		float _last_time;
		bool _has_run;
		
		Config _config;
		
		bool isAtEnd();
		void setOffset(int offset_with);
		void gotoLast();
	public:
		Animation(const std::string &path);
		void load();
		
		void step();
		void nextFrame();
		void prevFrame();
		void reset();
		// Reset stuff that should only be reset once per run
		void reallyReset();
		
		void gotoFirst();
		
		sf::Image *getImage();
		sf::IntRect getRect();
		
		void setPlaying(bool);
		bool getPlaying();
		bool isDone();
		void setElapsedTime(RakNet::Time);
		
		int getWidth();
		int getHeight();
		
		int getFrameNum();
		Config &getConfig();
	};
}

#endif

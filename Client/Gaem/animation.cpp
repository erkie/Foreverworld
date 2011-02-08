/*
 *  animation.cpp
 *  Spel
 *
 *  Created by Erik Andersson on 2010-10-17.
 *  Copyright 2010 Åva gymnasium. All rights reserved.
 *
 */

#include <string>
#include <fstream>
#include <iostream>

#include <SFML/Graphics.hpp>

#include "Gaem/gaem.h"
#include "Gaem/config.h"
#include "Gaem/resourcemanager.h"
#include "Gaem/exception.h"

#include "Gaem/animation.h"

#include "Common/messages.h"

namespace Gaem
{
	Animation::Animation(const std::string &path): _path(path), _run_once(false), _playing(true), _last_time(0), _has_run(false)
	{
		load();
	}
	
	void Animation::load()
	{
		std::ifstream file(_path.c_str());
		
		if ( file.good() )
		{
			std::string image_name;
			
			_config = Config(file);

			_frame_time = _config.getFloat("frame time", 1.0);
			_width = _config.getFloat("width", 1.0);
			image_name = _config.get("file");
			_run_once = _config.isTrue("run once");
			
			_image = Gaem::Gaem::getInstance()->getResourceManager()->getImage(image_name);
			_image->SetSmooth(true);
			
			_height = _image->GetHeight();
			
			_rect = sf::IntRect(0, 0, _width, _image->GetHeight());
		}
		else
		{
			throw GAEM_NONFATAL_EXCEPTION("Could not load info file: " + _path);
		}
	}
	
	void Animation::step()
	{
		if ( ! _playing )
			return;
		
		float tdelta = Gaem::Gaem::getInstance()->getTDelta();
		if ( tdelta + _last_time > _frame_time )
		{
			nextFrame();
			_last_time = 0;
		}
		else
		{
			_last_time += tdelta;
		}
	}
	
	void Animation::nextFrame()
	{
		// Only move foreward if it's looped
		// or if it hasn't already been run already
		if ( ! _run_once || (_run_once && ! _has_run) )
		{
			_rect.Offset(_width, 0);
		}
		
		// Reset animation if looped
		if ( _rect.Left >= (int)_image->GetWidth() )
		{
			if ( ! _run_once )
				reset();
			// Deactivate animations
			else
			{
				_has_run = true;
				prevFrame();
			}
		}
	}
	
	void Animation::prevFrame()
	{
		_rect.Offset(-_width, 0);
		if ( _rect.Left < 0 )
			_rect = sf::IntRect(_image->GetWidth() - _width, 0, _image->GetWidth(), _image->GetHeight());
	}
	
	void Animation::reset()
	{
		_rect = sf::IntRect(0, 0, _width, _image->GetHeight());
		_has_run = false;
		_last_time = 0;
	}
	
	void Animation::setPlaying(bool to)
	{
		_playing = to;
	}
	
	bool Animation::getPlaying()
	{
		return _playing;
	}
	
	bool Animation::isDone()
	{
		if ( ! _run_once ) return false;
		return _has_run;
	}
	
	void Animation::setElapsedTime(RakNet::Time t)
	{
		std::cout << "Setting: " << t << '\n';
	}
	
	sf::Image *Animation::getImage()
	{
		return _image;
	}
	
	sf::IntRect Animation::getRect()
	{
		return _rect;
	}
	
	int Animation::getWidth()
	{
		return _width;
	}
	
	int Animation::getHeight()
	{
		return _height;
	}
	
	int Animation::getFrameNum()
	{
		return _image->GetWidth() / _width * _rect.Left / _image->GetWidth() + 1;
	}
	
	Config &Animation::getConfig()
	{
		return _config;
	}
}

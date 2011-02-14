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
	Animation::Animation(const std::string &path): _total_width(0), _path(path), _run_once(false), _playing(true), _last_time(0), _has_run(false)
	{
		load();
	}
	
	void Animation::load()
	{
		std::ifstream file(_path.c_str());
		
		if ( file.good() )
		{
			_config = Config(file);

			_frame_time = _config.getFloat("frame time", 1.0);
			_width = _config.getFloat("width", 1.0);
			_run_once = _config.isTrue("run once");
			
			bool do_smooth = ! _config.isTrue("no smooth");
			
			if ( _config.isVector("file") )
			{
				cfg_strings images = _config.getVector("file");
				for ( cfg_strings::iterator iter = images.begin(); iter != images.end(); ++iter )
				{
					if ( *iter == "" ) continue;
					
					std::string name = *iter;
					
					sf::Image *img = Gaem::Gaem::getInstance()->getResourceManager()->getImage(name);
					img->SetSmooth(do_smooth);
					
					_total_width += img->GetWidth();
					
					_images.push_back(img);
				}
			}
			else
			{
				std::string image_name = _config.get("file");
				
				sf::Image *img = Gaem::Gaem::getInstance()->getResourceManager()->getImage(image_name);
				img->SetSmooth(do_smooth);
				
				_total_width += img->GetWidth();
				
				_images.push_back(img);
			}

			_current_image = 0;
			_height = _images[_current_image]->GetHeight();
			
			// Set to first frame
			_rect = sf::IntRect(0, 0, _width, _height);
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
			setOffset(_width);
		}
		
		// Reset animation if looped
		if ( isAtEnd() )
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
		setOffset(-_width);
		if ( _rect.Left < 0 )
			gotoLast();
	}
	
	void Animation::reset()
	{
		_current_image = 0;
		_rect = sf::IntRect(0, 0, _width, _height);
		_has_run = false;
		_last_time = 0;
	}
	
	void Animation::setOffset(int offset_with)
	{
		_rect.Offset(offset_with, 0);
		if ( _rect.Left >= (int)_images[_current_image]->GetWidth() && _current_image < (int)_images.size()-1 )
		{
			_rect = sf::IntRect(0, 0, _width, _height);
			_current_image++;
		}
		else if ( _rect.Left < 0 && _current_image > 0 )
		{
			_current_image--;
			_rect = sf::IntRect(_images[_current_image]->GetWidth() - _width, 0, _images[_current_image]->GetWidth(), _height);
		}
	}
	
	bool Animation::isAtEnd()
	{
		return _current_image == (int)_images.size() - 1 && _rect.Left >= (int)_images[_current_image]->GetWidth();
	}
	
	void Animation::gotoLast()
	{
		_current_image = _images.size()-1;
		_rect = sf::IntRect(_images[_current_image]->GetWidth() - _width, 0, _images[_current_image]->GetWidth(), _height);
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
		return _images[_current_image];
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
		// Get width up to current_image
		int tot_left = 0;
		for ( int i = 0; i < _current_image; i++ )
		{
			tot_left += _images[_current_image]->GetWidth();
		}
		
		// Add current offset
		tot_left += _rect.Left;
		
		return ((float)_total_width / (float)_width) * ((float)tot_left / (float)_total_width) + 1;
	}
	
	Config &Animation::getConfig()
	{
		return _config;
	}
}

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

#include <SFML/Graphics.hpp>

#include "Gaem/gaem.h"
#include "Gaem/config.h"
#include "Gaem/resourcemanager.h"
#include "Gaem/exception.h"

#include "Gaem/animation.h"

namespace Gaem
{
	Animation::Animation(const std::string &path): _path(path), _last_time(0), _run_once(false)
	{
		load();
	}
	
	void Animation::load()
	{
		std::ifstream file(_path.c_str());
		
		if ( file.good() )
		{
			std::string image_name;
			
			Config values(file);

			_frame_time = values.getFloat("frame time", 1.0);
			_width = values.getFloat("width", 1.0);
			image_name = values.get("file");
			_run_once = values.isTrue("run once");
			
			_image = Gaem::Gaem::getInstance()->getResourceManager()->getImage(image_name);
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
		_rect.Offset(_width, 0);
		if ( _rect.Left >= _image->GetWidth() )
			reset();
	}
	
	void Animation::reset()
	{
		_rect = sf::IntRect(0, 0, _width, _image->GetHeight());
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
}

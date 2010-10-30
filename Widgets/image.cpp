/*
 *  image.cpp
 *  Spel
 *
 *  Created by Erik Andersson on 2010-10-28.
 *  Copyright 2010 Åva gymnasium. All rights reserved.
 *
 */

#include <guichan/guichan.hpp>

#include "Widgets/image.h"

namespace Widgets
{
	Image::Image(): _image(NULL), _has_image(false), _i_loaded_the_image(false)
	{
		setWidth(0);
		setHeight(0);
	}
	
	Image::Image(const std::string &name)
	{
		_image = gcn::Image::load(name);
		_has_image = true;
		_i_loaded_the_image = true;
	}
	
	Image::Image(gcn::Image *img)
	{
		_image = img;
		_has_image = true;
		_i_loaded_the_image = false;
	}
	
	void Image::setImage(gcn::Image *img)
	{
		if ( _has_image && _i_loaded_the_image )
		{
			delete _image;
		}
		
		_image = img;
		_has_image = true;
		_i_loaded_the_image = false;
	}
	
	void Image::draw(gcn::Graphics *graphics)
	{
		graphics->drawImage(_image, 0, 0);
	}
}

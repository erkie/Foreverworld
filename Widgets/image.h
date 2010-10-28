/*
 *  image.h
 *  Spel
 *
 *  Created by Erik Andersson on 2010-10-28.
 *  Copyright 2010 Åva gymnasium. All rights reserved.
 *
 */

#ifndef __WIDGETS_IMAGE_H__
#define __WIDGETS_IMAGE_H__

#include <guichan/guichan.hpp>

namespace Widgets
{
	class Image: public gcn::Widget
	{
	protected:
		gcn::Image *_image;
		bool _has_image, _i_loaded_the_image;
	public:
		Image();
		Image(const std::string &name);
		Image(gcn::Image *);
		
		void setImage(gcn::Image*);
		
		void draw(gcn::Graphics*);
	};
}

#endif

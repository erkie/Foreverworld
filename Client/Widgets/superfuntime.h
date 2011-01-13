/*
 *  superfuntime.h
 *  Spel
 *
 *  Created by Erik Andersson on 2011-01-12.
 *  Copyright 2011 Åva gymnasium. All rights reserved.
 *
 */

#ifndef __WIDGETS_SUPERFUNTIME_H__
#define __WIDGETS_SUPERFUNTIME_H__

#include <list>
#include <guichan/guichan.hpp>

namespace Widgets
{
	class SuperFunTime: public gcn::Widget
	{
		struct Runner
		{
			gcn::Image *image;
			float x, y;
			float ax, ay;
		};
		typedef std::list<Runner*> runner_list;
		
		runner_list _images;
	public:
		SuperFunTime();
		~SuperFunTime();
		
		void addImage(gcn::Image *);
		
		void logic();
		void draw(gcn::Graphics*);
	};
}

#endif

/*
 *  superfuntime.cpp
 *  Spel
 *
 *  Created by Erik Andersson on 2011-01-12.
 *  Copyright 2011 Åva gymnasium. All rights reserved.
 *
 */

#include <SFML/System.hpp>
#include <guichan/guichan.hpp>

#include "Gaem/gaem.h"

#include "Widgets/superfuntime.h"

namespace Widgets
{
	SuperFunTime::SuperFunTime()
	{
	}
	
	SuperFunTime::~SuperFunTime()
	{
		for ( runner_list::iterator iter = _images.begin(); iter != _images.end(); iter++ )
			delete *iter;
	}
	
	void SuperFunTime::addImage(gcn::Image *img)
	{
		Runner *run = new Runner;
		run->image = img;
		run->x = sf::Randomizer::Random(0, getWidth() - img->getWidth());
		run->y = sf::Randomizer::Random(0, getHeight() - img->getHeight());
		run->ax = sf::Randomizer::Random(150, 300);
		run->ay = sf::Randomizer::Random(150, 300);
		_images.push_back(run);
	}
	
	void SuperFunTime::logic()
	{
		float tdelta = Gaem::Gaem::getInstance()->getTDelta();
		
		for ( runner_list::iterator iter = _images.begin(); iter != _images.end(); iter++ )
		{
			Runner *run = *iter;
			run->x += run->ax * tdelta;
			run->y += run->ay * tdelta;
			
			if ( run->x + run->image->getWidth() > getWidth() )
			{
				run->x = getWidth() - run->image->getWidth();
				run->ax *= -1;
			}
			
			if ( run->y + run->image->getHeight() > getHeight() )
			{
				run->y = getHeight() - run->image->getHeight();
				run->ay *= -1;
			}
			
			if ( run->y < 0 )
			{
				run->y = 0;
				run->ay *= -1;
			}
			
			if ( run->x < 0 )
			{
				run->x = 0;
				run->ax *= -1;
			}
		}
	}
	
	void SuperFunTime::draw(gcn::Graphics *graphics)
	{
		// Draw border?
		gcn::Color faceColor = getBaseColor();
		gcn::Color highlightColor, shadowColor;
		int alpha = getBaseColor().a;
		
		highlightColor = faceColor + 0x303030;
		highlightColor.a = alpha;
		shadowColor = faceColor - 0x303030;
		shadowColor.a = alpha;
		
		graphics->setColor(faceColor);
		graphics->fillRectangle(gcn::Rectangle(1, 1, getDimension().width-1, getHeight() - 1));
		
		graphics->setColor(shadowColor);
		graphics->drawLine(0, 0, getWidth() - 1, 0);
		graphics->drawLine(0, 1, 0, getHeight() - 1);
		
		graphics->setColor(highlightColor);
		graphics->drawLine(getWidth() - 1, 1, getWidth() - 1, getHeight() - 1);
		graphics->drawLine(1, getHeight() - 1, getWidth() - 1, getHeight() - 1);
		
		graphics->setFont(getFont());
		graphics->setColor(gcn::Color(0, 0, 0));
		
		// Draw me
		
		for ( runner_list::iterator iter = _images.begin(); iter != _images.end(); iter++ )
			graphics->drawImage((*iter)->image, (*iter)->x, (*iter)->y);
	}
}

/*
 *  animationdemo.cpp
 *  Spel
 *
 *  Created by Erik Andersson on 2010-10-25.
 *  Copyright 2010 Åva gymnasium. All rights reserved.
 *
 */

#include <iostream>

#include <SFML/Graphics.hpp>

#include <guichan/guichan.hpp>
#include <guichan/sfml.hpp>

#include "Gaem/gaem.h"

#include "Widgets/animationdemo.h"

namespace Widgets
{
	AnimationDemo::AnimationDemo(): _sprite(NULL)
	{
		
	}
	
	AnimationDemo::~AnimationDemo()
	{
		if ( _sprite )
			delete _sprite;
	}
	
	void AnimationDemo::setAnimation(const std::string &name)
	{
		if ( _sprite )
			delete _sprite;
		_sprite = new Gaem::AnimatedSprite();
		_sprite->loadAnimation("my", name);
		_sprite->setAnimation("my");
	}
	
	void AnimationDemo::logic()
	{
		if ( _sprite )
		{
			_sprite->step();
			
			_sprite->setX(getWidth() / 2 - _sprite->getWidth() / 2);
			_sprite->setY(getHeight() / 2 - _sprite->getHeight() / 2);
		}
	}
	
	void AnimationDemo::draw(gcn::Graphics *graphics)
	{
		gcn::Color faceColor = getBaseColor();
		gcn::Color highlightColor, shadowColor;
		int alpha = getBaseColor().a;

		highlightColor = faceColor + 0x303030;
		highlightColor.a = alpha;
		shadowColor = faceColor - 0x303030;
		shadowColor.a = alpha;

		graphics->setColor(faceColor);
		graphics->fillRectangle(gcn::Rectangle(1, 1, getDimension().width-1, getHeight() - 1));

		graphics->setColor(highlightColor);
		graphics->drawLine(0, 0, getWidth() - 1, 0);
		graphics->drawLine(0, 1, 0, getHeight() - 1);

		graphics->setColor(shadowColor);
		graphics->drawLine(getWidth() - 1, 1, getWidth() - 1, getHeight() - 1);
		graphics->drawLine(1, getHeight() - 1, getWidth() - 1, getHeight() - 1);
		
		graphics->fillRectangle(gcn::Rectangle(0, 0, getWidth(), getHeight()));
		
		if ( _sprite )
		{
			_sprite->setX(_sprite->getX() + graphics->getCurrentClipArea().xOffset);
			_sprite->setY(_sprite->getY() + graphics->getCurrentClipArea().yOffset);
			Gaem::Gaem::getInstance()->getWindow()->Draw(*_sprite->getSprite());
		}
	}
}

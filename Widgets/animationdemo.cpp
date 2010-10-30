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
	AnimationDemo::AnimationDemo(): _sprite(NULL), _scale(1.0), _offset_x(0), _offset_y(0)
	{
		addMouseListener(this);
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
		_sprite->getSprite()->SetScale(_scale, _scale);
		_sprite->loadAnimation("my", name);
		_sprite->setAnimation("my");
	}
	
	void AnimationDemo::logic()
	{
		if ( _sprite )
		{
			_sprite->step();
			
			_sprite->getSprite()->SetScale(_scale, _scale);
			
			_sprite->setX(getWidth() / 2 - _sprite->getWidth() / 2);
			_sprite->setY(getHeight() / 2 - _sprite->getHeight() / 2);
		}
	}
	
	void AnimationDemo::setScale(float scale)
	{
		_scale = scale;
	}
	
	float AnimationDemo::getScale()
	{
		return _scale;
	}
	
	void AnimationDemo::start()
	{
		if ( _sprite )
			_sprite->getAnimation()->setPlaying(true);
	}
	
	void AnimationDemo::stop()
	{
		if ( _sprite )
			_sprite->getAnimation()->setPlaying(false);
	}
	
	void AnimationDemo::step()
	{
		if ( _sprite )
		{
			_sprite->getAnimation()->nextFrame();
			_sprite->step();
		}
	}
	
	void AnimationDemo::rewind()
	{
		if ( _sprite )
		{
			_sprite->getAnimation()->prevFrame();
			_sprite->step();
		}
	}
	
	void AnimationDemo::reset()
	{
		if ( _sprite )
			_sprite->getAnimation()->reset();
	}
	
	void AnimationDemo::flip()
	{
		if ( _sprite )
			_sprite->flip(!_sprite->isFlipped());
	}
	
	void AnimationDemo::mouseDragged(gcn::MouseEvent &mouseEvent)
	{
		_offset_x = mouseEvent.getX() - _last_x;
		_offset_y = mouseEvent.getY() - _last_y;
	}
	
	void AnimationDemo::mouseMoved(gcn::MouseEvent &mouseEvent)
	{
		_last_x = mouseEvent.getX();
		_last_y = mouseEvent.getY();
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
			_sprite->setX(_sprite->getX() + graphics->getCurrentClipArea().xOffset + _offset_x);
			_sprite->setY(_sprite->getY() + graphics->getCurrentClipArea().yOffset + _offset_y);
			Gaem::Gaem::getInstance()->getWindow()->Draw(*_sprite->getSprite());
		}
	}
}

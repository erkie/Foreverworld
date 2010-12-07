/*
 *  lazysprite.cpp
 *  Spel
 *
 *  Created by Erik Andersson on 2010-12-02.
 *  Copyright 2010 Åva gymnasium. All rights reserved.
 *
 */

#include <iostream>

#include "Gaem/gaem.h"
#include "Gaem/resourcemanager.h"

#include "Gaem/sprite.h"
#include "Gaem/lazysprite.h"

namespace Gaem
{
	LazySprite::LazySprite(): Sprite(), _loaded(false)
	{
		
	}
	
	LazySprite::LazySprite(const std::string &path): _path(path), _loaded(false)
	{
		
	}
	
	
	void LazySprite::setPath(const std::string &path)
	{
		_path = path;
	}
	
	void LazySprite::load()
	{
		if ( _loaded ) return;
		//std::cout << "Loading " << _path << '\n';
		_sprite.SetImage(*Gaem::getInstance()->getResourceManager()->getImage(_path));
		_loaded = true;
	}
	
	void LazySprite::unload()
	{
		_sprite = sf::Sprite();
		Gaem::getInstance()->getResourceManager()->removeImage(_path);
		_loaded = false;
	}
}

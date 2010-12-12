/*
 *  lazysprite.h
 *  Spel
 *
 *  Created by Erik Andersson on 2010-12-02.
 *  Copyright 2010 Åva gymnasium. All rights reserved.
 *
 */

#ifndef __GAEM_LAZYSPRITE__
#define __GAEM_LAZYSPRITE__

#include "Gaem/sprite.h"

namespace Gaem
{
	class LazySprite: public Sprite
	{
		std::string _path;
		bool _loaded;
	public:
		LazySprite();
		LazySprite(const std::string &);
		
		void setPath(const std::string &);
		
		bool isLoaded();
		
		// Load the image so this sprite can be drawn
		void load();
		
		// Discard the image from memory
		void unload();
	};
}

#endif
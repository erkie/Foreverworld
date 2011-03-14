/*
 *  SoundManager.h
 *  Spel
 *
 *  Created by Erik Andersson on 2011-03-14.
 *  Copyright 2011 Åva gymnasium. All rights reserved.
 *
 */

#ifndef __GAEM_SOUNDMANAGER_H__
#define __GAEM_SOUNDMANAGER_H__

#include <SFML/Audio.hpp>

namespace Gaem
{
	class SoundManager
	{
		sf::Music *_soundtrack;
		sf::SoundBuffer *_boost;
	public:
		SoundManager();
		~SoundManager();
		
		void playBoost();
		
		void playMusic();
		void pauseMusic();
	};
}

#endif

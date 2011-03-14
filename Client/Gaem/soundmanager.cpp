/*
 *  SoundManager.cpp
 *  Spel
 *
 *  Created by Erik Andersson on 2011-03-14.
 *  Copyright 2011 Åva gymnasium. All rights reserved.
 *
 */

#include "Gaem/exception.h"
#include "Gaem/soundmanager.h"

namespace Gaem
{
	SoundManager::SoundManager()
	{
		_soundtrack = new sf::Music;
		if ( ! _soundtrack->OpenFromFile("resources/soundtrack.ogg") )
		{
			throw GAEM_NONFATAL_EXCEPTION("Could not load soundtrack");
		}
		
		_boost = new sf::SoundBuffer;
		if ( ! _boost->LoadFromFile("resources/boost.ogg") )
		{
			throw GAEM_NONFATAL_EXCEPTION("Could not load boost sound");
		}
		
		sf::Sound sound;
		sound.SetBuffer(*_boost);
		sound.Play();
	}
	
	SoundManager::~SoundManager()
	{
		delete _soundtrack;
		delete _boost;
	}
	
	void SoundManager::playBoost()
	{
		
	}
	
	void SoundManager::playMusic()
	{
		
	}
	
	void SoundManager::pauseMusic()
	{
		
	}
}

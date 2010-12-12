/*
 *  chat.h
 *  Spel
 *
 *  Created by Erik Andersson on 2010-12-07.
 *  Copyright 2010 Åva gymnasium. All rights reserved.
 *
 */

#ifndef __MENUS_CHAT_H__
#define __MENUS_CHAT_H__

#include "Gaem/menumanager.h"

namespace Menus
{
	class Chat: public Gaem::Menu
	{
	public:
		static Chat *instance;
	
		void init();
		void show();
	};
}

#endif

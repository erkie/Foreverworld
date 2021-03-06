/*
 *  loading.h
 *  Spel
 *
 *  Created by Erik Andersson on 2010-10-03.
 *  Copyright 2010 Åva gymnasium. All rights reserved.
 *
 */

#ifndef __MENUS_LOADING_H__
#define __MENUS_LOADING_H__

#include "Gaem/menumanager.h"

namespace Menus
{
	class Loading: public Gaem::Menu
	{
		gcn::Label *_indicator;
		
		std::string _message;
	public:
		void init();
		void setMessage(const std::string& message);
	};
}

#endif

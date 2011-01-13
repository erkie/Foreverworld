/*
 *  confirm.h
 *  Spel
 *
 *  Created by Erik Andersson on 2011-01-13.
 *  Copyright 2011 Åva gymnasium. All rights reserved.
 *
 */

// Subclass this class for your own callback needs

#ifndef __MENUS_CONFIRM_H__
#define __MENUS_CONFIRM_H__

#include "Gaem/menumanager.h"

namespace Menus
{
	class Confirm: public Gaem::Menu
	{
	protected:
		std::string _message, _oktext, _canceltext;
	public:
		Confirm();
		Confirm(const std::string &message, const std::string &ok, const std::string &cancel);
				
		void init();
		void show();
	};
}

#endif

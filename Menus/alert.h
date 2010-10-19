/*
 *  alert.h
 *  Spel
 *
 *  Created by Erik Andersson on 2010-10-03.
 *  Copyright 2010 Åva gymnasium. All rights reserved.
 *
 */

#ifndef __MENUS_ALERT_H__
#define __MENUS_ALERT_H__

#include <string>
#include <guichan/guichan.hpp>

#include "Widgets/fixedlabel.h"
#include "Gaem/menumanager.h"

namespace Menus
{
	class Alert: public ::Gaem::Menu
	{
		std::string _message;
		
		gcn::Window *_my_root;
		gcn::Button *_okay;
		Widgets::FixedLabel *_label;
	public:
		Alert();
		Alert(const std::string &);
		
		void init();
		void setMessage(const std::string &);
	};
}

#endif

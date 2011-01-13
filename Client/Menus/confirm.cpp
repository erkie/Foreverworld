/*
 *  confirm.cpp
 *  Spel
 *
 *  Created by Erik Andersson on 2011-01-13.
 *  Copyright 2011 Åva gymnasium. All rights reserved.
 *
 */

#include <guichan/guichan.hpp>

#include "Gaem/menumanager.h"
#include "Widgets/fixedlabel.h"

#include "Menus/confirm.h"

namespace Menus
{
	Confirm::Confirm(): _message(""), _oktext("OK"), _canceltext("Cancel")
	{}
	
	Confirm::Confirm(const std::string &message, const std::string &ok, const std::string &cancel):
		_message(message), _oktext(ok), _canceltext(cancel)
	{}
	
	void Confirm::init()
	{
		// The window
		gcn::Window *base = newWidget<gcn::Window>();
		base->setTitleBarHeight(25);
		base->setSize(500, 100);
		base->setCaption("Question");
		
		int field_width = base->getChildrenArea().width - 10*2;
		
		// The message
		Widgets::FixedLabel *label = newNamedWidget<Widgets::FixedLabel>("message");
		label->setCaption(_message);
		label->setWidth(field_width);
		label->adjustHeight();
		label->setPosition(10, 10);
		
		base->add(label);
		
		int y = label->getBottom() + 10;
		
		// The OK-button
		gcn::Button *okbutton = newNamedWidget<gcn::Button>("ok_button");
		okbutton->setCaption(_oktext);
		okbutton->adjustSize();
		okbutton->setY(y);
		
		// The cancelbutton
		gcn::Button *cancelbutton = newNamedWidget<gcn::Button>("cancel_button");
		cancelbutton->setCaption(_canceltext);
		cancelbutton->adjustSize();
		cancelbutton->setY(y);
		
		int total_width = cancelbutton->getWidth() + okbutton->getWidth() + 10;
		int center = field_width / 2 - total_width / 2;
		
		okbutton->setX(center);
		cancelbutton->setX(center + okbutton->getWidth() + 10);
		
		base->add(okbutton);
		base->add(cancelbutton);
		
		base->setHeight(cancelbutton->getBottom() + 10 + 25);
		
		setRoot(base);
		centerRoot();
	}
	
	void Confirm::show()
	{
		centerRoot();
	}
}

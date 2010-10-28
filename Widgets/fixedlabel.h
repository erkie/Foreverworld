/*
 *  fixedlabel.h
 *  Spel
 *
 *  Created by Erik Andersson on 2010-10-06.
 *  Copyright 2010 Åva gymnasium. All rights reserved.
 *
 */

#ifndef __WIDGETS_FIXEDLABEL_H__
#define __WIDGETS_FIXEDLABEL_H__

#include <string>
#include <list>
#include <guichan/guichan.hpp>

#include "My/myutilities.h"

namespace Widgets
{
	class FixedLabel: public gcn::Label
	{
	protected:
		std::list<std::string> _rows;
	public:
		FixedLabel(const std::string &t = "");
		
		void draw(gcn::Graphics *graphics);
		void adjustHeight();
	};
}

#endif

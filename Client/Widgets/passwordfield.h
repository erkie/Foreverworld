/*
 *  password.h
 *  Spel
 *
 *  Created by Erik Andersson on 2010-10-06.
 *  Copyright 2010 Åva gymnasium. All rights reserved.
 *
 */

#ifndef __WIDGETS_PASSWORDFIELD_H__
#define __WIDGETS_PASSWORDFIELD_H__

#include <guichan/guichan.hpp>

namespace Widgets
{
	class PasswordField : public gcn::TextField
	{
    public:
        /**
         * Constructor, initializes the password field with the given string.
         */
        PasswordField(const std::string &text = "");	
			
        /**
         * Draws the password field.
         */
        virtual void draw(gcn::Graphics *graphics);
	};
}

#endif


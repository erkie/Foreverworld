/*
 *  passwordfield.cpp
 *  Spel
 *
 *  Created by Erik Andersson on 2010-10-06.
 *  Copyright 2010 Åva gymnasium. All rights reserved.
 *
 */

#include <guichan/guichan.hpp>
#include "My/myutilities.h"

#include "Widgets/passwordfield.h"

namespace Widgets
{
	PasswordField::PasswordField(const std::string &text):
		gcn::TextField(text)
	{}
	
	void PasswordField::draw(gcn::Graphics *graphics)
	{
		// std::string uses cow, thus cheap copy
		const std::string original = mText;
		mText.assign(mText.length(), '*');
		
		//////////////////////////////////
		// Because calling TextField::draw(graphics) doesn't work
		using namespace gcn;
		
		Color faceColor = getBaseColor();
        Color highlightColor, shadowColor;
        int alpha = getBaseColor().a;
        highlightColor = faceColor + 0x303030;
        highlightColor.a = alpha;
        shadowColor = faceColor - 0x303030;
        shadowColor.a = alpha;
		
		// Draw a border.
        graphics->setColor(shadowColor);
        graphics->drawLine(0, 0, getWidth() - 1, 0);
        graphics->drawLine(0, 1, 0, getHeight() - 2);
        graphics->setColor(highlightColor);
        graphics->drawLine(getWidth() - 1, 1, getWidth() - 1, getHeight() - 1);
        graphics->drawLine(0, getHeight() - 1, getWidth() - 1, getHeight() - 1);
		
		// Push a clip area so the other drawings don't need to worry
		// about the border.
        graphics->pushClipArea(Rectangle(1, 1, getWidth() - 2, getHeight() - 2));
		
        graphics->setColor(getBackgroundColor());
        graphics->fillRectangle(Rectangle(0, 0, getWidth(), getHeight()));
		
        if (isFocused())
        {
            graphics->setColor(getSelectionColor());
            graphics->drawRectangle(Rectangle(0, 0, getWidth() - 2, getHeight() - 2));
            graphics->drawRectangle(Rectangle(1, 1, getWidth() - 4, getHeight() - 4));
        }
		
        if (isFocused())
        {
            drawCaret(graphics, getFont()->getWidth(mText.substr(0, mCaretPosition)) - mXScroll);
        }
		
        graphics->setColor(getForegroundColor());
        graphics->setFont(getFont());
        graphics->drawText(mText, 3 - mXScroll, 1);
		
        graphics->popClipArea();
		
		//////////////////////////////////
		
		mText = original;
	}
}

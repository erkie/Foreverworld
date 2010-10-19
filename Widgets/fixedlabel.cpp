/*
 *  fixedlabel.cpp
 *  Spel
 *
 *  Created by Erik Andersson on 2010-10-06.
 *  Copyright 2010 Åva gymnasium. All rights reserved.
 *
 */

#include <sstream>
#include <string>

#include "Widgets/fixedlabel.h"

namespace Widgets
{
	FixedLabel::FixedLabel(const std::string &text): gcn::Label(text)
	{}
	
	void FixedLabel::draw(gcn::Graphics *graphics)
	{
		graphics->setFont(getFont());
        graphics->setColor(getForegroundColor());
        graphics->drawText(getCaption(), 0, 0, getAlignment());
	}
	
	void FixedLabel::adjustHeight()
	{
		typedef std::vector<std::string> wordlist;
		
		wordlist words = string_split<std::string, char>(mCaption);
		std::stringstream all;
		
		int space_size = getFont()->getWidth(" ");
		int current_width = 0;
		int newlines = 1;
		
		for (wordlist::iterator word = words.begin(); word != words.end(); word++)
		{
			int width = getFont()->getWidth(*word);
			
			if ( current_width + width + space_size > getWidth() )
			{ 
				all << '\n' << *word;
				current_width = width;
				newlines++;
			}
			else
			{
				if ( word != words.begin() )
					all << ' ';
				all << *word;
				current_width += width + space_size;
			}
		}
		
		setCaption(all.str());
		setHeight(getFont()->getHeight() * newlines + newlines); // + newlines = retarded workaround for bug I don't know
	}
}

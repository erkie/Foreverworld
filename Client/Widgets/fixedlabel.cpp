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
		
		int i = 0;
		for ( std::list<std::string>::iterator iter = _rows.begin(); iter != _rows.end(); iter++, i++ )
		{
			graphics->drawText(*iter, 0, i * getFont()->getHeight(), getAlignment());
		}
	}
	
	void FixedLabel::adjustHeight()
	{
		typedef std::vector<std::string> wordlist;
		
		wordlist words = string_split<std::string, char>(mCaption);
		std::stringstream current_row;
		
		int space_size = getFont()->getWidth(" ");
		int current_width = 0;
		int newlines = 1;
		
		_rows.empty();
		
		for (wordlist::iterator word = words.begin(); word != words.end(); word++)
		{
			int width = getFont()->getWidth(*word);
			
			// New row!
			if ( current_width + width + space_size > getWidth() )
			{
				_rows.push_back(current_row.str());
				current_row.str("");
				
				current_row << *word;
				current_width = width;
				newlines++;
			}
			else
			{
				if ( word != words.begin() )
					current_row << ' ';
				current_row << *word;
				current_width += width + space_size;
			}
		}
		
		_rows.push_back(current_row.str());
		
		setHeight(getFont()->getHeight() * newlines + newlines); // + newlines = retarded workaround for bug I don't know
	}
}

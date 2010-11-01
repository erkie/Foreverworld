/*
 *  informationtable.cpp
 *  Spel
 *
 *  Created by Erik Andersson on 2010-11-01.
 *  Copyright 2010 Åva gymnasium. All rights reserved.
 *
 */

#include <algorithm>

#include "informationtable.h"

namespace Widgets
{
	InformationTable::InformationTable(): gcn::Widget(), _padding(5)
	{
		
	}
	
	InformationTable::~InformationTable()
	{
		for ( row_type::iterator iter = _rows.begin(); iter != _rows.end(); iter++ )
			delete [] *iter;
	}
	
	void InformationTable::setRow(const std::string &name, const std::string &value)
	{
		bool is_new = true;
		for ( row_type::iterator iter = _rows.begin(); iter != _rows.end(); iter++ )
		{
			if ( (*iter)[0] == name )
			{
				(*iter)[1] = value;
				is_new = false;
				break;
			}
		}
		
		if ( is_new )
		{
			std::string *row = new std::string[2];
			row[0] = name;
			row[1] = value;
			_rows.push_back(row);
		}
	}
	
	void InformationTable::removeRow(const std::string &name)
	{
		for ( row_type::iterator iter = _rows.begin(); iter != _rows.end(); iter++ )
		{
			if ( (*iter)[0] == name )
			{
				delete [] *iter;
				_rows.erase(iter);
				break;
			}
		}
	}
	
	void InformationTable::adjustHeight()
	{
		setHeight(_rows.size() * getFont()->getHeight() + _rows.size() * _padding * 2);
	}
	
	void InformationTable::setPadding(int padding)
	{
		_padding = padding;
	}
	
	int InformationTable::getPadding() const
	{ 
		return _padding;
	}
	
	void InformationTable::draw(gcn::Graphics *graphics)
	{
		gcn::Color faceColor = getBaseColor();
		gcn::Color highlightColor, shadowColor;
		int alpha = getBaseColor().a;
		
		highlightColor = faceColor + 0x303030;
		highlightColor.a = alpha;
		shadowColor = faceColor - 0x303030;
		shadowColor.a = alpha;
		
		graphics->setColor(faceColor);
		graphics->fillRectangle(gcn::Rectangle(1, 1, getDimension().width-1, getHeight() - 1));
		
		graphics->setColor(shadowColor);
		graphics->drawLine(0, 0, getWidth() - 1, 0);
		graphics->drawLine(0, 1, 0, getHeight() - 1);
		
		graphics->setColor(highlightColor);
		graphics->drawLine(getWidth() - 1, 1, getWidth() - 1, getHeight() - 1);
		graphics->drawLine(1, getHeight() - 1, getWidth() - 1, getHeight() - 1);
		
		graphics->setFont(getFont());
		graphics->setColor(gcn::Color(0, 0, 0));
		
		// Get width of widest row
		int max_width = 0;
		for ( row_type::iterator iter = _rows.begin(); iter != _rows.end(); iter++ )
		{
			max_width = std::max(getFont()->getWidth((*iter)[0]), max_width);
		}
		
		int font_height = getFont()->getHeight();
		
		// Draw columns and rows
		int i = 0;
		for ( row_type::iterator iter = _rows.begin(); iter != _rows.end(); iter++, i++ )
		{
			std::string name = (*iter)[0];
			std::string value = (*iter)[1];
			int y = (i+1) * _padding + i * _padding + i * font_height;
			graphics->drawText(name, _padding, y);
			graphics->drawText(value, max_width + _padding * 3, y);
		}
	}
	
	void InformationTable::logic()
	{
		
	}
}

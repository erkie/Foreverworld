/*
 *  informationtable.h
 *  Spel
 *
 *  Created by Erik Andersson on 2010-11-01.
 *  Copyright 2010 Åva gymnasium. All rights reserved.
 *
 */

#ifndef __WIDGETS_INFORMATIONTABLE_H__
#define __WIDGETS_INFORMATIONTABLE_H__

#include <list>
#include <string>
#include <guichan/guichan.hpp>

namespace Widgets
{
	class InformationTable: public gcn::Widget
	{
		typedef std::list<std::string*> row_type;
		row_type _rows;
		int _padding;
	public:
		InformationTable();
		~InformationTable();
		
		void setRow(const std::string &row, const std::string &value);
		void removeRow(const std::string &row);
		void adjustHeight();
		
		void setPadding(int);
		int getPadding() const;
		
		virtual void draw(gcn::Graphics *);
		virtual void logic();
	};
}

#endif

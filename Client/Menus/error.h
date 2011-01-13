/*
 *  error.h
 *  Spel
 *
 *  Created by Erik Andersson on 2011-01-13.
 *  Copyright 2011 Åva gymnasium. All rights reserved.
 *
 */

#ifndef __MENUS_ERROR_H__
#define __MENUS_ERROR_H__

#include "Menus/confirm.h"

namespace Menus
{
	class Error: public Confirm
	{
	public:
		Error();
		
		void init();
	};
}

#endif
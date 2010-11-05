/*
 *  exception.cpp
 *  Spel
 *
 *  Created by Erik Andersson on 2010-10-04.
 *  Copyright 2010 Åva gymnasium. All rights reserved.
 *
 */

#include "Gaem/exception.h"

namespace Gaem
{
	Exception::Exception(): _message("?"), _function("?"), _filename("?"), _lineno(0)
	{}
	
	Exception::Exception(const std::string &message):
		_message(message), _function("?"), _filename("?"), _lineno(0)
	{}
	
	Exception::Exception(const std::string &message, const std::string &function, const std::string &filename, unsigned int lineno):
		_message(message), _function(function), _filename(filename), _lineno(lineno)
	{}
	
	const std::string &Exception::getMessage() { return _message; }
	const std::string &Exception::getFunction() { return _function; }
	const std::string &Exception::getFilename() { return _filename; }
	unsigned int &Exception::getLine() { return _lineno; }
}

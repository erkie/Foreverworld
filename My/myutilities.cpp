/*
 *  myutilities.cpp
 *  Spel
 *
 *  Created by Erik Andersson on 2010-10-13.
 *  Copyright 2010 Åva gymnasium. All rights reserved.
 *
 */

#include <string>
#include <fstream>

#include "My/myutilities.h"

bool file_exists(const std::string &f)
{
	std::fstream file(f.c_str());
	return file.good();
}

std::string trim(const std::string &str, const std::string &chars)
{
    size_t s = str.find_first_not_of(chars);
    size_t e = str.find_last_not_of(chars);
	
    if(( std::string::npos == s) || ( std::string::npos == e))
        return "";
    else
        return str.substr(s, e-s+1);
}
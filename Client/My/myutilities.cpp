/*
 *  myutilities.cpp
 *  Spel
 *
 *  Created by Erik Andersson on 2010-10-13.
 *  Copyright 2010 Åva gymnasium. All rights reserved.
 *
 */

#include <iostream>
#include <string>
#include <fstream>

#include "My/myutilities.h"
#include "My/mydirent.h"

void str_replace(std::string& value, std::string const& search,std::string const& replace)
{
    std::string::size_type  next;
	
    for(next = value.find(search);        // Try and find the first match
        next != std::string::npos;        // next is npos if nothing was found
        next = value.find(search,next)    // search for the next match starting after
		// the last match that was found.
		)
    {
			// Inside the loop. So we found a match.
        value.replace(next,search.length(),replace);   // Do the replacement.
        next += replace.length();                      // Move to just after the replace
			// This is the point were we start
			// the next search from. 
    }
}

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

bool is_valid_email(const std::string &email)
{
	if ( email == "" )
		return false;
	
	// Find a @
	size_t pos = email.find("@");
	if ( pos == std::string::npos )
	{
		std::cout << "Failed on email.find(\"@\")\n";
		return false;
	}
	
	// Find a . after the @
	if ( email.find(".", pos) == std::string::npos )
	{
		std::cout << "Failed on email.find('.', pos)\n";
		return false;
	}
	
	// No spaces
	if ( email.find(" ") != std::string::npos )
	{
		std::cout << "Failed on email.find(' ')\n";
		return false;
	}
	
	return true;
}

void list_files(const std::string &path, file_list &res)
{
	DIR *dir;
	struct dirent *ent;
	dir = opendir (path.c_str());
	if (dir != NULL)
	{
		// print all the files and directories within directory 
		while ((ent = readdir (dir)) != NULL)
		{
			std::string name = ent->d_name;
			if ( name == "." || name == ".." ) continue;
			res.push_back(std::string(name));
		}
		closedir(dir);
	}
	else
	{
		// could not open directory
		std::cerr << "Could not open directory " << path << '\n';
	}
}

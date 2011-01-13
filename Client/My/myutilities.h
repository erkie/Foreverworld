/*
 *  myutilities.h
 *  Spel
 *
 *  Created by Erik Andersson on 2010-10-06.
 *  Copyright 2010 Åva gymnasium. All rights reserved.
 *
 */

#ifndef __MY_MYUTILITIES_H__
#define __MY_MYUTILITIES_H__

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <list>
#include <algorithm>
#include <iterator>

template<class T, class C>
std::vector<T> string_split(T str)
{
	using namespace std;
	
	basic_istringstream<C> iss(str);
	vector<T> tokens;
	
	copy(istream_iterator<T, C, char_traits<C> >(iss), // I DO NOT GET THE ERROR HERE
		 istream_iterator<T, C, char_traits<C> >(),
		 back_inserter<vector<T> >(tokens));
	return tokens;
}

void str_replace(std::string& value, std::string const& search,std::string const& replace);

bool file_exists(const std::string &f);
std::string trim(const std::string &str, const std::string &chars = " \n\r\t");

bool is_valid_email(const std::string &);

typedef std::vector<std::string> file_list;
void list_files(const std::string &dir, file_list &res);

std::string urlencode(const std::string &c);
std::string char2hex( char dec );

#endif

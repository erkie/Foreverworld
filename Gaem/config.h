/*
 *  config.h
 *  Spel
 *
 *  Created by Erik Andersson on 2010-09-28.
 *  Copyright 2010 Åva gymnasium. All rights reserved.
 *
 */

#ifndef __GAEM_CONFIG_H__
#define __GAEM_CONFIG_H__

#include <string>
#include <map>

namespace Gaem
{
	using std::map;
	using std::string;

	class Config
	{
		map<string, string> _values;
	public:
		typedef map<string, string> cfg_values;

		Config(const std::string &);
		Config(std::istream &);

		void load();
		string get(string key);
		void set(string key, string value);

		float getFloat(string key, float def = 0);
		int getInt(string key, int def = 0);

		// If the value is either 1 or true
		bool isTrue(string);

		// Anything other then 1 or true is interpreted as false
		bool isFalse(string);

		cfg_values parseFile(const std::string &file);
		cfg_values parseFile(std::istream &);
	};
}

#endif

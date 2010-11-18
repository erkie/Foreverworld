/*
 *  config.cpp
 *  Spel
 *
 *  Created by Erik Andersson on 2010-09-28.
 *  Copyright 2010 Åva gymnasium. All rights reserved.
 *
 */

#include <fstream>
#include <sstream>

#include "Gaem/config.h"
#include "Gaem/exception.h"

#include "My/myutilities.h"

namespace Gaem
{
	using std::stringstream;

	Config::Config(const std::string &name)
	{
		if ( name == "master" )
		{
			// --
			// Change to read from file?
			// --
			
			_values["use_vsync"] = "true";
			_values["max_framerate"] = "60";

			_values["window_fullscreen"] = "false";

			// Ignored if using fullscreen
			_values["window_width"] = "700";
			_values["window_height"] = "500";
			
			_values["server_host"] = "glonk.se";
			_values["server_port"] = "6010";
		}
		else
		{
			std::ifstream file(name.c_str());

			if ( file.good() )
			{
				_values = parseFile(file);
			}
			else
			{
				throw GAEM_NONFATAL_EXCEPTION("Could not find config file: (not good) " + name);
			}

		}
	}

	Config::Config(std::istream &file)
	{
		_values = parseFile(file);
	}

	string Config::get(string key)
	{
		return _values[key];
	}

	void Config::set(string key, string value)
	{
		_values[key] = value;
	}

	int Config::getInt(string key, int def)
	{
		int res = 0;
		stringstream ss(get(key));
		ss >> res;
		if (! ss)
			return def;
		return res;
	}

	float Config::getFloat(string key, float def)
	{
		float res = 0.0;
		stringstream ss(get(key));
		ss >> res;
		if (! ss)
			return def;
		return res;
	}

	bool Config::isTrue(string key)
	{
		return _values[key] == "1" || _values[key] == "true" || _values[key] == "yes";
	}

	bool Config::isFalse(string key)
	{
		return !isTrue(key);
	}

	Config::cfg_values Config::parseFile(std::istream &file)
	{
		map<string, string> ret;

		while ( file.good() )
		{
			std::string key, value;
			getline(file, key, ':');
			getline(file, value);

			ret[key] = trim(value);
		}

		return ret;
	}
}

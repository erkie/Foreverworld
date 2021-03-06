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

	Config::Config() {}

	Config::Config(const std::string &name)
	{
		if ( name == "master" )
		{
			// --
			// Change to read from file?
			// --
			
			try {
				Config test("config.txt");
				_values["server_host"] = test.get("server_host");
				_values["server_port"] = test.get("server_port");
			}
			catch (Exception e)
			{
				_values["server_host"] = "glonk.se";
				_values["server_port"] = "6010";
			}
			
			_values["use_vsync"] = "true";
			_values["max_framerate"] = "60";

			_values["window_fullscreen"] = "false";

			// Ignored if using fullscreen
			_values["window_width"] = "1000";
			_values["window_height"] = "700";
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
				throw GAEM_NONFATAL_EXCEPTION("Could not find config file: (not .good()) '" + name + "'");
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
	
	cfg_strings Config::getVector(string key)
	{
		std::vector<std::string> ret(64);
		
		// Check all keys for <key> with [] at end
		for ( cfg_values::iterator iter = _values.begin(); iter != _values.end(); iter++ )
		{
			string k = (*iter).first;
			
			// Check if k is postfixed with "key[" and ends with "]"
			
			if ( k.compare(0, key.length()+1, key + "[") == 0 && k.find_first_of("]", 0) == (int)k.length()-1  )
			{
				std::stringstream ss(k);
				int i;
				
				ss.ignore(100000, '[');
				
				ss >> i;
				ret[i-1] = (*iter).second;
			}
		}
		return ret;
	}
	
	bool Config::isVector(string key)
	{
		// Check all keys for <key> with [] at end
		for ( cfg_values::iterator iter = _values.begin(); iter != _values.end(); iter++ )
		{
			string k = (*iter).first;
			
			// Check if k is postfixed with "key[" and ends with "]"
			if ( k.compare(0, key.length()+1, key + "[") == 0 && k.find_first_of("]", 0) == (int)k.length()-1  )
				return true;
		}
		return false;
	}

	Config::cfg_values Config::parseFile(std::istream &file)
	{
		Config::cfg_values ret;

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

/*
 *  exception.h
 *  Spel
 *
 *  Created by Erik Andersson on 2010-10-04.
 *  Copyright 2010 Åva gymnasium. All rights reserved.
 *
 */

#ifndef __GAEM_EXCEPTION_H__
#define __GAEM_EXCEPTION_H__

#include <string>

#ifndef __FUNCTION__
#define __FUNCTION__ "?"
#endif

#define GAEM_EXCEPTION(mess) ::Gaem::Exception(mess, __FUNCTION__, __FILE__, __LINE__)
#define GAEM_NONFATAL_EXCEPTION(mess) ::Gaem::NonFatalException(mess, __FUNCTION__, __FILE__, __LINE__)

namespace Gaem
{
	class Exception
	{
	protected:
		std::string _message;
		std::string _function;
		std::string _filename;
		unsigned int _lineno;
	public:
		Exception();
		Exception(const std::string &message);
		Exception(const std::string &message, const std::string &function, const std::string &filename, unsigned int lineno);
		
		const std::string &getMessage();
		const std::string &getFunction();
		const std::string &getFilename();
		unsigned int &getLine();
	};
	
	class NonFatalException: public Exception
	{
	public:
		NonFatalException(): Exception() {}
		NonFatalException(const std::string &message): Exception(message) {}
		NonFatalException(const std::string &message, const std::string &function, const std::string &filename, unsigned int lineno)
			: Exception(message, function, filename, lineno) {}
	};
}

#endif

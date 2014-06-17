/*
Mocap File Translators
Copyright (C) 2014 Alastair Macleod
http://mocap.ca/

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef ___PEEL_FILE_TRANSLATOR_H___
#define ___PEEL_FILE_TRANSLATOR_H___

#include <string>
#include <stdarg.h>
#include <stdio.h>

class FileTranslator
{
public:
    FileTranslator() : m_error(false) {}
    ~FileTranslator() {}

	//virtual bool load(const char *file, bool debug) = 0;

	bool          m_error;
	std::string   m_errorMsg;

	//! Set the error message and return false
	bool          error(const char *msg, ...)
	{
		va_list args;
		char message[4096];

		va_start(args, msg);
#ifdef _WIN32
		vsnprintf_s(message, 4096, 4096, msg, args);
#else
		vsprintf(message, msg, args);
#endif
		va_end(args);

		m_errorMsg = message;
		m_error = true;

		return false;
	}

	//! Reset the error flag and clear the string
	void          resetError()
	{
		m_errorMsg = "";
		m_error = false;
	}


};



#endif

/*

Generic Motion Capture File Reader Class

Copyright (c) 2009, Alastair Macleod

All rights reserved.
 
http://www.mocap.ca/
al@mocap.ca

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

   1. Redistributions of source code must retain the above copyright notice,
      this list of conditions and the following disclaimer.
   2. Redistributions in binary form must reproduce the above copyright notice,
      this list of conditions and the following disclaimer in the documentation
	  and/or other materials provided with the distribution.
   3. The name of the author may not be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

#ifndef ___PEEL_FILE_TRANSLATOR_H___
#define ___PEEL_FILE_TRANSLATOR_H___

#include <string>
#include <stdarg.h>
#include <stdio.h>

class FileTranslator
{
public:
	FileTranslator() : m_error(false) {};
	~FileTranslator() {};

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

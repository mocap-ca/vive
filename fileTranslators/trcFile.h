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

#ifndef ___PEEL_TRCFILE_H_
#define ___PEEL_TRCFILE_H_


#include <string>

#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iostream>
#include <locale>
#include <math.h>

#include "fileTranslator.h"

using namespace std;


class TrcFile : public FileTranslator
{
public:
	TrcFile();
	~TrcFile();

	bool m_debug;

	bool load(const char * file, bool debug);
	vector<string> tokenize(const string& str,const string& delimiters);
	virtual bool createMarker(const char *name) = 0;
	virtual bool addKey(size_t i, float time, float x, float y, float z) = 0;
	virtual void setRate(double) = 0;

};
#endif

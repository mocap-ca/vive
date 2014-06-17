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


#ifndef ___PEEL_HTR_FILE___H___
#define ___PEEL_HTR_FILE___H___

#include "fileTranslator.h"
#include <string>
#include <sstream>
#include <vector>
#include "joint.h"

#define LINEBUFLEN 10240

class HtrFile : public FileTranslator
{
public:
	HtrFile(JointList &joints);
	~HtrFile();

	bool load(const char * file, bool debug);

	bool       m_debug;
	size_t     m_frames;
	double     m_rate;
	JointList &m_joints;

private:

	void tokenize(const char *string, std::vector<std::string> & list);


};



#endif

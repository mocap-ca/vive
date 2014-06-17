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

#ifndef PEEL_ASFAMCFILE_H
#define PEEL_ASFAMCFILE_H

#include "fileTranslator.h"
#include "joint.h"
#include "vectorMath.h"

using namespace std;

class AsfAmcFile : public FileTranslator
{
public:
	AsfAmcFile(JointList &joints);

	bool load(const char * asfFile, const char * amcFile, bool debug);

	JointList&    m_joints;


	float  m_mass;
	string m_name;
	double m_lengthScale;
	string m_angle;

	vector<string> m_documentation;
	string m_order;

};

#endif

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



#ifndef ___BVHFILE_H_
#define ___BVHFILE_H_


#include <string>



#include "fileTranslator.h"
#include "joint.h"


using namespace std;


class BvhFile : public FileTranslator
{
public:
	              BvhFile(JointList &joints);
	              ~BvhFile();
	
	void          tokenize(string in, std::vector<string> &out);	

	bool          load(const char * fileName, bool debug);

private:
	Joint *       parse(fstream &fp, Joint *parent);

	//virtual Joint* createJoint(const Joint &) = 0;
	//virtual bool   addKey(size_t joint, size_t chan, double &) = 0;

	bool          m_debug;

public:
	JointList&    m_joints;
	double        m_rate;
	std::vector<JointChannel*> m_channels;


};
#endif

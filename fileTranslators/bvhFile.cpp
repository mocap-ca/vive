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

#include "bvhFile.h"

#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iostream>
#include <locale>
#include <math.h>


#define LINEBUFLEN 10240


using namespace std;


BvhFile::BvhFile(JointList& joints)
:m_joints(joints)
{}

BvhFile::~BvhFile()
{}



bool BvhFile::load(const char * file, bool debug)
{
	fstream         fp;
	stringstream    ss;
	vector<string>  args;
	vector<string>  vals;
	vector<string>  data;
	char            line[LINEBUFLEN];

	resetError();
	m_debug    = debug;


	// Read and parse the file

	fp.open(file, ios_base::in);

	if(!fp.good()) return error("Could not open file");

	string s;
	string readl;
	
	getline(fp, readl);
	if(readl.substr(0,9) != "HIERARCHY") return error("1: Invalid file format");

	if(parse(fp, NULL)==NULL)
	{
		if(m_debug) cout << m_errorMsg << std::endl;
		return false;
	}

	fp >> s;
	if(s != "MOTION") return error("2: Invalid file format");

	fp >> s;
	if(s != "Frames:") return error("2: Invalid file format");

	int frames;
	fp >> frames;

	fp >> s;
	fp >> s;

	double frames_time;
	fp >> frames_time;

	if(frames_time == 0.0) return error("Invalid frame rate");
	
	m_rate = 1.0 / frames_time;
	m_joints.m_max = frames;

	fp.getline(line, LINEBUFLEN);

	for(int i=0; i< frames; i++)
	{
		fp.getline(line, LINEBUFLEN);
		istringstream iss(line);

		for(std::vector<JointChannel*>::iterator ch = m_channels.begin();
			ch != m_channels.end();
			ch++)
		{
			double value;
			iss >> value;
			(*ch)->m_values.push_back(value);
		}
	}

	fp.close();
	return true;
}

#define PERR(cond, msg) if ( cond ) { error(msg); return false; }

void BvhFile::tokenize(string in, std::vector<string> &out)
{
	out.clear();
	istringstream iss(in);
	do
	{
		string s;
		iss >> s;
		if(s.length()==0) return;;
		out.push_back(s);
	}
	while(iss);
}

Joint *BvhFile::parse(fstream &fp, Joint *parent)
{
	bool           isRoot = false;
	Joint   *      current = NULL;
	JointChannel * chan;
	
	std::vector<string> tokens;
	std::string line;
	
	// Each section must start with ROOT, JOINT or END
	
	getline(fp, line);
	if(!fp) { error("Unexpected EOF"); return NULL; }
	
	tokenize(line, tokens);
	if(tokens.size() == 0) 
	{
		error("Invalid format");
		return NULL;
	}
	
	if(tokens[0] == "ROOT")
	{
		current = new Joint(tokens[1]);
		isRoot = true;
	}
	if(tokens[0] == "JOINT")
	{
		current = new Joint(tokens[1]);
	}
	if(tokens[0] == "End")
	{
		current = new Joint("End");	
	}
	
	if(current == NULL)
	{
		error("Invalid Format");
		return NULL;
	}
	
	m_joints.add(current);
	if(parent != NULL) parent->addChild(current);
	
	// Next line must be a {
	
	getline(fp, line);
	tokenize(line, tokens);	
	if(tokens.size()==0 || tokens[0] != "{")
	{
		error("Format error");
		return NULL;
	}
	
	// The following lines may be OFFSET, CHANNELS or another JOINT|END node
	
	for(;;)
	{	
		// get the stream position, so we can unget a joint or end node and pass down
		streampos pos = fp.tellg();
		
		getline(fp, line);
		tokenize(line, tokens);
		if(tokens.size() == 0)
		{
			error("Invalid format");
			return NULL;
		}
		
	
		if(tokens[0] == "OFFSET")
		{
			current->m_trans[0] = atof(tokens[1].c_str());
			current->m_trans[1] = atof(tokens[2].c_str());
			current->m_trans[2] = atof(tokens[3].c_str());
		}
	
		if(tokens[0] == "CHANNELS")
		{
			string rotOrder;
			
			for(size_t i=0; atoi(tokens[1].c_str()); i++)
			{
				if( i == tokens.size() - 2) break;
				string s = tokens[i+2];
				
				if(s == "Xrotation")
				{
					chan = current->addChannel(JointChannel::RX);
					rotOrder.push_back('X');
				}

				if(s == "Yrotation")
				{
					chan = current->addChannel(JointChannel::RY);
					rotOrder.push_back('Y');
				}
				if(s == "Zrotation")
				{
					chan = current->addChannel(JointChannel::RZ);
					rotOrder.push_back('Z');
				}
				if(s == "Xposition")  chan = current->addChannel(JointChannel::TX);
				if(s == "Yposition")  chan = current->addChannel(JointChannel::TY);
				if(s == "Zposition")  chan = current->addChannel(JointChannel::TZ);
				
				if(chan != NULL) m_channels.push_back(chan);
			}
			
			if(rotOrder.size() == 3 || rotOrder.size() == 2)
				current->setRotationOrder(rotOrder);
		}
	
		if(tokens[0] == "JOINT" || tokens[0] == "End")
		{
			fp.seekg(pos);
			parse(fp, current);
		}
		
		if(tokens[0] == "}") break;
	}

	return current;
};


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


#include "htrFile.h"

#include <string>

#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iostream>
#include <locale>
#include <math.h>
#include <stdlib.h>

#define HTR_SCALE_FACTOR 10.



HtrFile::HtrFile(JointList &joints)
: m_debug(false)
, m_frames(0)
, m_rate(0.0)
, m_joints(joints)
{

}

HtrFile::~HtrFile()
{

}


bool HtrFile::load(const char *file, bool debug)
{
	char                     line[LINEBUFLEN];
	std::fstream             fp;
	std::string              s;
	std::vector<std::string> items;

	int         numSegments;
	int         numFrames;
	int         frameRate;
	std::string rotationOrder;
	std::string lengthUnits;
	std::string rotationUnits;
	char        axisGravity;
	char        axisBone;
	float       scale;

	resetError();
	m_debug = debug;


	fp.open(file, std::ios_base::in);
	if(!fp.good()) return error("Could not open file");

	fp.getline(line, LINEBUFLEN);
	tokenize(line, items);


	if(items.size() == 0 || items[0] != "[Header]")
		return error("File does not appear to be a htr file");

	// Header
	while(1)
	{
		items.clear();

		fp.getline(line, LINEBUFLEN);
		if(fp.eof() || fp.bad() || fp.fail())
			return error("Unexpected end of file while parsing header");

		tokenize(line, items);
		if(items.size() == 1 && items[0] == "[SegmentNames&Hierarchy]")
			break;

		if(items.size() != 2) continue;

		if(items[0] == "NumSegments")         numSegments   = atoi(items[1].c_str());
		if(items[0] == "NumFrames")           numFrames     = atoi(items[1].c_str());
		if(items[0] == "DataFrameRate")       frameRate     = atoi(items[1].c_str());
		if(items[0] == "EulerRotationOrder")  rotationOrder = items[1];
		if(items[0] == "CalibrationUnits")    lengthUnits   = items[1];
		if(items[0] == "RotationUnits")       rotationUnits = items[1];
		if(items[0] == "GlobalAxisofGravity") axisGravity   = items[1].at(0);
		if(items[0] == "BoneLengthAxis" )     axisBone      = items[1].at(0);
		if(items[0] == "ScaleFactor")         scale         = atof(items[1].c_str());

		m_frames  = (size_t)numFrames;
		m_rate    = (double)frameRate; 

	}

	JointList joints;

	// Segment names
	while(1)
	{
		items.clear();

		fp.getline(line, LINEBUFLEN);
		if(fp.eof() || fp.bad() || fp.fail())
			return error("Unexpected end of file while parsing segments");

		tokenize(line, items);
		if(items.size() == 1 && items[0] == "[BasePosition]")
			break;

		if(items.size() != 2) continue;

		Joint *j = new Joint(items[0]);
		m_joints.add(j);
		j->setRotationOrder(rotationOrder);

		if(items[1] != "GLOBAL")
		{
			for(std::vector<Joint*>::iterator i = joints.m_joints.begin(); i != joints.m_joints.end(); i++)
			{
				// Test to see if the joint in the list is this ones parent
				// if so, list this as a child.
				if((*i)->m_name == items[1])
				{
					(*i)->m_children.push_back(j);
					break;
				}
			}
		}
	}

	// Base position
	while(1)
	{
		items.clear();

		fp.getline(line, LINEBUFLEN);
		if(fp.eof() || fp.bad() || fp.fail())
			return error("Unexpected end of file while parsing base position");

		tokenize(line, items);
		if(items.size() == 1 && items[0][0] == '[')
			break;

		if(items.size() != 8) continue;

		std::string segment = items[0];
		double tx      = atof(items[1].c_str()) / HTR_SCALE_FACTOR; 
		double ty      = atof(items[2].c_str()) / HTR_SCALE_FACTOR; 
		double tz      = atof(items[3].c_str()) / HTR_SCALE_FACTOR; 
		double rx      = atof(items[4].c_str()); 
		double ry      = atof(items[5].c_str()); 
		double rz      = atof(items[6].c_str()); 
		//double length  = atof(items[7].c_str()) / HTR_SCALE_FACTOR;


		for(std::vector<Joint*>::iterator i = joints.m_joints.begin(); i != joints.m_joints.end(); i++)
		{
			if(segment == (*i)->m_name)
			{
				(*i)->m_trans[0] = tx;
				(*i)->m_trans[1] = ty;
				(*i)->m_trans[2] = tz;
				(*i)->m_ori[0]   = rx;
				(*i)->m_ori[1]   = ry;
				(*i)->m_ori[2]   = rz;
				(*i)->addChannel(JointChannel::TX);
				(*i)->addChannel(JointChannel::TY);
				(*i)->addChannel(JointChannel::TZ);
				(*i)->addChannel(JointChannel::RX);
				(*i)->addChannel(JointChannel::RY);
				(*i)->addChannel(JointChannel::RZ);
				break;
			}
		}
	}

	std::string node;
    Joint *current= NULL;

	size_t count = 0;
	// Animation - line with node name was already got by last loop
	while(1)
	{


		if(items.size() == 1  && items[0].length() > 2)
		{
			if(items[0][0] == '[' && items[0].at(items[0].size()-1) == ']')
			{
				node = items[0].substr(1, items[0].length()-2);
				current = NULL;
				for(size_t i=0; i < joints.m_joints.size(); i++)
				{
                    Joint *j = joints.m_joints[i];
					if(j->m_name == node)
					{
						current = j;
						break;
					}
				}
				count++;
			}
		}
		else
		if(items.size() == 8)
		{
			if(current != NULL)
			{
				//int    frame   = atoi(items[0].c_str());
				double tx      = atof(items[1].c_str()) / HTR_SCALE_FACTOR; 
				double ty      = atof(items[2].c_str()) / HTR_SCALE_FACTOR; 
				double tz      = atof(items[3].c_str()) / HTR_SCALE_FACTOR; 
				double rx      = atof(items[4].c_str()); 
				double ry      = atof(items[5].c_str()); 
				double rz      = atof(items[6].c_str()); 
				//double length  = atof(items[7].c_str()) / HTR_SCALE_FACTOR;
				current->getChannel(0)->m_values.push_back(tx);
				current->getChannel(1)->m_values.push_back(ty);
				current->getChannel(2)->m_values.push_back(tz);
				current->getChannel(3)->m_values.push_back(rx);
				current->getChannel(4)->m_values.push_back(ry);
				current->getChannel(5)->m_values.push_back(rz);
			}
		}

		fp.getline(line, LINEBUFLEN);
		if(fp.eof() || fp.bad() || fp.fail())
			return error("Unexpected end of file while parsing base position");

		items.clear();

		tokenize(line, items);
		if(items.size() == 1 && items[0] == "[EndOfFile]")
			break;

	}



	return true;



}


void HtrFile::tokenize(const char *string, std::vector<std::string> & list)
{
	if(string == NULL) return;

	const char *ptr = string;

	std::string value;

	while(*ptr != '\0')
	{
		if(*ptr == '\t' || *ptr == ' ' || *ptr == '\r')
		{
			// Whitespace
			if(value.length() > 0)
			{
				list.push_back(value);
				value="";
			}
		}
		else
		if(value.size() == 0 && *ptr == '#')
		{
			// ignore rest of line
			return;
		}
		else
		{
			// character  - add to string
			value.push_back(*ptr);
		}
		ptr++;
	}

	if(value.length() > 0)
		list.push_back(value);
}



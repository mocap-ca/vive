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

#include "asfAmcFile.h"

#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iostream>
#include <locale>
#include <math.h>
#include <map>


using namespace std;


AsfAmcFile::AsfAmcFile(JointList &j) : m_joints(j)
{};

bool AsfAmcFile::load(const char * asfFile, const char * amcFile, bool debug)
{
	fstream asf_fp;
	fstream amc_fp;

	m_lengthScale = 1.;

	asf_fp.open(asfFile, ios_base::in);
	if(!asf_fp.good()) return error("Could not load asf file");

	std::string line;
	std::string section;
	std::string argument;
	std::string title;

	string name;
	double length;
	vmath::vec3<double> lastTrans;
	vmath::vec3<double> direction;
	vmath::vec3<double> axis;
	string axisOrder;
	std::vector<JointChannel::eTypes> dofs;

	Joint *rootJoint = NULL;
	lastTrans.x = 0.;
	lastTrans.y = 0.;
	lastTrans.z = 0.;

	map<string, Joint*> jointMap;

	while(asf_fp.good())
	{
		getline(asf_fp, line);
		if(line.length() == 0) continue;
		if(line[0] == '#') continue;
		if(line[0] == ':')
		{
			line.erase(0, 1);
			std::istringstream ss(line);
			ss >> section;
			std::ws(ss);
			getline(ss, argument);

			if(section == "root")
			{
				rootJoint = new Joint("root");
				rootJoint->addChannel(JointChannel::TX);
				rootJoint->addChannel(JointChannel::TY);
				rootJoint->addChannel(JointChannel::TZ);
				rootJoint->addChannel(JointChannel::RX);
				rootJoint->addChannel(JointChannel::RY);
				rootJoint->addChannel(JointChannel::RZ);
			}
			if(section == "bonedata" && rootJoint != NULL)
			{
				m_joints.add(rootJoint);
				jointMap["root"] = rootJoint;
			}
		}
		else
		{
			if(section== "documentation")
			{
				m_documentation.push_back(line);
				continue;
			}


			std::istringstream ss(line);
			std::string argument;
			ss >> argument;

			if(section== "units")
			{
				if(argument == "mass")   ss >> m_mass;
				if(argument == "name")   ss >> m_name;
				if(argument == "length") ss >> m_lengthScale;
				if(argument == "angle")  ss >> m_angle;
			}

			if(section== "root")
			{
				if(argument == "order")
				{
					string order;
					getline(ss, order);
				}

				if(argument == "axis")
				{
					ss >> axisOrder;
					rootJoint->setRotationOrder(axisOrder);
				}

				if(argument == "position" )
				{
					double x, y, z;
					ss >> x;
					ss >> y;
					ss >> z;
					rootJoint->m_trans[0] = x * 2.54 / m_lengthScale ;
					rootJoint->m_trans[1] = y * 2.54 / m_lengthScale;
					rootJoint->m_trans[2] = z * 2.54 / m_lengthScale;
				}

				if(argument == "orientation" )
				{
					ss >> rootJoint->m_ori[0];
					ss >> rootJoint->m_ori[1];
					ss >> rootJoint->m_ori[2];
				}
			}

			if(section== "bonedata")
			{
				if(argument == "begin")
				{
					name = "";
					direction.x = 0.f;
					direction.y = 0.f;
					direction.z = 0.f;
					length = 0.f;
					axis.x = 0.f;
					axis.y = 0.f;
					axis.z = 0.f;
				}

				if(argument == "end")
				{
					Joint *j = new Joint(name);
					j->m_axis[0] = axis.x;
					j->m_axis[1] = axis.y;
					j->m_axis[2] = axis.z;

					

					lastTrans = vmath::normalize(direction) * (length * 2.54 / m_lengthScale);
					j->m_trans[0] = lastTrans.x;
					j->m_trans[1] = lastTrans.y;
					j->m_trans[2] = lastTrans.z;

					// Add channels (based on dofs)
					for(std::vector<JointChannel::eTypes>::iterator i = dofs.begin(); i != dofs.end(); i++)
					{
						j->addChannel(*i);
					}
					dofs.clear();

					// Add joint to list and map (used for parenting later)
					m_joints.add(j);
					jointMap[name] = j;

				}


				if(argument == "name")   ss >> name;
				if(argument == "length") ss >> length;
				if(argument == "direction")
				{
					ss >> direction.x;
					ss >> direction.y;
					ss >> direction.z;
				}

				if(argument == "axis")
				{
					ss >> axis.x;
					ss >> axis.y;
					ss >> axis.z;
					ss >> axisOrder;
				}
				if(argument == "dof")
				{
					while(ss.good())
					{
						string s;
						ss >> s;
						JointChannel::eTypes type = JointChannel::getType(s);
						if(type != JointChannel::INVALID) dofs.push_back(type);
					}
				}
			}

			if(section == "hierarchy")
			{
				if(argument == "begin") continue;
				if(argument == "end") continue;

				std::map<string, Joint *>::iterator parent = jointMap.find(argument);
				if(parent == jointMap.end()) continue;

				string x;
				while(ss.good())
				{
					ss >> x;
					std::map<string, Joint *>::iterator child = jointMap.find(x);
					if(child != jointMap.end()) (*parent).second->addChild( (*child).second );
				}
			}	
		}
	}

	asf_fp.close();

	if(amcFile == NULL) return true;

	amc_fp.open(amcFile, ios_base::in);
	if(!amc_fp.good()) return error("Could not load amc file");

	int frame;
	double value;

	while(amc_fp.good())
	{
		getline(amc_fp, line);
		if(line.length() == 0) continue;
		if(line[0] == '#') continue;
		if(line[0] == ':')
		{
			continue;
		}

		istringstream ss(line);
		if(isdigit(line[0]))
		{
			ss >> frame;
			if(frame > m_joints.m_max) m_joints.m_max = frame;
			continue;
		}

		ss >> title;

		Joint *j = m_joints.find(title);
		if( j==NULL) continue;

		for(std::vector<JointChannel*>::iterator i = j->m_channels.begin();
			i != j->m_channels.end();
			i++)
		{
			ss >> value;
			if( (*i)->m_type == JointChannel::TX || (*i)->m_type == JointChannel::TY || (*i)->m_type == JointChannel::TZ)
			{
				value *= 2.54;
				value /= m_lengthScale;
			}
			(*i)->m_values.push_back(value);
		}				
	}

	amc_fp.close();

	return true;
	
};

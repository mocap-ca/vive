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


#ifndef ___PEEL_SIMPLE_JOINT_H___
#define ___PEEL_SIMPLE_JOINT_H___

#include <string>
#include <vector>

#include "vectorMath.h"

#include <algorithm>
#include <string>

class JointChannel
{
public:
	enum eTypes { INVALID=0, RX, RY, RZ, TX, TY, TZ }  m_type; 

	JointChannel(std::string name)
	{
		m_type = getType(name);
	};

	static
	enum eTypes getType(std::string s)
	{
		std::transform(s.begin(), s.end(), s.begin(), ::tolower);
		if( s == "rx" ) return RX;
		if( s == "ry" ) return RY;
		if( s == "rz" ) return RZ;
		if( s == "tx" ) return TX;
		if( s == "ty" ) return TY;
		if( s == "tz" ) return TZ;
		if( s == "Xrotation") return RX;
		if( s == "Yrotation") return RY;
		if( s == "Zrotation") return RZ;
		if( s == "Xposition") return TX;
		if( s == "Yposition") return TY;
		if( s == "Zposition") return TZ;
		return INVALID;
	}

	JointChannel(enum eTypes t) : m_type(t) {};
	std::vector<double> m_values;
};

class Joint
{
public:

	enum eRotationOrder
	{
		XYZ = 0,
		XZY,
		YXZ,
		YZX,
		ZXY,
		ZYX
	};

	void clear()
	{
		m_trans[0] = 0.0;
		m_trans[1] = 0.0;
		m_trans[2] = 0.0;
		m_ori[0]   = 0.0;
		m_ori[1]   = 0.0;
		m_ori[2]   = 0.0;
		m_axis[0]  = 0.0;
		m_axis[1]  = 0.0;
		m_axis[2]  = 0.0;
		m_rotationOrder = XYZ;


		m_pre = vmath::identity4<double>();
		m_post = vmath::identity4<double>();

	}

	Joint() { clear(); }

	Joint(std::string name) 
	: m_name(name)
	{ clear(); }

	Joint(std::string name, double tx, double ty, double tz)
		: m_name(name)
	{
		clear();
		m_trans[0] = tx;
		m_trans[1] = ty;
		m_trans[2] = tz;
	}

	~Joint()
	{
	}
	
	JointChannel * addChannel(enum JointChannel::eTypes t)
	{
		JointChannel *j = new JointChannel(t);
		m_channels.push_back(j);
		return j;
	}

	bool addKey(size_t chan, double value)
	{
		if(chan >= m_channels.size()) return false;
		m_channels[chan]->m_values.push_back(value);
        return true;
	}

	JointChannel * getChannel(size_t i)
	{
		if(i >= m_channels.size()) return NULL;
		return m_channels[i];
	}

    JointChannel *  getChannel(enum JointChannel::eTypes t)
	{
		for(std::vector<JointChannel*>::iterator i = m_channels.begin(); i != m_channels.end(); i++)
		{
			if((*i)->m_type == t) return *i;
		}
		return NULL;
	}

	double getChannelVal(enum JointChannel::eTypes t, size_t frame)
	{
		JointChannel const * chan = getChannel(t);
		if(chan == NULL || frame >= chan->m_values.size())
		{
			if(t == JointChannel::TX) return m_trans[0];
			if(t == JointChannel::TY) return m_trans[1];
			if(t == JointChannel::TZ) return m_trans[2];
			if(t == JointChannel::RX) return m_ori[0];
			if(t == JointChannel::RY) return m_ori[1];
			if(t == JointChannel::RZ) return m_ori[2];
			return 0.0;
		}
		return chan->m_values[frame];
	}

	void setRotationOrder(std::string order)
	{
		if(order == "xyz" || order == "XYZ" || order == "XY") m_rotationOrder = XYZ;
		if(order == "xzy" || order == "XZY" || order == "XZ") m_rotationOrder = XZY;
		if(order == "yxz" || order == "YXZ" || order == "YX") m_rotationOrder = YXZ;
		if(order == "yzx" || order == "YZX" || order == "YZ") m_rotationOrder = YZX;
		if(order == "zxy" || order == "ZXY" || order == "ZX") m_rotationOrder = ZXY;
		if(order == "zyx" || order == "ZYX" || order == "ZY") m_rotationOrder = ZYX;
	}		

	void getBaseOrientation(vmath::mat4<double> & out)
	{
		getRotationMatrix(out, m_ori[0], m_ori[1], m_ori[2], m_rotationOrder);
	}

	void getAxis(vmath::mat4<double> & out )
	{
		getRotationMatrix(out, m_axis[0], m_axis[1], m_axis[2], m_rotationOrder);
	}



	void getRotationMatrix(vmath::mat4<double> & out, 
		double rx,
		double ry,
		double rz,
		Joint::eRotationOrder order)
	{
		vmath::mat4<double> rotx = vmath::rotation_matrix_x<double>(rx);
		vmath::mat4<double> roty = vmath::rotation_matrix_y<double>(ry);
		vmath::mat4<double> rotz = vmath::rotation_matrix_z<double>(rz);

		switch(order)
		{
	 		case XYZ: out =  rotx * roty * rotz; break;
	 		case XZY: out =  rotx * rotz * roty; break;
	 		case YXZ: out =  roty * rotx * rotz; break;
	 		case YZX: out =  roty * rotz * rotx; break;
	 		case ZXY: out =  rotz * rotx * roty; break;
	 		case ZYX: out =  rotz * roty * rotx; break;
			default:  out =  rotx * roty * rotz; break;
		}
	}


	void getOrientation(size_t i, vmath::mat4<double> & out)
	{

		double ax = getChannelVal(JointChannel::RX, i);
		double ay = getChannelVal(JointChannel::RY, i);
		double az = getChannelVal(JointChannel::RZ, i);

		//vmath::mat4<double> rotx = vmath::rotation_matrix<double>(ax, 1.0, 0.0, 0.0);
		//vmath::mat4<double> roty = vmath::rotation_matrix<double>(ay, 0.0, 1.0, 0.0);
		//vmath::mat4<double> rotz = vmath::rotation_matrix<double>(az, 0.0, 0.0, 1.0);

		vmath::mat4<double> rotx = vmath::rotation_matrix_x<double>(ax);
		vmath::mat4<double> roty = vmath::rotation_matrix_y<double>(ay);
		vmath::mat4<double> rotz = vmath::rotation_matrix_z<double>(az);


		switch(m_rotationOrder)
		{
	 		case XYZ: out =  rotx * roty * rotz; break;
	 		case XZY: out =  rotx * rotz * roty; break;
	 		case YXZ: out =  roty * rotx * rotz; break;
	 		case YZX: out =  roty * rotz * rotx; break;
	 		case ZXY: out =  rotz * rotx * roty; break;
	 		case ZYX: out =  rotz * roty * rotx; break;
			default:  out =  rotx * roty * rotz; break;
		}
	}
	
	void addChild(Joint *j)
	{
		if(j==NULL) return;
		m_children.push_back(j);
	}

	std::vector<JointChannel*>  m_channels;
	std::string           m_name;
	std::vector<Joint*>   m_children;
	double                m_trans[3];
	double                m_ori[3];
	double                m_axis[3];
	enum eRotationOrder   m_rotationOrder;
	vmath::mat4<double>   m_pre;
	vmath::mat4<double>   m_post;
};


class JointList
{
public:
	JointList()
	: m_rate(0.f)
	, m_max(0)
	{};
	
	~JointList() { clear(); }

	void clear()
	{
		// Delete all joints
		for(std::vector<Joint*>::iterator i = m_joints.begin();
			i != m_joints.end();
			i++)
		{
			if( *i != NULL )
			{
				delete *i;
				*i = NULL;
			}
		}

		m_joints.clear();
	};

	// Returns a pointer to the the array copy
	void add( Joint *j)
	{
		m_joints.push_back(j);
	}
    

	Joint* get(size_t i)
	{
		if(i >= m_joints.size())
			return NULL;
		else
			return m_joints[i];
	}

	Joint *find(std::string name)
	{
		for( std::vector<Joint*>::iterator i = m_joints.begin();
			i != m_joints.end();
			i++)
		{
			if ((*i)->m_name == name) return *i;
		}
		return NULL;
	}

	std::vector<Joint*>  m_joints;
	double              m_rate;
    size_t              m_max;
};

#endif

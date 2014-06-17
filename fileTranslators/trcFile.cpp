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


#include "trcFile.h"
#include <stdlib.h>

#define LINEBUFLEN 10240

using namespace std;

TrcFile::TrcFile() {}
TrcFile::~TrcFile() {}

bool TrcFile::load( const char * fileName, bool debug)
{
	fstream fp;
	stringstream ss;
	vector<string> args;
	vector<string> vals;
	vector<string> data;
	char line[LINEBUFLEN];

	string DataRate           = "";
	string CameraRate         = "";
	string NumFrames          = "";
	string NumMarkers         = "";
	string Units              = ""; 
	string OrigDataRate       = "";
	string OrigDataStartFrame = "";
	string OrigNumFrames      = "";
	string type;   
	string typeval; 
	string up;   
	string file;

	float  frame;
	float  time;
	float  timeScale = 1.0f;
	float  unitScale = 1.0f;

	m_debug  = debug;

	// Read and parse the file

	fp.open(fileName, ios_base::in);

	if(!fp.good()) { return error("Could not open file"); }

	fp.getline(line, LINEBUFLEN);
	ss.str(line);

	ss >> type;
	ss >> typeval;
	ss >> up;
	ss >> file;

	// Get Argument line 

	/*
	DataRate	CameraRate	NumFrames	NumMarkers	Units	OrigDataRate	OrigDataStartFrame	OrigNumFrames	
	120			120			336			45			mm		120				1					336	
	*/

	fp.getline(line, LINEBUFLEN);
	args = tokenize(line, "\t");

	fp.getline(line, LINEBUFLEN);
	vals = tokenize(line, "\t");

	if(args.size() != vals.size())
	{
		fp.close();
		return error("Number of Arguments and number of values do not match");
	}

	for(size_t i=0; i<args.size(); i++)
	{
		if(args.at(i) == "DataRate")        DataRate   = vals.at(i);
		else if(args.at(i) == "CameraRate") CameraRate = vals.at(i);
		else if(args.at(i) == "NumFrames")  NumFrames  = vals.at(i);
		else if(args.at(i) == "NumMarkers") NumMarkers = vals.at(i);
		else if(args.at(i) == "Units")      Units = vals.at(i);
		else if(args.at(i) == "OrigDataRate")       OrigDataRate       = vals.at(i);
		else if(args.at(i) == "OrigDataStartFrame") OrigDataStartFrame = vals.at(i);
		else if(args.at(i) == "OrigNumFrames")      OrigNumFrames      = vals.at(i);
		else if(m_debug) cout << "!" << args.at(i) << ":  " << vals.at(i) << endl;
	}

	if(m_debug)
	{
		cout << "Data Rate: "        << DataRate << endl;
		cout << "Camera Rate: "      << CameraRate << endl;
		cout << "Frames: "           << NumFrames << endl;
		cout << "Markers: "          << NumMarkers << endl;
		cout << "Units: "            << Units << endl;
		cout << "Orig Data Rate: "   << OrigDataRate << endl;
		cout << "Orig Start Frame: " << OrigDataStartFrame << endl;
		cout << "Orig Num Frames: "  << OrigNumFrames << endl;
	}


	if(Units == "mm")      unitScale = 1.0f;
	else if(Units == "cm") unitScale = 10.f;
	else if(Units == "m")  unitScale = 1000.f;
	else if(m_debug) cout << "Scale not recognised." << endl;
	

	if(DataRate != "")
	{
		timeScale= (float)atof(DataRate.c_str());
		if(timeScale<0.001f)
		{
			fp.close();
			return error("Could not get data rate");
		}
	}

	double frameRate = atof(CameraRate.c_str());
	setRate(frameRate);

	if(timeScale < 0.000001f)
	{
		 fp.close();
		 return error("Zero time scale, rate is %s", DataRate.c_str());
	}

	if(m_debug) { cout << "Time scale: " << timeScale << endl; }
	 
	// Get nodes and channels (xyz)

	fp.getline(line, LINEBUFLEN);
	if(m_debug) cout << "args: " << line << endl;
	args = tokenize(string(line), "\t ");

	fp.getline(line, LINEBUFLEN);
	if(m_debug) cout << "vals: " << line << endl;
	vals = tokenize(string(line), "\t "); 

	if(m_debug)
	{
		cout << " args: " << args.size() << endl;
		cout << " vals: " << vals.size() << endl;
	}

	size_t nodes = 0;

	for(size_t i=2; i<args.size(); i++)
	{
		if(args.at(i).size()==0) break;

		createMarker(args.at(i).c_str());

		nodes++;

		if(i+2 >=args.size())       break;
		if(args.at(++i).size()!=0)  break;
		if(args.at(++i).size()!=0)	break;
	}

	size_t node_i;

	while(fp.is_open() && !fp.eof() && !fp.bad())
	{
		fp.getline(line, LINEBUFLEN);
		if(fp.eof()) break;

		data = tokenize(string(line), "\t");

		if(data.size() < 2) continue;

		frame  = (float)atof(data.at(0).c_str());
		time   = (float)atof(data.at(1).c_str());

		node_i=0;

		for(size_t i=2; i<data.size()-2; i+=3)
		{		
			if(node_i >= nodes)
			{
				if(m_debug) cout << "Creating unlabelled" << endl;
				createMarker("UnLabelled");
				nodes++;
			}

			float vtx, vty, vtz;

			if(data.at(i).length() == 0 || data.at(i).length() == 0 || data.at(i).length() == 0)
			{
				node_i++;
				continue;
			}

			vtx = (float)atof(data.at(i).c_str())   * unitScale;
			vty = (float)atof(data.at(i+1).c_str()) * unitScale;
			vtz = (float)atof(data.at(i+2).c_str()) * unitScale;

			addKey(node_i, frame / timeScale, vtx, vty, vtz);

			node_i++;
		}
	}

	fp.close();
	return true;
}




vector<string> TrcFile::tokenize(const string& str,const string& delimiters)
{
	vector<string> tokens;
    	    	
	// find first "non-delimiter".
	string::size_type lastPos = 0;
    string::size_type pos = str.find_first_of(delimiters, lastPos);

	while (string::npos != pos || string::npos != lastPos)
	{
    	tokens.push_back(str.substr(lastPos, pos - lastPos));
    	//lastPos = str.find_first_not_of(delimiters, pos);
		if(pos>=str.size())
			break;
		lastPos = pos+1;
    	pos = str.find_first_of(delimiters, lastPos);
	}

	if(tokens.size() > 0 && tokens.at(0).compare("1308")==0)
	{
		return tokens;
	}
	return tokens;
}



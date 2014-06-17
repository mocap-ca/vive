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


#include "c3dFile.h"


c3dFile::c3dFile()
{
	m_analogFrames  = 0;
	m_pointFrames   = 0;
	m_file          = NULL;
	m_debug         = false;
	m_scaleFactor   = 1.0f;
	m_scaleValue    = 1.0f;

	m_timecode.used              = false;
	m_timecode.dropFrame         = false;
	m_timecode.fieldNumbers.clear();
	m_timecode.standard          = 30;
	m_timecode.subFramePerSample = 0;
	m_timecode.offset            = 0;
	m_timecode.timecode[0]       = 0;
	m_timecode.timecode[1]       = 0;
	m_timecode.timecode[2]       = 0;
	m_timecode.timecode[3]       = 0;
}

c3dFile::~c3dFile()
{

}


void c3dFile::covertFloatTo(float f, char* bytes, unsigned char proc)
{
	if(proc==1)
	{
		// Intel
		char* p = (char*)&f;
		bytes[0] = p[0];
		bytes[1] = p[1];
		bytes[2] = p[2];
		bytes[3] = p[3];
	}
	else
	if(proc==2)
	{
		// Dec
		char* p = (char*)&f;
		bytes[0] = p[2];
		bytes[1] = p[3];
		bytes[2] = p[0];
		bytes[3] = p[1];
		if (bytes[0] || bytes[1] || bytes[2] || bytes[3])
			++bytes[1];      // adjust exponent
	}
	else
	if(proc==3)
	{
		// SGI/Mips
		char* p = (char*)&f;
		bytes[0] = p[3];
		bytes[1] = p[2];
		bytes[2] = p[1];
		bytes[3] = p[0];
	}
	else
	{
		bytes[0]=0x00;
		bytes[1]=0x00;
		bytes[2]=0x00;
		bytes[3]=0x00;
	}
}


void c3dFile::conform(unsigned short &byte, unsigned char proc)
{
	if(proc==3)
	{
		short b = byte & 0x00ff;
		byte = byte >> 8;
		byte = byte | b << 8;
	}
}

float c3dFile::convertToFloat(char bytes[4], unsigned char proc)
{
    char p[4];

	if(proc == 1)
	{
		// Intel    - little-endian
	    p[0] = bytes[0];
	    p[1] = bytes[1];
	    p[2] = bytes[2];
	    p[3] = bytes[3];
	}
	else
	if(proc == 2)
	{
		// Dec VAX, PDP-11  - little-endian
	    p[0] = bytes[2];
	    p[1] = bytes[3];
	    p[2] = bytes[0];
	    p[3] = bytes[1];

		if (p[0] || p[1] || p[2] || p[3])
	        --p[3];          // adjust exponent
	}
	else
	if(proc == 3)
	{
		// SGI/MIPS  
	    p[0] = bytes[3];
	    p[1] = bytes[2];
	    p[2] = bytes[1];
	    p[3] = bytes[0];
	}
	else
		return 0.0f;

    return *(float*)p;
}

/*
void c3dFile::ConvertFloatToDec(float f, char* bytes, unsigned char proc)
{
    char* p = (char*)&f;
    bytes[0] = p[2];
    bytes[1] = p[3];
    bytes[2] = p[0];
    bytes[3] = p[1];
    if (bytes[0] || bytes[1] || bytes[2] || bytes[3])
        ++bytes[1];      // adjust exponent
}
*/


bool c3dFile::open(const char* fileName)
{
#ifdef _WIN32
	if(fopen_s(&m_file, fileName , "rb") == EINVAL )  return false;
#else
	m_file = fopen(fileName, "rb");
	if(m_file == NULL) return false;
#endif

	/*********************
	        Header
	*********************/
	size_t read = fread(&m_header, 1, sizeof(m_header), m_file);
	if(read != sizeof(m_header)) { fclose(m_file); return false; }
	if((m_header.key & 0xff00) != 0x5000) { fclose(m_file); return false; }

	return true;

}




//fileParam.fullName().asChar()
bool c3dFile::parse(const char* groupName,  bool useTimecode, bool debug)
{
	stringstream    ss;
	signed char	num_char;
	short		next_offset;
	char            param_header[4];
	char            param_blocks;
	char            param_data_offset;
	signed char     param_group_id;
	char*           param_name;
	bool            last_param = false;
	bool            param_locked;
	long            current_pos;

	m_debug         = debug;
	m_analogFrames  = 0;
	m_pointFrames   = 0;

	if(m_file == NULL) return false;

	/*********************
	   Parameter Section 
	**********************/

	param_data_offset = m_header.key & 0x00ff;
	if(param_data_offset==0) { fclose(m_file); return false; }

	memset(m_groups, 0, 255 * sizeof(char*));

	fseek(m_file, param_data_offset*256, SEEK_SET);
	fread(param_header, 1, 4, m_file);

	param_blocks  = param_header[2];
	m_procType    = param_header[3];

	// now we know the proc type, we can conform the header
	conform(m_header.frames_per_field, m_procType-83);
	conform(m_header.num_markers, m_procType-83);
	conform(m_header.num_channels, m_procType-83);
	conform(m_header.first_field, m_procType-83);
	conform(m_header.last_field, m_procType-83);
	conform(m_header.max_interpolation, m_procType-83);
	conform(m_header.body_data_offset, m_procType-83);

	m_scaleFactor = convertToFloat(m_header.scale_factor, m_procType-83);
	m_frameRate   = convertToFloat(m_header.frame_rate, m_procType-83);

	if(debug)
	{
		cout << "Header:" << endl;
		cout << "Frames Per Field: "   << m_header.frames_per_field << endl;
		cout << "Number Of Channels: " << m_header.num_channels << endl;
		cout << "First Field:        " << m_header.first_field << endl;
		cout << "Last Field:         " << m_header.last_field << endl;
		cout << "Max Interpolation:  " << m_header.max_interpolation << endl;
		cout << "Frame Rate:         " << m_frameRate << endl;
		cout << "Scale Factor:       " << m_scaleFactor << endl;
	}

	setRate(m_frameRate);

	// Parse Parameters

	while(!last_param)
	{
		//  Number of characters in the Parameter name (1 to 127) 
		//  May be set to a negative number to indicate that the parameter is "locked".
		fread(&num_char, sizeof(unsigned char), 1, m_file);
		if(num_char <0)
		{
			num_char = -num_char;
			param_locked = true;
		}
		else
			param_locked = false;

		//  Group ID number (-1 to ñ127 - always negative).
		//  Group ID number (positive) to which the Parameter belongs (+1 to +127)
		fread(&param_group_id, sizeof(unsigned char), 1, m_file);

		// The parameter name, normally upper case numeric or underscore only
		if(num_char > 0 )
		{
			// Group name (ASCII characters ñ upper case A-Z, 0-9 and underscore _ only)
			param_name = (char*)malloc(num_char+1);
			fread(param_name, sizeof(char), num_char, m_file);
			param_name[num_char] = '\0';
		}
		else
			param_name = NULL;

		// Save current position so we can skip forward from here using an offset
		current_pos = ftell(m_file);

		// Get the position of the next group/parameter (which we will add to current_pos)
		fread(&next_offset, sizeof(short), 1, m_file);
		conform((unsigned short&)next_offset, m_procType - 83);

		// Check to see if this is the last one
		if(next_offset <= 0) last_param = true;

		if(last_param && param_name==NULL) break;

		if(param_group_id < 0)
		{
			// This is a group
			m_groups[-param_group_id] = param_name;
			if(m_debug && param_name != NULL) cout << "=== GROUP:" << param_name << " ===" << endl;
		}
		else
		{
			/*****  READ PARAMETER ******/
			readParam(param_group_id, param_name);
		}

		if(param_name && param_group_id > 0) // save the group names, they are saved in m_groups
		{
			free(param_name);
			param_name = NULL;
		}

		if(m_debug)
		{
			// Number of characters in the Group description.
			fread(&num_char, sizeof(unsigned char), 1, m_file);
			char *group_desc=NULL;
			if(num_char > 1)
			{
				group_desc = (char*)malloc(num_char+1);
				fread(group_desc, sizeof(char), num_char, m_file);
				group_desc[num_char] = '\0';
				printf("Desc: %s\n", group_desc);
				free(group_desc);
			}
		}

		// Move forward to next element (from saved position)
		fseek(m_file, current_pos + next_offset, SEEK_SET);

	}

	if(groupName != NULL) createGroup(groupName);

	setSizes(m_header.num_markers, m_header.num_channels);

	for(size_t i=0; i<m_pointLabels.size(); i++)
		createLocator(i, m_pointLabels.at(i), true);

	for(size_t i=m_pointLabels.size(); i < m_header.num_markers;  i++)
		createLocator(i, std::string(""), true);

	for(size_t i=0; i<m_analogLabels.size(); i++)
		createLocator(i, m_analogLabels.at(i), false);

	for(size_t i=m_analogLabels.size(); i < m_header.num_channels;  i++)
		createLocator(i, std::string(""), false);

	float tc_offset = 0.0;
	if(useTimecode && m_timecode.used)
	{
		tc_offset = (float)m_timecode.timecode[0] * 60.f * 60.f;
		tc_offset+= (float)m_timecode.timecode[1] * 60.f;
		tc_offset+= (float)m_timecode.timecode[2];

		// Convert seconds to frames
		tc_offset *= m_timecode.standard;

		// Drop frames
		if(m_timecode.dropFrame) tc_offset = tc_offset * 1000 / 1001;
			
		// Add timecode frames
		tc_offset += m_timecode.timecode[3];

		// Convert back to seconds.
		tc_offset /= m_timecode.standard;

		if(m_debug) cout << "Timecode offset:" << tc_offset << " seconds" << endl;

	}

	/***********************
	     BODY SECTION	
	***********************/


	float valx, valy, valz;
	signed short *sh;

	// Go to the body start
	fseek(m_file, (m_header.body_data_offset - 1)*512, SEEK_SET);

	char *dat_marker = NULL;
	char *dat_analog = NULL;
	char *ptr=NULL;

	size_t size_marker = m_header.num_markers;
	size_t size_analog = m_header.frames_per_field * m_analogFrames;

	if(m_scaleFactor < 0)
	{
		size_marker *= 16;
		size_analog *= 4;
	}
	else
	{
		size_marker *= 8;
		size_analog *= 2;
	}

	if(size_marker > 0) dat_marker = (char*)malloc(size_marker);
	if(size_analog > 0) dat_analog = (char*)malloc(size_analog);

	setRange(1 , m_header.last_field - m_header.first_field);

	if(m_debug) cout << "Scale: " << m_scaleValue << endl;

	// For each frame
	for(int frame_n=0; frame_n < m_header.last_field - m_header.first_field+1 ; frame_n++)
	{
		if(size_marker > 0)
		{
			fread(dat_marker, 1, size_marker, m_file);
			ptr = dat_marker;

			// For each marker
			for(unsigned int mark_n=0; mark_n < m_header.num_markers; mark_n++)
			{
				if(m_scaleFactor < 0)
				{
					// Use scale factor and conversion
					//fread(line, 4, 4, m_file);
					valx = convertToFloat((char*)ptr, m_procType-83) * m_scaleValue;
					valy = convertToFloat((char*)ptr+4, m_procType-83) * m_scaleValue;
					valz = convertToFloat((char*)ptr+8, m_procType-83) * m_scaleValue;
					ptr+=16;
				}
				else
				{
					// Use scale
					//fread(&traj, sizeof(unsigned short), 4, m_file);
					c3dTrajDataT *traj = (c3dTrajDataT*)ptr;
					conform(traj->x, m_procType-83);
					conform(traj->y, m_procType-83);
					conform(traj->z, m_procType-83);
					valx = (short)traj->x * m_scaleFactor * m_scaleValue;
					valy = (short)traj->y * m_scaleFactor * m_scaleValue;
					valz = (short)traj->z * m_scaleFactor * m_scaleValue;
					ptr+= 8;
				}

				// frame_n * tScale
				addKey(mark_n, (((float)frame_n + (float)m_header.first_field ) / m_frameRate) + tc_offset, valx, valy, valz);
			}
		}

		if(size_analog > 0)
		{
			
			fread(dat_analog, 1, size_analog, m_file);

			ptr = dat_analog;


			// Analog Data
			for(float n = 0; n < m_header.frames_per_field; n++)
			{
				float subFrame = (float)n/(float)m_header.frames_per_field;
				float theTime = (float)frame_n + (float)m_header.first_field + subFrame;
				for(int anlg = 0 ; anlg < m_analogFrames; anlg++)
				{
					if(m_scaleFactor<0)
					{
						addAnalogKey(anlg, (((float)frame_n + (float)m_header.first_field + subFrame)  / m_frameRate) + tc_offset, convertToFloat(ptr, m_procType-83));
						ptr+=4;
					}
					else
					{
						sh = (short*)ptr;
						conform((unsigned short&)sh, m_procType-83);
						addAnalogKey(anlg, (theTime  / m_frameRate) + tc_offset, (float)(*sh));
						ptr+=2;
					}
				}

			}
		}

		// Call virtual
		if(!frameDone(frame_n))
			break;

	}

	fclose(m_file);

	for(int i=0; i < 255; i++)
	{
		if(m_groups[i])
		{
			free(m_groups[i]);
			m_groups[i] = 0;
		}
	}

	m_pointLabels.clear();
	m_analogLabels.clear();

	if(dat_marker) free(dat_marker);
	if(dat_analog) free(dat_analog);

	allDone();

	return true;
}



bool c3dFile::readParam(size_t param_group_id, char* param_name)
{
	//char			param_len;
	//unsigned char   data_sizes[7];
	//char*			param_desc;
	//bool            param_locked = false;

	signed   char   param_ele;
	unsigned char   param_dims;
	char*           group_name = m_groups[param_group_id];

	if(group_name == NULL) return false;

	enum groups { POINT, ANALOG, TIMECODE, UNKNOWN } eGroup = UNKNOWN;

	if(strcmp(group_name,"POINT")==0)    eGroup = POINT;
	if(strcmp(group_name,"ANALOG")==0)   eGroup = ANALOG;
	if(strcmp(group_name,"TIMECODE")==0) eGroup = TIMECODE;


	// this is a parameter
	//printf("Parameter %i: %s:%s ", param_group_id, group_namee, param_name);

 	// Length in bytes of each data element
	fread(&param_ele, sizeof(char), 1, m_file);

	// Number of dimensions (0-7) of the parameter ñ 0 if the parameter is scalar.
	fread(&param_dims, sizeof(unsigned char), 1, m_file);

	if(param_ele== -1 || param_ele == 1)
	{
		// Byte Data
		if(param_dims == 0)
		{
			// Single Byte
			char val;
			fread(&val, sizeof(char), 1, m_file);

			if(eGroup == TIMECODE)
			{
				if(strcmp(param_name, "DROP_FRAMES")==0)
					m_timecode.dropFrame = (val == 1);
			}

			if(m_debug) cout << "\t" << param_name << " byte: " << (int)val << endl;
		}
		else
		if(param_dims == 1)
		{
			// Single string
			char *str;
			size_t len;
			fread(&len, sizeof(char), 1, m_file);

			str = (char*)malloc(len+1);
			fread(str, sizeof(char), len, m_file);
			str[len]='\0';

			if(eGroup == TIMECODE)
			{
				if(strcmp(param_name, "STANDARD")==0)
				{
					if(strcmp(str, "NTSC")==0) m_timecode.standard = 30;
					if(strcmp(str, "PAL")==0)  m_timecode.standard = 25;
					if(strcmp(str, "FILM")==0) m_timecode.standard = 24;
				}
			}

			if(eGroup == POINT)
			{
				if(strcmp(param_name, "UNITS")==0)
				{
					if(str[0] != 0 && str[0] == 'm' && (str[1] == 0x20 || str[1] == 0x00))
					{
						m_scaleValue=1000.0f;
					}
				}
			}


			if(m_debug) cout << "\t" << param_name << " string: " << str << endl;

			free(str);

		}
		else
		if(param_dims == 2)
		{
			// Array of strings

			/****** READ LABELS ******/

			if(memcmp(param_name,"LABELS", 6)==0)
			{
				if(eGroup == POINT)
					readLabels(true);
				else if(eGroup == ANALOG)
					readLabels(false);
			}
			else
			{
				unsigned char unit_size;			
				unsigned char num_units;

				fread(&unit_size, sizeof(unsigned char), 1, m_file);
				fread(&num_units, sizeof(unsigned char), 1, m_file);

				if(m_debug)
				{

					char *val = (char*)malloc(unit_size * num_units);
					fread(val, sizeof(char), unit_size * num_units, m_file);

					char s[256];
					cout << "\t" << param_name << " strings: ";
					for(unsigned int i=0; i < num_units; i++)
					{
#ifdef _WIN32
						strncpy_s(s, 255, val+i * unit_size, unit_size);
#else
						memcpy(s, val+i * unit_size, unit_size);
#endif
						s[unit_size]='\0';
						cout << "\t" << s << endl;
					}
					cout << "\n";
					free(val);
				}
				else
				{
					fseek(m_file, unit_size * num_units, SEEK_CUR);
				}
			}

		}
		else
		{
			if(m_debug) cout << "\t" << param_name << " Unsupported dimension: " << (unsigned int)param_dims;
		}
	}
	else
	if(param_ele == 2)
	{
		// Short Ints
		if(param_dims == 0)
		{
			// Single Short Int
			short val;
			fread(&val, sizeof(short), 1, m_file);
			if(m_debug) cout << "\t" << param_name << " int: " << val << endl;
			conform((unsigned short &)val, m_procType - 83);

			if(strcmp(param_name,"USED")==0)
			{
				if(eGroup == ANALOG)   m_analogFrames  = val;
				if(eGroup == POINT)    m_pointFrames   = val;
				if(eGroup == TIMECODE) m_timecode.used = (val == 1);
			}
		}
		else
		if(param_dims == 1)
		{
			// Array[] of Short Ints
			char len;//, *str;
			fread(&len, sizeof(char), 1, m_file);

			short *vals = (short*)malloc(len * 2);
			fread(vals, 2, len, m_file);

			if(m_debug)
			{
				cout << "\t";
				for(size_t xx = 0; xx < len; xx++)
					cout << vals[xx] << " ";
				cout << endl;
			}

		}
		else
		if(param_dims == 2)
		{
			// Array[][] / Matrix of short ints.
			unsigned char unit_size;			
			unsigned char num_units;

			fread(&unit_size, sizeof(unsigned char), 1, m_file);
			fread(&num_units, sizeof(unsigned char), 1, m_file);

			short *val = (short*)malloc(unit_size * num_units * 2); // Times 2 because param_ele== 2 (short ints)
			fread(val, sizeof(short), unit_size * num_units, m_file);
			conform((unsigned short &) *val, m_procType - 83);

			if(eGroup == TIMECODE)
			{
				if(strcmp(param_name, "FIELD_NUMBERS")==0)
				{
					short *ptr = val;
					for(size_t i=0; i<num_units; i++)
						m_timecode.fieldNumbers.push_back(*(ptr++));
				}

				if(strcmp(param_name, "SUBFRAMESPERFRAME")==0)
				{
					if(unit_size==1 && num_units==1)
						m_timecode.subFramePerSample = *val;
				}

				if(strcmp(param_name, "OFFSETS")==0)
				{
					if(unit_size==1 && num_units==1)
						m_timecode.offset = *val;
				}

				if(strcmp(param_name, "TIMECODES")==0)
				{
					if(unit_size==4 && num_units==1)
					{
						short *ptr = val;
						m_timecode.timecode[0] = *(ptr++);
						m_timecode.timecode[1] = *(ptr++);
						m_timecode.timecode[2] = *(ptr++);
						m_timecode.timecode[3] = *(ptr++);
					}
				}
			}


			if(m_debug)
			{
				//char s[256];
				cout << "\t" << param_name << " int[" << (int)num_units << "][" << (int)unit_size << "]:";
				for(int i=0; i < num_units; i++)
				{
					for(int j=0; j < unit_size; j++)
					{
						cout << (short)*(((short*)(val)) + (i + j)) << " ";
					}
					cout << endl;
				}
			}

			free(val);
		}
		else
		{
			if(m_debug) cout << "\t" << param_name << " unsupported element 2, dimension " << (unsigned int)param_dims << endl;
		}
	}
	else
	if(param_ele== 4)
	{
		// Float Data
		if(param_dims == 0)
		{
			// Single Float
			char val[4];
			fread(&val, sizeof(char), 4, m_file);
			float fval = convertToFloat(val, m_procType-83);
			if(m_debug) cout << "\t" << param_name << " float: " << fval << endl;
		}
		else
		if(param_dims == 1)
		{
			// Array of Floats
			char len;
			fread(&len, sizeof(char), 1, m_file);
			if(m_debug)
			{
				char *val;
				val = (char*)malloc(len *4);
				fread(val, 4, len, m_file);
				if(m_debug)
				{
					cout << "\t" << param_name << " floats: ";
					for(int i=0; i<len; i++)
						 cout << *(((float*)val)+i) << " ";
					cout << endl;
				}
				
				free(val);
				fseek(m_file, len*4, SEEK_CUR);
			}
		}
		else
		{
			// unsupported
			if(m_debug) cout << "\t" << param_name << " unsupported element 4, dimension " << (unsigned int)param_dims << endl;
		}
	}
	else
		if(m_debug) cout << "\t" << param_name << " unsupported element " << (int)param_ele << ", dimension " << (unsigned int)param_dims << endl;

	return true;
}


bool c3dFile::readLabels(bool pointData)
{
	unsigned char unit_size;			
	unsigned char num_units;

	fread(&unit_size, sizeof(unsigned char), 1, m_file);
	fread(&num_units, sizeof(unsigned char), 1, m_file);

	char *val = (char*)malloc(unit_size * num_units);
	fread(val, sizeof(char), unit_size * num_units, m_file);

	char s[256];

	for(unsigned int i=0; i < num_units; i++)
	{
#ifdef _WIN32
		strncpy_s(s, 255, val+i * unit_size, unit_size);
#else
		memcpy(s, val + i * unit_size, unit_size);
#endif
		s[unit_size]='\0';
		
		// Chomp
		unsigned int p;
		for(p = unit_size; p > 0 && s[p-1]==0x20; s[--p]='\0') {};

		// Validate
		for(p=0; s[p]!='\0';p++)
		{
			if( !(	(s[p] >= 'a' && s[p] <= 'z') ||
					(s[p] >= 'A' && s[p] <= 'Z') ||
					(s[p] >= '0' && s[p] <= '9') ||
					(s[p] == '_')   ) )
											s[p] = '_';
		}

		if(pointData)
		{
			// Point data
			if(m_debug)	cout << "\tPoint: " << s << endl;
			m_pointLabels.push_back(s);
		}
		else
		{
			// Analog Data
			if(m_debug)	cout << "\tAnalog: " << s << endl;
			m_analogLabels.push_back(s);
		}
	}
	free(val);

	return true;
}

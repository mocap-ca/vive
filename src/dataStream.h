#ifndef DATASTREAM_H
#define DATASTREAM_H

// Include this if using stl classes

#include <string>
#include <vector>
#include <utility>
#include <sstream>
#include <stdint.h>

#define DSTREAM DataStream
#define TSTREAM std::stringstream
#define STRING std::string
#define LIST   std::vector
#define PAIR   std::pair
#define FLOAT  float
#define uint   unsigned int

#include <vector>
#include <string>

// This is a really simple data stream that caches the raw bytes and can be read back
// using an incrementing pointer.  I found stl's stream too difficult to use.
// if someone else wants to implment them I'm totally fine with that. 
class DataStream
{
public:
	DataStream();
	DataStream(const char *, size_t);

	std::string  str();
	DataStream&  readBytes(char*, size_t);
	DataStream&  writeBytes(const char *, size_t);
	bool         good();

	std::vector<char> data;
	int    state;
	size_t ptr;
};


// Overrides for pushing data on to the stream.
// Strings are formatted as the length followed by the string without
// a terminating character.  All other data is pushed in its binary form

DataStream & operator<= (DataStream& stream, std::string const& s);
DataStream & operator<= (DataStream& stream, const char *s);
DataStream & operator<= (DataStream& stream, int16_t const& i);
DataStream & operator<= (DataStream& stream, int32_t const& i);
DataStream & operator<= (DataStream& stream, uint16_t const& i);
DataStream & operator<= (DataStream& stream, uint32_t const& i);
DataStream & operator<= (DataStream& stream, float const& f);

DataStream & operator>= (DataStream& stream, std::string &s);
DataStream & operator>= (DataStream& stream, float &f);
DataStream & operator>= (DataStream& stream, int16_t &i);

bool streamOk(DataStream&);
bool streamOk(TSTREAM&);

DSTREAM& setBad(DataStream&);



#endif
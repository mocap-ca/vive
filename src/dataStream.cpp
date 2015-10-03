#include "dataStream.h"
#include "defines.h"


DataStream::DataStream() : state(0), ptr(0) {}
DataStream::DataStream(const char *inbuf, size_t len)
	: state(0)
	, data(inbuf, inbuf + len)
	, ptr(0)
{}

bool DataStream::good()
{
	return state == 0;
}

DataStream& setBad(DataStream &stream)
{
	stream.state = 1;
	return stream;
}

DataStream& DataStream::readBytes(char* outbuf, size_t readLen)
{
	memcpy(outbuf, &data[ptr], readLen);
	ptr += readLen;
	return *this;
}

DataStream& DataStream::writeBytes(const char *inbuf, size_t len)
{
	data.insert(data.end(), inbuf, inbuf + len);
	return *this;
}

std::string DataStream::str()
{
	return std::string(data.begin(), data.end());
}

DSTREAM & operator<= (DSTREAM& stream, std::string const& s)
{
	int32_t sz = s.size();
	stream.writeBytes((const char*)&sz, 2);
	return stream.writeBytes(s.c_str(), s.size());
}

DSTREAM & operator<= (DSTREAM & stream, const char *s)
{
	return stream.writeBytes(s, strlen(s));
}

DSTREAM & operator<= (DSTREAM & stream, int16_t const& i)
{
	return stream.writeBytes((const char *)&i, 2);
}

DSTREAM & operator<= (DSTREAM & stream, int32_t const& i)
{
	return stream.writeBytes((const char *)&i, 4);
}

DSTREAM & operator<= (DSTREAM & stream, uint32_t const& i)
{
	return stream.writeBytes((const char *)&i, 4);
}

DSTREAM & operator<= (DSTREAM & stream, float const& f)
{
	return stream.writeBytes((const char *)&f, sizeof(f));
}

DSTREAM & operator >= (DSTREAM & stream, std::string &s)
{
	int16_t sz;
	stream >= sz;
	if (!stream.good()) return stream;
	char *buf = (char*)malloc(sz);
	if (buf == NULL)
	{
		stream.state = 1;
		return stream;
	}
	size_t sz_ = sz;
	stream.readBytes(buf, sz_);
	s.assign(buf, sz);
	free(buf);

	return stream;
}

DSTREAM & operator >= (DSTREAM & stream, float &f)
{
	return stream.readBytes((char*)&f, sizeof(float));
}

DSTREAM & operator >= (DSTREAM & stream, int16_t &i)
{
	return stream.readBytes((char*)&i, sizeof(int16_t));
}

bool streamOk(DSTREAM &stream)
{
	return stream.good();
}

bool streamOk(TSTREAM &stream)
{
	return stream.good();
}


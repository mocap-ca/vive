#include "DataStreamQt.h"

#include <QTextStream>
#include <QDataStream>
#include <QString>
#include <QList>


QDataStream & operator<= (QDataStream& stream, QString const& s)
{
	int32_t sz = s.length();
	stream.writeRawData((const char*)&sz, 2);
	QByteArray data = s.toUtf8();
	if (stream.writeRawData(data, data.length()) == -1)
		return setBad(stream);
	return stream;
}
QDataStream & operator<= (QDataStream& stream, const char *s)
{
	return stream << s;
}

QDataStream & operator<= (QDataStream& stream, int16_t const& i)
{
	if (stream.writeRawData((const char*)&i, 2) == -1) return setBad(stream);
	return stream;
}

QDataStream & operator<= (QDataStream& stream, int32_t const& i)
{
	if (stream.writeRawData((const char*)&i, 4) == -1) return setBad(stream);
	return stream;
}

QDataStream & operator<= (QDataStream& stream, uint16_t const& i)
{
	if (stream.writeRawData((const char*)&i, 2) == -1) return setBad(stream);
	return stream;
}

QDataStream & operator<= (QDataStream& stream, uint32_t const& i)
{
	if (stream.writeRawData((const char*)&i, 4) == -1) return setBad(stream);
	return stream;
}

QDataStream & operator<= (QDataStream& stream, float const& f)
{
	if (stream.writeRawData((const char*)&f, sizeof(f)) == -1) return setBad(stream);
	return stream;
}

QDataStream & operator >= (QDataStream& stream, QString &s)
{
	int16_t sz;
	stream >= sz;
	if (!streamOk(stream)) return stream;
	char *buf = (char*)malloc(sz);
	if (buf == NULL) return setBad(stream);

	uint sz_ = sz;
	stream.readRawData(buf, sz_);
	s.fromUtf8(buf, sz);
	free(buf);

	return stream;
}

QDataStream & operator >= (QDataStream& stream, float &f)
{
	char *ptr = (char*)&f;
	uint len = sizeof(float);
	if (stream.readRawData(ptr, len) == -1)
		return setBad(stream);
	return stream;
}

QDataStream & operator >= (QDataStream& stream, int16_t &i)
{
	char *ptr = (char*)&i;
	uint len = sizeof(int16_t);
	if (stream.readRawData(ptr, len) == -1)
		return setBad(stream);
	return stream;
}

bool streamOk(QDataStream &stream)
{
	return stream.status() == QDataStream::Ok;
}

bool streamOk(QTextStream &stream)
{
	return stream.status() == QDataStream::Ok;
}


QDataStream& setBad(QDataStream &stream)
{
	stream.setStatus(QDataStream::ReadCorruptData);
	return stream;
}
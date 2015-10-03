#ifndef DATASTREAM_QT
#define DATASTREAM_QT


#define DSTREAM QDataStream
#define TSTREAM QTextStream
#define STRING  QString
#define LIST    QList
#define FLOAT   float
#define PAIR    QPair



QDataStream & operator<= (QDataStream& stream, std::string const& s);
QDataStream & operator<= (QDataStream& stream, const char *s);
QDataStream & operator<= (QDataStream& stream, int16_t const& i);
QDataStream & operator<= (QDataStream& stream, int32_t const& i);
QDataStream & operator<= (QDataStream& stream, uint16_t const& i);
QDataStream & operator<= (QDataStream& stream, uint32_t const& i);
QDataStream & operator<= (QDataStream& stream, float const& f);

QDataStream & operator>= (QDataStream& stream, std::string &s);
QDataStream & operator>= (QDataStream& stream, float &f);
QDataStream & operator>= (QDataStream& stream, int16_t &i);

#endif
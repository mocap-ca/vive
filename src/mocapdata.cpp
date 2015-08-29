#include "mocapData.h"
#include <iostream>

#ifdef VIVESTL

DataStream::DataStream() : state(0), ptr(0) {}
DataStream::DataStream(const char *inbuf, size_t len)
	: state(0)
	, data(inbuf, inbuf + len)
	, ptr(0)
{}

bool DataStream::good()
{	return state == 0; }

DataStream& setBad(DataStream &stream)
{
	stream.state = 1; 
	return stream;
}

DataStream& DataStream::readBytes(char* outbuf, size_t readLen)
{	
	memcpy(outbuf, &data[ptr], readLen );
	ptr += readLen;
	return *this;
}

DataStream& DataStream::writeBytes(const char *inbuf, size_t len)
{
	data.insert(data.end(), inbuf, inbuf + len);
	return *this;
}

std::string DataStream::str()
{	return std::string(data.begin(), data.end());  }

DSTREAM & operator<= (DSTREAM& stream, std::string const& s)
{
	int32_t sz = s.size();
	stream.writeBytes((const char*)&sz, 2);
	return stream.writeBytes(s.c_str(), s.size());
}

DSTREAM & operator<= (DSTREAM & stream, const char *s) 
{	return stream.writeBytes(s, strlen(s)); }

DSTREAM & operator<= (DSTREAM & stream, int16_t const& i)
{	return stream.writeBytes((const char *)&i, 2);  }

DSTREAM & operator<= (DSTREAM & stream, int32_t const& i) 
{	return stream.writeBytes((const char *)&i, 4);  }

DSTREAM & operator<= (DSTREAM & stream, uint32_t const& i) 
{	return stream.writeBytes((const char *)&i, 4); }

DSTREAM & operator<= (DSTREAM & stream, float const& f) 
{	return stream.writeBytes((const char *)&f, sizeof(f));  }

DSTREAM & operator >= (DSTREAM & stream, std::string &s)
{
	int16_t sz;
	stream >= sz;
	if (!stream.good()) return stream;
	char *buf = (char*) malloc(sz);
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
{	return stream.readBytes((char*)&f, sizeof(float));   }

DSTREAM & operator >= (DSTREAM & stream, int16_t &i)
{	return stream.readBytes((char*)&i, sizeof(int16_t));   }

bool streamOk(DSTREAM &stream)
{	return stream.good();   }

bool streamOk(TSTREAM &stream)
{	return stream.good();   }

#else


QDataStream & operator<= (QDataStream& stream, QString const& s)
{
    int32_t sz = s.length();
    stream.writeRawData((const char*)&sz, 2);
    QByteArray data = s.toUtf8();
    if(stream.writeRawData(data, data.length()) == -1)
        return setBad(stream);
    return stream;
}
QDataStream & operator<= (QDataStream& stream, const char *s) 
{    return stream << s; }

QDataStream & operator<= (QDataStream& stream, int16_t const& i) 
{
    if(stream.writeRawData((const char*)&i, 2) == -1) return setBad(stream);
    return stream;
}

QDataStream & operator<= (QDataStream& stream, int32_t const& i)
{
    if(stream.writeRawData((const char*)&i, 4) == -1) return setBad(stream);
    return stream;
}

QDataStream & operator<= (QDataStream& stream, uint16_t const& i)
{
    if(stream.writeRawData((const char*)&i, 2) == -1) return setBad(stream);
    return stream;
}

QDataStream & operator<= (QDataStream& stream, uint32_t const& i) 
{
    if( stream.writeRawData((const char*)&i, 4) == -1) return setBad(stream);
    return stream;
}

QDataStream & operator<= (QDataStream& stream, float const& f)
{
    if(stream.writeRawData((const char*)&f, sizeof(f)) == -1) return setBad(stream);
    return stream;
}

QDataStream & operator >= (QDataStream& stream, QString &s)
{
    int16_t sz;
    stream >= sz;
    if (!streamOk(stream)) return stream;
    char *buf = (char*) malloc(sz);
    if (buf == NULL) return setBad(stream);

    uint sz_ = sz;
    stream.readRawData(buf, sz_);
    s.fromUtf8(buf, sz);
    free(buf);

    return stream;
}

QDataStream & operator >= (QDataStream& stream, float &f)
{
    char *ptr = (char*) &f;
    uint len = sizeof(float);
    if( stream.readRawData(ptr, len) == -1)
        return setBad(stream);
    return stream;
}

QDataStream & operator >= (QDataStream& stream, int16_t &i)
{
    char *ptr = (char*) &i;
    uint len = sizeof(int16_t);
    if( stream.readRawData(ptr, len) == -1)
        return setBad(stream);
    return stream;
}

bool streamOk(QDataStream &stream)
{    return stream.status() == QDataStream::Ok;   }

bool streamOk(QTextStream &stream)
{    return stream.status() == QDataStream::Ok;   }


QDataStream& setBad(QDataStream &stream)
{
    stream.setStatus( QDataStream::ReadCorruptData );
    return stream;
}

#endif



/**********************
 *     Mocap Item     *
 **********************/

MocapItem::MocapItem(int _type, int _id)
 : id(_id)
 , type(_type)
{}

MocapItem::MocapItem(int _type, STRING _name)
	: id(-1)
	, type(_type)
	, name(_name)
{}

MocapItem::MocapItem(int type)
    : id(-1)
    , type(type)
{}

bool MocapItem::operator==(const MocapItem& other) const
{
    if(other.type != type) return false;

    if(other.id != -1 && id != -1)
        return other.id == id && other.type == type;

    return other.name == name;
}

TSTREAM& operator << ( TSTREAM &stream, MocapItem& item)
{
    item.str(stream);
    return stream;
}

void MocapItem::header(DSTREAM &stream)
{
	stream <= (int16_t)id;
	if (type == MARKER_TYPE) stream <= STRING("MARKER");
	if (type == SEGMENT_TYPE) stream <= STRING("SEGMENT");
    stream <= name;
}

void MocapItem::parseHeader(DSTREAM &stream)
{
	// id and type(string) will have already been pulled
    stream >= name;
}

void MocapItem::frame(DSTREAM &stream)
{
	int16_t _id = id;
	stream <= _id;
}

/**********************
 *      Item List
 **********************/

int ItemList::update(const ItemList &other)
{
    // TODO : remove markers as needed
    int ret= 0;
    for (LIST<MocapItem*>::const_iterator i = other.data.begin(); i != other.data.end(); i++)
    {
        bool found = false;
        for (LIST<MocapItem*>::const_iterator j = data.begin(); j != data.end(); j++ )
        {
            if(*i == *j)
            {
                (*j)->update((*i));
                ret++;
                found = true;
            }
        }
        if(!found)
        {
#ifdef VIVESTL
            data.push_back( (*i)->copy() );
#else
            data.append( (*i)->copy() );
#endif
        }
    }
    return ret;
}

void ItemList::append( MocapItem* item)
{
#ifdef VIVESTL
     data.push_back(item);
#else
     data.append(item);
#endif
}


/***********************
 *     Id List
 **********************/

IDList* IDList::inst = NULL;

IDList* IDList::Instance()
{
    if(inst) return inst;
    inst = new IDList();
    return inst;
}

int IDList::getId(MocapItem *item)
{
    if(item->id != -1) return item->id;

    int maxid = 0;
    for( LIST< PAIR< int, MocapItem* > >::const_iterator i = items.begin(); i != items.end(); i++)
    {
        int        thisId   = (*i).first;
        MocapItem *thisItem = (*i).second;
        if ( *item == *thisItem ) return thisId;
        if( thisId > maxid ) maxid = thisId;
    }

    maxid++;
    item->id = maxid;
#ifdef VIVESTL
	items.push_back(IDItemType(maxid, item));
#else
	items.append( IDItemType(maxid, item) );
#endif
	return maxid;
}

MocapItem* IDList::find(int id)
{
    for( LIST< PAIR< int, MocapItem* > >::const_iterator i = items.begin(); i != items.end(); i++)
    {
        if((*i).first == id) return (*i).second;
    }

    return NULL;
}





bool IDList::parseHeader(DSTREAM &stream)
{
	int16_t len, header0;

	stream >= header0;
	if (!streamOk(stream)) return false;

	if (header0 != 0x01FA) return false;

	stream >= len;

	if (!streamOk(stream))  return false;

	items.clear();

	for (int16_t i = 0; i < len; i++)
	{
		int16_t id;
		STRING type;

		stream >= id;
		stream >= type;

		MocapItem *item = NULL;
		if (type == "MARKER")  item = (MocapItem*) new MocapMarker(id);
		if (type == "SEGMENT") item = (MocapItem*) new MocapSegment(id);
		if (item == NULL) throw STRING("Error");
		item->parseHeader(stream);
		items.push_back(IDItemType((int)id, item));
	}

	return streamOk(stream);

}


// parse a raw frame, updating the item in the list
bool IDList::parseFrame(DSTREAM &stream)
{
    int16_t id;
    stream >= id;

    if(!streamOk(stream)) return false;

    MocapItem *item = find(id);
    if(item == NULL) return false;

    item->parseFrame( stream );

    return streamOk(stream);
}

bool IDList::parseAll(DSTREAM &stream)
{
	int16_t len;
	stream >= len;

	for (int16_t i = 0; i < len; i++)
	{
		if (!parseFrame(stream)) return false;
	}
	return true;
}




// Push all the header data on to the stream
bool IDList::formatHeader(DSTREAM &stream, bool localOnly)
{
	stream <= (int16_t)0x01FA;
	stream <= (int16_t)items.size();

	for (IDListType::iterator i = items.begin(); i != items.end(); i++)
	{
		MocapItem *item = (*i).second;
		if (localOnly && !item->isLocal) continue;
		item->header(stream);
	}
	return streamOk(stream);
}

bool IDList::formatAll(DSTREAM& stream)
{
	stream <= (int16_t)items.size();

	for (IDListType::iterator i = items.begin(); i != items.end(); i++)
	{
		MocapItem *item = (*i).second;
		item->frame(stream);
	}

	return streamOk(stream);

}









// Push all the frame data on to the stream
// NB, this would be used by the TCP protocol.
bool IDList::str(TSTREAM &stream, bool localOnly)
{
	stream << items.size() << "\n";

	for (IDListType::iterator i = items.begin(); i != items.end(); i++)
	{
		if (localOnly && !(*i).second->isLocal) continue;
		stream << *((*i).second);
	}

	return streamOk(stream);
}




/**********************
 *   Mocap Marker
 **********************/

MocapMarker::MocapMarker( STRING name, FLOAT tr[3])
: MocapItem(MARKER_TYPE, name)
{
    for(int i=0; i < 3; i++) translation[i] = tr[i];
    IDList::Instance()->getId(this);
}

MocapMarker::MocapMarker( int id )
: MocapItem(MARKER_TYPE, id ){}

MocapMarker::MocapMarker()
    : MocapItem(MARKER_TYPE)
{    for(int i=0; i < 3; i++) translation[i] = 0.f; }

MocapItem* MocapMarker::copy()
{
    MocapMarker *marker = new MocapMarker( name, translation);
    marker->id = id;
    return marker;
}

inline bool operator == (const MocapMarker &a, const MocapMarker &b)
{    return a.name == b.name;   }

void MocapMarker::update(const MocapItem *other)
{
    const MocapMarker *marker = dynamic_cast<const MocapMarker*>(other);
    if(marker == NULL) return;;
    for(int i=0; i < 3; i++) translation[i] = marker->translation[i];
}

void MocapMarker::str(TSTREAM & stream)
{
    stream << translation[0] << "\t";
    stream << translation[1] << "\t";
    stream << translation[2] << "\n";
}

void MocapMarker::header(DSTREAM & stream)
{    MocapItem::header(stream);  }

void MocapMarker::parseHeader(DSTREAM & stream)
{    MocapItem::parseHeader(stream);  }

void MocapMarker::frame(DSTREAM & stream )
{
	MocapItem::frame(stream);  // send the id
    stream <= translation[0];
    stream <= translation[1];
    stream <= translation[2];
}

void MocapMarker::parseFrame(DSTREAM & stream)
{
	// the id will have already been pulled
    stream >= translation[0];
    stream >= translation[1];
    stream >= translation[2];
}


/**********************
 *    Mocap Segment
 *********************/

MocapSegment::MocapSegment()
    : MocapItem(SEGMENT_TYPE)
{
    for(int i=0; i < 3; i++) translation[i] = 0.f;
    for(int i=0; i < 4; i++) rotation[i]    = 0.f;
}

MocapSegment::MocapSegment( int id )
: MocapItem(SEGMENT_TYPE, id ){}


MocapSegment::MocapSegment(const STRING name, const FLOAT tr[3], const FLOAT rot[4])
: MocapItem(SEGMENT_TYPE, name)
{
    for(int i=0; i < 3; i++) translation[i] = tr[i];
    for(int i=0; i < 4; i++) rotation[i]    = rot[i];
    IDList::Instance()->getId(this);
}

MocapItem* MocapSegment::copy()
{
    MocapItem *segment = new MocapSegment( name, translation, rotation);
    segment->id = id;
    return segment;
}

void MocapSegment::update(const MocapItem* other)
{
    const MocapSegment *segment = dynamic_cast<const MocapSegment*>(other);
    if(segment == NULL) return;;
    for(int i=0; i < 3; i++) translation[i] = segment->translation[i];
    for(int i=0; i < 4; i++) rotation[i]    = segment->rotation[i];
}

// serialize as string
void MocapSegment::str ( TSTREAM& stream )
{
    stream << name << "\t";
    for(size_t i=0; i < 3; i++) stream << translation[i]    << "\t";
    for(size_t i=0; i < 4; i++)
    {
        stream << rotation[i];
        if(i < 3) stream << "\t";
    }
    stream << "\n";
}

void MocapSegment::header(DSTREAM & stream)
{    MocapItem::header(stream);  }

void MocapSegment::parseHeader(DSTREAM & stream)
{    MocapItem::parseHeader(stream);  }


void MocapSegment::frame( DSTREAM& stream )
{
	MocapItem::frame(stream); // send the id
    for(size_t i=0; i < 3; i++) stream <= translation[i];
    for(size_t i=0; i < 4; i++) stream <= rotation[i];
}

void MocapSegment::parseFrame( DSTREAM& stream )
{
	// the id wil have already been pulled.
    for(size_t i=0; i < 3; i++) stream >= translation[i];
    for(size_t i=0; i < 4; i++) stream >= rotation[i];
}

bool operator == (const MocapSegment &a, const MocapSegment &b)
{    return a.name == b.name;  }





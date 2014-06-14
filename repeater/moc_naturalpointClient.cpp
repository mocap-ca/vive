/****************************************************************************
** Meta object code from reading C++ file 'naturalpointClient.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.2.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "naturalpointClient.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'naturalpointClient.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.2.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_NaturalPointConnector_t {
    QByteArrayData data[8];
    char stringdata[92];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_NaturalPointConnector_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_NaturalPointConnector_t qt_meta_stringdata_NaturalPointConnector = {
    {
QT_MOC_LITERAL(0, 0, 21),
QT_MOC_LITERAL(1, 22, 10),
QT_MOC_LITERAL(2, 33, 0),
QT_MOC_LITERAL(3, 34, 9),
QT_MOC_LITERAL(4, 44, 13),
QT_MOC_LITERAL(5, 58, 12),
QT_MOC_LITERAL(6, 71, 8),
QT_MOC_LITERAL(7, 80, 10)
    },
    "NaturalPointConnector\0connecting\0\0"
    "connected\0disconnecting\0disconnected\0"
    "newFrame\0outMessage\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_NaturalPointConnector[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   44,    2, 0x06,
       3,    0,   45,    2, 0x06,
       4,    0,   46,    2, 0x06,
       5,    0,   47,    2, 0x06,
       6,    1,   48,    2, 0x06,
       7,    1,   51,    2, 0x06,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::UInt,    2,
    QMetaType::Void, QMetaType::QString,    2,

       0        // eod
};

void NaturalPointConnector::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        NaturalPointConnector *_t = static_cast<NaturalPointConnector *>(_o);
        switch (_id) {
        case 0: _t->connecting(); break;
        case 1: _t->connected(); break;
        case 2: _t->disconnecting(); break;
        case 3: _t->disconnected(); break;
        case 4: _t->newFrame((*reinterpret_cast< uint(*)>(_a[1]))); break;
        case 5: _t->outMessage((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (NaturalPointConnector::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&NaturalPointConnector::connecting)) {
                *result = 0;
            }
        }
        {
            typedef void (NaturalPointConnector::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&NaturalPointConnector::connected)) {
                *result = 1;
            }
        }
        {
            typedef void (NaturalPointConnector::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&NaturalPointConnector::disconnecting)) {
                *result = 2;
            }
        }
        {
            typedef void (NaturalPointConnector::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&NaturalPointConnector::disconnected)) {
                *result = 3;
            }
        }
        {
            typedef void (NaturalPointConnector::*_t)(uint );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&NaturalPointConnector::newFrame)) {
                *result = 4;
            }
        }
        {
            typedef void (NaturalPointConnector::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&NaturalPointConnector::outMessage)) {
                *result = 5;
            }
        }
    }
}

const QMetaObject NaturalPointConnector::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_NaturalPointConnector.data,
      qt_meta_data_NaturalPointConnector,  qt_static_metacall, 0, 0}
};


const QMetaObject *NaturalPointConnector::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *NaturalPointConnector::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_NaturalPointConnector.stringdata))
        return static_cast<void*>(const_cast< NaturalPointConnector*>(this));
    return QThread::qt_metacast(_clname);
}

int NaturalPointConnector::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void NaturalPointConnector::connecting()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void NaturalPointConnector::connected()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void NaturalPointConnector::disconnecting()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void NaturalPointConnector::disconnected()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}

// SIGNAL 4
void NaturalPointConnector::newFrame(uint _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void NaturalPointConnector::outMessage(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}
struct qt_meta_stringdata_NaturalPointClient_t {
    QByteArrayData data[3];
    char stringdata[41];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_NaturalPointClient_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_NaturalPointClient_t qt_meta_stringdata_NaturalPointClient = {
    {
QT_MOC_LITERAL(0, 0, 18),
QT_MOC_LITERAL(1, 19, 19),
QT_MOC_LITERAL(2, 39, 0)
    },
    "NaturalPointClient\0naturalPointMessage\0"
    "\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_NaturalPointClient[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   19,    2, 0x0a,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    2,

       0        // eod
};

void NaturalPointClient::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        NaturalPointClient *_t = static_cast<NaturalPointClient *>(_o);
        switch (_id) {
        case 0: _t->naturalPointMessage((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject NaturalPointClient::staticMetaObject = {
    { &BaseClient::staticMetaObject, qt_meta_stringdata_NaturalPointClient.data,
      qt_meta_data_NaturalPointClient,  qt_static_metacall, 0, 0}
};


const QMetaObject *NaturalPointClient::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *NaturalPointClient::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_NaturalPointClient.stringdata))
        return static_cast<void*>(const_cast< NaturalPointClient*>(this));
    return BaseClient::qt_metacast(_clname);
}

int NaturalPointClient::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = BaseClient::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE

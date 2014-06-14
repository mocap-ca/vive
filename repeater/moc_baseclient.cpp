/****************************************************************************
** Meta object code from reading C++ file 'baseclient.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.2.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "baseclient.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'baseclient.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.2.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_BaseClient_t {
    QByteArrayData data[21];
    char stringdata[277];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_BaseClient_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_BaseClient_t qt_meta_stringdata_BaseClient = {
    {
QT_MOC_LITERAL(0, 0, 10),
QT_MOC_LITERAL(1, 11, 15),
QT_MOC_LITERAL(2, 27, 0),
QT_MOC_LITERAL(3, 28, 14),
QT_MOC_LITERAL(4, 43, 18),
QT_MOC_LITERAL(5, 62, 17),
QT_MOC_LITERAL(6, 80, 11),
QT_MOC_LITERAL(7, 92, 11),
QT_MOC_LITERAL(8, 104, 20),
QT_MOC_LITERAL(9, 125, 8),
QT_MOC_LITERAL(10, 134, 1),
QT_MOC_LITERAL(11, 136, 10),
QT_MOC_LITERAL(12, 147, 9),
QT_MOC_LITERAL(13, 157, 9),
QT_MOC_LITERAL(14, 167, 17),
QT_MOC_LITERAL(15, 185, 20),
QT_MOC_LITERAL(16, 206, 16),
QT_MOC_LITERAL(17, 223, 19),
QT_MOC_LITERAL(18, 243, 17),
QT_MOC_LITERAL(19, 261, 4),
QT_MOC_LITERAL(20, 266, 9)
    },
    "BaseClient\0stateConnecting\0\0stateConnected\0"
    "stateDisconnecting\0stateDisconnected\0"
    "outMessage_\0updateFrame\0BaseClient::ClientId\0"
    "newFrame\0i\0mocapStart\0mocapStop\0"
    "mocapWait\0UIConnectingState\0"
    "UIDisconnectingState\0UIConnectedState\0"
    "UIDisconnectedState\0handleButtonClick\0"
    "tick\0isRunning\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_BaseClient[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      17,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   99,    2, 0x06,
       3,    0,  100,    2, 0x06,
       4,    0,  101,    2, 0x06,
       5,    0,  102,    2, 0x06,
       6,    1,  103,    2, 0x06,
       7,    2,  106,    2, 0x06,

 // slots: name, argc, parameters, tag, flags
       9,    1,  111,    2, 0x0a,
      11,    0,  114,    2, 0x0a,
      12,    0,  115,    2, 0x0a,
      13,    0,  116,    2, 0x0a,
      14,    0,  117,    2, 0x0a,
      15,    0,  118,    2, 0x0a,
      16,    0,  119,    2, 0x0a,
      17,    0,  120,    2, 0x0a,
      18,    0,  121,    2, 0x0a,
      19,    0,  122,    2, 0x0a,
      20,    0,  123,    2, 0x0a,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, 0x80000000 | 8, QMetaType::UInt,    2,    2,

 // slots: parameters
    QMetaType::Void, QMetaType::UInt,   10,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Bool,

       0        // eod
};

void BaseClient::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        BaseClient *_t = static_cast<BaseClient *>(_o);
        switch (_id) {
        case 0: _t->stateConnecting(); break;
        case 1: _t->stateConnected(); break;
        case 2: _t->stateDisconnecting(); break;
        case 3: _t->stateDisconnected(); break;
        case 4: _t->outMessage_((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 5: _t->updateFrame((*reinterpret_cast< BaseClient::ClientId(*)>(_a[1])),(*reinterpret_cast< uint(*)>(_a[2]))); break;
        case 6: _t->newFrame((*reinterpret_cast< uint(*)>(_a[1]))); break;
        case 7: _t->mocapStart(); break;
        case 8: _t->mocapStop(); break;
        case 9: _t->mocapWait(); break;
        case 10: _t->UIConnectingState(); break;
        case 11: _t->UIDisconnectingState(); break;
        case 12: _t->UIConnectedState(); break;
        case 13: _t->UIDisconnectedState(); break;
        case 14: _t->handleButtonClick(); break;
        case 15: _t->tick(); break;
        case 16: { bool _r = _t->isRunning();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (BaseClient::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&BaseClient::stateConnecting)) {
                *result = 0;
            }
        }
        {
            typedef void (BaseClient::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&BaseClient::stateConnected)) {
                *result = 1;
            }
        }
        {
            typedef void (BaseClient::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&BaseClient::stateDisconnecting)) {
                *result = 2;
            }
        }
        {
            typedef void (BaseClient::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&BaseClient::stateDisconnected)) {
                *result = 3;
            }
        }
        {
            typedef void (BaseClient::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&BaseClient::outMessage_)) {
                *result = 4;
            }
        }
        {
            typedef void (BaseClient::*_t)(BaseClient::ClientId , uint );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&BaseClient::updateFrame)) {
                *result = 5;
            }
        }
    }
}

const QMetaObject BaseClient::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_BaseClient.data,
      qt_meta_data_BaseClient,  qt_static_metacall, 0, 0}
};


const QMetaObject *BaseClient::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *BaseClient::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_BaseClient.stringdata))
        return static_cast<void*>(const_cast< BaseClient*>(this));
    return QObject::qt_metacast(_clname);
}

int BaseClient::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 17)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 17;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 17)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 17;
    }
    return _id;
}

// SIGNAL 0
void BaseClient::stateConnecting()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void BaseClient::stateConnected()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void BaseClient::stateDisconnecting()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void BaseClient::stateDisconnected()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}

// SIGNAL 4
void BaseClient::outMessage_(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void BaseClient::updateFrame(BaseClient::ClientId _t1, uint _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}
QT_END_MOC_NAMESPACE

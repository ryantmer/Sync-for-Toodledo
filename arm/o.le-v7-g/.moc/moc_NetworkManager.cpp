/****************************************************************************
** Meta object code from reading C++ file 'NetworkManager.hpp'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/NetworkManager.hpp"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'NetworkManager.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_NetworkManager[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: signature, parameters, type, tag, flags
      29,   16,   15,   15, 0x05,
      69,   59,   15,   15, 0x05,
     118,  101,   15,   15, 0x05,

 // slots: signature, parameters, type, tag, flags
     151,  145,   15,   15, 0x0a,
     203,  197,   15,   15, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_NetworkManager[] = {
    "NetworkManager\0\0url,response\0"
    "networkResponse(QUrl,QString)\0url,error\0"
    "networkResponseFailed(QUrl,int)\0"
    "connectionActive\0networkStatusChanged(bool)\0"
    "state\0onNetworkStateChanged(QNetworkSession::State)\0"
    "reply\0onNetworkRequestFinished(QNetworkReply*)\0"
};

void NetworkManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        NetworkManager *_t = static_cast<NetworkManager *>(_o);
        switch (_id) {
        case 0: _t->networkResponse((*reinterpret_cast< QUrl(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 1: _t->networkResponseFailed((*reinterpret_cast< QUrl(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 2: _t->networkStatusChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->onNetworkStateChanged((*reinterpret_cast< QNetworkSession::State(*)>(_a[1]))); break;
        case 4: _t->onNetworkRequestFinished((*reinterpret_cast< QNetworkReply*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData NetworkManager::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject NetworkManager::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_NetworkManager,
      qt_meta_data_NetworkManager, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &NetworkManager::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *NetworkManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *NetworkManager::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_NetworkManager))
        return static_cast<void*>(const_cast< NetworkManager*>(this));
    return QObject::qt_metacast(_clname);
}

int NetworkManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void NetworkManager::networkResponse(QUrl _t1, QString _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void NetworkManager::networkResponseFailed(QUrl _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void NetworkManager::networkStatusChanged(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE

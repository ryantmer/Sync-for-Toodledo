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
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      33,   16,   15,   15, 0x05,
      67,   59,   15,   15, 0x05,

 // slots: signature, parameters, type, tag, flags
      88,   82,   15,   15, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_NetworkManager[] = {
    "NetworkManager\0\0activeConnection\0"
    "networkStateChanged(bool)\0message\0"
    "toast(QString)\0state\0"
    "onNetworkStateChanged(QNetworkSession::State)\0"
};

void NetworkManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        NetworkManager *_t = static_cast<NetworkManager *>(_o);
        switch (_id) {
        case 0: _t->networkStateChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->toast((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->onNetworkStateChanged((*reinterpret_cast< QNetworkSession::State(*)>(_a[1]))); break;
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
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void NetworkManager::networkStateChanged(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void NetworkManager::toast(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE

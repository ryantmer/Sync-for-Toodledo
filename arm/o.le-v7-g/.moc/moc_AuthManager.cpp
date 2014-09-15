/****************************************************************************
** Meta object code from reading C++ file 'AuthManager.hpp'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/AuthManager.hpp"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AuthManager.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_AuthManager[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      26,   13,   12,   12, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_AuthManager[] = {
    "AuthManager\0\0url,response\0"
    "onNetworkResponse(QUrl,QString)\0"
};

void AuthManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        AuthManager *_t = static_cast<AuthManager *>(_o);
        switch (_id) {
        case 0: _t->onNetworkResponse((*reinterpret_cast< QUrl(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData AuthManager::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject AuthManager::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_AuthManager,
      qt_meta_data_AuthManager, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &AuthManager::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *AuthManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *AuthManager::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_AuthManager))
        return static_cast<void*>(const_cast< AuthManager*>(this));
    return QObject::qt_metacast(_clname);
}

int AuthManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE

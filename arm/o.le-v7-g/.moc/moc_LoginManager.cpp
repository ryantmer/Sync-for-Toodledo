/****************************************************************************
** Meta object code from reading C++ file 'LoginManager.hpp'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/LoginManager.hpp"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'LoginManager.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_LoginManager[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: signature, parameters, type, tag, flags
      26,   14,   13,   13, 0x05,
      55,   13,   13,   13, 0x05,
      77,   13,   13,   13, 0x05,
      98,   13,   13,   13, 0x05,
     122,   13,   13,   13, 0x05,
     153,  145,   13,   13, 0x05,

 // slots: signature, parameters, type, tag, flags
     168,   13,   13,   13, 0x0a,
     192,   13,   13,   13, 0x0a,
     221,  215,   13,   13, 0x0a,
     260,   13,   13,   13, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_LoginManager[] = {
    "LoginManager\0\0accessToken\0"
    "accessTokenReceived(QString)\0"
    "refreshTokenExpired()\0accessTokenExpired()\0"
    "refreshTokenRefreshed()\0accessTokenRefreshed()\0"
    "message\0toast(QString)\0onRefreshTokenExpired()\0"
    "onAccessTokenExpired()\0reply\0"
    "onTokenRequestFinished(QNetworkReply*)\0"
    "onLoggedOut()\0"
};

void LoginManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        LoginManager *_t = static_cast<LoginManager *>(_o);
        switch (_id) {
        case 0: _t->accessTokenReceived((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->refreshTokenExpired(); break;
        case 2: _t->accessTokenExpired(); break;
        case 3: _t->refreshTokenRefreshed(); break;
        case 4: _t->accessTokenRefreshed(); break;
        case 5: _t->toast((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 6: _t->onRefreshTokenExpired(); break;
        case 7: _t->onAccessTokenExpired(); break;
        case 8: _t->onTokenRequestFinished((*reinterpret_cast< QNetworkReply*(*)>(_a[1]))); break;
        case 9: _t->onLoggedOut(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData LoginManager::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject LoginManager::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_LoginManager,
      qt_meta_data_LoginManager, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &LoginManager::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *LoginManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *LoginManager::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_LoginManager))
        return static_cast<void*>(const_cast< LoginManager*>(this));
    return QObject::qt_metacast(_clname);
}

int LoginManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void LoginManager::accessTokenReceived(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void LoginManager::refreshTokenExpired()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void LoginManager::accessTokenExpired()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void LoginManager::refreshTokenRefreshed()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}

// SIGNAL 4
void LoginManager::accessTokenRefreshed()
{
    QMetaObject::activate(this, &staticMetaObject, 4, 0);
}

// SIGNAL 5
void LoginManager::toast(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}
QT_END_MOC_NAMESPACE

/****************************************************************************
** Meta object code from reading C++ file 'ToodleDoTen.hpp'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/ToodleDoTen.hpp"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ToodleDoTen.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ToodleDoTen[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      15,   14, // methods
       1,   89, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: signature, parameters, type, tag, flags
      18,   13,   12,   12, 0x05,
      41,   13,   12,   12, 0x05,
      65,   13,   12,   12, 0x05,
      90,   12,   12,   12, 0x05,

 // slots: signature, parameters, type, tag, flags
     106,  102,   12,   12, 0x0a,
     132,   12,   12,   12, 0x0a,
     157,   12,   12,   12, 0x0a,

 // methods: signature, parameters, type, tag, flags
     200,  191,  181,   12, 0x02,
     239,  230,  225,   12, 0x02,
     269,   12,   12,   12, 0x02,
     288,  279,   12,   12, 0x02,
     309,  279,   12,   12, 0x02,
     331,  279,   12,   12, 0x02,
     355,   12,   12,   12, 0x02,
     372,  364,   12,   12, 0x02,

 // properties: name, type, flags
     406,  391, 0x00095409,

       0        // eod
};

static const char qt_meta_stringdata_ToodleDoTen[] = {
    "ToodleDoTen\0\0data\0taskAdded(QVariantMap)\0"
    "taskEdited(QVariantMap)\0"
    "taskRemoved(QVariantMap)\0loggedOut()\0"
    "url\0onWebViewUrlChanged(QUrl)\0"
    "onAccessTokenRefreshed()\0"
    "onRefreshTokenExpired()\0QDateTime\0"
    "unixTime\0unixTimeToDateTime(uint)\0"
    "uint\0dateTime\0dateTimeToUnixTime(QDateTime)\0"
    "refresh()\0taskData\0addTask(QVariantMap)\0"
    "editTask(QVariantMap)\0removeTask(QVariantMap)\0"
    "logout()\0message\0showToast(QString)\0"
    "TaskDataModel*\0dataModel\0"
};

void ToodleDoTen::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        ToodleDoTen *_t = static_cast<ToodleDoTen *>(_o);
        switch (_id) {
        case 0: _t->taskAdded((*reinterpret_cast< QVariantMap(*)>(_a[1]))); break;
        case 1: _t->taskEdited((*reinterpret_cast< QVariantMap(*)>(_a[1]))); break;
        case 2: _t->taskRemoved((*reinterpret_cast< QVariantMap(*)>(_a[1]))); break;
        case 3: _t->loggedOut(); break;
        case 4: _t->onWebViewUrlChanged((*reinterpret_cast< QUrl(*)>(_a[1]))); break;
        case 5: _t->onAccessTokenRefreshed(); break;
        case 6: _t->onRefreshTokenExpired(); break;
        case 7: { QDateTime _r = _t->unixTimeToDateTime((*reinterpret_cast< uint(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QDateTime*>(_a[0]) = _r; }  break;
        case 8: { uint _r = _t->dateTimeToUnixTime((*reinterpret_cast< QDateTime(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< uint*>(_a[0]) = _r; }  break;
        case 9: _t->refresh(); break;
        case 10: _t->addTask((*reinterpret_cast< QVariantMap(*)>(_a[1]))); break;
        case 11: _t->editTask((*reinterpret_cast< QVariantMap(*)>(_a[1]))); break;
        case 12: _t->removeTask((*reinterpret_cast< QVariantMap(*)>(_a[1]))); break;
        case 13: _t->logout(); break;
        case 14: _t->showToast((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData ToodleDoTen::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject ToodleDoTen::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_ToodleDoTen,
      qt_meta_data_ToodleDoTen, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ToodleDoTen::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ToodleDoTen::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ToodleDoTen::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ToodleDoTen))
        return static_cast<void*>(const_cast< ToodleDoTen*>(this));
    return QObject::qt_metacast(_clname);
}

int ToodleDoTen::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< TaskDataModel**>(_v) = dataModel(); break;
        }
        _id -= 1;
    } else if (_c == QMetaObject::WriteProperty) {
        _id -= 1;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 1;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void ToodleDoTen::taskAdded(QVariantMap _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ToodleDoTen::taskEdited(QVariantMap _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void ToodleDoTen::taskRemoved(QVariantMap _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void ToodleDoTen::loggedOut()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}
QT_END_MOC_NAMESPACE

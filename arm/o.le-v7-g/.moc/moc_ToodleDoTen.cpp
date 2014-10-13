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
static const uint qt_meta_data_ToodledoTen[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      24,   14, // methods
       2,  134, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       7,       // signalCount

 // signals: signature, parameters, type, tag, flags
      18,   13,   12,   12, 0x05,
      57,   41,   12,   12, 0x05,
      93,   13,   12,   12, 0x05,
     118,   13,   12,   12, 0x05,
     143,   41,   12,   12, 0x05,
     181,   13,   12,   12, 0x05,
     208,   12,   12,   12, 0x05,

 // slots: signature, parameters, type, tag, flags
     224,  220,   12,   12, 0x0a,
     250,   12,   12,   12, 0x0a,
     275,   12,   12,   12, 0x0a,
     299,   12,   12,   12, 0x0a,

 // methods: signature, parameters, type, tag, flags
     334,  325,  315,   12, 0x02,
     373,  364,  359,   12, 0x02,
     403,   12,   12,   12, 0x02,
     418,   12,   12,   12, 0x02,
     435,   13,   12,   12, 0x02,
     456,   41,   12,   12, 0x02,
     490,   13,   12,   12, 0x02,
     514,   13,   12,   12, 0x02,
     537,   41,   12,   12, 0x02,
     573,   13,   12,   12, 0x02,
     612,   12,  599,   12, 0x02,
     628,   12,   12,   12, 0x02,
     645,  637,   12,   12, 0x02,

 // properties: name, type, flags
     679,  664, 0x00095409,
     710,  693, 0x00095409,

       0        // eod
};

static const char qt_meta_stringdata_ToodledoTen[] = {
    "ToodledoTen\0\0data\0taskAdded(QVariantMap)\0"
    "oldData,newData\0taskEdited(QVariantMap,QVariantMap)\0"
    "taskRemoved(QVariantMap)\0"
    "folderAdded(QVariantMap)\0"
    "folderEdited(QVariantMap,QVariantMap)\0"
    "folderRemoved(QVariantMap)\0loggedOut()\0"
    "url\0onWebViewUrlChanged(QUrl)\0"
    "onAccessTokenRefreshed()\0"
    "onRefreshTokenExpired()\0onAppMinimize()\0"
    "QDateTime\0unixTime\0unixTimeToDateTime(uint)\0"
    "uint\0dateTime\0dateTimeToUnixTime(QDateTime)\0"
    "refreshTasks()\0refreshFolders()\0"
    "addTask(QVariantMap)\0"
    "editTask(QVariantMap,QVariantMap)\0"
    "removeTask(QVariantMap)\0addFolder(QVariantMap)\0"
    "editFolder(QVariantMap,QVariantMap)\0"
    "removeFolder(QVariantMap)\0QVariantList\0"
    "getFolderList()\0logout()\0message\0"
    "showToast(QString)\0TaskDataModel*\0"
    "taskDataModel\0FolderDataModel*\0"
    "folderDataModel\0"
};

void ToodledoTen::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        ToodledoTen *_t = static_cast<ToodledoTen *>(_o);
        switch (_id) {
        case 0: _t->taskAdded((*reinterpret_cast< QVariantMap(*)>(_a[1]))); break;
        case 1: _t->taskEdited((*reinterpret_cast< QVariantMap(*)>(_a[1])),(*reinterpret_cast< QVariantMap(*)>(_a[2]))); break;
        case 2: _t->taskRemoved((*reinterpret_cast< QVariantMap(*)>(_a[1]))); break;
        case 3: _t->folderAdded((*reinterpret_cast< QVariantMap(*)>(_a[1]))); break;
        case 4: _t->folderEdited((*reinterpret_cast< QVariantMap(*)>(_a[1])),(*reinterpret_cast< QVariantMap(*)>(_a[2]))); break;
        case 5: _t->folderRemoved((*reinterpret_cast< QVariantMap(*)>(_a[1]))); break;
        case 6: _t->loggedOut(); break;
        case 7: _t->onWebViewUrlChanged((*reinterpret_cast< QUrl(*)>(_a[1]))); break;
        case 8: _t->onAccessTokenRefreshed(); break;
        case 9: _t->onRefreshTokenExpired(); break;
        case 10: _t->onAppMinimize(); break;
        case 11: { QDateTime _r = _t->unixTimeToDateTime((*reinterpret_cast< uint(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QDateTime*>(_a[0]) = _r; }  break;
        case 12: { uint _r = _t->dateTimeToUnixTime((*reinterpret_cast< QDateTime(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< uint*>(_a[0]) = _r; }  break;
        case 13: _t->refreshTasks(); break;
        case 14: _t->refreshFolders(); break;
        case 15: _t->addTask((*reinterpret_cast< QVariantMap(*)>(_a[1]))); break;
        case 16: _t->editTask((*reinterpret_cast< QVariantMap(*)>(_a[1])),(*reinterpret_cast< QVariantMap(*)>(_a[2]))); break;
        case 17: _t->removeTask((*reinterpret_cast< QVariantMap(*)>(_a[1]))); break;
        case 18: _t->addFolder((*reinterpret_cast< QVariantMap(*)>(_a[1]))); break;
        case 19: _t->editFolder((*reinterpret_cast< QVariantMap(*)>(_a[1])),(*reinterpret_cast< QVariantMap(*)>(_a[2]))); break;
        case 20: _t->removeFolder((*reinterpret_cast< QVariantMap(*)>(_a[1]))); break;
        case 21: { QVariantList _r = _t->getFolderList();
            if (_a[0]) *reinterpret_cast< QVariantList*>(_a[0]) = _r; }  break;
        case 22: _t->logout(); break;
        case 23: _t->showToast((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData ToodledoTen::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject ToodledoTen::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_ToodledoTen,
      qt_meta_data_ToodledoTen, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ToodledoTen::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ToodledoTen::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ToodledoTen::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ToodledoTen))
        return static_cast<void*>(const_cast< ToodledoTen*>(this));
    return QObject::qt_metacast(_clname);
}

int ToodledoTen::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 24)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 24;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< TaskDataModel**>(_v) = taskDataModel(); break;
        case 1: *reinterpret_cast< FolderDataModel**>(_v) = folderDataModel(); break;
        }
        _id -= 2;
    } else if (_c == QMetaObject::WriteProperty) {
        _id -= 2;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 2;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void ToodledoTen::taskAdded(QVariantMap _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ToodledoTen::taskEdited(QVariantMap _t1, QVariantMap _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void ToodledoTen::taskRemoved(QVariantMap _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void ToodledoTen::folderAdded(QVariantMap _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void ToodledoTen::folderEdited(QVariantMap _t1, QVariantMap _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void ToodledoTen::folderRemoved(QVariantMap _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void ToodledoTen::loggedOut()
{
    QMetaObject::activate(this, &staticMetaObject, 6, 0);
}
QT_END_MOC_NAMESPACE

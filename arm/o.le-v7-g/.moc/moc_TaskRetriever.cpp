/****************************************************************************
** Meta object code from reading C++ file 'TaskRetriever.hpp'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/TaskRetriever.hpp"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'TaskRetriever.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_TaskRetriever[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      20,   15,   14,   14, 0x05,
      54,   47,   14,   14, 0x05,

 // slots: signature, parameters, type, tag, flags
      89,   76,   14,   14, 0x08,
     131,  121,   14,   14, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_TaskRetriever[] = {
    "TaskRetriever\0\0data\0tasksUpdated(QVariantList)\0"
    "taskId\0taskUpdateFailed(int)\0url,response\0"
    "onNetworkResponse(QUrl,QString)\0"
    "url,error\0onNetworkResponseFailed(QUrl,int)\0"
};

void TaskRetriever::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        TaskRetriever *_t = static_cast<TaskRetriever *>(_o);
        switch (_id) {
        case 0: _t->tasksUpdated((*reinterpret_cast< QVariantList(*)>(_a[1]))); break;
        case 1: _t->taskUpdateFailed((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->onNetworkResponse((*reinterpret_cast< QUrl(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 3: _t->onNetworkResponseFailed((*reinterpret_cast< QUrl(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData TaskRetriever::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject TaskRetriever::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_TaskRetriever,
      qt_meta_data_TaskRetriever, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &TaskRetriever::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *TaskRetriever::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *TaskRetriever::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_TaskRetriever))
        return static_cast<void*>(const_cast< TaskRetriever*>(this));
    return QObject::qt_metacast(_clname);
}

int TaskRetriever::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void TaskRetriever::tasksUpdated(QVariantList _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void TaskRetriever::taskUpdateFailed(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE

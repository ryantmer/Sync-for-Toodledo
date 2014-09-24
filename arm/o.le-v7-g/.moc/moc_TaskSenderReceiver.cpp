/****************************************************************************
** Meta object code from reading C++ file 'TaskSenderReceiver.hpp'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/TaskSenderReceiver.hpp"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'TaskSenderReceiver.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_TaskSenderReceiver[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      26,   20,   19,   19, 0x05,
      53,   20,   19,   19, 0x05,

 // slots: signature, parameters, type, tag, flags
      83,   78,   19,   19, 0x0a,
     109,   78,   19,   19, 0x0a,
     142,  136,   19,   19, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_TaskSenderReceiver[] = {
    "TaskSenderReceiver\0\0tasks\0"
    "tasksUpdated(QVariantList)\0"
    "tasksAdded(QVariantList)\0task\0"
    "onTaskAdded(QVariantList)\0"
    "onTaskEdited(QVariantList)\0reply\0"
    "onTasksReceived(QNetworkReply*)\0"
};

void TaskSenderReceiver::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        TaskSenderReceiver *_t = static_cast<TaskSenderReceiver *>(_o);
        switch (_id) {
        case 0: _t->tasksUpdated((*reinterpret_cast< QVariantList(*)>(_a[1]))); break;
        case 1: _t->tasksAdded((*reinterpret_cast< QVariantList(*)>(_a[1]))); break;
        case 2: _t->onTaskAdded((*reinterpret_cast< QVariantList(*)>(_a[1]))); break;
        case 3: _t->onTaskEdited((*reinterpret_cast< QVariantList(*)>(_a[1]))); break;
        case 4: _t->onTasksReceived((*reinterpret_cast< QNetworkReply*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData TaskSenderReceiver::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject TaskSenderReceiver::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_TaskSenderReceiver,
      qt_meta_data_TaskSenderReceiver, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &TaskSenderReceiver::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *TaskSenderReceiver::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *TaskSenderReceiver::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_TaskSenderReceiver))
        return static_cast<void*>(const_cast< TaskSenderReceiver*>(this));
    return QObject::qt_metacast(_clname);
}

int TaskSenderReceiver::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void TaskSenderReceiver::tasksUpdated(QVariantList _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void TaskSenderReceiver::tasksAdded(QVariantList _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE

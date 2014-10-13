/****************************************************************************
** Meta object code from reading C++ file 'TaskDataModel.hpp'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/TaskDataModel.hpp"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'TaskDataModel.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_TaskDataModel[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      23,   15,   14,   14, 0x05,

 // slots: signature, parameters, type, tag, flags
      43,   38,   14,   14, 0x0a,
      69,   38,   14,   14, 0x0a,
      94,   38,   14,   14, 0x0a,
     121,   14,   14,   14, 0x0a,
     135,   14,   14,   14, 0x0a,

 // methods: signature, parameters, type, tag, flags
     165,  155,  151,   14, 0x02,
     195,  155,  190,   14, 0x02,
     229,  155,  221,   14, 0x02,
     261,  155,  252,   14, 0x02,

       0        // eod
};

static const char qt_meta_stringdata_TaskDataModel[] = {
    "TaskDataModel\0\0message\0toast(QString)\0"
    "task\0onTaskEdited(QVariantMap)\0"
    "onTaskAdded(QVariantMap)\0"
    "onTaskRemoved(QVariantMap)\0onLoggedOut()\0"
    "onAboutToQuit()\0int\0indexPath\0"
    "childCount(QVariantList)\0bool\0"
    "hasChildren(QVariantList)\0QString\0"
    "itemType(QVariantList)\0QVariant\0"
    "data(QVariantList)\0"
};

void TaskDataModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        TaskDataModel *_t = static_cast<TaskDataModel *>(_o);
        switch (_id) {
        case 0: _t->toast((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->onTaskEdited((*reinterpret_cast< QVariantMap(*)>(_a[1]))); break;
        case 2: _t->onTaskAdded((*reinterpret_cast< QVariantMap(*)>(_a[1]))); break;
        case 3: _t->onTaskRemoved((*reinterpret_cast< QVariantMap(*)>(_a[1]))); break;
        case 4: _t->onLoggedOut(); break;
        case 5: _t->onAboutToQuit(); break;
        case 6: { int _r = _t->childCount((*reinterpret_cast< const QVariantList(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 7: { bool _r = _t->hasChildren((*reinterpret_cast< const QVariantList(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 8: { QString _r = _t->itemType((*reinterpret_cast< const QVariantList(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 9: { QVariant _r = _t->data((*reinterpret_cast< const QVariantList(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QVariant*>(_a[0]) = _r; }  break;
        default: ;
        }
    }
}

const QMetaObjectExtraData TaskDataModel::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject TaskDataModel::staticMetaObject = {
    { &bb::cascades::DataModel::staticMetaObject, qt_meta_stringdata_TaskDataModel,
      qt_meta_data_TaskDataModel, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &TaskDataModel::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *TaskDataModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *TaskDataModel::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_TaskDataModel))
        return static_cast<void*>(const_cast< TaskDataModel*>(this));
    typedef bb::cascades::DataModel QMocSuperClass;
    return QMocSuperClass::qt_metacast(_clname);
}

int TaskDataModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    typedef bb::cascades::DataModel QMocSuperClass;
    _id = QMocSuperClass::qt_metacall(_c, _id, _a);
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
void TaskDataModel::toast(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE

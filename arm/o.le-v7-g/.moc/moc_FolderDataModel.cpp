/****************************************************************************
** Meta object code from reading C++ file 'FolderDataModel.hpp'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/FolderDataModel.hpp"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'FolderDataModel.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_FolderDataModel[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      24,   17,   16,   16, 0x0a,
      52,   17,   16,   16, 0x0a,
      79,   17,   16,   16, 0x0a,
     108,   16,   16,   16, 0x0a,
     122,   16,   16,   16, 0x0a,

 // methods: signature, parameters, type, tag, flags
     152,  142,  138,   16, 0x02,
     182,  142,  177,   16, 0x02,
     216,  142,  208,   16, 0x02,
     248,  142,  239,   16, 0x02,

       0        // eod
};

static const char qt_meta_stringdata_FolderDataModel[] = {
    "FolderDataModel\0\0folder\0"
    "onFolderEdited(QVariantMap)\0"
    "onFolderAdded(QVariantMap)\0"
    "onFolderRemoved(QVariantMap)\0onLoggedOut()\0"
    "onAboutToQuit()\0int\0indexPath\0"
    "childCount(QVariantList)\0bool\0"
    "hasChildren(QVariantList)\0QString\0"
    "itemType(QVariantList)\0QVariant\0"
    "data(QVariantList)\0"
};

void FolderDataModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        FolderDataModel *_t = static_cast<FolderDataModel *>(_o);
        switch (_id) {
        case 0: _t->onFolderEdited((*reinterpret_cast< QVariantMap(*)>(_a[1]))); break;
        case 1: _t->onFolderAdded((*reinterpret_cast< QVariantMap(*)>(_a[1]))); break;
        case 2: _t->onFolderRemoved((*reinterpret_cast< QVariantMap(*)>(_a[1]))); break;
        case 3: _t->onLoggedOut(); break;
        case 4: _t->onAboutToQuit(); break;
        case 5: { int _r = _t->childCount((*reinterpret_cast< const QVariantList(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 6: { bool _r = _t->hasChildren((*reinterpret_cast< const QVariantList(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 7: { QString _r = _t->itemType((*reinterpret_cast< const QVariantList(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 8: { QVariant _r = _t->data((*reinterpret_cast< const QVariantList(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QVariant*>(_a[0]) = _r; }  break;
        default: ;
        }
    }
}

const QMetaObjectExtraData FolderDataModel::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject FolderDataModel::staticMetaObject = {
    { &bb::cascades::DataModel::staticMetaObject, qt_meta_stringdata_FolderDataModel,
      qt_meta_data_FolderDataModel, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &FolderDataModel::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *FolderDataModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *FolderDataModel::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_FolderDataModel))
        return static_cast<void*>(const_cast< FolderDataModel*>(this));
    typedef bb::cascades::DataModel QMocSuperClass;
    return QMocSuperClass::qt_metacast(_clname);
}

int FolderDataModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    typedef bb::cascades::DataModel QMocSuperClass;
    _id = QMocSuperClass::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    }
    return _id;
}
QT_END_MOC_NAMESPACE

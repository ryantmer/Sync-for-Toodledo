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
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      25,   17,   16,   16, 0x05,

 // slots: signature, parameters, type, tag, flags
      47,   40,   16,   16, 0x0a,
      75,   40,   16,   16, 0x0a,
     102,   40,   16,   16, 0x0a,
     131,   16,   16,   16, 0x0a,
     145,   16,   16,   16, 0x0a,

 // methods: signature, parameters, type, tag, flags
     175,  165,  161,   16, 0x02,
     205,  165,  200,   16, 0x02,
     239,  165,  231,   16, 0x02,
     271,  165,  262,   16, 0x02,

       0        // eod
};

static const char qt_meta_stringdata_FolderDataModel[] = {
    "FolderDataModel\0\0message\0toast(QString)\0"
    "folder\0onFolderEdited(QVariantMap)\0"
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
        case 0: _t->toast((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->onFolderEdited((*reinterpret_cast< QVariantMap(*)>(_a[1]))); break;
        case 2: _t->onFolderAdded((*reinterpret_cast< QVariantMap(*)>(_a[1]))); break;
        case 3: _t->onFolderRemoved((*reinterpret_cast< QVariantMap(*)>(_a[1]))); break;
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
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void FolderDataModel::toast(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE

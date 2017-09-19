/****************************************************************************
** Meta object code from reading C++ file 'voipservice.hpp'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/voipservice.hpp"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'voipservice.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_VoipService[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      13,   12,   12,   12, 0x0a,
      29,   12,   12,   12, 0x0a,
      46,   41,   12,   12, 0x0a,
      62,   12,   12,   12, 0x0a,
      77,   12,   12,   12, 0x08,
     117,   12,   12,   12, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_VoipService[] = {
    "VoipService\0\0newConnection()\0readyRead()\0"
    "code\0readyWrite(int)\0disconnected()\0"
    "handleInvoke(bb::system::InvokeRequest)\0"
    "onTimeout()\0"
};

void VoipService::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        VoipService *_t = static_cast<VoipService *>(_o);
        switch (_id) {
        case 0: _t->newConnection(); break;
        case 1: _t->readyRead(); break;
        case 2: _t->readyWrite((*reinterpret_cast< const int(*)>(_a[1]))); break;
        case 3: _t->disconnected(); break;
        case 4: _t->handleInvoke((*reinterpret_cast< const bb::system::InvokeRequest(*)>(_a[1]))); break;
        case 5: _t->onTimeout(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData VoipService::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject VoipService::staticMetaObject = {
    { &Application::staticMetaObject, qt_meta_stringdata_VoipService,
      qt_meta_data_VoipService, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &VoipService::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *VoipService::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *VoipService::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_VoipService))
        return static_cast<void*>(const_cast< VoipService*>(this));
    return Application::qt_metacast(_clname);
}

int VoipService::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Application::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}
QT_END_MOC_NAMESPACE

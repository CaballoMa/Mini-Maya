/****************************************************************************
** Meta object code from reading C++ file 'mygl.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.2.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "C:/Users/DELL/Desktop/homework-05-06-07-micro-maya-mesh-editor-CaballoMa/assignment_package/src/mygl.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mygl.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.2.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MyGL_t {
    const uint offsetsAndSize[48];
    char stringdata0[347];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_MyGL_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_MyGL_t qt_meta_stringdata_MyGL = {
    {
QT_MOC_LITERAL(0, 4), // "MyGL"
QT_MOC_LITERAL(5, 18), // "sig_sendVertexData"
QT_MOC_LITERAL(24, 0), // ""
QT_MOC_LITERAL(25, 5), // "Mesh*"
QT_MOC_LITERAL(31, 4), // "mesh"
QT_MOC_LITERAL(36, 16), // "sig_sendFaceData"
QT_MOC_LITERAL(53, 20), // "sig_sendHalfEdgeData"
QT_MOC_LITERAL(74, 12), // "slot_loadObj"
QT_MOC_LITERAL(87, 28), // "slot_highlightSelectedVertex"
QT_MOC_LITERAL(116, 16), // "QListWidgetItem*"
QT_MOC_LITERAL(133, 1), // "v"
QT_MOC_LITERAL(135, 26), // "slot_highlightSelectedFace"
QT_MOC_LITERAL(162, 1), // "f"
QT_MOC_LITERAL(164, 30), // "slot_highlightSelectedHalfEdge"
QT_MOC_LITERAL(195, 2), // "he"
QT_MOC_LITERAL(198, 16), // "slot_updateVertX"
QT_MOC_LITERAL(215, 16), // "slot_updateVertY"
QT_MOC_LITERAL(232, 16), // "slot_updateVertZ"
QT_MOC_LITERAL(249, 16), // "slot_updateFaceR"
QT_MOC_LITERAL(266, 16), // "slot_updateFaceG"
QT_MOC_LITERAL(283, 16), // "slot_updateFaceB"
QT_MOC_LITERAL(300, 14), // "slot_splitEdge"
QT_MOC_LITERAL(315, 16), // "slot_triangulate"
QT_MOC_LITERAL(332, 14) // "slot_subdivide"

    },
    "MyGL\0sig_sendVertexData\0\0Mesh*\0mesh\0"
    "sig_sendFaceData\0sig_sendHalfEdgeData\0"
    "slot_loadObj\0slot_highlightSelectedVertex\0"
    "QListWidgetItem*\0v\0slot_highlightSelectedFace\0"
    "f\0slot_highlightSelectedHalfEdge\0he\0"
    "slot_updateVertX\0slot_updateVertY\0"
    "slot_updateVertZ\0slot_updateFaceR\0"
    "slot_updateFaceG\0slot_updateFaceB\0"
    "slot_splitEdge\0slot_triangulate\0"
    "slot_subdivide"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MyGL[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
      16,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,  110,    2, 0x06,    1 /* Public */,
       5,    1,  113,    2, 0x06,    3 /* Public */,
       6,    1,  116,    2, 0x06,    5 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       7,    0,  119,    2, 0x0a,    7 /* Public */,
       8,    1,  120,    2, 0x0a,    8 /* Public */,
      11,    1,  123,    2, 0x0a,   10 /* Public */,
      13,    1,  126,    2, 0x0a,   12 /* Public */,
      15,    1,  129,    2, 0x0a,   14 /* Public */,
      16,    1,  132,    2, 0x0a,   16 /* Public */,
      17,    1,  135,    2, 0x0a,   18 /* Public */,
      18,    1,  138,    2, 0x0a,   20 /* Public */,
      19,    1,  141,    2, 0x0a,   22 /* Public */,
      20,    1,  144,    2, 0x0a,   24 /* Public */,
      21,    0,  147,    2, 0x0a,   26 /* Public */,
      22,    0,  148,    2, 0x0a,   27 /* Public */,
      23,    0,  149,    2, 0x0a,   28 /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 3,    4,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 9,   10,
    QMetaType::Void, 0x80000000 | 9,   12,
    QMetaType::Void, 0x80000000 | 9,   14,
    QMetaType::Void, QMetaType::Double,    2,
    QMetaType::Void, QMetaType::Double,    2,
    QMetaType::Void, QMetaType::Double,    2,
    QMetaType::Void, QMetaType::Double,    2,
    QMetaType::Void, QMetaType::Double,    2,
    QMetaType::Void, QMetaType::Double,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void MyGL::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MyGL *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->sig_sendVertexData((*reinterpret_cast< Mesh*(*)>(_a[1]))); break;
        case 1: _t->sig_sendFaceData((*reinterpret_cast< Mesh*(*)>(_a[1]))); break;
        case 2: _t->sig_sendHalfEdgeData((*reinterpret_cast< Mesh*(*)>(_a[1]))); break;
        case 3: _t->slot_loadObj(); break;
        case 4: _t->slot_highlightSelectedVertex((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        case 5: _t->slot_highlightSelectedFace((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        case 6: _t->slot_highlightSelectedHalfEdge((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        case 7: _t->slot_updateVertX((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 8: _t->slot_updateVertY((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 9: _t->slot_updateVertZ((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 10: _t->slot_updateFaceR((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 11: _t->slot_updateFaceG((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 12: _t->slot_updateFaceB((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 13: _t->slot_splitEdge(); break;
        case 14: _t->slot_triangulate(); break;
        case 15: _t->slot_subdivide(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (MyGL::*)(Mesh * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MyGL::sig_sendVertexData)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (MyGL::*)(Mesh * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MyGL::sig_sendFaceData)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (MyGL::*)(Mesh * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MyGL::sig_sendHalfEdgeData)) {
                *result = 2;
                return;
            }
        }
    }
}

const QMetaObject MyGL::staticMetaObject = { {
    QMetaObject::SuperData::link<OpenGLContext::staticMetaObject>(),
    qt_meta_stringdata_MyGL.offsetsAndSize,
    qt_meta_data_MyGL,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_MyGL_t
, QtPrivate::TypeAndForceComplete<MyGL, std::true_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<Mesh *, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<Mesh *, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<Mesh *, std::false_type>
, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<QListWidgetItem *, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<QListWidgetItem *, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<QListWidgetItem *, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<double, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<double, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<double, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<double, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<double, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<double, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>


>,
    nullptr
} };


const QMetaObject *MyGL::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MyGL::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MyGL.stringdata0))
        return static_cast<void*>(this);
    return OpenGLContext::qt_metacast(_clname);
}

int MyGL::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = OpenGLContext::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 16)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 16;
    }
    return _id;
}

// SIGNAL 0
void MyGL::sig_sendVertexData(Mesh * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void MyGL::sig_sendFaceData(Mesh * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void MyGL::sig_sendHalfEdgeData(Mesh * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE

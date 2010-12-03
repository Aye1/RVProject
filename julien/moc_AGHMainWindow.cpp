/****************************************************************************
** Meta object code from reading C++ file 'AGHMainWindow.h'
**
** Created: Fri Dec 3 14:54:14 2010
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "AGHMainWindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AGHMainWindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_AGHMainWindow[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
      15,   14,   14,   14, 0x0a,
      26,   14,   14,   14, 0x0a,
      38,   14,   14,   14, 0x0a,
      45,   14,   14,   14, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_AGHMainWindow[] = {
    "AGHMainWindow\0\0openFile()\0closeFile()\0"
    "play()\0playNote()\0"
};

const QMetaObject AGHMainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_AGHMainWindow,
      qt_meta_data_AGHMainWindow, 0 }
};

const QMetaObject *AGHMainWindow::metaObject() const
{
    return &staticMetaObject;
}

void *AGHMainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_AGHMainWindow))
        return static_cast<void*>(const_cast< AGHMainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int AGHMainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: openFile(); break;
        case 1: closeFile(); break;
        case 2: play(); break;
        case 3: playNote(); break;
        default: ;
        }
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE

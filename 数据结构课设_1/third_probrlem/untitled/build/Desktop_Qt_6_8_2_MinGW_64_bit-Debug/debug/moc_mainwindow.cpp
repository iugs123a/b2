/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.8.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../mainwindow.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.8.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN10MainWindowE_t {};
} // unnamed namespace


#ifdef QT_MOC_HAS_STRINGDATA
static constexpr auto qt_meta_stringdata_ZN10MainWindowE = QtMocHelpers::stringData(
    "MainWindow",
    "onLoadData",
    "",
    "updateCityComboBoxes",
    "onLoadAdjacencyData",
    "onCheckProvinceDistance",
    "onQueryShortestPaths",
    "findAllPaths",
    "QList<QStringList>",
    "start",
    "end",
    "maxNodes",
    "quickSortPaths",
    "QList<QStringList>&",
    "paths",
    "calculatePathLength",
    "path",
    "saveResultsToFile",
    "dijkstraPath",
    "dijkstraDistance",
    "floydPath",
    "floydDistance",
    "QList<std::pair<QStringList,double>>",
    "sortedPaths",
    "saveAvoidCityResultsToFile",
    "avoidCity",
    "normalPath",
    "normalDistance",
    "onQueryAvoidCityPaths",
    "onQueryKthShortestPath"
);
#else  // !QT_MOC_HAS_STRINGDATA
#error "qtmochelpers.h not found or too old."
#endif // !QT_MOC_HAS_STRINGDATA

Q_CONSTINIT static const uint qt_meta_data_ZN10MainWindowE[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   86,    2, 0x08,    1 /* Private */,
       3,    0,   87,    2, 0x08,    2 /* Private */,
       4,    0,   88,    2, 0x08,    3 /* Private */,
       5,    0,   89,    2, 0x08,    4 /* Private */,
       6,    0,   90,    2, 0x08,    5 /* Private */,
       7,    3,   91,    2, 0x08,    6 /* Private */,
      12,    1,   98,    2, 0x08,   10 /* Private */,
      15,    1,  101,    2, 0x08,   12 /* Private */,
      17,    7,  104,    2, 0x08,   14 /* Private */,
      24,   10,  119,    2, 0x08,   22 /* Private */,
      28,    0,  140,    2, 0x08,   33 /* Private */,
      29,    0,  141,    2, 0x08,   34 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    0x80000000 | 8, QMetaType::QString, QMetaType::QString, QMetaType::Int,    9,   10,   11,
    QMetaType::Void, 0x80000000 | 13,   14,
    QMetaType::Double, QMetaType::QStringList,   16,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QStringList, QMetaType::Double, QMetaType::QStringList, QMetaType::Double, 0x80000000 | 22,    9,   10,   18,   19,   20,   21,   23,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QStringList, QMetaType::Double, QMetaType::QStringList, QMetaType::Double, QMetaType::QStringList, QMetaType::Double, 0x80000000 | 22,    9,   10,   25,   18,   19,   20,   21,   26,   27,   23,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject MainWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_ZN10MainWindowE.offsetsAndSizes,
    qt_meta_data_ZN10MainWindowE,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_tag_ZN10MainWindowE_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<MainWindow, std::true_type>,
        // method 'onLoadData'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'updateCityComboBoxes'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onLoadAdjacencyData'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onCheckProvinceDistance'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onQueryShortestPaths'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'findAllPaths'
        QtPrivate::TypeAndForceComplete<QVector<QStringList>, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'quickSortPaths'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QVector<QStringList> &, std::false_type>,
        // method 'calculatePathLength'
        QtPrivate::TypeAndForceComplete<double, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QStringList &, std::false_type>,
        // method 'saveResultsToFile'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QStringList &, std::false_type>,
        QtPrivate::TypeAndForceComplete<double, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QStringList &, std::false_type>,
        QtPrivate::TypeAndForceComplete<double, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QVector<QPair<QStringList,double>> &, std::false_type>,
        // method 'saveAvoidCityResultsToFile'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QStringList &, std::false_type>,
        QtPrivate::TypeAndForceComplete<double, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QStringList &, std::false_type>,
        QtPrivate::TypeAndForceComplete<double, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QStringList &, std::false_type>,
        QtPrivate::TypeAndForceComplete<double, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QVector<QPair<QStringList,double>> &, std::false_type>,
        // method 'onQueryAvoidCityPaths'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onQueryKthShortestPath'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<MainWindow *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->onLoadData(); break;
        case 1: _t->updateCityComboBoxes(); break;
        case 2: _t->onLoadAdjacencyData(); break;
        case 3: _t->onCheckProvinceDistance(); break;
        case 4: _t->onQueryShortestPaths(); break;
        case 5: { QList<QStringList> _r = _t->findAllPaths((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[3])));
            if (_a[0]) *reinterpret_cast< QList<QStringList>*>(_a[0]) = std::move(_r); }  break;
        case 6: _t->quickSortPaths((*reinterpret_cast< std::add_pointer_t<QList<QStringList>&>>(_a[1]))); break;
        case 7: { double _r = _t->calculatePathLength((*reinterpret_cast< std::add_pointer_t<QStringList>>(_a[1])));
            if (_a[0]) *reinterpret_cast< double*>(_a[0]) = std::move(_r); }  break;
        case 8: _t->saveResultsToFile((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QStringList>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<double>>(_a[4])),(*reinterpret_cast< std::add_pointer_t<QStringList>>(_a[5])),(*reinterpret_cast< std::add_pointer_t<double>>(_a[6])),(*reinterpret_cast< std::add_pointer_t<QList<std::pair<QStringList,double>>>>(_a[7]))); break;
        case 9: _t->saveAvoidCityResultsToFile((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<QStringList>>(_a[4])),(*reinterpret_cast< std::add_pointer_t<double>>(_a[5])),(*reinterpret_cast< std::add_pointer_t<QStringList>>(_a[6])),(*reinterpret_cast< std::add_pointer_t<double>>(_a[7])),(*reinterpret_cast< std::add_pointer_t<QStringList>>(_a[8])),(*reinterpret_cast< std::add_pointer_t<double>>(_a[9])),(*reinterpret_cast< std::add_pointer_t<QList<std::pair<QStringList,double>>>>(_a[10]))); break;
        case 10: _t->onQueryAvoidCityPaths(); break;
        case 11: _t->onQueryKthShortestPath(); break;
        default: ;
        }
    }
}

const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ZN10MainWindowE.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 12;
    }
    return _id;
}
QT_WARNING_POP

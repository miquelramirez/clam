/****************************************************************************
** CLAM::VM::PlotController meta object code from reading C++ file 'PlotController.hxx'
**
** Created: Mon Mar 14 17:56:41 2005
**      by: The Qt MOC ($Id$)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "../../../CLAM/src/Visualization/QTVM/Controllers/PlotController.hxx"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.2.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *CLAM::VM::PlotController::className() const
{
    return "CLAM::VM::PlotController";
}

QMetaObject *CLAM::VM::PlotController::metaObj = 0;
static QMetaObjectCleanUp cleanUp_CLAM__VM__PlotController( "CLAM::VM::PlotController", &CLAM::VM::PlotController::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString CLAM::VM::PlotController::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "CLAM::VM::PlotController", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString CLAM::VM::PlotController::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "CLAM::VM::PlotController", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* CLAM::VM::PlotController::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QObject::staticMetaObject();
    static const QUMethod signal_0 = {"requestRefresh", 0, 0 };
    static const QUParameter param_signal_1[] = {
	{ 0, &static_QUType_ptr, "View", QUParameter::In }
    };
    static const QUMethod signal_1 = {"sendView", 1, param_signal_1 };
    static const QUParameter param_signal_2[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod signal_2 = {"hZoomRatio", 1, param_signal_2 };
    static const QUParameter param_signal_3[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod signal_3 = {"hScrollValue", 1, param_signal_3 };
    static const QUParameter param_signal_4[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod signal_4 = {"hScrollMaxValue", 1, param_signal_4 };
    static const QUParameter param_signal_5[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod signal_5 = {"vZoomRatio", 1, param_signal_5 };
    static const QUParameter param_signal_6[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod signal_6 = {"vScrollValue", 1, param_signal_6 };
    static const QUParameter param_signal_7[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod signal_7 = {"vScrollMaxValue", 1, param_signal_7 };
    static const QUParameter param_signal_8[] = {
	{ 0, &static_QUType_ptr, "TData", QUParameter::In }
    };
    static const QUMethod signal_8 = {"selPos", 1, param_signal_8 };
    static const QUParameter param_signal_9[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_9 = {"toolTip", 1, param_signal_9 };
    static const QUMethod signal_10 = {"leavingMouse", 0, 0 };
    static const QUParameter param_signal_11[] = {
	{ 0, &static_QUType_varptr, "\x18", QUParameter::In }
    };
    static const QUMethod signal_11 = {"changeCursor", 1, param_signal_11 };
    static const QUParameter param_signal_12[] = {
	{ 0, &static_QUType_ptr, "unsigned", QUParameter::In }
    };
    static const QUMethod signal_12 = {"insertedMark", 1, param_signal_12 };
    static const QUParameter param_signal_13[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In },
	{ 0, &static_QUType_ptr, "unsigned", QUParameter::In }
    };
    static const QUMethod signal_13 = {"removedMark", 2, param_signal_13 };
    static const QUParameter param_signal_14[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In },
	{ 0, &static_QUType_ptr, "unsigned", QUParameter::In }
    };
    static const QUMethod signal_14 = {"updatedMark", 2, param_signal_14 };
    static const QMetaData signal_tbl[] = {
	{ "requestRefresh()", &signal_0, QMetaData::Public },
	{ "sendView(View)", &signal_1, QMetaData::Public },
	{ "hZoomRatio(int)", &signal_2, QMetaData::Public },
	{ "hScrollValue(int)", &signal_3, QMetaData::Public },
	{ "hScrollMaxValue(int)", &signal_4, QMetaData::Public },
	{ "vZoomRatio(int)", &signal_5, QMetaData::Public },
	{ "vScrollValue(int)", &signal_6, QMetaData::Public },
	{ "vScrollMaxValue(int)", &signal_7, QMetaData::Public },
	{ "selPos(TData)", &signal_8, QMetaData::Public },
	{ "toolTip(QString)", &signal_9, QMetaData::Public },
	{ "leavingMouse()", &signal_10, QMetaData::Public },
	{ "changeCursor(QCursor)", &signal_11, QMetaData::Public },
	{ "insertedMark(unsigned)", &signal_12, QMetaData::Public },
	{ "removedMark(int,unsigned)", &signal_13, QMetaData::Public },
	{ "updatedMark(int,unsigned)", &signal_14, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"CLAM::VM::PlotController", parentObject,
	0, 0,
	signal_tbl, 15,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_CLAM__VM__PlotController.setMetaObject( metaObj );
    return metaObj;
}

void* CLAM::VM::PlotController::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "CLAM::VM::PlotController" ) )
	return this;
    return QObject::qt_cast( clname );
}

// SIGNAL requestRefresh
void CLAM::VM::PlotController::requestRefresh()
{
    activate_signal( staticMetaObject()->signalOffset() + 0 );
}

#include <qobjectdefs.h>
#include <qsignalslotimp.h>

// SIGNAL sendView
void CLAM::VM::PlotController::sendView( View t0 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 1 );
    if ( !clist )
	return;
    QUObject o[2];
    static_QUType_ptr.set(o+1,&t0);
    activate_signal( clist, o );
}

// SIGNAL hZoomRatio
void CLAM::VM::PlotController::hZoomRatio( int t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 2, t0 );
}

// SIGNAL hScrollValue
void CLAM::VM::PlotController::hScrollValue( int t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 3, t0 );
}

// SIGNAL hScrollMaxValue
void CLAM::VM::PlotController::hScrollMaxValue( int t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 4, t0 );
}

// SIGNAL vZoomRatio
void CLAM::VM::PlotController::vZoomRatio( int t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 5, t0 );
}

// SIGNAL vScrollValue
void CLAM::VM::PlotController::vScrollValue( int t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 6, t0 );
}

// SIGNAL vScrollMaxValue
void CLAM::VM::PlotController::vScrollMaxValue( int t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 7, t0 );
}

// SIGNAL selPos
void CLAM::VM::PlotController::selPos( TData t0 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 8 );
    if ( !clist )
	return;
    QUObject o[2];
    static_QUType_ptr.set(o+1,&t0);
    activate_signal( clist, o );
}

// SIGNAL toolTip
void CLAM::VM::PlotController::toolTip( QString t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 9, t0 );
}

// SIGNAL leavingMouse
void CLAM::VM::PlotController::leavingMouse()
{
    activate_signal( staticMetaObject()->signalOffset() + 10 );
}

// SIGNAL changeCursor
void CLAM::VM::PlotController::changeCursor( QCursor t0 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 11 );
    if ( !clist )
	return;
    QUObject o[2];
    static_QUType_varptr.set(o+1,&t0);
    activate_signal( clist, o );
}

// SIGNAL insertedMark
void CLAM::VM::PlotController::insertedMark( unsigned t0 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 12 );
    if ( !clist )
	return;
    QUObject o[2];
    static_QUType_ptr.set(o+1,&t0);
    activate_signal( clist, o );
}

// SIGNAL removedMark
void CLAM::VM::PlotController::removedMark( int t0, unsigned t1 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 13 );
    if ( !clist )
	return;
    QUObject o[3];
    static_QUType_int.set(o+1,t0);
    static_QUType_ptr.set(o+2,&t1);
    activate_signal( clist, o );
}

// SIGNAL updatedMark
void CLAM::VM::PlotController::updatedMark( int t0, unsigned t1 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 14 );
    if ( !clist )
	return;
    QUObject o[3];
    static_QUType_int.set(o+1,t0);
    static_QUType_ptr.set(o+2,&t1);
    activate_signal( clist, o );
}

bool CLAM::VM::PlotController::qt_invoke( int _id, QUObject* _o )
{
    return QObject::qt_invoke(_id,_o);
}

bool CLAM::VM::PlotController::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: requestRefresh(); break;
    case 1: sendView((View)(*((View*)static_QUType_ptr.get(_o+1)))); break;
    case 2: hZoomRatio((int)static_QUType_int.get(_o+1)); break;
    case 3: hScrollValue((int)static_QUType_int.get(_o+1)); break;
    case 4: hScrollMaxValue((int)static_QUType_int.get(_o+1)); break;
    case 5: vZoomRatio((int)static_QUType_int.get(_o+1)); break;
    case 6: vScrollValue((int)static_QUType_int.get(_o+1)); break;
    case 7: vScrollMaxValue((int)static_QUType_int.get(_o+1)); break;
    case 8: selPos((TData)(*((TData*)static_QUType_ptr.get(_o+1)))); break;
    case 9: toolTip((QString)static_QUType_QString.get(_o+1)); break;
    case 10: leavingMouse(); break;
    case 11: changeCursor((QCursor)(*((QCursor*)static_QUType_ptr.get(_o+1)))); break;
    case 12: insertedMark((unsigned)(*((unsigned*)static_QUType_ptr.get(_o+1)))); break;
    case 13: removedMark((int)static_QUType_int.get(_o+1),(unsigned)(*((unsigned*)static_QUType_ptr.get(_o+2)))); break;
    case 14: updatedMark((int)static_QUType_int.get(_o+1),(unsigned)(*((unsigned*)static_QUType_ptr.get(_o+2)))); break;
    default:
	return QObject::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool CLAM::VM::PlotController::qt_property( int id, int f, QVariant* v)
{
    return QObject::qt_property( id, f, v);
}

bool CLAM::VM::PlotController::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES

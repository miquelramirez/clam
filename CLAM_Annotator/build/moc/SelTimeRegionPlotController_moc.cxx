/****************************************************************************
** CLAM::VM::SelTimeRegionPlotController meta object code from reading C++ file 'SelTimeRegionPlotController.hxx'
**
** Created: Mon Mar 14 17:56:56 2005
**      by: The Qt MOC ($Id$)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "../../../CLAM/src/Visualization/QTVM/Controllers/SelTimeRegionPlotController.hxx"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.2.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *CLAM::VM::SelTimeRegionPlotController::className() const
{
    return "CLAM::VM::SelTimeRegionPlotController";
}

QMetaObject *CLAM::VM::SelTimeRegionPlotController::metaObj = 0;
static QMetaObjectCleanUp cleanUp_CLAM__VM__SelTimeRegionPlotController( "CLAM::VM::SelTimeRegionPlotController", &CLAM::VM::SelTimeRegionPlotController::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString CLAM::VM::SelTimeRegionPlotController::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "CLAM::VM::SelTimeRegionPlotController", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString CLAM::VM::SelTimeRegionPlotController::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "CLAM::VM::SelTimeRegionPlotController", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* CLAM::VM::SelTimeRegionPlotController::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = SelPosPlotController::staticMetaObject();
    static const QUParameter param_signal_0[] = {
	{ 0, &static_QUType_ptr, "MediaTime", QUParameter::In }
    };
    static const QUMethod signal_0 = {"selectedRegion", 1, param_signal_0 };
    static const QMetaData signal_tbl[] = {
	{ "selectedRegion(MediaTime)", &signal_0, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"CLAM::VM::SelTimeRegionPlotController", parentObject,
	0, 0,
	signal_tbl, 1,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_CLAM__VM__SelTimeRegionPlotController.setMetaObject( metaObj );
    return metaObj;
}

void* CLAM::VM::SelTimeRegionPlotController::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "CLAM::VM::SelTimeRegionPlotController" ) )
	return this;
    return SelPosPlotController::qt_cast( clname );
}

#include <qobjectdefs.h>
#include <qsignalslotimp.h>

// SIGNAL selectedRegion
void CLAM::VM::SelTimeRegionPlotController::selectedRegion( MediaTime t0 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 0 );
    if ( !clist )
	return;
    QUObject o[2];
    static_QUType_ptr.set(o+1,&t0);
    activate_signal( clist, o );
}

bool CLAM::VM::SelTimeRegionPlotController::qt_invoke( int _id, QUObject* _o )
{
    return SelPosPlotController::qt_invoke(_id,_o);
}

bool CLAM::VM::SelTimeRegionPlotController::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: selectedRegion((MediaTime)(*((MediaTime*)static_QUType_ptr.get(_o+1)))); break;
    default:
	return SelPosPlotController::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool CLAM::VM::SelTimeRegionPlotController::qt_property( int id, int f, QVariant* v)
{
    return SelPosPlotController::qt_property( id, f, v);
}

bool CLAM::VM::SelTimeRegionPlotController::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES

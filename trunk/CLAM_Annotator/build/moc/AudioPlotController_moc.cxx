/****************************************************************************
** CLAM::VM::AudioPlotController meta object code from reading C++ file 'AudioPlotController.hxx'
**
** Created: Mon Mar 14 17:56:58 2005
**      by: The Qt MOC ($Id$)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "../../../CLAM/src/Visualization/QTVM/Controllers/AudioPlotController.hxx"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.2.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *CLAM::VM::AudioPlotController::className() const
{
    return "CLAM::VM::AudioPlotController";
}

QMetaObject *CLAM::VM::AudioPlotController::metaObj = 0;
static QMetaObjectCleanUp cleanUp_CLAM__VM__AudioPlotController( "CLAM::VM::AudioPlotController", &CLAM::VM::AudioPlotController::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString CLAM::VM::AudioPlotController::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "CLAM::VM::AudioPlotController", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString CLAM::VM::AudioPlotController::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "CLAM::VM::AudioPlotController", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* CLAM::VM::AudioPlotController::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = SelTimeRegionPlotController::staticMetaObject();
    static const QUParameter param_signal_0[] = {
	{ 0, &static_QUType_ptr, "QwtScaleDiv", QUParameter::In }
    };
    static const QUMethod signal_0 = {"xRulerScaleDiv", 1, param_signal_0 };
    static const QUParameter param_signal_1[] = {
	{ 0, &static_QUType_ptr, "QwtScaleDiv", QUParameter::In }
    };
    static const QUMethod signal_1 = {"yRulerScaleDiv", 1, param_signal_1 };
    static const QMetaData signal_tbl[] = {
	{ "xRulerScaleDiv(QwtScaleDiv)", &signal_0, QMetaData::Public },
	{ "yRulerScaleDiv(QwtScaleDiv)", &signal_1, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"CLAM::VM::AudioPlotController", parentObject,
	0, 0,
	signal_tbl, 2,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_CLAM__VM__AudioPlotController.setMetaObject( metaObj );
    return metaObj;
}

void* CLAM::VM::AudioPlotController::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "CLAM::VM::AudioPlotController" ) )
	return this;
    return SelTimeRegionPlotController::qt_cast( clname );
}

#include <qobjectdefs.h>
#include <qsignalslotimp.h>

// SIGNAL xRulerScaleDiv
void CLAM::VM::AudioPlotController::xRulerScaleDiv( QwtScaleDiv t0 )
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

// SIGNAL yRulerScaleDiv
void CLAM::VM::AudioPlotController::yRulerScaleDiv( QwtScaleDiv t0 )
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

bool CLAM::VM::AudioPlotController::qt_invoke( int _id, QUObject* _o )
{
    return SelTimeRegionPlotController::qt_invoke(_id,_o);
}

bool CLAM::VM::AudioPlotController::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: xRulerScaleDiv((QwtScaleDiv)(*((QwtScaleDiv*)static_QUType_ptr.get(_o+1)))); break;
    case 1: yRulerScaleDiv((QwtScaleDiv)(*((QwtScaleDiv*)static_QUType_ptr.get(_o+1)))); break;
    default:
	return SelTimeRegionPlotController::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool CLAM::VM::AudioPlotController::qt_property( int id, int f, QVariant* v)
{
    return SelTimeRegionPlotController::qt_property( id, f, v);
}

bool CLAM::VM::AudioPlotController::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES

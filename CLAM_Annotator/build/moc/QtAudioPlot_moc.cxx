/****************************************************************************
** CLAM::VM::QtAudioPlot meta object code from reading C++ file 'QtAudioPlot.hxx'
**
** Created: Mon Mar 14 17:56:35 2005
**      by: The Qt MOC ($Id$)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "../../../CLAM/src/Visualization/QTVM/Plots/QtAudioPlot.hxx"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.2.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *CLAM::VM::QtAudioPlot::className() const
{
    return "CLAM::VM::QtAudioPlot";
}

QMetaObject *CLAM::VM::QtAudioPlot::metaObj = 0;
static QMetaObjectCleanUp cleanUp_CLAM__VM__QtAudioPlot( "CLAM::VM::QtAudioPlot", &CLAM::VM::QtAudioPlot::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString CLAM::VM::QtAudioPlot::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "CLAM::VM::QtAudioPlot", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString CLAM::VM::QtAudioPlot::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "CLAM::VM::QtAudioPlot", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* CLAM::VM::QtAudioPlot::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QtPresentation::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ 0, &static_QUType_ptr, "MediaTime", QUParameter::In }
    };
    static const QUMethod slot_0 = {"updateRegion", 1, param_slot_0 };
    static const QMetaData slot_tbl[] = {
	{ "updateRegion(MediaTime)", &slot_0, QMetaData::Protected }
    };
    static const QUParameter param_signal_0[] = {
	{ 0, &static_QUType_ptr, "MediaTime", QUParameter::In }
    };
    static const QUMethod signal_0 = {"regionTime", 1, param_signal_0 };
    static const QMetaData signal_tbl[] = {
	{ "regionTime(MediaTime)", &signal_0, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"CLAM::VM::QtAudioPlot", parentObject,
	slot_tbl, 1,
	signal_tbl, 1,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_CLAM__VM__QtAudioPlot.setMetaObject( metaObj );
    return metaObj;
}

void* CLAM::VM::QtAudioPlot::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "CLAM::VM::QtAudioPlot" ) )
	return this;
    if ( !qstrcmp( clname, "PlayablePlot" ) )
	return (PlayablePlot*)this;
    return QtPresentation::qt_cast( clname );
}

#include <qobjectdefs.h>
#include <qsignalslotimp.h>

// SIGNAL regionTime
void CLAM::VM::QtAudioPlot::regionTime( MediaTime t0 )
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

bool CLAM::VM::QtAudioPlot::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: updateRegion((MediaTime)(*((MediaTime*)static_QUType_ptr.get(_o+1)))); break;
    default:
	return QtPresentation::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool CLAM::VM::QtAudioPlot::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: regionTime((MediaTime)(*((MediaTime*)static_QUType_ptr.get(_o+1)))); break;
    default:
	return QtPresentation::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool CLAM::VM::QtAudioPlot::qt_property( int id, int f, QVariant* v)
{
    return QtPresentation::qt_property( id, f, v);
}

bool CLAM::VM::QtAudioPlot::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES

/****************************************************************************
** CLAM::VM::XRuler meta object code from reading C++ file 'XRuler.hxx'
**
** Created: Mon Mar 14 17:56:45 2005
**      by: The Qt MOC ($Id$)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "../../../CLAM/src/Visualization/QTVM/Widgets/Rulers/XRuler.hxx"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.2.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *CLAM::VM::XRuler::className() const
{
    return "CLAM::VM::XRuler";
}

QMetaObject *CLAM::VM::XRuler::metaObj = 0;
static QMetaObjectCleanUp cleanUp_CLAM__VM__XRuler( "CLAM::VM::XRuler", &CLAM::VM::XRuler::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString CLAM::VM::XRuler::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "CLAM::VM::XRuler", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString CLAM::VM::XRuler::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "CLAM::VM::XRuler", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* CLAM::VM::XRuler::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QwtScale::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ 0, &static_QUType_ptr, "QwtScaleDiv", QUParameter::In }
    };
    static const QUMethod slot_0 = {"SetScaleDiv", 1, param_slot_0 };
    static const QMetaData slot_tbl[] = {
	{ "SetScaleDiv(QwtScaleDiv)", &slot_0, QMetaData::Public }
    };
    static const QUParameter param_signal_0[] = {
	{ 0, &static_QUType_ptr, "QwtScaleDiv", QUParameter::In }
    };
    static const QUMethod signal_0 = {"scaleDiv", 1, param_signal_0 };
    static const QMetaData signal_tbl[] = {
	{ "scaleDiv(QwtScaleDiv)", &signal_0, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"CLAM::VM::XRuler", parentObject,
	slot_tbl, 1,
	signal_tbl, 1,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_CLAM__VM__XRuler.setMetaObject( metaObj );
    return metaObj;
}

void* CLAM::VM::XRuler::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "CLAM::VM::XRuler" ) )
	return this;
    return QwtScale::qt_cast( clname );
}

#include <qobjectdefs.h>
#include <qsignalslotimp.h>

// SIGNAL scaleDiv
void CLAM::VM::XRuler::scaleDiv( QwtScaleDiv t0 )
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

bool CLAM::VM::XRuler::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: SetScaleDiv((QwtScaleDiv)(*((QwtScaleDiv*)static_QUType_ptr.get(_o+1)))); break;
    default:
	return QwtScale::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool CLAM::VM::XRuler::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: scaleDiv((QwtScaleDiv)(*((QwtScaleDiv*)static_QUType_ptr.get(_o+1)))); break;
    default:
	return QwtScale::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool CLAM::VM::XRuler::qt_property( int id, int f, QVariant* v)
{
    return QwtScale::qt_property( id, f, v);
}

bool CLAM::VM::XRuler::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES

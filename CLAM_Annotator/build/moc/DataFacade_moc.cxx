/****************************************************************************
** DataFacade meta object code from reading C++ file 'DataFacade.hxx'
**
** Created: Mon Mar 14 17:56:17 2005
**      by: The Qt MOC ($Id$)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "../../src/common/DataFacade.hxx"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.2.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *DataFacade::className() const
{
    return "DataFacade";
}

QMetaObject *DataFacade::metaObj = 0;
static QMetaObjectCleanUp cleanUp_DataFacade( "DataFacade", &DataFacade::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString DataFacade::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "DataFacade", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString DataFacade::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "DataFacade", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* DataFacade::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QObject::staticMetaObject();
    static const QUMethod signal_0 = {"songEnded", 0, 0 };
    static const QMetaData signal_tbl[] = {
	{ "songEnded()", &signal_0, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"DataFacade", parentObject,
	0, 0,
	signal_tbl, 1,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_DataFacade.setMetaObject( metaObj );
    return metaObj;
}

void* DataFacade::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "DataFacade" ) )
	return this;
    return QObject::qt_cast( clname );
}

// SIGNAL songEnded
void DataFacade::songEnded()
{
    activate_signal( staticMetaObject()->signalOffset() + 0 );
}

bool DataFacade::qt_invoke( int _id, QUObject* _o )
{
    return QObject::qt_invoke(_id,_o);
}

bool DataFacade::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: songEnded(); break;
    default:
	return QObject::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool DataFacade::qt_property( int id, int f, QVariant* v)
{
    return QObject::qt_property( id, f, v);
}

bool DataFacade::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES

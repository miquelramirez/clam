/****************************************************************************
** CLAM::QTConfigurator meta object code from reading C++ file 'QTConfigurator.hxx'
**
** Created: Tue Jul 1 13:05:19 2003
**      by: The Qt MOC ($Id$)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "../../../src/Visualization/View/QTConfigurator.hxx"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.1.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *CLAM::QTConfigurator::className() const
{
    return "CLAM::QTConfigurator";
}

QMetaObject *CLAM::QTConfigurator::metaObj = 0;
static QMetaObjectCleanUp cleanUp_CLAM__QTConfigurator( "CLAM::QTConfigurator", &CLAM::QTConfigurator::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString CLAM::QTConfigurator::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "CLAM::QTConfigurator", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString CLAM::QTConfigurator::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "CLAM::QTConfigurator", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* CLAM::QTConfigurator::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QDialog::staticMetaObject();
    static const QUMethod slot_0 = {"Apply", 0, 0 };
    static const QUMethod slot_1 = {"Discard", 0, 0 };
    static const QUMethod slot_2 = {"Ok", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "Apply()", &slot_0, QMetaData::Public },
	{ "Discard()", &slot_1, QMetaData::Public },
	{ "Ok()", &slot_2, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"CLAM::QTConfigurator", parentObject,
	slot_tbl, 3,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_CLAM__QTConfigurator.setMetaObject( metaObj );
    return metaObj;
}

void* CLAM::QTConfigurator::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "CLAM::QTConfigurator" ) )
	return this;
    return QDialog::qt_cast( clname );
}

bool CLAM::QTConfigurator::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: Apply(); break;
    case 1: Discard(); break;
    case 2: Ok(); break;
    default:
	return QDialog::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool CLAM::QTConfigurator::qt_emit( int _id, QUObject* _o )
{
    return QDialog::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool CLAM::QTConfigurator::qt_property( int id, int f, QVariant* v)
{
    return QDialog::qt_property( id, f, v);
}

bool CLAM::QTConfigurator::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES

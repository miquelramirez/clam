/****************************************************************************
** NewProjectNameBase meta object code from reading C++ file 'NewProjectNameBase.h'
**
** Created: Mon Mar 14 17:57:15 2005
**      by: The Qt MOC ($Id$)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "../uic/NewProjectNameBase.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.2.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *NewProjectNameBase::className() const
{
    return "NewProjectNameBase";
}

QMetaObject *NewProjectNameBase::metaObj = 0;
static QMetaObjectCleanUp cleanUp_NewProjectNameBase( "NewProjectNameBase", &NewProjectNameBase::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString NewProjectNameBase::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "NewProjectNameBase", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString NewProjectNameBase::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "NewProjectNameBase", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* NewProjectNameBase::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QDialog::staticMetaObject();
    static const QUMethod slot_0 = {"okClicked", 0, 0 };
    static const QUMethod slot_1 = {"languageChange", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "okClicked()", &slot_0, QMetaData::Public },
	{ "languageChange()", &slot_1, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"NewProjectNameBase", parentObject,
	slot_tbl, 2,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_NewProjectNameBase.setMetaObject( metaObj );
    return metaObj;
}

void* NewProjectNameBase::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "NewProjectNameBase" ) )
	return this;
    return QDialog::qt_cast( clname );
}

bool NewProjectNameBase::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: okClicked(); break;
    case 1: languageChange(); break;
    default:
	return QDialog::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool NewProjectNameBase::qt_emit( int _id, QUObject* _o )
{
    return QDialog::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool NewProjectNameBase::qt_property( int id, int f, QVariant* v)
{
    return QDialog::qt_property( id, f, v);
}

bool NewProjectNameBase::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES

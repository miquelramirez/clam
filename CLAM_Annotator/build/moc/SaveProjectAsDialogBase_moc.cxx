/****************************************************************************
** SaveProjectAsDialogBase meta object code from reading C++ file 'SaveProjectAsDialogBase.h'
**
** Created: Mon Mar 14 17:57:19 2005
**      by: The Qt MOC ($Id$)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "../uic/SaveProjectAsDialogBase.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.2.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *SaveProjectAsDialogBase::className() const
{
    return "SaveProjectAsDialogBase";
}

QMetaObject *SaveProjectAsDialogBase::metaObj = 0;
static QMetaObjectCleanUp cleanUp_SaveProjectAsDialogBase( "SaveProjectAsDialogBase", &SaveProjectAsDialogBase::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString SaveProjectAsDialogBase::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "SaveProjectAsDialogBase", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString SaveProjectAsDialogBase::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "SaveProjectAsDialogBase", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* SaveProjectAsDialogBase::staticMetaObject()
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
	"SaveProjectAsDialogBase", parentObject,
	slot_tbl, 2,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_SaveProjectAsDialogBase.setMetaObject( metaObj );
    return metaObj;
}

void* SaveProjectAsDialogBase::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "SaveProjectAsDialogBase" ) )
	return this;
    return QDialog::qt_cast( clname );
}

bool SaveProjectAsDialogBase::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: okClicked(); break;
    case 1: languageChange(); break;
    default:
	return QDialog::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool SaveProjectAsDialogBase::qt_emit( int _id, QUObject* _o )
{
    return QDialog::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool SaveProjectAsDialogBase::qt_property( int id, int f, QVariant* v)
{
    return QDialog::qt_property( id, f, v);
}

bool SaveProjectAsDialogBase::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES

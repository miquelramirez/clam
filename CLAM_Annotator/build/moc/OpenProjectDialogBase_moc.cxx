/****************************************************************************
** OpenProjectDialogBase meta object code from reading C++ file 'OpenProjectDialogBase.h'
**
** Created: Mon Mar 14 17:57:11 2005
**      by: The Qt MOC ($Id$)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "../uic/OpenProjectDialogBase.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.2.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *OpenProjectDialogBase::className() const
{
    return "OpenProjectDialogBase";
}

QMetaObject *OpenProjectDialogBase::metaObj = 0;
static QMetaObjectCleanUp cleanUp_OpenProjectDialogBase( "OpenProjectDialogBase", &OpenProjectDialogBase::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString OpenProjectDialogBase::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "OpenProjectDialogBase", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString OpenProjectDialogBase::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "OpenProjectDialogBase", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* OpenProjectDialogBase::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QDialog::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ 0, &static_QUType_ptr, "QListViewItem", QUParameter::In }
    };
    static const QUMethod slot_0 = {"itemClicked", 1, param_slot_0 };
    static const QUMethod slot_1 = {"languageChange", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "itemClicked(QListViewItem*)", &slot_0, QMetaData::Public },
	{ "languageChange()", &slot_1, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"OpenProjectDialogBase", parentObject,
	slot_tbl, 2,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_OpenProjectDialogBase.setMetaObject( metaObj );
    return metaObj;
}

void* OpenProjectDialogBase::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "OpenProjectDialogBase" ) )
	return this;
    return QDialog::qt_cast( clname );
}

bool OpenProjectDialogBase::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: itemClicked((QListViewItem*)static_QUType_ptr.get(_o+1)); break;
    case 1: languageChange(); break;
    default:
	return QDialog::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool OpenProjectDialogBase::qt_emit( int _id, QUObject* _o )
{
    return QDialog::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool OpenProjectDialogBase::qt_property( int id, int f, QVariant* v)
{
    return QDialog::qt_property( id, f, v);
}

bool OpenProjectDialogBase::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES

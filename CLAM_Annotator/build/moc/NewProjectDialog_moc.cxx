/****************************************************************************
** NewProjectDialog meta object code from reading C++ file 'NewProjectDialog.hxx'
**
** Created: Mon Mar 14 17:56:22 2005
**      by: The Qt MOC ($Id$)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "../../src/NewProjectDialog.hxx"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.2.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *NewProjectDialog::className() const
{
    return "NewProjectDialog";
}

QMetaObject *NewProjectDialog::metaObj = 0;
static QMetaObjectCleanUp cleanUp_NewProjectDialog( "NewProjectDialog", &NewProjectDialog::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString NewProjectDialog::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "NewProjectDialog", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString NewProjectDialog::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "NewProjectDialog", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* NewProjectDialog::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = NewProjectDialogBase::staticMetaObject();
    static const QUMethod slot_0 = {"okClicked", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "okClicked()", &slot_0, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"NewProjectDialog", parentObject,
	slot_tbl, 1,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_NewProjectDialog.setMetaObject( metaObj );
    return metaObj;
}

void* NewProjectDialog::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "NewProjectDialog" ) )
	return this;
    return NewProjectDialogBase::qt_cast( clname );
}

bool NewProjectDialog::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: okClicked(); break;
    default:
	return NewProjectDialogBase::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool NewProjectDialog::qt_emit( int _id, QUObject* _o )
{
    return NewProjectDialogBase::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool NewProjectDialog::qt_property( int id, int f, QVariant* v)
{
    return NewProjectDialogBase::qt_property( id, f, v);
}

bool NewProjectDialog::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES

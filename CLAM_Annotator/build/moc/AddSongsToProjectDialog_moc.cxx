/****************************************************************************
** AddSongsToProjectDialog meta object code from reading C++ file 'AddSongsToProjectDialog.hxx'
**
** Created: Mon Mar 14 17:56:25 2005
**      by: The Qt MOC ($Id$)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "../../src/AddSongsToProjectDialog.hxx"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.2.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *AddSongsToProjectDialog::className() const
{
    return "AddSongsToProjectDialog";
}

QMetaObject *AddSongsToProjectDialog::metaObj = 0;
static QMetaObjectCleanUp cleanUp_AddSongsToProjectDialog( "AddSongsToProjectDialog", &AddSongsToProjectDialog::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString AddSongsToProjectDialog::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "AddSongsToProjectDialog", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString AddSongsToProjectDialog::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "AddSongsToProjectDialog", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* AddSongsToProjectDialog::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = NewProjectDialog::staticMetaObject();
    metaObj = QMetaObject::new_metaobject(
	"AddSongsToProjectDialog", parentObject,
	0, 0,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_AddSongsToProjectDialog.setMetaObject( metaObj );
    return metaObj;
}

void* AddSongsToProjectDialog::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "AddSongsToProjectDialog" ) )
	return this;
    return NewProjectDialog::qt_cast( clname );
}

bool AddSongsToProjectDialog::qt_invoke( int _id, QUObject* _o )
{
    return NewProjectDialog::qt_invoke(_id,_o);
}

bool AddSongsToProjectDialog::qt_emit( int _id, QUObject* _o )
{
    return NewProjectDialog::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool AddSongsToProjectDialog::qt_property( int id, int f, QVariant* v)
{
    return NewProjectDialog::qt_property( id, f, v);
}

bool AddSongsToProjectDialog::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES

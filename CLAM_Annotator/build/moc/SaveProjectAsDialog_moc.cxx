/****************************************************************************
** SaveProjectAsDialog meta object code from reading C++ file 'SaveProjectAsDialog.hxx'
**
** Created: Mon Mar 14 17:56:21 2005
**      by: The Qt MOC ($Id$)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "../../src/SaveProjectAsDialog.hxx"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.2.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *SaveProjectAsDialog::className() const
{
    return "SaveProjectAsDialog";
}

QMetaObject *SaveProjectAsDialog::metaObj = 0;
static QMetaObjectCleanUp cleanUp_SaveProjectAsDialog( "SaveProjectAsDialog", &SaveProjectAsDialog::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString SaveProjectAsDialog::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "SaveProjectAsDialog", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString SaveProjectAsDialog::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "SaveProjectAsDialog", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* SaveProjectAsDialog::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = SaveProjectAsDialogBase::staticMetaObject();
    static const QUMethod slot_0 = {"okClicked", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "okClicked()", &slot_0, QMetaData::Public }
    };
    static const QUParameter param_signal_0[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_0 = {"nameToBeSaveEmited", 1, param_signal_0 };
    static const QMetaData signal_tbl[] = {
	{ "nameToBeSaveEmited(const QString&)", &signal_0, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"SaveProjectAsDialog", parentObject,
	slot_tbl, 1,
	signal_tbl, 1,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_SaveProjectAsDialog.setMetaObject( metaObj );
    return metaObj;
}

void* SaveProjectAsDialog::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "SaveProjectAsDialog" ) )
	return this;
    return SaveProjectAsDialogBase::qt_cast( clname );
}

// SIGNAL nameToBeSaveEmited
void SaveProjectAsDialog::nameToBeSaveEmited( const QString& t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 0, t0 );
}

bool SaveProjectAsDialog::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: okClicked(); break;
    default:
	return SaveProjectAsDialogBase::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool SaveProjectAsDialog::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: nameToBeSaveEmited((const QString&)static_QUType_QString.get(_o+1)); break;
    default:
	return SaveProjectAsDialogBase::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool SaveProjectAsDialog::qt_property( int id, int f, QVariant* v)
{
    return SaveProjectAsDialogBase::qt_property( id, f, v);
}

bool SaveProjectAsDialog::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES

/****************************************************************************
** AnnotatorEnterBase meta object code from reading C++ file 'AnnotatorEnterBase.h'
**
** Created: Mon Mar 14 18:33:51 2005
**      by: The Qt MOC ($Id$)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "../uic/AnnotatorEnterBase.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.2.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *AnnotatorEnterBase::className() const
{
    return "AnnotatorEnterBase";
}

QMetaObject *AnnotatorEnterBase::metaObj = 0;
static QMetaObjectCleanUp cleanUp_AnnotatorEnterBase( "AnnotatorEnterBase", &AnnotatorEnterBase::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString AnnotatorEnterBase::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "AnnotatorEnterBase", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString AnnotatorEnterBase::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "AnnotatorEnterBase", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* AnnotatorEnterBase::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QDialog::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ "item", &static_QUType_ptr, "QIconViewItem", QUParameter::In }
    };
    static const QUMethod slot_0 = {"clicked", 1, param_slot_0 };
    static const QUMethod slot_1 = {"handCursor", 0, 0 };
    static const QUMethod slot_2 = {"arrowCursor", 0, 0 };
    static const QUMethod slot_3 = {"languageChange", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "clicked(QIconViewItem*)", &slot_0, QMetaData::Public },
	{ "handCursor()", &slot_1, QMetaData::Public },
	{ "arrowCursor()", &slot_2, QMetaData::Public },
	{ "languageChange()", &slot_3, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"AnnotatorEnterBase", parentObject,
	slot_tbl, 4,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_AnnotatorEnterBase.setMetaObject( metaObj );
    return metaObj;
}

void* AnnotatorEnterBase::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "AnnotatorEnterBase" ) )
	return this;
    return QDialog::qt_cast( clname );
}

bool AnnotatorEnterBase::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: clicked((QIconViewItem*)static_QUType_ptr.get(_o+1)); break;
    case 1: handCursor(); break;
    case 2: arrowCursor(); break;
    case 3: languageChange(); break;
    default:
	return QDialog::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool AnnotatorEnterBase::qt_emit( int _id, QUObject* _o )
{
    return QDialog::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool AnnotatorEnterBase::qt_property( int id, int f, QVariant* v)
{
    return QDialog::qt_property( id, f, v);
}

bool AnnotatorEnterBase::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES

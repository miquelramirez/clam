/****************************************************************************
** SliderWithValue meta object code from reading C++ file 'SliderWithValue.hxx'
**
** Created: Mon Mar 14 17:56:19 2005
**      by: The Qt MOC ($Id$)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "../../src/SliderWithValue.hxx"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.2.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *SliderWithValue::className() const
{
    return "SliderWithValue";
}

QMetaObject *SliderWithValue::metaObj = 0;
static QMetaObjectCleanUp cleanUp_SliderWithValue( "SliderWithValue", &SliderWithValue::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString SliderWithValue::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "SliderWithValue", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString SliderWithValue::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "SliderWithValue", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* SliderWithValue::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QWidget::staticMetaObject();
    metaObj = QMetaObject::new_metaobject(
	"SliderWithValue", parentObject,
	0, 0,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_SliderWithValue.setMetaObject( metaObj );
    return metaObj;
}

void* SliderWithValue::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "SliderWithValue" ) )
	return this;
    return QWidget::qt_cast( clname );
}

bool SliderWithValue::qt_invoke( int _id, QUObject* _o )
{
    return QWidget::qt_invoke(_id,_o);
}

bool SliderWithValue::qt_emit( int _id, QUObject* _o )
{
    return QWidget::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool SliderWithValue::qt_property( int id, int f, QVariant* v)
{
    return QWidget::qt_property( id, f, v);
}

bool SliderWithValue::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES

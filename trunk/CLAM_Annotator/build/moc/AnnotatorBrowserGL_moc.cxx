/****************************************************************************
** AnnotatorBrowserGL meta object code from reading C++ file 'AnnotatorBrowserGL.hxx'
**
** Created: Mon Mar 14 17:56:39 2005
**      by: The Qt MOC ($Id$)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "../../src/AnnotatorBrowserGL/AnnotatorBrowserGL.hxx"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.2.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *AnnotatorBrowserGL::className() const
{
    return "AnnotatorBrowserGL";
}

QMetaObject *AnnotatorBrowserGL::metaObj = 0;
static QMetaObjectCleanUp cleanUp_AnnotatorBrowserGL( "AnnotatorBrowserGL", &AnnotatorBrowserGL::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString AnnotatorBrowserGL::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "AnnotatorBrowserGL", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString AnnotatorBrowserGL::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "AnnotatorBrowserGL", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* AnnotatorBrowserGL::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QGLWidget::staticMetaObject();
    static const QUMethod slot_0 = {"playStop", 0, 0 };
    static const QUMethod slot_1 = {"update", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "playStop()", &slot_0, QMetaData::Public },
	{ "update()", &slot_1, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"AnnotatorBrowserGL", parentObject,
	slot_tbl, 2,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_AnnotatorBrowserGL.setMetaObject( metaObj );
    return metaObj;
}

void* AnnotatorBrowserGL::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "AnnotatorBrowserGL" ) )
	return this;
    return QGLWidget::qt_cast( clname );
}

bool AnnotatorBrowserGL::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: playStop(); break;
    case 1: update(); break;
    default:
	return QGLWidget::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool AnnotatorBrowserGL::qt_emit( int _id, QUObject* _o )
{
    return QGLWidget::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool AnnotatorBrowserGL::qt_property( int id, int f, QVariant* v)
{
    return QGLWidget::qt_property( id, f, v);
}

bool AnnotatorBrowserGL::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES

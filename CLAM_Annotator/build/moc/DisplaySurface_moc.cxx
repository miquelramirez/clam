/****************************************************************************
** CLAM::VM::DisplaySurface meta object code from reading C++ file 'DisplaySurface.hxx'
**
** Created: Mon Mar 14 17:56:43 2005
**      by: The Qt MOC ($Id$)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "../../../CLAM/src/Visualization/QTVM/Surfaces/DisplaySurface.hxx"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.2.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *CLAM::VM::DisplaySurface::className() const
{
    return "CLAM::VM::DisplaySurface";
}

QMetaObject *CLAM::VM::DisplaySurface::metaObj = 0;
static QMetaObjectCleanUp cleanUp_CLAM__VM__DisplaySurface( "CLAM::VM::DisplaySurface", &CLAM::VM::DisplaySurface::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString CLAM::VM::DisplaySurface::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "CLAM::VM::DisplaySurface", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString CLAM::VM::DisplaySurface::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "CLAM::VM::DisplaySurface", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* CLAM::VM::DisplaySurface::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QGLWidget::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ 0, &static_QUType_ptr, "View", QUParameter::In }
    };
    static const QUMethod slot_0 = {"receivedView", 1, param_slot_0 };
    static const QUParameter param_slot_1[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_1 = {"updateToolTip", 1, param_slot_1 };
    static const QUParameter param_slot_2[] = {
	{ 0, &static_QUType_varptr, "\x18", QUParameter::In }
    };
    static const QUMethod slot_2 = {"changeCursor", 1, param_slot_2 };
    static const QMetaData slot_tbl[] = {
	{ "receivedView(View)", &slot_0, QMetaData::Private },
	{ "updateToolTip(QString)", &slot_1, QMetaData::Private },
	{ "changeCursor(QCursor)", &slot_2, QMetaData::Private }
    };
    static const QUMethod signal_0 = {"leavingMouse", 0, 0 };
    static const QMetaData signal_tbl[] = {
	{ "leavingMouse()", &signal_0, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"CLAM::VM::DisplaySurface", parentObject,
	slot_tbl, 3,
	signal_tbl, 1,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_CLAM__VM__DisplaySurface.setMetaObject( metaObj );
    return metaObj;
}

void* CLAM::VM::DisplaySurface::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "CLAM::VM::DisplaySurface" ) )
	return this;
    return QGLWidget::qt_cast( clname );
}

// SIGNAL leavingMouse
void CLAM::VM::DisplaySurface::leavingMouse()
{
    activate_signal( staticMetaObject()->signalOffset() + 0 );
}

bool CLAM::VM::DisplaySurface::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: receivedView((View)(*((View*)static_QUType_ptr.get(_o+1)))); break;
    case 1: updateToolTip((QString)static_QUType_QString.get(_o+1)); break;
    case 2: changeCursor((QCursor)(*((QCursor*)static_QUType_ptr.get(_o+1)))); break;
    default:
	return QGLWidget::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool CLAM::VM::DisplaySurface::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: leavingMouse(); break;
    default:
	return QGLWidget::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool CLAM::VM::DisplaySurface::qt_property( int id, int f, QVariant* v)
{
    return QGLWidget::qt_property( id, f, v);
}

bool CLAM::VM::DisplaySurface::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES

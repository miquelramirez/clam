/****************************************************************************
** CLAM::VM::QtPlayer meta object code from reading C++ file 'QtPlayer.hxx'
**
** Created: Mon Mar 14 17:56:52 2005
**      by: The Qt MOC ($Id$)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "../../../CLAM/src/Visualization/QTVM/Widgets/Playable/QtPlayer.hxx"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.2.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *CLAM::VM::QtPlayer::className() const
{
    return "CLAM::VM::QtPlayer";
}

QMetaObject *CLAM::VM::QtPlayer::metaObj = 0;
static QMetaObjectCleanUp cleanUp_CLAM__VM__QtPlayer( "CLAM::VM::QtPlayer", &CLAM::VM::QtPlayer::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString CLAM::VM::QtPlayer::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "CLAM::VM::QtPlayer", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString CLAM::VM::QtPlayer::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "CLAM::VM::QtPlayer", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* CLAM::VM::QtPlayer::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QWidget::staticMetaObject();
    static const QUMethod slot_0 = {"play", 0, 0 };
    static const QUMethod slot_1 = {"pause", 0, 0 };
    static const QUMethod slot_2 = {"stop", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "play()", &slot_0, QMetaData::Public },
	{ "pause()", &slot_1, QMetaData::Public },
	{ "stop()", &slot_2, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"CLAM::VM::QtPlayer", parentObject,
	slot_tbl, 3,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_CLAM__VM__QtPlayer.setMetaObject( metaObj );
    return metaObj;
}

void* CLAM::VM::QtPlayer::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "CLAM::VM::QtPlayer" ) )
	return this;
    return QWidget::qt_cast( clname );
}

bool CLAM::VM::QtPlayer::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: play(); break;
    case 1: pause(); break;
    case 2: stop(); break;
    default:
	return QWidget::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool CLAM::VM::QtPlayer::qt_emit( int _id, QUObject* _o )
{
    return QWidget::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool CLAM::VM::QtPlayer::qt_property( int id, int f, QVariant* v)
{
    return QWidget::qt_property( id, f, v);
}

bool CLAM::VM::QtPlayer::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES

/****************************************************************************
** CLAM::VM::ScrollBar meta object code from reading C++ file 'ScrollBar.hxx'
**
** Created: Mon Mar 14 17:57:00 2005
**      by: The Qt MOC ($Id$)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "../../../CLAM/src/Visualization/QTVM/Widgets/Scroll/ScrollBar.hxx"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.2.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *CLAM::VM::ScrollBar::className() const
{
    return "CLAM::VM::ScrollBar";
}

QMetaObject *CLAM::VM::ScrollBar::metaObj = 0;
static QMetaObjectCleanUp cleanUp_CLAM__VM__ScrollBar( "CLAM::VM::ScrollBar", &CLAM::VM::ScrollBar::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString CLAM::VM::ScrollBar::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "CLAM::VM::ScrollBar", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString CLAM::VM::ScrollBar::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "CLAM::VM::ScrollBar", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* CLAM::VM::ScrollBar::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QScrollBar::staticMetaObject();
    static const QUMethod signal_0 = {"clicked", 0, 0 };
    static const QUMethod signal_1 = {"released", 0, 0 };
    static const QMetaData signal_tbl[] = {
	{ "clicked()", &signal_0, QMetaData::Private },
	{ "released()", &signal_1, QMetaData::Private }
    };
    metaObj = QMetaObject::new_metaobject(
	"CLAM::VM::ScrollBar", parentObject,
	0, 0,
	signal_tbl, 2,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_CLAM__VM__ScrollBar.setMetaObject( metaObj );
    return metaObj;
}

void* CLAM::VM::ScrollBar::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "CLAM::VM::ScrollBar" ) )
	return this;
    return QScrollBar::qt_cast( clname );
}

// SIGNAL clicked
void CLAM::VM::ScrollBar::clicked()
{
    activate_signal( staticMetaObject()->signalOffset() + 0 );
}

// SIGNAL released
void CLAM::VM::ScrollBar::released()
{
    activate_signal( staticMetaObject()->signalOffset() + 1 );
}

bool CLAM::VM::ScrollBar::qt_invoke( int _id, QUObject* _o )
{
    return QScrollBar::qt_invoke(_id,_o);
}

bool CLAM::VM::ScrollBar::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: clicked(); break;
    case 1: released(); break;
    default:
	return QScrollBar::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool CLAM::VM::ScrollBar::qt_property( int id, int f, QVariant* v)
{
    return QScrollBar::qt_property( id, f, v);
}

bool CLAM::VM::ScrollBar::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES

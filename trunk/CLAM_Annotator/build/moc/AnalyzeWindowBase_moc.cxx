/****************************************************************************
** AnalyzeWindowBase meta object code from reading C++ file 'AnalyzeWindowBase.h'
**
** Created: Mon Mar 14 19:30:21 2005
**      by: The Qt MOC ($Id$)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "../uic/AnalyzeWindowBase.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.2.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *AnalyzeWindowBase::className() const
{
    return "AnalyzeWindowBase";
}

QMetaObject *AnalyzeWindowBase::metaObj = 0;
static QMetaObjectCleanUp cleanUp_AnalyzeWindowBase( "AnalyzeWindowBase", &AnalyzeWindowBase::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString AnalyzeWindowBase::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "AnalyzeWindowBase", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString AnalyzeWindowBase::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "AnalyzeWindowBase", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* AnalyzeWindowBase::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QDialog::staticMetaObject();
    static const QUMethod slot_0 = {"emit_analyze", 0, 0 };
    static const QUMethod slot_1 = {"emit_analyzeAll", 0, 0 };
    static const QUMethod slot_2 = {"close", 0, 0 };
    static const QUMethod slot_3 = {"languageChange", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "emit_analyze()", &slot_0, QMetaData::Public },
	{ "emit_analyzeAll()", &slot_1, QMetaData::Public },
	{ "close()", &slot_2, QMetaData::Public },
	{ "languageChange()", &slot_3, QMetaData::Protected }
    };
    static const QUMethod signal_0 = {"analyze", 0, 0 };
    static const QUMethod signal_1 = {"analyzeAll", 0, 0 };
    static const QMetaData signal_tbl[] = {
	{ "analyze()", &signal_0, QMetaData::Public },
	{ "analyzeAll()", &signal_1, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"AnalyzeWindowBase", parentObject,
	slot_tbl, 4,
	signal_tbl, 2,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_AnalyzeWindowBase.setMetaObject( metaObj );
    return metaObj;
}

void* AnalyzeWindowBase::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "AnalyzeWindowBase" ) )
	return this;
    return QDialog::qt_cast( clname );
}

// SIGNAL analyze
void AnalyzeWindowBase::analyze()
{
    activate_signal( staticMetaObject()->signalOffset() + 0 );
}

// SIGNAL analyzeAll
void AnalyzeWindowBase::analyzeAll()
{
    activate_signal( staticMetaObject()->signalOffset() + 1 );
}

bool AnalyzeWindowBase::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: emit_analyze(); break;
    case 1: emit_analyzeAll(); break;
    case 2: close(); break;
    case 3: languageChange(); break;
    default:
	return QDialog::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool AnalyzeWindowBase::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: analyze(); break;
    case 1: analyzeAll(); break;
    default:
	return QDialog::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool AnalyzeWindowBase::qt_property( int id, int f, QVariant* v)
{
    return QDialog::qt_property( id, f, v);
}

bool AnalyzeWindowBase::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES

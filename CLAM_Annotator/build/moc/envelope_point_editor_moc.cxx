/****************************************************************************
** CLAM::Envelope_Point_Editor meta object code from reading C++ file 'envelope_point_editor.hxx'
**
** Created: Mon Mar 14 17:56:27 2005
**      by: The Qt MOC ($Id$)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "../../../CLAM/src/Visualization/QTVM/Widgets/BPFEditor/envelope_point_editor.hxx"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.2.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *CLAM::Envelope_Point_Editor::className() const
{
    return "CLAM::Envelope_Point_Editor";
}

QMetaObject *CLAM::Envelope_Point_Editor::metaObj = 0;
static QMetaObjectCleanUp cleanUp_CLAM__Envelope_Point_Editor( "CLAM::Envelope_Point_Editor", &CLAM::Envelope_Point_Editor::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString CLAM::Envelope_Point_Editor::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "CLAM::Envelope_Point_Editor", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString CLAM::Envelope_Point_Editor::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "CLAM::Envelope_Point_Editor", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* CLAM::Envelope_Point_Editor::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QWidget::staticMetaObject();
    static const QUMethod signal_0 = {"envelopeChanged", 0, 0 };
    static const QMetaData signal_tbl[] = {
	{ "envelopeChanged()", &signal_0, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"CLAM::Envelope_Point_Editor", parentObject,
	0, 0,
	signal_tbl, 1,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_CLAM__Envelope_Point_Editor.setMetaObject( metaObj );
    return metaObj;
}

void* CLAM::Envelope_Point_Editor::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "CLAM::Envelope_Point_Editor" ) )
	return this;
    return QWidget::qt_cast( clname );
}

// SIGNAL envelopeChanged
void CLAM::Envelope_Point_Editor::envelopeChanged()
{
    activate_signal( staticMetaObject()->signalOffset() + 0 );
}

bool CLAM::Envelope_Point_Editor::qt_invoke( int _id, QUObject* _o )
{
    return QWidget::qt_invoke(_id,_o);
}

bool CLAM::Envelope_Point_Editor::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: envelopeChanged(); break;
    default:
	return QWidget::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool CLAM::Envelope_Point_Editor::qt_property( int id, int f, QVariant* v)
{
    return QWidget::qt_property( id, f, v);
}

bool CLAM::Envelope_Point_Editor::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES

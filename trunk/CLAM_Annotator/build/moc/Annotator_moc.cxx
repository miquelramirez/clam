/****************************************************************************
** Annotator meta object code from reading C++ file 'Annotator.hxx'
**
** Created: Tue Mar 15 10:54:49 2005
**      by: The Qt MOC ($Id$)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "../../src/Annotator.hxx"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.2.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *Annotator::className() const
{
    return "Annotator";
}

QMetaObject *Annotator::metaObj = 0;
static QMetaObjectCleanUp cleanUp_Annotator( "Annotator", &Annotator::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString Annotator::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "Annotator", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString Annotator::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "Annotator", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* Annotator::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = AnnotatorBase::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In },
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_0 = {"descriptorsTableChanged", 2, param_slot_0 };
    static const QUMethod slot_1 = {"fileNew", 0, 0 };
    static const QUMethod slot_2 = {"fileOpen", 0, 0 };
    static const QUMethod slot_3 = {"addSongsToProject", 0, 0 };
    static const QUParameter param_slot_4[] = {
	{ 0, &static_QUType_ptr, "AnnotatorDataFacade::StringList", QUParameter::In }
    };
    static const QUMethod slot_4 = {"addSongs", 1, param_slot_4 };
    static const QUMethod slot_5 = {"deleteSongsFromProject", 0, 0 };
    static const QUMethod slot_6 = {"fileSaveAs", 0, 0 };
    static const QUMethod slot_7 = {"fileSave", 0, 0 };
    static const QUMethod slot_8 = {"fileMenuAboutToShow", 0, 0 };
    static const QUMethod slot_9 = {"editMenuAboutToShow", 0, 0 };
    static const QUMethod slot_10 = {"showAnalyzeWindow", 0, 0 };
    static const QUMethod slot_11 = {"analyze", 0, 0 };
    static const QUMethod slot_12 = {"analyzeAll", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "descriptorsTableChanged(int,int)", &slot_0, QMetaData::Public },
	{ "fileNew()", &slot_1, QMetaData::Public },
	{ "fileOpen()", &slot_2, QMetaData::Public },
	{ "addSongsToProject()", &slot_3, QMetaData::Public },
	{ "addSongs(const AnnotatorDataFacade::StringList&)", &slot_4, QMetaData::Public },
	{ "deleteSongsFromProject()", &slot_5, QMetaData::Public },
	{ "fileSaveAs()", &slot_6, QMetaData::Public },
	{ "fileSave()", &slot_7, QMetaData::Public },
	{ "fileMenuAboutToShow()", &slot_8, QMetaData::Public },
	{ "editMenuAboutToShow()", &slot_9, QMetaData::Public },
	{ "showAnalyzeWindow()", &slot_10, QMetaData::Public },
	{ "analyze()", &slot_11, QMetaData::Public },
	{ "analyzeAll()", &slot_12, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"Annotator", parentObject,
	slot_tbl, 13,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_Annotator.setMetaObject( metaObj );
    return metaObj;
}

void* Annotator::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "Annotator" ) )
	return this;
    return AnnotatorBase::qt_cast( clname );
}

bool Annotator::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: descriptorsTableChanged((int)static_QUType_int.get(_o+1),(int)static_QUType_int.get(_o+2)); break;
    case 1: fileNew(); break;
    case 2: fileOpen(); break;
    case 3: addSongsToProject(); break;
    case 4: addSongs((const AnnotatorDataFacade::StringList&)*((const AnnotatorDataFacade::StringList*)static_QUType_ptr.get(_o+1))); break;
    case 5: deleteSongsFromProject(); break;
    case 6: fileSaveAs(); break;
    case 7: fileSave(); break;
    case 8: fileMenuAboutToShow(); break;
    case 9: editMenuAboutToShow(); break;
    case 10: showAnalyzeWindow(); break;
    case 11: analyze(); break;
    case 12: analyzeAll(); break;
    default:
	return AnnotatorBase::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool Annotator::qt_emit( int _id, QUObject* _o )
{
    return AnnotatorBase::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool Annotator::qt_property( int id, int f, QVariant* v)
{
    return AnnotatorBase::qt_property( id, f, v);
}

bool Annotator::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES

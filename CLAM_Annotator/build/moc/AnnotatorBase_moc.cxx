/****************************************************************************
** AnnotatorBase meta object code from reading C++ file 'AnnotatorBase.h'
**
** Created: Tue Mar 15 09:18:27 2005
**      by: The Qt MOC ($Id$)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "../uic/AnnotatorBase.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.2.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *AnnotatorBase::className() const
{
    return "AnnotatorBase";
}

QMetaObject *AnnotatorBase::metaObj = 0;
static QMetaObjectCleanUp cleanUp_AnnotatorBase( "AnnotatorBase", &AnnotatorBase::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString AnnotatorBase::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "AnnotatorBase", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString AnnotatorBase::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "AnnotatorBase", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* AnnotatorBase::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QMainWindow::staticMetaObject();
    static const QUMethod slot_0 = {"fileNew", 0, 0 };
    static const QUMethod slot_1 = {"fileOpen", 0, 0 };
    static const QUMethod slot_2 = {"fileSave", 0, 0 };
    static const QUMethod slot_3 = {"fileSaveAs", 0, 0 };
    static const QUMethod slot_4 = {"filePrint", 0, 0 };
    static const QUMethod slot_5 = {"fileExit", 0, 0 };
    static const QUMethod slot_6 = {"viewTitle", 0, 0 };
    static const QUMethod slot_7 = {"viewArtist", 0, 0 };
    static const QUMethod slot_8 = {"viewGenre", 0, 0 };
    static const QUMethod slot_9 = {"viewLyrics", 0, 0 };
    static const QUMethod slot_10 = {"viewItem", 0, 0 };
    static const QUMethod slot_11 = {"viewStructureItem", 0, 0 };
    static const QUMethod slot_12 = {"viewIntraSong", 0, 0 };
    static const QUMethod slot_13 = {"viewSongDescriptors", 0, 0 };
    static const QUMethod slot_14 = {"chooseColor", 0, 0 };
    static const QUParameter param_slot_15[] = {
	{ "q", &static_QUType_ptr, "QWidget", QUParameter::In }
    };
    static const QUMethod slot_15 = {"changeViewMenu", 1, param_slot_15 };
    static const QUMethod slot_16 = {"viewIntraSongViewMenu", 0, 0 };
    static const QUParameter param_slot_17[] = {
	{ "a", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_17 = {"AnnotatorBrowserGLgroup_toggled", 1, param_slot_17 };
    static const QUParameter param_slot_18[] = {
	{ "item", &static_QUType_ptr, "QListViewItem", QUParameter::In }
    };
    static const QUMethod slot_18 = {"songsClicked", 1, param_slot_18 };
    static const QUMethod slot_19 = {"addSongsToProject", 0, 0 };
    static const QUMethod slot_20 = {"deleteSongsFromProject", 0, 0 };
    static const QUMethod slot_21 = {"songAnalyzeAction_activated", 0, 0 };
    static const QUMethod slot_22 = {"analyze", 0, 0 };
    static const QUMethod slot_23 = {"analyzeAll", 0, 0 };
    static const QUParameter param_slot_24[] = {
	{ 0, &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_24 = {"AnnotatorBase_usesTextLabelChanged", 1, param_slot_24 };
    static const QUMethod slot_25 = {"languageChange", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "fileNew()", &slot_0, QMetaData::Public },
	{ "fileOpen()", &slot_1, QMetaData::Public },
	{ "fileSave()", &slot_2, QMetaData::Public },
	{ "fileSaveAs()", &slot_3, QMetaData::Public },
	{ "filePrint()", &slot_4, QMetaData::Public },
	{ "fileExit()", &slot_5, QMetaData::Public },
	{ "viewTitle()", &slot_6, QMetaData::Public },
	{ "viewArtist()", &slot_7, QMetaData::Public },
	{ "viewGenre()", &slot_8, QMetaData::Public },
	{ "viewLyrics()", &slot_9, QMetaData::Public },
	{ "viewItem()", &slot_10, QMetaData::Public },
	{ "viewStructureItem()", &slot_11, QMetaData::Public },
	{ "viewIntraSong()", &slot_12, QMetaData::Public },
	{ "viewSongDescriptors()", &slot_13, QMetaData::Public },
	{ "chooseColor()", &slot_14, QMetaData::Public },
	{ "changeViewMenu(QWidget*)", &slot_15, QMetaData::Public },
	{ "viewIntraSongViewMenu()", &slot_16, QMetaData::Public },
	{ "AnnotatorBrowserGLgroup_toggled(bool)", &slot_17, QMetaData::Public },
	{ "songsClicked(QListViewItem*)", &slot_18, QMetaData::Public },
	{ "addSongsToProject()", &slot_19, QMetaData::Public },
	{ "deleteSongsFromProject()", &slot_20, QMetaData::Public },
	{ "songAnalyzeAction_activated()", &slot_21, QMetaData::Public },
	{ "analyze()", &slot_22, QMetaData::Public },
	{ "analyzeAll()", &slot_23, QMetaData::Public },
	{ "AnnotatorBase_usesTextLabelChanged(bool)", &slot_24, QMetaData::Public },
	{ "languageChange()", &slot_25, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"AnnotatorBase", parentObject,
	slot_tbl, 26,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_AnnotatorBase.setMetaObject( metaObj );
    return metaObj;
}

void* AnnotatorBase::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "AnnotatorBase" ) )
	return this;
    return QMainWindow::qt_cast( clname );
}

bool AnnotatorBase::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: fileNew(); break;
    case 1: fileOpen(); break;
    case 2: fileSave(); break;
    case 3: fileSaveAs(); break;
    case 4: filePrint(); break;
    case 5: fileExit(); break;
    case 6: viewTitle(); break;
    case 7: viewArtist(); break;
    case 8: viewGenre(); break;
    case 9: viewLyrics(); break;
    case 10: viewItem(); break;
    case 11: viewStructureItem(); break;
    case 12: viewIntraSong(); break;
    case 13: viewSongDescriptors(); break;
    case 14: chooseColor(); break;
    case 15: changeViewMenu((QWidget*)static_QUType_ptr.get(_o+1)); break;
    case 16: viewIntraSongViewMenu(); break;
    case 17: AnnotatorBrowserGLgroup_toggled((bool)static_QUType_bool.get(_o+1)); break;
    case 18: songsClicked((QListViewItem*)static_QUType_ptr.get(_o+1)); break;
    case 19: addSongsToProject(); break;
    case 20: deleteSongsFromProject(); break;
    case 21: songAnalyzeAction_activated(); break;
    case 22: analyze(); break;
    case 23: analyzeAll(); break;
    case 24: AnnotatorBase_usesTextLabelChanged((bool)static_QUType_bool.get(_o+1)); break;
    case 25: languageChange(); break;
    default:
	return QMainWindow::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool AnnotatorBase::qt_emit( int _id, QUObject* _o )
{
    return QMainWindow::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool AnnotatorBase::qt_property( int id, int f, QVariant* v)
{
    return QMainWindow::qt_property( id, f, v);
}

bool AnnotatorBase::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES

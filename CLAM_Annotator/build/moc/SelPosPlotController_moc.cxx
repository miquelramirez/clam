/****************************************************************************
** CLAM::VM::SelPosPlotController meta object code from reading C++ file 'SelPosPlotController.hxx'
**
** Created: Mon Mar 14 17:56:54 2005
**      by: The Qt MOC ($Id$)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "../../../CLAM/src/Visualization/QTVM/Controllers/SelPosPlotController.hxx"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.2.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *CLAM::VM::SelPosPlotController::className() const
{
    return "CLAM::VM::SelPosPlotController";
}

QMetaObject *CLAM::VM::SelPosPlotController::metaObj = 0;
static QMetaObjectCleanUp cleanUp_CLAM__VM__SelPosPlotController( "CLAM::VM::SelPosPlotController", &CLAM::VM::SelPosPlotController::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString CLAM::VM::SelPosPlotController::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "CLAM::VM::SelPosPlotController", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString CLAM::VM::SelPosPlotController::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "CLAM::VM::SelPosPlotController", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* CLAM::VM::SelPosPlotController::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = SegmentationMarksPlotController::staticMetaObject();
    metaObj = QMetaObject::new_metaobject(
	"CLAM::VM::SelPosPlotController", parentObject,
	0, 0,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_CLAM__VM__SelPosPlotController.setMetaObject( metaObj );
    return metaObj;
}

void* CLAM::VM::SelPosPlotController::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "CLAM::VM::SelPosPlotController" ) )
	return this;
    return SegmentationMarksPlotController::qt_cast( clname );
}

bool CLAM::VM::SelPosPlotController::qt_invoke( int _id, QUObject* _o )
{
    return SegmentationMarksPlotController::qt_invoke(_id,_o);
}

bool CLAM::VM::SelPosPlotController::qt_emit( int _id, QUObject* _o )
{
    return SegmentationMarksPlotController::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool CLAM::VM::SelPosPlotController::qt_property( int id, int f, QVariant* v)
{
    return SegmentationMarksPlotController::qt_property( id, f, v);
}

bool CLAM::VM::SelPosPlotController::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES

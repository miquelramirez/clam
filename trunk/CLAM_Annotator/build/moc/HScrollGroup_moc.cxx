/****************************************************************************
** CLAM::VM::HScrollGroup meta object code from reading C++ file 'HScrollGroup.hxx'
**
** Created: Mon Mar 14 17:56:49 2005
**      by: The Qt MOC ($Id$)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "../../../CLAM/src/Visualization/QTVM/Widgets/Scroll/HScrollGroup.hxx"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.2.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *CLAM::VM::HScrollGroup::className() const
{
    return "CLAM::VM::HScrollGroup";
}

QMetaObject *CLAM::VM::HScrollGroup::metaObj = 0;
static QMetaObjectCleanUp cleanUp_CLAM__VM__HScrollGroup( "CLAM::VM::HScrollGroup", &CLAM::VM::HScrollGroup::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString CLAM::VM::HScrollGroup::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "CLAM::VM::HScrollGroup", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString CLAM::VM::HScrollGroup::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "CLAM::VM::HScrollGroup", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* CLAM::VM::HScrollGroup::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QWidget::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_0 = {"updateZoomRatio", 1, param_slot_0 };
    static const QUParameter param_slot_1[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_1 = {"updateScrollValue", 1, param_slot_1 };
    static const QUParameter param_slot_2[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_2 = {"setMaxScrollValue", 1, param_slot_2 };
    static const QMetaData slot_tbl[] = {
	{ "updateZoomRatio(int)", &slot_0, QMetaData::Public },
	{ "updateScrollValue(int)", &slot_1, QMetaData::Public },
	{ "setMaxScrollValue(int)", &slot_2, QMetaData::Public }
    };
    static const QUMethod signal_0 = {"zoomIn", 0, 0 };
    static const QUMethod signal_1 = {"zoomOut", 0, 0 };
    static const QUParameter param_signal_2[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod signal_2 = {"scrollValueChanged", 1, param_signal_2 };
    static const QUParameter param_signal_3[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod signal_3 = {"zoomRatio", 1, param_signal_3 };
    static const QUParameter param_signal_4[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod signal_4 = {"maxScrollValue", 1, param_signal_4 };
    static const QUParameter param_signal_5[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod signal_5 = {"updatedScrollValue", 1, param_signal_5 };
    static const QUMethod signal_6 = {"scrollBarClicked", 0, 0 };
    static const QUMethod signal_7 = {"scrollBarReleased", 0, 0 };
    static const QMetaData signal_tbl[] = {
	{ "zoomIn()", &signal_0, QMetaData::Public },
	{ "zoomOut()", &signal_1, QMetaData::Public },
	{ "scrollValueChanged(int)", &signal_2, QMetaData::Public },
	{ "zoomRatio(int)", &signal_3, QMetaData::Public },
	{ "maxScrollValue(int)", &signal_4, QMetaData::Public },
	{ "updatedScrollValue(int)", &signal_5, QMetaData::Public },
	{ "scrollBarClicked()", &signal_6, QMetaData::Public },
	{ "scrollBarReleased()", &signal_7, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"CLAM::VM::HScrollGroup", parentObject,
	slot_tbl, 3,
	signal_tbl, 8,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_CLAM__VM__HScrollGroup.setMetaObject( metaObj );
    return metaObj;
}

void* CLAM::VM::HScrollGroup::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "CLAM::VM::HScrollGroup" ) )
	return this;
    return QWidget::qt_cast( clname );
}

// SIGNAL zoomIn
void CLAM::VM::HScrollGroup::zoomIn()
{
    activate_signal( staticMetaObject()->signalOffset() + 0 );
}

// SIGNAL zoomOut
void CLAM::VM::HScrollGroup::zoomOut()
{
    activate_signal( staticMetaObject()->signalOffset() + 1 );
}

// SIGNAL scrollValueChanged
void CLAM::VM::HScrollGroup::scrollValueChanged( int t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 2, t0 );
}

// SIGNAL zoomRatio
void CLAM::VM::HScrollGroup::zoomRatio( int t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 3, t0 );
}

// SIGNAL maxScrollValue
void CLAM::VM::HScrollGroup::maxScrollValue( int t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 4, t0 );
}

// SIGNAL updatedScrollValue
void CLAM::VM::HScrollGroup::updatedScrollValue( int t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 5, t0 );
}

// SIGNAL scrollBarClicked
void CLAM::VM::HScrollGroup::scrollBarClicked()
{
    activate_signal( staticMetaObject()->signalOffset() + 6 );
}

// SIGNAL scrollBarReleased
void CLAM::VM::HScrollGroup::scrollBarReleased()
{
    activate_signal( staticMetaObject()->signalOffset() + 7 );
}

bool CLAM::VM::HScrollGroup::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: updateZoomRatio((int)static_QUType_int.get(_o+1)); break;
    case 1: updateScrollValue((int)static_QUType_int.get(_o+1)); break;
    case 2: setMaxScrollValue((int)static_QUType_int.get(_o+1)); break;
    default:
	return QWidget::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool CLAM::VM::HScrollGroup::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: zoomIn(); break;
    case 1: zoomOut(); break;
    case 2: scrollValueChanged((int)static_QUType_int.get(_o+1)); break;
    case 3: zoomRatio((int)static_QUType_int.get(_o+1)); break;
    case 4: maxScrollValue((int)static_QUType_int.get(_o+1)); break;
    case 5: updatedScrollValue((int)static_QUType_int.get(_o+1)); break;
    case 6: scrollBarClicked(); break;
    case 7: scrollBarReleased(); break;
    default:
	return QWidget::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool CLAM::VM::HScrollGroup::qt_property( int id, int f, QVariant* v)
{
    return QWidget::qt_property( id, f, v);
}

bool CLAM::VM::HScrollGroup::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES

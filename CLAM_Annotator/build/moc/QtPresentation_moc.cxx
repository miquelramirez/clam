/****************************************************************************
** CLAM::VM::QtPresentation meta object code from reading C++ file 'QtPresentation.hxx'
**
** Created: Mon Mar 14 17:56:33 2005
**      by: The Qt MOC ($Id$)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "../../../CLAM/src/Visualization/QTVM/Plots/QtPresentation.hxx"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.2.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *CLAM::VM::QtPresentation::className() const
{
    return "CLAM::VM::QtPresentation";
}

QMetaObject *CLAM::VM::QtPresentation::metaObj = 0;
static QMetaObjectCleanUp cleanUp_CLAM__VM__QtPresentation( "CLAM::VM::QtPresentation", &CLAM::VM::QtPresentation::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString CLAM::VM::QtPresentation::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "CLAM::VM::QtPresentation", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString CLAM::VM::QtPresentation::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "CLAM::VM::QtPresentation", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* CLAM::VM::QtPresentation::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QtPlot::staticMetaObject();
    static const QUMethod slot_0 = {"hZoomIn", 0, 0 };
    static const QUMethod slot_1 = {"hZoomOut", 0, 0 };
    static const QUParameter param_slot_2[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_2 = {"hScrollValue", 1, param_slot_2 };
    static const QUParameter param_slot_3[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_3 = {"receivedHZoomRatio", 1, param_slot_3 };
    static const QUParameter param_slot_4[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_4 = {"setMaxHScroll", 1, param_slot_4 };
    static const QUParameter param_slot_5[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_5 = {"updateHScroll", 1, param_slot_5 };
    static const QUMethod slot_6 = {"vZoomIn", 0, 0 };
    static const QUMethod slot_7 = {"vZoomOut", 0, 0 };
    static const QUParameter param_slot_8[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_8 = {"vScrollValue", 1, param_slot_8 };
    static const QUParameter param_slot_9[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_9 = {"receivedVZoomRatio", 1, param_slot_9 };
    static const QUParameter param_slot_10[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_10 = {"setMaxVScroll", 1, param_slot_10 };
    static const QUParameter param_slot_11[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_11 = {"updateVScroll", 1, param_slot_11 };
    static const QUParameter param_slot_12[] = {
	{ 0, &static_QUType_ptr, "QwtScaleDiv", QUParameter::In }
    };
    static const QUMethod slot_12 = {"setXRulerScaleDiv", 1, param_slot_12 };
    static const QUParameter param_slot_13[] = {
	{ 0, &static_QUType_ptr, "QwtScaleDiv", QUParameter::In }
    };
    static const QUMethod slot_13 = {"setYRulerScaleDiv", 1, param_slot_13 };
    static const QUMethod slot_14 = {"switchColors", 0, 0 };
    static const QUParameter param_slot_15[] = {
	{ 0, &static_QUType_ptr, "TData", QUParameter::In }
    };
    static const QUMethod slot_15 = {"setSelPos", 1, param_slot_15 };
    static const QUParameter param_slot_16[] = {
	{ 0, &static_QUType_ptr, "unsigned", QUParameter::In }
    };
    static const QUMethod slot_16 = {"insertMark", 1, param_slot_16 };
    static const QUParameter param_slot_17[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In },
	{ 0, &static_QUType_ptr, "unsigned", QUParameter::In }
    };
    static const QUMethod slot_17 = {"removeMark", 2, param_slot_17 };
    static const QUParameter param_slot_18[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In },
	{ 0, &static_QUType_ptr, "unsigned", QUParameter::In }
    };
    static const QUMethod slot_18 = {"updateMark", 2, param_slot_18 };
    static const QMetaData slot_tbl[] = {
	{ "hZoomIn()", &slot_0, QMetaData::Public },
	{ "hZoomOut()", &slot_1, QMetaData::Public },
	{ "hScrollValue(int)", &slot_2, QMetaData::Public },
	{ "receivedHZoomRatio(int)", &slot_3, QMetaData::Public },
	{ "setMaxHScroll(int)", &slot_4, QMetaData::Public },
	{ "updateHScroll(int)", &slot_5, QMetaData::Public },
	{ "vZoomIn()", &slot_6, QMetaData::Public },
	{ "vZoomOut()", &slot_7, QMetaData::Public },
	{ "vScrollValue(int)", &slot_8, QMetaData::Public },
	{ "receivedVZoomRatio(int)", &slot_9, QMetaData::Public },
	{ "setMaxVScroll(int)", &slot_10, QMetaData::Public },
	{ "updateVScroll(int)", &slot_11, QMetaData::Public },
	{ "setXRulerScaleDiv(QwtScaleDiv)", &slot_12, QMetaData::Public },
	{ "setYRulerScaleDiv(QwtScaleDiv)", &slot_13, QMetaData::Public },
	{ "switchColors()", &slot_14, QMetaData::Public },
	{ "setSelPos(TData)", &slot_15, QMetaData::Public },
	{ "insertMark(unsigned)", &slot_16, QMetaData::Public },
	{ "removeMark(int,unsigned)", &slot_17, QMetaData::Public },
	{ "updateMark(int,unsigned)", &slot_18, QMetaData::Public }
    };
    static const QUMethod signal_0 = {"HZoomIn", 0, 0 };
    static const QUMethod signal_1 = {"HZoomOut", 0, 0 };
    static const QUParameter param_signal_2[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod signal_2 = {"HScrollValue", 1, param_signal_2 };
    static const QUParameter param_signal_3[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod signal_3 = {"HZoomRatio", 1, param_signal_3 };
    static const QUParameter param_signal_4[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod signal_4 = {"HMaxScroll", 1, param_signal_4 };
    static const QUParameter param_signal_5[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod signal_5 = {"UpdatedHScroll", 1, param_signal_5 };
    static const QUMethod signal_6 = {"HScrollClicked", 0, 0 };
    static const QUMethod signal_7 = {"HScrollReleased", 0, 0 };
    static const QUMethod signal_8 = {"VZoomIn", 0, 0 };
    static const QUMethod signal_9 = {"VZoomOut", 0, 0 };
    static const QUParameter param_signal_10[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod signal_10 = {"VScrollValue", 1, param_signal_10 };
    static const QUParameter param_signal_11[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod signal_11 = {"VZoomRatio", 1, param_signal_11 };
    static const QUParameter param_signal_12[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod signal_12 = {"VMaxScroll", 1, param_signal_12 };
    static const QUParameter param_signal_13[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod signal_13 = {"UpdatedVScroll", 1, param_signal_13 };
    static const QUMethod signal_14 = {"VScrollClicked", 0, 0 };
    static const QUMethod signal_15 = {"VScrollReleased", 0, 0 };
    static const QUParameter param_signal_16[] = {
	{ 0, &static_QUType_ptr, "QwtScaleDiv", QUParameter::In }
    };
    static const QUMethod signal_16 = {"xRulerScaleDiv", 1, param_signal_16 };
    static const QUParameter param_signal_17[] = {
	{ 0, &static_QUType_ptr, "QwtScaleDiv", QUParameter::In }
    };
    static const QUMethod signal_17 = {"yRulerScaleDiv", 1, param_signal_17 };
    static const QUMethod signal_18 = {"switchColorsRequested", 0, 0 };
    static const QUParameter param_signal_19[] = {
	{ 0, &static_QUType_ptr, "TData", QUParameter::In }
    };
    static const QUMethod signal_19 = {"selPos", 1, param_signal_19 };
    static const QUParameter param_signal_20[] = {
	{ 0, &static_QUType_ptr, "unsigned", QUParameter::In }
    };
    static const QUMethod signal_20 = {"insertedMark", 1, param_signal_20 };
    static const QUParameter param_signal_21[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In },
	{ 0, &static_QUType_ptr, "unsigned", QUParameter::In }
    };
    static const QUMethod signal_21 = {"removedMark", 2, param_signal_21 };
    static const QUParameter param_signal_22[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In },
	{ 0, &static_QUType_ptr, "unsigned", QUParameter::In }
    };
    static const QUMethod signal_22 = {"updatedMark", 2, param_signal_22 };
    static const QMetaData signal_tbl[] = {
	{ "HZoomIn()", &signal_0, QMetaData::Public },
	{ "HZoomOut()", &signal_1, QMetaData::Public },
	{ "HScrollValue(int)", &signal_2, QMetaData::Public },
	{ "HZoomRatio(int)", &signal_3, QMetaData::Public },
	{ "HMaxScroll(int)", &signal_4, QMetaData::Public },
	{ "UpdatedHScroll(int)", &signal_5, QMetaData::Public },
	{ "HScrollClicked()", &signal_6, QMetaData::Public },
	{ "HScrollReleased()", &signal_7, QMetaData::Public },
	{ "VZoomIn()", &signal_8, QMetaData::Public },
	{ "VZoomOut()", &signal_9, QMetaData::Public },
	{ "VScrollValue(int)", &signal_10, QMetaData::Public },
	{ "VZoomRatio(int)", &signal_11, QMetaData::Public },
	{ "VMaxScroll(int)", &signal_12, QMetaData::Public },
	{ "UpdatedVScroll(int)", &signal_13, QMetaData::Public },
	{ "VScrollClicked()", &signal_14, QMetaData::Public },
	{ "VScrollReleased()", &signal_15, QMetaData::Public },
	{ "xRulerScaleDiv(QwtScaleDiv)", &signal_16, QMetaData::Public },
	{ "yRulerScaleDiv(QwtScaleDiv)", &signal_17, QMetaData::Public },
	{ "switchColorsRequested()", &signal_18, QMetaData::Public },
	{ "selPos(TData)", &signal_19, QMetaData::Public },
	{ "insertedMark(unsigned)", &signal_20, QMetaData::Public },
	{ "removedMark(int,unsigned)", &signal_21, QMetaData::Public },
	{ "updatedMark(int,unsigned)", &signal_22, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"CLAM::VM::QtPresentation", parentObject,
	slot_tbl, 19,
	signal_tbl, 23,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_CLAM__VM__QtPresentation.setMetaObject( metaObj );
    return metaObj;
}

void* CLAM::VM::QtPresentation::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "CLAM::VM::QtPresentation" ) )
	return this;
    return QtPlot::qt_cast( clname );
}

// SIGNAL HZoomIn
void CLAM::VM::QtPresentation::HZoomIn()
{
    activate_signal( staticMetaObject()->signalOffset() + 0 );
}

// SIGNAL HZoomOut
void CLAM::VM::QtPresentation::HZoomOut()
{
    activate_signal( staticMetaObject()->signalOffset() + 1 );
}

// SIGNAL HScrollValue
void CLAM::VM::QtPresentation::HScrollValue( int t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 2, t0 );
}

// SIGNAL HZoomRatio
void CLAM::VM::QtPresentation::HZoomRatio( int t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 3, t0 );
}

// SIGNAL HMaxScroll
void CLAM::VM::QtPresentation::HMaxScroll( int t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 4, t0 );
}

// SIGNAL UpdatedHScroll
void CLAM::VM::QtPresentation::UpdatedHScroll( int t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 5, t0 );
}

// SIGNAL HScrollClicked
void CLAM::VM::QtPresentation::HScrollClicked()
{
    activate_signal( staticMetaObject()->signalOffset() + 6 );
}

// SIGNAL HScrollReleased
void CLAM::VM::QtPresentation::HScrollReleased()
{
    activate_signal( staticMetaObject()->signalOffset() + 7 );
}

// SIGNAL VZoomIn
void CLAM::VM::QtPresentation::VZoomIn()
{
    activate_signal( staticMetaObject()->signalOffset() + 8 );
}

// SIGNAL VZoomOut
void CLAM::VM::QtPresentation::VZoomOut()
{
    activate_signal( staticMetaObject()->signalOffset() + 9 );
}

// SIGNAL VScrollValue
void CLAM::VM::QtPresentation::VScrollValue( int t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 10, t0 );
}

// SIGNAL VZoomRatio
void CLAM::VM::QtPresentation::VZoomRatio( int t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 11, t0 );
}

// SIGNAL VMaxScroll
void CLAM::VM::QtPresentation::VMaxScroll( int t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 12, t0 );
}

// SIGNAL UpdatedVScroll
void CLAM::VM::QtPresentation::UpdatedVScroll( int t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 13, t0 );
}

// SIGNAL VScrollClicked
void CLAM::VM::QtPresentation::VScrollClicked()
{
    activate_signal( staticMetaObject()->signalOffset() + 14 );
}

// SIGNAL VScrollReleased
void CLAM::VM::QtPresentation::VScrollReleased()
{
    activate_signal( staticMetaObject()->signalOffset() + 15 );
}

#include <qobjectdefs.h>
#include <qsignalslotimp.h>

// SIGNAL xRulerScaleDiv
void CLAM::VM::QtPresentation::xRulerScaleDiv( QwtScaleDiv t0 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 16 );
    if ( !clist )
	return;
    QUObject o[2];
    static_QUType_ptr.set(o+1,&t0);
    activate_signal( clist, o );
}

// SIGNAL yRulerScaleDiv
void CLAM::VM::QtPresentation::yRulerScaleDiv( QwtScaleDiv t0 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 17 );
    if ( !clist )
	return;
    QUObject o[2];
    static_QUType_ptr.set(o+1,&t0);
    activate_signal( clist, o );
}

// SIGNAL switchColorsRequested
void CLAM::VM::QtPresentation::switchColorsRequested()
{
    activate_signal( staticMetaObject()->signalOffset() + 18 );
}

// SIGNAL selPos
void CLAM::VM::QtPresentation::selPos( TData t0 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 19 );
    if ( !clist )
	return;
    QUObject o[2];
    static_QUType_ptr.set(o+1,&t0);
    activate_signal( clist, o );
}

// SIGNAL insertedMark
void CLAM::VM::QtPresentation::insertedMark( unsigned t0 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 20 );
    if ( !clist )
	return;
    QUObject o[2];
    static_QUType_ptr.set(o+1,&t0);
    activate_signal( clist, o );
}

// SIGNAL removedMark
void CLAM::VM::QtPresentation::removedMark( int t0, unsigned t1 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 21 );
    if ( !clist )
	return;
    QUObject o[3];
    static_QUType_int.set(o+1,t0);
    static_QUType_ptr.set(o+2,&t1);
    activate_signal( clist, o );
}

// SIGNAL updatedMark
void CLAM::VM::QtPresentation::updatedMark( int t0, unsigned t1 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 22 );
    if ( !clist )
	return;
    QUObject o[3];
    static_QUType_int.set(o+1,t0);
    static_QUType_ptr.set(o+2,&t1);
    activate_signal( clist, o );
}

bool CLAM::VM::QtPresentation::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: hZoomIn(); break;
    case 1: hZoomOut(); break;
    case 2: hScrollValue((int)static_QUType_int.get(_o+1)); break;
    case 3: receivedHZoomRatio((int)static_QUType_int.get(_o+1)); break;
    case 4: setMaxHScroll((int)static_QUType_int.get(_o+1)); break;
    case 5: updateHScroll((int)static_QUType_int.get(_o+1)); break;
    case 6: vZoomIn(); break;
    case 7: vZoomOut(); break;
    case 8: vScrollValue((int)static_QUType_int.get(_o+1)); break;
    case 9: receivedVZoomRatio((int)static_QUType_int.get(_o+1)); break;
    case 10: setMaxVScroll((int)static_QUType_int.get(_o+1)); break;
    case 11: updateVScroll((int)static_QUType_int.get(_o+1)); break;
    case 12: setXRulerScaleDiv((QwtScaleDiv)(*((QwtScaleDiv*)static_QUType_ptr.get(_o+1)))); break;
    case 13: setYRulerScaleDiv((QwtScaleDiv)(*((QwtScaleDiv*)static_QUType_ptr.get(_o+1)))); break;
    case 14: switchColors(); break;
    case 15: setSelPos((TData)(*((TData*)static_QUType_ptr.get(_o+1)))); break;
    case 16: insertMark((unsigned)(*((unsigned*)static_QUType_ptr.get(_o+1)))); break;
    case 17: removeMark((int)static_QUType_int.get(_o+1),(unsigned)(*((unsigned*)static_QUType_ptr.get(_o+2)))); break;
    case 18: updateMark((int)static_QUType_int.get(_o+1),(unsigned)(*((unsigned*)static_QUType_ptr.get(_o+2)))); break;
    default:
	return QtPlot::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool CLAM::VM::QtPresentation::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: HZoomIn(); break;
    case 1: HZoomOut(); break;
    case 2: HScrollValue((int)static_QUType_int.get(_o+1)); break;
    case 3: HZoomRatio((int)static_QUType_int.get(_o+1)); break;
    case 4: HMaxScroll((int)static_QUType_int.get(_o+1)); break;
    case 5: UpdatedHScroll((int)static_QUType_int.get(_o+1)); break;
    case 6: HScrollClicked(); break;
    case 7: HScrollReleased(); break;
    case 8: VZoomIn(); break;
    case 9: VZoomOut(); break;
    case 10: VScrollValue((int)static_QUType_int.get(_o+1)); break;
    case 11: VZoomRatio((int)static_QUType_int.get(_o+1)); break;
    case 12: VMaxScroll((int)static_QUType_int.get(_o+1)); break;
    case 13: UpdatedVScroll((int)static_QUType_int.get(_o+1)); break;
    case 14: VScrollClicked(); break;
    case 15: VScrollReleased(); break;
    case 16: xRulerScaleDiv((QwtScaleDiv)(*((QwtScaleDiv*)static_QUType_ptr.get(_o+1)))); break;
    case 17: yRulerScaleDiv((QwtScaleDiv)(*((QwtScaleDiv*)static_QUType_ptr.get(_o+1)))); break;
    case 18: switchColorsRequested(); break;
    case 19: selPos((TData)(*((TData*)static_QUType_ptr.get(_o+1)))); break;
    case 20: insertedMark((unsigned)(*((unsigned*)static_QUType_ptr.get(_o+1)))); break;
    case 21: removedMark((int)static_QUType_int.get(_o+1),(unsigned)(*((unsigned*)static_QUType_ptr.get(_o+2)))); break;
    case 22: updatedMark((int)static_QUType_int.get(_o+1),(unsigned)(*((unsigned*)static_QUType_ptr.get(_o+2)))); break;
    default:
	return QtPlot::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool CLAM::VM::QtPresentation::qt_property( int id, int f, QVariant* v)
{
    return QtPlot::qt_property( id, f, v);
}

bool CLAM::VM::QtPresentation::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES

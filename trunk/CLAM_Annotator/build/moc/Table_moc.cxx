/****************************************************************************
** Table meta object code from reading C++ file 'Table.hxx'
**
** Created: Mon Mar 14 17:56:37 2005
**      by: The Qt MOC ($Id$)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "../../src/common/Table.hxx"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.2.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *Table::className() const
{
    return "Table";
}

QMetaObject *Table::metaObj = 0;
static QMetaObjectCleanUp cleanUp_Table( "Table", &Table::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString Table::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "Table", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString Table::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "Table", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* Table::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QTable::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ "col", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_0 = {"columnClicked", 1, param_slot_0 };
    static const QUParameter param_slot_1[] = {
	{ "col1", &static_QUType_int, 0, QUParameter::In },
	{ "col2", &static_QUType_int, 0, QUParameter::In },
	{ "swapHeader", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_1 = {"swapColumns", 3, param_slot_1 };
    static const QMetaData slot_tbl[] = {
	{ "columnClicked(int)", &slot_0, QMetaData::Protected },
	{ "swapColumns(int,int,bool)", &slot_1, QMetaData::Protected }
    };
    static const QUParameter param_signal_0[] = {
	{ "col1", &static_QUType_int, 0, QUParameter::In },
	{ "col2", &static_QUType_int, 0, QUParameter::In },
	{ "swapHeader", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod signal_0 = {"SwapColumns", 3, param_signal_0 };
    static const QMetaData signal_tbl[] = {
	{ "SwapColumns(int,int,bool)", &signal_0, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"Table", parentObject,
	slot_tbl, 2,
	signal_tbl, 1,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_Table.setMetaObject( metaObj );
    return metaObj;
}

void* Table::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "Table" ) )
	return this;
    return QTable::qt_cast( clname );
}

#include <qobjectdefs.h>
#include <qsignalslotimp.h>

// SIGNAL SwapColumns
void Table::SwapColumns( int t0, int t1, bool t2 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 0 );
    if ( !clist )
	return;
    QUObject o[4];
    static_QUType_int.set(o+1,t0);
    static_QUType_int.set(o+2,t1);
    static_QUType_bool.set(o+3,t2);
    activate_signal( clist, o );
}

bool Table::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: columnClicked((int)static_QUType_int.get(_o+1)); break;
    case 1: swapColumns((int)static_QUType_int.get(_o+1),(int)static_QUType_int.get(_o+2),(bool)static_QUType_bool.get(_o+3)); break;
    default:
	return QTable::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool Table::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: SwapColumns((int)static_QUType_int.get(_o+1),(int)static_QUType_int.get(_o+2),(bool)static_QUType_bool.get(_o+3)); break;
    default:
	return QTable::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool Table::qt_property( int id, int f, QVariant* v)
{
    return QTable::qt_property( id, f, v);
}

bool Table::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES

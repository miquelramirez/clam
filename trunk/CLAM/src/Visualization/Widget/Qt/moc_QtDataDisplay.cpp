/****************************************************************************
** CLAMGUI::QtDataDisplay meta object code from reading C++ file 'QtDataDisplay.hxx'
**
** Created: Sun Jun 23 19:42:02 2002
**      by: The Qt MOC ($Id$)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#define Q_MOC_QtDataDisplay
#if !defined(Q_MOC_OUTPUT_REVISION)
#define Q_MOC_OUTPUT_REVISION 8
#elif Q_MOC_OUTPUT_REVISION != 8
#error "Moc format conflict - please regenerate all moc files"
#endif

#include "QtDataDisplay.hxx"
#include <qmetaobject.h>
#include <qapplication.h>

#if defined(Q_SPARCWORKS_FUNCP_BUG)
#define Q_AMPERSAND
#else
#define Q_AMPERSAND &
#endif


const char *CLAMGUI::QtDataDisplay::className() const
{
    return "CLAMGUI::QtDataDisplay";
}

QMetaObject *CLAMGUI::QtDataDisplay::metaObj = 0;

void CLAMGUI::QtDataDisplay::initMetaObject()
{
    if ( metaObj )
	return;
    if ( strcmp(QWidget::className(), "QWidget") != 0 )
	badSuperclassWarning("CLAMGUI::QtDataDisplay","QWidget");
    (void) staticMetaObject();
}

#ifndef QT_NO_TRANSLATION
QString CLAMGUI::QtDataDisplay::tr(const char* s)
{
    return ((QNonBaseApplication*)qApp)->translate("CLAMGUI::QtDataDisplay",s);
}

#endif // QT_NO_TRANSLATION
QMetaObject* CLAMGUI::QtDataDisplay::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    (void) QWidget::staticMetaObject();
#ifndef QT_NO_PROPERTIES
#endif // QT_NO_PROPERTIES
    typedef void(CLAMGUI::QtDataDisplay::*m1_t0)(int);
    typedef void(CLAMGUI::QtDataDisplay::*m1_t1)(int);
    typedef void(CLAMGUI::QtDataDisplay::*m1_t2)(int);
    typedef void(CLAMGUI::QtDataDisplay::*m1_t3)(int);
    typedef void(CLAMGUI::QtDataDisplay::*m1_t4)();
    m1_t0 v1_0 = Q_AMPERSAND CLAMGUI::QtDataDisplay::XAxisZoom;
    m1_t1 v1_1 = Q_AMPERSAND CLAMGUI::QtDataDisplay::YAxisZoom;
    m1_t2 v1_2 = Q_AMPERSAND CLAMGUI::QtDataDisplay::XAxisDisp;
    m1_t3 v1_3 = Q_AMPERSAND CLAMGUI::QtDataDisplay::YAxisDisp;
    m1_t4 v1_4 = Q_AMPERSAND CLAMGUI::QtDataDisplay::SetupDisplay;
    QMetaData *slot_tbl = QMetaObject::new_metadata(5);
    QMetaData::Access *slot_tbl_access = QMetaObject::new_metaaccess(5);
    slot_tbl[0].name = "XAxisZoom(int)";
    slot_tbl[0].ptr = *((QMember*)&v1_0);
    slot_tbl_access[0] = QMetaData::Public;
    slot_tbl[1].name = "YAxisZoom(int)";
    slot_tbl[1].ptr = *((QMember*)&v1_1);
    slot_tbl_access[1] = QMetaData::Public;
    slot_tbl[2].name = "XAxisDisp(int)";
    slot_tbl[2].ptr = *((QMember*)&v1_2);
    slot_tbl_access[2] = QMetaData::Public;
    slot_tbl[3].name = "YAxisDisp(int)";
    slot_tbl[3].ptr = *((QMember*)&v1_3);
    slot_tbl_access[3] = QMetaData::Public;
    slot_tbl[4].name = "SetupDisplay()";
    slot_tbl[4].ptr = *((QMember*)&v1_4);
    slot_tbl_access[4] = QMetaData::Public;
    typedef void(CLAMGUI::QtDataDisplay::*m2_t0)(const Viewport&);
    m2_t0 v2_0 = Q_AMPERSAND CLAMGUI::QtDataDisplay::RefreshPlot;
    QMetaData *signal_tbl = QMetaObject::new_metadata(1);
    signal_tbl[0].name = "RefreshPlot(const Viewport&)";
    signal_tbl[0].ptr = *((QMember*)&v2_0);
    metaObj = QMetaObject::new_metaobject(
	"CLAMGUI::QtDataDisplay", "QWidget",
	slot_tbl, 5,
	signal_tbl, 1,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    metaObj->set_slot_access( slot_tbl_access );
#ifndef QT_NO_PROPERTIES
#endif // QT_NO_PROPERTIES
    return metaObj;
}

#include <qobjectdefs.h>
#include <qsignalslotimp.h>

// SIGNAL RefreshPlot
void CLAMGUI::QtDataDisplay::RefreshPlot( const Viewport& t0 )
{
    // No builtin function for signal parameter type const Viewport&
    QConnectionList *clist = receivers("RefreshPlot(const Viewport&)");
    if ( !clist || signalsBlocked() )
	return;
    typedef void (QObject::*RT0)();
    typedef RT0 *PRT0;
    typedef void (QObject::*RT1)(const Viewport&);
    typedef RT1 *PRT1;
    RT0 r0;
    RT1 r1;
    QConnectionListIt it(*clist);
    QConnection   *c;
    QSenderObject *object;
    while ( (c=it.current()) ) {
	++it;
	object = (QSenderObject*)c->object();
	object->setSender( this );
	switch ( c->numArgs() ) {
	    case 0:
		r0 = *((PRT0)(c->member()));
		(object->*r0)();
		break;
	    case 1:
		r1 = *((PRT1)(c->member()));
		(object->*r1)(t0);
		break;
	}
    }
}

/****************************************************************************
** CLAMGUI::QtAppSignaler meta object code from reading C++ file 'QtAppSignaler.hxx'
**
** Created: Sun Jun 23 20:42:37 2002
**      by: The Qt MOC ($Id$)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#define Q_MOC_QtAppSignaler
#if !defined(Q_MOC_OUTPUT_REVISION)
#define Q_MOC_OUTPUT_REVISION 8
#elif Q_MOC_OUTPUT_REVISION != 8
#error "Moc format conflict - please regenerate all moc files"
#endif

#include "QtAppSignaler.hxx"
#include <qmetaobject.h>
#include <qapplication.h>

#if defined(Q_SPARCWORKS_FUNCP_BUG)
#define Q_AMPERSAND
#else
#define Q_AMPERSAND &
#endif


const char *CLAMGUI::QtAppSignaler::className() const
{
    return "CLAMGUI::QtAppSignaler";
}

QMetaObject *CLAMGUI::QtAppSignaler::metaObj = 0;

void CLAMGUI::QtAppSignaler::initMetaObject()
{
    if ( metaObj )
	return;
    if ( strcmp(QObject::className(), "QObject") != 0 )
	badSuperclassWarning("CLAMGUI::QtAppSignaler","QObject");
    (void) staticMetaObject();
}

#ifndef QT_NO_TRANSLATION
QString CLAMGUI::QtAppSignaler::tr(const char* s)
{
    return ((QNonBaseApplication*)qApp)->translate("CLAMGUI::QtAppSignaler",s);
}

#endif // QT_NO_TRANSLATION
QMetaObject* CLAMGUI::QtAppSignaler::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    (void) QObject::staticMetaObject();
#ifndef QT_NO_PROPERTIES
#endif // QT_NO_PROPERTIES
    typedef void(CLAMGUI::QtAppSignaler::*m1_t0)();
    m1_t0 v1_0 = Q_AMPERSAND CLAMGUI::QtAppSignaler::LastWindowClosed;
    QMetaData *slot_tbl = QMetaObject::new_metadata(1);
    QMetaData::Access *slot_tbl_access = QMetaObject::new_metaaccess(1);
    slot_tbl[0].name = "LastWindowClosed()";
    slot_tbl[0].ptr = *((QMember*)&v1_0);
    slot_tbl_access[0] = QMetaData::Public;
    metaObj = QMetaObject::new_metaobject(
	"CLAMGUI::QtAppSignaler", "QObject",
	slot_tbl, 1,
	0, 0,
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

/****************************************************************************
** CLAMGUI::QtGLPort meta object code from reading C++ file 'QtGLPort.hxx'
**
** Created: Thu Jun 27 08:36:39 2002
**      by: The Qt MOC ($Id$)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#define Q_MOC_QtGLPort
#if !defined(Q_MOC_OUTPUT_REVISION)
#define Q_MOC_OUTPUT_REVISION 8
#elif Q_MOC_OUTPUT_REVISION != 8
#error "Moc format conflict - please regenerate all moc files"
#endif

#include "QtGLPort.hxx"
#include <qmetaobject.h>
#include <qapplication.h>

#if defined(Q_SPARCWORKS_FUNCP_BUG)
#define Q_AMPERSAND
#else
#define Q_AMPERSAND &
#endif


const char *CLAMGUI::QtGLPort::className() const
{
    return "CLAMGUI::QtGLPort";
}

QMetaObject *CLAMGUI::QtGLPort::metaObj = 0;

void CLAMGUI::QtGLPort::initMetaObject()
{
    if ( metaObj )
	return;
    if ( strcmp(QGLWidget::className(), "QGLWidget") != 0 )
	badSuperclassWarning("CLAMGUI::QtGLPort","QGLWidget");
    (void) staticMetaObject();
}

#ifndef QT_NO_TRANSLATION
QString CLAMGUI::QtGLPort::tr(const char* s)
{
    return ((QNonBaseApplication*)qApp)->translate("CLAMGUI::QtGLPort",s);
}

#endif // QT_NO_TRANSLATION
QMetaObject* CLAMGUI::QtGLPort::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    (void) QGLWidget::staticMetaObject();
#ifndef QT_NO_PROPERTIES
#endif // QT_NO_PROPERTIES
    typedef void(CLAMGUI::QtGLPort::*m1_t0)(const Viewport&);
    m1_t0 v1_0 = Q_AMPERSAND CLAMGUI::QtGLPort::ViewportChanged;
    QMetaData *slot_tbl = QMetaObject::new_metadata(1);
    QMetaData::Access *slot_tbl_access = QMetaObject::new_metaaccess(1);
    slot_tbl[0].name = "ViewportChanged(const Viewport&)";
    slot_tbl[0].ptr = *((QMember*)&v1_0);
    slot_tbl_access[0] = QMetaData::Public;
    typedef void(CLAMGUI::QtGLPort::*m2_t0)();
    m2_t0 v2_0 = Q_AMPERSAND CLAMGUI::QtGLPort::ViewportReceived;
    QMetaData *signal_tbl = QMetaObject::new_metadata(1);
    signal_tbl[0].name = "ViewportReceived()";
    signal_tbl[0].ptr = *((QMember*)&v2_0);
    metaObj = QMetaObject::new_metaobject(
	"CLAMGUI::QtGLPort", "QGLWidget",
	slot_tbl, 1,
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

// SIGNAL ViewportReceived
void CLAMGUI::QtGLPort::ViewportReceived()
{
    activate_signal( "ViewportReceived()" );
}

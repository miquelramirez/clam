#include "FocusPropertyFollower.hxx"
#include <QtGui/QApplication>

FocusPropertyFollower::FocusPropertyFollower(QWidget * parent)
	: QLabel(parent)
{
	connect(qApp, SIGNAL(focusChanged(QWidget*,QWidget*)),
		this, SLOT(updateMessageOnFocus()));
}
FocusPropertyFollower::~FocusPropertyFollower() {}
void FocusPropertyFollower::updateMessageOnFocus()
{
	QWidget * focused = QApplication::focusWidget();
	if (not focused) return;
	setText(focused->whatsThis());
}


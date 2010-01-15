#ifndef BoolControlSenderWidget_hxx
#define BoolControlSenderWidget_hxx

#include <CLAM/CLAM_Math.hxx>

#include <CLAM/Processing.hxx>
#include "BoolControlSender.hxx"
#include <QtGui/QWidget>

class QAbstractButton;

class BoolControlSenderWidget : public QWidget
{
	Q_OBJECT
public:
	BoolControlSenderWidget(CLAM::Processing * processing);
	~BoolControlSenderWidget();

private slots:
	void buttonToggled(bool value);

private:
	std::vector<QAbstractButton*> _buttons;
	BoolControlSender * _sender;
	bool _updating;
};

#endif//BoolControlSenderWidget_hxx


#include "BoolControlDisplay.hxx"
//#include "BoolControlPrinter.hxx"
#include <CLAM/InControlBase.hxx>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>

BoolControlDisplay::BoolControlDisplay(CLAM::Processing * processing)
	: _processing(processing)
{
	QHBoxLayout *layout = new QHBoxLayout();
	setLayout(layout);

	unsigned nLabels = _processing->GetNInControls();
	for (unsigned i = 0; i < nLabels; i++)
	{
		CLAM::InControlBase & control = _processing->GetInControl(i);
		QString name = control.GetName().c_str();

		QLabel *label = new QLabel(name);
		layout->addWidget(label);
		_labels.push_back(label);
	}
	startTimer(50);
}

BoolControlDisplay::~BoolControlDisplay()
{
}

void BoolControlDisplay::timerEvent(QTimerEvent *event)
{
	int nLabels = _processing->GetNInControls();

	for (int i = 0; i < nLabels; i++)
	{
		CLAM::InControlBase & control = _processing->GetInControl(i);
		const std::string value=control.GetLastValueAsString();
		_labels[i]->setText(value.c_str());
	}
}


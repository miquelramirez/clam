#include "BoolControlDisplay.hxx"
#include <CLAM/InControlBase.hxx>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include "EmbededWidgets.hxx"

static CLAM::EmbededWidgetCreator <BoolControlDisplay> reg("BoolControlPrinter");

BoolControlDisplay::BoolControlDisplay(CLAM::Processing * processing)
	: _processing(processing)
	, _ledOn(":/icons/images/ledon1.png")
	, _ledOff(":/icons/images/ledoff1.png")
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
		CLAM::InControl<bool> * boolControl = 
			dynamic_cast<CLAM::InControl<bool> * >(&_processing->GetInControl(i));
		if (not boolControl) continue;
		bool value=boolControl->GetLastValue();
		_labels[i]->setPixmap(value?_ledOn:_ledOff);
	}
}


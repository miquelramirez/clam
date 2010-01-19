#include "ControlPrinterWidget.hxx"
#include "ControlPrinter.hxx"
#include <CLAM/InControlBase.hxx>
#include <QtGui/QVBoxLayout>
#include <QtGui/QLabel>

ControlPrinterWidget::ControlPrinterWidget(CLAM::Processing * processing)
	: _processing(processing)
{
	QVBoxLayout *layout = new QVBoxLayout();
	setLayout(layout);

	unsigned nLabels = _processing->GetNInControls();
	for (unsigned i = 0; i < nLabels; i++)
	{
		QLabel * label = new QLabel("100.00");
		label->setText("");
		label->setAlignment(Qt::AlignRight);
		layout->addWidget(label);
		_labels.push_back(label);
	}
	updateLabels();
	startTimer(50);
}

ControlPrinterWidget::~ControlPrinterWidget()
{
}

void ControlPrinterWidget::timerEvent(QTimerEvent *event)
{
	updateLabels();
}
void ControlPrinterWidget::updateLabels()
{
	int nLabels = _processing->GetNInControls();

	for (int i = 0; i < nLabels; i++)
	{
		CLAM::InControlBase & control = _processing->GetInControl(i);
		const std::string value = control.GetLastValueAsString();
		_labels[i]->setText(value.c_str());
	}
}


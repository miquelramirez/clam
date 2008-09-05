#include "ControlPrinterWidget.hxx"
#include <CLAM/InControl.hxx>
#include <QtGui/QGridLayout>
#include <QtGui/QLabel>

ControlPrinterWidget::ControlPrinterWidget(CLAM::Processing * processing)
	: _processing(processing)
{
	QGridLayout *grid = new QGridLayout();
	setLayout(grid);

	unsigned nLabels = _processing->GetNInControls();
	for (unsigned i = 0; i < nLabels; i++)
	{
		CLAM::InControl & control = _processing->GetInControl(i);
		QString name(control.GetName().c_str());

		QLabel *label = new QLabel(name);
		grid->addWidget(label, i, 0, Qt::AlignRight);

		label = new QLabel("100.00");
		label->setText("");
		grid->addWidget(label, i, 1);

		_labels.push_back(label);
	}

	startTimer(50);
}

ControlPrinterWidget::~ControlPrinterWidget()
{
}

void ControlPrinterWidget::timerEvent(QTimerEvent *event)
{
	int nLabels = _processing->GetNInControls();
	for (int i = 0; i < nLabels; i++)
	{
		CLAM::InControl & control = _processing->GetInControl(i);
		CLAM::TControlData value = control.GetLastValue();
		_labels[i]->setText(QString::number(value, 'f', 6));
	}
}


#include "ControlPrinterWidget.hxx"
#include <CLAM/ControlPrinter.hxx>
#include <CLAM/InControlBase.hxx>
#include <QtGui/QVBoxLayout>
#include <QtGui/QLabel>
#include "EmbededWidgets.hxx"

static CLAM::EmbededWidgetCreator <ControlPrinterWidget> reg1("ControlPrinter");
static CLAM::EmbededWidgetCreator <ControlPrinterWidget> reg2("ControlTraceWriter");
static CLAM::EmbededWidgetCreator <ControlPrinterWidget> reg3("ControlPrinterTyped");

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


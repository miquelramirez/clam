#include "BoolControlSenderWidget.hxx"
#include <QtGui/QCheckBox>
#include <QtGui/QHBoxLayout>
#include "QSynthKnob.hxx"
#include "EmbededWidgets.hxx"

static CLAM::EmbededWidgetCreator <BoolControlSenderWidget> reg("BoolControlSender");

BoolControlSenderWidget::BoolControlSenderWidget(CLAM::Processing * processing)
	: _updating(false)
{
	_sender = dynamic_cast<BoolControlSender* >(processing);
	CLAM_ASSERT(_sender, "BoolControlSenderWidget only works "
				"with BoolControlSender processings.");

	const BoolControlSender::Config * config = 
		dynamic_cast<const BoolControlSender::Config *>(&_sender->GetConfig());
	CLAM_ASSERT( config, "Unexpected Configuration type for an BoolControlSender" );

	QHBoxLayout *layout = new QHBoxLayout();
	setLayout(layout);

	unsigned nButtons = _sender->GetNOutControls();
	for (unsigned i = 0; i < nButtons; i++)
	{
		CLAM::OutControlBase & control = _sender->GetOutControl(i);
		QString name = control.GetName().c_str();

		QCheckBox *button = new QCheckBox(name);
		layout->addWidget(button);
		_buttons.push_back(button);
		connect(button, SIGNAL(toggled(bool)), 
				this, SLOT(buttonToggled(bool)));
	}
}

BoolControlSenderWidget::~BoolControlSenderWidget()
{
}

void BoolControlSenderWidget::buttonToggled(bool value)
{
	if (_updating) return;
	for (unsigned i=0; i<_buttons.size(); i++)
	{
		if (sender() != _buttons[i]) continue;
		_updating = true;
		_sender->SendControl(i,value);
		_updating = false;
		return;
	}
}




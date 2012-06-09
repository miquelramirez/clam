#include <qlayout.h>
#include <q3buttongroup.h>
#include <qradiobutton.h>
#include "Qt_SMS_Gender_Selector.hxx"
//Added by qt3to4:
#include <Q3GridLayout>
#include <Q3BoxLayout>
#include <Q3VBoxLayout>

namespace QtSMS
{
	Qt_SMS_Gender_Selector::Qt_SMS_Gender_Selector(QWidget* parent)
		: QWidget(parent)
	{
		Q3ButtonGroup* buttonGroup = new Q3ButtonGroup("Select",this);
		buttonGroup->setRadioButtonExclusive(true);
		buttonGroup->setFont(QFont("Sans",11));

		QRadioButton* m2f = new QRadioButton(buttonGroup);
		QRadioButton* f2m = new QRadioButton(buttonGroup);

		Q3BoxLayout* innerLayout = new Q3VBoxLayout(buttonGroup,10,1);
		innerLayout->addWidget(m2f);
		innerLayout->addWidget(f2m);

		m2f->setText("Change a male voice into a female one");
		f2m->setText("Change a female voice into a male one");

		m2f->setChecked(true);

		// layout
		Q3GridLayout* layout = new Q3GridLayout(this,3,1,10);
		layout->addWidget(buttonGroup,1,0);

		connect(buttonGroup,SIGNAL(clicked(int)),this,SIGNAL(genderChanged(int)));
	}

	Qt_SMS_Gender_Selector::~Qt_SMS_Gender_Selector()
	{
	}
}

// END


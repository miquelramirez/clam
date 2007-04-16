#include <qlayout.h>
#include <qbuttongroup.h>
#include <qradiobutton.h>
#include "Qt_SMS_Gender_Selector.hxx"

namespace QtSMS
{
	Qt_SMS_Gender_Selector::Qt_SMS_Gender_Selector(QWidget* parent)
		: QWidget(parent)
	{
		QButtonGroup* buttonGroup = new QButtonGroup("Select",this);
		buttonGroup->setRadioButtonExclusive(true);
		buttonGroup->setFont(QFont("Sans",11));

		QRadioButton* m2f = new QRadioButton(buttonGroup);
		QRadioButton* f2m = new QRadioButton(buttonGroup);

		QBoxLayout* innerLayout = new QVBoxLayout(buttonGroup,10,1);
		innerLayout->addWidget(m2f);
		innerLayout->addWidget(f2m);

		m2f->setText("Change a male voice into a female one");
		f2m->setText("Change a female voice into a male one");

		m2f->setChecked(true);

		// layout
		QGridLayout* layout = new QGridLayout(this,3,1,10);
		layout->addWidget(buttonGroup,1,0);

		connect(buttonGroup,SIGNAL(clicked(int)),this,SIGNAL(genderChanged(int)));
	}

	Qt_SMS_Gender_Selector::~Qt_SMS_Gender_Selector()
	{
	}
}

// END


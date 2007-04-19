#include <qlayout.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <CLAM/EditTagDialog.hxx>

namespace CLAM
{
    namespace VM
    {
		EditTagDialog::EditTagDialog(const QString& contents, QWidget* parent)
			: QDialog(parent)
		{
			setCaption("Edit tag");
			InitEditTagDialog(contents);
		}

		EditTagDialog::~EditTagDialog()
		{
		}

		QString EditTagDialog::text() const
		{
			return mLineEdit->text();
		}

		void EditTagDialog::InitEditTagDialog(const QString& contents)
		{
			// main layout
			QBoxLayout* mainLayout = new QVBoxLayout(this,5);
			mainLayout->addStretch();

			// top: label and lineedit
			QBoxLayout* topLayout = new QHBoxLayout(mainLayout);
			mLabel = new QLabel(this);
			mLabel->setText("Tag");
			mLineEdit = new QLineEdit(contents,this);
			topLayout->addWidget(mLabel);
			topLayout->addWidget(mLineEdit);

			mainLayout->addStretch();

			// bottom: ok and cancel buttons
			QBoxLayout* bottomLayout = new QHBoxLayout(mainLayout);
			mOkButton = new QPushButton(this);
			mOkButton->setText("Ok");
			mOkButton->setDefault(true);
			mCancelButton = new QPushButton(this);
			mCancelButton->setText("Cancel");
			bottomLayout->addWidget(mOkButton);
			bottomLayout->addStretch();
			bottomLayout->addWidget(mCancelButton);
	    
			// connections
			connect(mOkButton,SIGNAL(clicked()),this,SLOT(accept()));
			connect(mCancelButton,SIGNAL(clicked()),this,SLOT(reject()));
		}
    }
}

// END


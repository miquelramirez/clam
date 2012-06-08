#include <qlayout.h>
#include <qlabel.h>
#include <qtimer.h>
#include <CLAM/WaitMsgIndicator.hxx>
#include <CLAM/QtWaitMessageImpl.hxx>

namespace CLAM
{
	namespace VM
	{
		QtWaitMessageImpl::QtWaitMessageImpl(QWidget * parent)
			: QDialog(parent)
		{
			mLabel = new QLabel(this);
			mIndicator = new WaitMsgIndicator(this);
			mIndicator->setFixedHeight(25);

			QBoxLayout* layout = new QVBoxLayout(this);
			layout->setMargin(15);
			layout->setSpacing(10);

			layout->addWidget(mLabel);
			layout->addWidget(mIndicator);
		}

		QtWaitMessageImpl::~QtWaitMessageImpl()
		{
		}

		void QtWaitMessageImpl::LabelText(const std::string& label)
		{
			mLabel->setText(label.c_str());
		}

		void QtWaitMessageImpl::closeEvent(QCloseEvent* e)
		{
			/* empty body */
		}
	}
}

// END


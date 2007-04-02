#ifndef __EDITTAGDIALOG__
#define __EDITTAGDIALOG__

#include <qdialog.h>

class QLabel;
class QLineEdit;
class QPushButton;

namespace CLAM
{
    namespace VM
    {
		class EditTagDialog : public QDialog
		{
			Q_OBJECT
		public:
			EditTagDialog(const QString& contents, QWidget* parent=0);
			~EditTagDialog();

			QString text() const;

		private:
			QLabel*      mLabel;
			QLineEdit*   mLineEdit;
			QPushButton* mOkButton; 
			QPushButton* mCancelButton;

			void InitEditTagDialog(const QString& contents);
		};
    }
}

#endif


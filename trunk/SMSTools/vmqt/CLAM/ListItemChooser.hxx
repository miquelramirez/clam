#ifndef __LISTITEMCHOOSER__
#define __LISTITEMCHOOSER__

#include <list>
#include <qdialog.h>

class QListView;
class QPushButton;

namespace CLAM
{
	namespace VM
	{
		class ListItemChooser : public QDialog
		{
			Q_OBJECT
		public:
			ListItemChooser(const QString& title, const QString& label, std::list<QString> contents, QWidget* parent=0);
			~ListItemChooser();

			QString selection() const;

		protected:
			void resizeEvent(QResizeEvent* e);

		private:
			QListView*   mListView;
			QPushButton* mOkButton; 
			QPushButton* mCancelButton;

			void InitListItemChooser(const QString& label, std::list<QString>& contents);
		};
	}
}


#endif


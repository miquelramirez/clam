#include <qlayout.h>
#include <qlistview.h>
#include <qpushbutton.h>
#include <CLAM/ListItemChooser.hxx>

namespace CLAM
{
	namespace VM
	{
		ListItemChooser::ListItemChooser(const QString& title, 
										 const QString& label, 
										 std::list<QString> contents, 
										 QWidget* parent)
			: QDialog(parent)
		{
			setCaption(title);
			setGeometry(x(),y(),300,150);
			InitListItemChooser(label,contents);
		}

		ListItemChooser::~ListItemChooser()
		{
		}

		QString ListItemChooser::selection() const
		{
			return mListView->selectedItem()->text(0);
		}

		void ListItemChooser::InitListItemChooser(const QString& label, std::list<QString>& contents)
		{
			// string list
			mListView = new QListView(this);
			mListView->setSelectionMode(QListView::Single);
			mListView->setSorting(-1);
			mListView->addColumn(label);
			mListView->setColumnWidthMode(0,QListView::Manual);
			std::list<QString>::iterator it = contents.begin();
			for(; it != contents.end(); it++)
			{
				QListViewItem* item = new QListViewItem(mListView);
				item->setText(0,(*it));
			}
			mListView->setSelected(mListView->firstChild(),true);

			// buttons
			mOkButton = new QPushButton(this);
			mOkButton->setText("Accept");
			mOkButton->setDefault(true);
			mCancelButton = new QPushButton(this);
			mCancelButton->setText("Cancel");
			
			// layout
			QBoxLayout* rightPanel = new QVBoxLayout;
			rightPanel->addWidget(mOkButton);
			rightPanel->addWidget(mCancelButton);
			rightPanel->addStretch();

			QBoxLayout* mainLayout = new QHBoxLayout(this);
			mainLayout->setMargin(5);
			mainLayout->setSpacing(0);
			mainLayout->addWidget(mListView);
			mainLayout->addLayout(rightPanel);
	    
			// connections
			connect(mOkButton,SIGNAL(clicked()),this,SLOT(accept()));
			connect(mCancelButton,SIGNAL(clicked()),this,SLOT(reject()));
		}

		void ListItemChooser::resizeEvent(QResizeEvent* e)
		{
			QDialog::resizeEvent(e);
			mListView->setColumnWidth(0,mListView->width());
		}
	}
}

// END


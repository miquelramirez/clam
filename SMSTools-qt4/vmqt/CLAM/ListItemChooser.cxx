#include <qlayout.h>
#include <q3listview.h>
#include <qpushbutton.h>
//Added by qt3to4:
#include <QResizeEvent>
#include <Q3BoxLayout>
#include <Q3HBoxLayout>
#include <Q3VBoxLayout>
#include <Q3ListView>
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
			mListView = new Q3ListView(this);
			mListView->setSelectionMode(Q3ListView::Single);
			mListView->setSorting(-1);
			mListView->addColumn(label);
			mListView->setColumnWidthMode(0,Q3ListView::Manual);
			std::list<QString>::iterator it = contents.begin();
			for(; it != contents.end(); it++)
			{
				Q3ListViewItem* item = new Q3ListViewItem(mListView);
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
			Q3BoxLayout* rightPanel = new Q3VBoxLayout;
			rightPanel->addWidget(mOkButton);
			rightPanel->addWidget(mCancelButton);
			rightPanel->addStretch();

			Q3BoxLayout* mainLayout = new Q3HBoxLayout(this);
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


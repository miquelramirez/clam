#include "NewProjectDialog.hxx"
#include "NewProjectName.hxx"
#include "AnnotatorDataFacade.hxx"
#include <qheader.h>
#include <qlistview.h>
#include <qwidget.h>
#include <qcheckbox.h>

#include <iostream>

void NewProjectDialog::removeAllEntries()
{
	QPtrList<QListViewItem> lst;
	QListViewItemIterator it( mFoldersView );
	while ( it.current() ) 
	{
		delete it.current();
	}
}

void NewProjectDialog::addNewItems( AnnotatorDataFacade & data)
{
	QListViewItem * mySongs = new QListViewItem( mFoldersView, "My songs");
	mySongs->setPixmap( 0, QPixmap::fromMimeSource( "stock_open.png" ) );
	AnnotatorDataFacade::StringList stringList ;
	data.getFolders( stringList);
	for (AnnotatorDataFacade::StringList::iterator it = stringList.begin() ; it != stringList.end() ; it++ )
	{
		QListViewItem * item = new QListViewItem( mySongs, *it);
		item->setPixmap( 0, QPixmap::fromMimeSource( "stock_open.png" ) );
		AnnotatorDataFacade::StringList filesList;
		data.getFilesInFolder(*it, filesList);
		for (AnnotatorDataFacade::StringList::iterator itFiles = filesList.begin() ; itFiles != filesList.end() ; itFiles++)
		{
			new QListViewItem( item, QString(itFiles->c_str() ) );
		}
	}
	mySongs->setOpen(true);
	mFoldersView->header()->hide();
}

NewProjectDialog::NewProjectDialog(AnnotatorDataFacade & data, QWidget * parent, const char * name, WFlags f) : NewProjectDialogBase( parent, name, f), mData(data)
{
	removeAllEntries();
	addNewItems( data );
}

bool NewProjectDialog::isAFolder( QListViewItem * item ) const
{
	return item->childCount() > 0;
}

void NewProjectDialog::doSomethingWithTheListOfFiles( AnnotatorDataFacade::StringList * list) const
{
	NewProjectName * newProjectName = new NewProjectName( mData, list, 0, "project name", WDestructiveClose );
	newProjectName->show();
}

bool NewProjectDialog::recursive() const
{
	return mRecursivelyCheckBox->isChecked();
}

void NewProjectDialog::getFilesInTheFolder( AnnotatorDataFacade::StringList & list, QListViewItem * folder ) const
{
	if ( folder->childCount() <= 0 )
		return;
	QListViewItem * item = folder->firstChild() ;
	while ( item ) 
	{
		if ( !isAFolder( item ) )
		{
			list.push_back( std::string( item->text(0).ascii() ));
		}
		else
		{
			if ( recursive() )
				getFilesInTheFolder( list, item );
		}
	item = item->nextSibling();
	}
}

void NewProjectDialog::listOfFiles( AnnotatorDataFacade::StringList & list ) const
{
	QListViewItemIterator it;
	it = QListViewItemIterator( mFoldersView );
	while ( it.current() ) 
	{
		if ( it.current()->isSelected() )
		{
			if ( !isAFolder( it.current() ) )
			{
				list.push_back( std::string( it.current()->text(0).ascii() ));
			}
			else
			{
				getFilesInTheFolder( list, it.current() );
			}
		}
		++it;
	}
	
}

void NewProjectDialog::okClicked()
{
	AnnotatorDataFacade::StringList * list = new AnnotatorDataFacade::StringList;
	listOfFiles( *list );
	doSomethingWithTheListOfFiles( list );
	close();
}


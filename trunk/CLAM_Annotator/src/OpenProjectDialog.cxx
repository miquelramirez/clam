#include "Annotator.hxx"
#include "OpenProjectDialog.hxx"
#include <qlistview.h>
#include <iostream>
#include "AnnotatorDataFacade.hxx"

void OpenProjectDialog::insertItemsFromAnnotatorDataFacadeIntoListView()
{
	AnnotatorDataFacade::StringList listOfProjects;
	mData.getProjects( listOfProjects );
	AnnotatorDataFacade::StringList::iterator it = listOfProjects.begin();
	AnnotatorDataFacade::StringList::iterator itEnd = listOfProjects.end();
	for ( ; it != itEnd ; it++)
	{
		QListViewItem * item = new QListViewItem( mProjectsView, *it);
	}
	mProjectsView->header()->hide();

}

void OpenProjectDialog::removeCurrentListViewItems()
{
	std::vector<QListViewItem *> toBeDeleted;
	QListViewItemIterator itListView( mProjectsView );
	for ( ; itListView.current() ; )
	{
		toBeDeleted.push_back( itListView.current() );
		++itListView;
	}
	std::vector<QListViewItem *>::iterator itToBeDeleted = toBeDeleted.begin();
	std::vector<QListViewItem *>::iterator itToBeDeletedEnd = toBeDeleted.end();
	for ( ; itToBeDeleted != itToBeDeletedEnd  ; itToBeDeleted++)
	{
		delete *itToBeDeleted;
	}
}

OpenProjectDialog::OpenProjectDialog(AnnotatorDataFacade & data, QWidget * parent, const char * name, WFlags f ) : OpenProjectDialogBase (parent, name, f), mData(data)
{
	removeCurrentListViewItems();
	insertItemsFromAnnotatorDataFacadeIntoListView();
}

void OpenProjectDialog::itemClicked( QListViewItem * item )
{
	if (item == 0)
		return;
	AnnotatorDataFacade::StringList listOfFiles;
	mData.getFilesInProject( std::string(item->text(0).ascii() ) , listOfFiles );
	Annotator * annotator = new Annotator(std::string(item->text(0).ascii()), listOfFiles, mData, 0, "Annotator", WDestructiveClose );
	annotator->showMaximized();
	close();
}

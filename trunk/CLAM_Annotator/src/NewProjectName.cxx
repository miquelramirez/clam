#include "NewProjectName.hxx"
#include "Annotator.hxx"

#include <qlineedit.h>

#include <iostream>

NewProjectName::NewProjectName(AnnotatorDataFacade & data,AnnotatorDataFacade::StringList * files, QWidget * parent, const char * name, WFlags f) : NewProjectNameBase(parent, name, f) , mFiles( files ), mData( data )
{
}

void NewProjectName::okClicked()
{
	for (AnnotatorDataFacade::StringList::iterator it = mFiles->begin() ; it != mFiles->end() ; it++)
	{
		mData.loadSongInProject( std::string( mEdit->text().ascii() ), *it);
	}
	Annotator * annotator = new Annotator( std::string( mEdit->text().ascii() ), *mFiles, mData, 0, "annotator", WDestructiveClose);
	annotator->showMaximized();
	delete mFiles;
	close();
}


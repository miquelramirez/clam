#include "AddSongsToProjectDialog.hxx"
#include "Annotator.hxx"

#include <iostream>

AddSongsToProjectDialog::AddSongsToProjectDialog(AnnotatorDataFacade & data, QWidget * parent, const char * name, WFlags f) : NewProjectDialog( data, parent, name, f)
{
	if ( QString("Annotator") != parent->className() )
	{
		std::cout<<"add songs to project dialog was called from some class that is not an Annotator class"<<std::endl;
		std::cout<<" Actually, the name of the class is "<<parent->className()<<std::endl;
	}

}

void AddSongsToProjectDialog::doSomethingWithTheListOfFiles( AnnotatorDataFacade::StringList * list) const
{
	dynamic_cast<Annotator*>(parent())->addSongs(*list);
	delete list;
}


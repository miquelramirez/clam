#include "AnnotatorDataFacade.hxx"
#include "DataFacade.hxx"
#include "TXTSongParser.hxx"
#include "AnnotatorEnter.hxx"
#include "NewProjectDialog.hxx"
#include "OpenProjectDialog.hxx"
#include <qiconview.h>
#include <qcursor.h>

#include <iostream>

AnnotatorEnter::AnnotatorEnter( QWidget * parent, const char * name, WFlags f) : AnnotatorEnterBase( parent, name, f)
{
	mData = 0;
}

void AnnotatorEnter::show()
{
	DataFacade data;
	TXTSongParser songParser( "DataTest/", data );
	mData = new AnnotatorDataFacade( data );
	AnnotatorEnterBase::show();
}

void AnnotatorEnter::handCursor()
{
	setCursor(QCursor(Qt::PointingHandCursor));
}

void AnnotatorEnter::arrowCursor()
{
	setCursor(QCursor(Qt::ArrowCursor));
}

void AnnotatorEnter::clicked( QIconViewItem * item)
{
	if ( item == 0)
		return;
	if (item->text() == tr("Exit") )
	{
		close();
	}
	else if (item->text() == tr("New project") )
	{
		NewProjectDialog * newProjectDialog = new NewProjectDialog( *mData, 0,"new project dialog",WDestructiveClose);	
		newProjectDialog->show();
		close();	
	}
	else if (item->text() == tr("Open project") )
	{
		OpenProjectDialog * dialog = new OpenProjectDialog( *mData, 0, "open project dialog", WDestructiveClose);
		dialog->show();
		close();
	}
	else
	{
		std::cout<<item->text()<<" not implemented"<<std::endl;
	}
}

AnnotatorEnter::~AnnotatorEnter()
{
	delete mData;
}


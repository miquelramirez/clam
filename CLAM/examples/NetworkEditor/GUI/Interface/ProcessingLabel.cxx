
#include "ProcessingLabel.hxx"
#include <iostream>
#include "Oscillator.hxx"
#include "FactoryToolBox.hxx"
#include "Factory.hxx"
#include <qframe.h>
#include <qlineedit.h>

namespace NetworkGUI
{

typedef CLAM::Factory<CLAM::Processing> ProcessingFactory;

ProcessingLabel::ProcessingLabel(   const std::string & name, QWidget *parent )
	: QLabel( parent , "processing label" ),
	  mName( name )
{
	setPalette( QPalette( QColor( 200, 200, 200 )));
	resize(130,30);
	setFont(QFont( "Verdana", 11));
	setFrameStyle( QFrame::Panel | QFrame::Sunken );
	setAlignment( AlignCenter );
	setText(QString(name.c_str()));	
}

ProcessingLabel::~ProcessingLabel()
{
}

void ProcessingLabel::mouseDoubleClickEvent( QMouseEvent *)
{
	std::cout << "creat nou " << mName << std::endl;
	setPalette( QPalette( QColor( 250, 250, 250 )));
	FactoryToolBox* parent = (FactoryToolBox*)parentWidget();

	std::stringstream name;
	name.str("");
	name << mName;
	name << "_" << parent->GetNumProcessings();
	parent->IncrementNumProcessings();
	ProcessingFactory & factory = ProcessingFactory::GetInstance();
	parent->AddNewProcessing.Emit( name.str(), factory.Create(mName) );

}

void ProcessingLabel::mouseReleaseEvent( QMouseEvent *)
{
	setPalette( QPalette( QColor( 200, 200, 200 )));
}

} // namespace NetworkGUI

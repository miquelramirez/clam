
#include "ProcessingTree.hxx"
#include "ProcessingItem.hxx"
#include "MainWindow.hxx"
#include "Factory.hxx"
#include <string>
#include <sstream>
#include <qheader.h>
#include <qdragobject.h> 

namespace NetworkGUI
{

typedef CLAM::Factory<CLAM::Processing> ProcessingFactory;

ProcessingTree::ProcessingTree( Qt_NetworkPresentation & network, QWidget * parent, const char * name  )
	: QListView(parent, name),
	  mNumProc(0),
	  mItemPressed(false),
	  mSelectedItem(0)
{	
	addColumn( "Processing" );
	setRootIsDecorated( TRUE );
	QListViewItem * gen = new QListViewItem( this,"Generators" );
	ProcessingItem * osc = new ProcessingItem( gen, "Oscillator");
	QListViewItem * binops = new QListViewItem( this,"Binary Operations" );
	ProcessingItem * multiplier = new ProcessingItem( binops, "AudioMultiplier" );
	ProcessingItem * adder = new ProcessingItem( binops, "AudioAdder" );
//	ProcessingItem * mixer_2 = new ProcessingItem( binops, "Mixer 2" );
	QListViewItem * analysis = new QListViewItem( this,"Analysis" );
	ProcessingItem * fft_rfftw = new ProcessingItem( analysis, "FFT_rfftw" );
	QListViewItem * inputoutput = new QListViewItem( this,"Input/Output" );
	ProcessingItem * audiofilein = new ProcessingItem( inputoutput, "AudioFileIn" );
	ProcessingItem * audiofileout = new ProcessingItem( inputoutput, "AudioFileOut" );
	ProcessingItem * audioout = new ProcessingItem( inputoutput, "AudioOut" );

	show();
	header()->hide();

	connect( this, SIGNAL( doubleClicked(QListViewItem *) ),
		 this, SLOT( CreateProcessing(QListViewItem *) ));
	connect( this, SIGNAL( pressed(QListViewItem *) ),
		 this, SLOT( PressProcessing(QListViewItem *) ));

	ProcessingCreated.Wrap( this, &ProcessingTree::IncreaseNumProc );

	AddNewProcessing.Connect( network.AddNewProcessing );	
	network.ProcessingCreated.Connect( ProcessingCreated );
}

ProcessingTree::~ProcessingTree()
{
}

void ProcessingTree::IncreaseNumProc()
{
	mNumProc++;
}

void ProcessingTree::CreateProcessing(QListViewItem * item)
{
	if(dynamic_cast<ProcessingItem*>(item)==0)
		return;

	std::string className(item->text(0).ascii());

	std::stringstream name;
	name.str("");
	name << className;
	name << "_" << mNumProc;

	IncreaseNumProc();

	ProcessingFactory & factory = ProcessingFactory::GetInstance();

	AddNewProcessing.Emit( name.str(), factory.Create(className) );
	
}

void ProcessingTree::PressProcessing(QListViewItem * item)
{	
	if(dynamic_cast<ProcessingItem*>(item)==0)
		return;
	
	mItemPressed=true;
	mSelectedItem = item;

	startDrag();
}

void ProcessingTree::contentsMouseReleaseEvent ( QMouseEvent * e )
{
	if (mItemPressed)
	{
		mItemPressed= false;
		mSelectedItem = 0;
	}
}

void ProcessingTree::startDrag()
{
	std::string className(mSelectedItem->text(0).ascii());

	std::stringstream completeName;
	completeName.str("");

	// in the format "classname.concretename", like "Oscillator.Oscillator_3"

	completeName << className << ".";

	completeName << className << "_" << mNumProc;

	QDragObject *d = new QTextDrag( completeName.str() , this );
	d->dragCopy();
}

} // namespace NetworkGUI

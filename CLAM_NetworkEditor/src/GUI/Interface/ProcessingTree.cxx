/*
 * Copyright (c) 2001-2003 MUSIC TECHNOLOGY GROUP (MTG)
 *                         UNIVERSITAT POMPEU FABRA
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

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
	ProcessingItem * osc = new ProcessingItem( gen, "SimpleOscillator");
	ProcessingItem * osc2 = new ProcessingItem( gen, "Oscillator");
	ProcessingItem * adsr = new ProcessingItem( gen, "ADSR");

	QListViewItem * binops = new QListViewItem( this,"Binary Operations" );
	ProcessingItem * multiplier = new ProcessingItem( binops, "AudioMultiplier" );
	ProcessingItem * adder = new ProcessingItem( binops, "AudioAdder" );
	
	QListViewItem * controls = new QListViewItem( this, "Controls" );
	ProcessingItem * autopanner = new ProcessingItem( controls, "AutoPanner" );	
	ProcessingItem * flagcontrol = new ProcessingItem( controls, "FlagControl" );	
	ProcessingItem * random = new ProcessingItem( controls, "Random" );	
	ProcessingItem * oneoverf = new ProcessingItem( controls, "OneOverF" );	
	
	QListViewItem * inputoutput = new QListViewItem( this,"Input/Output" );
	ProcessingItem * audiofilein = new ProcessingItem( inputoutput, "AudioFileIn" );
	ProcessingItem * audiofileout = new ProcessingItem( inputoutput, "AudioFileOut" );
	ProcessingItem * audioout = new ProcessingItem( inputoutput, "AudioOut" );

//	QListViewItem * externals = new QListViewItem( this, "Externals" );
//	ProcessingItem * ladspaloader = new ProcessingItem( externals, "LadspaLoader" );
	show();
	header()->hide();

	connect( this, SIGNAL( doubleClicked(QListViewItem *) ),
		 this, SLOT( CreateProcessing(QListViewItem *) ));
	connect( this, SIGNAL( pressed(QListViewItem *) ),
		 this, SLOT( PressProcessing(QListViewItem *) ));

	SlotProcessingCreated.Wrap( this, &ProcessingTree::IncreaseNumProc );

	SignalAddNewProcessing.Connect( network.SlotAddProcessing );	
	network.SignalProcessingCreated.Connect( SlotProcessingCreated );
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

	SignalAddNewProcessing.Emit( name.str(), factory.Create(className) );
	
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

	QDragObject *d = new QTextDrag( completeName.str().c_str(), this );
	d->dragCopy();
}

} // namespace NetworkGUI

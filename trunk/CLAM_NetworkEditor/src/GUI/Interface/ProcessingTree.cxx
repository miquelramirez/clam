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
	ProcessingItem * squarewave = new ProcessingItem( gen, "SquareWave" );	
	
	QListViewItem * aritops = new QListViewItem( this,"Arithmetic operations" );
	ProcessingItem * multiplier = new ProcessingItem( aritops, "AudioMultiplier" );
	ProcessingItem * adder = new ProcessingItem( aritops, "AudioAdder" );
	ProcessingItem * mixer = new ProcessingItem( aritops, "AudioMixer" );
	
	QListViewItem * controls = new QListViewItem( this, "Controls" );
	ProcessingItem * autopanner = new ProcessingItem( controls, "AutoPanner" );	
	ProcessingItem * flagcontrol = new ProcessingItem( controls, "FlagControl" );	
	ProcessingItem * random = new ProcessingItem( controls, "Random" );	
	ProcessingItem * oneoverf = new ProcessingItem( controls, "OneOverF" );	
	
	QListViewItem * inputoutput = new QListViewItem( this,"Input/Output" );
	ProcessingItem * audioout = new ProcessingItem( inputoutput, "AudioOut" );
	ProcessingItem * audioin = new ProcessingItem( inputoutput, "AudioIn" );
	
	QListViewItem * files = new QListViewItem( this,"Audio File I/O" );
	ProcessingItem * monoaudiofilereader = new ProcessingItem( files, "MonoAudioFileReader" );
	ProcessingItem * monoaudiofilewriter = new ProcessingItem( files, "MonoAudioFileWriter" );
	ProcessingItem * multichannelaudiofilereader = new ProcessingItem( files, "MultiChannelAudioFileReader" );
	ProcessingItem * multichannelaudiofilewriter = new ProcessingItem( files, "MultiChannelAudioFileWriter" );

	QListViewItem * analysis= new QListViewItem( this,"Analysis" );	
	ProcessingItem * fft = new ProcessingItem( analysis, "FFT_rfftw" );	
	ProcessingItem * smsanalysiscore = new ProcessingItem( analysis, "SMSAnalysisCore" );	
	QListViewItem * synthesis = new QListViewItem( this,"Synthesis" );
	ProcessingItem * ifft = new ProcessingItem( synthesis, "IFFT_rfftw" );	
	ProcessingItem * smssynthesis = new ProcessingItem( synthesis, "SMSSynthesis" );	

	QListViewItem * smstrans = new QListViewItem(this, "SMS Transformations");
	ProcessingItem * freqshift = new ProcessingItem( smstrans, "SMSFreqShift" );
	ProcessingItem * smspitchshift = new ProcessingItem( smstrans, "SMSPitchShift" );
	ProcessingItem * smsresidualgain = new ProcessingItem( smstrans, "SMSResidualGain" );
	ProcessingItem * smssinusoidalgain = new ProcessingItem( smstrans, "SMSSinusoidalGain" );
	ProcessingItem * smsspectralshapeshift = new ProcessingItem( smstrans, "SMSSpectralShapeShift" );

		
	QListViewItem * utils = new QListViewItem( this, "Utils" );
#ifndef WIN32
	ProcessingItem * ladspaloader = new ProcessingItem( utils, "LadspaLoader" );
#endif
	ProcessingItem * outcontrolsender = new ProcessingItem( utils, "OutControlSender" );	

	QListViewItem* plots = new QListViewItem(this,"Plots");
	ProcessingItem* audioplotprocessing = new ProcessingItem(plots,"AudioPlotProcessing");
	ProcessingItem* spectrumplotprocessing = new ProcessingItem(plots,"SpectrumPlotProcessing");
	ProcessingItem* peaksplotprocessing = new ProcessingItem(plots, "PeaksPlotProcessing");
	ProcessingItem* fundplotprocessing = new ProcessingItem(plots, "FundPlotProcessing");
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

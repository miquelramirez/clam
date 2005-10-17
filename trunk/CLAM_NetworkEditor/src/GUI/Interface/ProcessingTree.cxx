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
	new ProcessingItem( gen, "SimpleOscillator");
	new ProcessingItem( gen, "Oscillator");
	new ProcessingItem( gen, "ADSR");
	new ProcessingItem( gen, "SquareWave" );	
	new ProcessingItem( gen, "WaveGenerator" );	

	QListViewItem * aritops = new QListViewItem( this,"Arithmetic operations" );
	new ProcessingItem( aritops, "AudioMultiplier" );
	new ProcessingItem( aritops, "AudioAdder" );
	new ProcessingItem( aritops, "AudioMixer" );
	new ProcessingItem( aritops, "SpectralPeakArrayAdder" );
	new ProcessingItem( aritops, "SpectrumAdder" );
	new ProcessingItem( aritops, "SpectrumAdder2" );
	
	QListViewItem * controls = new QListViewItem( this, "Controls" );
	new ProcessingItem( controls, "AutoPanner" );	
	new ProcessingItem( controls, "FlagControl" );	
	new ProcessingItem( controls, "Random" );	
	new ProcessingItem( controls, "OneOverF" );	
#if USE_OSCPACK
	new ProcessingItem( controls, "OSCSender" );	
#endif	

	QListViewItem * externalizers = new QListViewItem( this, "Externalizers" );
	new ProcessingItem( externalizers, "ExternInControl" );
	new ProcessingItem( externalizers, "ExternOutControl" );
	new ProcessingItem( externalizers, "ExternGenerator" );
	new ProcessingItem( externalizers, "ExternSink" );

	QListViewItem * inputoutput = new QListViewItem( this,"Input/Output" );
	new ProcessingItem( inputoutput, "AudioOut" );
//	new ProcessingItem( inputoutput, "AudioIn" );
	
	QListViewItem * midi = new QListViewItem( this,"MIDI" );
	new ProcessingItem( midi, "MIDIKeyboard" );
	new ProcessingItem( midi, "MIDIInControl" );
	new ProcessingItem( midi, "MIDIDispatcher" );

	QListViewItem * files = new QListViewItem( this,"Audio File I/O" );
	new ProcessingItem( files, "MonoAudioFileReader" );
	new ProcessingItem( files, "MonoAudioFileWriter" );
	new ProcessingItem( files, "MultiChannelAudioFileReader" );
	new ProcessingItem( files, "MultiChannelAudioFileWriter" );

	QListViewItem * analysis = new QListViewItem( this,"Analysis" );	
	new ProcessingItem( analysis, "FFT_rfftw" );	
	new ProcessingItem( analysis, "SMSAnalysisCore" );	

	QListViewItem * synthesis = new QListViewItem( this,"Synthesis" );
	new ProcessingItem( synthesis, "IFFT_rfftw" );	
	new ProcessingItem( synthesis, "SMSSynthesis" );	

	QListViewItem * smstrans = new QListViewItem(this, "SMS Transformations");
	new ProcessingItem( smstrans, "SMSFreqShift" );
	new ProcessingItem( smstrans, "SMSPitchShift" );
	new ProcessingItem( smstrans, "SMSResidualGain" );
	new ProcessingItem( smstrans, "SMSSinusoidalGain" );
	new ProcessingItem( smstrans, "SMSSineFilter" );
	new ProcessingItem( smstrans, "SMSOddEvenHarmonicRatio" );
	new ProcessingItem( smstrans, "SMSSpectralShapeShift" );
	new ProcessingItem( smstrans, "SMSPitchDiscretization" );
	new ProcessingItem( smstrans, "SMSGenderChange" );

		
	QListViewItem * utils = new QListViewItem( this, "Utils" );
#ifndef WIN32
	new ProcessingItem( utils, "LadspaLoader" );
#endif
	new ProcessingItem( utils, "OutControlSender" );	
	new ProcessingItem( utils, "Fundamental2Control" );	
	new ProcessingItem( utils, "ControlPrinter" );	
	new ProcessingItem( utils, "ControlScaler" );	
	new ProcessingItem( utils, "ControlMapper" );	

	QListViewItem* winplots = new QListViewItem(this,"Plots in new windows");
	new ProcessingItem(winplots, "AudioPlotProcessing");
	new ProcessingItem(winplots, "SpectrumPlotProcessing");
	new ProcessingItem(winplots, "PeaksPlotProcessing");
	new ProcessingItem(winplots, "FundPlotProcessing");
	new ProcessingItem(winplots, "SinTracksPlotProcessing");
	new ProcessingItem(winplots, "FundTrackPlotProcessing");
	new ProcessingItem(winplots, "SpecgramPlotProcessing");
	new ProcessingItem(winplots, "AudioBuffPlotProcessing");

	QListViewItem* canvasplots = new QListViewItem(this,"Plots in canvas");
	new ProcessingItem(canvasplots,"AudioPortMonitor");
	new ProcessingItem(canvasplots,"SpectrumPortMonitor");
	new ProcessingItem(canvasplots,"PeaksPortMonitor");
	new ProcessingItem(canvasplots,"FundamentalPortMonitor");
	new ProcessingItem(canvasplots,"AudioBuffPortMonitor");
	new ProcessingItem(canvasplots,"SpecgramPortMonitor");
	new ProcessingItem(canvasplots,"FundTrackPortMonitor");
	new ProcessingItem(canvasplots,"SinTracksPortMonitor");
	
	QListViewItem* icaRelated = new QListViewItem(this,"ICA");
	new ProcessingItem(icaRelated,"ICA");
	new ProcessingItem(icaRelated,"CrossMixer");
	new ProcessingItem(icaRelated,"AmariError");
	
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
	QDragObject *d = new QTextDrag( className.c_str(), this );
	d->dragCopy();
}

} // namespace NetworkGUI

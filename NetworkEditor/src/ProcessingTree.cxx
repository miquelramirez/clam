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
#include <QtGui/QMouseEvent>
#include <QtGui/QHeaderView>

#include <iostream> //TODO remnovemove to extracted cxx
#include <ladspa.h> //TODO move to extracted cxx
#include <dlfcn.h> //TODO move as well

namespace NetworkGUI
{
const static char * processingClasses[] = {
	"Audio Input/Output",
	"AudioSource",
	"AudioSink",
#ifndef WIN32
	"AudioOut",
#endif
//	"AudioIn",
	0,
	"Audio File I/O",
	"MonoAudioFileReader",
	"MonoAudioFileWriter",
	"MultiChannelAudioFileReader",
	"MultiChannelAudioFileWriter",
	0,
	"Generators",
	"SimpleOscillator",
	"Oscillator",
	"ADSR",
	"SquareWave",	
//	"WaveGenerator",	TODO register
	0,
	"Arithmetic operations",
	"AudioMultiplier",
	"AudioAdder",
	"AudioMixer",
//	"SpectralPeakArrayAdder", TODO register
//	"SpectrumAdder", TODO register
//	"SpectrumAdder2", TODO register
	0,
	"Controls",
	"OutControlSender",
	"ControlSource",
	"ControlSink",
	"ControlPrinter",
	"ControlScaler", 
	"AutoPanner",
	"FlagControl",
	"Random",
	"OneOverF",
#if USE_OSCPACK
	"OSCSender",
#endif	
	0,
	"MIDI",
//	"MIDIKeyboard",
//	"MIDIInControl",
	"MIDIDispatcher",
	0,
	"Analysis",
	"FFT",
	"FFT_rfftw",
	"FFT_fftw3",
	"SpectralAnalysis",
	"SMSAnalysisCore",
	"TonalAnalysis",
	0,
	"Synthesis",
	"IFFT_rfftw",
	"SpectralSynthesis",
	"OverlapAdd",
	"SMSSynthesis",
	0,
	"SMS Transformations",
	"SMSFreqShift",
	"SMSPitchShift",
	"SMSResidualGain",
	"SMSSinusoidalGain",
	"SMSSineFilter",
	"SMSOddEvenHarmonicRatio",
	"SMSSpectralShapeShift",
	"SMSPitchDiscretization",
	"SMSGenderChange",
	"SMSTranspose",
	0,
	"Spectral Transformations",
	"OscillatingSpectralNotch",
	"SpectralDelay",
	"Vocoder",
	"ThreeBandFilter",
	"Robotization",
	"SampleAndHold",
	"ThreeBandCompressor",
	"Peakalizer",
	"ThreeBandAM",
//	"Deeser",
	0,	
	"Utils",
#ifndef WIN32
//	"LadspaLoader", // TODO: Not working because lack of custom configuration
#endif
	"Fundamental2Control",
//	"ControlMapper" //	TODO register
	0,
	"Graphical Monitors",
	"Vumeter",
	"Oscilloscope",
	"SpectrumView",
	"PeakView",
	"Tonnetz",
	"KeySpace",
	"PolarChromaPeaks",
	"ChordRanking",
//	"Tunner", // Commented until the next release
	0,
	0
};

ProcessingTree::ProcessingTree( QWidget * parent)
	: QTreeWidget(parent)
{
	setColumnCount(1);
	setHeaderLabels(QStringList() << tr("Processings toolbox"));
	setRootIsDecorated( true );
	header()->hide();

	//TODO refactor two fors in one ?
	for (unsigned i=0; processingClasses[i]; i++)
	{
		QTreeWidgetItem * group = new QTreeWidgetItem( this, QStringList() << processingClasses[i] );
		i++;
		for (; processingClasses[i]; i++)
		{
			QTreeWidgetItem * item = new QTreeWidgetItem( group, QStringList() << processingClasses[i]);
			item->setIcon(0, QIcon(":/icons/images/processing.png"));
		}
	}
	
/** DUMMY
 * process the LADSPA plugins and add them to the QTreeWidget
 **/
	LadspaPlugins plugins = SearchLadspaPlugins();
	LadspaPlugins::const_iterator it=plugins.begin();
	QTreeWidgetItem * ladspaTree = new QTreeWidgetItem( this, QStringList() << "LADSPA (dummy)" );
	for (; it != plugins.end(); it++)
	{
		QTreeWidgetItem * group = new QTreeWidgetItem( ladspaTree, QStringList() << it->c_str() );
		it++;
		for (; *it != ""; it++)
		{
			QTreeWidgetItem * item = new QTreeWidgetItem( group, QStringList() << it->c_str() );
			item->setIcon(0, QIcon(":/icons/images/processing.png"));
		}
		
	}
 
	
	connect( this, SIGNAL( itemPressed(QTreeWidgetItem *,int) ),
		 this, SLOT( PressProcessing(QTreeWidgetItem *,int) ));
}

/** DUMB: easy way to generate a new tree 
 * follow structure of ProcessingClasses 
 * This will end up being a LADSPA loader
 **/
ProcessingTree::LadspaPlugins ProcessingTree::SearchLadspaPlugins() 
{
//TODO extract typedef of return type
	LadspaPlugins result;
	result.push_back("Filters");
	result.push_back("LowPass");
	result.push_back("HighPass");
	result.push_back("");
	result.push_back("Amplifier");
	result.push_back("Mono");
	result.push_back("Stereo");
	result.push_back("");
	result.push_back("Oscillators");
	result.push_back("Simple");
	result.push_back("Sine");
	result.push_back("");

	LADSPA_Descriptor_Function descriptorTable = 0;
	void* handle = dlopen( "/usr/lib/ladspa/caps.so", RTLD_NOW);
	descriptorTable = (LADSPA_Descriptor_Function)dlsym(handle, "ladspa_descriptor");
	for (unsigned long i=0; descriptorTable(i); i++)
	{
		std::cout << i << std::endl;
		LADSPA_Descriptor* descriptor = (LADSPA_Descriptor*)descriptorTable(i);
		result.push_back(descriptor->Name);
	}	
	result.push_back("");

	return result;
	
}

ProcessingTree::~ProcessingTree()
{
}

void ProcessingTree::PressProcessing(QTreeWidgetItem * item, int column)
{	
	if (!item) return;
	if (!item->parent()) return;

	QString className = item->text(0);
	QDrag *drag = new QDrag( this);
	QMimeData * data = new QMimeData;
	data->setText(className);
	drag->setMimeData(data);
	drag->setPixmap(QPixmap(":/icons/images/processing.png"));
	Qt::DropAction dropAction = drag->start();
}


} // namespace NetworkGUI

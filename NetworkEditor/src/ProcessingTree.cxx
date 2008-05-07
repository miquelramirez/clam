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
#include <iostream> 

#include <CLAM/ProcessingFactory.hxx> 


namespace NetworkGUI
{
/*const static char * processingClasses[] = {
	"Audio Input/Output",
	"AudioSource",
	"AudioSink",
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
	"WindowGenerator",
	"HarmonicPeaksGenerator",
//	"WaveGenerator",	TODO register
	0,
	"Arithmetic operations",
	"AudioMultiplier",
	"AudioAdder",
	"AudioMixer",
//	"SpectralPeakArrayAdder", TODO register
//	"SpectrumAdder", TODO register
//	"SpectrumAdder2", TODO register
	"SpectrumSubstracter2",
	"SpectrumProduct",
	0,
	"Controls",
	"OutControlSender",
	"ControlSurface",
	"ControlSource",
	"ControlSink",
	"ControlPrinter",
	"ControlTraceReader",
	"ControlTraceWriter",
	"ControlScaler", 
	"AutoPanner",
	"FlagControl",
	"Random",
	"OneOverF",
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
	"FFT_ooura",
	"FFT_numrec",
	"AudioWindowing",
	"SpectralAnalysis",
	"SMSAnalysisCore",
	"LPC_AutoCorrelation",
	"TonalAnalysis",
	"MelFilterBank",
	"CepstralTransform",
	0,
	"Synthesis",
	"IFFT",
	"IFFT_rfftw",
	"IFFT_fftw3",
	"IFFT_ooura",
	"OverlapAdd",
	"SpectralSynthesis",
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
	"SMSHarmonizer",
	"SMSMorph",
	"SMSTimeStretch",
	"SMSTranspose",
	"VowelResonator",
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
	"Fundamental2Control",
//	"ControlMapper" //	TODO register
	0,
	"Graphical Monitors",
	"Vumeter",
	"Oscilloscope",
	"BufferOscilloscope",
	"SpectrumView",
	"PeakView",
	"Tonnetz",
	"KeySpace",
	"Spectrogram",
	"PolarChromaPeaks",
	"ChordRanking",
	"LPModelView",
	"MelSpectrumView",
	"MelCepstrumView",
	"VectorView",
	"Tunner",
	0,
	"Usage Examples",
	"FilenameUsageExample",
	0,
	"Sample by Sample (Experimental)",
	"Data2Audio",
	"AudioStreamer",
	"DataAdder",
	"DataMultiplier",
	"DataGain",
	"DataSink",
	"DataDelay",
	"SilentSource",
	0,
	"Ebow Synthesizer (Experimental - GSoC)",
	"AudioDatabaseReader",
	"SDIFDatabaseProcessing",
	0,
	0
};*/

ProcessingTree::ProcessingTree( QWidget * parent)
	: QTreeWidget(parent)
{
	setColumnCount(1);
	setHeaderLabels(QStringList() << tr("Processings toolbox"));
	setRootIsDecorated( true );
	header()->hide();

	CLAM::ProcessingFactory & factory = CLAM::ProcessingFactory::GetInstance();
	CLAM::ProcessingFactory::Values categories = factory.GetSetOfValues("category");
	CLAM::ProcessingFactory::Values::const_iterator itCategory;

	for(itCategory = categories.begin(); itCategory != categories.end(); itCategory++)
	{
		CLAM::ProcessingFactory::Keys keys = factory.GetKeys("category", *itCategory);
		//std::cout << "Category: " << *itCategory << std::endl;
		if( keys.size() == 0 ) continue;
		CLAM::ProcessingFactory::Keys::const_iterator itKey;
		std::string category = *itCategory;
		QTreeWidgetItem * categoryTree = new QTreeWidgetItem( this, QStringList() << category.c_str());
		for(itKey = keys.begin(); itKey != keys.end(); itKey++)
		{
			std::string key = *itKey;

			std::string description = key;
			CLAM::ProcessingFactory::Values descriptionList=factory.GetValuesFromAttribute(key, "description");
			if (! descriptionList.empty()) description=descriptionList.front();

			QTreeWidgetItem * item = new QTreeWidgetItem( categoryTree, QStringList() << description.c_str());
			
			CLAM::ProcessingFactory::Values iconList=factory.GetValuesFromAttribute(key,"icon");
			std::string iconPath = "processing.png";
			if (! iconList.empty()) iconPath = iconList.front();
			item->setIcon(0, QIcon(QString(":/icons/images/%1").arg(iconPath.c_str())));

			item->setText(1, key.c_str());
		}
	}

	
	connect( this, SIGNAL( itemPressed(QTreeWidgetItem *,int) ),
		 this, SLOT( PressProcessing(QTreeWidgetItem *,int) ));
}

ProcessingTree::~ProcessingTree()
{
}

void ProcessingTree::PressProcessing(QTreeWidgetItem * item, int column)
{	
	if (!item) return;
	if (!item->parent()) return;

	// always use column 1
	QString className = item->text(1);
	QDrag *drag = new QDrag( this);
	QMimeData * data = new QMimeData;
	data->setText(className);
	drag->setMimeData(data);
	drag->setPixmap(QPixmap(":/icons/images/processing.png"));
	Qt::DropAction dropAction = drag->start();
}


} // namespace NetworkGUI


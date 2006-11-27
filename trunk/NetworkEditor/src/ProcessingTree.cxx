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

namespace NetworkGUI
{
const static char * processingClasses[] = {
	"Audio Input/Output",
	"AudioSource",
	"AudioSink",
	"AudioOut",
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
	"MIDIKeyboard",
	"MIDIInControl",
	"MIDIDispatcher",
	0,
	"Analysis",
	"FFT_rfftw",
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
	"Deeser",
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
	"Tunner",
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

	QString className = item->text(0);
	QDrag *drag = new QDrag( this);
	QMimeData * data = new QMimeData;
	data->setText(className);
	drag->setMimeData(data);
	drag->setPixmap(QPixmap(":/icons/images/processing.png"));
	Qt::DropAction dropAction = drag->start();
}


} // namespace NetworkGUI

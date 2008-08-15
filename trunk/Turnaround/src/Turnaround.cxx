/*
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
 */

#include "Turnaround.hxx"

#include "TurnaroundVersion.hxx"

#include <QtGui/QFileDialog>
#include <QtGui/QProgressDialog>

#include <CLAM/PANetworkPlayer.hxx>
#include <CLAM/JACKNetworkPlayer.hxx>
#include <CLAM/TonalAnalysis.hxx>
#include <CLAM/AudioFileMemoryLoader.hxx>
#include <CLAM/MonoAudioFileReader.hxx>
#include "KeySpace.hxx"
#include "VectorView.hxx"
#include "Tonnetz.hxx"
#include "ChordRanking.hxx"
#include "PolarChromaPeaks.hxx"
#include "SegmentationView.hxx"
#include "ProgressControl.hxx"
#include "FrameDivision.hxx"
#include "FloatVectorStorage.hxx"
#include "FloatPairVectorStorage.hxx"

#include <iostream>

Turnaround::Turnaround()
	: QMainWindow(0)
	, Ui::Turnaround()
	, _networkPlayer(0)
	, _tonalAnalysis(0)
	, _frameDivision(0)
{
	setupUi(this);
	
	connect(fileOpenAction, SIGNAL(triggered()), this, SLOT(fileOpen()));
	connect(fileExitAction, SIGNAL(triggered()), this, SLOT(close()));
	connect(playbackPlayAction, SIGNAL(triggered()), this, SLOT(play()));
	connect(playbackPauseAction, SIGNAL(triggered()), this, SLOT(pause()));
	connect(playbackStopAction, SIGNAL(triggered()), this, SLOT(stop()));

	_vboxLayout = new QVBoxLayout(centralwidget);
	_progressControlWidget = new ProgressControlWidget(centralwidget);
	_vboxLayout->addWidget(_progressControlWidget);

	_fileReader = _network.AddProcessing("AudioFileMemoryLoader");

	CLAM::ProgressControl * progress = new CLAM::ProgressControl;
	_progressControl = _network.GetUnusedName("ProgressControl");
	_network.AddProcessing(_progressControl, progress);
	_network.ConnectControls(_fileReader+".Current Time Position", _progressControl+".Progress Update");	
	_network.ConnectControls(_progressControl+".Progress Jump", _fileReader+".Current Time Position (%)");	
	_progressControlWidget->SetProcessing(progress);

	_audioSink = _network.AddProcessing("AudioSink");
	_network.ConnectPorts(_fileReader+".Samples Read", _audioSink+".AudioIn");

	_vectorView = new CLAM::VM::VectorView(centralwidget);
	_vboxLayout->addWidget(_vectorView);

	_tonnetz = new CLAM::VM::Tonnetz(centralwidget);
	_vboxLayout->addWidget(_tonnetz);

	_keySpace = new CLAM::VM::KeySpace(centralwidget);
	_vboxLayout->addWidget(_keySpace);

	_chordRanking = new CLAM::VM::ChordRanking(centralwidget);
	_vboxLayout->addWidget(_chordRanking);

	_polarChromaPeaks = new PolarChromaPeaks(centralwidget);
	_vboxLayout->addWidget(_polarChromaPeaks);

	_segmentationView = new SegmentationView(centralwidget);
	_segmentationView->beCentred(true);
	_vboxLayout->addWidget(_segmentationView);

	_networkPlayer = new CLAM::PANetworkPlayer;
	_network.SetPlayer(_networkPlayer);

	_tonalAnalysis = new CLAM::TonalAnalysis;

	startTimer(50);
}

Turnaround::~Turnaround()
{
	if (!_network.IsStopped())
		_network.Stop();

	delete _networkPlayer;
	delete _tonalAnalysis;
}

void Turnaround::fileOpen()
{
	QString qFileName = QFileDialog::getOpenFileName(this, 
			tr("Choose an audio file"), QString::null, 
			tr("Audio files (*.wav *.ogg *.mp3)"));
	if(qFileName == QString::null) return;
	loadAudioFile(qFileName.toLocal8Bit().constData());
}

void Turnaround::loadAudioFile(const std::string & fileName)
{
	if (!_network.IsStopped())
		_network.Stop();

	// Point the widgets to no source
	_vectorView->noDataSource();
	_tonnetz->noDataSource();
	_keySpace->noDataSource();
	_chordRanking->noDataSource();
	_segmentationView->noDataSource();

	QProgressDialog progress(tr("Analyzing chords..."), 0, 0, 1, this);
	progress.setWindowModality(Qt::WindowModal);
	progress.setValue(0);

	_fileReaderConfig.SetSourceFile(fileName);
	if (!_network.ConfigureProcessing(_fileReader, _fileReaderConfig))
		return;

	// Begin analysis
	CLAM::MonoAudioFileReader fileReader(_fileReaderConfig);
	CLAM::AudioInPort &analysisInput = (CLAM::AudioInPort&)(_tonalAnalysis->GetInPort("Audio Input"));

	const unsigned hop = analysisInput.GetHop();
	const unsigned frameSize = analysisInput.GetSize();
	const unsigned nSamples = fileReader.GetHeader().GetSamples();
	const unsigned long nFrames = (unsigned long)(floor((float)(nSamples-frameSize+hop)/(float)hop));
	const CLAM::TData sampleRate = fileReader.GetHeader().GetSampleRate();
	_length = CLAM::TData(nSamples) / sampleRate;

	std::cout << "Number of frames: " << nFrames << std::endl;

	progress.setMaximum(nFrames);
	
	FloatVectorStorageConfig storageConfig;

	storageConfig.SetBins(12);
	storageConfig.SetFrames(nFrames);
	FloatVectorStorage pcpStorage(storageConfig);

	storageConfig.SetBins(24);
	storageConfig.SetFrames(nFrames);
	FloatVectorStorage chordCorrelationStorage(storageConfig);

	FloatPairVectorStorageConfig pairStorageConfig;
	pairStorageConfig.SetFrames(nFrames);
	FloatPairVectorStorage chromaPeaksStorage(pairStorageConfig);

	CLAM::ConnectPorts(fileReader, "Samples Read", *_tonalAnalysis, "Audio Input");
	CLAM::ConnectPorts(*_tonalAnalysis, "Pitch Profile", pcpStorage, "Data Input");
	CLAM::ConnectPorts(*_tonalAnalysis, "Chord Correlation", chordCorrelationStorage, "Data Input");
	CLAM::ConnectPorts(*_tonalAnalysis, "Chroma Peaks", chromaPeaksStorage, "Data Input");

	fileReader.Start();
	_tonalAnalysis->Start();
	pcpStorage.Start();
	chordCorrelationStorage.Start();
	chromaPeaksStorage.Start();

	unsigned long i = 0;
	while (fileReader.Do())
	{
		if (! analysisInput.CanConsume())
			continue;
		_tonalAnalysis->Do();
		pcpStorage.Do();
		chordCorrelationStorage.Do();
		chromaPeaksStorage.Do();
		progress.setValue(++i);
	}

	fileReader.Stop();
	_tonalAnalysis->Stop();
	pcpStorage.Stop();
	chordCorrelationStorage.Stop();
	chromaPeaksStorage.Stop();

	_frameDivision = new CLAM_Annotator::FrameDivision;
	_frameDivision->SetFirstCenter(frameSize / 2);
	_frameDivision->SetInterCenterGap(hop);

	const unsigned nBins = 12;
	const char * notes[] = { 
		"G", "G#", "A", "A#",
		"B", "C", "C#", "D",
		"D#", "E", "F", "F#"
	};
	std::vector<std::string> binLabels(notes, notes+nBins);

	_pcpSource.setDataSource(nBins, 0, 0, binLabels);
	_pcpSource.updateData(pcpStorage.Data(), sampleRate, _frameDivision, nFrames);
	_vectorView->setDataSource(_pcpSource);
	_tonnetz->setDataSource(_pcpSource);

	const char * minorChords[] = { 
		"g", "g#", "a", "a#",
		"b", "c", "c#", "d",
		"d#", "e", "f", "f#"
	};
	binLabels.insert(binLabels.end(), minorChords, minorChords+nBins);

	_chordCorrelationSource.setDataSource(nBins*2, 0, 0, binLabels); // nBins?
	_chordCorrelationSource.updateData(chordCorrelationStorage.Data(), sampleRate, _frameDivision, nFrames);
	_keySpace->setDataSource(_chordCorrelationSource);
	_chordRanking->setDataSource(_chordCorrelationSource);

	_chromaPeaksSource.setDataSource(1);
	_chromaPeaksSource.updateData(chromaPeaksStorage.PositionStorage(), chromaPeaksStorage.MagnitudeStorage(), sampleRate, _frameDivision);
	_polarChromaPeaks->setDataSource(_chromaPeaksSource);

	CLAM::OutPort<CLAM::Segmentation> &segmentationOutput = (CLAM::OutPort<CLAM::Segmentation>&)(_tonalAnalysis->GetOutPort("Chord Segmentation"));
	_segmentationSource.updateData(segmentationOutput.GetData());
	_segmentationView->setDataSource(_segmentationSource);
	// End analysis

	_network.Start();
}

void Turnaround::play()
{
	if (_network.IsStopped())
		_network.Start();
}

void Turnaround::pause()
{
	// TODO
	if (!_network.IsStopped())
		_network.Stop();
}

void Turnaround::stop()
{
	if (not _network.IsStopped())
		_network.Stop();
}

void Turnaround::timerEvent(QTimerEvent *event)
{
	double time = _network.GetInControlByCompleteName(_progressControl+".Progress Update").GetLastValue() * _length;
	_pcpSource.setCurrentTime(time);
	_chordCorrelationSource.setCurrentTime(time);
	_chromaPeaksSource.setCurrentTime(time);
	_segmentationSource.setCurrentTime(time);
}

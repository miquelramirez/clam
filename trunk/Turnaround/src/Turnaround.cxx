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

#include <CLAM/PANetworkPlayer.hxx>
#include <CLAM/JACKNetworkPlayer.hxx>
#include <CLAM/TonalAnalysis.hxx>
#include <CLAM/AudioFileMemoryLoader.hxx>
#include <CLAM/MonoAudioFileReader.hxx>
#include "VectorViewMonitor.hxx"
#include "ProgressControl.hxx"
#include "FrameDivision.hxx"
#include "FloatVectorStorage.hxx"

#include <iostream> // debug

Turnaround::Turnaround()
	: QMainWindow( 0 )
	, Ui::Turnaround( )
	, _dataSource( 0 )
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
	
	_network.SetPlayer(new CLAM::PANetworkPlayer);

	startTimer(50);
}

Turnaround::~Turnaround()
{
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
	// TODO
		
	if (!_network.IsStopped())
		_network.Stop();
		
	_fileReaderConfig.SetSourceFile(fileName);
	
	analyse();
	
	if (!_network.ConfigureProcessing(_fileReader, _fileReaderConfig))
		return;
	
	_network.Start();
}

void Turnaround::analyse()
{
	CLAM::MonoAudioFileReader fileReader(_fileReaderConfig);
	CLAM::TonalAnalysis tonalAnalysis;
	CLAM::AudioInPort &analysisInput = (CLAM::AudioInPort&)(tonalAnalysis.GetInPort("Audio Input"));
	fileReader.GetOutPort("Samples Read").ConnectToIn(analysisInput);

	const unsigned hop = analysisInput.GetHop();
	const unsigned frameSize = analysisInput.GetSize();
	const unsigned nSamples = fileReader.GetHeader().GetSamples();
	const unsigned long nFrames = (unsigned long)(floor((float)(nSamples-frameSize+hop)/(float)hop));
	const CLAM::TData sampleRate = fileReader.GetHeader().GetSampleRate();
	_length = CLAM::TData(nSamples) / sampleRate;
	
	std::cout << nFrames << std::endl; // debug
	
	FloatVectorStorage storage;
	tonalAnalysis.GetOutPort("Pitch Profile").ConnectToIn(storage.GetInPort("Data Input"));
	
	fileReader.Start();
	tonalAnalysis.Start();
	storage.Start();

	while (fileReader.Do())
	{
		if (! analysisInput.CanConsume())
			continue;
		tonalAnalysis.Do();
		storage.Do();
	}

	CLAM_Annotator::FrameDivision * frameDivision = new CLAM_Annotator::FrameDivision;
	frameDivision->SetFirstCenter(frameSize / 2);
	frameDivision->SetInterCenterGap(hop);

	std::vector<std::string> binLabels;
	binLabels.push_back("G");
	binLabels.push_back("G#");
	binLabels.push_back("A");
	binLabels.push_back("A#");
	binLabels.push_back("B");
	binLabels.push_back("C");
	binLabels.push_back("C#");
	binLabels.push_back("D");
	binLabels.push_back("D#");
	binLabels.push_back("E");
	binLabels.push_back("F");
	binLabels.push_back("F#");
	
	_dataSource = new CLAM::VM::PoolFloatArrayDataSource;
	_dataSource->setDataSource(12, 0, 0, binLabels);
	_dataSource->updateData(storage.Data(), sampleRate, frameDivision, nFrames);
	_vectorView->setDataSource(*_dataSource);
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
	if (!_network.IsStopped())
		_network.Stop();
}

void Turnaround::timerEvent(QTimerEvent *event)
{
	if (_dataSource)
		_dataSource->setCurrentTime(_network.GetInControlByCompleteName(_progressControl+".Progress Update").GetLastValue() * _length);
}

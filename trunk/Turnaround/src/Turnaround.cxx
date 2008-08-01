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
#include <CLAM/TonalAnalysis.hxx>
#include "KeySpaceMonitor.hxx"
#include "ProgressControl.hxx"

Turnaround::Turnaround(const std::string & nameProject = "")
	: QMainWindow( 0 )
	, Ui::Turnaround( )
{
	setupUi(this);
	
	connect(fileOpenAction, SIGNAL(triggered()), this, SLOT(fileOpen()));
	connect(fileExitAction, SIGNAL(triggered()), this, SLOT(close()));
	connect(playbackPlayAction, SIGNAL(triggered()), this, SLOT(play()));
	connect(playbackPauseAction, SIGNAL(triggered()), this, SLOT(pause()));
	connect(playbackStopAction, SIGNAL(triggered()), this, SLOT(stop()));

	_fileReader = _network.AddProcessing("AudioFileMemoryLoader");
	
	CLAM::ProgressControl * progress = new CLAM::ProgressControl;
	_progressControl = _network.GetUnusedName("ProgressControl");
	_network.AddProcessing(_progressControl, progress);
	_network.ConnectControls(_fileReader+".Current Time Position", _progressControl+".Progress Update");	
	_network.ConnectControls(_progressControl+".Progress Jump", _fileReader+".Current Time Position (%)");	
	progressControl->SetProcessing(progress);
	
	_audioSink = _network.AddProcessing("AudioSink");
	_network.ConnectPorts(_fileReader+".Samples Read", _audioSink+".AudioIn");
	
	_tonalAnalysis = _network.AddProcessing("TonalAnalysis");
	_network.ConnectPorts(_fileReader+".Samples Read", _tonalAnalysis+".Audio Input");	
	
	KeySpaceMonitor * monitor = new KeySpaceMonitor;
	_keySpaceMonitor = _network.GetUnusedName("KeySpaceMonitor");
	_network.AddProcessing(_keySpaceMonitor, monitor);
	_network.ConnectPorts(_tonalAnalysis+".Chord Correlation", _keySpaceMonitor+".Input");
	keySpace->setDataSource(*monitor);
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
	loadAudioFile(qFileName.toStdString());
}

void Turnaround::loadAudioFile(const std::string & fileName)
{
	// TODO
	
	if (!_network.IsStopped())
		_network.Stop();
		
	_fileReaderConfig.SetSourceFile(fileName);
	if (!_network.ConfigureProcessing(_fileReader, _fileReaderConfig))
		return;

	_network.SetPlayer(new CLAM::PANetworkPlayer);
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
	if (!_network.IsStopped())
		_network.Stop();
}

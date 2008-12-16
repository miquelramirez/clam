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

#include "MainWindow.hxx"

#include "TurnaroundVersion.hxx"

#include "ui_About.hxx"
#include <QtGui/QFileDialog>
#include <QtGui/QProgressDialog>
#include <QtGui/QMessageBox>
#include <QtGui/QVBoxLayout>
#include <QSettings>
#include <QDesktopServices>

#include <CLAM/CLAMVersion.hxx>
#include <CLAM/PANetworkPlayer.hxx>
#include <CLAM/TonalAnalysis.hxx>
#include <CLAM/AudioFileMemoryLoader.hxx>
#include <CLAM/MonoAudioFileReader.hxx>
#include <CLAM/AudioTextDescriptors.hxx>
#include "KeySpace.hxx"
#include "Spectrogram.hxx"
#include "Tonnetz.hxx"
#include "ChordRanking.hxx"
#include "PolarChromaPeaks.hxx"
#include "SegmentationView.hxx"
#include "ProgressControl.hxx"
#include "FloatVectorStorage.hxx"
#include "FloatPairVectorStorage.hxx"

#include <iostream>
#include <vector>
#include <string>

MainWindow::MainWindow()
	: QMainWindow(0)
	, Ui::MainWindow()
	, _networkPlayer(0)
	, _tonalAnalysis(0)
	, _pcpStorage(0)
	, _timerID(0)
{
	setupUi(this);
	
	connect(fileOpenAction, SIGNAL(triggered()), this, SLOT(fileOpen()));
	connect(fileExitAction, SIGNAL(triggered()), this, SLOT(close()));
	connect(playbackPlayAction, SIGNAL(triggered()), this, SLOT(play()));
	connect(playbackPauseAction, SIGNAL(triggered()), this, SLOT(pause()));
	connect(playbackStopAction, SIGNAL(triggered()), this, SLOT(stop()));
	connect(viewSpectrogramAction, SIGNAL(toggled(bool)), this, SLOT(toggleSpectrogram(bool)));
	connect(viewTonnetzAction, SIGNAL(toggled(bool)), this, SLOT(toggleTonnetz(bool)));
	connect(viewKeySpaceAction, SIGNAL(toggled(bool)), this, SLOT(toggleKeySpace(bool)));
	connect(viewChromaPeaksAction, SIGNAL(toggled(bool)), this, SLOT(toggleChromaPeaks(bool)));
	connect(viewChordRankingAction, SIGNAL(toggled(bool)), this, SLOT(toggleChordRanking(bool)));
	connect(viewSegmentationAction, SIGNAL(toggled(bool)), this, SLOT(toggleSegmentation(bool)));
	connect(helpAboutAction, SIGNAL(triggered()), this, SLOT(about()));	
	connect(helpOnlineTutorialAction, SIGNAL(triggered()), this, SLOT(onlineTutorial()));	

	QVBoxLayout * vboxLayout = new QVBoxLayout(centralwidget);
	_progressControlWidget = new ProgressControlWidget(centralwidget);
	vboxLayout->addWidget(_progressControlWidget);

	_fileReader = _network.AddProcessing("AudioFileMemoryLoader");

	CLAM::ProgressControl * progress = new CLAM::ProgressControl;
	_progressControl = _network.GetUnusedName("ProgressControl");
	_network.AddProcessing(_progressControl, progress);
	_network.ConnectControls(_fileReader+".Current Time Position", _progressControl+".Progress Update");
	_network.ConnectControls(_progressControl+".Progress Jump", _fileReader+".Current Time Position (%)");
	_progressControlWidget->SetProcessing(progress);

	_audioSink = _network.AddProcessing("AudioSink");
	_network.ConnectPorts(_fileReader+".Samples Read", _audioSink+".AudioIn");

	_spectrogram = new CLAM::VM::Spectrogram(centralwidget);
	vboxLayout->addWidget(_spectrogram);

	_tonnetz = new CLAM::VM::Tonnetz(centralwidget);
	vboxLayout->addWidget(_tonnetz);

	_keySpace = new CLAM::VM::KeySpace(centralwidget);
	vboxLayout->addWidget(_keySpace);

	QHBoxLayout * hboxLayout = new QHBoxLayout;
	vboxLayout->addLayout(hboxLayout);

	_polarChromaPeaks = new PolarChromaPeaks(centralwidget);
	hboxLayout->addWidget(_polarChromaPeaks);

	_chordRanking = new CLAM::VM::ChordRanking(centralwidget);
	hboxLayout->addWidget(_chordRanking);

	_segmentationView = new SegmentationView(centralwidget);
	_segmentationView->beCentred(true);
	vboxLayout->addWidget(_segmentationView);

	_networkPlayer = new CLAM::PANetworkPlayer;
	_network.SetPlayer(_networkPlayer);

	_tonalAnalysis = new CLAM::TonalAnalysis;

	_aboutDialog = new QDialog(this);
	Ui::About aboutUi;
	aboutUi.setupUi(_aboutDialog);
	aboutUi.versionInfo->setText(tr(
			"<p><b>Turnaround version %1</b></p>"
			"<p>Using CLAM version %2</p>")
		.arg(Turnaround::GetFullVersion())
		.arg(CLAM::GetFullVersion()));

	viewSpectrogramAction->setChecked(true);
	viewTonnetzAction->setChecked(true);
	viewKeySpaceAction->setChecked(true);
	viewChromaPeaksAction->setChecked(true);
	viewChordRankingAction->setChecked(true);
	viewSegmentationAction->setChecked(true);

	QSettings settings;
	_recentFiles = settings.value("RecentFiles").toStringList();
	updateRecentMenu();
	viewSpectrogramAction->setChecked(settings.value("SpectrogramVisible", true).toBool());
	viewTonnetzAction->setChecked(settings.value("TonnetzVisible", true).toBool());
	viewKeySpaceAction->setChecked(settings.value("KeySpaceVisible", true).toBool());
	viewChromaPeaksAction->setChecked(settings.value("ChromaPeaksVisible", true).toBool());
	viewChordRankingAction->setChecked(settings.value("ChordRankingVisible", true).toBool());
	viewSegmentationAction->setChecked(settings.value("SegmentationVisible", true).toBool());
}

MainWindow::~MainWindow()
{
	QSettings settings;
	settings.setValue("RecentFiles", _recentFiles);
	settings.setValue("SpectrogramVisible",  viewSpectrogramAction->isChecked());
	settings.setValue("TonnetzVisible",      viewTonnetzAction->isChecked());
	settings.setValue("KeySpaceVisible",     viewKeySpaceAction->isChecked());
	settings.setValue("ChromaPeaksVisible",  viewChromaPeaksAction->isChecked());
	settings.setValue("ChordRankingVisible", viewChordRankingAction->isChecked());
	settings.setValue("SegmentationVisible", viewSegmentationAction->isChecked());

	stop();

	delete _tonalAnalysis;
	if (_pcpStorage)
	{
		delete _pcpStorage;
		delete _chordCorrelationStorage;
		delete _chromaPeaksStorage;
	}
}

void MainWindow::updateRecentMenu()
{
	menuOpenRecent->clear();
	
	if (_recentFiles.size() == 0)
	{
		menuOpenRecent->setEnabled(false);
		return;
	}
	menuOpenRecent->setEnabled(true);
	
	int i=0;
	for (QStringList::iterator it = _recentFiles.begin(); it!=_recentFiles.end(); it++)
	{
		QString text = QString("&%1 %2").arg(++i).arg(*it);
		QAction * recentFileAction = new QAction(text, this);
		recentFileAction->setData(*it);
		menuOpenRecent->addAction(recentFileAction);
		connect(recentFileAction, SIGNAL(triggered()), this, SLOT(recentFileOpen()));
	}
}

void MainWindow::appendRecentFile(const QString & recentFile)
{
	_recentFiles.removeAll(recentFile);
	_recentFiles.push_front(recentFile);
	while (_recentFiles.size() > 8)
		_recentFiles.pop_back();
	updateRecentMenu();
}

void MainWindow::fileOpen()
{
	QString qFileName = QFileDialog::getOpenFileName(this, 
			tr("Choose an audio file"), QString::null, 
			tr("Audio files (*.wav *.ogg *.mp3)"));
	if(qFileName == QString::null) return;
	appendRecentFile(qFileName);
	loadAudioFile(qFileName.toLocal8Bit().constData());
}

void MainWindow::recentFileOpen()
{
	QAction *action = qobject_cast<QAction *>(sender());
	if (!action) return;
	QString file = action->data().toString();
	if (file == QString::null) return;
	appendRecentFile(file);
	loadAudioFile(file.toLocal8Bit().constData());
}

std::vector<std::string> MainWindow::initBinLabelVector() const
{
	static const char * roots[] = {
		"G",
		"G#",
		"A",
		"A#",
		"B",
		"C",
		"C#",
		"D",
		"D#",
		"E",
		"F",
		"F#",
	};
	static struct Modes {
		const char * name;
		unsigned nRoots;
	} modes[] =
	{
		{"None",	1},
		{"Major",	12},
		{"Minor",	12},
		{"Major7",	12}, // Cmaj7
		{"Dominant7",	12}, // C7
		{"MinorMajor7",	12}, // Cm/maj7
		{"Minor7",	12}, // Cm7
//		{"Suspended2",	12}, // Csus2
//		{"Suspended4",	12}, // Csus4
//		{"Major6",	12}, // C6
//		{"Minor6",	12}, // Cm6
//		{"6/9",		12}, // C6/9
		{"Augmented",	4}, // Caug
		{"Diminished",	12}, // Cdim
		{"Diminished7",	12}, // Cdim7
//		{"Fifth",	12}, // C5
		{0, 0}
	};
	std::vector<std::string> chordNames;
	for (unsigned i = 0; modes[i].name; i++)
	{
		std::string mode=modes[i].name;
		if (modes[i].nRoots == 1)
		{
			chordNames.push_back(mode);
			continue;
		}
		for (unsigned root=0; root<modes[i].nRoots; root++)
		{
			std::string rootName(roots[root]);
			chordNames.push_back(rootName+mode);
		}
	}
	return chordNames;
}

void MainWindow::loadAudioFile(const std::string & fileName)
{
	stop();

	// Point the widgets to no source
	_spectrogram->noDataSource();
	_tonnetz->noDataSource();
	_keySpace->noDataSource();
	_chordRanking->noDataSource();
	_segmentationView->noDataSource();
	
	// Point the data sources to no storage and delete old storages
	_pcpSource.clearData();
	_keySpaceSource.clearData();
	_chordCorrelationSource.clearData();
	_chromaPeaksSource.clearData();
	if (_pcpStorage)
	{
		delete _pcpStorage;
		delete _chordCorrelationStorage;
		delete _chromaPeaksStorage;
	}

	QProgressDialog progress(tr("Analyzing chords..."), tr("Abort"), 0, 2, this);
	progress.setWindowModality(Qt::WindowModal);
	progress.setValue(1);
	QApplication::processEvents();

	_fileReaderConfig.SetSourceFile(fileName);
	if (!_network.ConfigureProcessing(_fileReader, _fileReaderConfig))
	{
		progress.close();
		QMessageBox::critical(this,
			tr("Loading error"),
			tr("Error loading the media\n%1")
				.arg(_network.GetProcessing(_fileReader).GetConfigErrorMessage().c_str())
			);
		return;
	}
	
	// Begin analysis
	CLAM::MonoAudioFileReader fileReader(_fileReaderConfig);
	CLAM::AudioInPort &analysisInput = (CLAM::AudioInPort&)(_tonalAnalysis->GetInPort("Audio Input"));

	std::string title = "Turnaround: ";
	const CLAM::AudioTextDescriptors &textDescriptors = fileReader.GetTextDescriptors();
	title += textDescriptors.HasArtist() ? textDescriptors.GetArtist() : "Unknown Artist";
	title += " - ";
	title += textDescriptors.HasTitle() ? textDescriptors.GetTitle() : "Unknown Title";
	setWindowTitle(title.c_str());

	const unsigned hop = analysisInput.GetHop();
	const unsigned frameSize = analysisInput.GetSize();
	const unsigned nSamples = fileReader.GetHeader().GetSamples();
	const unsigned long nFrames = (unsigned long)(floor((float)(nSamples-frameSize+hop)/(float)hop));
	const CLAM::TData sampleRate = fileReader.GetHeader().GetSampleRate();
	_length = CLAM::TData(nSamples) / sampleRate;

	const unsigned nBins = 12;
	const unsigned maxBins = 101;

	std::cout << "Number of frames: " << nFrames << std::endl;

	progress.setMaximum(nFrames);
	
	FloatVectorStorageConfig storageConfig;

	storageConfig.SetBins(nBins);
	storageConfig.SetFrames(nFrames);
	_pcpStorage = new FloatVectorStorage(storageConfig);

	storageConfig.SetBins(maxBins);
	storageConfig.SetFrames(nFrames);
	_chordCorrelationStorage = new FloatVectorStorage(storageConfig);

	FloatPairVectorStorageConfig pairStorageConfig;
	pairStorageConfig.SetFrames(nFrames);
	_chromaPeaksStorage = new FloatPairVectorStorage(pairStorageConfig);

	CLAM::ConnectPorts(fileReader, "Samples Read", *_tonalAnalysis, "Audio Input");
	CLAM::ConnectPorts(*_tonalAnalysis, "Pitch Profile", *_pcpStorage, "Data Input");
	CLAM::ConnectPorts(*_tonalAnalysis, "Chord Correlation", *_chordCorrelationStorage, "Data Input");
	CLAM::ConnectPorts(*_tonalAnalysis, "Chroma Peaks", *_chromaPeaksStorage, "Data Input");

	fileReader.Start();
	_tonalAnalysis->Start();
	_pcpStorage->Start();
	_chordCorrelationStorage->Start();
	_chromaPeaksStorage->Start();

	unsigned long i = 0;
	while (fileReader.Do())
	{
		if (! analysisInput.CanConsume())
			continue;
		progress.setValue(i++);
		_tonalAnalysis->Do();
		_pcpStorage->Do();
		_chordCorrelationStorage->Do();
		_chromaPeaksStorage->Do();
	}

	fileReader.Stop();
	_tonalAnalysis->Stop();
	_pcpStorage->Stop();
	_chordCorrelationStorage->Stop();
	_chromaPeaksStorage->Stop();

	_frameDivision.SetFirstCenter(frameSize / 2);
	_frameDivision.SetInterCenterGap(hop);

	const char * notes[] = { 
		"G", "G#", "A", "A#",
		"B", "C", "C#", "D",
		"D#", "E", "F", "F#"
	};
	std::vector<std::string> binLabels(notes, notes+nBins);

	_pcpSource.setDataSource(nBins, 0, 0, binLabels);
	_pcpSource.setStorage(_pcpStorage, sampleRate, &_frameDivision, nFrames);
	_spectrogram->setDataSource(_pcpSource);
	_tonnetz->setDataSource(_pcpSource);

	const char * minorChords[] = { 
		"g", "g#", "a", "a#",
		"b", "c", "c#", "d",
		"d#", "e", "f", "f#"
	};
	binLabels.insert(binLabels.end(), minorChords, minorChords+nBins);

	_keySpaceSource.setDataSource(nBins*2, maxBins - nBins*2, 1, binLabels);
	_keySpaceSource.setStorage(_chordCorrelationStorage, sampleRate, &_frameDivision, nFrames);
	_keySpace->setDataSource(_keySpaceSource);

	_chordCorrelationSource.setDataSource(maxBins, 0, 0, initBinLabelVector());
	_chordCorrelationSource.setStorage(_chordCorrelationStorage, sampleRate, &_frameDivision, nFrames);
	_chordRanking->setDataSource(_chordCorrelationSource);

	_chromaPeaksSource.setDataSource(1);
	_chromaPeaksSource.setStorage(_chromaPeaksStorage, sampleRate, &_frameDivision);
	_polarChromaPeaks->setDataSource(_chromaPeaksSource);

	CLAM::OutPort<CLAM::Segmentation> &segmentationOutput = (CLAM::OutPort<CLAM::Segmentation>&)(_tonalAnalysis->GetOutPort("Chord Segmentation"));
	_segmentationSource.updateData(segmentationOutput.GetData());
	_segmentationView->setDataSource(_segmentationSource);
	// End analysis

	play();
}

void MainWindow::play()
{
	if (not _network.IsPlaying())
		_network.Start();
	if (_timerID == 0)
		_timerID = startTimer(50);
}

void MainWindow::pause()
{
	if (_network.IsPlaying())
		_network.Pause();
}

void MainWindow::stop()
{
	if (not _network.IsStopped())
		_network.Stop();
	if (_timerID > 0)
	{
		killTimer(_timerID);
		_timerID = 0;
	}
}

void MainWindow::timerEvent(QTimerEvent *event)
{
	CLAM::FloatInControl & inControl =
		(CLAM::FloatInControl&) _network.GetInControlByCompleteName(_progressControl+".Progress Update");
	double time = inControl.GetLastValue() * _length;
	_pcpSource.setCurrentTime(time);
	_keySpaceSource.setCurrentTime(time);
	_chordCorrelationSource.setCurrentTime(time);
	_chromaPeaksSource.setCurrentTime(time);
	_segmentationSource.setCurrentTime(time);
}

void MainWindow::toggleSpectrogram(bool checked)
{
	_spectrogram->setVisible(checked);
}

void MainWindow::toggleTonnetz(bool checked)
{
	_tonnetz->setVisible(checked);
}

void MainWindow::toggleKeySpace(bool checked)
{
	_keySpace->setVisible(checked);
}

void MainWindow::toggleChromaPeaks(bool checked)
{
	_polarChromaPeaks->setVisible(checked);
}

void MainWindow::toggleChordRanking(bool checked)
{
	_chordRanking->setVisible(checked);
}

void MainWindow::toggleSegmentation(bool checked)
{
	_segmentationView->setVisible(checked);
}

void MainWindow::about()
{
	_aboutDialog->show();
}

void MainWindow::onlineTutorial()
{
	QString helpUrl = "http://iua-share.upf.es/wikis/clam/index.php/Turnaround_tutorial";
	QDesktopServices::openUrl(helpUrl);
}

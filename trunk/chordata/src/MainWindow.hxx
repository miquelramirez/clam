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

#ifndef TURNAROUND_HXX
#define TURNAROUND_HXX

#include "ui_MainWindow.hxx"

#include <string>

#include <CLAM/Network.hxx>
#include <CLAM/MonoAudioFileReader.hxx>
#include "PoolFloatArrayDataSource.hxx"
#include "PoolPeakDataSource.hxx"
#include "PoolSegmentationDataSource.hxx"
#include <CLAM/qtmonitors/ProgressControlWidget.hxx>
#include "FrameDivision.hxx"

namespace CLAM
{
	class NetworkPlayer;
	class TonalAnalysis;
	class MIDIPianoWidget;
namespace VM
{
	class Spectrogram;
	class Tonnetz;
	class KeySpace;
	class ChordRanking;
}
}
class PolarChromaPeaks;
class SegmentationView;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow();
	virtual ~MainWindow();
	
	void setBackend(QString &backend);

public slots:
	void fileOpen();
	void recentFileOpen();

	void play();
	void pause();
	void stop();

	void about();
	void onlineTutorial();

protected:
	void timerEvent(QTimerEvent *event);

private:
	std::vector<std::string> initBinLabelVector() const;
	void loadAudioFile(const std::string & fileName);
	void updateRecentMenu();
	void appendRecentFile(const QString & recentFile);
	
	CLAM::Network _network;
	CLAM::NetworkPlayer *_networkPlayer;
	CLAM::MonoAudioFileReaderConfig _fileReaderConfig;
	std::string _fileReader;
	std::string _progressControl;
	std::string _controlPiano;
	
	CLAM::TonalAnalysis *_tonalAnalysis;
	
	ProgressControlWidget *_progressControlWidget;
	CLAM::VM::Spectrogram *_spectrogram;
	CLAM::VM::Tonnetz *_tonnetz;
	CLAM::VM::KeySpace *_keySpace;
	CLAM::VM::ChordRanking *_chordRanking;
	PolarChromaPeaks *_polarChromaPeaks;
	SegmentationView *_segmentationView;
	CLAM::MIDIPianoWidget	*_pianoView;

	FloatVectorStorage *_pcpStorage;
	FloatVectorStorage *_chordCorrelationStorage;
	FloatPairVectorStorage *_chromaPeaksStorage;
	
	CLAM::VM::PoolFloatArrayDataSource _pcpSource;
	CLAM::VM::PoolFloatArrayDataSource _keySpaceSource;
	CLAM::VM::PoolFloatArrayDataSource _chordCorrelationSource;
	CLAM::VM::PoolPeakDataSource _chromaPeaksSource;
	CLAM::VM::PoolSegmentationDataSource _segmentationSource;
	CLAM_Annotator::FrameDivision _frameDivision;
	CLAM::TData _length;
	
	CLAM::TControlData _pausedProgress;

	QStringList _recentFiles;
	int _timerID;
	QDialog *_aboutDialog;
	Ui::MainWindow _ui;
};

#endif


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

#include "ui_Turnaround.hxx"

#include <string>

#include <CLAM/Network.hxx>
#include <CLAM/MonoAudioFileReader.hxx>
#include "PoolFloatArrayDataSource.hxx"
#include "ProgressControlWidget.hxx"
#include "VectorView.hxx"

#include <QtGui/QVBoxLayout>

class Turnaround : public QMainWindow, public Ui::Turnaround
{
	Q_OBJECT

public:
	Turnaround();
	virtual ~Turnaround();
	
public slots:
	void fileOpen();
	
	void play();
	void pause();
	void stop();
	
protected:
	void timerEvent(QTimerEvent *event);

private:
	void loadAudioFile(const std::string & fileName);
	void analyse();
	
	CLAM::Network _network;
	CLAM::MonoAudioFileReaderConfig _fileReaderConfig;
	std::string _fileReader;
	std::string _progressControl;
	std::string _audioSink;
	std::string _tonalAnalysis;
	std::string _vectorViewMonitor;
	
	QVBoxLayout *_vboxLayout;
	ProgressControlWidget *_progressControlWidget;
	CLAM::VM::VectorView *_vectorView;
	
	CLAM::VM::PoolFloatArrayDataSource *_dataSource;
	CLAM::TData _length;
};

#endif
